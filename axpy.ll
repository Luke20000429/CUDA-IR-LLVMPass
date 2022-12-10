; ModuleID = 'axpy.cu'
source_filename = "axpy.cu"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type <{ i32 (...)**, i32, [4 x i8] }>
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type <{ %"class.std::locale::facet.base", [4 x i8], %struct.__locale_struct*, i8, [7 x i8], i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8, [6 x i8] }>
%"class.std::locale::facet.base" = type <{ i32 (...)**, i32 }>
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet.base", [4 x i8] }
%"class.std::num_get" = type { %"class.std::locale::facet.base", [4 x i8] }
%struct.dim3 = type { i32, i32, i32 }
%struct.CUstream_st = type opaque

$_ZN4dim3C2Ejjj = comdat any

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@__const.main.host_x = private unnamed_addr constant [4 x float] [float 1.000000e+00, float 2.000000e+00, float 3.000000e+00, float 4.000000e+00], align 16
@.str = private unnamed_addr constant [25 x i8] c"CUDA error at %s %d: %s\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"axpy.cu\00", align 1
@_ZSt4cout = external dso_local global %"class.std::basic_ostream", align 8
@.str.2 = private unnamed_addr constant [3 x i8] c"y[\00", align 1
@.str.3 = private unnamed_addr constant [5 x i8] c"] = \00", align 1
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_axpy.cu, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1) @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #3
  ret void
}

declare dso_local void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #1

; Function Attrs: nounwind
declare dso_local void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #2

; Function Attrs: nounwind
declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #3

; Function Attrs: noinline norecurse optnone uwtable
define dso_local void @_Z19__device_stub__axpyfPfS_(float noundef %a, float* noundef %x, float* noundef %y) #4 {
entry:
  %a.addr = alloca float, align 4
  %x.addr = alloca float*, align 8
  %y.addr = alloca float*, align 8
  %grid_dim = alloca %struct.dim3, align 8
  %block_dim = alloca %struct.dim3, align 8
  %shmem_size = alloca i64, align 8
  %stream = alloca i8*, align 8
  %grid_dim.coerce = alloca { i64, i32 }, align 8
  %block_dim.coerce = alloca { i64, i32 }, align 8
  store float %a, float* %a.addr, align 4
  store float* %x, float** %x.addr, align 8
  store float* %y, float** %y.addr, align 8
  %kernel_args = alloca i8*, i64 3, align 16
  %0 = bitcast float* %a.addr to i8*
  %1 = getelementptr i8*, i8** %kernel_args, i32 0
  store i8* %0, i8** %1, align 8
  %2 = bitcast float** %x.addr to i8*
  %3 = getelementptr i8*, i8** %kernel_args, i32 1
  store i8* %2, i8** %3, align 8
  %4 = bitcast float** %y.addr to i8*
  %5 = getelementptr i8*, i8** %kernel_args, i32 2
  store i8* %4, i8** %5, align 8
  %6 = call i32 @__cudaPopCallConfiguration(%struct.dim3* %grid_dim, %struct.dim3* %block_dim, i64* %shmem_size, i8** %stream)
  %7 = load i64, i64* %shmem_size, align 8
  %8 = load i8*, i8** %stream, align 8
  %9 = bitcast { i64, i32 }* %grid_dim.coerce to i8*
  %10 = bitcast %struct.dim3* %grid_dim to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %9, i8* align 8 %10, i64 12, i1 false)
  %11 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %grid_dim.coerce, i32 0, i32 0
  %12 = load i64, i64* %11, align 8
  %13 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %grid_dim.coerce, i32 0, i32 1
  %14 = load i32, i32* %13, align 8
  %15 = bitcast { i64, i32 }* %block_dim.coerce to i8*
  %16 = bitcast %struct.dim3* %block_dim to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %15, i8* align 8 %16, i64 12, i1 false)
  %17 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %block_dim.coerce, i32 0, i32 0
  %18 = load i64, i64* %17, align 8
  %19 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %block_dim.coerce, i32 0, i32 1
  %20 = load i32, i32* %19, align 8
  %21 = bitcast i8* %8 to %struct.CUstream_st*
  %call = call noundef i32 @cudaLaunchKernel(i8* noundef bitcast (void (float, float*, float*)* @_Z19__device_stub__axpyfPfS_ to i8*), i64 %12, i32 %14, i64 %18, i32 %20, i8** noundef %kernel_args, i64 noundef %7, %struct.CUstream_st* noundef %21)
  br label %setup.end

setup.end:                                        ; preds = %entry
  ret void
}

declare dso_local i32 @__cudaPopCallConfiguration(%struct.dim3*, %struct.dim3*, i64*, i8**)

declare dso_local i32 @cudaLaunchKernel(i8*, i64, i32, i64, i32, i8**, i64, %struct.CUstream_st*)

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #5

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main(i32 noundef %argc, i8** noundef %argv) #6 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %kDataLen = alloca i32, align 4
  %a = alloca float, align 4
  %host_x = alloca [4 x float], align 16
  %host_y = alloca [4 x float], align 16
  %device_x = alloca float*, align 8
  %device_y = alloca float*, align 8
  %err = alloca i32, align 4
  %err4 = alloca i32, align 4
  %err13 = alloca i32, align 4
  %agg.tmp = alloca %struct.dim3, align 4
  %agg.tmp21 = alloca %struct.dim3, align 4
  %agg.tmp.coerce = alloca { i64, i32 }, align 4
  %agg.tmp21.coerce = alloca { i64, i32 }, align 4
  %err24 = alloca i32, align 4
  %err33 = alloca i32, align 4
  %i = alloca i32, align 4
  %err49 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 4, i32* %kDataLen, align 4
  store float 2.000000e+00, float* %a, align 4
  %0 = bitcast [4 x float]* %host_x to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 16 %0, i8* align 16 bitcast ([4 x float]* @__const.main.host_x to i8*), i64 16, i1 false)
  br label %do.body

do.body:                                          ; preds = %entry
  %call = call noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(float** noundef %device_x, i64 noundef 16)
  store i32 %call, i32* %err, align 4
  %1 = load i32, i32* %err, align 4
  %cmp = icmp ne i32 %1, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %do.body
  %2 = load i32, i32* %err, align 4
  %call1 = call i8* @cudaGetErrorString(i32 noundef %2)
  %call2 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 30, i8* noundef %call1)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end:                                           ; preds = %do.body
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.body3

do.body3:                                         ; preds = %do.end
  %call5 = call noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(float** noundef %device_y, i64 noundef 16)
  store i32 %call5, i32* %err4, align 4
  %3 = load i32, i32* %err4, align 4
  %cmp6 = icmp ne i32 %3, 0
  br i1 %cmp6, label %if.then7, label %if.end10

if.then7:                                         ; preds = %do.body3
  %4 = load i32, i32* %err4, align 4
  %call8 = call i8* @cudaGetErrorString(i32 noundef %4)
  %call9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 31, i8* noundef %call8)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end10:                                         ; preds = %do.body3
  br label %do.end11

do.end11:                                         ; preds = %if.end10
  br label %do.body12

do.body12:                                        ; preds = %do.end11
  %5 = load float*, float** %device_x, align 8
  %6 = bitcast float* %5 to i8*
  %arraydecay = getelementptr inbounds [4 x float], [4 x float]* %host_x, i64 0, i64 0
  %7 = bitcast float* %arraydecay to i8*
  %call14 = call i32 @cudaMemcpy(i8* noundef %6, i8* noundef %7, i64 noundef 16, i32 noundef 1)
  store i32 %call14, i32* %err13, align 4
  %8 = load i32, i32* %err13, align 4
  %cmp15 = icmp ne i32 %8, 0
  br i1 %cmp15, label %if.then16, label %if.end19

if.then16:                                        ; preds = %do.body12
  %9 = load i32, i32* %err13, align 4
  %call17 = call i8* @cudaGetErrorString(i32 noundef %9)
  %call18 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 33, i8* noundef %call17)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end19:                                         ; preds = %do.body12
  br label %do.end20

do.end20:                                         ; preds = %if.end19
  call void @_ZN4dim3C2Ejjj(%struct.dim3* noundef nonnull align 4 dereferenceable(12) %agg.tmp, i32 noundef 1, i32 noundef 1, i32 noundef 1)
  call void @_ZN4dim3C2Ejjj(%struct.dim3* noundef nonnull align 4 dereferenceable(12) %agg.tmp21, i32 noundef 4, i32 noundef 1, i32 noundef 1)
  %10 = bitcast { i64, i32 }* %agg.tmp.coerce to i8*
  %11 = bitcast %struct.dim3* %agg.tmp to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 4 %10, i8* align 4 %11, i64 12, i1 false)
  %12 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %agg.tmp.coerce, i32 0, i32 0
  %13 = load i64, i64* %12, align 4
  %14 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %agg.tmp.coerce, i32 0, i32 1
  %15 = load i32, i32* %14, align 4
  %16 = bitcast { i64, i32 }* %agg.tmp21.coerce to i8*
  %17 = bitcast %struct.dim3* %agg.tmp21 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 4 %16, i8* align 4 %17, i64 12, i1 false)
  %18 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %agg.tmp21.coerce, i32 0, i32 0
  %19 = load i64, i64* %18, align 4
  %20 = getelementptr inbounds { i64, i32 }, { i64, i32 }* %agg.tmp21.coerce, i32 0, i32 1
  %21 = load i32, i32* %20, align 4
  %call22 = call i32 @__cudaPushCallConfiguration(i64 %13, i32 %15, i64 %19, i32 %21, i64 noundef 0, i8* noundef null)
  %tobool = icmp ne i32 %call22, 0
  br i1 %tobool, label %kcall.end, label %kcall.configok

kcall.configok:                                   ; preds = %do.end20
  %22 = load float, float* %a, align 4
  %23 = load float*, float** %device_x, align 8
  %24 = load float*, float** %device_y, align 8
  call void @_Z19__device_stub__axpyfPfS_(float noundef %22, float* noundef %23, float* noundef %24)
  br label %kcall.end

kcall.end:                                        ; preds = %kcall.configok, %do.end20
  br label %do.body23

do.body23:                                        ; preds = %kcall.end
  %call25 = call i32 @cudaDeviceSynchronize()
  store i32 %call25, i32* %err24, align 4
  %25 = load i32, i32* %err24, align 4
  %cmp26 = icmp ne i32 %25, 0
  br i1 %cmp26, label %if.then27, label %if.end30

if.then27:                                        ; preds = %do.body23
  %26 = load i32, i32* %err24, align 4
  %call28 = call i8* @cudaGetErrorString(i32 noundef %26)
  %call29 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 39, i8* noundef %call28)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end30:                                         ; preds = %do.body23
  br label %do.end31

do.end31:                                         ; preds = %if.end30
  br label %do.body32

do.body32:                                        ; preds = %do.end31
  %arraydecay34 = getelementptr inbounds [4 x float], [4 x float]* %host_y, i64 0, i64 0
  %27 = bitcast float* %arraydecay34 to i8*
  %28 = load float*, float** %device_y, align 8
  %29 = bitcast float* %28 to i8*
  %call35 = call i32 @cudaMemcpy(i8* noundef %27, i8* noundef %29, i64 noundef 16, i32 noundef 2)
  store i32 %call35, i32* %err33, align 4
  %30 = load i32, i32* %err33, align 4
  %cmp36 = icmp ne i32 %30, 0
  br i1 %cmp36, label %if.then37, label %if.end40

if.then37:                                        ; preds = %do.body32
  %31 = load i32, i32* %err33, align 4
  %call38 = call i8* @cudaGetErrorString(i32 noundef %31)
  %call39 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 41, i8* noundef %call38)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end40:                                         ; preds = %do.body32
  br label %do.end41

do.end41:                                         ; preds = %if.end40
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %do.end41
  %32 = load i32, i32* %i, align 4
  %cmp42 = icmp slt i32 %32, 4
  br i1 %cmp42, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call43 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0))
  %33 = load i32, i32* %i, align 4
  %call44 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) %call43, i32 noundef %33)
  %call45 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) %call44, i8* noundef getelementptr inbounds ([5 x i8], [5 x i8]* @.str.3, i64 0, i64 0))
  %34 = load i32, i32* %i, align 4
  %idxprom = sext i32 %34 to i64
  %arrayidx = getelementptr inbounds [4 x float], [4 x float]* %host_y, i64 0, i64 %idxprom
  %35 = load float, float* %arrayidx, align 4
  %call46 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEf(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) %call45, float noundef %35)
  %call47 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) %call46, i8* noundef getelementptr inbounds ([2 x i8], [2 x i8]* @.str.4, i64 0, i64 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %36 = load i32, i32* %i, align 4
  %inc = add nsw i32 %36, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !5

for.end:                                          ; preds = %for.cond
  br label %do.body48

do.body48:                                        ; preds = %for.end
  %call50 = call i32 @cudaDeviceReset()
  store i32 %call50, i32* %err49, align 4
  %37 = load i32, i32* %err49, align 4
  %cmp51 = icmp ne i32 %37, 0
  br i1 %cmp51, label %if.then52, label %if.end55

if.then52:                                        ; preds = %do.body48
  %38 = load i32, i32* %err49, align 4
  %call53 = call i8* @cudaGetErrorString(i32 noundef %38)
  %call54 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 noundef 48, i8* noundef %call53)
  call void @exit(i32 noundef 1) #10
  unreachable

if.end55:                                         ; preds = %do.body48
  br label %do.end56

do.end56:                                         ; preds = %if.end55
  ret i32 0
}

; Function Attrs: mustprogress noinline optnone uwtable
define internal noundef i32 @_ZL10cudaMallocIfE9cudaErrorPPT_m(float** noundef %devPtr, i64 noundef %size) #7 {
entry:
  %devPtr.addr = alloca float**, align 8
  %size.addr = alloca i64, align 8
  store float** %devPtr, float*** %devPtr.addr, align 8
  store i64 %size, i64* %size.addr, align 8
  %0 = load float**, float*** %devPtr.addr, align 8
  %1 = bitcast float** %0 to i8*
  %2 = bitcast i8* %1 to i8**
  %3 = load i64, i64* %size.addr, align 8
  %call = call i32 @cudaMalloc(i8** noundef %2, i64 noundef %3)
  ret i32 %call
}

declare dso_local i32 @printf(i8* noundef, ...) #1

declare dso_local i8* @cudaGetErrorString(i32 noundef) #1

; Function Attrs: noreturn nounwind
declare dso_local void @exit(i32 noundef) #8

declare dso_local i32 @cudaMemcpy(i8* noundef, i8* noundef, i64 noundef, i32 noundef) #1

declare dso_local i32 @__cudaPushCallConfiguration(i64, i32, i64, i32, i64 noundef, i8* noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN4dim3C2Ejjj(%struct.dim3* noundef nonnull align 4 dereferenceable(12) %this, i32 noundef %vx, i32 noundef %vy, i32 noundef %vz) unnamed_addr #9 comdat align 2 {
entry:
  %this.addr = alloca %struct.dim3*, align 8
  %vx.addr = alloca i32, align 4
  %vy.addr = alloca i32, align 4
  %vz.addr = alloca i32, align 4
  store %struct.dim3* %this, %struct.dim3** %this.addr, align 8
  store i32 %vx, i32* %vx.addr, align 4
  store i32 %vy, i32* %vy.addr, align 4
  store i32 %vz, i32* %vz.addr, align 4
  %this1 = load %struct.dim3*, %struct.dim3** %this.addr, align 8
  %x = getelementptr inbounds %struct.dim3, %struct.dim3* %this1, i32 0, i32 0
  %0 = load i32, i32* %vx.addr, align 4
  store i32 %0, i32* %x, align 4
  %y = getelementptr inbounds %struct.dim3, %struct.dim3* %this1, i32 0, i32 1
  %1 = load i32, i32* %vy.addr, align 4
  store i32 %1, i32* %y, align 4
  %z = getelementptr inbounds %struct.dim3, %struct.dim3* %this1, i32 0, i32 2
  %2 = load i32, i32* %vz.addr, align 4
  store i32 %2, i32* %z, align 4
  ret void
}

declare dso_local i32 @cudaDeviceSynchronize() #1

declare dso_local noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8), i8* noundef) #1

declare dso_local noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8), i32 noundef) #1

declare dso_local noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEf(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8), float noundef) #1

declare dso_local i32 @cudaDeviceReset() #1

declare dso_local i32 @cudaMalloc(i8** noundef, i64 noundef) #1

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_axpy.cu() #0 section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }
attributes #4 = { noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { argmemonly nofree nounwind willreturn }
attributes #6 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #10 = { noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 11, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 1}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 14.0.6"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
