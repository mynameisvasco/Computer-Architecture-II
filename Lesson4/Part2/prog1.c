#include <detpic32.h>
#include "../../util.h":

int send2displays(unsigned int value)
{
    static char displayFlag = 0;
    int high = value / 16;
    int low = value % 16;

    high = display7codes(high);
    low = display7codes(low);

    if(displayFlag)
    {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0; 
        LATB = (LATB & 0x80FF) | (high << 8);
    }
    else
    {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1; 
        LATB = (LATB & 0x80FF) | (low << 8);
    }
    displayFlag = !displayFlag;
    return 0;
}

int main(void)
{
    TRISB = TRISB & 0x80FF; //configure RB8 to RB14 as outputs
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs

    int counter = 0;
    while(1)
    {
       int i = 0;
		do {
			delay(10); // wait 10 ms
			send2displays( counter );
		} while(++i < 20); // 200 ms = 5 Hz
		counter++;
    }
    return 0;
}