#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>




static char input[100];
static char output[100];
static int k;
static int light;
static int delay1,delay2,delay3,extra1,extra,extra2,g1_delay,g2_delay,r1_delay,r2_delay,y1_delay,y2_delay,interval;

void USART2_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);

void transmit_data(USART_TypeDef*);

void TIM5Config(void);
void TIM2Config(void);
void delay(uint16_t ms);

void processString(void);
void traffic_config(int);

void monitor_time(void);

void traffic_status(void);

static int time_count=0;




void traffic_config(int light_num)
{
		
		if(light_num==1)
		{
			sprintf(input,"traffic light 1 G Y R %d %d %d %d\n",g1_delay,y1_delay,r1_delay,extra2);

			
		}
		else if (light_num==2)
		{
				sprintf(input,"traffic light 2 G Y R %d %d %d %d\n",g2_delay,y2_delay,r2_delay,extra1);
		}
		
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
}

void monitor_time(void)
{
		sprintf(input,"traffic monitor %d\n",interval);
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
}



void processString(void)
{
			char command[100];
	
			//command transmission
			transmit_data(UART4);
			strcpy(command,output);
			if(command[0]=='c')
			{
						//UART_SendString(USART2,command);
						if(command[15]=='l')
						{
								//UART_SendString(USART2,"yes");
								sscanf(command,"config traffic light %d G Y R %d %d %d %d",&light,&delay1,&delay2,&delay3,&extra);
								
								
								if(light==1)
								{
										g1_delay=delay1;
										y1_delay=delay2;
										r1_delay=delay3;
										extra2=extra;
									
								}
								else if(light==2)
								{
										g2_delay=delay1;
										y2_delay=delay2;
										r2_delay=delay3;
										extra1=extra;
										
								}
								
								
						}
						
						else if (command[15]=='m')
						{
									sscanf(command,"config traffic monitor %d",&interval);
					
							
						}
			}
			else if(command[0]=='r')
			{
					if(strlen(command)==4)
					{
							traffic_config(1);
							traffic_config(2);
							monitor_time();
					}
					
					else if(command[13]=='l')
					{
							//UART_SendString(USART2,command);
							
							sscanf(command,"read traffic light %d",&light);
							traffic_config(light);
					}
					else if(command[13]=='m')
					{
							monitor_time();
					}
			}
			strcpy(input,"");
			strcpy(output,"");
}

void traffic_status(void)
{
		//UART_SendString(USART2,"yes");
		char status1[10],status2[10],status3[10];
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_6)==GPIO_PIN_SET)
		{
				strcpy(status1,"ON");
		}
		else
		{
			strcpy(status1,"OFF");
		}
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET)
		{
				strcpy(status2,"ON");
		}
		else
		{
			strcpy(status2,"OFF");
		}
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_8)==GPIO_PIN_SET)
		{
				strcpy(status3,"ON");
		}
		else
		{
			strcpy(status3,"OFF");
		}
		
		
		sprintf(input,"%d traffic light 1 %s %s %s\n",time_count,status1,status2,status3);
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
		
		
		if(GPIO_ReadPin(GPIOA,GPIO_PIN_10)==GPIO_PIN_SET)
		{
				strcpy(status1,"ON");
			sprintf(input,"%d north south heavy traffic\n",time_count);
		
		}
		else
		{
			strcpy(status1,"OFF");
			sprintf(input,"%d north south light traffic\n",time_count);
		}
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
		
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_1)==GPIO_PIN_SET)
		{
				strcpy(status1,"ON");
		}
		else
		{
			strcpy(status1,"OFF");
		}
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_0)==GPIO_PIN_SET)
		{
				strcpy(status2,"ON");
		}
		else
		{
			strcpy(status2,"OFF");
		}
		
		if(GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_SET)
		{
				strcpy(status3,"ON");
		}
		else
		{
			strcpy(status3,"OFF");
		}
		
		
		sprintf(input,"%d traffic light 2 %s %s %s\n",time_count,status1,status2,status3);
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
		
		
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET)
		{
				strcpy(status1,"ON");
			sprintf(input,"%d East West heavy traffic\n",time_count);
		
		}
		else
		{
			strcpy(status1,"OFF");
			sprintf(input,"%d East West light traffic\n",time_count);
		}
		transmit_data(UART5);
		UART_SendString(USART2,output);
		strcpy(input,"");
		strcpy(output,"");
		
		
		
		
}


void USART2_IRQHandler(void)
{
			USART2->CR1 &= (uint32_t)~(1<<5);//turning off the interrupt
			UART_GetString(USART2,input);
			
			USART2->CR1|=(1<<5);//enabling the receive interrupt again
}

void UART4_IRQHandler(void)
{
		if(UART4->SR &(1<<5))
		{
				UART4->CR1 &= (uint32_t)~(1<<5);
				output[k]=(uint8_t)UART4->DR; //taking data from data register
				UART4->SR&=(uint32_t)~(1<<5);
				UART4->CR1|=(1<<5);
			
		}
		
		if(UART4->SR & (1<<7))
		{
				UART4->DR=input[k]; //copying data in the data register
			  UART4->SR &= (uint32_t)~(1<<7); //transmission bit disable
        UART4->CR1 &=(uint32_t) ~(1<<7); //transmission interrupt disable
				
		}
	
}

void UART5_IRQHandler(void)
{
	if(UART5->SR &(1<<5))
		{
				UART5->CR1 &= (uint32_t)~(1<<5); 
				output[k]=(uint8_t)UART5->DR;
				UART5->SR&=(uint32_t)~(1<<5);
				UART5->CR1|=(1<<5);
			
		}
		
		if(UART5->SR & (1<<7))
		{
				UART5->DR=input[k];
			  UART5->SR &= (uint32_t)~(1<<7);
        UART5->CR1 &=(uint32_t) ~(1<<7);
				
		}
}


void TIM5Config(void){
	RCC->APB1ENR |= (1<<3);
	TIM5->CR1 |= (1<<0);
	
	
	TIM5->PSC = 45000 - 1; 
	TIM5->ARR = 0xFFFF; 
	
	while(!(TIM5->SR & (1<<0)));
	
}
void TIM2Config(void){
	RCC->APB1ENR |= (1<<0);
		TIM2->CR1 |= (1<<0);
	
	TIM2->PSC = 45000 - 1; 
	TIM2->ARR = 0xFFFF; 
	

	
	while(!(TIM2->SR & (1<<0)));
	
}

void delay(uint16_t ms){
	ms = (uint32_t)2 * ms;
	TIM5->CNT = 0;
	while(TIM5->CNT < ms){
		
			
       if(strlen(input) != 0)
			{
						processString();
        
			}
			
			if(TIM2->CNT > (uint16_t)interval*1000*2)
			{
				
				time_count += interval;
				traffic_status();
				TIM2->CNT = 0;
			}
		
	
	}
}

void transmit_data(USART_TypeDef* uart)
{
		uint8_t i=0;
		k=0;
		for(i=0;i<strlen(input);i++)
		{
				uart->CR1|=(1<<7); //transmit interrupt enable
				while(uart->SR & (1<<7)); //wait till the transmit bit is disabled
				ms_delay(5); // doesn't work without the delay(?)
				k++;
		}
		output[k]='\0';
		strcpy(input,"");
	
	
}


int main(void)
{   
		

	
	//Configuration 
		initClock();
		sysInit();
		UART2_Config();
		UART4_Config();
		UART5_Config();
		TIM5Config();
		TIM2Config();
	
	//GPIO configuration
	
	
		RCC -> AHB1ENR |= (1<<0);
		RCC -> AHB1ENR |= (1<<1);
		RCC->AHB1ENR |=(1<<2);
	
	
	//for output
		GPIO_InitTypeDef InitDef;
		InitDef.Mode=GPIO_MODE_OUTPUT_PP;
		InitDef.Speed=GPIO_SPEED_FREQ_HIGH;
		InitDef.Pin=GPIO_PIN_4|GPIO_PIN_8|GPIO_PIN_6|GPIO_PIN_7;
	
		GPIO_InitTypeDef InitDef1;
		InitDef1.Mode=GPIO_MODE_OUTPUT_PP;
		InitDef1.Speed=GPIO_SPEED_FREQ_HIGH;
		InitDef1.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
		
		
		 GPIO_Init(GPIOA, &InitDef);
		 GPIO_Init(GPIOB,&InitDef1);
		 
		 
		 //for output
		 	GPIO_InitTypeDef InitDef3;
		InitDef.Mode=GPIO_MODE_INPUT;
		InitDef.Speed=GPIO_SPEED_FREQ_HIGH;
		InitDef.Pin=GPIO_PIN_10|GPIO_PIN_12;
	
		GPIO_InitTypeDef InitDef4;
		InitDef1.Mode=GPIO_MODE_INPUT;
		InitDef1.Speed=GPIO_SPEED_FREQ_HIGH;
		InitDef1.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8;
		
		 GPIO_Init(GPIOA, &InitDef3);
		 GPIO_Init(GPIOC,&InitDef4);
		
		
	
		//enabling the interrupts
    NVIC_EnableIRQ(USART2_IRQn);
   
    NVIC_EnableIRQ(UART4_IRQn);
		NVIC_EnableIRQ(UART5_IRQn);
	
  
		//emptying buffers
		strcpy(input,"");
		strcpy(output,"");
		
		//initializing delays
		g1_delay=5;
		r1_delay=5;
		y1_delay=2;
		
		g2_delay=5;
		r2_delay=5;
		y2_delay=2;
		
		interval=3;
		
		extra1=2;
		extra2=2;
		
	
	
	
	//initializing timers
		
			TIM2->CNT=0;
			
 
		while(1)
	{
		
				//if input not	empty send it to the traffic center
      if(strlen(input) != 0)
			{
						//UART_SendString(USART2,input);
				//transmit_data(UART4);
						processString();
			}
		
			
		
			int up_down=rand()%2;
			int right_left=rand()%2;
		
			GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET); //Green1 on
			GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET); //red2 on
		
			
			//traffic on off
			if(up_down==1)
			{
					GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
			}
			
			if(right_left==1)
			{
					GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
			}
			
			delay((uint16_t)g1_delay*1000);
			
			//extra delay
			
			
			if(extra2>0)
			{	
					delay((uint16_t)extra2*1000);
			}
			
			
			GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET); //Green1 off
			
			GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); //yellow1 on
			delay((uint16_t)y1_delay*1000);
			
			GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);// red2 off
			GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET); // green2 on
			
			GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); //yellow1 off
			GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); //red1 on
			
			delay((uint16_t)g2_delay*1000);
			
			if(extra1>0)
			{
					delay((uint16_t)extra1*1000);
			}
			
			
			GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET); //green2 off
			GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);//yellow2 on
			delay((uint16_t)y2_delay*1000);
			
			GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); //red1 off
			
			GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);//yellow2 off
			
			GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET); //traffic off
			GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET); //traffic 
			
			/*sprintf(ti,"The time is %d",TIM2->CNT);
			UART_SendString(USART2,ti);
			strcpy(ti,"");*/
			
			
			
			
					
     
		
    }
			
			
			
			
			
			
			
			
		
		
}
		
	
	





