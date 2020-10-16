///
/// This pass inlines every function call on device code.
/// That is every call to __device__ functions performed
/// by kernels or other __device__ functions is inlined.
///
/// The intention is to inline only user provided device
/// functions so the following are excluded:
///   - malloc-family calls in device code
///   - Cuda API calls (math, atomic, warp-primitive, etc..)
///
/// Inlining happens in two steps:
///   1. Functions are marked with AlwaysInline attribute
///   2. AllwaysInliner is invoked to perform the inlining

#include "kerma/Transforms/Canonicalize/DeviceFunctionInliner.h"

#include "kerma/NVVM/NVVMUtilities.h"

#include <llvm/Analysis/AssumptionCache.h>
#include <llvm/Analysis/InlineCost.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/OptimizationRemarkEmitter.h>
#include <llvm/IR/CallSite.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/PassAnalysisSupport.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>

using namespace kerma;
using namespace llvm;

char DeviceFunctionInliner::ID = 114;

DeviceFunctionInliner::DeviceFunctionInliner() : ModulePass(ID) {}

bool DeviceFunctionInliner::doInline(Module& M) {
  InlineFunctionInfo IFI;

  SmallVector<CallBase *, 16> Calls;
  SmallVector<Function *, 16> InlinedFunctions;
  bool Changed = false;

  for ( auto& F : M) {
    if ( !F.isDeclaration() && !F.isIntrinsic() && F.hasFnAttribute(Attribute::AlwaysInline) && isInlineViable(F)) {
      Calls.clear();

      for (User *U : F.users())
        if (auto* CB = dyn_cast<CallBase>(U))
          if (CB->getCalledFunction() == &F)
            Calls.push_back(CB);

      for ( CallBase *CB : Calls) {
        CallsChecked++;
        auto *Prev  = CB->getPrevNonDebugInstruction();
        auto *Next  = CB->getNextNonDebugInstruction();
        auto *Caller= CB->getCaller();
        if ( InlineFunction(CB, IFI, nullptr, false) ) {
          CallsInlined++;
          Info.push_back( InlineInfo(CB->getCalledFunction(), Caller, Prev, Next));
          Changed = true;
        }
      }
    }

    erase_if(InlinedFunctions, [&](Function *F) {
      F->removeDeadConstantUsers();
      return !F->isDefTriviallyDead();
    });

    // Delete the non-comdat ones from the module and also from our vector.
    auto NonComdatBegin = partition(InlinedFunctions,
                                    [&](Function *F) {
                                      return F->hasComdat();
                                    });

    for (auto* F : make_range(NonComdatBegin, InlinedFunctions.end()))
      M.getFunctionList().erase(F);

    InlinedFunctions.erase(NonComdatBegin, InlinedFunctions.end());

    if (!InlinedFunctions.empty()) {
      filterDeadComdatFunctions(M, InlinedFunctions);
      // The remaining functions are actually dead.
      for (auto* F : InlinedFunctions)
        M.getFunctionList().erase(F);
    }
  }

  return Changed;
}

bool DeviceFunctionInliner::runOnModule(Module& M) {
  if ( !nvvm::isDeviceModule(M))
    return false;

  CallsChecked = 0;
  CallsInlined = 0;
  Info.clear();

  // step 1: Mark for inline
  for ( auto& F : M) {
    if ( F.isDeclaration() || F.isIntrinsic()
                           || nvvm::isKernelFunction(F)
                           || nvvm::isCudaAPIFunction(F)
                           || nvvm::isNVVMAtomic(F)
                           || nvvm::isNVVMIntrinsic(F) )
      continue;

    F.removeFnAttr(Attribute::AttrKind::OptimizeNone);
    F.removeFnAttr(Attribute::AttrKind::NoInline);
    F.addFnAttr(Attribute::AttrKind::AlwaysInline);
  }

  // step 2: perform the inlining
  doInline(M);

#ifdef KERMA_OPT_PLUGIN
  errs() << '[' << formatv("{0,15}", "DeviceInliner") << "] " << "Inlined " << getNumCallsInlined() << " calls\n";
#endif

  return false;
}

