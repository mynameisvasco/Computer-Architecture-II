#include <detpic32.h>
#include "../util.h"

int main(void)
{
    LATEbits.LATE0 = 0;
    TRISEbits.TRISE0 = 0;
    LATEbits.LATE1 = 0;
    TRISEbits.TRISE1 = 0;
    LATEbits.LATE1 = 0;
    TRISEbits.TRISE1 = 0;
    LATEbits.LATE2 = 0;
    TRISEbits.TRISE2 = 0;
    LATEbits.LATE3 = 0;
    TRISEbits.TRISE3 = 0;

    while(1)
    {
        int i;
        for(i = 0x0000; i < 0x1111; i++)
        {   
            LATE = i;
            delay(250); 
        }
    }
    return 0;
}