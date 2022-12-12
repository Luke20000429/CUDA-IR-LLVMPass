#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/Format.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>


#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      auto func_name = F.getName();
      errs() << "I saw a function called " << func_name << "!\n";
      // if (func_name.find("cudaMallocManaged") != std::string::npos) {
      //   auto addr = F.getArg(0);
      //   errs() << "Alloc unified memory " << addr->getValueName()->getValue() << "!\n";
      //   Value* val = cast<Value>(addr);
      //   Value::use_iterator sUse = val->use_begin();
      //   Value::use_iterator sEnd = val->use_end();
      //   for (; sUse != sEnd; ++sUse) {
      //       if(StoreInst* si = dyn_cast<StoreInst>(&*sUse)) {
      //         DebugLoc loc = si->getDebugLoc();
      //         StringRef filename = loc->getFilename();
      //         int lineNum = loc->getLine();
      //         int columnNum = loc->getColumn();
      //         errs() << "Find usage of addr in " << filename << ";" << lineNum << "\n";
      //       }
      //     }
      // }
      for (auto &bb : F) {
        for (auto &i : bb) {
          if(CallInst* ci = dyn_cast<CallInst>(&i)) {
            auto call_name = ci->getCalledFunction()->getName();
            if (call_name.find("_ZL17cudaMallocManaged") != std::string::npos) {
              errs() << "Call malloc function: " << call_name << "\n";
              for (auto &arg : ci->args()) {
                errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
                for (auto user : arg->users()) {
                  Instruction* use_inst;
                  if ((use_inst = dyn_cast<Instruction>(user)) && use_inst->mayReadFromMemory()) {
                    errs() << "User: " << *user << "\n";
                  }
                }
              }
            }
            if (call_name.find("__device_stub__") != std::string::npos) {
              errs() << "Call kernel: " << call_name << "\n";
              for (auto &arg : ci->args()) {
                errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
                for (auto user : arg->users()) {
                  Instruction* use_inst;
                  if ((use_inst = dyn_cast<Instruction>(user)) && use_inst->mayReadFromMemory()) {
                    errs() << "User: " << *user << "\n";
                  }
                }
              }
            }
          }
        }
      }

      return false;
    }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
