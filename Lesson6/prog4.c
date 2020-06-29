#include <detpic32.h>
#include "util.h"

volatile unsigned char voltage = 0; // Global variable

void _int_(27) isr_adc(void)
{
    int *p = (int *)(&ADC1BUF0);
    int val_ad = 0;
    int media = 0;
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        media += p[i * 4];
    }
    val_ad = media / 8;
    voltage = (val_ad * 33 + 511) / 1023;
    //printInt(ADC1BUF0, 16 | 3 << 16);

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

int main(void)
{

    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F; //configure RD5 to RD6 as outputs
    LATBbits.LATB15 = 1;    //LIGA O PONTO

    TRISBbits.TRISB4 = 1;  //desativa a entrada digital
    AD1PCFGbits.PCFG4 = 0; //configura a entrada como analógica

    AD1CHSbits.CH0SA = 4; //escolhe a entrada 4, que é analógica
    AD1CON2bits.SMPI = 7; //faz interrupção após uma conversão consecutiva (SMPI = X - 1)

    AD1CON1bits.CLRASAM = 1; //clear

    AD1CON1bits.SSRC = 7; // Conversion Trigger

    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON1bits.ON = 1;    // Enable A/D Converter

    IPC6bits.AD1IP = 2;

    IEC1bits.AD1IE = 1;

    IFS1bits.AD1IF = 0; // Reset AD1IF flag

    EnableInterrupts();
    AD1CON1bits.ASAM = 1; // começa a conversão
    int i = 0;

    while (1)
    {
        delay(10);
        if (i++ == 25)
        {
            AD1CON1bits.ASAM = 1; // começa a conversão
            i = 0;
        }
        send2displays(voltage, 10);
    }

    return 0;
}
