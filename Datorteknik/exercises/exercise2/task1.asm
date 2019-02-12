li 	$t0, 0x4F
la	$t1, 0x800020F0		# Labels and addresses are interchangable
sw	$t0, 0($t1)		


# alternativt
.equ 	pattern, 0x800020F0	
la 	$t1, pattern