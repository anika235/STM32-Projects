#include "stm32f4xx.h"
#include "CLOCK.h"
#include "SYS_INIT.h"
#include "GPIO.h"
#include "stdlib.h"

void GPIOA_delay(int time)
{
	int tmp = time/1000;
	for (int i=0;i<tmp;i++)
	{
		GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
		GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
		ms_delay(333);
		
		GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);
		
		GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);
		GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);
		ms_delay(333);
		
		GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);
		
		GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_SET);
		GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);
		ms_delay(334);
		
		GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET);
		
	}
}
void GPIOB_delay(int time)
{
	int tmp = time/1000;
	for (int i=0;i<tmp;i++)
	{	
		
		GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
		GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET);
		ms_delay(334);
		
		GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);
		
	
		GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
		GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);
		ms_delay(333);
		
		GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);
		
		GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_SET);
		GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);
		ms_delay(333);
		
		GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
		
		
	}
}

void right_left_stop(int left,int right,GPIO_PinState pinstate)
{
	
		if(pinstate==GPIO_PIN_SET)
		{
			//A left white
			if(right<=0) GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
			if(right<=1) GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);
			if(right<=2) GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);
			
			//a right white
			if(left<=0) GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
			if(left<=1) GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);
			if(left<=2) GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_SET);
		}
		
		if(pinstate==GPIO_PIN_RESET)
		{
			//A white off
			GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_RESET);
			
				
			GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET);
		}
		
}

void upper_lower_stop(int up,int down,GPIO_PinState pinstate)
{
		if(pinstate==GPIO_PIN_SET)
		{
				//B upper white
			if(up<=0) GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_SET);
			if(up<=1) GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			if(up<=2) GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
			
			//B down white
			if(down<=0) GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET);
			if(down<=1) GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);
			if(down<=2) GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);
		}
		
		if(pinstate==GPIO_PIN_RESET)
		{
			//white off
			GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
			
			GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
		}
		
}

int main(void)
{
	initClock();
	sysInit();
	RCC -> AHB1ENR |= (1<<0);
	RCC -> AHB1ENR |= (1<<1);
	RCC -> AHB1ENR |=(1<<2);
	
	GPIO_InitTypeDef InitDef;
	InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	
	
	GPIO_InitTypeDef InitDef2;
	InitDef2.Mode=GPIO_MODE_INPUT;
	
	
	GPIO_Init(GPIOA,&InitDef);
	GPIO_Init (GPIOB, &InitDef);
	GPIO_Init(GPIOC,&InitDef2);
//	GPIO_Init(GPIOB,&InitDef2);
	int left, right, up, down;
	srand(69);
	
	while(1)
	{
		
		up=rand()%3;
		down=rand()%3;
		left=rand()%3;
		right=rand()%3;
		
		//vertical
		//red and green on
		GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);//A-> red
		GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_SET);//B-> green
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_8)==GPIO_PIN_SET)
		{

				
				right_left_stop(left,right, GPIO_PIN_SET);
				GPIOB_delay(5000);
		}
		

		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_1)==GPIO_PIN_RESET && GPIO_ReadPin(GPIOC,GPIO_PIN_7)==GPIO_PIN_RESET)
		{
			GPIOB_delay(5000);	
		}			

		//green off
		GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_RESET);//B-> green
		
		//yellow on 
		GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);//B->yellow
		GPIOB_delay(2000);
	
		//red off
		GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);//A->red
		right_left_stop(left, right,GPIO_PIN_RESET);
		
		//YEllow off
		GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);//B
		
		//green on
		GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_SET);//A
		
		//red on
		GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET);//B
		
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_11)==GPIO_PIN_SET)
		{
			upper_lower_stop(up,down,GPIO_PIN_SET);
			GPIOA_delay(5000);
		}
		
		/*upper_lower_stop(up,down,GPIO_PIN_SET);
		GPIOA_delay(5000);*/
		if(GPIO_ReadPin(GPIOC,GPIO_PIN_6)==GPIO_PIN_RESET && GPIO_ReadPin(GPIOC,GPIO_PIN_10)==GPIO_PIN_RESET)
		{
			GPIOA_delay(5000);	
		}			
	
		//green off
		GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_RESET);//A
		//yellow on
		GPIO_WritePin(GPIOA, GPIO_PIN_11,GPIO_PIN_SET);//A
		GPIOA_delay(2000);
		
		//yellow off
		GPIO_WritePin(GPIOA, GPIO_PIN_11,GPIO_PIN_RESET);//A
		//red off
		GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_RESET);//B
		upper_lower_stop(up,down,GPIO_PIN_RESET);
	
	}
}
