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
#include "LQ_VL53.h"
#include "include.h"

/* 选择屏幕，使用TFT，请注释以下OLED宏定义 */
//#define OLED

/*************************************************************************
*  函数名称：void Test_Vl53(void)
*  功能说明：测试VL53测距模块
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void Test_Vl53(void)
{

#ifdef OLED
  OLED_Init();
  OLED_CLS();
  OLED_P8x16Str(5,0,"LQ VL53 Test");
#else
  TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示
  TFTSPI_CLS(u16BLUE);           //清屏
  TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
  char txt[16];

  IIC_Init();

//  unsigned char VL53_STAR = 0x02;    //0x02 连续测量模式    0x01 单次测量模式
  unsigned char dis_buff[2];
  uint16_t dis, last_dis = 0;
  /* 开始测距 */
  VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53L0X_Mode2);
  while(1)
  {
    /* 获取测量数据 */
    VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
    /* 转换数据 */
    dis = (dis_buff[0]<<8) | (dis_buff[1]);
    /* 超过2M没有障碍 */
    if(dis > 8000)
    {
      dis = 0;
    }
    if(dis == 20)
    {
      dis = last_dis;
    }
    last_dis = dis;
    sprintf(txt, "DIS %5d mm",dis);
#ifdef OLED
    OLED_P8x16Str(0,5,txt);
#else
    TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
    delay_ms(100);
  LQ_Printf( "DIS %5d mm\n",dis);
  }
}

/*************************************************************************
*  函数名称：void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char data)
*  功能说明：VL53 写1个寄存器 
*  参数说明：dev：    IIC地址
             reg：    寄存器 
             data：   写入数据
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char data)
{
  IIC_WriteByteToSlave(dev<<1, reg, data);
}

/*************************************************************************
*  函数名称：void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data)
*  功能说明：VL53 读1个寄存器 
*  参数说明：dev：    IIC地址
             reg：    寄存器 
             length：   数据长度  
             data：数据缓存地址
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data)
{
  IIC_ReadMultByteFromSlave(dev<<1, reg, length, data);
}
