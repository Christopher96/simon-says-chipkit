  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "time: "
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0, 1000
	jal	delay		# delay(1000)
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
  
hexasc:
	or 	$t0, $0, $a0	# mask argument, ignore upper bytes
	
	addi	$t1, $t0, 0x30 	# start from ascii 0x30
	
	li	$t2, 9
	ble	$t0, $t2, cont 	# continue if argument less than 9
	nop
	
	addi 	$t1, $t0, 0x40	# start from ascii 0x40
	addi	$t1, $t1, -9	# subtract 9 to start from 1

cont:
	lui	$v0, 0		# load upper with 0
	andi	$v0, $t1, 0x7F	# mask lower with 0111 1111, 7 least significant bits
	
	jr 	$ra		# jump to return address of main routine
	nop
	
delay:
	PUSH	$a0		# int ms
	PUSH	$ra
	li	$t0, 90		# time constant
	
d_loop:
	ble 	$a0, $0, d_end
	nop
	addi	$a0, $a0, -1
	li	$t1, 0		# int i = 0
	j	sleep
	nop
	
sleep:
	addi	$t1, $t1, 1	# i = i + 1
	ble	$t1, $t0, sleep # i < time constant
	nop
	j 	d_loop
	nop
	
d_end:
	POP 	$ra
	POP	$a0
	jr	$ra
	nop

	
time2string:
	PUSH	$ra
	PUSH	$a0
	PUSH	$s0
	li	$s0, 3		# 4 digits
	
t2s_loop:
	mul	$t0, $s0, 4	# calculate shamt
	srlv	$t1, $a1, $t0	# shift to next digit
	andi	$t2, $t1, 0xF 	# mask last nibble and store digit
	move	$a0, $t2	# load digit as argument
	
	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	
	
	move	$a0, $v0	# copy return value to argument register

	li	$v0, 11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window

	beq	$s0, $0, t2s_end# end loop if no more digits
	nop
	bne	$s0, 2, nocol	# print colon only at index 2
	nop
	
	li	$a0, 0x3A
	li	$v0, 11		
	syscall			# print colon
	
nocol:
	addi	$s0, $s0, -1	# decrement by one
	j	t2s_loop	# loop with next digit
	nop
	
t2s_end:
	POP	$s0
	POP	$a0
	POP 	$ra

	jr 	$ra
	nop
