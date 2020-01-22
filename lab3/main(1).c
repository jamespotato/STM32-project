#include "stm32f10x.h"
#include "EIE3810_TFTLCD.h"

void Delay(u32);
void exp1();
void exp2();
void exp3();
void exp4();

int main(void)
{
 EIE3810_TFTLCD_Init();
 Delay(1000000);
 EIE3810_TFTLCD_WrCmd(0x3600);
 EIE3810_TFTLCD_WrData(0x00);
 EIE3810_TFTLCD_DrawAll(0,0,WHITE);
 exp1();
 exp2();
// exp3();
 exp4();
}
void exp1(){
 int i;
 for (i=0;i<20;i++){
  EIE3810_TFTLCD_DrawDot(10+i,10,BLACK);
  EIE3810_TFTLCD_DrawDot(10+i,20,BLUE);
  EIE3810_TFTLCD_DrawDot(10+i,30,GREEN);
  EIE3810_TFTLCD_DrawDot(10+i,40,RED);
  EIE3810_TFTLCD_DrawDot(10+i,50,BLUE);
 }
}
void exp2(){
 EIE3810_TFTLCD_FillRectangle(100,100,100,100,YELLOW);
}

void exp3(){
 while(1) 
 {
  int i;
  for (i=9;i>=0;i--){ 
   EIE3810_TFTLCD_SevenSegment(202, 470, i, GREEN);
   Delay(10000000);
   EIE3810_TFTLCD_FillRectangle(202,76,330,141,WHITE);
 }
  for (i=0;i<10;i++){
   EIE3810_TFTLCD_SevenSegment(202, 470, i, YELLOW);
   Delay(10000000);
   EIE3810_TFTLCD_FillRectangle(202,76,330,141,WHITE);
  }
 }
}

void exp4(){
 char cArr[] = {'1','1','6','0','1','0','0','6','0'};
 
 for (int i=0; i< sizeof(cArr);i++){
   EIE3810_TFTLCD_ShowChar(400+8*i,500,cArr[i],BLUE,YELLOW);
 }
}


void Delay(u32 count)
{
 u32 i;
 for (i=0;i<count;i++);
}
