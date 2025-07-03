; Synthetic LLVM IR with thousands of redundant expressions for GVN testing
; This file contains many duplicate computations to stress-test value numbering

define i64 @heavy_computation(i64 %a, i64 %b, i64 %c, i64 %d) {
entry:
  ; Expanded redundancy for stress testing
  %add6 = add i64 %a, %b    ; Additional duplicate
  %add7 = add i64 %b, %a    ; Additional commutative duplicate
  %add8 = add i64 %a, %b    ; Additional duplicate
  %add9 = add i64 %b, %a    ; Additional commutative duplicate

  %mul6 = mul i64 %c, %d    ; Additional duplicate
  %mul7 = mul i64 %d, %c    ; Additional commutative duplicate
  %mul8 = mul i64 %c, %d    ; Additional duplicate
  %mul9 = mul i64 %d, %c    ; Additional commutative duplicate

  %sub6 = sub i64 %add6, %mul6  ; Equivalent to sub1
  %sub7 = sub i64 %add7, %mul7  ; Equivalent to sub1
  %sub8 = sub i64 %add8, %mul8  ; Equivalent to sub1
  %sub9 = sub i64 %add9, %mul9  ; Equivalent to sub1

  %complex6 = add i64 %add6, %mul6
  %complex7 = add i64 %add7, %mul7  ; Same as complex1
  %complex8 = add i64 %add8, %mul8  ; Same as complex1
  %complex9 = add i64 %add9, %mul9  ; Same as complex1

  %shl6 = shl i64 %a, 3    ; Additional duplicate
  %shl7 = shl i64 %a, 3    ; Additional duplicate

  %shr6 = ashr i64 %b, 2   ; Additional duplicate
  %shr7 = ashr i64 %b, 2   ; Additional duplicate

  %combo6 = add i64 %shl6, %shr6  ; Same as combo1
  %combo7 = add i64 %shl7, %shr7  ; Same as combo1

  %pattern6 = mul i64 %combo6, 7  ; Same as pattern1
  %pattern7 = mul i64 %combo7, 7  ; Same as pattern1

  %result6 = add i64 %pattern6, %complex6  ; Same as result1
  %result7 = add i64 %pattern7, %complex7  ; Same as result1

  %extra6 = sub i64 %result6, 42  ; Same as extra1
  %extra7 = sub i64 %result7, 42  ; Same as extra1
  ; Create many redundant basic arithmetic operations
  %add1 = add i64 %a, %b
  %add2 = add i64 %a, %b    ; Duplicate
  %add3 = add i64 %b, %a    ; Commutative duplicate
  %add4 = add i64 %a, %b    ; Another duplicate
  %add5 = add i64 %b, %a    ; Another commutative duplicate
  
  %mul1 = mul i64 %c, %d
  %mul2 = mul i64 %c, %d    ; Duplicate
  %mul3 = mul i64 %d, %c    ; Commutative duplicate
  %mul4 = mul i64 %c, %d    ; Another duplicate
  %mul5 = mul i64 %d, %c    ; Another commutative duplicate
  
  %sub1 = sub i64 %add1, %mul1
  %sub2 = sub i64 %add2, %mul2  ; Equivalent to sub1
  %sub3 = sub i64 %add3, %mul3  ; Equivalent to sub1
  %sub4 = sub i64 %add4, %mul4  ; Equivalent to sub1
  %sub5 = sub i64 %add5, %mul5  ; Equivalent to sub1
  
  ; More complex redundant expressions
  %complex1 = add i64 %add1, %mul1
  %complex2 = add i64 %add2, %mul2  ; Same as complex1
  %complex3 = add i64 %add3, %mul3  ; Same as complex1
  %complex4 = add i64 %add4, %mul4  ; Same as complex1
  %complex5 = add i64 %add5, %mul5  ; Same as complex1
  
  ; Shift operations with redundancy
  %shl1 = shl i64 %a, 3
  %shl2 = shl i64 %a, 3    ; Duplicate
  %shl3 = shl i64 %a, 3    ; Duplicate
  %shl4 = shl i64 %a, 3    ; Duplicate
  %shl5 = shl i64 %a, 3    ; Duplicate
  
  %shr1 = ashr i64 %b, 2
  %shr2 = ashr i64 %b, 2   ; Duplicate
  %shr3 = ashr i64 %b, 2   ; Duplicate
  %shr4 = ashr i64 %b, 2   ; Duplicate
  %shr5 = ashr i64 %b, 2   ; Duplicate
  
  ; Combinations with previous results
  %combo1 = add i64 %shl1, %shr1
  %combo2 = add i64 %shl2, %shr2  ; Same as combo1
  %combo3 = add i64 %shl3, %shr3  ; Same as combo1
  %combo4 = add i64 %shl4, %shr4  ; Same as combo1
  %combo5 = add i64 %shl5, %shr5  ; Same as combo1
  
  ; More arithmetic patterns
  %pattern1 = mul i64 %combo1, 7
  %pattern2 = mul i64 %combo2, 7  ; Same as pattern1
  %pattern3 = mul i64 %combo3, 7  ; Same as pattern1
  %pattern4 = mul i64 %combo4, 7  ; Same as pattern1
  %pattern5 = mul i64 %combo5, 7  ; Same as pattern1
  
  ; Final result computation with many redundant paths
  %result1 = add i64 %pattern1, %complex1
  %result2 = add i64 %pattern2, %complex2  ; Same as result1
  %result3 = add i64 %pattern3, %complex3  ; Same as result1
  %result4 = add i64 %pattern4, %complex4  ; Same as result1
  %result5 = add i64 %pattern5, %complex5  ; Same as result1
  
  ; Even more redundant computations
  %extra1 = sub i64 %result1, 42
  %extra2 = sub i64 %result2, 42  ; Same as extra1
  %extra3 = sub i64 %result3, 42  ; Same as extra1
  %extra4 = sub i64 %result4, 42  ; Same as extra1
  %extra5 = sub i64 %result5, 42  ; Same as extra1
  
  ret i64 %extra1
}

define i64 @massive_redundancy(i64 %x, i64 %y) {
entry:
  ; Create a massive amount of redundant expressions
  %base1 = add i64 %x, %y
  %base2 = add i64 %x, %y    ; Duplicate 1
  %base3 = add i64 %y, %x    ; Commutative duplicate
  %base4 = add i64 %x, %y    ; Duplicate 2
  %base5 = add i64 %y, %x    ; Commutative duplicate 2
  %base6 = add i64 %x, %y    ; Duplicate 3
  %base7 = add i64 %y, %x    ; Commutative duplicate 3
  %base8 = add i64 %x, %y    ; Duplicate 4
  %base9 = add i64 %y, %x    ; Commutative duplicate 4
  %base10 = add i64 %x, %y   ; Duplicate 5
  
  ; Square each base (more redundancy)
  %sq1 = mul i64 %base1, %base1
  %sq2 = mul i64 %base2, %base2   ; Same as sq1
  %sq3 = mul i64 %base3, %base3   ; Same as sq1
  %sq4 = mul i64 %base4, %base4   ; Same as sq1
  %sq5 = mul i64 %base5, %base5   ; Same as sq1
  %sq6 = mul i64 %base6, %base6   ; Same as sq1
  %sq7 = mul i64 %base7, %base7   ; Same as sq1
  %sq8 = mul i64 %base8, %base8   ; Same as sq1
  %sq9 = mul i64 %base9, %base9   ; Same as sq1
  %sq10 = mul i64 %base10, %base10 ; Same as sq1
  
  ; Double each square
  %double1 = shl i64 %sq1, 1
  %double2 = shl i64 %sq2, 1    ; Same as double1
  %double3 = shl i64 %sq3, 1    ; Same as double1
  %double4 = shl i64 %sq4, 1    ; Same as double1
  %double5 = shl i64 %sq5, 1    ; Same as double1
  %double6 = shl i64 %sq6, 1    ; Same as double1
  %double7 = shl i64 %sq7, 1    ; Same as double1
  %double8 = shl i64 %sq8, 1    ; Same as double1
  %double9 = shl i64 %sq9, 1    ; Same as double1
  %double10 = shl i64 %sq10, 1  ; Same as double1
  
  ; Add constants to each
  %const1 = add i64 %double1, 1234
  %const2 = add i64 %double2, 1234   ; Same as const1
  %const3 = add i64 %double3, 1234   ; Same as const1
  %const4 = add i64 %double4, 1234   ; Same as const1
  %const5 = add i64 %double5, 1234   ; Same as const1
  %const6 = add i64 %double6, 1234   ; Same as const1
  %const7 = add i64 %double7, 1234   ; Same as const1
  %const8 = add i64 %double8, 1234   ; Same as const1
  %const9 = add i64 %double9, 1234   ; Same as const1
  %const10 = add i64 %double10, 1234 ; Same as const1
  
  ; Final sum (all should be the same)
  %sum = add i64 %const1, %const2
  %sum2 = add i64 %const3, %const4   ; Should be same as sum
  %sum3 = add i64 %const5, %const6   ; Should be same as sum
  %sum4 = add i64 %const7, %const8   ; Should be same as sum
  %sum5 = add i64 %const9, %const10  ; Should be same as sum
  
  %final = add i64 %sum, %sum2
  %final2 = add i64 %sum3, %sum4     ; Should be same as final
  %total = add i64 %final, %final2
  %total2 = add i64 %total, %sum5
  
  ret i64 %total2
}

define void @pointer_heavy_redundancy(ptr %ptr, i64 %offset) {
entry:
  ; Many redundant pointer arithmetic operations
  %gep1 = getelementptr inbounds i64, ptr %ptr, i64 %offset
  %gep2 = getelementptr inbounds i64, ptr %ptr, i64 %offset    ; Duplicate
  %gep3 = getelementptr inbounds i64, ptr %ptr, i64 %offset    ; Duplicate
  %gep4 = getelementptr inbounds i64, ptr %ptr, i64 %offset    ; Duplicate
  %gep5 = getelementptr inbounds i64, ptr %ptr, i64 %offset    ; Duplicate
  
  ; Redundant offset calculations
  %off1 = add i64 %offset, 8
  %off2 = add i64 %offset, 8    ; Duplicate
  %off3 = add i64 %offset, 8    ; Duplicate
  %off4 = add i64 %offset, 8    ; Duplicate
  %off5 = add i64 %offset, 8    ; Duplicate
  
  ; More pointer arithmetic with computed offsets
  %gep_off1 = getelementptr inbounds i64, ptr %ptr, i64 %off1
  %gep_off2 = getelementptr inbounds i64, ptr %ptr, i64 %off2  ; Same as gep_off1
  %gep_off3 = getelementptr inbounds i64, ptr %ptr, i64 %off3  ; Same as gep_off1
  %gep_off4 = getelementptr inbounds i64, ptr %ptr, i64 %off4  ; Same as gep_off1
  %gep_off5 = getelementptr inbounds i64, ptr %ptr, i64 %off5  ; Same as gep_off1
  
  ; Loads from same addresses
  %load1 = load i64, ptr %gep1, align 8
  %load2 = load i64, ptr %gep2, align 8    ; Same address as load1
  %load3 = load i64, ptr %gep3, align 8    ; Same address as load1
  %load4 = load i64, ptr %gep4, align 8    ; Same address as load1
  %load5 = load i64, ptr %gep5, align 8    ; Same address as load1
  
  ; Computations on loaded values
  %comp1 = add i64 %load1, 100
  %comp2 = add i64 %load2, 100    ; Same as comp1
  %comp3 = add i64 %load3, 100    ; Same as comp1
  %comp4 = add i64 %load4, 100    ; Same as comp1
  %comp5 = add i64 %load5, 100    ; Same as comp1
  
  ; Stores back to computed addresses
  store i64 %comp1, ptr %gep_off1, align 8
  store i64 %comp2, ptr %gep_off2, align 8
  store i64 %comp3, ptr %gep_off3, align 8
  store i64 %comp4, ptr %gep_off4, align 8
  store i64 %comp5, ptr %gep_off5, align 8
  
  ret void
}

define i64 @nested_redundancy(i64 %a, i64 %b, i64 %c) {
entry:
  ; Level 1 redundancy
  %l1_1 = add i64 %a, %b
  %l1_2 = add i64 %a, %b    ; Duplicate
  %l1_3 = add i64 %b, %a    ; Commutative
  %l1_4 = add i64 %a, %b    ; Duplicate
  %l1_5 = add i64 %b, %a    ; Commutative
  
  %l1_mul1 = mul i64 %b, %c
  %l1_mul2 = mul i64 %b, %c    ; Duplicate
  %l1_mul3 = mul i64 %c, %b    ; Commutative
  %l1_mul4 = mul i64 %b, %c    ; Duplicate
  %l1_mul5 = mul i64 %c, %b    ; Commutative
  
  ; Level 2 redundancy (using level 1 results)
  %l2_1 = add i64 %l1_1, %l1_mul1
  %l2_2 = add i64 %l1_2, %l1_mul2    ; Same as l2_1
  %l2_3 = add i64 %l1_3, %l1_mul3    ; Same as l2_1
  %l2_4 = add i64 %l1_4, %l1_mul4    ; Same as l2_1
  %l2_5 = add i64 %l1_5, %l1_mul5    ; Same as l2_1
  
  %l2_sub1 = sub i64 %l1_1, %l1_mul1
  %l2_sub2 = sub i64 %l1_2, %l1_mul2    ; Same as l2_sub1
  %l2_sub3 = sub i64 %l1_3, %l1_mul3    ; Same as l2_sub1
  %l2_sub4 = sub i64 %l1_4, %l1_mul4    ; Same as l2_sub1
  %l2_sub5 = sub i64 %l1_5, %l1_mul5    ; Same as l2_sub1
  
  ; Level 3 redundancy
  %l3_1 = mul i64 %l2_1, %l2_sub1
  %l3_2 = mul i64 %l2_2, %l2_sub2    ; Same as l3_1
  %l3_3 = mul i64 %l2_3, %l2_sub3    ; Same as l3_1
  %l3_4 = mul i64 %l2_4, %l2_sub4    ; Same as l3_1
  %l3_5 = mul i64 %l2_5, %l2_sub5    ; Same as l3_1
  
  ; Level 4 redundancy
  %l4_1 = add i64 %l3_1, %c
  %l4_2 = add i64 %l3_2, %c    ; Same as l4_1
  %l4_3 = add i64 %l3_3, %c    ; Same as l4_1
  %l4_4 = add i64 %l3_4, %c    ; Same as l4_1
  %l4_5 = add i64 %l3_5, %c    ; Same as l4_1
  
  ; Final level redundancy
  %final1 = xor i64 %l4_1, %l4_2
  %final2 = xor i64 %l4_3, %l4_4    ; Should be 0 (same as final1)
  %result = add i64 %final1, %final2
  
  ret i64 %result
}

define i64 @extreme_redundancy_generator(i64 %seed) {
entry:
  ; Generate 50 copies of the same computation pattern
  %t1 = add i64 %seed, 1
  %t2 = add i64 %seed, 1
  %t3 = add i64 %seed, 1
  %t4 = add i64 %seed, 1
  %t5 = add i64 %seed, 1
  %t6 = add i64 %seed, 1
  %t7 = add i64 %seed, 1
  %t8 = add i64 %seed, 1
  %t9 = add i64 %seed, 1
  %t10 = add i64 %seed, 1
  %t11 = add i64 %seed, 1
  %t12 = add i64 %seed, 1
  %t13 = add i64 %seed, 1
  %t14 = add i64 %seed, 1
  %t15 = add i64 %seed, 1
  %t16 = add i64 %seed, 1
  %t17 = add i64 %seed, 1
  %t18 = add i64 %seed, 1
  %t19 = add i64 %seed, 1
  %t20 = add i64 %seed, 1
  %t21 = add i64 %seed, 1
  %t22 = add i64 %seed, 1
  %t23 = add i64 %seed, 1
  %t24 = add i64 %seed, 1
  %t25 = add i64 %seed, 1
  %t26 = add i64 %seed, 1
  %t27 = add i64 %seed, 1
  %t28 = add i64 %seed, 1
  %t29 = add i64 %seed, 1
  %t30 = add i64 %seed, 1
  %t31 = add i64 %seed, 1
  %t32 = add i64 %seed, 1
  %t33 = add i64 %seed, 1
  %t34 = add i64 %seed, 1
  %t35 = add i64 %seed, 1
  %t36 = add i64 %seed, 1
  %t37 = add i64 %seed, 1
  %t38 = add i64 %seed, 1
  %t39 = add i64 %seed, 1
  %t40 = add i64 %seed, 1
  %t41 = add i64 %seed, 1
  %t42 = add i64 %seed, 1
  %t43 = add i64 %seed, 1
  %t44 = add i64 %seed, 1
  %t45 = add i64 %seed, 1
  %t46 = add i64 %seed, 1
  %t47 = add i64 %seed, 1
  %t48 = add i64 %seed, 1
  %t49 = add i64 %seed, 1
  %t50 = add i64 %seed, 1
  
  ; Square each one (50 identical multiplications)
  %s1 = mul i64 %t1, %t1
  %s2 = mul i64 %t2, %t2
  %s3 = mul i64 %t3, %t3
  %s4 = mul i64 %t4, %t4
  %s5 = mul i64 %t5, %t5
  %s6 = mul i64 %t6, %t6
  %s7 = mul i64 %t7, %t7
  %s8 = mul i64 %t8, %t8
  %s9 = mul i64 %t9, %t9
  %s10 = mul i64 %t10, %t10
  %s11 = mul i64 %t11, %t11
  %s12 = mul i64 %t12, %t12
  %s13 = mul i64 %t13, %t13
  %s14 = mul i64 %t14, %t14
  %s15 = mul i64 %t15, %t15
  %s16 = mul i64 %t16, %t16
  %s17 = mul i64 %t17, %t17
  %s18 = mul i64 %t18, %t18
  %s19 = mul i64 %t19, %t19
  %s20 = mul i64 %t20, %t20
  %s21 = mul i64 %t21, %t21
  %s22 = mul i64 %t22, %t22
  %s23 = mul i64 %t23, %t23
  %s24 = mul i64 %t24, %t24
  %s25 = mul i64 %t25, %t25
  %s26 = mul i64 %t26, %t26
  %s27 = mul i64 %t27, %t27
  %s28 = mul i64 %t28, %t28
  %s29 = mul i64 %t29, %t29
  %s30 = mul i64 %t30, %t30
  %s31 = mul i64 %t31, %t31
  %s32 = mul i64 %t32, %t32
  %s33 = mul i64 %t33, %t33
  %s34 = mul i64 %t34, %t34
  %s35 = mul i64 %t35, %t35
  %s36 = mul i64 %t36, %t36
  %s37 = mul i64 %t37, %t37
  %s38 = mul i64 %t38, %t38
  %s39 = mul i64 %t39, %t39
  %s40 = mul i64 %t40, %t40
  %s41 = mul i64 %t41, %t41
  %s42 = mul i64 %t42, %t42
  %s43 = mul i64 %t43, %t43
  %s44 = mul i64 %t44, %t44
  %s45 = mul i64 %t45, %t45
  %s46 = mul i64 %t46, %t46
  %s47 = mul i64 %t47, %t47
  %s48 = mul i64 %t48, %t48
  %s49 = mul i64 %t49, %t49
  %s50 = mul i64 %t50, %t50
  
  ; Sum them all up (this will demonstrate the power of GVN)
  %sum1 = add i64 %s1, %s2
  %sum2 = add i64 %sum1, %s3
  %sum3 = add i64 %sum2, %s4
  %sum4 = add i64 %sum3, %s5
  %sum5 = add i64 %sum4, %s6
  %sum6 = add i64 %sum5, %s7
  %sum7 = add i64 %sum6, %s8
  %sum8 = add i64 %sum7, %s9
  %sum9 = add i64 %sum8, %s10
  %sum10 = add i64 %sum9, %s11
  %sum11 = add i64 %sum10, %s12
  %sum12 = add i64 %sum11, %s13
  %sum13 = add i64 %sum12, %s14
  %sum14 = add i64 %sum13, %s15
  %sum15 = add i64 %sum14, %s16
  %sum16 = add i64 %sum15, %s17
  %sum17 = add i64 %sum16, %s18
  %sum18 = add i64 %sum17, %s19
  %sum19 = add i64 %sum18, %s20
  %sum20 = add i64 %sum19, %s21
  %sum21 = add i64 %sum20, %s22
  %sum22 = add i64 %sum21, %s23
  %sum23 = add i64 %sum22, %s24
  %sum24 = add i64 %sum23, %s25
  %sum25 = add i64 %sum24, %s26
  %sum26 = add i64 %sum25, %s27
  %sum27 = add i64 %sum26, %s28
  %sum28 = add i64 %sum27, %s29
  %sum29 = add i64 %sum28, %s30
  %sum30 = add i64 %sum29, %s31
  %sum31 = add i64 %sum30, %s32
  %sum32 = add i64 %sum31, %s33
  %sum33 = add i64 %sum32, %s34
  %sum34 = add i64 %sum33, %s35
  %sum35 = add i64 %sum34, %s36
  %sum36 = add i64 %sum35, %s37
  %sum37 = add i64 %sum36, %s38
  %sum38 = add i64 %sum37, %s39
  %sum39 = add i64 %sum38, %s40
  %sum40 = add i64 %sum39, %s41
  %sum41 = add i64 %sum40, %s42
  %sum42 = add i64 %sum41, %s43
  %sum43 = add i64 %sum42, %s44
  %sum44 = add i64 %sum43, %s45
  %sum45 = add i64 %sum44, %s46
  %sum46 = add i64 %sum45, %s47
  %sum47 = add i64 %sum46, %s48
  %sum48 = add i64 %sum47, %s49
  %final_sum = add i64 %sum48, %s50
  
  ret i64 %final_sum
}
