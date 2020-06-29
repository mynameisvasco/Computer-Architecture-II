#include <detpic32.h>
#include "util.h"

int main(void)
{
    //Configure Timer T3 (2Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler
    PR3 = 39062;         // Fout = 20MHz / (256 * (39063 + 1)) = 2Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
    IEC0bits.T3IE = 0;   // Disable interrupts

    int i = 0;
    while (1)
    {
        while (!IFS0bits.T3IF)
            ;
        i++;
        //Reset T3IF
        IFS0bits.T3IF = 0;
        printInt10(i);
    }
}