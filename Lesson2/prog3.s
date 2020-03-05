        .equ READ_CORE_TIMER, 11
        .equ RESET_CORE_TIMER, 12
        .equ PUT_CHAR, 3
        .equ PRINT_INT, 6 
        .data
        .text
        .globl main

main:   addu $sp, $sp, -4
        sw $ra, 0($sp)
        li $a0, 20
loop:   jal delay                       # while(1) {
        lui $t1, 0xBF88                 #
        lw $t2,0x6050($t1)              #   //reads values from switches in binary
        andi $t2, $t2, 0x0000000F		#   //ignore everything but the switch value
        li $v0, PRINT_INT               
        move $a0, $t2
        li $a1, 0x00040002
        syscall                         #   print_int(switch_values);
        li $v0, PUT_CHAR
        li $a0, '\n'
        syscall                         #   putChar('\n');
        j loop
        lw $ra, 0($sp)
        addu $sp, $sp, 4
fn:     jr $ra                          # }


delay:  ble $a0, 0, end                 # for(; ms > 0; ms--) {
        li $v0, RESET_CORE_TIMER        #   resetCoreTimer();
        syscall
rct:    li $v0, READ_CORE_TIMER
        syscall                         #       print_int(counter);
        blt $v0, 20000, rct             #   while(readCoreTimer() < 20000);
        addi $a0, $a0, -1               #   ms--;
        j delay                         # }
end:    jr $ra