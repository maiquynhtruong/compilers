; ModuleID = 'codegen.bc'
source_filename = "tests/test-var.src"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

@format_str = private unnamed_addr constant [3 x i8] c"%d\00"
@val_str = private unnamed_addr constant [13 x i8] c"Hihihihihihi\00"

declare i32 @printf(...)

declare void @getBool(i8)

declare void @getInteger(i32)

declare void @getFloat(float)

declare void @getString(i8*)

declare void @getChar(i8)

declare void @putBool(i8)

declare void @putInteger(i32)

declare void @putFloat(float)

declare void @putString(i8*)

declare void @putChar(i8)

define void @main() {
entry:
  %putInteger = call i32 (...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @format_str, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @val_str, i32 0, i32 0))
  ret void
}
