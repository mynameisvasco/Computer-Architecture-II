#include <detpic32.h>
#include "../util.h">

int main(void)
{
    LATEbits.LATE0 = 0;
    TRISEbits.TRISE0 = 0; // RD0 configured as output
    while(1)
    {
        delay(500); // half period = 0.5s
        LATEbits.LATE0 = !LATEbits.LATE0;
    }
    return 0;
}
