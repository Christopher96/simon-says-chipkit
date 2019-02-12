.data
.global factlist
.align 2
factlist:
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0

.text
.global makelist
makelist:
	PUSH 	$ra
	PUSH 	$s0
	PUSH 	$s1
	PUSH 	$s2
	move 	$s0, $a0
	move 	$s1, $a1
	li	$s2, $0 	# i = 0
makeloop:
	slt 	$t0, $s2, $s1	# i < length?
	beq	$t0, $0, makeend
	move 	$a0, $s0
	jal 	fact
	sll	$t0, $s2, 2
	la	$t1, factlist	# ra ?ndras och koden som anropas f?r ?ndra v0, v1, a0-a3, t0-t8
	add	$t2, $t1, $t0
	sw	$v0, 0($t2)
	addi 	$s0, $s0, 1
	j	makeloop
makeend:
	POP 	$s2 		# obs: omv?nd ordning mot PUSH
	POP 	$s1
	POP	$s0
	POP 	$ra
	jr	$ra
	