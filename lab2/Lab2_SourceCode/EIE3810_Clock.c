#include "stm32f10x.h"
#include "EIE3810_CLOCK.h"

void EIE3810_clock_tree_init(void)
{
	u8 PLL=7;  //0b0111
	u8 temp=0;
	RCC->CR |= 0x00010000; //set bit-16 in CR to 1 to enable HSE
	while(!((RCC->CR>>17)&0x1));//wait until the HSERDY flag to be 1 when HSE is ready
	RCC->CFGR &= 0xFFFDFFFF; //set bit-17 in CFGR(PLLXTPRE) to 0, HSE not divided.
	RCC->CFGR |= 1<<16; //set bit-16 in CFGR(PLLSRC) to 1, select HSE as PLL input clock
	RCC->CFGR |= PLL<<18; //set PLLMUL to 0111 to multiple the clock by 9
	RCC->CR |=0x01000000;//set bit-24 in CR to 1 to enable PLL
	while(!(RCC->CR>>25));//wait until the PLLRDY flag to be 1 when PLL is locked
	RCC->CFGR &=0xFFFFFFFE;//set bit-0 in CFGR to 0 (con.)
	RCC->CFGR |=0x00000002;//set bit-1 in CFGR to 1 to select PLL as system clock
	//CFGR == 0b**** **** **01 1101 **** **** **** **10
	while(temp !=0x02) //check if bit-3 and bit-2 equal to 10(PLL selected as system clock)
	{
		temp=RCC->CFGR>>2; 
		temp &= 0x03; //assign bit-3 and bit-2 to temp in binary
	}	
	RCC->CFGR &= 0xFFFFFC0F;//set from bit-4 to bit-9 to 0
	RCC->CFGR |= 0x00000400;//set bit-10 to 1
	//PPRE1 become 100, HCLK devided by 2(36MHz); HPRE become 0000, SYSCLK not divided
	RCC->CFGR &= 0xFFFFDFFF;//set bit-13 to 0 to PPRE2 to 0xx (no divided, 72MHz)
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states	
	RCC->APB1ENR |= 1<<17; //to enable USART2
	RCC->APB2ENR |= 1<<14; //to enable USART1
}
