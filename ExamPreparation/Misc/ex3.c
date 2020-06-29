#include <detpic32.h>
#define PR2_VAL 999
#define OC1RS_VAL 500

int totalTime = 0;
int totalActiveTime = 0;

void _int_(8) isr_timer2()
{
    //
    IFS0bits.T2IF = 0;
}

int main()
{
    EnableInterrupts();
    /* Configurar Timer */
    T2CONbits.TCKPS = 5;
    PR2 = PR2_VAL;
    TMR2 = 0;
    T2CONbits.TON = 1;

    /* Configurar OC2 */
    // PR / OCRS = dutyCycle
    OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
    OC1RS = OC1RS_VAL;
    OC1CONbits.ON = 1;

    /* Configurar interrupção do timer */
    IFS0bits.T2IF = 1;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 2;

    while (1)
        ;

    return 0;
}
