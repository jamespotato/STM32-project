#include "stm32f10x.h"
#include "EIE3810_LED.h"
#include "stm32f10x.h"
void Delay(u32);
void EIE3810_Key2_EXTIInit(void);
void EIE3810_KeyUp_EXTIInit(void);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
u32 count = 0;
int main(void)
{
 EIE3810_LED_Init();
 EIE3810_NVIC_SetPriorityGroup(5);
 EIE3810_Key2_EXTIInit();
 EIE3810_KeyUp_EXTIInit();
 DS0_OFF;
 DS1_OFF;
 while(1)
 {
  count++;
 }
}
void Delay(u32 count){
 u32 i;
 for (i=0;i<count;i++);
}
void EIE3810_Key2_EXTIInit(void)
{
 RCC->APB2ENR |= 1<<6;  //Enable the GPIOE
 GPIOE->CRL &= 0xFFFFF0FF; //Clear the bit2
 GPIOE->CRL |=0x00000800; //Set it as input pull-up mode
 GPIOE->ODR |= 1<<2; //set PE2 to set original 
 RCC->APB2ENR |= 0x01; //Enable AFIO
 AFIO->EXTICR[0] &= 0xFFFFF0FF;  //Clear the  EXIT2[3:0]
 AFIO->EXTICR[0] |= 0x00000400; //Select the PE2 pin as the source input for EXTIx external interrupt
 EXTI->IMR |= 1<<2;  //Make the interrupt request from Line2 i s not masked
 EXTI->FTSR |= 1<<2; //enable faliing trigger, RTSR enable rising trigger 
 NVIC->IP[8] = 0x65; //set interrupt #8 with priority 0110
 NVIC->ISER[0] |= (1<<8);  //Set the Interrupt Set-Enable Registeer to enable EXTI2. (interrupy #8)
}
void EIE3810_KeyUp_EXTIInit(void)
{
 RCC->APB2ENR |= 1<<2;//enable GPIOA
 GPIOA->CRL &=0xFFFFFFF0;//reset PA0
 GPIOA->CRL |=0x00000008;//set PA0 to input pull-down mode
 GPIOA->ODR &=0xE; //set the original status of PA0 to "reset" by & 1110
 RCC->APB2ENR |=0x01; //enable AFIO
 AFIO->EXTICR[0] &= 0xFFFFFFF0; //reset EXIT0
 AFIO->EXTICR[0] |= 0x00000000; //select PA0 pin as the input for EXTI0
 EXTI->IMR |= 1<<0; //demask the interrupt request from line 2
 EXTI->FTSR |= 1<<0;//enable falling trigger
 NVIC->IP[6] = 0x35;  //EXTI0 is interrupt 6, in exp2,its preempt priority is 0x75
 NVIC->ISER[0] |= 1<<6;  // Set Interrupt Set-Enable Register to enable
}

void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
 u32 temp1, temp2;
 temp2 = prigroup&0x00000007;//only 3 bits are valid
 temp2 <<=8;//3 bit are [10:8], start from bit-8
 temp1 = SCB->AIRCR;//read the data from AIRCR
 temp1 &= 0x0000F8FF;//reset bit [10:8]
 temp1 |= 0x05FA0000;//provide a rigister key as 0x5FA in the VECTKEY field. 
      //Otherwise the write value is ignored
 temp1 |= temp2;// set bit [10:8] as sepcified priority group number
 SCB->AIRCR = temp1;// write back to the AIRCR
}
void EXTI2_IRQHandler(void)
{
 u8 i;
 for (i=0;i<10;i++)
 {
  DS0_ON;
  Delay(3000000);
  DS0_OFF;
  Delay(3000000);
 }
 EXTI->PR = 1<<2; //clear the pending bit PR2 by writing a '1' into it
}
void EXTI0_IRQHandler(void)
{
 u8 i;
 for (i=0;i<10;i++)
 {
  DS1_ON;
  Delay(3000000);
  DS1_OFF;
  Delay(3000000);
 }
 EXTI->PR = 1<<0; //clear the pending bit PR2 by writing a '1' into it
}