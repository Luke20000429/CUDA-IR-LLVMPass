# Very Simple Instruction

## To download llvm-10 (work on Ubuntu 18.04-22.04)
```
mkdir llvm-install
cd llvm-install

wget -c https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz

tar xf clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz -C . --strip-components=1
rm *.tar.xz
cd ..
```

## To compile
```
mkdir build
cd build
cmake ..
make -j
```