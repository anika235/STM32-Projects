#include "stm32f4xx.h"
#include "CLOCK.h"
#include "SYS_INIT.h"
#include "GPIO.h"


int main(void)
{
	initClock();
	sysInit();
	
	RCC -> AHB1ENR |= (1<<0);
	GPIO_InitTypeDef InitDef;
	InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	InitDef.Pull = GPIO_NOPULL;
	InitDef.Speed = GPIO_SPEED_FREQ_LOW;
	
	GPIO_Init(GPIOA,&InitDef);
	
	
	while(1)
	{
		GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET);
		ms_delay(1000);
		GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
		ms_delay(1000);
	}
}
