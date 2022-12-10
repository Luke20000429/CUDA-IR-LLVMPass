; ModuleID = 'axpy.cu'
source_filename = "axpy.cu"
target datalayout = "e-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-cuda"

%struct.__cuda_builtin_threadIdx_t = type { i8 }

@threadIdx = extern_weak dso_local addrspace(1) global %struct.__cuda_builtin_threadIdx_t, align 1

; Function Attrs: convergent mustprogress noinline norecurse nounwind optnone
define dso_local void @_Z4axpyfPfS_(float noundef %a, float* noundef %x, float* noundef %y) #0 {
entry:
  %a.addr = alloca float, align 4
  %x.addr = alloca float*, align 8
  %y.addr = alloca float*, align 8
  store float %a, float* %a.addr, align 4
  store float* %x, float** %x.addr, align 8
  store float* %y, float** %y.addr, align 8
  %0 = load float, float* %a.addr, align 4
  %1 = load float*, float** %x.addr, align 8
  %2 = call i32 @llvm.nvvm.read.ptx.sreg.tid.x() #2
  %idxprom = zext i32 %2 to i64
  %arrayidx = getelementptr inbounds float, float* %1, i64 %idxprom
  %3 = load float, float* %arrayidx, align 4
  %mul = fmul contract float %0, %3
  %4 = load float*, float** %y.addr, align 8
  %5 = call i32 @llvm.nvvm.read.ptx.sreg.tid.x() #2
  %idxprom2 = zext i32 %5 to i64
  %arrayidx3 = getelementptr inbounds float, float* %4, i64 %idxprom2
  store float %mul, float* %arrayidx3, align 4
  ret void
}

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.nvvm.read.ptx.sreg.tid.x() #1

attributes #0 = { convergent mustprogress noinline norecurse nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_61" "target-features"="+ptx75,+sm_61" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!nvvm.annotations = !{!4}
!llvm.ident = !{!5, !6}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 11, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 4, !"nvvm-reflect-ftz", i32 0}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{void (float, float*, float*)* @_Z4axpyfPfS_, !"kernel", i32 1}
!5 = !{!"clang version 14.0.6"}
!6 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
