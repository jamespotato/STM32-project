#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_Clock.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_Key.h"
#include "EIE3810_USART.h"
#include "EIE3810_Buzzer.h"
void Delay(u32);
void JOYPAD_Init(void);
void JOYPAD_Delay(u16 t);
u8 JOYPAD_Read(void);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void EIE3810_TIM4_Init(u16 arr, u16 psc);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
void upperright(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed);
void upperleft(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed);
void downright(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed);
void downleft(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed);
void Show_first_page(void);
void Show_second_page(void);
void Show_third_page(void);
void Show_fourth_page(void);
int Choose_difficulty(void);
u16 ReceiveNumber(void);
static int bux = 200;
static int bdx = 200;
static int round = 0;
static int second = 0;
static int minute = 0;
static int start = 0;
static int count = 0;
static int uppower = 0;
static int downpower = 0;
int main(void){
 EIE3810_clock_tree_init();
 EIE3810_LED_Init();
 EIE3810_Buzzer_Init();
 EIE3810_TFTLCD_Init();
 EIE3810_NVIC_SetPriorityGroup(5);
 JOYPAD_Init();
 EIE3810_USART1_init(72,4800);
 EIE3810_TFTLCD_DrawAll(0,0,WHITE);
 EIE3810_Key_Init();
 EIE3810_TIM3_Init(49, 7199);
 EIE3810_TIM4_Init(999, 7199);

 while(1){
 start= 0;
 bux = 200;
 bdx = 200;
 round = 0;

 EIE3810_TFTLCD_DrawAll(0,0,WHITE);
  Show_first_page();
 Delay(20000000);
 EIE3810_TFTLCD_FillRectangle(0,480,200,400,WHITE);
 Show_second_page();
 Delay(5000000);
 int level = Choose_difficulty();
 EIE3810_TFTLCD_FillRectangle(0,480,200,500,WHITE);
 Delay(5000);
 Show_third_page();
 Delay(20000000);

  //upperright(120,700,20,7,RED,5);
 EIE3810_TFTLCD_FillRectangle(0,480,790,10,WHITE);
 EIE3810_TFTLCD_FillRectangle(0,480,0,10,WHITE);
 EIE3810_TFTLCD_FillRectangle(200,80,790,10,BLUE);
  EIE3810_TFTLCD_FillRectangle(200,80,0,10,BLUE);
 u16 dir =  ReceiveNumber();
 char t1[]={'T','h','e',' ','r','a','n','d','o','m',' ','n','u','m','b','e','r',' ','i','s',':',' ',dir+48};
 for(int i=0;i< sizeof(t1);i++)
  {
   EIE3810_TFTLCD_ShowChar(400-8*i, 300, t1[sizeof(t1)-i-1], WHITE, RED);
  }

 Delay(20000000);
 EIE3810_TFTLCD_FillRectangle(10,460,200,300,WHITE);
 Delay(5000);
 Show_fourth_page();
  start = 1;
 if (level == 1){
  upperleft(240,780,10,dir,YELLOW,3);
  }
 if (level == 0){
  upperleft(240,780,10,dir,YELLOW,5);
 }
}
}
int Choose_difficulty(void)
{
 while (1) {

 int level = 1;
 if(readKey_up()==0x0001)                   //  choose easy
   {
    char t2[]={'E','a','s','y'};
    char t3[]={'H','a','r','d'};
    for(int i=0;i< sizeof(t2);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(200-12*i, 250, t2[sizeof(t2)-i-1], WHITE, BLUE);
    }
    for(int i=0;i< sizeof(t3);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(200-12*i, 300, t3[sizeof(t3)-i-1], BLUE , WHITE);
    }
    //multiplier=1;
    int level = 0;

   }
   if(readKey1()==0x0000)                    //  choose hard
   {
    char t2[]={'E','a','s','y'};
    char t3[]={'H','a','r','d'};
    for(int i=0;i< sizeof(t2);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(200-12*i, 250, t2[sizeof(t2)-i-1], BLUE, WHITE);
    }
    for(int i=0;i< sizeof(t3);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(200-12*i, 300, t3[sizeof(t3)-i-1], WHITE , BLUE);
    }
    int level = 1;
   }
   if(readKey0()==0x0000)
   {
    if (level == 0){
       return(0);
    }
    if (level == 1){
      return(1);
    }
    break;
   }
  }
 }
u16 ReceiveNumber(void)
{
 u32 buffer;
 while(1){

 if (USART1->SR & (1<<5))
 {
    buffer = USART1->DR;
   if(buffer==0){
    return (0);
    break;
   }
   else if (buffer ==1){
    return (1);
    break;
   }
   else if (buffer ==2){
    return (2);
    break;
   }
   else if (buffer ==3){
    return (3);
    break;
   }
   else if (buffer ==4){
    return (4);
    break;
   }
   else if (buffer ==5){
    return (5);
    break;
   }
   else if (buffer ==6){
    return (6);
    break;
   }
   else if (buffer ==7){
    return (7);
    break;
   }


 }
 }
}

void Show_first_page(void)
{
 char s1[]={'W','e','l','c','o','m','e',' ','t','o',' ','m','i','n','i',' ','P','r','o','j','e','c','t','!'};
 char s2[]={'T','h','i','s',' ','i','s',' ','t','h','e',' ','F','i','n','a','l',' ','L','a','b','.'};
 char s3[]={'A','r','e',' ','y','o','u',' ','r','e','a','d','y','?'};
 char s4[]={'O','K','!',' ','L','e','t','\'','s',' ','s','t','a','r','t','.'};
 for(int i=0;i< sizeof(s1);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(400-12*i, 200, s1[sizeof(s1)-i-1], WHITE, BLUE);
  }
 for(int i=0;i< sizeof(s2);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(400-12*i, 250, s2[sizeof(s2)-i-1], WHITE, RED);
  }
 for(int i=0;i< sizeof(s3);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(400-12*i, 300, s3[sizeof(s3)-i-1], WHITE, RED);
  }
 for(int i=0;i< sizeof(s4);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(400-12*i, 350, s4[sizeof(s4)-i-1], WHITE, RED);
  }
}
void Show_second_page(void)
{
 char t1[]={'s','e','l','e','c','t',' ','d','i','f','f','i','c','u','l','t','y',' ','l','e','v','e','l',':'};
 char t2[]={'E','a','s','y'};
 char t3[]={'H','a','r','d'};
 char t4[]={'P','r','e','s','s',' ','K','E','Y','0',' ','t','o',' ','e','n','t','e','r','.'};
 for(int i=0;i< sizeof(t1);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(300-12*i, 200, t1[sizeof(t1)-i-1], WHITE, RED);
  }
 for(int i=0;i< sizeof(t2);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(200-12*i, 250, t2[sizeof(t2)-i-1], WHITE, BLUE);
  }
 for(int i=0;i< sizeof(t3);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(200-12*i, 300, t3[sizeof(t3)-i-1], BLUE, WHITE);
  }
 for(int i=0;i< sizeof(t4);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(350-12*i, 350, t4[sizeof(t4)-i-1], WHITE, RED);
  }
 }
void Show_third_page(void)
{
 EIE3810_TFTLCD_FillRectangle(0,480,100,500,WHITE);
 char m[]={'U','s','e',' ','U','S','A','R','T',' ','f','o','r',' ','a',' ','r','a','n','d','o','m',' ','d','i','r','e','c','t','i','o','n','.'};
 for(int i=0;i< sizeof(m);i++)
  {
   EIE3810_TFTLCD_ShowChar2412(400-12*i, 200, m[sizeof(m)-i-1], WHITE, RED);
  }
}
void Show_fourth_page(void)
{
 int r1 = round/10;
 int r2 = round%10;

 EIE3810_TFTLCD_FillRectangle(80,30,380,10,WHITE);
char m[]={'R','O','U','N','D',':',' ',r1+48,r2+48};
  for(int i=0;i< sizeof(m);i++)
 {
  EIE3810_TFTLCD_ShowChar(5+10*i, 385, m[i], BLUE, WHITE);
 }

 char h[]={'T','I','M','E',':',' '};
 for(int i=0;i< sizeof(h);i++)
 {
  EIE3810_TFTLCD_ShowChar(5+10*i, 415, h[i], BLUE, WHITE);
 }

}
void downright(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed){
 int xc,yc,radius;
 xc = x;
 yc = y;
 radius = r;
 toggleBuzzer();
 Delay(1000001);
 toggleBuzzer();
 while (1){
   if (xc <= 480- radius && yc<790-radius){
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,color);
    Delay((10-speed)*30000);
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,WHITE);
    if (randnum == 0){
     xc = xc + 1;
     yc = yc + 1;}
    if (randnum == 1){
     xc = xc + 1;
     yc = yc + 2;}
    if (randnum == 2){
     xc = xc + 1;
     yc = yc + 3;}
    if (randnum == 3){
     xc = xc + 2;
     yc = yc + 1;}
    if (randnum == 4){
     xc = xc + 2;
     yc = yc + 3;}
    if (randnum == 5){
     xc = xc + 4;
     yc = yc + 3;}
    if (randnum == 6){
     xc = xc + 2;
     yc = yc + 5;}
    if (randnum == 7){
     xc = xc + 5;
     yc = yc + 3;}
   }
   else if (xc>= 480-radius && yc<790-radius){
     downleft(xc,yc,r,randnum,color,speed);
   break;
    }
    else if (yc>=790-radius){
      if (bdx-5<=xc && bdx>=xc-85){
    round +=1;
        upperright(xc,yc,r,randnum,color,speed);
    break;
      }
      else
      {
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    char s1[]={'J','O','P','A','D',' ','W','I','N','!'};
    for(int i=0;i< sizeof(s1);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(400-12*i, 400, s1[sizeof(s1)-i-1], WHITE, BLUE);
    }
    Delay(20000000);
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    Delay(500);
    break;
      }
    }
   }
}

void downleft(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed){
 toggleBuzzer();
 Delay(1000001);
 toggleBuzzer();
 int xc,yc,radius;
 xc = x;
 yc = y;
 radius = r;
 while (1){
   if (xc >= radius && yc<=790-radius){
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,color);
    Delay((10-speed)*30000);
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,WHITE);
    if (randnum == 0){
     xc = xc - 1;
     yc = yc + 1;}
    if (randnum == 1){
     xc = xc - 1;
     yc = yc + 2;}
    if (randnum == 2){
     xc = xc - 1;
     yc = yc + 3;}
    if (randnum == 3){
     xc = xc - 2;
     yc = yc + 1;}
    if (randnum == 4){
     xc = xc - 2;
     yc = yc + 3;}
    if (randnum == 5){
     xc = xc - 4;
     yc = yc + 3;}
    if (randnum == 6){
     xc = xc - 2;
     yc = yc + 5;}
    if (randnum == 7){
     xc = xc - 5;
     yc = yc + 3;}
   }

   else if (xc<=radius && yc<790-radius){
     downright(xc,yc,r,randnum,color,speed);
   break;
    }
   else if (yc>=790-radius){
      if (bdx-5<=xc && bdx>=xc-85){
    round +=1;
        upperleft(xc,yc,r,randnum,color,speed);
    break;
      }
      else
      {
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    char s1[]={'J','O','Y','P','A','D',' ','W','I','N','!'};
    for(int i=0;i< sizeof(s1);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(400-12*i, 400, s1[sizeof(s1)-i-1], WHITE, BLUE);
    }
    Delay(20000000);
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    Delay(500);
    break;
      }
    }

   }
}
void upperright(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed){
 int xc,yc,radius;
 xc = x;
 yc = y;
 radius = r;
 toggleBuzzer();
 Delay(1000001);
 toggleBuzzer();
 while (1){
   if (xc <= 480- radius && yc>=radius+10){
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,color);
    Delay((10-speed)*30000);
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,WHITE);
    if (randnum == 0){
     xc = xc + 1;
     yc = yc - 1;}
    if (randnum == 1){
     xc = xc + 1;
     yc = yc - 2;}
    if (randnum == 2){
     xc = xc + 1;
     yc = yc - 3;}
    if (randnum == 3){
     xc = xc + 2;
     yc = yc - 1;}
    if (randnum == 4){
     xc = xc + 2;
     yc = yc - 3;}
    if (randnum == 5){
     xc = xc + 4;
     yc = yc - 3;}
    if (randnum == 6){
     xc = xc + 2;
     yc = yc - 5;}
    if (randnum == 7){
     xc = xc + 5;
     yc = yc - 3;}
   }
   else if (xc>= 480-radius && yc>radius+10){
     upperleft(xc,yc,r,randnum,color,speed);
   break;
    }
   else if (yc<=radius+10){
      if (bux-5<=xc && bux>=xc-85){
    round +=1;
        downright(xc,yc,r,randnum,color,speed);
    break;
      }
      else
      {
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    char s1[]={'K','E','Y','B','O','A','R','D',' ','W','I','N','!'};
    for(int i=0;i< sizeof(s1);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(400-12*i, 400, s1[sizeof(s1)-i-1], WHITE, BLUE);
    }
    Delay(20000000);
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    Delay(500);
    break;
      }
    }
   }
}
void upperleft(u16 x, u16 y, u8 r, u16 randnum, u16 color, u16 speed){
 int xc,yc,radius;
 xc = x;
 yc = y;
 radius = r;
 toggleBuzzer();
 Delay(1000001);
 toggleBuzzer();
 while (1){

   if (xc >= radius && yc>=radius+10){
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,color);
    Delay((10-speed)*30000);
    EIE3810_TFTLCD_DrawCircle(xc,yc,r,1,WHITE);
    if (randnum == 0){
     xc = xc - 1;
     yc = yc - 1;}
    if (randnum == 1){
     xc = xc - 1;
     yc = yc - 2;}
    if (randnum == 2){
     xc = xc - 1;
     yc = yc - 3;}
    if (randnum == 3){
     xc = xc - 2;
     yc = yc - 1;}
    if (randnum == 4){
     xc = xc - 2;
     yc = yc - 3;}
    if (randnum == 5){
     xc = xc - 4;
     yc = yc - 3;}
    if (randnum == 6){
     xc = xc - 2;
     yc = yc - 5;}
    if (randnum == 7){
     xc = xc - 3;
     yc = yc - 5;}}
   else if (xc<= radius && yc>radius+10){
     upperright(xc,yc,r,randnum,color,speed);
   break;
    }
   else if (yc<=radius+10){
      if (bux-5<=xc && bux>=xc-85){
    round +=1;
        downleft(xc,yc,r,randnum,color,speed);
    break;
      }
      else
      {
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    char s1[]={'K','E','Y','B','O','A','R','D',' ','W','I','N','!'};
    for(int i=0;i< sizeof(s1);i++)
    {
     EIE3810_TFTLCD_ShowChar2412(400-12*i, 400, s1[sizeof(s1)-i-1], WHITE, BLUE);
    }
    Delay(20000000);
    EIE3810_TFTLCD_DrawAll(0,0,WHITE);
    Delay(500);
    break;
      }
    }

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
  }
  if ((temp>>3)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'S',BLUE,WHITE);
  }
  if ((temp>>4)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'U',BLUE,WHITE);
  }
  if ((temp>>5)&0x01){
   EIE3810_TFTLCD_DrawAll(0,0,WHITE);
   EIE3810_TFTLCD_ShowChar(200,200,'D',BLUE,WHITE);
  }
  if ((temp>>6)&0x01){
   EIE3810_TFTLCD_FillRectangle(bux+10,80,0,10,WHITE);
   if (bux >=0){
  bux-=1;}
   EIE3810_TFTLCD_FillRectangle(bux,80,0,10,BLUE);
  }
  if ((temp>>7)&0x01){
   EIE3810_TFTLCD_FillRectangle(bux-10,80,0,10,WHITE);
  if (bux <=400){
    bux+=1;}
   EIE3810_TFTLCD_FillRectangle(bux,80,0,10,BLUE);
  }
 if (readKey0() == 0x0000){
   downpower = 0;
    EIE3810_TFTLCD_FillRectangle(bdx-10,80,790,10,WHITE);
     if (bdx <=400){
         bdx+=1;}
     EIE3810_TFTLCD_FillRectangle(bdx,80,790,10,BLUE);
 }

 if (readKey2() == 0x0000){
   downpower = 0;
    EIE3810_TFTLCD_FillRectangle(bdx+10,80,790,10,WHITE);
     if (bdx >=0){
         bdx-=1;}
     EIE3810_TFTLCD_FillRectangle(bdx,80,790,10,BLUE);
 }

 if (readKey1() == 0x0000){
   EIE3810_TFTLCD_ShowChar2412(200,400,'P',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar2412(215,400,'A',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar2412(230,400,'U',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar2412(250,400,'S',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar2412(270,400,'E',BLUE,WHITE);
   while(1)
   {
     if (readKey_up() == 0x0001){
       EIE3810_TFTLCD_FillRectangle(200,100,390,410,WHITE);
      break;
        }
    }


 }


 }
 TIM3->SR &= ~(1<<0);//get rid of the pending
}
void EIE3810_TIM4_Init(u16 arr, u16 psc)
{
//TIM3
RCC->APB1ENR|=1<<2;//enable TIM3
TIM4->ARR=arr;//set TIM3 auto-reload register
TIM4->PSC=psc;//set prescaler register
TIM4->DIER|=1<<0;//TIM3 update interrup enable
TIM4->CR1|=0x01;//couter enable
NVIC->IP[30]=0x35;//set the priority of TIM3 interrupt to 0100
NVIC->ISER[0]=(1<<30);//enable interrupt #29
}
void TIM4_IRQHandler(void)
{
 u8 temp = 0;
 if (start ==1){
  count+=1;
  Show_fourth_page();


 if (count == 10){
   EIE3810_TFTLCD_FillRectangle(70,30,380,60,WHITE);
  second +=1;
  count = 0;}
   if (second==60){
  minute+=1;
  second = 0;
  }

  int m1 = minute/10;
  int m2 = minute%10;
  int s1 = second/10;
  int s2 = second%10;

  EIE3810_TFTLCD_ShowChar(70,415,m1+48,BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar(80,415,m2+48,BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar(90,415,':',BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar(100,415,s1+48,BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar(110,415,s2+48,BLUE,WHITE);


  temp = JOYPAD_Read();

  if ((temp>>3)&0x01){
   EIE3810_TFTLCD_ShowChar2412(200,400,'P',BLUE,WHITE);
   EIE3810_TFTLCD_ShowChar2412(215,400,'A',BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar2412(230,400,'U',BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar2412(250,400,'S',BLUE,WHITE);
  EIE3810_TFTLCD_ShowChar2412(270,400,'E',BLUE,WHITE);
  while(1)
   {
   Delay(500000);
     temp = JOYPAD_Read();
     if ((temp>>3)&0x01){
       EIE3810_TFTLCD_FillRectangle(200,100,390,410,WHITE);
      break;
        }
    }
  }


 }
TIM4->SR &= ~(1<<0);//get rid of the pending
}