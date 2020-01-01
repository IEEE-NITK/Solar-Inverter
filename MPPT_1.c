#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#define _XTAL_FREQ 20000000 

void PWM_setup(){
 TRISC = 0b001111; //PORT C config
 PR2 = (_XTAL_FREQ/(120000)) - 1;   //Setting PWM frequency to 30kHz
 T2CON = 0b00000100; // Configuring timer 2
 CCP1CON = 0b10111100; // configuring comp-pwm
 CCPR1L = 20; //sens 1.56 --> 1% duty cycle
 PWM1CON = 0x00;   // setting dead band delay to zero
}

void ADC_setup(){
    ANSEL = 0b11110000; //pins 7,8,9,10 analog inputs
    ADCON0 = 0b10010001; //bit1 --> set to start
    ADCON1 = 0b01100000; //ADC clock select
}

int pv_I(){
    ADCON0 = 0b10010111;
    while(ADCON0bits.GO);
    if(ADCON0bits.GO == 0){
        return ((ADRESH << 2) | ADRESL);
    }
    else{
        return 0;
    }
}

int pv_V(){
    ADCON0 = 0b10011111;
    while(ADCON0bits.GO);
    if(ADCON0bits.GO == 0){
        return ((ADRESH << 2) | ADRESL);
    }
    else{
        return 0;
    }
       
}

int bat_I(){
    ADCON0 = 0b10010011;
    while(ADCON0bits.GO);
    if(ADCON0bits.GO == 0){
        return ((ADRESH << 2) | ADRESL);
    }
    else{
        return 0;
    }
}

int bat_V(){
    ADCON0 = 0b10011011;
    while(ADCON0bits.GO);
    if(ADCON0bits.GO == 0){
        return ((ADRESH << 2) | ADRESL);
    }
    else{
        return 0;
    }
}

int mppt(int pvi1, int pvv1 , int bati1, int batv1, int pvi0, int pvv0, int bati0, int batv0, int f){
    int vth = 0;
    int vmax = 0;
    if(f){
        CCPR1L += 2;
        return 0;
    }
    if( batv1 < vth ){
        
    }
    elif
    
    
    
}
void main(){
 
    int pv_i1 = 0;
    int pv_v1 = 0;
    int bat_i1 = 0;
    int bat_v1 = 0;
    int pv_i0 = 0;
    int pv_v0 = 0;
    int bat_i0 = 0;
    int bat_v0 = 0;
    int flag = 1;
    
    PWM_setup();
    ADC_setup();
    
    while(1){
        
        pv_i1 = pv_I();
        pv_v1 = pv_V();
        bat_i1 = bat_I();
        bat_v1 = bat_V();
        
        flag = mppt(pv_i1, pv_v1, bat_i1, bat_v1, pv_i0, pv_v0, bat_i0, bat_v0, flag);
        
        pv_i0 = pv_i1;
        pv_v0 = pv_v1;
        bat_i0 = bat_i1;
        bat_v0 = bat_v1;
        
    }

}

