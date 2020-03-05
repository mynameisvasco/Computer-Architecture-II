        #159 page contains all LAT, TRIS and PORT addresses
        .equ READ_CORE_TIMER, 11
        .equ RESET_CORE_TIMER, 12
        .equ PRINT_INT, 6 
        .equ SFR_BASE_HI, 0xBF88            # 16 MSbits of SFR area
        .equ TRISE, 0x6100                  # TRISE address is 0xBF886100
        .equ PORTE, 0x6110 				    # PORTE address is 0xBF886110
        .equ LATE, 0x6120                   # LATE address is 0xBF886120

        .data                   
        .text
        .globl main
    
main:   addu $sp, $sp, -4                   #SAVE CONTEXT 
        sw $ra, 0($sp)                      #SAVE CONTEXT

        li $t0, 0
        lui $t1, SFR_BASE_HI            
        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFFE
        sw $t2, TRISE($t1)

loop:   lw $t2, LATE($t1)
        andi $t2, $t2, 0xFFFE
        or $t2, $t2, $t0
        sw $t2, LATE($t1)
        li $a0, 500
        jal delay
        xori $t0, $t0, 1
        li $v0, PRINT_INT               
        move $a0, $t0
        li $a1, 10
        syscall
        j loop
        lw $ra, 0($sp)
        addu $sp, $sp, 4
        jr $ra

delay:  ble $a0, 0, end                 # for(; ms > 0; ms--) {
        li $v0, RESET_CORE_TIMER        #   resetCoreTimer();
        syscall
rct:    li $v0, READ_CORE_TIMER
        syscall                         #       print_int(counter);
        blt $v0, 20000, rct             #   while(readCoreTimer() < 20000);
        addi $a0, $a0, -1               #   ms--;
        j delay                         # }
end:    jr $ra