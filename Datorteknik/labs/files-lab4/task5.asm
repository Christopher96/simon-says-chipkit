
.text

addi 	$a0, $a0, 8	# int n = input
beq	$a0, $0, done	# i == 0?

addi	$a1, $a1, 1	# int multiplier = 1
addi	$a2, $a2, 2	# int i = 2
addi	$v0, $v0, 1	# int sum = 1	

loop:	
	add	$a3, $a3, $a2 	# j += i
	addi	$a3, $a3, -1	# j -= 1
	fact: 	
		beq	$a3, $0, end	# j == 0?
		add	$v0, $v0, $a1	# sum += multiplier
		add	$a3, $a3, -1	# j -= 1
		beq	$0, $0, fact	# add again	
	end:
	beq	$a2, $a0, done	# i == n?
	r1: 
		beq 	$a1, $v0, r1_done 	# multiplier = sum
		addi	$a1, $a1, 1
		beq	$0, $0, r1
	r1_done:
	add 	$a2, $a2, 1	# i++
	beq	$0, $0, loop	# loop again
	
done:	
beq	$0, $0, done	# loop forever


















