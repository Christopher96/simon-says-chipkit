
.text

addi	$a0, $a0, 8 	# int n = input
addi	$a1, $a1, 1	# int i = 1
addi	$v0, $v0, 1	# int sum = 1

loop:
	beq	$a1, $a0, done	# i == n?
	mul	$a2, $a1, $v0	# i*sum
	add	$v0, $v0, $a2	# sum += i*sum
	addi	$a1, $a1, 1	# i++
	beq	$0, $0, loop	# loop again
done:

beq	$0, $0, done 	# loop forever


















