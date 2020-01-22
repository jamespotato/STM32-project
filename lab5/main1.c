#include "stm32f10x.h"
#include "EIE3810_LED.h"
#include "EIE3810_clock.h"


void EIE3810_Key2_EXTIInit(void);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);

	
int main(void)
{
	EIE3810_clock_tree_init();
	EIE3810_LED_Init();
	EIE3810_NVIC_SetPriorityGroup(5); //Set PRIGROUP
	EIE3810_TIM3_Init(4999,7199); //set arr to 4999 and psc to 7199 to flash in 1Hz
	//EIE3810_TIM3_Init(2499,7199);//set arr to 2499 and psc to 7199 to flash in 2Hz
	while(1)
	{
		;
	}
}


void EIE3810_TIM3_Init(u16 arr, u16 psc)
{
	//TIM3
	RCC->APB1ENR|=1<<1;//enable TIM#
	TIM3->ARR=arr;//set TIM3 auto-reload register
	TIM3->PSC=psc;//set prescaler register
	TIM3->DIER|=1<<0;//TIM3 update interrup enable
	TIM3->CR1|=0x01;//couter enable
	NVIC->IP[29]=0x45;//set the priority of TIM3 interrupt to 0100
	NVIC->ISER[0]=(1<<29);//enable interrupt #29
}



void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp1, temp2;
	temp2 = prigroup&0x00000007;
	temp2 <<=8;
	temp1 = SCB->AIRCR;
	temp1 &= 0x0000F8FF;
	temp1 |= 0x05FA0000;
	temp1 |= temp2;
	SCB->AIRCR = temp1;
}

void TIM3_IRQHandler(void)
{
	if (TIM3->SR & 1<<0)//if update interrupt pending
	{
		GPIOB->ODR ^=1<<5;//toggle LED 0 by XOR a 1
	}
	TIM3->SR &= ~(1<<0);//get rid of the pending
}

