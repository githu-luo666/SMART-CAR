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


////////////////////////////////////////////////////////////////////////////////

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Soft_SPI_Init(void)
@功能说明：模拟SPI初始化
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_Init(void)
{  
    LQ_GPIO_Init_Config(Soft_SPI_SCK_PIN, GPIO_OUT_PP_UP);
    LQ_GPIO_Init_Config(Soft_SPI_MISO_PIN,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(Soft_SPI_MOSI_PIN, GPIO_OUT_PP_UP);
    LQ_GPIO_Init_Config(Soft_SPI_CS_PIN,GPIO_OUT_PP_UP);
      
    Soft_SPI_SCK_OUT_L;  
    Soft_SPI_CS_OUT_H;  
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
@功能说明：模拟SPI读写数据及长度
@参数说明：uint8_t *buf数据指针,u16 len长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
{
  Soft_SPI_CS_OUT_L;
  while(len--)
  {
      int i=0;
      for(;i<8;i++)
      {  
        if(lqbuff[0] >= 0x80)
        {
            Soft_SPI_MOSI_OUT_H;
        }else{
            Soft_SPI_MOSI_OUT_L;  
        }   
        lqbuff[0]<<=1;
        Soft_SPI_SCK_OUT_H;   
        lqbuff[0]|=Soft_SPI_MISO_IN;
        for(int n=0;n<10;n++);      
        Soft_SPI_SCK_OUT_L;  
        for(int n=0;n<10;n++);
      }
      lqbuff++;
  }
  Soft_SPI_CS_OUT_H;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI从设备读取数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
{  
  unsigned short i;
  buf[0] = reg | 0x80;  
  Soft_SPI_ReadWriteNbyte(buf, len + 1); 
  for(i=0;i<len;i++)
    buf[i]=buf[i+1];
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI向设备写入数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f;          //先发送寄存器
    buff[1] = value;        //再发送数据
    Soft_SPI_ReadWriteNbyte(buff, 2);
}

