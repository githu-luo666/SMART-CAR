/*******************************************************************************
LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技CYT4BB7核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V0.1版权所有，单位使用请先联系授权
【最后更新】2025年2月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】IAR Systems 9.40.1版本
【Target 】 CYT4BB7(Dual Core M7, cFlash:4MB wFlash:256KB RAM:758KB  144Pins)
【Crystal】 16.000Mhz
【SYS PLL】 100MHz + 250MHz + 250MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
*******************************************************************************/

#include "LQ_IPSLCD.h"
#include "LQ_TFTPicFont.h"
#include "include.h"

/*
USE_HORIZONTAL 0:  竖屏，排针在屏幕的左侧
USE_HORIZONTAL 1:  竖屏，排针在屏幕的右侧
USE_HORIZONTAL 2:  横屏，排针在屏幕的下侧
USE_HORIZONTAL 3:  横屏，排针在屏幕的上侧
*/
#ifdef IPS114
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 135
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 135
#endif
#endif

#ifdef IPS130
#define LCD_W 240
#define LCD_H 240
#endif

#ifdef IPS154
#define LCD_W 240
#define LCD_H 240
#endif

#ifdef IPS200
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320
#else
#define LCD_W 320
#define LCD_H 240
#endif
#endif

/*************************************************************************
*  函数名称：void LCD_Init(void)
*  功能说明：LCD屏幕初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：在IPSLCD.h中选择屏幕
*************************************************************************/
void LCD_Init(void)
{
  LQ_SPI_InitConfig(SCB5_CLK_P07_2, SCB5_MISO_P07_0, SCB5_MOSI_P07_1, SCB5_SEL0_P07_3, 62500000);
  LQ_GPIO_Init_Config(IPS_DC,  GPIO_OUT_PP_UP);
  LQ_GPIO_Init_Config(IPS_RST, GPIO_OUT_PP_UP);
  delay_ms(1);
  IPS_RST_H;
  IPS_DC_H;
  delay_ms(1);
  /*复位*/
  IPS_RST_L;
  delay_ms(200);
  IPS_RST_H;
  delay_ms(200);
  LCD_Write_Cmd(0x11); 
  delay_ms(200);
  LCD_Write_Cmd(0x36); 
  if(USE_HORIZONTAL==0)LCD_Write_Byte(0x00);
  else if(USE_HORIZONTAL==1)LCD_Write_Byte(0xC0);
  else if(USE_HORIZONTAL==2)LCD_Write_Byte(0x70);
  else LCD_Write_Byte(0xA0);
  
  LCD_Write_Cmd(0x3A);
  LCD_Write_Byte(0x05);
  LCD_Write_Cmd(0xB2);
  LCD_Write_Byte(0x0C);
  LCD_Write_Byte(0x0C);
  LCD_Write_Byte(0x00);
  LCD_Write_Byte(0x33);
  LCD_Write_Byte(0x33); 
  LCD_Write_Cmd(0xB7); 
  LCD_Write_Byte(0x35);  
  LCD_Write_Cmd(0xBB);
  LCD_Write_Byte(0x19);
  LCD_Write_Cmd(0xC0);
  LCD_Write_Byte(0x2C);
  LCD_Write_Cmd(0xC2);
  LCD_Write_Byte(0x01);
  LCD_Write_Cmd(0xC3);
  LCD_Write_Byte(0x12);   
  LCD_Write_Cmd(0xC4);
  LCD_Write_Byte(0x20);  
  LCD_Write_Cmd(0xC6); 
  LCD_Write_Byte(0x0F);    
  LCD_Write_Cmd(0xD0); 
  LCD_Write_Byte(0xA4);
  LCD_Write_Byte(0xA1);
  LCD_Write_Cmd(0xE0);
  LCD_Write_Byte(0xD0);
  LCD_Write_Byte(0x04);
  LCD_Write_Byte(0x0D);
  LCD_Write_Byte(0x11);
  LCD_Write_Byte(0x13);
  LCD_Write_Byte(0x2B);
  LCD_Write_Byte(0x3F);
  LCD_Write_Byte(0x54);
  LCD_Write_Byte(0x4C);
  LCD_Write_Byte(0x18);
  LCD_Write_Byte(0x0D);
  LCD_Write_Byte(0x0B);
  LCD_Write_Byte(0x1F);
  LCD_Write_Byte(0x23);
  LCD_Write_Cmd(0xE1);
  LCD_Write_Byte(0xD0);
  LCD_Write_Byte(0x04);
  LCD_Write_Byte(0x0C);
  LCD_Write_Byte(0x11);
  LCD_Write_Byte(0x13);
  LCD_Write_Byte(0x2C);
  LCD_Write_Byte(0x3F);
  LCD_Write_Byte(0x44);
  LCD_Write_Byte(0x51);
  LCD_Write_Byte(0x2F);
  LCD_Write_Byte(0x1F);
  LCD_Write_Byte(0x1F);
  LCD_Write_Byte(0x20);
  LCD_Write_Byte(0x23);
  LCD_Write_Cmd(0x21); 
  LCD_Write_Cmd(0x29);
}

/*************************************************************************
*  函数名称：void IPSdelay_1us(unsigned int Del)	
*  功能说明：屏幕延时
*  参数说明：Del 延时时间 单位us
*  函数返回：无
*  调用方法：IPSdelay_1us(1); 延时1us
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void IPSdelay_1us(unsigned int Del)		//
{
  while(Del--)
  {
    __asm("NOP");
  }
}

/*************************************************************************
*  函数名称：void LCD_Write_Cmd(unsigned char cmd)	
*  功能说明：向屏幕写入命令
*  参数说明：cmd 要写入的命令，通过该参数传递具体的命令
*  函数返回：无
*  调用方法：LCD_Write_Cmd(0xb7); 向LCD屏幕写入命令0xb7
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Write_Cmd(unsigned char cmd)
{
  IPS_DC_L;
  uint8_t buff[1];
  buff[0] = cmd;
  LQ_SPI_WriteArray(SCB5, buff, 1);
  Cy_SysTick_DelayInUs(3);
  //  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
}

/*************************************************************************
*  函数名称：void LCD_Write_Byte(unsigned char dat)	
*  功能说明：向屏幕写入一个字节的数据
*  参数说明：dat 要写入的数据字节，通过该参数传递具体要写入的数值
*  函数返回：无
*  调用方法：LCD_Write_Byte(0x00); 向LCD屏幕写入数据字节0x00
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Write_Byte(unsigned char dat)
{
  IPS_DC_H;
  uint8_t buff[1];
  buff[0] = dat;
  LQ_SPI_WriteArray(SCB5, buff, 1);
  Cy_SysTick_DelayInUs(3);
  //  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
  return;
}
/*************************************************************************
*  函数名称：void LCD_Write_Word(unsigned int dat)	
*  功能说明：向屏幕写入一个半字(通常为16位)的数据
*  参数说明：dat 要写入的半字数据，通过该参数传入需要写入设备的16位数据值
*  函数返回：无
*  调用方法：LCD_Write_Word(0xFFFF); 向LCD屏幕写入半字数据0xFFFF
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Write_Word(unsigned int dat)
{
  IPS_DC_H;
  uint8_t buff[2];
  buff[0] = dat >> 8;
  buff[1] = (uint8_t)dat;
  LQ_SPI_WriteArray(SCB5, buff, 2);
  Cy_SysTick_DelayInUs(6);
  //  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
}

/*************************************************************************
*  函数名称：void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
*  功能说明：设置屏幕显示的行和列的起始与结束地址，以便后续对特定区域操作。
*  参数说明：x1, x2：设置列的起始和结束地址。x1 为列的起始地址，x2 为列的结束地址，通过这两个参数可以确定操作的列范围。
*            y1, y2：设置行的起始和结束地址。y1 为行的起始地址，y2 为行的结束地址，通过这两个参数可以确定操作的行范围。
*  函数返回：无
*  调用方法：LCD_Address_Set(10,10,20,20);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  int W_offset = 0, H_offset = 0;
  if(USE_HORIZONTAL==0)
  {
#ifdef IPS114
    W_offset = 52; H_offset = 40;
#endif
#ifdef IPS130
    W_offset = 0; H_offset = 0;
#endif
#ifdef IPS154
    W_offset = 0; H_offset = 0;
#endif
#ifdef IPS200
    W_offset = 0; H_offset = 0;
#endif
  }
  else if(USE_HORIZONTAL==1)
  {
#ifdef IPS114
    W_offset = 53; H_offset = 40;
#endif
#ifdef IPS130
    W_offset = 0; H_offset = 80;
#endif
#ifdef IPS154
    W_offset = 0; H_offset = 80;
#endif
#ifdef IPS200
    W_offset = 0; H_offset = 0;
#endif
  }
  else if(USE_HORIZONTAL==2)
  {
#ifdef IPS114
    W_offset = 40; H_offset = 53;
#endif
#ifdef IPS130
    W_offset = 0; H_offset = 0;
#endif
#ifdef IPS154
    W_offset = 0; H_offset = 0;
#endif
#ifdef IPS200
    W_offset = 0; H_offset = 0;
#endif
  }
  else
  {
#ifdef IPS114
    W_offset = 40; H_offset = 52;
#endif
#ifdef IPS130
    W_offset = 80; H_offset = 0;
#endif
#ifdef IPS154
    W_offset = 80; H_offset = 0;
#endif
#ifdef IPS200
    W_offset = 0; H_offset = 0;
#endif
  }
  LCD_Write_Cmd(0x2a);//列地址设置
  LCD_Write_Word(x1+W_offset);
  LCD_Write_Word(x2+W_offset);
  LCD_Write_Cmd(0x2b);//行地址设置
  LCD_Write_Word(y1+H_offset);
  LCD_Write_Word(y2+H_offset);
  LCD_Write_Cmd(0x2c);//储存器写
}
/*************************************************************************
*  函数名称：void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
*  功能说明：在屏幕指定位置画点
*  参数说明：x,y：画点的坐标；color：点的颜色
*  函数返回：无
*  调用方法：LCD_DrawPoint(10,10,BLUE); 在10,10画蓝色点
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
{
  LCD_Address_Set(x,y,x,y);//设置光标位置 
  LCD_Write_Word(color);
} 
/*************************************************************************
*  函数名称：void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
*  功能说明：在屏幕指定位置显示指定颜色单个字符
*  参数说明：  x, y：显示坐标。x 表示字符在水平方向上的起始位置，y 表示字符在垂直方向上的起始位置，通过这两个参数可以确定字符在显示设备上的具体显示位置。
*              num：要显示的字符。可以是 ASCII 码对应的字符，函数会根据该字符的值来获取相应的字模进行显示。
*              fc：字的颜色。用于指定字符本身的颜色，不同的设备可能使用不同的颜色表示方式，例如 RGB 值等。
*              bc：字的背景颜色。用于指定字符所在区域的背景颜色，同样可以使用不同的颜色表示方式。
*              sizey：字号。可选择的值为 12、16、24、32，分别代表不同大小的字体。通过选择合适的字号，可以调整字符显示的大小。
*              mode：显示模式。0 表示非叠加模式，即字符会覆盖背景；1 表示叠加模式，字符会与背景进行叠加显示。
*  函数返回：无
*  调用方法：LCD_ShowChar(10,10,'A',BLUE,12,1)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
  unsigned char temp,sizex,t,m=0;
  unsigned int i,TypefaceNum;//一个字符所占字节大小
  unsigned int x0=x;
  sizex=sizey/2;
  TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
  num=num-' ';    //得到偏移后的值
  LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
  for(i=0;i<TypefaceNum;i++)
  { 
    if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
    else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
    else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
    else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
    else return;
    for(t=0;t<8;t++)
    {
      if(!mode)//非叠加模式
      {
        if(temp&(0x01<<t))LCD_Write_Word(fc);
        else LCD_Write_Word(bc);
        m++;
        if(m%sizex==0)
        {
          m=0;
          break;
        }
      }
      else//叠加模式
      {
        if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
        x++;
        if((x-x0)==sizex)
        {
          x=x0;
          y++;
          break;
        }
      }
    }
  }   	 	  
}
/*************************************************************************
*  函数名称：void LCD_ShowString(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
*  功能说明：在屏幕指定位置显示指定颜色字符串
*  参数说明：  x, y：显示坐标。x 表示字符在水平方向上的起始位置，y 表示字符在垂直方向上的起始位置，通过这两个参数可以确定字符在显示设备上的具体显示位置。
*              num：要显示的字符。可以是 ASCII 码对应的字符，函数会根据该字符的值来获取相应的字模进行显示。
*              fc：字的颜色。用于指定字符本身的颜色，不同的设备可能使用不同的颜色表示方式，例如 RGB 值等。
*              bc：字的背景颜色。用于指定字符所在区域的背景颜色，同样可以使用不同的颜色表示方式。
*              sizey：字号。可选择的值为 12、16、24、32，分别代表不同大小的字体。通过选择合适的字号，可以调整字符显示的大小。
*              mode：显示模式。0 表示非叠加模式，即字符会覆盖背景；1 表示叠加模式，字符会与背景进行叠加显示。
*  函数返回：无
*  调用方法：LCD_ShowChar(10,10,"ABCD",BLUE,12,1)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowString(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{         
  while(*p!='\0')
  {       
    LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
    x+=sizey/2;
    p++;
  }  
}
/*************************************************************************
*  函数名称：void LCD_ShowStr_12(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
*  功能说明：在屏幕上显示12*6大小的字符串
*  参数说明： x: 不同型号 IPS 屏幕下的横坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，横坐标范围是 0 - 39；竖屏时，横坐标范围是 0 - 21。
*                IPS130: 横屏时，横坐标范围是 0 - 39；竖屏时，横坐标范围是 0 - 39。
*                IPS154: 横屏时，横坐标范围是 0 - 39；竖屏时，横坐标范围是 0 - 39。
*                IPS200: 横屏时，横坐标范围是 0 - 52；竖屏时，横坐标范围是 0 - 21。
*             y: 不同型号 IPS 屏幕下的纵坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，纵坐标范围是 0 - 10；竖屏时，纵坐标范围是 0 - 19。
*                IPS130: 横屏时，纵坐标范围是 0 - 19；竖屏时，纵坐标范围是 0 - 19。
*                IPS154: 横屏时，纵坐标范围是 0 - 19；竖屏时，纵坐标范围是 0 - 19。
*                IPS200: 横屏时，纵坐标范围是 0 - 19；竖屏时，纵坐标范围是 0 - 21。
*                *p: 指向要显示的字符串的指针，该字符串将被显示在屏幕指定位置。
*                fc: 字的颜色，用于指定字符串中字符的显示颜色。
*                bc: 字的背景颜色，用于指定字符串所在区域的背景颜色。
*  函数返回：无
*  调用方法：LCD_ShowStr_12(10,10,"ABCD",BLUE,GREEN)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowStr_12(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
{         
  while(*p!='\0')
  {       
    LCD_ShowChar(x*6,y*12,*p,fc,bc,12,0);
    x++;
    p++;
  }  
}
/*************************************************************************
*  函数名称：void LCD_ShowStr_16(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
*  功能说明：在屏幕上显示8*16大小的字符串
*  参数说明： x: 不同型号 IPS 屏幕下的横坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，横坐标范围是 0 - 29；竖屏时，横坐标范围是 0 - 15。
*                IPS130: 横屏时，横坐标范围是 0 - 29；竖屏时，横坐标范围是 0 - 29。
*                IPS154: 横屏时，横坐标范围是 0 - 29；竖屏时，横坐标范围是 0 - 29。
*                IPS200: 横屏时，横坐标范围是 0 - 29；竖屏时，横坐标范围是 0 - 21。
*             y: 不同型号 IPS 屏幕下的纵坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，纵坐标范围是 0 - 7； 竖屏时，纵坐标范围是 0 - 14。
*                IPS130: 横屏时，纵坐标范围是 0 - 14；竖屏时，纵坐标范围是 0 - 14。
*                IPS154: 横屏时，纵坐标范围是 0 - 14；竖屏时，纵坐标范围是 0 - 14。
*                IPS200: 横屏时，纵坐标范围是 0 - 14；竖屏时，纵坐标范围是 0 - 21。
*                *p: 指向要显示的字符串的指针，该字符串将被显示在屏幕指定位置。
*                fc: 字的颜色，用于指定字符串中字符的显示颜色。
*                bc: 字的背景颜色，用于指定字符串所在区域的背景颜色。
*  函数返回：无
*  调用方法：LCD_ShowStr_16(10,10,"ABCD",BLUE,GREEN)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowStr_16(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
{         
  while(*p!='\0')
  {       
    LCD_ShowChar(x*8,y*16,*p,fc,bc,16,0);
    x++;
    p++;
  }  
}
/*************************************************************************
*  函数名称：void LCD_ShowStr_24(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
*  功能说明：在屏幕上显示12*24大小的字符串
*  参数说明： x: 不同型号 IPS 屏幕下的横坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，横坐标范围是 0 - 19；竖屏时，横坐标范围是 0 - 10。
*                IPS130: 横屏时，横坐标范围是 0 - 19；竖屏时，横坐标范围是 0 - 21。
*                IPS154: 横屏时，横坐标范围是 0 - 19；竖屏时，横坐标范围是 0 - 21。
*                IPS200: 横屏时，横坐标范围是 0 - 25；竖屏时，横坐标范围是 0 - 21。
*             y: 不同型号 IPS 屏幕下的纵坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，纵坐标范围是 0 - 4； 竖屏时，纵坐标范围是 0 - 9。
*                IPS130: 横屏时，纵坐标范围是 0 - 9； 竖屏时，纵坐标范围是 0 - 21。
*                IPS154: 横屏时，纵坐标范围是 0 - 19；竖屏时，纵坐标范围是 0 - 21。
*                IPS200: 横屏时，纵坐标范围是 0 - 19；竖屏时，纵坐标范围是 0 - 21。
*                *p: 指向要显示的字符串的指针，该字符串将被显示在屏幕指定位置。
*                fc: 字的颜色，用于指定字符串中字符的显示颜色。
*                bc: 字的背景颜色，用于指定字符串所在区域的背景颜色。
*  函数返回：无
*  调用方法：LCD_ShowStr_24(5,5,"ABCD",BLUE,GREEN)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowStr_24(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
{         
  while(*p!='\0')
  {       
    LCD_ShowChar(x*12,y*24,*p,fc,bc,24,0);
    x++;
    p++;
  }  
}
/*************************************************************************
*  函数名称：void LCD_ShowStr_24(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
*  功能说明：在屏幕上显示12*24大小的字符串
*  参数说明： x: 不同型号 IPS 屏幕下的横坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，横坐标范围是 0 - 14；竖屏时，横坐标范围是 0 - 7。
*                IPS130: 横屏时，横坐标范围是 0 - 14；竖屏时，横坐标范围是 0 - 21。
*                IPS154: 横屏时，横坐标范围是 0 - 14；竖屏时，横坐标范围是 0 - 21。
*                IPS200: 横屏时，横坐标范围是 0 - 19；竖屏时，横坐标范围是 0 - 21。
*             y: 不同型号 IPS 屏幕下的纵坐标，具体坐标范围根据屏幕型号和屏幕方向而定：
*                IPS114: 横屏时，纵坐标范围是 0 - 3； 竖屏时，纵坐标范围是 0 - 6。
*                IPS130: 横屏时，纵坐标范围是 0 - 6； 竖屏时，纵坐标范围是 0 - 21。
*                IPS154: 横屏时，纵坐标范围是 0 - 6； 竖屏时，纵坐标范围是 0 - 21。
*                IPS200: 横屏时，纵坐标范围是 0 - 6； 竖屏时，纵坐标范围是 0 - 21。
*                *p: 指向要显示的字符串的指针，该字符串将被显示在屏幕指定位置。
*                fc: 字的颜色，用于指定字符串中字符的显示颜色。
*                bc: 字的背景颜色，用于指定字符串所在区域的背景颜色。
*  函数返回：无
*  调用方法：LCD_ShowStr_32(5,5,"ABCD",BLUE,GREEN)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowStr_32(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc)
{         
  while(*p!='\0')
  {       
    LCD_ShowChar(x*16,y*32,*p,fc,bc,32,0);
    x++;
    p++;
  }  
}
/*************************************************************************
*  函数名称：void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
*  功能说明：在屏幕指定区域填充指定颜色
*  参数说明： xsta,ysta: x,y点的起始坐标
*             xend,yend: x,y点的终止坐标
*             color:填充的颜色
*  函数返回：无
*  调用方法：LCD_Fill(0,0,10,10,GREEN)
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{          
  unsigned int i,j; 
  LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
  for(i=ysta;i<yend;i++)
  {													   	 	
    for(j=xsta;j<xend;j++)
    {
      LCD_Write_Word(color);
    }
  } 					  	    
}
/*************************************************************************
*  函数名称：void LCD_CLS(unsigned int color)
*  功能说明：清屏成指定颜色
*  参数说明：color:填充的颜色           
*  函数返回：无
*  调用方法：LCD_CLS(GREEN)；
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_CLS(unsigned int color)
{
  LCD_Fill(0,0,LCD_W,LCD_H,color);  	    
}
/*************************************************************************
*  函数名称：void LCD_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
*  功能说明：画线
*  参数说明：xs ：起始x 
             ys ：起始y
             xe ：结束x IPS114  (横屏x:0-240, 竖屏x:0-135)
             xe ：结束x IPS130  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS154  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS200  (横屏x:0-320, 竖屏x:0-240)
             ys ：结束y IPS114  (横屏y:0-135, 竖屏x:0-240)
             ys ：结束y IPS130  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS154  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS200  (横屏y:0-240, 竖屏x:0-320) 
             color_dat:GREEN
*  函数返回：无
*  调用方法：LCD_Draw_Line(0,0,10,10,GREEN)；
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
{
  int i,ds;
  int dx,dy,inc_x, inc_y;
  int xerr = 0, yerr = 0;				        //初始化变量
  
  if(xs==xe)	  						//如果是垂直线则只需对竖直坐标计数
  {
    LCD_Address_Set(xs,ys,xe,ye);
    for(i=0;i<(ye-ys+1);i++)
    {
      LCD_Write_Word(color_dat);
    }
  }
  else if(ys==ye)						//如果是水平线则只需对水平坐标计数
  {
    LCD_Address_Set(xs,ys,xe,ye);
    for(i=0;i<(xe-xs+1);i++)
    {
      LCD_Write_Word(color_dat);
    }
  } 
  else								//如果是斜线，则重新计算，使用画点函数画出直线
  {
    dx = xe - xs;						//计算坐标增量
    dy = ye - ys;
    
    if(dx > 0) inc_x = 1;					//设置单步方向
    else 
    {
      inc_x = -1; dx = -dx;
    }
    if(dy > 0) inc_y = 1;					//设置单步方向
    else
    {
      inc_y = -1; dy = -dy;
    }
    
    if(dx > dy) {ds = dx;}					//选取基本增量坐标点
    else		{ds = dy;}
    
    for(i = 0; i <= ds+1; i++)					//画线输出
    {
      LCD_DrawPoint(xs, ys,color_dat);	//画点
      xerr += dx;
      yerr += dy;
      if(xerr > ds)
      {
        xerr -= ds;
        xs   += inc_x;
      }
      if(yerr > ds)
      {
        yerr -= ds;
        ys   += inc_y;
      }
    }
  }  
}
/*************************************************************************
*  函数名称：void LCD_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
*  功能说明：画矩形
*  参数说明：xs ：起始x 
             ys ：起始y
             xe ：结束x IPS114  (横屏x:0-240, 竖屏x:0-135)
             xe ：结束x IPS130  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS154  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS200  (横屏x:0-320, 竖屏x:0-240)
             ys ：结束y IPS114  (横屏y:0-135, 竖屏x:0-240)
             ys ：结束y IPS130  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS154  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS200  (横屏y:0-240, 竖屏x:0-320) 
             color_dat:GREEN
*  函数返回：无
*  调用方法：LCD_Draw_Line(0,0,10,10,GREEN)；
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
{
  LCD_Draw_Line(xs,ys,xs,ye,color_dat);	  //画矩形左边
  LCD_Draw_Line(xe,ys,xe,ye,color_dat);	  //画矩形右边
  LCD_Draw_Line(xs,ys,xe,ys,color_dat);	  //画矩形上边
  LCD_Draw_Line(xs,ye,xe,ye,color_dat);	  //画矩形下边
}
/*************************************************************************
*  函数名称：void LCD_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
*  功能说明：画圆
*  参数说明：xs ：起始x 
             ys ：起始y
             xe ：结束x IPS114  (横屏x:0-240, 竖屏x:0-135)
             xe ：结束x IPS130  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS154  (横屏x:0-240, 竖屏x:0-240)
             xe ：结束x IPS200  (横屏x:0-320, 竖屏x:0-240)
             ys ：结束y IPS114  (横屏y:0-135, 竖屏x:0-240)
             ys ：结束y IPS130  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS154  (横屏y:0-240, 竖屏x:0-240)
             ys ：结束y IPS200  (横屏y:0-240, 竖屏x:0-320) 
             r  : 半径
             color_dat:GREEN
*  函数返回：无
*  调用方法：LCD_Draw_Line(0,0,10,10,5,GREEN)；
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat)
{
  unsigned char dx, dy = r; 
  
  if((x>=r) &&((LCD_W-x)>=r) && (y>=r) && ((LCD_H-y)>=r))		//确定所画圆在屏幕范围内，没有超出最外边界(暂不支持与屏幕边相交)
  {
    for(dx = 0; dx <= r; dx++)
    {
      while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
      LCD_DrawPoint(x + dx, y - dy, color_dat);
      LCD_DrawPoint(x - dx, y - dy, color_dat);
      LCD_DrawPoint(x - dx, y + dy, color_dat);
      LCD_DrawPoint(x + dx, y + dy, color_dat);
      
      LCD_DrawPoint(x + dy, y - dx, color_dat);
      LCD_DrawPoint(x - dy, y - dx, color_dat);
      LCD_DrawPoint(x - dy, y + dx, color_dat);
      LCD_DrawPoint(x + dy, y + dx, color_dat);
    }
  }
}

/*************************************************************************
*  函数名称：void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width,const unsigned char pic[])
*  功能说明：显示图片
*  参数说明：x,y起点坐标,length 图片长度,width  图片宽度,pic[]  图片数组    
*  函数返回：无
*  调用方法：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width,const unsigned char pic[])
{
  unsigned int i,j;
  unsigned long k=0;
  LCD_Address_Set(x,y,x+length-1,y+width-1);
  for(i=0;i<length;i++)
  {
    for(j=0;j<width;j++)
    {
      LCD_Write_Byte(pic[k*2]);
      LCD_Write_Byte(pic[k*2+1]);
      k++;
    }
  }			
}
/*************************************************************************
*  函数名称：void LCD_ShowChinese12x12(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
*  功能说明：显示12x12汉字
*  参数说明：x,y显示的行数和列数，*s_dat 要显示的汉字，word_color 字的颜色，back_color 字的背景颜色
*  函数返回：无
*  调用方法：LCD_ShowChinese12x12(0,0,'北',BLUE,GREEN);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowChinese12x12(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
{
  unsigned char wm=0,ii = 0,i,j;
  int adder=1;
  while(s_dat[ii] != '\0')
  {
    wm = 0;
    adder = 1;
    while(tfont_Idx[wm] > 127)
    {
      if(tfont_Idx[wm] == (uint8_t)s_dat[ii])
      {
        if(tfont_Idx[wm + 1] == (uint8_t)s_dat[ii + 1])
        {
          adder = wm * 12;
          break;
        }
      }
      wm += 2;
    }
    
    if(adder != 1)// 显示汉字
    {
      LCD_Address_Set(x*12,y*12,(x+1)*12-1,(y+1)*12-1);         //定位字符显示区域
      for(j=0; j <24; j++)
      {
        if((j%2) == 0){
          for(i=0; i<8; i++)
          {
            if((tfont_12x12[adder]) &  (0x80>>i))
              LCD_Write_Word(word_color);
            else
              LCD_Write_Word(back_color);
          }
        }
        else{
          for(i=0; i<4; i++)
          {
            if((tfont_12x12[adder]) &  (0x80>>i))
              LCD_Write_Word(word_color);
            else
              LCD_Write_Word(back_color);
          }
        }
        adder += 1;
      }
    }
    else              //显示空白字符
    {}
    //y+=1;//左右方向
    x+=1;//上下方向
    ii += 2;
  }
}

/*************************************************************************
*  函数名称：void LCD_ShowChinese16x16(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
*  功能说明：显示16x16汉字
*  参数说明：x,y显示的行数和列数，*s_dat 要显示的汉字，word_color 字的颜色，back_color 字的背景颜色
*  函数返回：无
*  调用方法：LCD_ShowChinese16x16(0,0,'北',BLUE,GREEN);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowChinese16x16(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
{
  unsigned char wm=0,ii = 0,i,j;
  int adder=1;
  while(s_dat[ii] != '\0')
  {
    wm = 0;
    adder = 1;
    while(tfont_Idx[wm] > 127)
    {
      if(tfont_Idx[wm] == (uint8_t)s_dat[ii])
      {
        if(tfont_Idx[wm + 1] == (uint8_t)s_dat[ii + 1])
        {
          adder = wm * 16;
          break;
        }
      }
      wm += 2;
    }
    
    if(adder != 1)// 显示汉字
    {
      LCD_Address_Set(x*16,y*16,(x+1)*16-1,(y+1)*16-1);         //定位字符显示区域
      for(j=0; j <32; j++)
      {
        for(i=0; i<8; i++)
        {
          if((tfont_16x16[adder]) &  (0x80>>i))
            LCD_Write_Word(word_color);
          else
            LCD_Write_Word(back_color);
        }
        adder += 1;
      }
    }
    else              //显示空白字符
    {}
    //y+=1;//左右方向
    x+=1;//上下方向
    ii += 2;
  }
}

/*************************************************************************
*  函数名称：void LCD_ShowChinese24x24(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
*  功能说明：显示24x24汉字
*  参数说明：x,y显示的行数和列数，*s_dat 要显示的汉字，word_color 字的颜色，back_color 字的背景颜色
*  函数返回：无
*  调用方法：LCD_ShowChinese24x24(0,0,'北',BLUE,GREEN);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowChinese24x24(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
{
  unsigned char wm=0,ii = 0,i,j;
  int adder=1;
  while(s_dat[ii] != '\0')
  {
    wm = 0;
    adder = 1;
    while(tfont_Idx[wm] > 127)
    {
      if(tfont_Idx[wm] == (uint8_t)s_dat[ii])
      {
        if(tfont_Idx[wm + 1] == (uint8_t)s_dat[ii + 1])
        {
          adder = wm * 36;
          break;
        }
      }
      wm += 2;
    }
    
    if(adder != 1)// 显示汉字
    {
      LCD_Address_Set(x*24,y*24,(x+1)*24-1,(y+1)*24-1);         //定位字符显示区域
      for(j=0; j <72; j++)
      {
        for(i=0; i<8; i++)
        {
          if((tfont_24x24[adder]) &  (0x80>>i))
            LCD_Write_Word(word_color);
          else
            LCD_Write_Word(back_color);
        }
        adder += 1;
      }
    }
    else              //显示空白字符
    {}
    //y+=1;//左右方向
    x+=1;//上下方向
    ii += 2;
  }
}

/*************************************************************************
*  函数名称：void LCD_ShowChinese32x32(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
*  功能说明：显示32x32汉字
*  参数说明：x,y显示的行数和列数，*s_dat 要显示的汉字，word_color 字的颜色，back_color 字的背景颜色
*  函数返回：无
*  调用方法：LCD_ShowChinese32x32(0,0,'北',BLUE,GREEN);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_ShowChinese32x32(unsigned int x, unsigned int y,  char *s_dat,unsigned int word_color,unsigned int back_color)
{
  unsigned char wm=0,ii = 0,i,j;
  int adder=1;
  while(s_dat[ii] != '\0')
  {
    wm = 0;
    adder = 1;
    while(tfont_Idx[wm] > 127)
    {
      if(tfont_Idx[wm] == (uint8_t)s_dat[ii])
      {
        if(tfont_Idx[wm + 1] == (uint8_t)s_dat[ii + 1])
        {
          adder = wm * 64;
          break;
        }
      }
      wm += 2;
    }
    
    if(adder != 1)// 显示汉字
    {
      LCD_Address_Set(x*32,y*32,(x+1)*32-1,(y+1)*32-1);         //定位字符显示区域
      for(j=0; j <128; j++)
      {
        for(i=0; i<8; i++)
        {
          if((tfont_32x32[adder]) &  (0x80>>i))
            LCD_Write_Word(word_color);
          else
            LCD_Write_Word(back_color);
        }
        adder += 1;
      }
    }
    else              //显示空白字符
    {}
    x+=1;//上下方向
    ii += 2;
  }
}

/*************************************************************************
*  函数名称：void LCD_ShowChinese(unsigned int x,unsigned int y,char *s,unsigned int fc,unsigned int bc,unsigned char sizey)
*  功能说明：显示汉字串
*  参数说明：x,y显示的行数和列数，*s 要显示的汉字串，fc 字的颜色，bc 字的背景颜色 ,sizey 字号 可选12，16，24，32
*  函数返回：无
*  调用方法：LCD_ShowChinese(0,0,"北京龙邱",BLUE,GREEN,12);
*  修改时间：2024年11月22日
*  备    注：字库在font.h中，取模选项：宋体，阴码，逐行式，点阵16*16，顺向，十六进制，C51格式
*************************************************************************/
void LCD_ShowChinese(unsigned int x,unsigned int y,char *s,unsigned int fc,unsigned int bc,unsigned char sizey)
{
  while(*s!=0)
  {
    if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc);
    else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc);
    else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc);
    else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc);
    else return;
    s+=2;
    x++;
  }
}

/*************************************************************************
*  函数名称：void LCD_Show_Logo(unsigned char xs,unsigned char ys)
*  功能说明：显示龙邱LOGO
*  参数说明：x,y显示的行数和列数
*  函数返回：无
*  调用方法：LCD_Show_Logo(0,0);
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void LCD_Show_Logo(unsigned char xs,unsigned char ys)
{
  LCD_ShowPicture(xs,ys,160,54,gImage_lo);       //80,gImage_ftflogo);     //
}

/*************************************************************************
*  函数名称：void LCDSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
*  功能说明：显示灰度图像
*  参数说明：high_start  显示图像起始位置的纵坐标
             wide_start  显示图像起始位置的横坐标
             high        显示图像的高度
             wide        显示图像的宽度
             Pixle       显示图像数据的地址
*  函数返回：无
*  调用方法：LCDSPI_Road(0, 0, 60, 94, (unsigned char *)&img[0][0]);  // 图像显示
*  修改时间：2024年11月22日
*  备    注：屏幕左上角为(0,0)
*************************************************************************/
void LCDSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
  unsigned long i;
  /* 设置显示范围 */
  LCD_Address_Set(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);
  unsigned short color;
  unsigned long temp = high * wide;
  /* 显示图像 */
  for(i = 0; i < temp; i++)
  {
    /* 将灰度转化为 RGB565 */
    color = (Pixle[i] >> 3) << 11;
    color |= (Pixle[i] >> 2) << 5;
    color |= Pixle[i] >> 3;
    /* 显示 */
    LCD_Write_Word (color);
  }
  
}

/*************************************************************************
*  函数名称：void LCDSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
*  功能说明：显示二值化图像
*  参数说明：high_start  显示图像起始位置的纵坐标
             wide_start  显示图像起始位置的横坐标
             high        显示图像的高度
             wide        显示图像的宽度
             Pixle       显示图像数据的地址
*  函数返回：无
*  调用方法：无
*  修改时间：2024年11月22日
*  备    注：屏幕左上角为(0,0)
*************************************************************************/
void LCDSPI_BinRoad(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
  unsigned long i;
  /* 设置显示范围 */
  LCD_Address_Set(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);
  
  unsigned long temp = high * wide;
  /* 显示图像 */
  for(i = 0; i < temp; i++)
  {
    if(Pixle[i])
    {
      /* 显示 */
      LCD_Write_Word (0xffff);
    }
    else
    {
      /* 显示 */
      LCD_Write_Word (0);
    }
  }
}

/*************************************************************************
*  函数名称：void Test_IPSLCD(void)
*  功能说明：屏幕测试
*  参数说明：无
*  函数返回：无
*  调用方法：Test_IPSLCD();
*  修改时间：2024年11月22日
*  备    注：在IPSLCD.h中选择屏幕大小，IPS1.14、IPS1.30、IPS1.54、IPS2.0
*************************************************************************/
void Test_IPSLCD(void)
{ 
  char txt[32];
  unsigned short count = 1;
  GPIO_LED_Init();
  LCD_Init();                                            //LCD初始化  0:横屏  1：竖屏
  LCD_CLS(BLACK);
  LCD_Draw_Line(50,50,200,130,GREEN);										//画线
  LCD_Draw_Rectangle(50,50,200,130,GREEN);							//画矩形框
  LCD_Draw_Circle(50, 50, 50, GREEN);										//画圆
  delay_ms(1000);                                        //延时等待
  while(1)
  {
    sprintf(txt, "Val:%05d", count++);                    //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
    LCD_ShowStr_24(0,4,txt,RED_IPS,LIGHTBLUE);            //显示24*12字符串
    LCD_ShowChinese(0,2,"龙邱科技",RED_IPS,WHITE,32);     //显示汉字
    LCD_ShowPicture(200,95,40,40,gImage_1);               //显示图片
    LCD_Show_Logo(0, 0);                                  //显示LOGO
    LCD_Write_Byte(0xFF);
    LED_Ctrl(LED0,RVS);
  }
}
