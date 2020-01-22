#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_Clock.h"
#include "EIE3810_TFTLCD.h"

void Delay(u32);
void JOYPAD_Init(void);
void JOYPAD_Delay(u16 t);
u8 JOYPAD_Read(void);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
int main(void){
 EIE3810_clock_tree_init();
 EIE3810_LED_Init();
 EIE3810_TFTLCD_Init();
 EIE3810_NVIC_SetPriorityGroup(5);
 JOYPAD_Init();
 EIE3810_TIM3_Init(99, 7199);//100Hz 
  EIE3810_TFTLCD_DrawAll(0,0,WHITE);
 while(1)
 {
  ;
 }
}
void JOYPAD_Init(void)
{
 RCC->APB2ENR|=1<<3;  //Enable GPIOB
 RCC->APB2ENR|=1<<5;  //Enable GPIOE
 GPIOB->CRH&=0XFFFF00FF;  //Clear the bit of the pin11, pin10
 GPIOB->CRH|=0X00003800;  //Set the pin10 as the Input with pull-ip mode, 
                          //and the pin11 with the general purpose output push-pull 
 GPIOB->ODR|=3<<10;   //initialize PB10 PB11 to 1
 GPIOD->CRL&=0XFFFF0FFF; //Clear the bit3
 GPIOD->CRL|=0X00003000; //Set up pin bit3 as the input with pull-up/pull-down
 GPIOD->ODR|=1<<3;    //Enable the input pull-up mode
}
void JOYPAD_DELAY(u16 t)
{
 while(t--);
}
u8 JOYPAD_Read(void)
{
 vu8 temp=0;
 u8 t;
 GPIOB->BSRR |= 1<<11; //Set PB11 to high at the start of reading
 Delay(80); //Delay for a while
 GPIOB->BSRR |= 1<<27; //Set PB27 to high at the start of reading
 for(t=0;t<8;t++)
 {
  temp>>=1; //Move the temp to one right digit
  if((((GPIOB->IDR)>>10)&0x01)==0) temp|=0x80; //detect a low voltage of PB10 and set bit 7-t to 1
  GPIOD->BSRR |= (1<<3);    
  Delay(80);//generate a high voltage in PD3 lasted delay(80) 
  GPIOD->BSRR |=(1<<19);
  Delay(80);//generate a low voltage in PD3 lasted delay(80)
 }
 return temp;
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
 u8 temp = 0;
 if (TIM3->SR & 1<<0){//if update interrupt pending
  temp = JOYPAD_Read();
  if ((temp)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'A',BLUE,WHITE);
  }
  if ((temp>>1)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'B',BLUE,WHITE);
  }
  if ((temp>>2)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'S',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'E',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(220,200,'L',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(230,200,'E',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(240,200,'C',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(250,200,'T',BLUE,WHITE);
  }
  if ((temp>>3)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'S',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'T',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(220,200,'A',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(230,200,'R',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(240,200,'T',BLUE,WHITE);   
  }
  if ((temp>>4)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'U',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'P',BLUE,WHITE);
  }
  if ((temp>>5)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'D',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'O',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(220,200,'W',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(230,200,'N',BLUE,WHITE);
  }
  if ((temp>>6)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'L',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'E',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(220,200,'F',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(230,200,'T',BLUE,WHITE);
  }
  if ((temp>>7)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'R',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(210,200,'I',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(220,200,'G',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(230,200,'H',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar(240,200,'T',BLUE,WHITE);
  }
  
 }
 TIM3->SR &= ~(1<<0);//get rid of the pending
}