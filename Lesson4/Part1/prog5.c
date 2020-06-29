#include <detpic32.h>
#include "util.h"

int main(void)
{
    TRISB = TRISB | 0x000F;
    LATDbits.LATD6 = 0;     // disable display high
    LATDbits.LATD5 = 1;     // Select display low
    TRISB = TRISB & 0x80FF; // configure RB0 to RB3 as inputs & configure RB8 to RB14
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs
    while (1)
    {
        int dip_switch = PORTB & 0x000F; // read dip-switch
        int display_value = display7codes(dip_switch);
        printInt10(display_value);
        LATB = (LATB & 0x80FF) | (display_value << 8);
    }
    return 0;
}