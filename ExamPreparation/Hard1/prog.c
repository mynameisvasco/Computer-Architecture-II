#include <detpic32.h>
#define GLOBAL_PRESC 5
#define GLOBAL_PR 62499
#define INITAL_TIME 400;
#define WHITE 0
#define BLACK 1

int digits[] = {0x3F, 0x06, 0x5B,
                0x4F, 0x66, 0x6D,
                0x7D, 0x07, 0x7F,
                0x6F, 0x77, 0x7C,
                0x39, 0x5E, 0x79, 0x71};

int whiteTimeLeft = INITAL_TIME;
int blackTimeLeft = INITAL_TIME;
short activeColor = WHITE;
short didEnd = 0;

void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < PBCLK / 1000)
            ;
    }
}

void _int_(24) isr_uart1()
{
    char c = getChar();
    /* 
    * When b key is pressed switch current active player 
    * WHen x key is pressed it's a checkmate
    * */
    if (c == 'b' & !didEnd)
    {
        if (activeColor == WHITE)
        {
            activeColor = BLACK;
            printStr("Black color is playing...\n");
        }
        else
        {
            activeColor = WHITE;
            printStr("White color is playing...\n");
        }
    }
    else if (c == 'x' && !didEnd)
    {
        if (activeColor == WHITE)
        {
            printStr("White color won the game...\n");
        }
        else
        {
            printStr("Black color won the game...\n");
        }
        didEnd = 1;
    }

    IFS0bits.U1RXIF = 0;
}

void _int_(8) isr_timer2()
{
    if (activeColor == WHITE)
    {
        whiteTimeLeft--;
    }
    IFS0bits.T2IF = 0;
}

void _int_(12) isr_timer3()
{
    if (activeColor == BLACK)
    {
        blackTimeLeft--;
    }
    IFS0bits.T3IF = 0;
}

int main()
{

    /* Configurar led */
    TRISE = (TRISE & 0xF0);

    /* Configurar display*/
    TRISB = (TRISB & 0x00FF);
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    /* Configurar timer das brancas */
    T2CONbits.TCKPS = GLOBAL_PRESC;
    PR2 = GLOBAL_PR;
    TMR2 = 0;
    T2CONbits.TON = 1;

    /* Configurar timer das pretas */
    T3CONbits.TCKPS = GLOBAL_PRESC;
    PR3 = GLOBAL_PR;
    TMR3 = 0;
    T3CONbits.TON = 1;

    /* Configurar a UART */
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    /* Configurar interrupções dos timers */
    IFS0bits.T2IF = 1;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 1;
    IFS0bits.T3IF = 1;
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 1;

    /* Configurar interrupções da UART */
    IEC0bits.U1RXIE = 1;
    IPC6bits.U1IP = 2;

    int ledsDigit = 4;
    int rightDisplayDigit = 0;
    int leftDisplayDigit = 0;
    short displayFlag = 1;

    EnableInterrupts();
    while (1)
    {

        if (!didEnd)
        {
            if (activeColor == WHITE)
            {
                ledsDigit = whiteTimeLeft / 100;
                rightDisplayDigit = whiteTimeLeft % 10;
                leftDisplayDigit = (whiteTimeLeft % 100) / 10;
            }
            else
            {
                ledsDigit = blackTimeLeft / 100;
                rightDisplayDigit = blackTimeLeft % 10;
                leftDisplayDigit = (blackTimeLeft % 100) / 10;
            }

            switch (ledsDigit)
            {
            case 4:
                LATE = (LATE & 0xF0) | 0x0F;
                break;

            case 3:
                LATE = (LATE & 0xF0) | 0x07;
                break;
            case 2:
                LATE = (LATE & 0xF0) | 0x03;
                break;
            case 1:
                LATE = (LATE & 0xF0) | 0x01;
                break;
            default:
                LATE = (LATE & 0xF0);
                break;
            }

            if (displayFlag)
            {
                LATB = (LATB & 0x00FF) | (digits[rightDisplayDigit] << 8);
                LATDbits.LATD5 = 1;
                LATDbits.LATD6 = 0;
            }
            else
            {
                LATB = (LATB & 0x00FF) | (digits[leftDisplayDigit] << 8);
                LATDbits.LATD5 = 0;
                LATDbits.LATD6 = 1;
            }

            displayFlag = !displayFlag;

            delay(1);
        }
    }
    return 0;
}