$LLVM_BIN/clang++ -x cuda -Xclang -disable-O0-optnone -S -emit-llvm -g3 -gcolumn-info $1 -O0 --cuda-gpu-arch=sm_52 --cuda-path=$CUDA_HOME