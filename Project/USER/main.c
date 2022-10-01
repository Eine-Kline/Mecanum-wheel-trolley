#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "hc05.h"
#include "usart3.h"			 	 
#include "string.h"	   
#include "usmart.h"	
#include "timer.h"
#include "control.h"
 /************************************************
 Bluetooth£º  
			PA4-->KEY
		  PB10-->RXD
		  PB11-->TXD
			GND-->GND
		  5V-->VCC 
	Direction control£º
			PC0-->FRONTLEFT-IN1
			PC1-->FRONTLEFT-IN2
			PC2-->FRONTRIGHT-IN1  IN3
			PC3-->FRONTRIGHT-IN2  IN4
			PC4-->BACKLEFT-IN1     IN5
			PC5-->BACKLEFT-IN2    IN6
			PC6-->BACKRIGHT-IN1    IN7
			PC7-->BACKRIGHT-IN2    IN8
	Speed control£º
			PB6-->FRONTLEFT      EN1
			PB7-->FRONTRIGHT    EN2
			PB8-->BACKLEFT     EN3
			PB9-->BACKRIGHT     EN4
************************************************/

u32 temp1=0,temp2=0,temp3=0,temp4=0;
extern u16 time[2];
extern u8 biti; 
int main(void)
 {	 
		u16 i=100;  
	u8 reclen=0;
	
	delay_init();	    	 //Delay function initialization	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//Set NVIC interruption 2:2 to seize priority£¬2-digit response priority
	uart_init(115200);	 	//The initial serial port is 9600
	TIM3_PWM_Int_Init(99,7199);
	LED_Init();				//Initialize the hardware interface connected to the LED
 	init_wheel_control();		
    usmart_dev.init(72); 	//Initialize USMART		
	delay_ms(1000);			//Waiting for the bluetooth module become stable 
	
	while(HC05_Init()) 		//Initialize ATK-HC05 module  
	{
		u3_printf("error\r\n");
	}
	USART3_RX_STA=0;
	
 	while(1) 
	{			  
		if(USART3_RX_STA&0X8000)			//Have already received one group of data
		{
				
 			reclen=USART3_RX_STA&0X7FFF;	// Get the length of data 
		  	USART3_RX_BUF[reclen]=0;	 	// Adding end character
			//Speed setting
			if(reclen==1)
			{
				if(strcmp((const char*)USART3_RX_BUF,"0")==0){time[0]=90;time[1]=9;}
				else if(strcmp((const char*)USART3_RX_BUF,"1")==0){time[0]=80;time[1]=19;}
				else if(strcmp((const char*)USART3_RX_BUF,"2")==0){time[0]=70;time[1]=29;}
				else if(strcmp((const char*)USART3_RX_BUF,"3")==0){time[0]=60;time[1]=39;}
				else if(strcmp((const char*)USART3_RX_BUF,"4")==0){time[0]=50;time[1]=49;}
				else if(strcmp((const char*)USART3_RX_BUF,"5")==0){time[0]=40;time[1]=59;}
				else if(strcmp((const char*)USART3_RX_BUF,"6")==0){time[0]=30;time[1]=69;}
				else if(strcmp((const char*)USART3_RX_BUF,"7")==0){time[0]=20;time[1]=79;}
				else if(strcmp((const char*)USART3_RX_BUF,"8")==0){time[0]=10;time[1]=89;}
				else if(strcmp((const char*)USART3_RX_BUF,"9")==0){time[0]=1;time[1]=98;}
				
				u3_printf("speed=%s\r\n",USART3_RX_BUF);
			}
			// Set direction
			if(reclen==4||reclen==5)
			{
				if(strcmp((const char*)USART3_RX_BUF,"none")==0)car_dir_mood=0;
				else if(strcmp((const char*)USART3_RX_BUF,"head")==0)car_dir_mood=1;
				else if(strcmp((const char*)USART3_RX_BUF,"back")==0)car_dir_mood=2;
				else if(strcmp((const char*)USART3_RX_BUF,"left")==0)car_dir_mood=3;
				else if(strcmp((const char*)USART3_RX_BUF,"right")==0)car_dir_mood=4;
				else if(strcmp((const char*)USART3_RX_BUF,"clock")==0)car_dir_mood=5;
				else if(strcmp((const char*)USART3_RX_BUF,"anti")==0)car_dir_mood=6;
				else if(strcmp((const char*)USART3_RX_BUF,"NtoW")==0)car_dir_mood=7;
				else if(strcmp((const char*)USART3_RX_BUF,"NtoE")==0)car_dir_mood=8;
				else if(strcmp((const char*)USART3_RX_BUF,"StoW")==0)car_dir_mood=9;
				else if(strcmp((const char*)USART3_RX_BUF,"StoE")==0)car_dir_mood=10;
				else if(strcmp((const char*)USART3_RX_BUF,"Stop")==0)car_dir_mood=11;
				wheel_control(car_dir_mood);
			}
			
			if(reclen==2)
			{
				if(strcmp((const char*)USART3_RX_BUF,"ok")==0){i=30;while(i--){delay_ms(10);GPIO_SetBits(GPIOA,GPIO_Pin_4);}}
				else if(strcmp((const char*)USART3_RX_BUF,"no")==0){i=30;while(i--){delay_ms(10);GPIO_ResetBits(GPIOA,GPIO_Pin_4);}}
				u3_printf("okk");
			}
 			USART3_RX_STA=0;	 
		}
	}
}
