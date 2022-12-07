SM   = sm_61
CP   = compute_61
SRC  = input.cu
EXE  = input
DEBUG = -g 
OPTS = -O2 -std=c++11
CFLAGS =
LLVM_HOME = /home/liuxs/workspace/kerma/llvm-install
CLANG_HOME = /usr/local
CUDA_HOME = /usr/local/cuda-11.8/
CLANG = ${CLANG_HOME}/bin/clang++
CLANG_LIB = ${CLANG_HOME}/lib/clang/14.0.6
LLC = ${CLANG_HOME}/bin/llc
CUDA_LIB = /usr/local/cuda/lib64
PTXAS = ptxas
FATBIN = fatbinary

PASS_LIB=

clean :
	rm -f *.cubin *.ll *.o *.ptx *.s *.bc *.fatbin *.cui

device.bc :
	$(info ## Generating device.bc )
	$(CLANG) $(DEBUG) $(OPTS) $(CFLAGS) -x cuda -c --cuda-device-only -emit-llvm $(SRC) -o device.bc --cuda-path=${CUDA_HOME} --cuda-gpu-arch=${SM}

device.ptx : device.bc
	$(info ## Generating device.ptx )
	$(LLC) device.bc -filetype=asm -o device.ptx

device.o : device.ptx
	$(info ## Generating device.o )
	$(PTXAS) --gpu-name $(SM) device.ptx -o device.o -v

device.fatbin : device.o
	$(info ## Generating device.fatbin )
	$(FATBIN) -64 --create device.fatbin --image=profile=$(SM),file=device.o --image=profile=$(CP),file=device.ptx  

host.cui : $(SRC)
	$(info ## Generating host.cui )
	$(CLANG) $(DEBUG) $(OPTS) $(CFLAGS) -E --cuda-host-only -g $(SRC) -o host.cui

host.bc : device.fatbin host.cui
	$(info ## Generating host.bc )
	$(CLANG) \
		-cc1 -v -triple x86_64-unknown-linux-gnu -aux-triple nvptx64-nvidia-cuda -main-file-name $(SRC) $(OPTS) -o host.bc \
		-x cuda-cpp-output host.cui -fcuda-include-gpubinary device.fatbin -emit-llvm-bc -emit-llvm-uselists\
		-target-sdk-version=10.1 -disable-free -resource-dir $(CLANG_LIB) \
		-mrelocation-model static -mthread-model posix -fmath-errno -mframe-pointer=all -mconstructor-aliases \
		-target-cpu x86-64 -v -debug-info-kind=limited -dwarf-version=4 \
		-debugger-tuning=gdb -fdeprecated-macro -ferror-limit 19 -fcxx-exceptions -fexceptions \
		-pthread -fobjc-runtime=gcc
	 
	# -fdebug-compilation-dir /home/shen956/llvm/llvm/lib/Transforms/Hello/compile
host.o : host.bc
	$(info ## Generating host.o )
	$(CLANG) host.bc -c -o host.o

all : host.o
	$(CLANG) $(DEBUG) host.o -o $(EXE) -L${CUDA_LIB} -lcudart_static -ldl -lrt -lm -pthread $(PASS_LIB)