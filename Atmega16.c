/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
© Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 28.03.2015
Author  : Maxim
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>

#include <delay.h>

#define   SetBit(reg, bit)          reg |= (1<<bit)           
#define   ClearBit(reg, bit)       reg &= (~(1<<bit))
#define   InvBit(reg, bit)          reg ^= (1<<bit)
#define   BitIsSet(reg, bit)       ((reg & (1<<bit)) != 0)
#define   BitIsClear(reg, bit)    ((reg & (1<<bit)) == 0)


#define PWM_A 0
#define PWM_B 1
#define PWM_DEFAULT 2
#define ON  3
#define OFF 4

#define CHANEL_A_ON  SetBit(TCCR1A,7);
#define CHANEL_A_OFF ClearBit(TCCR1A,7);

#define CHANEL_B_ON  SetBit(TCCR1A,5);
#define CHANEL_B_OFF ClearBit(TCCR1A,5);

#define ADC_VREF_TYPE 0x40


unsigned char mode_pwm=PWM_DEFAULT;

void main_pwm_on(unsigned int ADC);

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}

// Declare your global variables here

void main(void)
{
unsigned int ADC=0;
unsigned char mode_on_off=OFF;
#include <init.h>



//CHANEL_A_ON;
//CHANEL_B_ON;

CHANEL_A_OFF;
CHANEL_B_OFF;

while (1){
      ADC=read_adc(0);
//==================================
        mode_pwm=PWM_DEFAULT;      
        if(ADC<503){ 
            mode_pwm=PWM_A;        
        }
        if(ADC>522){ 
            mode_pwm=PWM_B;        
        }            
//=================================
        switch(mode_on_off){
            case ON:{
                main_pwm_on(ADC);
                break;
            }
            case OFF:{
                if(mode_pwm==PWM_DEFAULT){
                    mode_on_off=ON;
                }
                break;
            }
        }

}
}

//=============================================================================
//=======================‘ÛÌÍˆËˇ===============================================
//=============================================================================

void main_pwm_on(unsigned int ADC){
signed int PWM=0;
      switch(mode_pwm){
        case PWM_A:{ 
            CHANEL_A_ON;
            CHANEL_B_OFF;
                            
            PWM=2*(503-ADC);
            if(PWM<0){PWM=PWM*(-1);}
            //PWM=1023-(ADC>>1);
            //---------------
            if(PWM>=1023){PWM=1023;}
            if(PWM<=0){PWM=0;}            
            OCR1AH=(PWM>>8);
            OCR1AL=PWM;        
            break;
        }
        case PWM_B:{
            CHANEL_B_ON;
            CHANEL_A_OFF;
                            
            PWM=2*(503-ADC);
            if(PWM<0){PWM=PWM*(-1);}
                            
            //PWM=1023-(ADC<<1);
            //---------------
            if(PWM>=1023){PWM=1023;}
            if(PWM<=0){PWM=0;}
            OCR1BH=(PWM>>8);
            OCR1BL=PWM;        
            break;
        }
        case PWM_DEFAULT: 
        default:{
            OCR1BH=0;
            OCR1BL=0;
            //-------
            OCR1AH=0;
            OCR1AL=0;
            //-------                         
            CHANEL_B_OFF;
            CHANEL_A_OFF;        
            break;
        }        
      }

}




//      switch(1){
//      //----------ÿ»Ã_A_ON U=2,45 -----------------
//      //----------ÿ»Ã_B_OFF U=0-2,45 -----------------      
//        if(ADC<501){ 
//            CHANEL_A_ON;
//            CHANEL_B_OFF;
//            OCR1AH=(ADC>>8);
//            OCR1AL=ADC;                    
//            break;
//        }           
//        //---------U=2,46-2,54---------------------
//        if(ADC<520){
//            CHANEL_B_OFF;
//            CHANEL_A_OFF;        
//            break;
//        }
//        //---------U=2,54-5---------------------
//            CHANEL_B_ON;
//            CHANEL_A_OFF;
//            OCR1BH=(ADC>>8);
//            OCR1BL=ADC;            
//      }
