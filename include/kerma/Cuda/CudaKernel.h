/*
 * @file: include/kerma/Support/Cuda.h
 *
 * Includes various wrappers for CUDA related things
 */
#ifndef KERMA_SUPPORT_CUDA_H
#define KERMA_SUPPORT_CUDA_H

#include "kerma/cuda/CudaSupport.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Function.h"

namespace kerma
{

/// Represents a 'side' for CUDA. That is, either HOST or DEVICE
/// Usually used to mark stuff like depending on which IR file (host, device) was used
enum CudaSide {
  HOST,
  DEVICE,
  UKNOWN
};

class CudaKernel;
class CudaKernelLaunch;
class CudaKernelLaunchConfiguration;

class CudaKernel {
  
public:
  CudaKernel(llvm::Function *fn, CudaSide irModuleSide);
  CudaKernel(llvm::Function *fn);
  ~CudaKernel()=default;

public:
  /*
   * Get a pointer to the llvm::Function for this kernel
   */
  llvm::Function *getFn();

  /*
   * Retrieve the side of the LLVM IR file this kernel was detected at
   */
  CudaSide getIrModuleSide();

  /*
   * Retrieve the number of arguments of this kernel
   */
  int getNumArgs();

  /*
   * Retrieve the i-th argument of this kernel
   */
  llvm::Argument *getArg(int i);

  /*
   * Retrieve the first (source code) line of the kernel's definition
   */
  int getLineStart();

  /*
   * Retrieve the last (source code) line of the kernel's definition
   */
  int getLineEnd();

  /*
   * Retrieve the number of source code this kernel function spans
   */
  int getNumLines();
  
};

class CudaKernelLaunchConfiguration
{
  CudaKernelLaunchConfiguration();
  CudaKernelLaunchConfiguration(llvm::Value &grid,
                                llvm::Value &block);
  CudaKernelLaunchConfiguration(llvm::Value &grid,
                                llvm::Value &block,
                                llvm::Value &shMem);
  ~CudaKernelLaunchConfiguration() = default;

  llvm::Value *getGrid();
  llvm::Value *getBlock();
  llvm::Value *getSharedMemory();

  llvm::Value *getX(llvm::Value *dim3Value);
  llvm::Value *getY(llvm::Value *dim3Value);
  llvm::Value *getZ(llvm::Value *dim3Value);

};

class CudaKernelLaunch
{ 
  CudaKernelLaunch(CudaKernel &kernel, int line = -1);
  CudaKernelLaunch(CudaKernel &kernel, CudaKernelLaunchConfiguration &config, int line = -1);
  ~CudaKernelLaunch() = default;

  CudaKernelLaunchConfiguration *getLaunchConfigutation();

  void setLaunchConfiguration(CudaKernelLaunchConfiguration *config);

  void setLine(int line);

  int getLine();

  bool inLoop();

  bool inBranch();
};

} /* NAMESPACE kerma */

#endif /* KERMA_SUPPORT_CUDA_H */