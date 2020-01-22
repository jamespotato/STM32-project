#ifndef __EIE3810_TFTLCD_H
#define __EIE3810_TFTLCD_H
#include "stm32f10x.h"
#define BLACK  0x0000
#define WHITE  0xFFFF
#define BLUE  0x001F
#define GREEN  0x07E0
#define RED   0xF800
#define YELLOW 0xFFE0
#define LCD_COMMAND   ((u32) 0x6C000000)
#define LCD_DATA   ((u32) 0x6C000800)
#define LCD_LIGHT_ON GPIOB->BSRR |=0x00000001 //Question: What does this line do?

void EIE3810_TFTLCD_Init(void);
void EIE3810_TFTLCD_SetParameter(void);
void EIE3810_TFTLCD_WrCmdData(u16 cmd,u16 data);
void EIE3810_TFTLCD_WrCmd(u16 cmd);
void EIE3810_TFTLCD_WrData(u16 data);
void EIE3810_TFTLCD_DrawDot(u16 x, u16 y, u16 colors);
void EIE3810_TFTLCD_Clear(u16 color);
void EIE3810_TFTLCD_DrawAll(u16 x, u16 y, u16 color);
void EIE3810_TFTLCD_FillRectangle(u16 start_x, u16 length_x, u16 start_y, u16 length_y, u16 color);
void segement_0(u16 x,u16 y,u16 color);
void segement_1(u16 x,u16 y,u16 color);
void segement_2(u16 x,u16 y,u16 color);
void segement_3(u16 x,u16 y,u16 color);
void segement_4(u16 x,u16 y,u16 color);
void segement_5(u16 x,u16 y,u16 color);
void segement_6(u16 x,u16 y,u16 color);
void segement_7(u16 x,u16 y,u16 color);
void segement_8(u16 x,u16 y,u16 color);
void segement_9(u16 x,u16 y,u16 color);
void EIE3810_TFTLCD_SevenSegment(u16 start_x, u16 start_y, u8 digit, u16 color);
void EIE3810_TFTLCD_ShowChar(u16 x,u16 y, u8 ascii, u16 color, u16 bgcolor);
#endif

