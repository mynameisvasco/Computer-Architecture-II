        #159 page contains all LAT, TRIS and PORT addresses

        .equ SFR_BASE_HI, 0xBF88            # 16 MSbits of SFR area
        .equ TRISE, 0x6100                  # TRISE address is 0xBF886100
        .equ PORTE, 0x6110 				    # PORTE address is 0xBF886110
        .equ LATE, 0x6120                   # LATE address is 0xBF886120

        .equ TRISB, 0x6040                  # TRISB address is 0xBF886040
		.equ PORTB, 0x6050 				    # PORTB address is 0xBF886050
		.equ LATB,  0x6060 				    # LATB  address is 0xBF886060
        .data                   
        .text
        .globl main

main:   lui $t1, SFR_BASE_HI                #

        lw $t2, TRISE($t1)                  # READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t2, $t2, 0xFFF6               #
        sw $t2, TRISE($t1)                  # TRISE(0) = 0 (output)

        lw $t2, TRISB($t1)                  # 
        ori $t2, $t2, 0x000F                # 
        sw $t2, TRISB($t1)                  # TRISB(0) = 1 (input)

loop:   lw $t0, PORTB($t1)     
        andi $t0, $t0, 0x000F    
        lw $t2, LATE($t1)    
        andi $t2, $t2, 0xFFF6
        nor $t2, $t2, $t0
        sw $t2, LATE($t1)  
        j loop
        jr $ra