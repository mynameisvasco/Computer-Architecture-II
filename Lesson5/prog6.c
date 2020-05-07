#include <detpic32.h>
#include "../util.h"

int main(void)
{
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs
    
    TRISEbits.TRISE0 = 0; // RD0 configured as output
    LATEbits.LATE0 = 0;

    LATBbits.LATB15 = 1;    //LIGA O PONTO
    TRISBbits.TRISB4 = 1;  //desativa a entrada digital
    AD1PCFGbits.PCFG4 = 0; //configura a entrada como analógica

    AD1CHSbits.CH0SA = 4;  //escolhe a entrada 4, que é analógica
    AD1CON2bits.SMPI = 3; //faz interrupção após uma conversão consecutiva (SMPI = X - 1)

    AD1CON1bits.CLRASAM = 1; //clear
        
    AD1CON1bits.SSRC = 7; // Conversion Trigger

    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON1bits.ON = 1;    // Enable A/D Converter

    volatile int aux;
    
    while(1)
    {
        LATEbits.LATE0 = 1;
        AD1CON1bits.ASAM = 1; // começa a conversão
        while( IFS1bits.AD1IF == 0 );
        LATEbits.LATE0 = 0;

        int *p = (int *) (&ADC1BUF0);
        int i = 0;

        int val_ad = 0;
        int media = 0;
        int v = 0;

        if(i++ % 25 == 0)
        {
            for(i = 0; i < 4; i++)
            {
                media += p[i*4];
            }
            val_ad = media / 4;
            v  = (val_ad * 33 + 511) / 1023;
        }

        aux = ADC1BUF0;

        send2displays(v, 10);
        IFS1bits.AD1IF = 0;
        delay(10);
    }
}