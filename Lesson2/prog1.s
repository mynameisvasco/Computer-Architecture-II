        .equ READ_CORE_TIMER, 11
        .equ RESET_CORE_TIMER, 12
        .equ PUT_CHAR, 3
        .equ PRINT_INT, 6 
        .data
        .text
        .globl main

main:   li $t0, 0                       # int counter = 0;
while:  li $v0, READ_CORE_TIMER         # while (1) {
        syscall 
        bge $v0, 200000, main          #   while(readCoreTimer() < 200000) {
        li $v0, RESET_CORE_TIMER        #       resetCoreTimer();
        syscall
        li $v0, PUT_CHAR
        li $a0, ' '
        syscall                         #       putChar(' ');
        addi $t0, $t0, 1                #       counter++;
        li $v0, PRINT_INT
        move $a0, $t0
        li $a1, 10
        syscall                         #       print_int(counter);
        j while                         #   }  
        li $v0, 0                       #       return 0;   
        jr $ra
