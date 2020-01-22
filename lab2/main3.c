#include "stm32f10x.h"
void EIE3810_clock_tree_init(void);
//void EIE3810_USART2_init(u32, u32);
void EIE3810_USART1_init(u32, u32);
void Delay(u32);

void USART_print(u8 USARTport, char *st)
{
	u8 i = 0;
	while(st[i] != 0x00)
	{
		if (USARTport == 1){
			USART1->DR = st[i];
		while (!((USART1->SR>>7)&0x01));
		}
		if (USARTport == 2){
			USART2->DR = st[i];
		while (!(USART2->SR>>7))
		}

		if (i==255) break;
		i++;
	}
}


int main(void)
{
	EIE3810_clock_tree_init();
	//EIE3810_USART2_init(36, 9600);
	EIE3810_USART1_init(72, 9600);
	while(1)
	{
		USART_print(1,123456);
	}	
}




void EIE3810_clock_tree_init(void)
{
	u8 PLL=7;  //0b0111
	u8 temp=0;
	RCC->CR |= 0x00010000; //set bit-16 in CR to 1 to enable HSE
	while(!((RCC->CR>>17)&0x1));//wait until the HSERDY flag to be 1 when HSE is ready,the loop will break
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
	RCC->CFGR &= 0xFFFFDFFF;//Important chane: set bit-13 to 0 to PPRE2 to 0xx (no divided, 72MHz)
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states	
	//RCC->APB1ENR |= 1<<17; //to enable USART2
	RCC->APB2ENR |= 1<<14; //to enable USART1
}

/**
void EIE3810_USART2_init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16); //calculate UARTDIV in decimal
	mantissa=temp; //take integer portion (12 bits)
	fraction=(temp-mantissa)*16;//take fraction portion (4 bits)
	mantissa<<=4;
	mantissa+=fraction;//Convert them together 
	RCC->APB2ENR |= 1<<2; //enable GPIOA
	GPIOA->CRL &= 0xFFFF00FF; //initialize PA8 and PA9
	GPIOA->CRL |= 0x00008B00; //set PA2 altermate function ouput push-pull with 50MHz; PA3 to input with pull-up/pull-down
	RCC->APB1RSTR |= 1<<17; //reset USART2 by setting bit-17 to 1
	RCC->APB1RSTR &= ~(1<<17); //clear the reset status by settign bit-17 to 0 
	USART2->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART2->CR1=0x2008; //enable USART2,set word length(1 start bit, 8 data bit, n stop bit);
						//disable parity, enable transmitter by assigning 0b0010 0000 0000 1000 to CR1
}
**/

void EIE3810_USART1_init(u32 pclk2, u32 baudrate)
{
	//USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk2*1000000)/(baudrate*16); //calculate UARTDIV in decimal
	mantissa=temp; //take integer portion (12 bits)
	fraction=(temp-mantissa)*16;//take fraction portion (4 bits)
	mantissa<<=4;
	mantissa+=fraction;//Convert them together 
	RCC->APB2ENR |= 1<<2; //enable GPIOA
	GPIOA->CRH &= 0xFFFFF00F; //initialize PA9 and PA10
	GPIOA->CRH |= 0x000008B0; //set PA9 altermate function ouput push-pull with 50MHz; 
								//PA10 to input with pull-up/pull-down
	RCC->APB2RSTR |= 1<<14; //reset USART1 by setting bit-14 to 1
	RCC->APB2RSTR &= ~(1<<14); //clear the reset status
	USART1->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART1->CR1=0x2008; //enable USART1,set word length(1 start bit, 8 data bit, n stop bit);
						//disable parity, enable transmitter by assigning 0b0010 0000 0000 1000 to CR1
}