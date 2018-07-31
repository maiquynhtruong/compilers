; ModuleID = 'scanf.bc'
source_filename = "my_module"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format = private unnamed_addr constant [30 x i8] c"Hey %s, please enter a string\00"
@value = private unnamed_addr constant [4 x i8] c"Mai\00"
@format.1 = private unnamed_addr constant [3 x i8] c"%s\00"
@string = internal constant [100 x i8] c"A\00\040\AA\FB<\98\01\0A\93\04\01\00\00\00\80\82\C0\0F\DF\7F\00\00\F0\09\93\04\01\00\00\00\C0\F9\C3\EB\FE\7F\00\00\C6A\80\04\01\00\00\00\F0\09\93\04\01\00\00\00\DEK\80\04\01\00\00\00\03\01\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\10\FB\C3\EB"
@format.2 = private unnamed_addr constant [19 x i8] c"The string was %s\0A\00"

define void @main() {
entry:
  %printf = call i32 (...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @format, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @value, i32 0, i32 0))
  %scanf = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format.1, i32 0, i32 0), [100 x i8]* @string)
  %printf1 = call i32 (...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @format.2, i32 0, i32 0), [100 x i8]* @string)
  ret void
}

declare i32 @printf(...)

declare i32 @scanf(...)
