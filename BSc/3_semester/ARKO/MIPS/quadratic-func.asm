	.globl	main
	.data
	
output:	.asciiz	"out.bmp"
get_a:	.asciiz "a = "
get_b:	.asciiz "b = "
get_c:	.asciiz "c = "
open_err_msg:	.asciiz	"blad otwarcia pliku!"

# move address by 2 bytes when accesing
bitmap_header:
		.word 	0x4D420000 # weird hack for mod 4 alignment
		.word 	0x00000046 # size of the whole bmp file !
		.word 	0x00000000
		.word	0x00000036 # offset of raw bitmap
		.word 	0x00000028 # DIB header
		.word	0x00000002 # width in pixels  !
		.word 	0x00000002 # height in pixels !
		.word 	0x00180001 # color panes and bits per pixel
		.word 	0x00000000
		.word   0x00000010 # size of the raw bitmap !
		.word 	0x00000B13
		.word 	0x00000B13
		.word 	0x00000000
		.word 	0x00000000
# bitmap properties
width:		.word	400
height:		.word   400

size:		.space 	4
raw_size:	.space 	4
bytes_per_row: 	.space	4
bmp_raw_data:	.space 	4

# x,y are between -16 and 16
# a,b,c are between -8 and 8
# optimal fixed-point convention is 15bits for integral part and 17bits for fraction part
# X = f(x) = -16 + 32*(x / maxx)

# x coresponds to bitmap, X corresponds to coordinate system our quadratic function will be drawn in
offsetX:	.word 	0xffe00000 	# -16.0
factorX:	.space 	4		# log(width / 32) (shift method)
factorY:	.space 	4		# log(height / 32) (shift method)

a_:		.space	4
b_:		.space 	4
c_:		.space 	4

	.text
main:


	la	$a0, get_a 	#get a
	li	$v0, 4
	syscall
	li	$v0, 5
	syscall
	move	$t0, $v0
	sll	$t0, $t0, 17
	#li	$t0, 0x0001000	# place to write fractional A
	sw	$t0, a_
	
	la	$a0, get_b	#get b
	li	$v0, 4
	syscall
	li	$v0, 5
	syscall
	move	$t0, $v0
	sll	$t0, $t0, 17
	#li	$t0, 0x00010000 # place to write fractional B
	sw	$t0, b_
	
	la	$a0, get_c	#get c
	li	$v0, 4
	syscall
	li	$v0, 5
	syscall
	move	$t0, $v0
	sll	$t0, $t0, 17
	#li	$t0, 0x00008000	# place to write fractional C
	sw	$t0, c_	
	
	
	# for shift method
	#lw	$t0, width	# calculate scaling factorX
	#srl	$t7, $t0, 5	# width / 32
	#jal	log
	#sw	$t7, factorX
	
	#lw	$t0, height	# calculate factorY
	#srl	$t7, $t0, 5	# height / 32
	#jal 	log
	#sw	$t7, factorY
	
modify_header:
	la 	$t1, bitmap_header 	# load address of bitmap header and move it by 2 bytes
	addi	$t1, $t1, 2
	lw	$t2, width 		#load needed values
	lw 	$t3, height
	
	sw	$t2, 0x12($t1)		# modify height and width in pixels in bitmap_header
	sw	$t3, 0x16($t1)
	
	li	$t4, 3
	mul	$t6, $t2, $t4		#width * 3bytes (24bits) = size of row in bytes (without padding)
	
	and 	$t4, $t6, 0x3		# calculate padding
	li	$t7, 4
	sub	$t7, $t7, $t4
	and	$t7, $t7, 0x3
	
	add	$t6, $t6, $t7
	sw	$t6, bytes_per_row
	
calculate_size:
	mul 	$t4, $t6, $t3	# bytes_per_row * height = total raw bitmap size
	sw	$t4, raw_size
	addi 	$t4, $t4, 54
	sw	$t4, size
	
	sw	$t4, 0x22($t1)
allocate_raw: 			# allocate memory for raw bitmap
	move	$a0, $t4
	li	$v0, 9
	syscall
	move	$t0, $v0 	#remember the address of the allocated memory
	sw	$t0, bmp_raw_data
	
	addi 	$t4, $t4, 54 	# raw + sizeof(BMP + DIB Headers) = size of the bmp file
	sw 	$t4, 0x2($t1)

fill_with_white:
	# height and width are still in t2, t3 registers	 !!!!!!
	li	$t4, 0
	lw 	$t5, raw_size
	li	$s0, 0xffffffff
	move 	$s1, $t0
loop:
	addi 	$t4, $t4, 4
	bgt	$t4, $t5, draw_X_axis
	sw	$s0, ($s1)
	addi	$s1, $s1, 4
	b loop

draw_X_axis:
	li 	$s0, 0x00
	
	move 	$s1, $t0
	srl	$s2, $t3, 1	# set s2 to height / 2
	lw 	$t5, bytes_per_row
	mul	$s2, $s2, $t5	# height / 2 * bytes_per_row = offset of x axis
	add	$s1, $s1, $s2
	move 	$s4, $s1	# save address of the first pixel for later (we will be calculating offset based of this)
	li	$t4, 0

loop2:
	addi 	$t4, $t4, 1
	bgt 	$t4, $t2, draw_Y_axis
	sb	$s0, ($s1)
	sb 	$s0, 1($s1)
	sb 	$s0, 2($s1)
	addi 	$s1, $s1, 3
	b loop2
	
draw_Y_axis:
	move 	$s1, $t0
	srl 	$s3, $t2, 1	#set s3 to width / 2
	addi	$s3, $s3, 1
	mul	$s3, $s3, 3
	add	$s1, $s1, $s3	#now s1 points at the top of Y axis
	li	$t4, 0
	
loop3:
	addi	$t4, $t4, 1
	bgt 	$t4, $t3, draw_quadratic_func
	sb	$s0, ($s1)
	sb 	$s0, 1($s1)
	sb 	$s0, 2($s1)
	add 	$s1, $s1, $t5	# move s1 by a whole row length
	b loop3
	
draw_quadratic_func:
	li	$t1, 0x00
	move	$s0, $s4	# set s0 to address of the first pixel on X axis
	lw	$s1, offsetX
	
	#lw 	$s2, factorX	# for shift method
	#lw 	$t7, factorY
	
	lw	$s2, width	# for div method
	li	$t0, 32
	lw	$t7, height
	srl	$t7, $t7, 1 	# set t7 to height / 2
	
	lw	$t6, a_
	lw	$s5, b_
	lw	$s7, c_
	li	$t4, 0 		# t5 still has bytes_per_row
	srl	$t2, $t3, 1 	# height / 2
	mul	$t8, $t2, -1

	

loop4:
	bgt	$t4, $t3, save_bitmap
	
	sll	$s3, $t4, 17	# X = f(x)
	
	# 32*(x / maxx)
	#srlv	$s3  $s3, $s2	# shift method
	
	div 	$s3, $s3, $s2	# div method
	mul	$s3, $s3, $t0

	add	$s3, $s3, $s1	
	
	move 	$s4, $s3	# save the X
	mult	$s3, $s3	# X^2
	mflo	$s3
	mfhi	$s6
	sll	$s6, $s6, 15
	srl 	$s3, $s3, 17
	add	$s3, $s3, $s6
	
	mult	$s3, $t6  	# A * X^2
	mflo	$s3
	mfhi	$s6
	sll	$s6, $s6, 15
	srl 	$s3, $s3, 17
	add	$s3, $s3, $s6
	
	mult	$s4, $s5	# BX
	mflo	$s4
	mfhi	$s6
	sll	$s6, $s6, 15
	srl	$s4, $s4, 17
	add	$s4, $s4, $s6
	
	add	$s3, $s3, $s4	# AX^2 + BX
	add	$s3, $s3, $s7	# AX^2 + BX + C
	
	addi	$t4, $t4, 1	# increment the counters
	addi	$s0, $s0, 3	
	
	
	# convert back to bitmap coordinate system
	
	#sllv	$s3, $s3, $t7	# apply scaling factor (shift method)
	
	div	$s3, $s3, 16
	mul	$s3, $s3, $t7	# g(Y) = y = (Y / 16)*(height / 2)	
	
	
	sra	$s3, $s3, 17	# convert back to integer (while keeping the sign)
	bgt 	$s3, $t2, loop4
	blt 	$s3, $t8, loop4
	
	mul	$s3, $s3, $t5	# y(X) * bytes_per_row = offset_from_x_axis
	add 	$s3, $s0, $s3	# address of first pixel on x axis + offset = position of our pixel
	
	sb	$t1, ($s3)	# store the black pixel
	sb	$t1, 1($s3)
	sb	$t1, 2($s3)
	
	b 	loop4
save_bitmap:	
	#open file in write mode
	li	$v0, 15
  	li   	$v0, 13       	# system call for open file
  	la   	$a0, output     # output file name
  	li   	$a1, 1        	# Open for writing (flags are 0: read, 1: write)
  	li   	$a2, 0       	# mode is ignored
  	syscall            	# open a file (file descriptor returned in $v0)
  	move 	$s6, $v0      	# save the fi/le descriptor
  
  	#save bmp header
  	la	$t1, bitmap_header # load bitmap header address
  	addi 	$t1, $t1, 2	   # move past padding
  	li	$v0, 15
  	move 	$a0, $s6
  	la 	$a1, ($t1)
  	li	$a2, 54
  	syscall
  	
  	#save bmp raw
  	li	$v0, 15
  	move 	$a0, $s6
  	la	$s1, bmp_raw_data
  	lw	$a1, ($s1)
  	lw	$a2, raw_size
  	syscall
  	
  	li	$v0, 10
  	syscall
  	
  # calculates logarithm of value in t7 (used in shift method)
 log:
 	li 	$t6, 0
 log_loop:
 	srl	$t7, $t7, 1
 	beqz	$t7, log_end
 	addi	$t6, $t6, 1
 	b 	log_loop	
 log_end:
 	move	$t7, $t6
 	jr	$ra
