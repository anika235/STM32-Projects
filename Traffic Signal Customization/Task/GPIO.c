/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "GPIO.h"
#include "CLOCK.h"

void GPIO_WritePin(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,GPIO_PinState PinState)
{

	if(PinState != GPIO_PIN_RESET)
		GPIOx->BSRR |=GPIO_pin;
	
	else
		GPIOx->BSRR |= (uint32_t)GPIO_pin << 16U;
	
}


GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
			GPIO_PinState bitstatus;
			
			
		 if((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
		 {
			 bitstatus = GPIO_PIN_SET;
			}
		 else
		 {
			bitstatus = GPIO_PIN_RESET;
		 }
		 return bitstatus;
 }


void GPIO_Init(GPIO_TypeDef* GPIOx,GPIO_InitTypeDef *GPIO_Init)
{
	
	 
	uint32_t temp;
	uint32_t position;
	uint32_t cur_position;
	uint32_t check_position;
	
    
	
	for (position = 0U;position < 12;position++){
		
		
		cur_position = (uint32_t)(1U << position);
		check_position = (uint32_t)(GPIO_Init->Pin) & cur_position;
		

		if(cur_position == check_position){
			
			/* Set the PIN Mode */
			temp = GPIOx->MODER;
			temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
			temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
			GPIOx->MODER = temp;
			

			
	
			
		}
	}

}


