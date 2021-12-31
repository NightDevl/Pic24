
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "p24FJ128GA010.h"
#include <stdio.h>
#include <string.h>
#define CTS _RF12 // Clear To Send, input, HW handshake
#define RTS _RF13 // Request To Send, output, HW handshake
#define BRATE 68
#define U_ENABLE 0x8008
#define U_TX 0x0400
#define BACKSPACE 0x8
#define BUF_SIZE 128

char tr[BUF_SIZE];
int index = 0;
int flag = 0;
float temperature = 0.0;
float potentiometer = 0.0;

void initU2( void){
    U2BRG = BRATE;
    U2MODE = U_ENABLE;
    U2MODEbits.BRGH = 1;
    U2STA = U_TX;
    _TRISF13 = 0;
    _TRISF12 = 1;
    RTS = 1;    
} // initU2

int putU2( int c) {
    while (CTS);
    while ( U2STAbits.UTXBF);
    U2TXREG = c;
    return c;
} // putU2

void putsU2( char *s) {
    while( *s)
    putU2( *s++);
} // putsU2

void U2_Interrupt_Init(){
    U2STAbits.URXISEL = 0;
    _U2RXIP = 4;
    _U2RXIF = 0;
    _U2RXIE = 1;    
    
}

void _ISR _U2RXInterrupt(void){
    _U2RXIF = 0;
    char c = U2RXREG;
    tr[index] = c;
    putU2(tr[index]);
    index++;
    if(index > BUF_SIZE)
        index = 0;
    if(c == '\r'){         
        if (strcmp(tr,"hello\r") == 0){
            putsU2("My name is Khoa\n\r");            
        }else if (strcmp(tr,"temperature\r") == 0){
            initTemperature();
            putsU2("Nhiet do: \r\n");            
            int temp = (int)(temperature*10);
            putU2(temp/100+48);
            putU2((temp%100)/10+48);
            putU2('.');
            putU2((temp%100)%10+48);
            putsU2("doC\r\n");
        }else if(strcmp(tr,"potentiometer\r")==0){
            putsU2("Vout cua bien tro: \r\n");
            initPotentiometer();
            int v = (int)(potentiometer*100);
            putU2(v/100+48);
            putU2('.');
            putU2((v%100)/10+48);
            putU2((v%100)%10+48);
            putsU2("doC\r\n");
        }
        else{        
            putsU2("I don't know\n\r");            
        }
        for(index = 0; index < BUF_SIZE; index++)
            tr[index] = '\0';
        index = 0;
    }
    PORTA = index;
}

void initTemperature() {
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1 = 0x00E0;
    AD1CSSL = 0;
    AD1CON2 = 0;
    AD1CON3 = 0x1F02;
    AD1CON1bits.ADON = 1;
    int code;
    code = readADC(4);
    temperature = (5*(float)(code)*1000/1024-500)/10;
}
void initPotentiometer() {
    AD1PCFGbits.PCFG5 = 0;
    AD1CON1 = 0x00E0;
    AD1CSSL = 0;
    AD1CON2 = 0;
    AD1CON3 = 0x1F02;
    AD1CON1bits.ADON = 1;
    int code;
    code = readADC(5);
    potentiometer = (5*(float)(code)/1024);
}
int readADC(int ch) {
    AD1CHS = ch; // 1. select analog input channel
    AD1CON1bits.SAMP = 1; // 2. start sampling
    while (!AD1CON1bits.DONE); // 3. wait for the conversion to complete
    return ADC1BUF0; // 4. read the conversion result
} // readADC
int main() {
    char s[BUF_SIZE];
    TRISA = 0;
    initU2();
    putsU2( "THUC HANH VI DIEU KHIEN!\n\r");
    U2_Interrupt_Init(); 
    //initADC();
    while(1){  
    }
}
