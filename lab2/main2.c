#include "stm32f10x.h"
void EIE3810_clock_tree_init(void);
void EIE3810_USART2_init(u32, u32);
void Delay(u32);


int main(void)
{
	EIE3810_clock_tree_init();
	EIE3810_USART2_init(36, 9600);
	while(1)
	{
		USART2->DR = 0x41;//0x41 represent A in ASCII
		Delay(50000);		//to make sure the former has been transformed
		USART2->DR = 0x42;//0x42 represent "B"
		Delay(50000);
		Delay(1000000);
	}	
}

void Delay(u32 count) 
{
	u32 i;
	for (i=0;i<count;i++);
}

void EIE3810_clock_tree_init(void)
{
	u8 PLL=7;  //which is 0b0111
	u8 temp=0;
	RCC->CR |= 0x00010000; //HSE_ON is at the bit-16,set it to 1 to enable HSE
	while(!((RCC->CR>>17)&0x1));//wait until HSE is ready(the HSERDY flag to be 1)
	RCC->CFGR &= 0xFFFDFFFF; //set PLLXTPRE(bit-17 in CFGR) to 0, HSE not divided.(16-19 is 0b1101 = 0xD)
	RCC->CFGR |= 1<<16; //set bit-16 in CFGR(PLLSRC) to 1, select HSE as PLL input clock
	RCC->CFGR |= PLL<<18; //set PLLMUL to 0111 to multiple the clock by 9
	RCC->CR |=0x01000000;//set bit-24 in CR to 1 to enable PLL
	while(!(RCC->CR>>25));//wait until the PLLRDY flag to be 1 when PLL is locked
	RCC->CFGR &=0xFFFFFFFE;//set bit-0 in CFGR to 0 (con.)
	RCC->CFGR |=0x00000002;//set bit-1 in CFGR to 1 to select PLL as system clock  ?
	//CFGR == 0b**** **** **01 1101 **** **** **** **10
	while(temp !=0x02) //Use a while loop to make sure bit-3 and bit-2 equal to 10(PLL selected as system clock)
	{
		temp=RCC->CFGR>>2; 
		temp &= 0x03; //assign bit-3 and bit-2 to temp in binary
	}	
	RCC->CFGR &= 0xFFFFFC0F;//set from bit-4 to bit-9 to 0
	RCC->CFGR |= 0x00000400;//set bit-10 to 1
	//PPRE1 become 100, HCLK devided by 2; HPRE become 0000, SYSCLK not divided
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states	
	RCC->APB1ENR |= 1<<17; //to enable USART2
	RCC->APB2ENR |= 1<<14; //to enable USART1
}

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
	GPIOA->CRL &= 0xFFFF00FF; //initialize PA2 and PA3
	GPIOA->CRL |= 0x00008B00; //set PA2 altermate function ouput push-pull with 50MHz; PA3 to input with pull-up/pull-down
	RCC->APB1RSTR |= 1<<17; //reset USART2 by setting bit-17 to 1
	RCC->APB1RSTR &= ~(1<<17); //clear the reset status by settign bit-17 to 0 
	USART2->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART2->CR1=0x2008; //enable USART2,set word length, disable parity, enable transmitter by assign 0b0010 0000 0000 1000 to CR1
}

void EIE3810_USART1_init(u32 pclk1, u32 baudrate)
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
	GPIOA->CRH &= 0xFFFFF00F; //initialize PA2 and PA3
	GPIOA->CRH |= 0x00008B00; //set PA2 altermate function ouput push-pull with 50MHz; PA3 to input with pull-up/pull-down
	RCC->APB2RSTR |= 1<<14; //reset USART2 by setting bit-17 to 1
	RCC->APB2RSTR &= ~(1<<14); //clear the reset status by settign bit-17 to 0 
	USART1->BRR=mantissa;//set baud rate register according to the binary number converted from UARTDIV
	USART1->CR1=0x2008; //enable USART2,set word length, disable parity, enable transmitter by assign 0b0010 0000 0000 1000 to CR1
}


