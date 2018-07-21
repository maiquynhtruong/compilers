; ModuleID = 'codegen.bc'
source_filename = "tests/test-if.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

declare i32 @printf(...)

declare void @getbool(i32)

declare void @getinteger(i32)

declare void @getfloat(float)

declare void @getstring(i8*)

declare void @getchar(i8)

declare void @putbool(i32)

declare void @putinteger(i32)

declare void @putfloat(float)

declare void @putstring(i8*)

declare void @putchar(i8)

define void @main() {
main_entry:
  %jake = alloca i32
  %zach = alloca i32
  br i1 true, label %then, label %else

then:                                             ; preds = %main_entry
  store i32 1, i32* %jake
  br label %merge

else:                                             ; preds = %main_entry
  store i32 3, i32* %zach
  br label %merge

merge:                                            ; preds = %else, %then
  ret void
}
