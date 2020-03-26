#include <detpic32.h>
#include "../util.h"

int main(void)
{
    TRISBbits.TRISB4 = 1;  //desativa a entrada digital
    AD1PCFGbits.PCFG4 = 0; //configura a entrada como analógica

    AD1CHSbits.CH0SA = 4;  //escolhe a entrada 4, que é analógica
    AD1CON2bits.SMPI = 3; //faz interrupção após uma conversão consecutiva (SMPI = X - 1)

    AD1CON1bits.CLRASAM = 1; //clear
        
    AD1CON1bits.SSRC = 7; // Conversion Trigger

    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON1bits.ON = 1;    // Enable A/D Converter

    while(1)
    {
        AD1CON1bits.ASAM = 1; // começa a conversão
    
        while( IFS1bits.AD1IF == 0 );

        int *p = (int *) (&ADC1BUF0);
        int i = 0;
        int val_ad = 0;
        int media = 0;
        int v = 0;
        for(i = 0; i < 4; i++)
        {
            media += p[i*4];
            // v = (val_ad * 33) / 1023;
            // average += v;
        }
        val_ad = media / 4;
        v  = (val_ad * 33 + 511) / 1023;

        int inteira = v / 10;
        int decimal = v % 10;

        printInt(inteira,10);
        //printInt(val_ad / 4, 10);
        putChar('.');
        printInt(decimal,10);
        putChar('\n');
        IFS1bits.AD1IF = 0;
        delay(1000);
    }
}