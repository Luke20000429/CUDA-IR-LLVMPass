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

#include <vector>

using namespace llvm;
using namespace std;

namespace {
  
  struct SkeletonPass : public FunctionPass {
    static char ID;
    Type *st;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      auto func_name = F.getName();
      errs() << "I saw a function called " << func_name << "!\n";
      Module* Mod = F.getParent();
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
      auto &ctx = F.getContext();
      // declare dso_local i32 @cudaMemPrefetchAsync(i8* noundef, i64 noundef, i32 noundef, %struct.CUstream_st* noundef) #1
      // StructType *st = StructType::getTypeByName(ctx, "CUstream_st");
      // if (st != nullptr) {
      //   errs() << "find type" << *st << *st->getPointerTo() << "\n";
      // }
      for (auto &bb : F) {
        for (auto &i : bb) {
          if(CallInst* ci = dyn_cast<CallInst>(&i)) {
            auto call_name = ci->getCalledFunction()->getName();
            // if (call_name.find("_ZL17cudaMallocManaged") != std::string::npos) {
            //   errs() << "Call malloc function: " << call_name << "\n";
            //   for (auto &arg : ci->args()) {
            //     errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
            //     for (auto user : arg->users()) {
            //       Instruction* use_inst;
            //       if ((use_inst = dyn_cast<Instruction>(user)) && use_inst->mayReadFromMemory()) {
            //         errs() << "User: " << *user << "\n";
            //       }
            //     }
            //   }
            // }
            // if (call_name.find("__device_stub__") != std::string::npos) {
            //   errs() << "Call kernel: " << call_name << "\n";
            //   for (auto &arg : ci->args()) {
            //     errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
            //     for (auto user : arg->users()) {
            //       Instruction* use_inst;
            //       if ((use_inst = dyn_cast<Instruction>(user)) && use_inst->mayReadFromMemory()) {
            //         errs() << "User: " << *user << "\n";
            //       }
            //     }
            //   }
            // }
            if (call_name.find("cudaMemPrefetchAsync") != std::string::npos) {
              errs() << "Call prefetch: " << call_name << "\n";
              // for (auto &arg : ci->args()) {
              //   errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << " " <<  << "\n";

              //   // for (auto user : arg->users()) {
              //   //   Instruction* use_inst;
              //   //   if ((use_inst = dyn_cast<Instruction>(user)) && use_inst->mayReadFromMemory()) {
              //   //     errs() << "User: " << *user << "\n";
              //   //   }
              //   // }
              // }
              st = ci->getArgOperand(3)->getType();
              errs() << *st << " name: " << st->getNonOpaquePointerElementType()->getStructName() <<  "\n";
              
            }
            FunctionCallee ff = insertPrefetchFunc(ctx, Mod);
          }
        }
      }

      return false;
    }

    FunctionCallee insertPrefetchFunc(LLVMContext& ctx,
                                      Module* module){
      if (StructType *st = StructType::getTypeByName(ctx, "struct.CUstream_st")) {
        Type* returnType = Type::getInt32Ty(ctx);
        vector<Type*> fArgs { Type::getInt8PtrTy(ctx),
                              Type::getInt64Ty(ctx), 
                              Type::getInt32Ty(ctx),
                              st->getPointerTo()
            };
        FunctionType* prefetch = FunctionType::get(returnType, fArgs, false);
        errs() << "Insert prefetch\n";
        return module->getOrInsertFunction(StringRef("cudaMemPrefetchAsync"), prefetch);
      }
      return FunctionCallee();
    }

  };
  

  // struct PrefetchPass : public ModulePass {
  //   static char ID;
  //   PrefetchPass() : ModulePass(ID) {}

  //   virtual bool runOnModule(Module &Mod) {
  //     errs() << "I saw a module !\n";
  //     auto &ctx = Mod.getContext();
  //     // declare dso_local i32 @cudaMemPrefetchAsync(i8* noundef, i64 noundef, i32 noundef, %struct.CUstream_st* noundef) #1
  //     if (st != nullptr) {
  //       errs() << *st << "\n";
  //       auto prefetch = Mod.getOrInsertFunction("cudaMemPrefetchAsync", Type::getInt32Ty(ctx), 
  //                                                                     Type::getInt8PtrTy(ctx), 
  //                                                                     Type::getInt64Ty(ctx), 
  //                                                                     Type::getInt32Ty(ctx),
  //                                                                     st);
  //     }
  //     return false;
  //   }
  // };
}

char SkeletonPass::ID = 0;
// char PrefetchPass::ID = 1;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
  // PM.add(new PrefetchPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
