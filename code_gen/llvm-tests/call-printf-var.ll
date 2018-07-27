; ModuleID = 'call-printf-var.c'
source_filename = "call-printf-var.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@.str = private unnamed_addr constant [19 x i8] c"The integer is %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"The float is %f\0A\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"The char is %c\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @print_variables_4(i32, float, i8 signext) #0 {
  %4 = alloca i32, align 4
  %5 = alloca float, align 4
  %6 = alloca i8, align 1
  store i32 %0, i32* %4, align 4
  store float %1, float* %5, align 4
  store i8 %2, i8* %6, align 1
  %7 = load i32, i32* %4, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str, i32 0, i32 0), i32 %7)
  %9 = load float, float* %5, align 4
  %10 = fpext float %9 to double
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), double %10)
  %12 = load i8, i8* %6, align 1
  %13 = sext i8 %12 to i32
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 %13)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @print_variables() #0 {
  %1 = alloca i32, align 4
  %2 = alloca float, align 4
  store i32 1, i32* %1, align 4
  %3 = load i32, i32* %1, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str, i32 0, i32 0), i32 %3)
  store float 0x3F8945B6C0000000, float* %2, align 4
  call void @print_variables_4(i32 1, float 2.000000e+00, i8 signext 51)
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @print_variables_2(i32, i1 zeroext) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  %5 = alloca float, align 4
  store i32 %0, i32* %3, align 4
  %6 = zext i1 %1 to i8
  store i8 %6, i8* %4, align 1
  store float 0x3F8945B6C0000000, float* %5, align 4
  %7 = load i32, i32* %3, align 4
  %8 = load float, float* %5, align 4
  call void @print_variables_4(i32 %7, float %8, i8 signext 97)
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @print_variables_3(i32, float, i8 signext, i8*, i1 zeroext) #0 {
  %6 = alloca i32, align 4
  %7 = alloca float, align 4
  %8 = alloca i8, align 1
  %9 = alloca i8*, align 8
  %10 = alloca i8, align 1
  %11 = alloca i32, align 4
  %12 = alloca float, align 4
  %13 = alloca i8, align 1
  %14 = alloca i8*, align 8
  %15 = alloca i8, align 1
  store i32 %0, i32* %6, align 4
  store float %1, float* %7, align 4
  store i8 %2, i8* %8, align 1
  store i8* %3, i8** %9, align 8
  %16 = zext i1 %4 to i8
  store i8 %16, i8* %10, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @print_variables()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 9.1.0 (clang-902.0.39.2)"}
