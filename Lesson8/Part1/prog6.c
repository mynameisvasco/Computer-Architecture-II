#include <detpic32.h>
#include "util.h"

int main(void)
{
    configUart(9600, 'N', 1);
    unsigned int time = 0;
    while (1)
    {
        while (U1STAbits.TRMT != 1)
            ; // Wait until TRMT == 1
        resetCoreTimer();
        putsUart("12345678\n");
        time = readCoreTimer() * 50;
        getChar();
        putChar('t');
        printInt10(time);
        putChar('\n');
        delay(1000);
    }
}