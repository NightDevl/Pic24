#include "xc.h"
#include <p24fj128ga010.h>
#include <stdio.h>
#include <string.h>
// CONFIG2
#pragma config POSCMOD = NONE
#pragma config OSCIOFNC = OFF
#pragma config FCKSM = CSDCMD
#pragma config FNOSC = FRCDIV
#pragma config IESO = ON
// CONFIG1
#pragma config WDTPS = PS32768
#pragma config FWPSA = PR128
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF
#pragma config ICS = PGx2
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF
void delayms(unsigned t) {
T1CON = 0x8000; // enable tmr1, Tcy, 1:1
while (t--) {
TMR1 = 0;
while (TMR1 < 2000);
}
} // Delayms
int main() {
int led_on = 0;
int led_off = 0;
int temp;
TRISA = 0; // all PORTA pins output
TRISD = 0xffff;
PORTA = 0x00;
while (1) {
if ((PORTD & 0x200) == 0)   // S3 : RD9
{ 
    while ((PORTD & 0x200) == 0);
    led_on=1;
    if(led_off==3){led_off=4;led_on=0;}
}
    
if ((PORTD & 0x2000) == 0)  // s2 : RD13
    {
        while ((PORTD & 0x2000) == 0);    
        if(led_on==1){led_on=2; led_off=0;}
        //else led_on=0;
        if(led_off==2){led_off=3;led_on=0;}
    } 
if ((PORTD & 0x80) == 0)    //s4 : RD7
        {
            while ((PORTD & 0x80) == 0);
            if(led_on==2){led_on=3; led_off=0;}
            //else led_on=0;
            if(led_off==1){led_off=2; led_on=0;}
        }
if ((PORTD & 0x40) == 0)        // s1:RD6
            {
                while ((PORTD & 0x40) == 0);
                
                led_off=1;
                if(led_on==3){led_on=4; led_off=0;}
                //else led_on=0;
            }

delayms(100);
if (led_on == 4) {
temp = 0xff;
}
if (led_off == 4) {
temp = 0x00;
}
PORTA = temp;
delayms(100);
}
return 0;
}