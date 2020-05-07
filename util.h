#include <detpic32.h>
#include <string.h>

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

void putcUart(char byte2send) 
{
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void putsUart(char *string2send)
{
    int size = strlen(string2send);
    int i = 0;
    while(i <= (size - 1))
    {
        putcUart(string2send[i]);
        i += 1;
    }
}

char getc(void) {
    // If OERR == 1 then reset OERR
    if(U1STAbits.OERR == 1) 
    {
        U1STAbits.OERR = 0;
    }
    while(U1STAbits.URXDA == 0); // Wait while URXDA == 0
    // If FERR or PERR then
    if(U1STAbits.FERR || U1STAbits.PERR)
    {
        // read UxRXREG (to discard the character) and return 0
        char c = U1RXREG;
        return 0;
    } 
    return U1RXREG;
}

void configUart(unsigned int baud, char parity, unsigned int stopbits) 
{
    U1BRG = ((20000000 + 8 * baud) / (16 * baud)) - 1;
    switch(parity) 
    {
        case 'N':
            U1MODEbits.PDSEL = 0; 
            break;
        case 'E':
            U1MODEbits.PDSEL = 1; 
            break;
        case 'O':
            U1MODEbits.PDSEL = 2; 
            break;
    }
    switch(stopbits) 
    {
        case 1:
            U1MODEbits.STSEL = 0; 
        case 2:
            U1MODEbits.STSEL = 1;
            break;
        default:
            U1MODEbits.STSEL = 0;
            break;    
    }
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    //IFS0bits.U1TXIF = 0;
    U1MODEbits.ON = 1;
}
