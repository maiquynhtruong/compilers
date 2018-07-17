; ModuleID = 'binary-search-recursive.c'
source_filename = "binary-search-recursive.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@main.arr = private unnamed_addr constant [5 x i32] [i32 2, i32 3, i32 4, i32 10, i32 40], align 16
@.str = private unnamed_addr constant [32 x i8] c"Element is not present in array\00", align 1
@.str.1 = private unnamed_addr constant [31 x i8] c"Element is present at index %d\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @binarySearch(i32*, i32, i32, i32) #0 {
  %5 = alloca i32, align 4
  %6 = alloca i32*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32* %0, i32** %6, align 8
  store i32 %1, i32* %7, align 4
  store i32 %2, i32* %8, align 4
  store i32 %3, i32* %9, align 4
  %11 = load i32, i32* %8, align 4
  %12 = load i32, i32* %7, align 4
  %13 = icmp sge i32 %11, %12
  br i1 %13, label %14, label %52

; <label>:14:                                     ; preds = %4
  %15 = load i32, i32* %7, align 4
  %16 = load i32, i32* %8, align 4
  %17 = load i32, i32* %7, align 4
  %18 = sub nsw i32 %16, %17
  %19 = sdiv i32 %18, 2
  %20 = add nsw i32 %15, %19
  store i32 %20, i32* %10, align 4
  %21 = load i32*, i32** %6, align 8
  %22 = load i32, i32* %10, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds i32, i32* %21, i64 %23
  %25 = load i32, i32* %24, align 4
  %26 = load i32, i32* %9, align 4
  %27 = icmp eq i32 %25, %26
  br i1 %27, label %28, label %30

; <label>:28:                                     ; preds = %14
  %29 = load i32, i32* %10, align 4
  store i32 %29, i32* %5, align 4
  br label %53

; <label>:30:                                     ; preds = %14
  %31 = load i32*, i32** %6, align 8
  %32 = load i32, i32* %10, align 4
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds i32, i32* %31, i64 %33
  %35 = load i32, i32* %34, align 4
  %36 = load i32, i32* %9, align 4
  %37 = icmp sgt i32 %35, %36
  br i1 %37, label %38, label %45

; <label>:38:                                     ; preds = %30
  %39 = load i32*, i32** %6, align 8
  %40 = load i32, i32* %7, align 4
  %41 = load i32, i32* %10, align 4
  %42 = sub nsw i32 %41, 1
  %43 = load i32, i32* %9, align 4
  %44 = call i32 @binarySearch(i32* %39, i32 %40, i32 %42, i32 %43)
  store i32 %44, i32* %5, align 4
  br label %53

; <label>:45:                                     ; preds = %30
  %46 = load i32*, i32** %6, align 8
  %47 = load i32, i32* %10, align 4
  %48 = add nsw i32 %47, 1
  %49 = load i32, i32* %8, align 4
  %50 = load i32, i32* %9, align 4
  %51 = call i32 @binarySearch(i32* %46, i32 %48, i32 %49, i32 %50)
  store i32 %51, i32* %5, align 4
  br label %53

; <label>:52:                                     ; preds = %4
  store i32 -1, i32* %5, align 4
  br label %53

; <label>:53:                                     ; preds = %52, %45, %38, %28
  %54 = load i32, i32* %5, align 4
  ret i32 %54
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [5 x i32], align 16
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %6 = bitcast [5 x i32]* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* bitcast ([5 x i32]* @main.arr to i8*), i64 20, i32 16, i1 false)
  store i32 5, i32* %3, align 4
  store i32 10, i32* %4, align 4
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 0
  %8 = load i32, i32* %3, align 4
  %9 = sub nsw i32 %8, 1
  %10 = load i32, i32* %4, align 4
  %11 = call i32 @binarySearch(i32* %7, i32 0, i32 %9, i32 %10)
  store i32 %11, i32* %5, align 4
  %12 = load i32, i32* %5, align 4
  %13 = icmp eq i32 %12, -1
  br i1 %13, label %14, label %16

; <label>:14:                                     ; preds = %0
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str, i32 0, i32 0))
  br label %19

; <label>:16:                                     ; preds = %0
  %17 = load i32, i32* %5, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.1, i32 0, i32 0), i32 %17)
  br label %19

; <label>:19:                                     ; preds = %16, %14
  %20 = phi i32 [ %15, %14 ], [ %18, %16 ]
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #1

declare i32 @printf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 9.1.0 (clang-902.0.39.2)"}
