#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#include <math.h>
#define _XTAL_FREQ 20000000 


int g_d0 = 140; //D(n-1)
int g_d1 = 0;  //D(n)
int g_del_d0 = 0; //delD(n-1)
int g_del_d1 = 10;//delD(n)
int g_k = 3; //TBD
int g_alpha = 2; //TBD
int pv_i1 = 0;
int pv_v1 = 0;
int bat_i1 = 0;
int bat_v1 = 0;
int pv_i0 = 0;
int pv_v0 = 0;
int bat_i0 = 0;
int bat_v0 = 0;

//int i = 0;
//int avg = 0;


void PWM_setup(){
 TRISC = 0b001111; //PORT C config
 PR2 = (_XTAL_FREQ/(120000)) - 1;   //Setting PWM frequency to 30kHz //110000
 T2CON = 0b00000100; // Configuring timer 2
 CCP1CON = 0b10111100; // configuring comp-pwm
 CCPR1L = 160 -  g_d0; //sens 1.56 --> 1% duty cycle
 PWM1CON = 0x00;   // setting dead band delay to zero
}

void ADC_setup(){
    ANSEL = 0b11110000; //pins 7,8,9,10 analog inputs
    ADCON0 = 0b00010001; //bit1 --> set to start
    ADCON1 = 0b00100000; //ADC clock select
}

int pv_I(){
    int i = 0;
    int avg = 0;
    
    for(i = 0;i < 100;i++){
        ADCON0 = 0b00010111;
       
        while(ADCON0bits.GO);
        
        if(ADCON0bits.GO == 0){
            avg = avg + ((ADRESH << 2) | ADRESL);
        }
       // __delay_us(10);
    }
    return (int)avg/100; 
}



int pv_V(){
    
    int i = 0;
    int avg = 0;
    
    for(i = 0;i < 100;i++){
        ADCON0 = 0b00011111;
       
        while(ADCON0bits.GO);
        
        if(ADCON0bits.GO == 0){
            avg = avg + ((ADRESH << 2) | ADRESL);
        }
      // __delay_us(10); 
    }
    return (int)avg/100; 
}

int bat_I(){
    
    int i = 0;
    int avg = 0;
    
    for(i = 0;i < 100;i++){
        ADCON0 = 0b00010011;
       
        while(ADCON0bits.GO);
        
        if(ADCON0bits.GO == 0){
            avg = avg + ((ADRESH << 2) | ADRESL);
}
        //__delay_us(10);
}
    return (int)avg/100; 
}

int bat_V(){
   
   int i = 0;
   int avg = 0;
   
    for(i = 0;i < 100;i++){
        ADCON0 = 0b00011011;
       
        while(ADCON0bits.GO);
        
        if(ADCON0bits.GO == 0){
            avg = avg + ((ADRESH << 2) | ADRESL);
}
   // __delay_us(10);
    }
    return (int)avg/100; 
}

void mppt(int f){
    int vth = 0; // voltage limit for constant current
    int vmax = 0; // maximum battery voltage
    int bat_i_max = 0; // maximum charging current 
   
    if(bat_i1 > 0 ){  //limiting to less than maximum battery current //bat_i_max
        
        if(f){                  //startup condition 
            __delay_ms(1000);
           CCPR1L -= 10; //initial perturbation
            __delay_ms(1000);
            }
        
        else if((CCPR1L < 160)&&(CCPR1L > 0)){               //max duty cycle
          
            if( bat_v1 > 0 ){    //vth      // maximize bat current
                
                if(((bat_i1 > bat_i0)&&(g_del_d1 > 0))||((bat_i1 < bat_i0 )&&(g_del_d1 < 0))){
                    
                    g_del_d1 = (int)(g_del_d0/g_alpha + g_k); // alpha and k
                    g_d1 = g_d0 + g_del_d1;
                    CCPR1L = 160 - g_d1; 
                    g_del_d0 = g_del_d1;
                    g_d0 = g_d1;                                                         
                }
                else if(((bat_i1 > bat_i0)&&(g_del_d1 < 0))||((bat_i1 < bat_i0 )&&(g_del_d1 > 0))){
                    
                    g_del_d1 = (int)(g_del_d0/2 - g_k); // alpha and k
                    g_d1 = g_d0 + g_del_d1;
                    CCPR1L = 160 - g_d1; 
                    g_del_d0 = g_del_d1;
                    g_d0 = g_d1; 
                    
                }
                
           
            }
            /*
            else if((bat_v1 >= vth) && (bat_v1 < vmax)){     //vth and vmax // constant voltage
                CCPR1L = (int)(bat_v1/pv_v1);                          //CHECK

            }
             */
            
         

            else if(bat_v1 >= 600){   //vmax                      //float charge
                CCP1CON = 0b10110000;

            }

        }
        else{
            
            CCPR1L = 80;
        }
  
}
    /*
    else if(bat_i1 >= bat_i_max){ //reverse MPPT   // reducing charging current
                if(((bat_i1 > bat_i0)&&(g_del_d1 > 0))||((bat_i1 < bat_i0 )&&(g_del_d1 < 0))){
                    
                    g_del_d1 = (int)(g_del_d0/g_alpha - g_k); // changed + to -
                    g_d1 = g_d0 + g_del_d1;
                    CCPR1L = g_d1; 
                    g_del_d0 = g_del_d1;
                    g_d0 = g_d1;                                                         
                }
                else if(((bat_i1 > bat_i0)&&(g_del_d1 < 0))||((bat_i1 < bat_i0 )&&(g_del_d1 > 0))){
                    
                    g_del_d1 = (int)(g_del_d0/2 + g_k); // changed - to +
                    g_d1 = g_d0 + g_del_d1;
                    CCPR1L = g_d1; 
                    g_del_d0 = g_del_d1;
                    g_d0 = g_d1; 
                    
                }
        
    }
     */
}



void main(){
 

    int flag = 1;
    
    PWM_setup();
    ADC_setup();
    
    while(1){
        
        //pv_i1 = pv_I();
        //pv_v1 = pv_V();
        bat_i1 = bat_I();
        bat_v1 = bat_V();
        
        
            
        mppt(flag);
        
        
        flag = 0;
       // pv_i0 = pv_i1;
        //pv_v0 = pv_v1;
        bat_i0 = bat_i1;
        bat_v0 = bat_v1;
      
    }
    

}

