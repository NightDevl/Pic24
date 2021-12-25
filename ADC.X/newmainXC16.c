
// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCDIV           // Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV))
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

#include <p24fj128ga010.h>
#define POT 6 // 10k potentiometer connected to AN5 input
void initADC(void){
AD1PCFGbits.PCFG6 = 0;

AD1CON1 = 0x00E0;
AD1CSSL = 0;
AD1CON2 = 0;
AD1CON3 = 0x1F02;
AD1CON1bits.ADON = 1;
}
int readADC( int ch){
AD1CHS = ch; 
AD1CON1bits.SAMP = 1; 
while (!AD1CON1bits.DONE); 
return ADC1BUF0; 
} 
main (){
int a;
initADC(); 
TRISA = 0xff00; 
float vout=0.0;
float nhietdo=0.0;
while( 1){
a = readADC( POT); 
vout = 5*(float)(a)/1024;
nhietdo= (vout/10)*1000;
PORTA =  nhietdo;
} 
} 
