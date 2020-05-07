#include <detpic32.h>
#include "../../util.h"

int main(void) 
{
    int baudrate = 115200;
    U1BRG = ((20000000 + 8 * baudrate) / (16 * baudrate)) - 1;
    U1MODEbits.PDSEL = 0; //8 data bits without parity
    U1MODEbits.STSEL = 0; //1 stop bit
    U1STAbits.UTXEN = 1;
    //IFS0bits.U1TXIF = 0;
    U1MODEbits.ON = 1;
    while(1) 
    {
        putsUart("THIS IS A TEST\n");
        delay(500);    
    }
}