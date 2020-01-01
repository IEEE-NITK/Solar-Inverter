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
    //long int PWM_freq = 30000 ;
    //int TMR2PRESCALE = 1;
   
    TRISC = 0b001111; //PORT C config
    PR2 = (_XTAL_FREQ/(120000)) - 1;
    
    CCP1CON = 0b10111100; // configuring comp-pwm
    CCPR1L = 20; //sens 1.56 --> 1% duty cycle
    PWM1CON = 0x00;   // setting dead band delay to zero
    T2CON = 0b00000100; // Configuring timer 2
   
    
    
    
        /*
    ANSEL = 0x00; // Analog pin select
    ADCON0 = 0b10011111; 
    ADCON1 = 0b0110000;
    */
}