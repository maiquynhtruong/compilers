; ModuleID = 'subset-sum.c'
source_filename = "subset-sum.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@main.set = private unnamed_addr constant [6 x i32] [i32 3, i32 34, i32 4, i32 12, i32 5, i32 2], align 16
@.str = private unnamed_addr constant [30 x i8] c"Found a subset with given sum\00", align 1
@.str.1 = private unnamed_addr constant [25 x i8] c"No subset with given sum\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define zeroext i1 @isSubsetSum(i32*, i32, i32) #0 {
  %4 = alloca i32*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i8*, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32* %0, i32** %4, align 8
  store i32 %1, i32* %5, align 4
  store i32 %2, i32* %6, align 4
  %12 = load i32, i32* %5, align 4
  %13 = add nsw i32 %12, 1
  %14 = zext i32 %13 to i64
  %15 = load i32, i32* %6, align 4
  %16 = add nsw i32 %15, 1
  %17 = zext i32 %16 to i64
  %18 = call i8* @llvm.stacksave()
  store i8* %18, i8** %7, align 8
  %19 = mul nuw i64 %14, %17
  %20 = alloca i8, i64 %19, align 16
  store i32 0, i32* %8, align 4
  br label %21

; <label>:21:                                     ; preds = %31, %3
  %22 = load i32, i32* %8, align 4
  %23 = load i32, i32* %5, align 4
  %24 = icmp sle i32 %22, %23
  br i1 %24, label %25, label %34

; <label>:25:                                     ; preds = %21
  %26 = load i32, i32* %8, align 4
  %27 = sext i32 %26 to i64
  %28 = mul nsw i64 %27, %17
  %29 = getelementptr inbounds i8, i8* %20, i64 %28
  %30 = getelementptr inbounds i8, i8* %29, i64 0
  store i8 1, i8* %30, align 1
  br label %31

; <label>:31:                                     ; preds = %25
  %32 = load i32, i32* %8, align 4
  %33 = add nsw i32 %32, 1
  store i32 %33, i32* %8, align 4
  br label %21

; <label>:34:                                     ; preds = %21
  store i32 1, i32* %9, align 4
  br label %35

; <label>:35:                                     ; preds = %45, %34
  %36 = load i32, i32* %9, align 4
  %37 = load i32, i32* %6, align 4
  %38 = icmp sle i32 %36, %37
  br i1 %38, label %39, label %48

; <label>:39:                                     ; preds = %35
  %40 = mul nsw i64 0, %17
  %41 = getelementptr inbounds i8, i8* %20, i64 %40
  %42 = load i32, i32* %9, align 4
  %43 = sext i32 %42 to i64
  %44 = getelementptr inbounds i8, i8* %41, i64 %43
  store i8 0, i8* %44, align 1
  br label %45

; <label>:45:                                     ; preds = %39
  %46 = load i32, i32* %9, align 4
  %47 = add nsw i32 %46, 1
  store i32 %47, i32* %9, align 4
  br label %35

; <label>:48:                                     ; preds = %35
  store i32 1, i32* %10, align 4
  br label %49

; <label>:49:                                     ; preds = %139, %48
  %50 = load i32, i32* %10, align 4
  %51 = load i32, i32* %5, align 4
  %52 = icmp sle i32 %50, %51
  br i1 %52, label %53, label %142

; <label>:53:                                     ; preds = %49
  store i32 1, i32* %11, align 4
  br label %54

; <label>:54:                                     ; preds = %135, %53
  %55 = load i32, i32* %11, align 4
  %56 = load i32, i32* %6, align 4
  %57 = icmp sle i32 %55, %56
  br i1 %57, label %58, label %138

; <label>:58:                                     ; preds = %54
  %59 = load i32, i32* %11, align 4
  %60 = load i32*, i32** %4, align 8
  %61 = load i32, i32* %10, align 4
  %62 = sub nsw i32 %61, 1
  %63 = sext i32 %62 to i64
  %64 = getelementptr inbounds i32, i32* %60, i64 %63
  %65 = load i32, i32* %64, align 4
  %66 = icmp slt i32 %59, %65
  br i1 %66, label %67, label %86

; <label>:67:                                     ; preds = %58
  %68 = load i32, i32* %10, align 4
  %69 = sub nsw i32 %68, 1
  %70 = sext i32 %69 to i64
  %71 = mul nsw i64 %70, %17
  %72 = getelementptr inbounds i8, i8* %20, i64 %71
  %73 = load i32, i32* %11, align 4
  %74 = sext i32 %73 to i64
  %75 = getelementptr inbounds i8, i8* %72, i64 %74
  %76 = load i8, i8* %75, align 1
  %77 = trunc i8 %76 to i1
  %78 = load i32, i32* %10, align 4
  %79 = sext i32 %78 to i64
  %80 = mul nsw i64 %79, %17
  %81 = getelementptr inbounds i8, i8* %20, i64 %80
  %82 = load i32, i32* %11, align 4
  %83 = sext i32 %82 to i64
  %84 = getelementptr inbounds i8, i8* %81, i64 %83
  %85 = zext i1 %77 to i8
  store i8 %85, i8* %84, align 1
  br label %86

; <label>:86:                                     ; preds = %67, %58
  %87 = load i32, i32* %11, align 4
  %88 = load i32*, i32** %4, align 8
  %89 = load i32, i32* %10, align 4
  %90 = sub nsw i32 %89, 1
  %91 = sext i32 %90 to i64
  %92 = getelementptr inbounds i32, i32* %88, i64 %91
  %93 = load i32, i32* %92, align 4
  %94 = icmp sge i32 %87, %93
  br i1 %94, label %95, label %134

; <label>:95:                                     ; preds = %86
  %96 = load i32, i32* %10, align 4
  %97 = sub nsw i32 %96, 1
  %98 = sext i32 %97 to i64
  %99 = mul nsw i64 %98, %17
  %100 = getelementptr inbounds i8, i8* %20, i64 %99
  %101 = load i32, i32* %11, align 4
  %102 = sext i32 %101 to i64
  %103 = getelementptr inbounds i8, i8* %100, i64 %102
  %104 = load i8, i8* %103, align 1
  %105 = trunc i8 %104 to i1
  br i1 %105, label %124, label %106

; <label>:106:                                    ; preds = %95
  %107 = load i32, i32* %10, align 4
  %108 = sub nsw i32 %107, 1
  %109 = sext i32 %108 to i64
  %110 = mul nsw i64 %109, %17
  %111 = getelementptr inbounds i8, i8* %20, i64 %110
  %112 = load i32, i32* %11, align 4
  %113 = load i32*, i32** %4, align 8
  %114 = load i32, i32* %10, align 4
  %115 = sub nsw i32 %114, 1
  %116 = sext i32 %115 to i64
  %117 = getelementptr inbounds i32, i32* %113, i64 %116
  %118 = load i32, i32* %117, align 4
  %119 = sub nsw i32 %112, %118
  %120 = sext i32 %119 to i64
  %121 = getelementptr inbounds i8, i8* %111, i64 %120
  %122 = load i8, i8* %121, align 1
  %123 = trunc i8 %122 to i1
  br label %124

; <label>:124:                                    ; preds = %106, %95
  %125 = phi i1 [ true, %95 ], [ %123, %106 ]
  %126 = load i32, i32* %10, align 4
  %127 = sext i32 %126 to i64
  %128 = mul nsw i64 %127, %17
  %129 = getelementptr inbounds i8, i8* %20, i64 %128
  %130 = load i32, i32* %11, align 4
  %131 = sext i32 %130 to i64
  %132 = getelementptr inbounds i8, i8* %129, i64 %131
  %133 = zext i1 %125 to i8
  store i8 %133, i8* %132, align 1
  br label %134

; <label>:134:                                    ; preds = %124, %86
  br label %135

; <label>:135:                                    ; preds = %134
  %136 = load i32, i32* %11, align 4
  %137 = add nsw i32 %136, 1
  store i32 %137, i32* %11, align 4
  br label %54

; <label>:138:                                    ; preds = %54
  br label %139

; <label>:139:                                    ; preds = %138
  %140 = load i32, i32* %10, align 4
  %141 = add nsw i32 %140, 1
  store i32 %141, i32* %10, align 4
  br label %49

; <label>:142:                                    ; preds = %49
  %143 = load i32, i32* %5, align 4
  %144 = sext i32 %143 to i64
  %145 = mul nsw i64 %144, %17
  %146 = getelementptr inbounds i8, i8* %20, i64 %145
  %147 = load i32, i32* %6, align 4
  %148 = sext i32 %147 to i64
  %149 = getelementptr inbounds i8, i8* %146, i64 %148
  %150 = load i8, i8* %149, align 1
  %151 = trunc i8 %150 to i1
  %152 = load i8*, i8** %7, align 8
  call void @llvm.stackrestore(i8* %152)
  ret i1 %151
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #1

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [6 x i32], align 16
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %5 = bitcast [6 x i32]* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %5, i8* bitcast ([6 x i32]* @main.set to i8*), i64 24, i32 16, i1 false)
  store i32 9, i32* %3, align 4
  store i32 6, i32* %4, align 4
  %6 = getelementptr inbounds [6 x i32], [6 x i32]* %2, i32 0, i32 0
  %7 = load i32, i32* %4, align 4
  %8 = load i32, i32* %3, align 4
  %9 = call zeroext i1 @isSubsetSum(i32* %6, i32 %7, i32 %8)
  %10 = zext i1 %9 to i32
  %11 = icmp eq i32 %10, 1
  br i1 %11, label %12, label %14

; <label>:12:                                     ; preds = %0
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str, i32 0, i32 0))
  br label %16

; <label>:14:                                     ; preds = %0
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str.1, i32 0, i32 0))
  br label %16

; <label>:16:                                     ; preds = %14, %12
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
