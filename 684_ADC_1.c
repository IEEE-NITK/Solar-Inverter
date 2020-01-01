#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
//#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
//#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#define _XTAL_FREQ 20000000 

void main(){
    int i = 0;
    int adch = 0;
    int adcl = 0;
    int adch_th = 0b11;
    int adcl_th = 0;
    
    TRISC = 0b000111; //RC5,4,3 O/P; RC 2,1,0 I/P
    ANSEL = 0b01110000; // Analog pin select 
    ADCON0 = 0b10011001; //bit1 --> set to start
    ADCON1 = 0b01100000; //ADC clock select
    PORTCbits.RC4 = 1;
    
    while(1){ 
        i = 0;
        adch = 0;
        adcl = 0;
        
        while(i<1000){//wait for acquisition time
            i++;
        }
        
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        
        if(ADCON0bits.GO == 0){
            adch = ADRESH;
            adcl = ADRESL;
        
        if(adch >= adch_th){
            PORTCbits.RC3 = 1;
        }
        else{
           PORTCbits.RC3 = 0; 
        }
        }
        
    
    }
    
    
    
   
}