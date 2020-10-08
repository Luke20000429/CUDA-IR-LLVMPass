/// If this global is present and has this value, then the
/// module is considered to be linked with the RT library.
__device__ __constant__ unsigned int __kerma_rt_linked__ = 0xFEEDC0DE;

/// Ultimately we want to minimize the complexity of each stub as
/// much as possible. For instance to remove brances and let the
/// insrumentation pass decide which one to invoke. This has the
/// advantage of improving runtime performance by peforming the
/// check at compile time, but increases code size.
/// This is an acceptable tradeoff for now!

#define __kerma_mem_access_type__ unsigned char
#define __kerma_mem_access_load__ 0
#define __kerma_mem_access_store__ 1
#define __kerma_mem_access_atomic__ 2

extern "C" __device__ void __kerma_rec_kernel(unsigned char id,
                                             const char *name) {
  if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 &&
      threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
    printf("%%K.%d.%s\n", id, name);
  }
}

extern "C" __device__ void __kerma_rec_base(unsigned char kernelid,
                                      const char *symbol,
                                      unsigned char addrspace,
                                      unsigned long baseaddr) {
  unsigned int linearBlockIdx =
      blockIdx.z * gridDim.x * gridDim.y + blockIdx.y * gridDim.x + blockIdx.x;
  if (linearBlockIdx == 0 && threadIdx.x == 0 && threadIdx.y == 0 &&
      threadIdx.z == 0) {
    printf("%%B.%u.<%d>%s@%lu\n", kernelid, addrspace, symbol, baseaddr);
  }
}

/// Record all threads in a block
extern "C" __device__ void __kerma_rec_access_b(__kerma_mem_access_type__ ty,
                                          unsigned int bid, unsigned int line,
                                          unsigned int col, const char *symbol,
                                          unsigned long loc) {
  unsigned int linearBlockIdx =
      blockIdx.z * gridDim.x * gridDim.y + blockIdx.y * gridDim.x + blockIdx.x;

  if (linearBlockIdx == bid) {
    unsigned int linearLocalThreadIdx = threadIdx.z * blockDim.x * blockDim.y +
                                        threadIdx.y * blockDim.x + threadIdx.x;

    char c = ty == __kerma_mem_access_load__
                 ? 'L'
                 : (ty == __kerma_mem_access_store__ ? 'S' : 'A');
    printf("!%c(%u,%u,%u:%u):%s[%lu]\n", c, bid, linearLocalThreadIdx, line,
           col, symbol, loc);
  }
}

/// Record threads in warp 0 of a block
extern "C" __device__ void __kerma_rec_access_w(__kerma_mem_access_type__ ty,
                                          unsigned int bid, unsigned int line,
                                          unsigned int col, const char *symbol,
                                          unsigned long loc) {
  unsigned int linearBlockIdx =
      blockIdx.z * gridDim.x * gridDim.y + blockIdx.y * gridDim.x + blockIdx.x;

  if (linearBlockIdx == bid) {
    unsigned int linearLocalThreadIdx = threadIdx.z * blockDim.x * blockDim.y +
                                        threadIdx.y * blockDim.x + threadIdx.x;
    if (linearLocalThreadIdx < 32) {
      char c = ty == __kerma_mem_access_load__
                   ? 'L'
                   : (ty == __kerma_mem_access_store__ ? 'S' : 'A');
      printf("!%c(%u,%u,%u:%u):%s[%lu]\n", c, bid, linearLocalThreadIdx, line,
             col, symbol, loc);
    }
  }
}

/// Record a specific thread in a block
extern "C" __device__ void __kerma_rec_access_b_t(__kerma_mem_access_type__ ty,
                                            unsigned int bid, unsigned int tid,
                                            unsigned int line, unsigned int col,
                                            const char *symbol,
                                            unsigned long loc) {
  unsigned int linearBlockIdx =
      blockIdx.z * gridDim.x * gridDim.y + blockIdx.y * gridDim.x + blockIdx.x;

  if (linearBlockIdx == bid) {
    unsigned int linearLocalThreadIdx = threadIdx.z * blockDim.x * blockDim.y +
                                        threadIdx.y * blockDim.x + threadIdx.x;

    if (linearLocalThreadIdx == tid) {
      char c = ty == __kerma_mem_access_load__
                   ? 'L'
                   : (ty == __kerma_mem_access_store__ ? 'S' : 'A');
      printf("!%c(%u,%u,%u:%u):%s[%lu]\n", c, bid, tid, line, col, symbol, loc);
    }
  }
}
