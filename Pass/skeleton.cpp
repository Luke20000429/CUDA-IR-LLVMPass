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
#include "llvm/ADT/DenseMap.h"

using namespace llvm;
using namespace std;

namespace {
  
  struct SkeletonPass : public FunctionPass {
    static char ID;
    unsigned var_cnt = 0;
    DenseMap<Value *, Value*> uvm;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      auto func_name = F.getName();
      errs() << "I saw a function called " << func_name << "!\n";
      Module* Mod = F.getParent();
      auto &ctx = F.getContext();
      
      for (auto &bb : F) {
        for (auto &i : bb) {
          if(CallInst* ci = dyn_cast<CallInst>(&i)) {
            auto call_name = ci->getCalledFunction()->getName();

            if (call_name.find("_ZL17cudaMallocManaged") != std::string::npos) {
              errs() << "Call malloc function: " << call_name << "\n";
              auto addr = ci->getArgOperand(0);
              auto size = ci->getArgOperand(1);
              errs() << "Address name: " << *addr << " size: " << *size << "\n"; 
              uvm[addr] = size;
            }
            if (call_name.find("__device_stub__") != std::string::npos) {
              errs() << "Call kernel: " << call_name << "\n";
              for (auto &arg : ci->args()) {
                errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
                if (arg->getType()->isPointerTy()) {
                  if (LoadInst *li = dyn_cast<LoadInst>(arg)) {
                    auto addr = li->getOperand(0);
                    auto it = uvm.find(addr);
                    if (it != uvm.end()) {
                      errs() << "uvm addr: " << *addr << ", size " << uvm[addr]->getNameOrAsOperand() << "\n";
                      IRBuilder<> builder(ci); // insert instructions ahead of it
                      insertPrefetchFunc(builder, ctx, Mod, addr, uvm[addr]);
                    } else {
                      errs() << "addr: " << *addr << " not a unified memory" << "\n";
                    }
                  }
                }
              }
            }
          }
          if(InvokeInst* ci = dyn_cast<InvokeInst>(&i)) {
            auto call_name = ci->getCalledFunction()->getName();

            if (call_name.find("_ZL17cudaMallocManaged") != std::string::npos) {
              errs() << "Call malloc function: " << call_name << "\n";
              auto addr = ci->getArgOperand(0);
              auto size = ci->getArgOperand(1);
              errs() << "Address name: " << *addr << " size: " << *size << "\n"; 
              uvm[addr] = size;
            }
            if (call_name.find("__device_stub__") != std::string::npos) {
              errs() << "Call kernel: " << call_name << "\n";
              for (auto &arg : ci->args()) {
                errs() << arg->getNameOrAsOperand() << " " << *arg->getType() << "\n";
                if (arg->getType()->isPointerTy()) {
                  if (LoadInst *li = dyn_cast<LoadInst>(arg)) {
                    auto addr = li->getOperand(0);
                    auto it = uvm.find(addr);
                    if (it != uvm.end()) {
                      errs() << "uvm addr: " << *addr << ", size " << uvm[addr]->getNameOrAsOperand() << "\n";
                      IRBuilder<> builder(ci); // insert instructions ahead of it
                      insertPrefetchFunc(builder, ctx, Mod, addr, uvm[addr]);
                    } else {
                      errs() << "addr: " << *addr << " not a unified memory" << "\n";
                    }
                  }
                }
              }
            }
          }
        }
      }

      return false;
    }

    int insertPrefetchFunc(IRBuilder<> &builder, LLVMContext& ctx, Module* Mod, Value *addr, Value *size) {
      // by pp is using pointer to pointer to type, like float**
      FunctionCallee prefetch = declarePrefetchFunc(ctx, Mod);
      Value* arr = builder.CreateLoad(Type::getFloatPtrTy(ctx), addr, "uvm_" + std::to_string(var_cnt));
      Value* vptr = builder.CreateBitCast(arr, Type::getInt8PtrTy(ctx), "cast_" + std::to_string(var_cnt));
      Value* device = builder.getInt32(0); // use device 0 by default
      Value *null_pointer = ConstantPointerNull::get(PointerType::get(StructType::getTypeByName(ctx, "struct.CUstream_st"), 0));
      Value* args[] = {vptr, size, device, null_pointer};
      builder.CreateCall(prefetch, args);
      var_cnt++;
      errs() << "Insert prefetch\n";
      return 0;
    }

    FunctionCallee declarePrefetchFunc(LLVMContext& ctx,
                                      Module* module){
      if (StructType *st = StructType::getTypeByName(ctx, "struct.CUstream_st")) {
        Type* returnType = Type::getInt32Ty(ctx);
        vector<Type*> fArgs { Type::getInt8PtrTy(ctx),
                              Type::getInt64Ty(ctx), 
                              Type::getInt32Ty(ctx),
                              st->getPointerTo()
            };
        FunctionType* prefetch = FunctionType::get(returnType, fArgs, false);
        // declare dso_local i32 @cudaMemPrefetchAsync(i8* noundef, i64 noundef, i32 noundef, %struct.CUstream_st* noundef) #1
        // pass (addr, size, 0, null), where addr require bitcast to i8* first
        return module->getOrInsertFunction(StringRef("cudaMemPrefetchAsync"), prefetch);
      }
      errs() << "CUstream_st undefined, failed to declare prefetch in this module\n";
      return FunctionCallee(); // how to deal with this ?
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
