#include <detpic32.h>
#include "../util.h"

int main(void)
{
    LATD = LATD & 0xFF9F;
    PORTB = PORTB & 0xFFF0;
    TRISB = TRISB & 0x80F0; // configure RB0 to RB3 as inputs & configure RB8 to RB14
    TRISB = TRISB | 0x000F;
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs
    LATDbits.LATD5 = 1; // Select display low
    LATDbits.LATD6 = 0;
    while(1)
    {
        int dip_switch = PORTB & 0x000F; // read dip-switch
        int display_value = display7codes(dip_switch);
        LATB = display_value & 0x80FF;
        printInt10(dip_switch);
    }
    return 0;
} 
