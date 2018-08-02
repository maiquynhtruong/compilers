; ModuleID = 'codegen.bc'
source_filename = "tests/test-array.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.1 = private unnamed_addr constant [3 x i8] c"%s\00"
@format_str.2 = private unnamed_addr constant [3 x i8] c"%d\00"
@format_str.3 = private unnamed_addr constant [3 x i8] c"%s\00"
@string = private unnamed_addr constant [2 x i8] c" \00"

declare i32 @printf(...)

declare i32 @scanf(...)

define void @main() {
main_entry:
  %numbers = alloca [10 x i32]
  %res = alloca [10 x i32]
  %i = alloca i32
  store i32 0, i32* %i
  br label %start_loop

start_loop:                                       ; preds = %loop, %main_entry
  %i1 = load i32, i32* %i
  %add = add i32 %i1, 1
  store i32 %add, i32* %i
  %i2 = load i32, i32* %i
  %lt = icmp slt i32 %i2, 10
  br i1 %lt, label %loop, label %end_loop

loop:                                             ; preds = %start_loop
  %i3 = load i32, i32* %i
  %dest_array_GEP = getelementptr inbounds [10 x i32], [10 x i32]* %numbers, i32 0, i32 %i3
  %i4 = load i32, i32* %i
  store i32 %i4, i32* %dest_array_GEP
  br label %start_loop

end_loop:                                         ; preds = %start_loop
  %factor_array_GEP = getelementptr inbounds [10 x i32], [10 x i32]* %numbers, i32 0, i32 0
  %factor_array_GEP5 = getelementptr inbounds [10 x i32], [10 x i32]* %res, i32 0, i32 0
  call void @square_array(i32* %factor_array_GEP, i32* %factor_array_GEP5)
  store i32 0, i32* %i
  br label %start_loop6

start_loop6:                                      ; preds = %loop7, %end_loop
  %i9 = load i32, i32* %i
  %add10 = add i32 %i9, 1
  store i32 %add10, i32* %i
  %i11 = load i32, i32* %i
  %lt12 = icmp slt i32 %i11, 10
  br i1 %lt12, label %loop7, label %end_loop8

loop7:                                            ; preds = %start_loop6
  %i13 = load i32, i32* %i
  %factor_array_GEP14 = getelementptr inbounds [10 x i32], [10 x i32]* %res, i32 0, i32 %i13
  %factor_array_load = load i32, i32* %factor_array_GEP14
  call void @putinteger(i32 %factor_array_load)
  call void @putstring(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @string, i32 0, i32 0))
  br label %start_loop6

end_loop8:                                        ; preds = %start_loop6
  ret void
}

define void @getinteger(i32* %val) {
getinteger:
  %getinteger1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i32* %val)
  ret void
}

define void @getstring(i8* %val) {
getstring:
  %getstring1 = call i32 (...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.1, i32 0, i32 0), i8* %val)
  ret void
}

define void @putinteger(i32 %val) {
putinteger:
  %putinteger1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.2, i32 0, i32 0), i32 %val)
  ret void
}

define void @putstring(i8* %val) {
putstring:
  %putstring1 = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str.3, i32 0, i32 0), i8* %val)
  ret void
}

define void @square_array(i32*, i32*) {
square_array:
  %array = alloca i32*
  store i32* %0, i32** %array
  %result = alloca i32*
  store i32* %1, i32** %result
  %i = alloca i32
  store i32 0, i32* %i
  br label %start_loop

start_loop:                                       ; preds = %loop, %square_array
  %i1 = load i32, i32* %i
  %add = add i32 %i1, 1
  store i32 %add, i32* %i
  %i2 = load i32, i32* %i
  %lt = icmp slt i32 %i2, 10
  br i1 %lt, label %loop, label %end_loop

loop:                                             ; preds = %start_loop
  %i3 = load i32, i32* %i
  %dest_pointer_load = load i32*, i32** %result
  %dest_pointer_GEP = getelementptr inbounds i32, i32* %dest_pointer_load, i32 %i3
  %i4 = load i32, i32* %i
  %factor_pointer_load = load i32*, i32** %array
  %factor_pointer_GEP = getelementptr inbounds i32, i32* %factor_pointer_load, i32 %i4
  %factor_array_load = load i32, i32* %factor_pointer_GEP
  %i5 = load i32, i32* %i
  %factor_pointer_load6 = load i32*, i32** %array
  %factor_pointer_GEP7 = getelementptr inbounds i32, i32* %factor_pointer_load6, i32 %i5
  %factor_array_load8 = load i32, i32* %factor_pointer_GEP7
  %mul = mul i32 %factor_array_load, %factor_array_load8
  store i32 %mul, i32* %dest_pointer_GEP
  br label %start_loop

end_loop:                                         ; preds = %start_loop
  ret void
}
