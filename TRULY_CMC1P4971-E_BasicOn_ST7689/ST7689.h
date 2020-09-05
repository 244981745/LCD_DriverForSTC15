#ifndef	_LCD_ST7689_H
#define	_LCD_ST7689_H
#include <STC15.H>

sbit	CS		=	P1^2;
sbit	MOSI	=	P1^3;
sbit	MISO	=	P1^4;
sbit	SCK		=	P1^5;
sbit	DC		=	P1^6;
sbit	BL		=	P1^6;

#define	SCREEN_X_MAX	128
#define	SCREEN_Y_MAX	160

#define SPIF        0x80        //SPSTAT.7                                
#define WCOL        0x40        //SPSTAT.6      
#define SSIG        0x80        //SPCTL.7                                 
#define SPEN        0x40        //SPCTL.6                                 
#define DORD        0x20        //SPCTL.5    


#define	u16	unsigned short

#define WHITE						0xFFFF
#define BLACK						0x0000	  
#define BLUE						0x001F  
#define BRED						0XF81F
#define GRED						0XFFE0
#define GBLUE						0X07FF
#define RED							0xF800
#define MAGENTA					0xF81F
#define GREEN						0x07E0
#define CYAN						0x7FFF
#define YELLOW					0xFFE0
#define BROWN						0XBC40 //棕色
#define BRRED						0XFC07 //棕红色
#define GRAY						0X8430 //灰色

void SpiPortInit(short port);

unsigned char LCD_WR_ByteSPI(unsigned char byte);
void LCD_WriteData(unsigned short dat); //写数据
void LCD_WriteByteSPI(unsigned char byte); //写一个8bit的数据
void LCD_WriteRegIndex(unsigned char Index); //写命令的参数
	
void LCD_Initial(void);

//void LCD_SetCursor(u16 x,u16 y);
void SPILCD_SetWindow(unsigned short xstat,unsigned short xend,unsigned short ystat,unsigned short yend);
void LCD_Clear(void);
void LCD_Fill_Pic(u16 x, u16 y, unsigned short pic_Width,unsigned short pic_Height,const unsigned char* pic);

void DrawPixel(u16 x, u16 y, int Color);
void SPILCD_ShowChar(unsigned short x,unsigned short y,unsigned char num, unsigned int fColor, unsigned int bColor,unsigned char flag);
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag);
//void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag);



#endif
 
