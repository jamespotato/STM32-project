#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_clock.h"

#define LED0_PWM_VAL TIM3->CCR2


void Delay(u32);
void EIE3810_TIM3_PWMInit(u16 arr, u16 psc);

	
int main(void)
{
	u16 LED0PWMVal=0;
	u8 dir=1;
	EIE3810_clock_tree_init();
	EIE3810_LED_Init();
	EIE3810_TIM3_PWMInit(9999, 71);//100Hz
	while(1)
	{
		Delay(1500);
		if (dir) LED0PWMVal++; //increae the value which is going to be loaded CCR2
		else LED0PWMVal--;//decrease the value which is going to be loaded in CCR2
		if (LED0PWMVal > 5000) dir=0;//start to decrease 
		if (LED0PWMVal ==0) dir=1;//start to increase
		LED0_PWM_VAL = LED0PWMVal;//set the value of TIM_CCR2
	}
}

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}


void EIE3810_TIM3_PWMInit(u16 arr, u16 psc)
{
	//load TIM3_CCR2 to actual compare 2 register
	RCC->APB2ENR |=1<<3;//enable GPIOB
	GPIOB->CRL &= 0xFF0FFFFF;//clear bit5
	GPIOB->CRL |= 0x00B00000;//set PB5 to 1011, 50MHz alternate function output Push-pull
	RCC->APB2ENR |= 1<<0;//enable AIFO
	AFIO->MAPR &= 0xFFFFF3FF;//clear bit TIM3_REMAP which is at [11:10]
	AFIO->MAPR |= 1<<11;//set TIM3_REMAP to '10'
	RCC->APB1ENR |= 1<<1;//enable TIM3
	TIM3->ARR = arr;//set auto-reload register
	TIM3->PSC = psc;//set prescaler register
	TIM3->CCMR1 |= 7<<12;//set output compare 2 mode as 0111.
	TIM3->CCMR1 |= 1<<11;//set OC2PE to 1, to enable output compare 2 preload
	TIM3->CCER |= 1<<4;//enable compare 2 output
	TIM3->CR1 = 0x0080;//enable auto-reload preload
	TIM3->CR1 |= 1<<0; //enable counter
}


