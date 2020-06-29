#include <detpic32.h>
#include "../util.h"

void setPWM(unsigned int dutyCycle)
{

    OC1RS = 49999 / (100 / dutyCycle);
}

int main(void)
{

    T3CONbits.TCKPS = 2; // 1:4 prescaler
    PR3 = 49999;         // Fout = 20MHz / (4 * (49999 + 1)) = 100Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 0;   // Disable interrupts
    TRISB = TRISB & 0xFFDF;
    TRISBbits.TRISB5 = 1;
    OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1; // Use timer T2 as the time base for PWM generation
    setPWM(65);            // Ton constant
    OC1CONbits.ON = 1;     // Enable OC1 module

    while (1)
    {
        while (!IFS0bits.T3IF)
            ;
        while (PORTBbits.RB5 != 0)
            ;
        while (PORTBbits.RB5 == 0)
            ;
        resetCoreTimer();
        while (PORTBbits.RB5 != 0)
            ;
        int tempoOn = readCoreTimer() / 20;
        printInt10(tempoOn);
        putChar(' ');

        while (PORTBbits.RB5 == 0)
            ;
        while (PORTBbits.RB5 != 0)
            ;
        resetCoreTimer();
        while (PORTBbits.RB5 == 0)
            ;
        int tempoOff = readCoreTimer() / 20;
        printInt10(tempoOff);
        putChar(' ');

        int periodo = tempoOn + tempoOff;

        int frequencia = 1000000 / periodo;

        printInt10(frequencia);
        putChar('\n');
    }
}