//USART1->CR1=0x202C; //enable USART1,set word length(1 start bit, 8 data bit, n stop bit);
      //disable parity, enable transmitter by assigning 0b0010 0001 0000 1100 to CR1

/***********************************************************************/


#include "stm32f10x.h"
#include "EIE3810_LED.h"
#include "stm32f10x.h"
#include "EIE3810_USART.h"
#include "EIE3810_Clock.h"
#include "EIE3810_TFTLCD.h"

void Delay(u32);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
void EIE3810_USART1_EXTIInit(void);
void USART1_IRQHandler(void);
u32 count = 0;
int main(void)
{
  EIE3810_clock_tree_init();
 EIE3810_LED_Init();
 EIE3810_TFTLCD_Init();
 Delay(500000);
 EIE3810_TFTLCD_DrawAll(0,0,WHITE);
 EIE3810_NVIC_SetPriorityGroup(5);
 EIE3810_USART1_init(72,9600);
 EIE3810_USART1_EXTIInit();
 USART_print(1,"1234567890");
 
 while(1)
 {
  USART_print(1,"EIE3810_Lab4");
  while (!((USART1->SR>>7)&0x1));
  Delay(1000000);
 }
}
void Delay(u32 count){
 u32 i;
 for (i=0;i<count;i++);
}

void EIE3810_USART1_EXTIInit(void)
{
 NVIC->IP[37] = 0x65;
 NVIC->ISER[1] |= 1<<5;
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
void USART1_IRQHandler(void)
{
 u32 buffer;
 if (USART1->SR &(1<<5))
 {
  buffer = USART1->DR;
  if (buffer == 'Q'){ TurnOnLED0(); EIE3810_TFTLCD_ShowChar(250,300,'Q',YELLOW,GREEN);}
  else if(buffer == 'H'){TurnOffLED0(); EIE3810_TFTLCD_ShowChar(250,300,'H',YELLOW,GREEN);}
  else if(buffer == 'R'){
   EIE3810_TFTLCD_ShowChar(250,300,'R',YELLOW,GREEN);
   for (int i=15;i>=0;i--){
    if (i%2 == 1){
     if (i>9){
      EIE3810_TFTLCD_ShowChar(40+16*(15-i),100,'1',RED,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),100, i+38,RED,WHITE);
      EIE3810_TFTLCD_ShowChar(40+16*(15-i),200,'1',BLUE,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),200, i+38,BLUE,WHITE);
      
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),75,((USART1->CR1 >> i)&0x01)+48,RED,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),175,((USART1->DR >> i)&0x01)+48,BLUE,WHITE);
     }
     
     else{
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),100,i+48,RED,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),200,i+48,BLUE,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),75,((USART1->CR1 >> i)&0x01)+48,RED,WHITE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),175,((USART1->DR >> i)&0x01)+48,BLUE,WHITE);
     }
    
    }
    
    else if (i%2 == 0){
     if (i>9){
      EIE3810_TFTLCD_ShowChar(40+16*(15-i),100,'1',WHITE,RED);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),100, i+38,WHITE,RED);
      EIE3810_TFTLCD_ShowChar(40+16*(15-i),200,'1',WHITE,BLUE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),200, i+38,WHITE,BLUE);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),75,((USART1->CR1 >> i)&0x01)+48,WHITE,RED);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),175,((USART1->DR >> i)&0x01)+48,WHITE,BLUE);
     }
     
     else{
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),100,i+48,WHITE,RED);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),200,i+48,WHITE,BLUE); 
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),75,((USART1->CR1 >> i)&0x01)+48,WHITE,RED);
      EIE3810_TFTLCD_ShowChar(48+16*(15-i),175,((USART1->DR >> i)&0x01)+48,WHITE,BLUE);      
     }
    }
   }
  
  
  
  
  
  
  
  }
 }
}
