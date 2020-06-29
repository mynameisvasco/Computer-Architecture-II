#include <detpic32.h>
#define BUF_SIZE 8
#define MASK 7
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int size;
} circularBuffer;

volatile circularBuffer txb;
volatile circularBuffer rxb;

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
}

void flushTXb()
{
    txb.size = 0;
    txb.head = 0;
    txb.tail = 0;
}

void flushRXb()
{
    rxb.size = 0;
    rxb.head = 0;
    rxb.tail = 0;
}

char myGetChar()
{
    if (rxb.size > 0)
    {
        DisableUart1RxInterrupt();
        char c = rxb.data[rxb.tail];
        rxb.head = (rxb.head + 1) & MASK;
        rxb.size--;
        EnableUart1RxInterrupt();
        return c;
    }

    return 0;
}

void myPutChar(char c)
{
    while (txb.size == BUF_SIZE)
        ;
    txb.data[txb.tail] = c;
    txb.tail = (txb.tail + 1) & MASK;
    DisableUart1TxInterrupt();
    txb.size++;
    EnableUart1TxInterrupt();
}

void myPutString(char *str)
{
    while (*str != '\0')
    {
        myPutChar(*str);
        str++;
    }
}

void _int_(24) isr_uart1()
{
    if (IFS0bits.U1TXIF == 1)
    {
        if (txb.size > 0)
        {
            while (U1STAbits.UTXBF == 1)
                ;
            U1TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) & MASK;
            txb.size--;
        }
        if (txb.size == 0)
        {
            DisableUart1TxInterrupt();
        }

        IFS0bits.U1TXIF = 0;
    }

    if (IFS0bits.U1RXIF == 1)
    {
        while (rxb.size == BUF_SIZE)
            ;
        while (U1STAbits.URXDA == 0)
            ;
        rxb.data[rxb.tail] = U1RXREG;
        rxb.tail = (txb.tail + 1) & MASK;
        rxb.size++;

        if (rxb.size == 0)
        {
            DisableUart1RxInterrupt();
        }
        IFS0bits.U1RXIF = 0;
    }
}

void configUart(unsigned int baudrate, char parity, int databits, int stopbits)
{
    U1BRG = ((PBCLK + databits * baudrate) / (16 * baudrate)) - 1;
    U1MODEbits.BRGH = 0;
    switch (parity)
    {
    case 'O':
        U1MODEbits.PDSEL = 2;
        break;
    case 'E':
        U1MODEbits.PDSEL = 1;
        break;
    case 'N':
        U1MODEbits.PDSEL = 0;
        break;
    }

    if (stopbits == 1 || stopbits == 2)
    {
        U1MODEbits.STSEL = stopbits - 1;
    }

    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
    U1MODEbits.ON = 1;

    /* Interrupções */
    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
    IEC0bits.U1TXIE = 1;
    IFS0bits.U1RXIF = 1;
    IFS0bits.U1TXIF = 1;
}

int main()
{
    flushRXb();
    flushTXb();
    EnableInterrupts();
    /* Configurar uart */
    configUart(115200, 'N', 8, 1);
    while (1)
    {
        myPutChar(myGetChar());
    }
    return 0;
}