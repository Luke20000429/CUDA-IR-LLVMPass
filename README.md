# Build Instructions 

## To build original CUDA code
```
nvcc axpy.cu -o axpy
```
or
```
clang++ axpy.cu -o axpy --cuda-gpu-arch=sm_61 -L/usr/local/cuda/lib64 -lcudart_static -ldl -lrt -pthread
```

## To see IR of original CUDA code
```
clang++ -std=c++11 -emit-llvm -c -S axpy.cu --cuda-gpu-arch=sm_61
```

## To build executable file with LLVM pass
```
clang++ -flegacy-pass-manager  -Xclang -load -Xclang build/Pass/libSkeletonPass.* axpy.cu -o axpy --cuda-gpu-arch=sm_61 -L/usr/local/cuda-11.8/lib64/ -lcudart_static -ldl -lrt -pthread
```

## To see IR after LLVM pass
```
clang++ -flegacy-pass-manager -Xclang -load -Xclang build/Pass/libSkeletonPass.* -std=c++11 -emit-llvm -c -S axpy.cu --cuda-gpu-arch=sm_61
```



