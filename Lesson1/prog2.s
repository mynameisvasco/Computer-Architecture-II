        .data
        .equ getChar,2 
        .equ putChar, 3
        .text
        .globl main
main:   li $t0, 0
wh:     li $v0, getChar
        syscall
        move $v0, $t0
        beq $t0, '\n', wh   
        move $a0, $t0
        li $v0, putChar
        syscall
        bne $t0, '\n', wh
        li $v0, 0
        jr $ra