#ifndef KERMA_ANALYSIS_TYPECHECK_H
#define KERMA_ANALYSIS_TYPECHECK_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Pass.h>

namespace kerma {

/// Check if a type is a pointer to a struct
bool isPtrToStruct(llvm::Type *Ty);

/// Check if a struct only contains scalars
bool isStructOfScalars(llvm::StructType *Ty);

/// Ignore pointers in the type. Return the
/// the first non-pointer type encountered
llvm::Type* stripPointers(llvm::Type *Ty);
llvm::Type* stripArrayNest(llvm::Type *Ty);


/// Check if the struct is "simple"
/// i.e has only scalar fields
bool isSimpleStruct(llvm::StructType *Ty);

/// Get the maximum number of indices needed to
/// index into this type. Example:
///   i32*** -> 3
///   i32*   -> 1
///   [2 x [32 x i32]]*  -> 3
///   i32 -> 0
unsigned int getMaxIndicesForType(llvm::Type *Ty);

namespace TypeCheck {
  const unsigned int MaxSupportedIndices = 2;
};

enum StructErr {
  ER_None=0,
  ER_NestedStructNotSimple,
  ER_StructFieldHasUnsupportedDims,
  ER_StructFieldIsPtrToUnsupportedStruct,
  ER_StructFieldIsArrOfUnsupportedStructs
};

class TypeCheckError {
private:
  unsigned int Line=0;
  unsigned int Col=0;
  std::string Msg;
public:
  TypeCheckError(const std::string& Msg, unsigned int Line=0, unsigned int Col=0);
  const std::string getMsg();
  const std::string getMsgWithSourceLoc();

  static TypeCheckError getUnsupportedDimensions(llvm::GlobalVariable& GV, unsigned int Dims);
  static TypeCheckError getUnsupportedDimensions(llvm::Argument& Arg, unsigned int Dims);
  static TypeCheckError getUnsupportedDimensions(llvm::AllocaInst& AI, unsigned int Dims);
  static TypeCheckError getUnsupportedDimensions(llvm::CallInst& AI, unsigned int Dims);

  static TypeCheckError getUnsupportedStruct(llvm::GlobalVariable& GV, StructErr Err);
  static TypeCheckError getUnsupportedStruct(llvm::Argument& Arg, StructErr Err);
  static TypeCheckError getUnsupportedStruct(llvm::AllocaInst& I, StructErr Err);
  static TypeCheckError getUnsupportedStruct(llvm::CallInst& AI, StructErr Err);

  // static TypeCheckError getUnsupportedStruct(llvm::Argument& Arg, StructErr Err);
};

/// This pass checks if kernel memory has a type
/// we support. In particular it checks globals,
/// allocas, and kernel arguments.
class TypeCheckerPass : public llvm::ModulePass {
private:
  llvm::SmallVector<TypeCheckError, 32> Errors;

private:
  StructErr typecheckStruct(llvm::StructType *Ty);

  void typecheckArrayOrPtr(llvm::Type *Ty, llvm::AllocaInst *AI);

  void typecheckStruct(llvm::StructType *Ty, llvm::AllocaInst *AI);

  void typecheck(llvm::Type *Ty, llvm::Argument *Arg);
  void typecheck(llvm::Type *Ty, llvm::CallInst *MallocCall);

  void typecheckKernelArgs(llvm::Function& Kernel);
  void typecheckKernelLocals(llvm::Function& Kernel);
  void typecheckKernels(llvm::Module& M);
  void typecheckGlobals(llvm::Module& M);

public:
  static char ID;
  TypeCheckerPass();
  virtual bool runOnModule(llvm::Module& M) override;
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  void dumpErrors();
  bool moduleTypechecks();
  llvm::SmallVector<TypeCheckError, 32> getErrors();

};

} // namespace kerma

#endif // KERMA_ANALYSIS_TYPECHECK_H