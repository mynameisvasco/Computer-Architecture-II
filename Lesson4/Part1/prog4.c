#include <detpic32.h>
#include "util.h"

int main(void)
{
    unsigned int segment;
    LATDbits.LATD6 = 1; // display high active
    LATDbits.LATD5 = 0; // display low inactive
    LATB = LATB & 0x80FF;
    TRISB = TRISB & 0x80FF; // configure RB8-RB14 as outputs
    TRISD = TRISD & 0xFF9F; // configure RD5-RD6 as outputs
    while (1)
    {
        LATDbits.LATD6 = !LATDbits.LATD6; //
        LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection
        segment = 256;
        int i;
        for (i = 0; i < 7; i++)
        {
            LATB = (LATB & 0x80FF) | segment; // send "segment" value to display
            delay(500);                       // wait 0.5 second
            segment = segment << 1;
        }
    }

    return 0;
}