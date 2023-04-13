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

#include "USART.h"
#include <stdint.h>
#include <stdio.h>
#include "CLOCK.h"
#include "GPIO.h"
#include "stm32f446xx.h"



void _USART_WRITE(USART_TypeDef *usart,uint8_t *s)
{
	while (*s) UART_SendChar(usart,*s++);
}
/*****Modify according to your need *****/
uint8_t _USART_READ(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		if(i==0 && buff[i]!= 0x03F){continue;}
		n=i;
	}
	return n;
}

uint8_t _USART_READ_STR(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		n=i;
		if(buff[i]=='\0' || buff[i] == '\n' || buff[i] == ' ')
		{ 	
			buff[i]='\0';
			break;
		}
	}
	return n;
}


void UART_SendChar(USART_TypeDef *usart,uint8_t c){
	usart->DR = c;
	while(!(usart->SR & (1<<7)));
}

// Send a string to the UART PORTx
void UART_SendString(USART_TypeDef *usart,const char *s){
	while (*s) {
		UART_SendChar(usart,*s);
		s++;
	}
}


uint8_t UART_GetChar(USART_TypeDef *usart){
	uint8_t tmp;
	while(!(usart->SR & (1<<5)));
	tmp=(uint8_t)usart->DR;
	return tmp;
}

void UART_GetString(USART_TypeDef *uart,char* buff)
{
	uint8_t i=0;
		char x=UART_GetChar(uart);
		while(x!='.')
		{
				buff[i]=x;
				i++;
				x=UART_GetChar(uart);
		}
		
		buff[i]='\0';
		


	}

/**********************************
* USART 2 Configuration
***********************************/
void UART2_Config(void){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<17); //enable UART 2
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<4); //bits [5:4] -> 1:0 -->Alternate function for pin PA2
	GPIOA->MODER |= (2<<6); //bits [7:6] -> 1:0 -->Alternate function for PA3
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6); //bits [5:4] -> 1:1 -> high speed PA2; bits [7:6] -> 1:1 -> high speed PA3 
	
	GPIOA->AFR[0] |= (7<<8);//Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA2
	GPIOA->AFR[0] |= (7<<12); //Bytes (15:14:13:12) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA3
	
	//3. Enable UART on USART_CR1 rgister
	USART2->CR1 = 0x00; //clear USART
	USART2->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART2->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART2->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART2->CR1 |= (1<<2); // enable RE for receiver 
	USART2->CR1 |= (1<<3); //enable TE for transmitter
	
	USART2->CR1 |= (1<<5); //enable RXNEIE or recieve interrupt for USART2
	
}

void  UART4_Config(void){
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<19); //enable UART 4
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	
		//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<0); //bits [1:0] -> 1:0 -->Alternate function for pin PA0->Tx of UART4
	GPIOA->MODER |= (2<<2); //bits [3:2] -> 1:0 -->Alternate function for PA1->Rx of UART4
	
	GPIOA->OSPEEDR |= (3<<0) | (3<<2); //bits [1:0] -> 1:1 -> high speed PA0; bits [3:2] -> 1:1 -> high speed PA1 
	
	GPIOA->AFR[0] |= (8<<0);//Bytes (3:2:1:0) = 1:0:0:0 --> AF8 Alternate function for USART4 at pin PA0
	GPIOA->AFR[0] |= (8<<4); //Bytes (7:6:5:4) = 1:0:0:0 --> AF8 Alternate function for USART4 at pin PA1
	
	//3. Enable UART on UART_CR1 rgister
	UART4->CR1 = 0x00; //clear UART
	UART4->CR1 |= (1<<13);  // UE-bit enable UART
	
	//4. Program M bit in USART CR1 to define the word length
	UART4->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	UART4->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	UART4->CR1 |= (1<<2); // enable RE for receiver 
	UART4->CR1 |= (1<<3); //enable TE for transmitter
	
	UART4->CR1 |= (1<<5); //enable RXNEIE or recieve interrupt for UART4
	
}

void  UART5_Config(void){
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<20); //enable UART 4
	RCC->AHB1ENR |= (1<<2); //enable GPIOC clock
	RCC->AHB1ENR |= (1<<3); //enable GPIOD clock
	
		//2. Configure UART pin for Alternate function
	GPIOC->MODER |= (2<<24); //bits [25:24] -> 1:0 -->Alternate function for pin PC12->Tx of UART5
	GPIOD->MODER |= (2<<4); //bits [5:4] -> 1:0 -->Alternate function for PD2->Rx of UART5
	
	GPIOC->OSPEEDR |= (3<<24); //bits [25:24] -> 1:1 -> high speed PC12
	GPIOD->OSPEEDR |= (3<<4); //bits [5:4] -> 1:1 -> high speed PD2
	
	GPIOC->AFR[1] |= (8<<16);//Bytes (19:18:17:16) = 1:0:0:0 --> AF8 Alternate function for UART5 at pin PC12
	GPIOD->AFR[0] |= (8<<8); //Bytes (11:10:9:8) = 1:0:0:0 --> AF8 Alternate function for UART5 at pin PD2
	
	//3. Enable UART on UART_CR1 rgister
	UART5->CR1 = 0x00; //clear UART
	UART5->CR1 |= (1<<13);  // UE-bit enable UART
	
	//4. Program M bit in UART CR1 to define the word length
	UART5->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the UART_BRR register.
	UART5->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in UART_CR1 register
	UART5->CR1 |= (1<<2); // enable RE for receiver 
	UART5->CR1 |= (1<<3); //enable TE for transmitter
	
	UART5->CR1 |= (1<<5); //enable RXNEIE or recieve interrupt for UART5
	
}


