#ifndef KERMA_PASS_DETECT_KERNELS_H
#define KERMA_PASS_DETECT_KERNELS_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <vector>

namespace kerma {

class DetectKernelsPass : public llvm::ModulePass {
public:
  static char ID;
  DetectKernelsPass();

public:
  bool runOnModule(llvm::Module &M) override;
  virtual void print(llvm::raw_ostream &O, const llvm::Module *M) const override;
  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

public:
  /// Get a vector containing all the kernel functions
  /// found in the module. The returned vector is a copy
  /// the internal vector of the pass and can be freely
  /// manipulated
  std::vector<llvm::Function*> getKernels();

  /// Get the kernel function found in the use-provided
  /// container.
  void getKernels(std::vector<llvm::Function*> &Kernels);

  /// Check if a function is a kernel
  bool isKernel(llvm::Function *F);

private:
  std::vector<llvm::Function*> Kernels;
};

std::unique_ptr<DetectKernelsPass> createDetectKernelsPass();


/// Check if a function is a CUDA kernel
bool isKerneFunction(const llvm::Function& F);

} // end namespace kerma

#endif