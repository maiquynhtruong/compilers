; ModuleID = 'fac.bc'
source_filename = "fac_module"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

define i32 @fac(i32) {
entry:
  %"n == 0" = icmp eq i32 %0, 0
  br i1 %"n == 0", label %end, label %iffalse

iffalse:                                          ; preds = %entry
  %"n - 1" = sub i32 %0, 1
  %"fac(n - 1)" = call i32 @fac(i32 %"n - 1")
  %"n * fac(n - 1)" = mul i32 %0, %"fac(n - 1)"
  br label %end

end:                                              ; preds = %iffalse, %entry
  %result = phi i32 [ %"n * fac(n - 1)", %iffalse ], [ 1, %entry ]
  ret i32 %result
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
