#include <detpic32.h>
#include <string.h>
#include "../util.h"

#define BUF_SIZE 8
#define INDEX_MASK 7
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

void comDrv_flushRx(void)
{
    // Initialize variables of the reception buffer
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
}

void comDrv_flushTx(void)
{
    // Initialize variables of the reception buffer
    txb.head = 0;
    txb.tail = 0;
    txb.count = 0;
}

void comDrv_putc(char ch)
{
    while (txb.count == BUF_SIZE)
    {
    }                        // Wait while buffer is full
    txb.data[txb.tail] = ch; // Copy character to the transmission
    // buffer at position "tail"
    txb.tail = (txb.tail + 1) & INDEX_MASK;
    DisableUart1TxInterrupt(); // Begin of critical section
    txb.count++;
    EnableUart1TxInterrupt(); // End of critical section
}

void comDrv_puts(char *s)
{
    while (*s != '\0')
    {
        comDrv_putc(*s);
        s++;
    }
}

char comDrv_getc(char *ch)
{
    if (rxb.count != 0)
    {
        DisableUart1RxInterrupt();
        *ch = rxb.data[rxb.head];
        rxb.count--;
        rxb.head = (rxb.head + 1) & INDEX_MASK;
        EnableUart1RxInterrupt();
        return 1;
    }
    return 0;
}

void _int_(24) isr_uart1(void)
{
    if (IFS0bits.U1TXIF == 1)
    {
        if (txb.count > 0)
        {
            while (U1STAbits.UTXBF == 1)
                ;
            // Copy character pointed by "head" to U1TXREG register
            U1TXREG = txb.data[txb.head];
            // Increment "head" variable (mod BUF_SIZE)
            txb.head = (txb.head + 1) & INDEX_MASK;
            txb.count--;
        }
        if (txb.count == 0)
        {
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF == 0;
    }
    if (IFS0bits.U1RXIF == 1)
    {
        while (U1STAbits.URXDA == 0)
            ;
        rxb.data[rxb.tail] = U1RXREG;
        rxb.tail = (rxb.tail + 1) & INDEX_MASK;
        if (rxb.count < BUF_SIZE)
        {
            rxb.count++;
        }
        else
        {
            rxb.head = (rxb.head + 1) & INDEX_MASK;
        }
        IFS0bits.U1RXIF == 0;
    }
}

int main(void)
{
    configUart(115200, 'N', 1);
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    comDrv_puts("PIC32 UART Device-driver\n");
    char c1;
    while (1)
    {
        if (comDrv_getc(&c1))
        {
            comDrv_putc(c1);
        }
    }
}