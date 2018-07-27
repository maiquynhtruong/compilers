; ModuleID = 'codegen.bc'
source_filename = "tests/test-string.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.1 = private unnamed_addr constant [3 x i8] c"%s\00"
@string = private unnamed_addr constant [12 x i8] c"Hello world\00"
@string.2 = private unnamed_addr constant [17 x i8] c"Parameter string\00"
@string.3 = private unnamed_addr constant [36 x i8] c"Local string assigned to a variable\00"
@string.4 = private unnamed_addr constant [9 x i8] c"It's Mai\00"

declare i32 @printf(...)

define void @main() {
main_entry:
  call void @print_string(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @string.4, i32 0, i32 0))
  ret void
}

define void @putinteger(i32 %val) {
putinteger:
  %val1 = alloca i32
  store i32 %val, i32* %val1
  %val2 = load i32, i32* %val1
  %putinteger3 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32 %val2)
  ret void
}

define void @putstring(i8* %val) {
putstring:
  %val1 = alloca i8*
  store i8* %val, i8** %val1
  %val2 = load i8*, i8** %val1
  %putstring3 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.1, i32 0, i32 0), i8* %val2)
  ret void
}

define void @print_string(i8* %arg1) {
print_string:
  %arg11 = alloca i8*
  store i8* %arg1, i8** %arg11
  %s = alloca i8*
  call void @putstring(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @string, i32 0, i32 0))
  call void @putstring(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @string.2, i32 0, i32 0))
  %arg12 = load i8*, i8** %arg11
  call void @putstring(i8* %arg12)
  store i8* getelementptr inbounds ([36 x i8], [36 x i8]* @string.3, i32 0, i32 0), i8** %s
  %s3 = load i8*, i8** %s
  call void @putstring(i8* %s3)
  ret void
}
