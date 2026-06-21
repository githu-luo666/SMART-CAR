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
code flash : 4160KB  0x10000000-0x103F0000 32KB*126(LS)+8KB*16(SS) LS:大扇区 SS:小扇区
work flash : 256KB   0x14000000-0x14030000 2KB*96(LS)+128B*512(SS) LS:大扇区 SS:小扇区
      SRAM : 768KB
       ROM : 64KB
P22
*******************************************************************************/
#include "LQ_flash.h"
#include "LQ_TFT18.h"
#include "LQ_key.h"
#include "LQ_UART.h"
#include <stdio.h>
#include <stdlib.h>

//整个扇区会被擦除，验证，写入。擦除后全为1
/*************************************************************************
*  函数名称：void LQ_WorkFlash_WriteByte(uint32_t SectorAddr, uint8_t *data, uint32_t Size)
*  功能说明：flash写入8位数据
*  参数说明：SectorAddr：存放地址   data：数据地址   Size：数据量大小
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*************************************************************************/
void LQ_WorkFlash_WriteByte(uint32_t SectorAddr, uint8_t *data, uint32_t Size)
{  
  //擦除扇区 阻塞模式
  Cy_FlashSectorErase(SectorAddr, CY_FLASH_DRIVER_BLOCKING);
  //验证扇区 阻塞模式
  Cy_WorkFlashBlankCheck(SectorAddr, CY_FLASH_DRIVER_BLOCKING);
  
  /** Programming **/
  for(uint32_t i_addr = SectorAddr, i_addrOffset = 0; i_addrOffset < Size&& i_addr < SectorAddr + CY_WORK_LES_SIZE_IN_BYTE; i_addr+=4, i_addrOffset+=4)
  {
      uint32_t i_dataPos = i_addrOffset % Size;

      // Flash 
      Cy_FlashWriteWork(i_addr, (uint32_t*)&data[i_dataPos], CY_FLASH_DRIVER_BLOCKING);
  }
}

/*************************************************************************
*  函数名称：void LQ_WorkFlash_ReadByte(uint32_t SectorAddr, uint8_t *data, uint32_t Size)
*  功能说明：flash读取8位数据
*  参数说明：SectorAddr：存放地址   data：数据地址   Size：数据量大小
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*************************************************************************/
void LQ_WorkFlash_ReadByte(uint32_t SectorAddr, uint8_t *data, uint32_t Size)
{
  uint32_t* p_TestFlsTop = (uint32_t*)SectorAddr;
  uint32_t SectorSizeInWord = Size / 4ul+1;
  uint32_t data_word;
  
  for(uint32_t i_wordId = 0, sizeID = 0; sizeID < Size; i_wordId++)
  {
      uint32_t i_dataPos = i_wordId % SectorSizeInWord;
      data_word = p_TestFlsTop[i_dataPos];
      data[sizeID++] = data_word>>0 &0xFF;
      data[sizeID++] = data_word>>8 &0xFF;
      data[sizeID++] = data_word>>16&0xFF;
      data[sizeID++] = data_word>>24&0xFF;
  }
}

/*************************************************************************
*  函数名称：void LQ_WorkFlash_WriteWord(uint32_t SectorAddr, uint32_t *data, uint32_t Size)
*  功能说明：flash写入32位数据
*  参数说明：SectorAddr：存放地址   data：数据地址   Size：数据量大小
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*************************************************************************/
void LQ_WorkFlash_WriteWord(uint32_t SectorAddr, uint32_t *data, uint32_t Size)
{  
  //擦除扇区 阻塞模式
  Cy_FlashSectorErase(SectorAddr, CY_FLASH_DRIVER_BLOCKING);
  //验证扇区 阻塞模式
  Cy_WorkFlashBlankCheck(SectorAddr, CY_FLASH_DRIVER_BLOCKING);
  
  /** Programming **/
  for(uint32_t i_addr = SectorAddr, i_addrOffset = 0; i_addrOffset < Size && i_addr < TEST_W_LS_ADDR + CY_WORK_LES_SIZE_IN_WORD; i_addr+=4, i_addrOffset++)
  {
      uint32_t i_dataPos = i_addrOffset % Size;

      // Flash 
      Cy_FlashWriteWork(i_addr, (uint32_t*)&data[i_dataPos], CY_FLASH_DRIVER_BLOCKING);
  }
}

/*************************************************************************
*  函数名称：void LQ_WorkFlash_ReadWord(uint32_t SectorAddr, uint32_t *data, uint32_t Size)
*  功能说明：flash读取32位数据
*  参数说明：SectorAddr：存放地址   data：数据地址   Size：数据量大小
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*************************************************************************/
void LQ_WorkFlash_ReadWord(uint32_t SectorAddr, uint32_t *data, uint32_t Size)
{
  uint32_t* p_TestFlsTop = (uint32_t*)SectorAddr;
  uint32_t SectorSizeInWord = 0;
  
  for(uint32_t i_wordId = 0; i_wordId < Size; i_wordId++)
  {
    SectorSizeInWord = p_TestFlsTop[i_wordId];
      data[i_wordId] = SectorSizeInWord;
  }
}
/*************************************************************************
*  函数名称：void Test_EEPROM(void)
*  功能说明：测试flash读写功能，flash内存分布见“LQ_flash.h”文件
*  参数说明：无
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：通过拨码开关2切换读写数据大小 拨码开关2拨到ON 复位单片机 切换到32位读写
*                                        拨码开关2拨到OFF 复位单片机 切换到8位读写
            按键0：生成随机数据（假随机）
            按键1：存入数据
            按键2：读取数据
*************************************************************************/
void Test_EEPROM(void)
{
  char txt[32];
  uint8_t wirte_byte[10]={0};   //8位数据写入
  uint8_t read_byte[10]={0};    //8位数据读取
  uint32_t write_word[6]={0};   //32位数据写入
  uint32_t read_word[6]={0};    //32位数据读取
  
  GPIO_KEY_Init();
  TFTSPI_Init(3);
  TFTSPI_Show_Logo(0,37);
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(3,0,"LQ EEPROM Test",u16WHITE,u16BLACK);     //字符串显示
  // Initialization 
#ifdef CY_CORE_CM7_0
  SCB_DisableDCache(); // Disables. Because cores share SRAM pointed by SROM scratch address
#endif
  Cy_FlashInit(false /*blocking*/);
  
  if(KEY_Read(DSW1) == 1)
  {
    while(1)
    {
      //按键0按下后更新数据
      if(KEY_Read(KEY0) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY0) == 0)
        {
          while(KEY_Read(KEY0) == 0);
          for(int i = 0; i < 10; i++)
          {
            /*产生10个位于-127到127区间的随机数放入写入8bit的数组中*/
            wirte_byte[i]=rand()%254-127;
          }
          /*屏幕显示这些数据*/
          sprintf(txt, "W:%02X %02X %02X %02X %02X", wirte_byte[0], wirte_byte[1], wirte_byte[2], wirte_byte[3], wirte_byte[4]);
          TFTSPI_P8X16Str(0, 1, txt, u16RED,u16BLACK);
          sprintf(txt, "W:%02X %02X %02X %02X %02X", wirte_byte[5], wirte_byte[6], wirte_byte[7], wirte_byte[8], wirte_byte[9]);
          TFTSPI_P8X16Str(0, 2, txt, u16RED,u16BLACK);
        }
      }
      //按键1按下后存入数据
      if(KEY_Read(KEY1) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY1) == 0)
        {
          while(KEY_Read(KEY1) == 0);
          
          LQ_WorkFlash_WriteByte(TEST_W_LS_ADDR, wirte_byte, 10);
        }
      }
      //按键2按下后读取数据
      if(KEY_Read(KEY2) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY2) == 0)
        {
          while(KEY_Read(KEY2) == 0);
          
          LQ_WorkFlash_ReadByte(TEST_W_LS_ADDR, read_byte, 10);
          sprintf(txt, "R:%02X %02X %02X %02X %02X", read_byte[0], read_byte[1], read_byte[2], read_byte[3], read_byte[4]);
          TFTSPI_P8X16Str(0, 6, txt, u16RED,u16BLACK);
          sprintf(txt, "  %02X %02X %02X %02X %02X", read_byte[5], read_byte[6], read_byte[7], read_byte[8], read_byte[9]);
          TFTSPI_P8X16Str(0, 7, txt, u16RED,u16BLACK);
        }
      }
      LED_Ctrl(LED0, RVS);
      Cy_SysTick_DelayInUs(100000);
    }
  }else
  {
    while(1)
    {
      //按键0按下后更新数据
      if(KEY_Read(KEY0) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY0) == 0)
        {
          while(KEY_Read(KEY0) == 0);
          
          
          for(int i = 0; i < 10; i++)
          {
            /*产生10个随机数放入写入32bit的数组中*/
            write_word[i]=rand()%131070-65535;
          }
          /*屏幕显示这些数据*/
          sprintf(txt, "W:%08X %08X", write_word[0], write_word[1]);
          TFTSPI_P8X16Str(0, 1, txt, u16RED,u16BLACK);
          sprintf(txt, "  %08X %08X", write_word[2], write_word[3]);
          TFTSPI_P8X16Str(0, 2, txt, u16RED,u16BLACK);
          sprintf(txt, "  %08X %08X", write_word[4], write_word[5]);
          TFTSPI_P8X16Str(0, 3, txt, u16RED,u16BLACK);
        }
      }
      //按键1按下后存入数据
      if(KEY_Read(KEY1) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY1) == 0)
        {
          while(KEY_Read(KEY1) == 0);
          LQ_WorkFlash_WriteWord(TEST_W_LS_ADDR, write_word, 6);
        }
      }
      //按键2按下后读取数据
      if(KEY_Read(KEY2) == 0)
      {
        Cy_SysTick_DelayInUs(10000);
        if(KEY_Read(KEY2) == 0)
        {
          while(KEY_Read(KEY2) == 0);
          
          LQ_WorkFlash_ReadWord(TEST_W_LS_ADDR, read_word, 6);
          sprintf(txt, "R:%08X %08X", read_word[0], read_word[1]);
          TFTSPI_P8X16Str(0, 5, txt, u16RED,u16BLACK);
          sprintf(txt, "  %08X %08X", read_word[2], read_word[3]);
          TFTSPI_P8X16Str(0, 6, txt, u16RED,u16BLACK);
          sprintf(txt, "  %08X %08X", read_word[4], read_word[5]);
          TFTSPI_P8X16Str(0, 7, txt, u16RED,u16BLACK);
        }
      }
      LED_Ctrl(LED0, RVS);
      Cy_SysTick_DelayInUs(100000);
    }
  }
}




















































