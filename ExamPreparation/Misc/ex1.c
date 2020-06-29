#include <detpic32.h>

short displayFlag = 0;
int xico = 0;
int high = 0;
int low = 0;
int digits[] = {0x3F, 0x06, 0x5B, 0x4F,
                0x66, 0x6D, 0x7D, 0x07,
                0x7F, 0x6F, 0x77, 0x7C,
                0x39, 0x5E, 0x79, 0x71};

void _int_(24) isr_uart()
{
    char c = getChar();
    putChar(c);
    putChar('\n');
    IFS0bits.U1RXIF = 0;
}

void _int_(8) isr_timer2()
{
    if (xico >= 99)
    {
        xico = 0;
    }
    high = xico / 10;
    low = xico % 10;
    printInt10(xico);
    xico++;
    IFS0bits.T2IF = 0;
}

void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < PBCLK / 1000)
            ;
    }
}

int main()
{
    EnableInterrupts();

    /* Configurar UART */
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    /* Configurar interrupção da UART */
    IFS0bits.U1AEIF = 1;
    IPC6bits.U1IP = 1;
    IEC0bits.U1RXIE = 1;

    /* Configurar Timer */
    T2CONbits.TCKPS = 7;
    PR2 = 65535; //Fout = 1,2Hz
    TMR2 = 0;
    T2CONbits.TON = 1;

    /* Configurar interrupção do Timer 2 */
    IFS0bits.T2IF = 1;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 2;

    /* Configurar portos do display*/
    TRISB = (TRISB & 0x00FF);
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    while (1)
    {
        /* Atualizar os displays com uma freq de 100 Hz*/
        if (displayFlag)
        {
            LATB = (LATB & 0x00FF) | (digits[high] << 8);
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 1;
        }
        else
        {
            LATB = (LATB & 0x00FF) | (digits[low] << 8);
            LATDbits.LATD5 = 1;
            LATDbits.LATD6 = 0;
        }
        displayFlag = !displayFlag;

        delay(10);
    }
}