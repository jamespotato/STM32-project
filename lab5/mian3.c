#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_clock.h"
u8 task1HeartBeat;
u8 LED0couter = 0;
u8 LED1couter = 0;
void EIE3810_SYSTICK_Init(void);
void EIE3810_Key2_EXTIInit(void);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);

int main(void){
 
 task1HeartBeat=0;
 EIE3810_clock_tree_init();
 EIE3810_SYSTICK_Init();
 EIE3810_NVIC_SetPriorityGroup(5);
 EIE3810_LED_Init();
 while(1){
 if (task1HeartBeat>=10){
  task1HeartBeat = 0;

  if (LED0couter==10){
   GPIOB->ODR ^=1<<5;//toggle LED 0 by XOR a 1
   LED0couter = 0;
  }
  if (LED1couter==25){
   GPIOE->ODR ^=1<<5;//toggle LED 0 by XOR a 1
   LED1couter = 0;
  } 

  LED0couter++;
  LED1couter++;

  
 }
 
 
 }
 
}
void EIE3810_SYSTICK_Init()
{
 //SYSTICK
 SysTick->CTRL = 0;//Clear SysTick->CTRL setting
 SysTick->LOAD = 8999;//tick every 9000 clock pulses,72000000/8/9000=1000
 SysTick->CTRL = 0x00010003; //COUNTFLAG set to 1; clocksource=0,FCLK/8;
        //counter enable with pending
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