  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  
.global hexasc
.global delay
.global time2string

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
	li	$t0, 1		# time constant
	
d_loop:
	ble 	$a0, $0, d_end
	nop
	addi	$a0, $a0, -1
	li	$t1, 0		# int i = 0
	j	sleep
	nop
	
sleep:
	addi	$t1, $t1, 1	    # i = i + 1
	ble	$t1, $t0, sleep	    # i < time constant
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
