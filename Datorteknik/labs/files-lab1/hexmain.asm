  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,15		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop: 	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #

hexasc:
	or 	$t0, $0, $a0	# mask argument, ignore upper bytes
	
	# if argument is less or equal to 9, print 0-9, otherwise A-F
	
	addi	$t1, $t0, 0x30 	# start from ascii 0x30
	
	li	$t2, 9
	ble	$t0, $t2, cont 	# continue if argument less than 9
	
	addi 	$t1, $t0, 0x40	# start from ascii 0x40
	subi	$t1, $t1, 9	# subtract 9 to start from 1

cont:
	lui	$v0, 0		# load upper with 0
	andi	$v0, $t1, 0x7F	# mask lower with 0111 1111, 7 least significant bits
	
	jr 	$ra		# jump to return address of main routine
	nop
	
	
