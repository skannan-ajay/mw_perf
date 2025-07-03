; Simple test function with redundant expressions
; Function Attrs: noinline nounwind
define i32 @example(i32 %a, i32 %b, i32 %c) #0 {
entry:
  %add1 = add i32 %a, %b
  %add2 = add i32 %a, %b    ; Same as add1 - should get same value number
  %add3 = add i32 %b, %a    ; Commutative - should also get same value number
  %mul1 = mul i32 %a, %b
  %mul2 = mul i32 %b, %a    ; Commutative multiplication
  %sub1 = sub i32 %add1, %c
  %sub2 = sub i32 %add2, %c ; Should be same as sub1
  %result = add i32 %sub1, %mul1
  ret i32 %result
}

define i32 @another_func(i32 %x, i32 %y) #0 {
entry:
  %sum = add i32 %x, %y
  %prod = mul i32 %x, %y
  %final = add i32 %sum, %prod
  ret i32 %final
}

attributes #0 = { noinline nounwind }
