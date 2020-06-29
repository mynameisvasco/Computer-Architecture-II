#include <detpic32.h>

int counter = 0;
unsigned char lastKey = 0;
int currentFreq = 10;

void show()
{
    int leftDigit = counter / 10;
    int rightDigit = counter % 10;
    printInt10(leftDigit);
    printInt10(rightDigit);
    putChar('\r');
}

void _int_(24) isr_uart1() {
    lastKey = getChar();
    if(lastKey == '1') {
        currentFreq = 10 * 1;
    } else if(lastKey == '2') {
        currentFreq = 10* 2;
    } else if(lastKey == '3') {
        currentFreq = 10*3;
    } else if(lastKey == '4') {
        currentFreq = 10*4;
    } else if (lastKey == '\n') {
        show();
    }
    IFS0bits.U1RXIF = 0;
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
    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
    while (1)
    {
        float ms = (1.0f /currentFreq)*1000;
        delay((int)ms);
        if (counter >= 100)
        {
            counter = 0;
        }
        else
        {
            counter++;
        }
    }

    return 0;
}