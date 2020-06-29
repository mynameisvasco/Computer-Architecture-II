#include <detpic32.h>
#include "util.h"

void _int_(12) isr_T3(void)
{
    //Reset T3IF
    putChar('3');

    IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1(void)
{
    //Reset T3IF
    putChar('1');
    IFS0bits.T1IF = 0;
}

int main(void)
{
    EnableInterrupts();

    T3CONbits.TCKPS = 4; // 1:32 prescaler
    PR3 = 62499;         // Fout = 20MHz / (16 * (62499 + 1)) = 20Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable interrupts

    //Configure Timer T1 (2Hz with interrupts disabled)
    T1CONbits.TCKPS = 7; // 1:256 prescaler
    PR1 = 39062;         // Fout = 20MHz / (256 * (39063 + 1)) = 2Hz
    TMR1 = 0;            // Reset timer T1 count register
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable interrupts

    while (1)
    {
    }

    return 0;
}