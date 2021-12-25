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
#include <xc.h>
#include <p24fj128ga010.h>
void Timer1_Init(int period, int prescale){
T1CON = 0x00;
TMR1 = 0x00;
PR1 = period;
T1CONbits.TCS=0;
T1CONbits.TSYNC =0;
T1CONbits.TGATE=1;
T1CONbits.TCKPS=prescale;
T1CONbits.TON=1;
}
int main() {
TRISA = 0;
PORTA = 0;
int count=0;
int PRx = 31250;
Timer1_Init(PRx,2);
while (1)
{
    if(TMR1 == PR1){
        //PORTA++;
        count++;
        if(count==3)PORTA=0xff;
        if(count>10)
        {
            PORTA=~PORTA;
        }
        TMR1=0;
    }
}
return 0;
}