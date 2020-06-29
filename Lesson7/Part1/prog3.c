#include <detpic32.h>
#include "util.h"

int i = 0;
int flag = 0;

void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
{
    //Reset T3IF
    flag = !flag;
    if (flag)
    {
        i++;
        printInt10(i);
    }
    IFS0bits.T3IF = 0;
}

int main(void)
{
    EnableInterrupts();
    //Configure Timer T3 (2Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler
    PR3 = 39062;         // Fout = 20MHz / (256 * (39063 + 1)) = 2Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable interrupts
    while (1)
    {
    }
}