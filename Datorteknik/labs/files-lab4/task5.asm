
.text

addi 	$a0, $a0, 3	# n = input	3
add	$a1, $a1, $a0	# i = n		3
loop:	
	beq	$a1, $0, done	# i == 0?	3
	add	$a2, $a2, $a1 	# j = i		3
	fact: 	
		beq	$a2, $0, end	# j == 0?	1
		add	$v0, $v0, $a2	# sum += i	9
		add	$a2, $a2, -1	# j--		1
		beq	$0, $0, fact	# add again	
	end:
	add 	$a1, $a1, -1	# i--
	beq	$0, $0, loop	# loop again
done:	

beq	$0, $0, done	# loop forever