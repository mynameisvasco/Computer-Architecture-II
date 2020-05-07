#include <detpic32.h>
#include "../util.h"

volatile int voltage = 0; // Global variable

void _int_(4) isr_T1(void)
{
    // Start A/D conversion
    AD1CON1bits.ASAM = 1;
    // Reset T1IF flag
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
    // Send "voltage" global variable to displays
    send2displays(voltage, 10);
    // Reset T3IF flag
    IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc(void)
{
    // Calculate buffer average (8 samples)
    // Calculate voltage amplitude
    // Convert voltage amplitude to decimal. Copy it to "voltage"
    int *p = (int *)(&ADC1BUF0);
    int val_ad = 0;
    int media = 0;
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        media += p[i * 4];
    }
    val_ad = media / 8;
    voltage = (val_ad * 33 + 511) / 1023;
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

int main(void)
{
    configAdc();

    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs
    LATBbits.LATB15 = 1;    //LIGA O PONTO

    T3CONbits.TCKPS = 2; // 1:4 prescaler
    PR3 = 49999;         // Fout = 20MHz / (4 * (49999 + 1)) = 100Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable interrupts

    //Configure Timer T1 (2Hz with interrupts disabled)
    T1CONbits.TCKPS = 2; // 1:64 prescaler
    PR1 = 78124;         // Fout = 20MHz / (64 * (78124 + 1)) = 4Hz
    TMR1 = 0;            // Reset timer T1 count register
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable interrupts

    IFS0bits.T1IF = 0;
    IFS0bits.T3IF = 0;

    EnableInterrupts();

    while (1)
        ;

    return 0;
}