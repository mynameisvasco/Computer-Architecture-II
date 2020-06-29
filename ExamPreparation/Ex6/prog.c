#include <detpic32.h>
#include <math.h>
int counter = 0;
unsigned char lastKey = 0;
int currentFreq = 10;
int currentK = 0;
int highDigit = 0;
int lowDigit = 0;
static char displayFlag = 0;
int displayCodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                      0x66, 0x6D, 0x7D, 0x07,
                      0x7F, 0x6F, 0x77, 0x7C,
                      0x39, 0x5E, 0x79, 0x71};

void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < PBCLK / 1000)
            ;
    }
}

void show()
{
    int leftDigit = counter / 10;
    int rightDigit = counter % 10;
    printInt10(leftDigit);
    printInt10(rightDigit);
    putChar('\r');
}

void _int_(24) isr_uart1()
{
    short changedFreq = 0;
    lastKey = getChar();
    if (lastKey == '1')
    {
        currentFreq = 10 * 1;
        changedFreq = 1;
    }
    else if (lastKey == '2')
    {
        currentFreq = 10 * 2;
        changedFreq = 1;
    }
    else if (lastKey == '3')
    {
        currentFreq = 10 * 3;
        changedFreq = 1;
    }
    else if (lastKey == '4')
    {
        currentFreq = 10 * 4;
        changedFreq = 1;
    }
    else if (lastKey == '\n')
    {
        show();
    }
    if (changedFreq != 0)
    {
        PR3 = ((PBCLK / currentK) / (currentFreq)) - 1;
        changedFreq = 0;
    }
    IFS0bits.U1RXIF = 0;
}

void _int_(12) isr_timer3()
{
    if (counter >= 99)
    {
        counter = 0;
    }
    else
    {
        counter++;
    }
    IFS0bits.T3IF = 0;
}

int main()
{
    EnableInterrupts();

    //UART configuration
    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    //Timer 3 configuration
    T3CONbits.TCKPS = 5;
    currentK = pow(2, 5);
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;

    //Configure 7seg display
    TRISB = TRISB & 0x00FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    while (1)
    {
        highDigit = displayCodes[counter / 10];
        lowDigit = displayCodes[counter % 10];

        if (displayFlag)
        {
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 1;
            LATB = (LATB & 0x00FF) | (highDigit << 8);
        }
        else
        {
            LATDbits.LATD5 = 1;
            LATDbits.LATD6 = 0;
            LATB = (LATB & 0x00FF) | (lowDigit << 8);
        }
        displayFlag = !displayFlag;
        delay(20);
    }

    return 0;
}