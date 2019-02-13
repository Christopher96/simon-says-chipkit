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

.global enable_interrupt
.global hexasc
.global delay
.global time2string

enable_interrupt:
	ei
	jr	$ra
	nop

hexasc:
	addi	$t0, $a0, 0x30 	# start from ascii 0x30
	
	li	$t1, 9
	ble	$a0, $t1, cont 	# continue if argument less than 9
	nop
	
	addi 	$t0, $t0, 7	# add 7 to start from ascii 0x40
cont:
	#andi	$t1, $t0, 0xF
	move	$v0, $t0
	
	jr 	$ra		# jump to return address of main routine
	nop
	
delay:
	PUSH	$a0		# int ms
	PUSH	$ra
	li	$t0, 4711	# time constant
	
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
	PUSH 	$a0
	PUSH 	$a1
	PUSH	$s0
	PUSH	$s1
	PUSH	$ra
	
	la	$s0, 0($a0)	# a0 timestr adress
	move	$s1, $a1	# a1 timeval

	andi	$a0, $s1, 0xF
	jal 	hexasc
	nop
	move	$t0, $v0
	sb	$t0, 4($s0)
	
	andi	$a0, $s1, 0xF0
	srl	$a0, $a0, 4
	jal 	hexasc
	nop
	move	$t0, $v0
	sb	$t0, 3($s0)
	
	li	$t0, 0x3a
	sb	$t0, 2($s0)
	
	andi	$a0, $s1, 0xF00
	srl	$a0, $a0, 8
	jal 	hexasc
	nop
	move	$t0, $v0
	sb	$t0, 1($s0)
	
	andi	$a0, $s1, 0xF000
	srl	$a0, $a0, 12
	jal 	hexasc
	nop
	move	$t0, $v0
	sb	$t0, 0($s0)
	
	POP	$ra
	POP	$s1
	POP	$s0
	POP	$a1
	POP	$a0	
	
	jr 	$ra
	nop
