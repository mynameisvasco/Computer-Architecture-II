        .equ READ_CORE_TIMER, 11
        .equ RESET_CORE_TIMER, 12
        .equ PUT_CHAR, 3
        .equ PRINT_INT, 6 
        .data
        .text
        .globl main

main: 	addu $sp, $sp, -4
        sw $ra, 0($sp)

loop: 	li $a0, 1000							# while(1) {
        jal delay                                                       #  delay(1000);
        li $a0, 1    							# 
        li $a1, 10 							# 
        li $v0, PRINT_INT 						# 
        syscall								#  printInt(1, 10);
        li $a0, ' ' 							# 
        li $v0, PUT_CHAR 						#
        syscall								#  putChar(' ');
        lw $ra, 0($sp)							#
        addu $sp, $sp, 4						#
	jr $ra 						                # } 


delay:  ble $a0, 0, end                                                 # for(; ms > 0; ms--) {
        li $v0, RESET_CORE_TIMER                                        #   resetCoreTimer();
        syscall                                                         #
rct:    li $v0, READ_CORE_TIMER                                         #
        syscall                                                         #       print_int(counter);
        blt $v0, 20000, rct                                             #   while(readCoreTimer() < 20000);
        addi $a0, $a0, -1                                               #   ms--;
        j delay                                                         # }
end:    jr $ra