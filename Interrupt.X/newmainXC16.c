
// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "p24FJ128GA010.h"
int count = 0;
void Timer1_Init(void){
    T1CON = 0x00;
    TMR1 = 0x00;
    PR1 = 62500;
    _T1IP = 0x06;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TCS = 0;
    T1CONbits.TSYNC = 0;
    T1CONbits.TCKPS = 2;    
    T1CONbits.TON = 1;
}

void _ISR _T1Interrupt(void){
    _T1IF = 0;
    count++;
    PORTA = count;       
}

void INT0_Interrupt_Init(){
    INTCON2bits.INT0EP = 1;
    _INT0IP = 5;
    _INT0IF = 0;
    _INT0IE = 1;
}
int mode =0;
void _ISR _INT0Interrupt(void){
    _INT0IF = 0;
    mode++;
    if(mode==3)mode=0;
    if(mode ==1){   
        _AD1IE = 0;
        Timer1_Init();
    }
    if(mode==2){
        _T1IE =0;
        ADC_Init();
    }
    AD1CHS = 6;
    AD1CON1bits.SAMP = 1;
}

void ADC_Init() {
    AD1PCFGbits.PCFG6 = 0;
    AD1CON1 = 0x00E0;
    AD1CSSL = 0;
    AD1CON2 = 0;
    AD1CON3 = 0x1F02;
    AD1CON1bits.ADON = 1;
    _AD1IP = 6;
    _AD1IF = 0;
    _AD1IE = 1;
}

int code;
void _ISR _ADC1Interrupt(void){
    _AD1IF = 0;
    code = ADC1BUF0;
    float vout = 0.0;
    float nhietdo = 0.0;
    vout = 5*(float)(code)/1024;
    nhietdo= (vout/10)*1000;
    PORTA = nhietdo;  
}
void main(){
    TRISA = 0;   
    INT0_Interrupt_Init();
    //Timer1_Init();
    //ADC_Init();  
    while(1){
        
    }
}