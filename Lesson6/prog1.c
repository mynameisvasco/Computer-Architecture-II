#include <detpic32.h>
#include "../util.h"

void _int_ (27) isr_adc(void){

    printInt(ADC1BUF0, 16 | 3 << 16);
    AD1CON1bits.ASAM = 1; // começa a conversão
    IFS1bits.AD1IF = 0; // Reset AD1IF flag

}

int main(void){

    TRISBbits.TRISB4 = 1;  //desativa a entrada digital
    AD1PCFGbits.PCFG4 = 0; //configura a entrada como analógica

    AD1CHSbits.CH0SA = 4;  //escolhe a entrada 4, que é analógica
    AD1CON2bits.SMPI = 0; //faz interrupção após uma conversão consecutiva (SMPI = X - 1)

    AD1CON1bits.CLRASAM = 1; //clear
        
    AD1CON1bits.SSRC = 7; // Conversion Trigger

    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON1bits.ON = 1;    // Enable A/D Converter

    IPC6bits.AD1IP = 2;

    IEC1bits.AD1IE = 1;

    IFS1bits.AD1IF = 0; // Reset AD1IF flag

    EnableInterrupts();
    AD1CON1bits.ASAM = 1; // começa a conversão

    while(1){
       
    }
    
}

