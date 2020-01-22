#include "stm32f10x.h"
#include "EIE3810_USART.h"

void USART_print(u8 USARTport, char *st)
{
	u8 i=0;
	while(st[i] != 0x00)
	{
		if (USARTport == 1)
		{
			USART1->DR = st[i];
			while(!((USART1->SR>>7)&0x1));
		}
		if (USARTport == 2) 
		{
			USART2->DR = st[i];
			while(!((USART2->SR>>7)&0x1));
		}
		if (i==255) break;
		i++;
	}
}



void EIE3810_USART2_init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16); //calculate UARTDIV in decimal
	mantissa=temp; //take integer portion (12 bits)
	fraction=(temp-mantissa)*16;//take fravtion portion (4 bits)
	mantissa<<=4;
	mantissa+=fraction;//Convert them together 
	RCC->APB2ENR |= 1<<2; //enable GPIOA
	GPIOA->CRL &= 0xFFFF00FF; //initialize PA2 and PA3
	GPIOA->CRL |= 0x00008B00; //set PA2 altermate function ouput push-pull with 50MHz; PA3 to input with pull-up/pull-down
	RCC->APB1RSTR |= 1<<17; //reset USART2 by setting bit-17 to 1
	RCC->APB1RSTR &= ~(1<<17); //clear the reset status by settign bit-17 to 0 
	USART2->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART2->CR1=0x2008; //enable USART2,set word length(1 start bit, 8 data bit, n stop bit);
						//disable parity, enable transmitter by assigning 0b0010 0000 0000 1000 to CR1
}

void EIE3810_USART1_init(u32 pclk2, u32 baudrate)
{
	//USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk2*1000000)/(baudrate*16); //calculate UARTDIV in decimal
	mantissa=temp; //take integer portion (12 bits)
	fraction=(temp-mantissa)*16;//take fravtion portion (4 bits)
	mantissa<<=4;
	mantissa+=fraction;//Convert them together 
	RCC->APB2ENR |= 1<<2; //enable GPIOA
	GPIOA->CRH &= 0xFFFFF00F; //initialize PA9 and PA10
	GPIOA->CRH |= 0x000008B0; //set PA9 altermate function ouput push-pull with 50MHz; PA10 to input with pull-up/pull-down
	RCC->APB2RSTR |= 1<<14; //reset USART1 by setting bit-14 to 1
	RCC->APB2RSTR &= ~(1<<14); //clear the reset status by settign bit-14 to 0 
	USART1->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART1->CR1=0x2008; //enable USART1,set word length(1 start bit, 8 data bit, n stop bit);
						//disable parity, enable transmitter by assigning 0b0010 0000 0000 1000 to CR1
}
