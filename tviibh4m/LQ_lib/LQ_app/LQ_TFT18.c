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
#include "include.h"
#include "LQ_TFTPicFont.h"

#define USE_QSPI 0      //使用硬件SPI则将其置1  软件SPI置0

//龙邱TFT SPI口模块管脚顺序
#define TFTSPI_CS     P07_2     // CS管脚 默认拉低，可以不用
#define TFTSPI_SCK    P07_1     // SPI SCK管脚
#define TFTSPI_SDI    P07_5     // SPI MOSI管脚
#define TFTSPI_DC     P07_7     // D/C管脚
#define TFTSPI_RST    P07_3     // RESET管脚

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void lq_tft_delayms(unsigned short ms)
@功能说明：简单延时函数
@参数说明：Del_1ms ：延时时间
@函数返回：无
@修改时间：2022/03/15
@调用方法：lq_tft_delayms(1);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static void lq_tft_delayms(unsigned short ms)
{
  Cy_SysTick_DelayInUs(ms);
}

#define TFTSPI_CS_H     LQ_Pin_Write(TFTSPI_CS, 1);      /*!< CS管脚 */
#define TFTSPI_SCK_H    LQ_Pin_Write(TFTSPI_SCK, 1);     /*!< SCL管脚 */
#define TFTSPI_SDI_H    LQ_Pin_Write(TFTSPI_SDI, 1);     /*!< SDI管脚 */
#define TFTSPI_DC_H     LQ_Pin_Write(TFTSPI_DC, 1);      /*!< DC管脚 */
#define TFTSPI_RST_H    LQ_Pin_Write(TFTSPI_RST, 1);     /*!< RST管脚 */

#define TFTSPI_CS_L     LQ_Pin_Write(TFTSPI_CS, 0);     /*!< CS管脚 */
#define TFTSPI_SCK_L    LQ_Pin_Write(TFTSPI_SCK, 0);    /*!< SCL管脚 */
#define TFTSPI_SDI_L    LQ_Pin_Write(TFTSPI_SDI, 0);    /*!< SDI管脚 */
#define TFTSPI_DC_L     LQ_Pin_Write(TFTSPI_DC, 0);     /*!< DC管脚 */
#define TFTSPI_RST_L    LQ_Pin_Write(TFTSPI_RST, 0);    /*!< RST管脚 */

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Init(unsigned short type)
@功能说明：TFT初始化
@参数说明：type： 0:横屏  1：竖屏，2：竖屏180度，3：横屏180度(正视屏幕，排针在右侧)
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Init();
@备    注：初始化时调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Init(unsigned char type)
{

#if USE_QSPI
#ifdef CY_CORE_CM0
    LQ_SPI_InitConfig(SCB5_CLK_P07_2, SCB5_MISO_P07_0, SCB5_MOSI_P07_1, SCB5_SEL0_P07_3, 125000000);
#endif 
#ifdef CY_CORE_CM7_0
    LQ_SPI_InitConfig(SCB5_CLK_P07_2, SCB5_MISO_P07_0, SCB5_MOSI_P07_1, SCB5_SEL0_P07_3, 62500000);
#endif
#else
  
  LQ_GPIO_Init_Config(TFTSPI_SCK, GPIO_OUT_STRONG);
  LQ_GPIO_Init_Config(TFTSPI_SDI, GPIO_OUT_STRONG);
  LQ_GPIO_Init_Config(TFTSPI_CS, GPIO_OUT_STRONG);

#endif
  //-----端口初始化----//
  LQ_GPIO_Init_Config(TFTSPI_DC, GPIO_OUT_STRONG);
  LQ_GPIO_Init_Config(TFTSPI_RST, GPIO_OUT_STRONG);


  TFTSPI_RST_L;
  lq_tft_delayms(50);
  TFTSPI_RST_H;
  lq_tft_delayms(50);
  TFTSPI_Write_Cmd(0x11);                   //关闭睡眠，振荡器工作
  lq_tft_delayms(50);
  TFTSPI_Write_Cmd(0x3a);                   //每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
  TFTSPI_Write_Byte(0x55);
  TFTSPI_Write_Cmd(0x26);
  TFTSPI_Write_Byte(0x04);
  TFTSPI_Write_Cmd(0xf2);                   //Driver Output Control(1)
  TFTSPI_Write_Byte(0x01);
  TFTSPI_Write_Cmd(0xe0);                   //Driver Output Control(1)
  TFTSPI_Write_Byte(0x3f);
  TFTSPI_Write_Byte(0x25);
  TFTSPI_Write_Byte(0x1c);
  TFTSPI_Write_Byte(0x1e);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x12);
  TFTSPI_Write_Byte(0x2a);
  TFTSPI_Write_Byte(0x90);
  TFTSPI_Write_Byte(0x24);
  TFTSPI_Write_Byte(0x11);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Cmd(0xe1);              //Driver Output Control(1)
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x05);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x15);
  TFTSPI_Write_Byte(0xa7);
  TFTSPI_Write_Byte(0x3d);
  TFTSPI_Write_Byte(0x18);
  TFTSPI_Write_Byte(0x25);
  TFTSPI_Write_Byte(0x2a);
  TFTSPI_Write_Byte(0x2b);
  TFTSPI_Write_Byte(0x2b);
  TFTSPI_Write_Byte(0x3a);
  TFTSPI_Write_Cmd(0xb1);      //0xb1       //设置屏幕刷新频率
  TFTSPI_Write_Byte(0x00);     //0x08       //DIVA=8
  TFTSPI_Write_Byte(0x00);      //0x08      //VPA =8，约90Hz  
  TFTSPI_Write_Cmd(0xb4);               //LCD Driveing control
  TFTSPI_Write_Byte(0x07);          //NLA=1,NLB=1,NLC=1
  TFTSPI_Write_Cmd(0xc0);              //LCD Driveing control  Power_Control1
  TFTSPI_Write_Byte(0x0a);
  TFTSPI_Write_Byte(0x02);
  TFTSPI_Write_Cmd(0xc1);              //LCD Driveing control
  TFTSPI_Write_Byte(0x02);
  TFTSPI_Write_Cmd(0xc5);              //LCD Driveing control
  TFTSPI_Write_Byte(0x4f);
  TFTSPI_Write_Byte(0x5a);
  TFTSPI_Write_Cmd(0xc7);              //LCD Driveing control
  TFTSPI_Write_Byte(0x40);
  TFTSPI_Write_Cmd(0x2a);              //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
  TFTSPI_Write_Byte(0x00);      //横坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);      //横坐标结束地址0x007f(127)
  TFTSPI_Write_Byte(0xa8);                //7f
  TFTSPI_Write_Cmd(0x2b);              //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
  TFTSPI_Write_Byte(0x00);      //纵坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);      //纵坐标结束地址0x009f(159)
  TFTSPI_Write_Byte(0xb3);                //9f
  TFTSPI_Write_Cmd(0x36);              //配置MPU和DDRAM对应关系
  if(type==1)
    TFTSPI_Write_Byte(0xC0);                  //竖屏显示          //MX=1,MY=1
  else  if(type==2)
    TFTSPI_Write_Byte(0x00);                  //竖屏翻转度显示         
  else  if(type==3)
    TFTSPI_Write_Byte(0x60);                  //横屏翻转度显示         
  else
    TFTSPI_Write_Byte(0xA0);                  //横屏显示

  TFTSPI_Write_Cmd(0xb7);              //LCD Driveing control
  TFTSPI_Write_Byte(0x00);             //CRL=0
  TFTSPI_Write_Cmd(0x29);              //开启屏幕显示
  TFTSPI_Write_Cmd(0x2c);              //设置为LCD接收数据/命令模式
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void tft18delay_1us(unsigned int Del)
@功能说明：简单延时函数
@参数说明：Del ：延时时间
@函数返回：无
@修改时间：2022/03/15
@调用方法：tft18delay_1us(1);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void tft18delay_1us(unsigned int Del)       //
{
  while(Del--)
  {
//    __asm("NOP");
     __NOP();
    

  }
}

#if USE_QSPI
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Cmd(unsigned short cmd)
@功能说明：写命令
@参数说明：cmd ：命令
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Cmd(unsigned char cmd)
{
  TFTSPI_DC_L;
  uint8_t buff[1];
  buff[0] = cmd;
  LQ_SPI_WriteArray(SCB5, buff, 1);

#ifdef CY_CORE_CM7_0
  Cy_SysTick_DelayInUs(2);
#endif
#ifdef CY_CORE_CM7_1
  Cy_SysTick_DelayInUs(2);
#endif
  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Byte(unsigned short dat)
@功能说明：写字节
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Byte(0x00);  //CRL=0
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Byte(unsigned char dat)
{
  TFTSPI_DC_H;
  uint8_t buff[1];
  buff[0] = dat;
  LQ_SPI_WriteArray(SCB5, buff, 1);

#ifdef CY_CORE_CM7_0
  Cy_SysTick_DelayInUs(2);
#endif
#ifdef CY_CORE_CM7_1
  Cy_SysTick_DelayInUs(2);
#endif
  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Word(unsigned short dat)
@功能说明：写半字
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Word(0xFFFF);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Word(unsigned short dat)
{
  TFTSPI_DC_H;
  uint8_t buff[2];
  buff[0] = dat >> 8;
  buff[1] = (uint8_t)dat;
  LQ_SPI_WriteArray(SCB5, buff, 2);
  
#ifdef CY_CORE_CM7_0
  Cy_SysTick_DelayInUs(2);
#endif   
#ifdef CY_CORE_CM7_1
  Cy_SysTick_DelayInUs(2);
#endif 
  
  while(Cy_SCB_SPI_IsBusBusy(SCB5) == true);       //等待总线传输完成
}

#else

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Cmd(unsigned short cmd)
@功能说明：写命令
@参数说明：cmd ：命令
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Cmd(unsigned char cmd)
{
  unsigned char i;

  TFTSPI_DC_L;// A0=0 ILI9163_A0=0;  //A0=0发送命令

  for(i=0;i<8;i++)
  {
    TFTSPI_SCK_L;tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
    if(cmd&0x80){TFTSPI_SDI_H;}// SDI=1
    else{TFTSPI_SDI_L;}// SDI=0
    TFTSPI_SCK_H;tft18delay_1us(1); // SCK=1  ILI9163_SCK = 1;
    cmd = (cmd<<1);
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Byte(unsigned short dat)
@功能说明：写字节
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Byte(0x00);  //CRL=0
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Byte(unsigned char dat)
{
  unsigned char i;

  TFTSPI_DC_H;// A0=1  ILI9163_A0=1;          //A0=1发送数据

  for(i=0;i<8;i++)
  {
    TFTSPI_SCK_L;tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
    if(dat&0x80){TFTSPI_SDI_H;}// SDI=1
    else{TFTSPI_SDI_L;}// SDI=0
    TFTSPI_SCK_H;tft18delay_1us(1); // SCK=1 ILI9163_SCK = 1;
    dat = (dat<<1);
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Write_Word(unsigned short dat)
@功能说明：写半字
@参数说明：dat ：数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Write_Word(0xFFFF);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Write_Word(unsigned short dat)
{
  unsigned char i;


  TFTSPI_DC_H;// A0=1  ILI9163_A0=1;

  for(i=0;i<16;i++)
  {
    TFTSPI_SCK_L;tft18delay_1us(1); // SCK=0  ILI9163_SCK=0;
    if(dat&0x8000){ TFTSPI_SDI_H;}// SDI=1
    else{TFTSPI_SDI_L;}// SDI=0
    TFTSPI_SCK_H;tft18delay_1us(1); // SCK=1  ILI9163_SCK=1;
    dat<<=1;
  }
}

#endif

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Set_Pos(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye)
@功能说明：重新定位输入信息位置
@参数说明：xs:起始x ys:起始y xe:结束x ys:结束y
@函数返回：
@修改时间：2022/03/15
@调用方法：TFTSPI_Set_Pos(10, 20, 30, 40);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye)
{
  TFTSPI_Write_Cmd(0x2A);       //Colulm addRSTs set
  TFTSPI_Write_Byte(0x00);      //行起始坐标高8位，始终为零
  TFTSPI_Write_Byte(xs);            //行起始坐标低8位
  TFTSPI_Write_Byte(0x00);      //行终止坐标高8位，始终为零
  TFTSPI_Write_Byte(xe);            //行终止坐标低8位
  TFTSPI_Write_Cmd(0x2B);       //Colulm addRSTs set
  TFTSPI_Write_Byte(0x00);      //列起始坐标高8位，始终为零
  TFTSPI_Write_Byte(ys);            //列起始坐标低8位
  TFTSPI_Write_Byte(0x00);      //列终止坐标高8位，始终为零
  TFTSPI_Write_Byte(ye);            //列终止坐标低8位
  TFTSPI_Write_Cmd(0x2C);       //GRAM接收MCU数据或命令
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Addr_Rst(void)
@功能说明：重置地址
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Addr_Rst();
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Addr_Rst(void)
{
  TFTSPI_Write_Cmd(0x2a);     //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
  TFTSPI_Write_Byte(0x00);       //横坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);       //横坐标结束地址0x007f(127)
  TFTSPI_Write_Byte(0xa8);       //7f  
  TFTSPI_Write_Cmd(0x2b);     //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
  TFTSPI_Write_Byte(0x00);       //纵坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);       //纵坐标结束地址0x009f(159)
  TFTSPI_Write_Byte(0xb3);       //9f  
  TFTSPI_Write_Cmd(0x2C);     //GRAM接收MCU数据或命令
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Fill_Area(unsigned short xs,unsigned short ys,unsigned short xe,
@                                   unsigned short ye,unsigned short color)
@功能说明：填充指定区域
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor ：填充的颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color)
{
  unsigned int i,j;
  
  TFTSPI_Set_Pos(xs,ys,xe,ye);
  for(i=0;i<(xe-xs);i++)  
  {
    for(j=0;j<(ye-ys);j++)
    {
      TFTSPI_Write_Word(color);
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_CLS(unsigned short color)
@功能说明：全屏显示单色画面
@参数说明：color ：填充的颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法： TFTSPI_CLS(u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_CLS(unsigned short color)
{
  unsigned int i,j;
  
  TFTSPI_Addr_Rst();
  
  for(i=0;i<TFT18W;i++)      //160
  {
    for(j=0;j<TFT18H;j++)
    {
      TFTSPI_Write_Word(color);
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Part(unsigned short xs,unsigned short ys,
@                               unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：填充矩形区域
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：填充的颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
{
  unsigned char i,j;
  
  TFTSPI_Set_Pos(xs,ys,xe,ye);
  for(j=0;j<(ye-ys+1);j++)
  {
    for(i=0;i<(xe-xs+1);i++)
    {
      TFTSPI_Write_Word(color_dat);
    }
  }   
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Rectangle(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：画矩形边框
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
{
  TFTSPI_Draw_Line(xs,ys,xs,ye,color_dat);    //画矩形左边
  TFTSPI_Draw_Line(xe,ys,xe,ye,color_dat);    //画矩形右边
  TFTSPI_Draw_Line(xs,ys,xe,ys,color_dat);    //画矩形上边
  TFTSPI_Draw_Line(xs,ye,xe,ye,color_dat);    //画矩形下边
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Circle(unsigned short x,unsigned short y,unsigned short r,unsigned short color_dat)
@功能说明：画圆
@参数说明：x ：圆心x   (0-127)y ：圆心y   (0-159)r ：半径    (0-128)color_dat ：颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Draw_Circle(50, 50, 30, u16YELLOW);
@备    注：圆心坐标不要超出屏幕范围
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat)
{
  unsigned char dx, dy = r; 
  
  if((x>=r) &&((TFT18W-x)>=r) && (y>=r) && ((TFT18H-y)>=r))     //确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
  {
    for(dx = 0; dx <= r; dx++)
    {
      while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
      TFTSPI_Draw_Dot(x + dx, y - dy, color_dat);
      TFTSPI_Draw_Dot(x - dx, y - dy, color_dat);
      TFTSPI_Draw_Dot(x - dx, y + dy, color_dat);
      TFTSPI_Draw_Dot(x + dx, y + dy, color_dat);
      
      TFTSPI_Draw_Dot(x + dy, y - dx, color_dat);
      TFTSPI_Draw_Dot(x - dy, y - dx, color_dat);
      TFTSPI_Draw_Dot(x - dy, y + dx, color_dat);
      TFTSPI_Draw_Dot(x + dy, y + dx, color_dat);
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Draw_Line(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat)
@功能说明：画线
@参数说明：xs ：起始xys ：起始yxe ：结束xys ：结束ycolor_dat ：颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat)
{
  int i,ds;
  int dx,dy,inc_x, inc_y;
  int xerr = 0, yerr = 0;                       //初始化变量
  
  if(xs==xe)                            //如果是画垂直线则只需对竖直坐标计数
  {
    TFTSPI_Set_Pos(xs,ys,xe,ye);
    for(i=0;i<(ye-ys+1);i++)
    {
      TFTSPI_Write_Word(color_dat);
    }
  }
  else if(ys==ye)                       //如果是水平线则只需要对水平坐标计数
  {
    TFTSPI_Set_Pos(xs,ys,xe,ye);
    for(i=0;i<(xe-xs+1);i++)
    {
      TFTSPI_Write_Word(color_dat);
    }
  } 
  else                              //如果是斜线，则重新计算，使用画点函数画出直线
  {
    dx = xe - xs;                       //计算坐标增量
    dy = ye - ys;
    
    if(dx > 0) inc_x = 1;                   //设置单步方向
    else 
    {
      inc_x = -1; dx = -dx;
    }
    if(dy > 0) inc_y = 1;                   //设置单步方向
    else
    {
      inc_y = -1; dy = -dy;
    }
    
    if(dx > dy) {ds = dx;}                  //选取基本增量坐标轴
    else        {ds = dy;}
    
    for(i = 0; i <= ds+1; i++)                  //画线输出
    {
      TFTSPI_Draw_Dot(xs, ys,color_dat);    //画点
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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：float constrain_float(float amt, float low, float high)
@功能说明：画点
@参数说明：x ：x   y ：y   color_dat ：颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Draw_Dot(10, 20, u16YELLOW);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat)
{
  TFTSPI_Set_Pos(x,y,x,y);
  TFTSPI_Write_Word(color_dat);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Pic(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,const unsigned char *ppic)
@功能说明：显示图片
@参数说明：ys ：起始y  xs ：起始x  xe ：结束x  ys ：结束y  ppic ：图片数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Show_Pic(138,2,20,10,gImage_Battery_S);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic)
{
  unsigned int i,j,k=0;
  
  TFTSPI_Set_Pos(xs,ys,xe-1,ye);
  for(i=0;i<ye-ys;i++)
  { 
    for(j=0;j<xe-xs;j++)
    {
      TFTSPI_Write_Word(((unsigned short)ppic[k])<<8|ppic[k+1]);
      k++;  k++;
    }
  }    
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P6X8(unsigned short x, unsigned short y, unsigned short c_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符输出(6*8字体)
@参数说明：x: 0 - 20(行)  y: 0 - 19(列)  word_color: 字体颜色  back_color: 背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P6X8(0, 6, txt, u16RED, u16BLUE);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P6X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned short word_color,unsigned short back_color)
{
  unsigned char i,j;
  
  TFTSPI_Set_Pos(x*6,y*8,(x+1)*6-1,(y+1)*8-1);          //定位字符显示区域

  for(j=0; j<8; j++)
  {
    for(i=0; i<6; i++)
    {
      if((Font_code8[c_dat-32][i]) & (0x01<<j))
        TFTSPI_Write_Word(word_color);
      else
        TFTSPI_Write_Word(back_color);
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X8(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*8字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P8x8(1, 1, "123456", u16YELLOW, u16RED);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned short word_color,unsigned short back_color)
{
  unsigned char i,j;
  
  TFTSPI_Set_Pos(x*8,y*8,(x+1)*8-1,(y+1)*8-1);          //定位字符显示区域
  
  for(j=0; j<8; j++)
  {
    TFTSPI_Write_Word(back_color);
    for(i=0; i<6; i++)
    {
      if((Font_code8[c_dat-32][i]) & (0x01<<j))
        TFTSPI_Write_Word(word_color);
      else
        TFTSPI_Write_Word(back_color);
    }
    TFTSPI_Write_Word(back_color);
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X16(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P8x16(1, 1, "123456", u16YELLOW, u16RED);
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X16(unsigned char x, unsigned char y, unsigned char c_dat,unsigned short word_color,unsigned short back_color)
{
  unsigned char i,j;
  
  TFTSPI_Set_Pos(x*8,y*16,(x+1)*8-1,(y+1)*16-1);            //定位字符显示区域
  
  for(j=0; j <16; j++)
  {
    for(i=0; i<8; i++)
    {
      if((Font_code16[c_dat-32][j]) & (0x01<<i))
        TFTSPI_Write_Word(word_color);
      else
        TFTSPI_Write_Word(back_color);
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P6X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符串输出(6*8字体)
@参数说明：x: x:0 -15(行)  y: y:0 -9 (列)  word_color:字体颜色  back_color:背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color)
{
  while(*s_dat) TFTSPI_P6X8(x++, y, *s_dat++,word_color,back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶字符串输出(8*8字体)
@参数说明：x: x:0 -15(行)  y: y:0 -9 (列)  word_color:字体颜色  back_color:背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color)
{
  while(*s_dat) TFTSPI_P8X8(x++, y, *s_dat++,word_color,back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P8X16Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(8*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P8x16Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：汉字只能是字库里的 字库没有的需要自行添加
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color)
{
  while(*s_dat) TFTSPI_P8X16(x++, y, *s_dat++,word_color,back_color);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_P16X16Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color)
@功能说明：液晶汉字字符串输出(16*16字体)
@参数说明：x: 0 - 7(行) y: 0 - 9(列) word_color: 字体颜色 back_color: 背景颜色
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
@备    注：汉字只能是字库里的 字库没有的需要自行添加
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color)
{
  unsigned char wm=0,ii = 0,i,j;
  int adder=1;
  
  while(s_dat[ii] != '\0')
  {
    wm = 0;
    adder = 1;
    while(hanzi_Idx[wm] > 127)
    {
      if(hanzi_Idx[wm] == (uint8_t)s_dat[ii])
      {
        if(hanzi_Idx[wm + 1] == s_dat[ii + 1])
        {
          adder = wm * 16;
          break;
        }
      }
      wm += 2;
    }
    
    if(adder != 1)// 显示汉字
    {
      TFTSPI_Set_Pos(x*16,y*16,(x+1)*16-1,(y+1)*16-1);          //定位字符显示区域
      for(j=0; j <32; j++)
      {
        for(i=0; i<8; i++)
        {
          if((hanzi16x16[adder]) &  (0x80>>i))
            TFTSPI_Write_Word(word_color);
          else
            TFTSPI_Write_Word(back_color);
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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Pic2(unsigned short xs,unsigned short ys,unsigned short w,unsigned short h,const unsigned char *ppic)
@功能说明：显示图片
@参数说明：xs ：起始x ys ：起始y xe ：结束x ys ：结束y ppic ：图片数据
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Show_Pic2(138,2,20,10,gImage_Battery_S);
@备    注：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic)
{
  unsigned int i;
  TFTSPI_Set_Pos(xs,ys,xs+w-1,ys+h);
  for(i=0;i<w*h;i++)
  {
    TFTSPI_Write_Word((ppic[2*i]<<8)+(ppic[2*i+1])); //高位在前，且两个数据组合成一个16位数据表示像素值
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Battery_Icon(void)
@功能说明：显示电池图标
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@调用方法： TFTSPI_Show_Battery_Icon();
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Battery_Icon(void)
{
  TFTSPI_Show_Pic2(138,2,20,10,gImage_Battery_S); //显示电池图标20*10像素
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Title(void)
@功能说明：显示标签
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Show_Title();
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Title(void)
{
  TFTSPI_Show_Pic2(0,0,92,28,gImage_title2);
  TFTSPI_Draw_Line(0,24,160,24,u16BLACK);   //补齐分割线
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Show_Logo(unsigned short xs,unsigned short ys)
@功能说明：显示龙邱LOG
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Show_Logo(unsigned char xs,unsigned char ys)
{
  TFTSPI_Show_Pic2(xs,ys,160,54,gImage_lo);       //80,gImage_ftflogo);     //
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_Road(unsigned short wide_start, unsigned short high_start, unsigned short high, unsigned short wide, unsigned char *Pixle)
@功能说明：TFT18屏 unsigned short 灰度数据显示
@参数说明：high_start ： 显示图像开始位置wide_start：显示图像开始位置 high： 显示图像高度 wide：显示图像宽度 Pixle：显示图像数据地址
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
@备    注：注意 屏幕左上为 （0，0）
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_Road(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
  unsigned long i;
  /* 设置显示范围 */
  TFTSPI_Set_Pos(wide_start, high_start, wide_start + wide - 1, high_start + high - 1);
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
    TFTSPI_Write_Word (color);
  }

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TFTSPI_BinRoad(unsigned short wide_start, unsigned short high_start, unsigned short high, unsigned short wide, unsigned char *Pixle)
@功能说明：TFT18屏 unsigned short 二值化数据显示
@参数说明：high_start ： 显示图像开始位置wide_start：显示图像开始位置 high： 显示图像高度 wide：显示图像宽度 Pixle：显示图像数据地址
@函数返回：无
@修改时间：2022/03/15
@调用方法：TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);// 显示摄像头图像
@备    注：注意 屏幕左上为 （0，0）
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTSPI_BinRoad(unsigned char wide_start, unsigned char high_start, unsigned char high, unsigned char wide, unsigned char *Pixle)
{
  unsigned long i;
  /* 设置显示范围 */
  TFTSPI_Set_Pos(wide_start, high_start, wide_start - 1 + wide, high_start - 1 + high);

  unsigned long temp = high * wide;
  /* 显示图像 */
  for(i = 0; i < temp; i++)
  {
    if(Pixle[i])
    {
      /* 显示 */
      TFTSPI_Write_Word (0xffff);
    }
    else
    {
      /* 显示 */
      TFTSPI_Write_Word (0);
    }
  }
}

//void TFTSPI_ShowBeacon(void )
//{
//    unsigned short i = 0, j = 0;
//
//    /* 设置显示范围 */
//    TFTSPI_Set_Pos(0, 0, 159, 120);
//
//    /* 显示图像 */
//    /* 二值化 */
//    for (i = 0; i < LCDH; i++)
//    {
//        for (j = 14; j < LCDW-15; j++)
//        {
//            if(Bin_Image[i][j])
//            {
//               /* 显示 */
//               TFTSPI_Write_Word (0xffff);
//            }
//            else
//            {
//              /* 显示 */
//               TFTSPI_Write_Word (0);
//            }
//        }
//    }
//
//}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

volatile int test_num=0;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_TFT18(void)
@功能说明：测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/03/15
@调用方法：
@备    注：USE_QSPI 宏定义切换硬软件SPI 硬件刷新一次屏幕为104ms 软件刷新一次屏幕为772ms
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_TFT18(void)
{ 
  GPIO_LED_Init();
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏，2，3
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
  Cy_SysTick_DelayInUs(1000000);         //延时等待


  while(1)
  {
    
  TFTSPI_CLS(u16BLACK);   //蓝色屏幕
  TFTSPI_CLS(u16GREEN);   //蓝色屏幕
  TFTSPI_CLS(u16RED);   //蓝色屏幕
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_CLS(u16YELLOW);   //蓝色屏幕
//    sprintf(txt, "test_num:%05d", count);                    //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
//    TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE);              //将txt中 内容显示出来
    LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁
//    Cy_SysTick_DelayInUs(100);              //延时等待
//    count++;
  }
}
