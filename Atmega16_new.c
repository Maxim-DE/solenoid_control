/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
� Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
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
//===================================================================================================
//===================================================================================================
//===================================================================================================
#define   SetBit(reg, bit)          reg |= (1<<bit)           
#define   ClearBit(reg, bit)       reg &= (~(1<<bit))
#define   InvBit(reg, bit)          reg ^= (1<<bit)
#define   BitIsSet(reg, bit)       ((reg & (1<<bit)) != 0)
#define   BitIsClear(reg, bit)    ((reg & (1<<bit)) == 0)
//===================================================================================================
//===========================��������� �����������===================================================
//===================================================================================================
#define		PORT_LED_SHORT  PORTD	// ����� ����� � �������� ������������ ���������	
#define		DDRX_LED_SHORT  DDRD
#define		BIT_LED_SHORT  	2		// ����� ������ ����� � �������� ������������ ���������

#define		LED_SHORT_ON    SetBit(PORT_LED_SHORT,BIT_LED_SHORT);
#define		LED_SHORT_OFF	ClearBit(PORT_LED_SHORT,BIT_LED_SHORT);		

//---------------------------��������� ������������ ������ ��-----------------------------------------
#define		PORT_LED_GAP  	PORTD
#define		DDRX_LED_GAP  	DDRD
#define		BIT_LED_GAP  	3

#define		LED_GAP_ON	 	SetBit(PORT_LED_GAP,BIT_LED_GAP);		
#define		LED_GAP_OFF		ClearBit(PORT_LED_GAP,BIT_LED_GAP);	
//===================================================================================================
//===================================================================================================
//===================================================================================================
#define PWM_A 0
#define PWM_B 1
#define PWM_DEFAULT 2
#define ON  3
#define OFF 4
#define WAIT 5

#define CHANEL_A_ON  SetBit(TCCR1A,7);
#define CHANEL_A_OFF ClearBit(TCCR1A,7);

#define CHANEL_B_ON  SetBit(TCCR1A,5);
#define CHANEL_B_OFF ClearBit(TCCR1A,5);

#define ADC_VREF_TYPE 0x40

enum{ EXIT,LED_ERROR_SHORT_ON,LED_ERROR_SHORT_OFF,LED_ERROR_GAP_ON,LED_ERROR_GAP_OFF,LED_BLIK,LED_BLIK_START};  // LED_STATE
//===================================================================================================
//===================================================================================================
//===================================================================================================
unsigned char mode_pwm=PWM_DEFAULT;
volatile unsigned int time=0;
volatile unsigned int time_led=0;
volatile unsigned char Led_test=ON; 
//===================================================================================================
//===================================================================================================
//===================================================================================================
void main_pwm_on(unsigned int ADC);
void proces_led(unsigned char mode_led);
void blik_led_start(void);
void blik_led(void);



//===================================================================================================
//===================================================================================================
//===================================================================================================
// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input){
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
//===================================================================================================
//===================================================================================================
//===================================================================================================
// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void){
// Place your code here
time=time+1;	//�����������
time_led=time_led+1;


//	switch(Led_test){
//		case ON:{
//			LED_GAP_ON;
//			LED_SHORT_ON;
//			Led_test=OFF;
//			break;
//		}
//		case OFF:{
//			LED_GAP_OFF;
//			LED_SHORT_OFF;		
//			Led_test=ON;
//			break;
//		}		
//	}

TCNT0=0x00;
}


//===================================================================================================
//===================================================================================================
//===================================================================================================
void main(void)
{
unsigned int ADC=0;
unsigned char mode_on_off=OFF;
unsigned char MSG_LED=EXIT;
//-------------������������� ����������� (���� ����� ��� 0)-----------------------
//------------------------��������� ��--------------------------------------------
SetBit(DDRX_LED_SHORT,BIT_LED_SHORT);
ClearBit(PORT_LED_SHORT,BIT_LED_SHORT);
//------------------------��������� ������----------------------------------------
SetBit(DDRX_LED_GAP,BIT_LED_GAP);
ClearBit(PORT_LED_GAP,BIT_LED_GAP);
//--------------------------------------------------------------------------------
#include <init.h>



//CHANEL_A_ON;
//CHANEL_B_ON;

����� ���� ��� ��������� ���� ������ ������ �.� ����� ���������� � ����(�� �����)
 ���� ������� ��� �� ��������� ������������

//��������� ����������� �� 8 � ���������������� ������ �� ���� ��� 100 ���� ������ 

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7,813 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
//TCCR0=0x05;
//��� ���������� ������� 8��� ������� ������ ������� 7,813 kHz
//� ��� 1 ��� ������ ����� 1 ms
//������ �������� �� 2 ms �� ������   
TCCR0=0;
TCCR0|=(0<<CS00)|(1<<CS01)|(0<<CS02); //����������� 1024
TCNT0=0xFF;
OCR0=0x00;




// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK|=(1<<TOIE0);




CHANEL_A_OFF;
CHANEL_B_OFF;
time=0;
#asm("sei")
//-------������� �����������------------
while(time<1000){
//while(1){
    //time_led=time_led+1;
	blik_led_start();
//    LED_GAP_OFF;
//    LED_SHORT_OFF;
}
//===================================================================================================
//=================================������� ����======================================================
//===================================================================================================
while (1){
//===========������ �������� � ���==============
      ADC=read_adc(0);
//============������ �����������================
		proces_led(MSG_LED);
//===========����� ������=======================
		if(ADC<50){MSG_LED=LED_ERROR_SHORT_ON;}
		if(ADC>973){MSG_LED=LED_ERROR_GAP_ON;}		
//===========����� ������� ��� ���==============
        mode_pwm=PWM_DEFAULT;      
        if(ADC<503){ 
            mode_pwm=PWM_A;        
        }
        if(ADC>522){ 
            mode_pwm=PWM_B;        
        }            
//===========������ ��� ��� � ���������=========
        switch(mode_on_off){
            case ON:{
                main_pwm_on(ADC);
                break;
            }
            case OFF:{
				MSG_LED=LED_BLIK;
                if(mode_pwm==PWM_DEFAULT){
                    mode_on_off=ON;
                }
                break;
            }
        }
}
}
//=============================================================================
//=======================�������===============================================
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
//------------------------------------------------------------------------
/*
  * @��������:    ������� ������ �� ������������ ��������� ���������.
  * @���������: LED_ERROR_SHORT_ON - ������   ��������� ��			
				LED_ERROR_SHORT_OFF- �������� ��������� �� 				
				LED_ERROR_GAP_ON - ������   ��������� ������			
				LED_ERROR_GAP_OFF- �������� ��������� ������ 								
				LED_BLIK - 		�������� ������� ����� ����������� � �������� ���		
				
  * @��������:    �����������
*/
void proces_led(unsigned char mode_led){
	switch(mode_led){
		case EXIT:{
			break;
		}
		//----------��-----------------------
		case LED_ERROR_SHORT_ON:{	
			LED_SHORT_ON;
			break;
		}
		case LED_ERROR_SHORT_OFF:{
			LED_SHORT_OFF;
			break;
		}		
		//----------������-----------------------		
		case LED_ERROR_GAP_ON:{		
			LED_GAP_ON;
			break;
		}
		case LED_ERROR_GAP_OFF:{
			LED_GAP_OFF;		
			break;
		}			
		//----------������� main_pwm=OFF---------------------
		case LED_BLIK:{
			blik_led();
			break;
		}
		// //----------������� INIT=OK(�� ���������� � ��������)----------
		// case LED_BLIK_START:{
			// blik_led();
			// break;
		// }		
		default:{break;}
	}
}

//------------------------------------------------------------------------
/*
  * @��������:    ������ ����� ������������ c �������� 2000 ms � ������� 5 ������ ����������
				  ��� �� ���������� � ��������.	
  * @��������:    �����������
*/
void blik_led_start(void){
	static unsigned char mode_blik_init=ON;
	
	switch(mode_blik_init){
		case ON:{
			LED_GAP_ON;
			LED_SHORT_ON;
			if(time_led>250){ 
				mode_blik_init=OFF;
				time_led=0;
			}	
			break;
		}
		case OFF:{
			LED_GAP_OFF;
			LED_SHORT_OFF;		
			if(time_led>250){
				mode_blik_init=ON;			
				time_led=0;
			}			
			break;
		}		
	}

}
//------------------------------------------------------------------------
/*
  * @��������:    ������ ����� ������������ � �������� 2000 ms ����������
				  ��� �� ����������. �  ������� ��� ��������� � ����������� ���������.
				  (����� ������������� ��������� �� � 0 ���������).	
  * @��������:    �����������
*/
void blik_led(void){
	static unsigned char mode_blik=ON;
	
	switch(mode_blik){
		case ON:{
			LED_GAP_ON;
			LED_SHORT_ON;
			if(time_led>1000){ 
				mode_blik=OFF;
				time_led=0;
			}	
			break;
		}
		case OFF:{
			LED_GAP_OFF;
			LED_SHORT_OFF;		
			if(time_led>1000){
				mode_blik=ON;			
				time_led=0;
			}			
			break;
		}		
	}

}
//------------------------------------------------------------------------

