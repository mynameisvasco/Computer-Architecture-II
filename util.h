#include <detpic32.h>

unsigned int display7codes(int i)
{
    int arr[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; 
    return arr[i];
}

void delay(unsigned int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

int send2displays(unsigned int value, int base)
{
    static char displayFlag = 0;
    int high = value / base;
    int low = value % base;

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

void configAdc()
{
    TRISBbits.TRISB4 = 1;  //desativa a entrada digital
    AD1PCFGbits.PCFG4 = 0; //configura a entrada como analógica

    AD1CHSbits.CH0SA = 4;  //escolhe a entrada 4, que é analógica
    AD1CON2bits.SMPI = 7; //faz interrupção após uma conversão consecutiva (SMPI = X - 1)

    AD1CON1bits.CLRASAM = 1; //clear
        
    AD1CON1bits.SSRC = 7; // Conversion Trigger

    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON1bits.ON = 1;    // Enable A/D Converter

    IPC6bits.AD1IP = 2;

    IEC1bits.AD1IE = 1;

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}
