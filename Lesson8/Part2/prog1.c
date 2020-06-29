#include <detpic32.h>
#include "util.h"

void _int_(24) isr_uart1(void)
{
    putcUart(getc());
    IFS0bits.U1RXIF = 0; // Clear UART1 rx interrupt flag
}

int main(void)
{
    configUart(115200, 'N', 1); // default "pterm" parameters
    // with RX interrupt enabled
    IPC6bits.U1IP = 2;
    IPC6bits.U1IS = 1;
    IEC0bits.U1RXIE = 1;
    EnableInterrupts();
    while (1)
        ;
}