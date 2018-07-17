; ModuleID = 'coin-change.c'
source_filename = "coin-change.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@main.arr = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3], align 4
@.str = private unnamed_addr constant [5 x i8] c" %d \00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @count(i32*, i32, i32) #0 {
  %4 = alloca i32*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i8*, align 8
  store i32* %0, i32** %4, align 8
  store i32 %1, i32* %5, align 4
  store i32 %2, i32* %6, align 4
  %12 = load i32, i32* %6, align 4
  %13 = add nsw i32 %12, 1
  %14 = zext i32 %13 to i64
  %15 = load i32, i32* %5, align 4
  %16 = zext i32 %15 to i64
  %17 = call i8* @llvm.stacksave()
  store i8* %17, i8** %11, align 8
  %18 = mul nuw i64 %14, %16
  %19 = alloca i32, i64 %18, align 16
  store i32 0, i32* %7, align 4
  br label %20

; <label>:20:                                     ; preds = %30, %3
  %21 = load i32, i32* %7, align 4
  %22 = load i32, i32* %5, align 4
  %23 = icmp slt i32 %21, %22
  br i1 %23, label %24, label %33

; <label>:24:                                     ; preds = %20
  %25 = mul nsw i64 0, %16
  %26 = getelementptr inbounds i32, i32* %19, i64 %25
  %27 = load i32, i32* %7, align 4
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds i32, i32* %26, i64 %28
  store i32 1, i32* %29, align 4
  br label %30

; <label>:30:                                     ; preds = %24
  %31 = load i32, i32* %7, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, i32* %7, align 4
  br label %20

; <label>:33:                                     ; preds = %20
  store i32 1, i32* %7, align 4
  br label %34

; <label>:34:                                     ; preds = %100, %33
  %35 = load i32, i32* %7, align 4
  %36 = load i32, i32* %6, align 4
  %37 = add nsw i32 %36, 1
  %38 = icmp slt i32 %35, %37
  br i1 %38, label %39, label %103

; <label>:39:                                     ; preds = %34
  store i32 0, i32* %8, align 4
  br label %40

; <label>:40:                                     ; preds = %96, %39
  %41 = load i32, i32* %8, align 4
  %42 = load i32, i32* %5, align 4
  %43 = icmp slt i32 %41, %42
  br i1 %43, label %44, label %99

; <label>:44:                                     ; preds = %40
  %45 = load i32, i32* %7, align 4
  %46 = load i32*, i32** %4, align 8
  %47 = load i32, i32* %8, align 4
  %48 = sext i32 %47 to i64
  %49 = getelementptr inbounds i32, i32* %46, i64 %48
  %50 = load i32, i32* %49, align 4
  %51 = sub nsw i32 %45, %50
  %52 = icmp sge i32 %51, 0
  br i1 %52, label %53, label %68

; <label>:53:                                     ; preds = %44
  %54 = load i32, i32* %7, align 4
  %55 = load i32*, i32** %4, align 8
  %56 = load i32, i32* %8, align 4
  %57 = sext i32 %56 to i64
  %58 = getelementptr inbounds i32, i32* %55, i64 %57
  %59 = load i32, i32* %58, align 4
  %60 = sub nsw i32 %54, %59
  %61 = sext i32 %60 to i64
  %62 = mul nsw i64 %61, %16
  %63 = getelementptr inbounds i32, i32* %19, i64 %62
  %64 = load i32, i32* %8, align 4
  %65 = sext i32 %64 to i64
  %66 = getelementptr inbounds i32, i32* %63, i64 %65
  %67 = load i32, i32* %66, align 4
  br label %69

; <label>:68:                                     ; preds = %44
  br label %69

; <label>:69:                                     ; preds = %68, %53
  %70 = phi i32 [ %67, %53 ], [ 0, %68 ]
  store i32 %70, i32* %9, align 4
  %71 = load i32, i32* %8, align 4
  %72 = icmp sge i32 %71, 1
  br i1 %72, label %73, label %83

; <label>:73:                                     ; preds = %69
  %74 = load i32, i32* %7, align 4
  %75 = sext i32 %74 to i64
  %76 = mul nsw i64 %75, %16
  %77 = getelementptr inbounds i32, i32* %19, i64 %76
  %78 = load i32, i32* %8, align 4
  %79 = sub nsw i32 %78, 1
  %80 = sext i32 %79 to i64
  %81 = getelementptr inbounds i32, i32* %77, i64 %80
  %82 = load i32, i32* %81, align 4
  br label %84

; <label>:83:                                     ; preds = %69
  br label %84

; <label>:84:                                     ; preds = %83, %73
  %85 = phi i32 [ %82, %73 ], [ 0, %83 ]
  store i32 %85, i32* %10, align 4
  %86 = load i32, i32* %9, align 4
  %87 = load i32, i32* %10, align 4
  %88 = add nsw i32 %86, %87
  %89 = load i32, i32* %7, align 4
  %90 = sext i32 %89 to i64
  %91 = mul nsw i64 %90, %16
  %92 = getelementptr inbounds i32, i32* %19, i64 %91
  %93 = load i32, i32* %8, align 4
  %94 = sext i32 %93 to i64
  %95 = getelementptr inbounds i32, i32* %92, i64 %94
  store i32 %88, i32* %95, align 4
  br label %96

; <label>:96:                                     ; preds = %84
  %97 = load i32, i32* %8, align 4
  %98 = add nsw i32 %97, 1
  store i32 %98, i32* %8, align 4
  br label %40

; <label>:99:                                     ; preds = %40
  br label %100

; <label>:100:                                    ; preds = %99
  %101 = load i32, i32* %7, align 4
  %102 = add nsw i32 %101, 1
  store i32 %102, i32* %7, align 4
  br label %34

; <label>:103:                                    ; preds = %34
  %104 = load i32, i32* %6, align 4
  %105 = sext i32 %104 to i64
  %106 = mul nsw i64 %105, %16
  %107 = getelementptr inbounds i32, i32* %19, i64 %106
  %108 = load i32, i32* %5, align 4
  %109 = sub nsw i32 %108, 1
  %110 = sext i32 %109 to i64
  %111 = getelementptr inbounds i32, i32* %107, i64 %110
  %112 = load i32, i32* %111, align 4
  %113 = load i8*, i8** %11, align 8
  call void @llvm.stackrestore(i8* %113)
  ret i32 %112
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #1

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [3 x i32], align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %5 = bitcast [3 x i32]* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %5, i8* bitcast ([3 x i32]* @main.arr to i8*), i64 12, i32 4, i1 false)
  store i32 3, i32* %3, align 4
  store i32 4, i32* %4, align 4
  %6 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i32 0, i32 0
  %7 = load i32, i32* %3, align 4
  %8 = load i32, i32* %4, align 4
  %9 = call i32 @count(i32* %6, i32 %7, i32 %8)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), i32 %9)
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #2

declare i32 @printf(i8*, ...) #3

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { argmemonly nounwind }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 9.1.0 (clang-902.0.39.2)"}
