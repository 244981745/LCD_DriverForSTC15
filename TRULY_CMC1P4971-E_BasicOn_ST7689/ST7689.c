#include "ST7689.h"
#include "ASCII.h"
#include <intrins.h>


void Delayms(unsigned short time)  //延时函数 @24.0MHz
{
	unsigned char i, j,k;

	switch(time)
	{
		case	2:
			{
				i = 47;j = 174;
				do{while (--j);} while (--i);
			}
			break;
		case	20:
			{
				_nop_();
				_nop_();
				i = 2;j = 211;k = 231;
				do{do{while (--k);} while (--j);} while (--i);
			}
			break;
		case	150:
			{
				_nop_();
				_nop_();
				i = 14;j = 174;k = 224;
				do{do{while (--k);} while (--j);} while (--i);
			}
			break;
		case	200:
			{
				_nop_();
				_nop_();
				i = 19;j = 62;k = 43;
				do{do{while (--k);} while (--j);} while (--i);
			}
			break;
		default:break;
	}
}

void SpiPortInit(short port)
{
    SPDAT		= 0;                  //初始化SPI数据
    SPSTAT	= SPIF | WCOL;       //清除SPI状态位
	if(1	==	port)
	{
		P1M0	=	0x00;
		P1M1	=	0x00;
		SPCTL	=	0xD8;
		AUXR1	&=	0xf3;
	}
	else if(2	==	port)
	{
		P2M0	=	0x00;
		P2M1	=	0x00;
		SPCTL	=	0xD8;
		AUXR1	&=	0xf7;
	}
	else if(3	==	port)
	{
		P4M0	=	0x00;
		P4M1	=	0x00;
		P5M0	=	0x00;
		P5M1	=	0x00;
		SPCTL	=	0xD8;
		AUXR1	&=	0xfb;
	}

}


void LCD_WriteByteSPI(unsigned char byte) //写一个8bit的数据
{
		CS		=	0;				//拉低从机的SS
		SPDAT	=	byte;			//触发SPI发送数据
		while (!(SPSTAT & SPIF));	//等待发送完成
		SPSTAT	=	SPIF | WCOL;		//清除SPI状态位
		CS		=	1;				//拉高从机的SS
}

void LCD_WriteData(unsigned short dat) //写数据
{
		DC	=	1;
		LCD_WriteByteSPI(dat>>8);		//	start byte RS=1,RW=0----Write a GRAM data
		LCD_WriteByteSPI(dat);
}

void LCD_WriteRegIndex(unsigned char Index) //写命令的参数
{
		DC	=	0;
		LCD_WriteByteSPI(Index);
		DC	=	1;
}


void LCD_Initial(void) //LCD初始化函数
{
		//复位
		CS	=	1;
		Delayms(200);//等待复位完成
		CS	=	0;
		Delayms(2);
		BL	=	1;//打开背光
	//************* Start Initial Sequence **********//
		
		LCD_WriteRegIndex(0x01);
		Delayms(150);
		//--------disable auto read + manual read once
		LCD_WriteRegIndex(0xD7);
		LCD_WriteByteSPI(0xDF);
		LCD_WriteRegIndex(0xE0);
		LCD_WriteByteSPI(0x00);
		Delayms(20);
		LCD_WriteRegIndex(0xE3);
		Delayms(20);
		LCD_WriteRegIndex(0xE1);
		//--------sleep out
		LCD_WriteRegIndex(0x28);
		LCD_WriteRegIndex(0x11);
		Delayms(200);
		//--------write contrast: default 3F
		LCD_WriteRegIndex(0x25);
		LCD_WriteByteSPI(0x3b);//3F 38
		//--------Vop setting
		LCD_WriteRegIndex(0xC0);
		LCD_WriteByteSPI(0x31);//34
		LCD_WriteByteSPI(0x01);
		//--------C3: bias setting, 1/10
		LCD_WriteRegIndex(0xC3);
		LCD_WriteByteSPI(0x04);
		//--------C4: Booster, X7
		LCD_WriteRegIndex(0xC4);
		LCD_WriteByteSPI(0x07);
		//--------CB: Vg with x2 control
		LCD_WriteRegIndex(0xCB);
		LCD_WriteByteSPI(0x01);
		//--------B7: com/seg scan direction for glass layout
		LCD_WriteRegIndex(0xB7);
		LCD_WriteByteSPI(0x00);
		//--------D0: analog circuit setting
		LCD_WriteRegIndex(0xD0);
		LCD_WriteByteSPI(0x1D);
		//--------BC: idle image saving mode
		LCD_WriteRegIndex(0xBC);
		LCD_WriteByteSPI(0x04);
		//--------BD: Display Compensation Step
		LCD_WriteRegIndex(0xBD);
		LCD_WriteByteSPI(0x00);
		//--------D7: auto read set
		LCD_WriteRegIndex(0xD7);
		LCD_WriteByteSPI(0xDF);
		//--------B5: N-line inversion set
		LCD_WriteRegIndex(0xB5);
		LCD_WriteByteSPI(0x85);//85
		//--------F0: Frame set
		LCD_WriteRegIndex(0xF0);
		LCD_WriteByteSPI(0x09);//06
		LCD_WriteByteSPI(0x11);//0B
		LCD_WriteByteSPI(0x13);//0D
		LCD_WriteByteSPI(0x15);//12
		//--------B4: partial saving power mode
		LCD_WriteRegIndex(0xB4);
		LCD_WriteByteSPI(0x18);
		//--------20: display inversion off
		LCD_WriteRegIndex(0x20);
		//--------2A: colomn address set
		LCD_WriteRegIndex(0x2A);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x7f);
		//--------2B: row address set
		LCD_WriteRegIndex(0x2B);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x9f);
		//--------36: memory address control
		LCD_WriteRegIndex(0x36);
		LCD_WriteByteSPI(0xc8);
		//--------3A: 16bit 65K
		LCD_WriteRegIndex(0x3A);
		LCD_WriteByteSPI(0x05);
		//--------BB: 
		LCD_WriteRegIndex(0xBB);
		LCD_WriteByteSPI(0x26);
		//--------B0:duty 1/162
		LCD_WriteRegIndex(0xB0);
		LCD_WriteByteSPI(0xA1);
		//--------F4:temperature set
		LCD_WriteRegIndex(0xF4);
		LCD_WriteByteSPI(0x42);
		LCD_WriteByteSPI(0x04);
		LCD_WriteByteSPI(0x01);
		LCD_WriteByteSPI(0x01);
		LCD_WriteByteSPI(0x23);
		LCD_WriteByteSPI(0x41);
		LCD_WriteByteSPI(0x21);
		LCD_WriteByteSPI(0x53);
		//--------F9: Gamma table
		LCD_WriteRegIndex(0xF9);
		LCD_WriteByteSPI(0x00);
		LCD_WriteByteSPI(0x03);
		LCD_WriteByteSPI(0x05);
		LCD_WriteByteSPI(0x07);
		LCD_WriteByteSPI(0x09);
		LCD_WriteByteSPI(0x0b);
		LCD_WriteByteSPI(0x0d);
		LCD_WriteByteSPI(0x0f);
		LCD_WriteByteSPI(0x11);
		LCD_WriteByteSPI(0x13);
		LCD_WriteByteSPI(0x15);
		LCD_WriteByteSPI(0x17);
		LCD_WriteByteSPI(0x19);
		LCD_WriteByteSPI(0x1B);
		LCD_WriteByteSPI(0x1D);
		LCD_WriteByteSPI(0x1F);
		LCD_WriteRegIndex(0x29);
		//read_data( );
		LCD_WriteRegIndex(0x2C);
}




/*************************************************
函数名：Lcd光标起点定位函数
功能：指定320240液晶上的一点作为写数据的起始点
入口参数：x 坐标 0~239
          y 坐标 0~319
返回值：无
*************************************************/
//void LCD_SetCursor(u16 x,u16 y)
//{ 
//	LCD_WriteRegIndex(0x20);
//	LCD_WriteData(x);//水平坐标
//	LCD_WriteRegIndex(0x21);
//	LCD_WriteData(y);//垂直坐标 
//}
void SPILCD_SetWindow(unsigned short xstat,unsigned short xend,unsigned short ystat,unsigned short yend)
{
	LCD_WriteRegIndex(0x2A);
	LCD_WriteByteSPI(xstat>>8&0xff);
	LCD_WriteByteSPI(xstat&0xff);
	LCD_WriteByteSPI(xend>>8&0xff);
	LCD_WriteByteSPI(xend & 0xff);
	
	LCD_WriteRegIndex(0x2B);
	LCD_WriteByteSPI(ystat>>8 & 0xff);
	LCD_WriteByteSPI(ystat&0xff);
	LCD_WriteByteSPI(yend>>8 & 0xff);
	LCD_WriteByteSPI(yend & 0xff);
	
	LCD_WriteRegIndex(0x2C);
}

void LCD_Clear(void)
{
	unsigned short i,j;
	SPILCD_SetWindow(0,SCREEN_X_MAX-1,0,SCREEN_Y_MAX-1);
	for (i = 0; i < SCREEN_Y_MAX; i++)
	{
		for(j = 0; j < SCREEN_X_MAX*2; j++)
		{
			LCD_WriteByteSPI(0xff);
		}
	}
	SPILCD_SetWindow(0,0,0,0);
}


/******************************************
函数名：Lcd图像填充128*160
功能：向Lcd指定位置填充图像,
入口参数：
******************************************/
void LCD_Fill_Pic(u16 x, u16 y, unsigned short pic_Width,unsigned short pic_Height,const unsigned char* pic)
{
	unsigned char i,j;
	unsigned short w,h,xmax,ymax;
	unsigned long area=0;
	
	w	=	pic_Width-1;
	h	=	pic_Height-1;
	xmax	=	SCREEN_X_MAX < pic_Width	?	SCREEN_X_MAX	:	pic_Width;
	ymax	=	SCREEN_Y_MAX < pic_Height	?	SCREEN_Y_MAX	:	pic_Height;
	
	SPILCD_SetWindow(x,w,y,h);
	for (i = y; i < ymax; i++)
	{
		for(j = x; j < xmax; j++)
		{
			LCD_WriteByteSPI(pic[area++]);
			LCD_WriteByteSPI(pic[area++]);
		}
	}
	SPILCD_SetWindow(0,0,0,0);
}

//=============== 在x，y 坐标上打一个颜色为Color的点 ===============
void DrawPixel(u16 x, u16 y, int Color)
{
	SPILCD_SetWindow(x,x,y,y); 
	LCD_WriteData(Color);
	LCD_WriteData(Color);
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//8*16字体 ASCII码 显示
//函数名：SPILCD_ShowChar
//参  数：
//(x,y): 
//num:要显示的字符:" "--->"~"
//flag:有背景色(1)无背景色(0)
void SPILCD_ShowChar(unsigned short x,unsigned short y,unsigned char num, unsigned int fColor, unsigned int bColor,unsigned char flag) 
{       
	unsigned char temp;
	unsigned int pos,i,j;  

	num=num-' ';//得到偏移后的值
	i=num*16;
	for(pos=0;pos<16;pos++)
		{
			temp=nAsciiDot[i+pos];	//调通调用ASCII字体
			for(j=0;j<8;j++)
		   {
		        if(temp&0x80)
							DrawPixel(x+j,y,fColor);
						else
							if(flag) DrawPixel(x+j,y,bColor); //如果背景色标志flag为1
							temp<<=1;
		    }
			 y++;
		}
}

//写一个16x16的汉字
//void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag)
//{
//	unsigned int i,j,k;
//	unsigned short m;
//	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
//		if ((nAsciiDot[k].Index[0]==c[0])&&(nAsciiDot[k].Index[1]==c[1]))
//		{ 
//			for(i=0;i<32;i++) 
//			{
//				m=nAsciiDot[k].Msk[i];
//				for(j=0;j<8;j++) 
//				{		
//					if((m&0x80)==0x80) {
//						DrawPixel(x+j,y,fColor);
//						}
//					else {
//						if(flag) DrawPixel(x+j,y,bColor);
//						}
//					m=m<<1;
//				} 
//				if(i%2){y++;x=x-8;}
//				else x=x+8;
//			}
//		} 
//	}
//}

//显示一串字
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
{
	unsigned char l=0;
	while(*s)
		{
			if( *s < 0x80) 
				{
					SPILCD_ShowChar(x+l*8,y,*s,fColor,bColor,flag);
					s++;l++;
				}
			else
				{
//					PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
					s+=2;l+=2;
				}
		}
}
