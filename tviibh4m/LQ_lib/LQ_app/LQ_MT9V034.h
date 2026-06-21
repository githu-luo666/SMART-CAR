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
#ifndef __LQ_MT9V034_H_
#define __LQ_MT9V034_H_

#define MT9V034_IMAGE_H 120 
#define MT9V034_IMAGE_W 188 
#define MT9V034_IMAGE_SIZE 22560 

#define MT9V034_SCL_PIN   P13_2   
#define MT9V034_SDA_PIN   P13_1   

#define MT9V034_SCL_Out LQ_GPIO_Init_Config(MT9V034_SCL_PIN, GPIO_OUT_PP_UP); 
#define MT9V034_SDA_Out LQ_GPIO_Init_Config(MT9V034_SDA_PIN, GPIO_OUT_PP_UP); 
#define MT9V034_SDA_In  LQ_GPIO_Init_Config(MT9V034_SDA_PIN, GPIO_IN_DOWN);   
#define MT9V034_SCL_High LQ_Pin_Write(MT9V034_SCL_PIN, 1); 
#define MT9V034_SCL_Low  LQ_Pin_Write(MT9V034_SCL_PIN, 0); 
#define MT9V034_SDA_High LQ_Pin_Write(MT9V034_SDA_PIN, 1); 
#define MT9V034_SDA_Low LQ_Pin_Write(MT9V034_SDA_PIN, 0);  
#define MT9V034_SDA_Data LQ_Pin_Read_In(MT9V034_SDA_PIN)   

#define CAMERA_CONTRAST 0x03c7                  // 调整相机图像的对比度   
#define CAMERA_AUTO_EXPOSURE 0                 // 值为 0 表示关闭相机的自动曝光功能  0
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS 32   // 定义相机自动曝光时的目标亮度值  50
#define CAMERA_MAX_EXPOSURE_TIME baoguang           // 定义相机允许的最大曝光时间
#define CAMERA_MIN_EXPOSURE_TIME baoguang              // 定义相机允许的最小曝光时间   
#define CAMERA_EXPOSURE_TIME baoguang               // 定义相机的曝光时间          

extern  uint8_t mt9v034Image[MT9V034_IMAGE_H][MT9V034_IMAGE_W];
extern volatile uint8_t mt9v034Flag;     
extern volatile uint32_t mt9v034Cnt;     
void MT9V034_Get_Image(unsigned char *IMG);
extern int baoguang;

void Test_MT9V034();
int MT9V034_Init(unsigned char fps);
void MT9V034_SetFrameRate(unsigned char fps);
static void MT9V034_SetFrameResolution(unsigned short height, unsigned short width);
void MT9V034_SetAutoExposure(char enable);









/*********************************************************************
 *摄像头SCCB底层驱动
 *内部调用
 ***********************************************************************/
void SCCB_Init(void);
void SCCB_Wait(void);
void SCCB_Stop(void);
void SCCB_Star(void);
unsigned char SCCB_SendByte(unsigned char Data);
void SCCB_RegWrite(unsigned char Device, unsigned char Address, unsigned short Data);
unsigned char SCCB_RegRead(unsigned char Device, unsigned char Address, unsigned short *Data);
void MTV_IICWriteReg16(unsigned char reg, unsigned short val);

#define MT9V034_I2C_ADDR 0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT 480
#define MAX_IMAGE_WIDTH 752
#define MT9V034_PIXEL_ARRAY_HEIGHT 492
#define MT9V034_PIXEL_ARRAY_WIDTH 782
#define MT9V034_CHIP_VERSION 0x00
#define MT9V034_CHIP_ID 0x1324

#define MT9V034_COLUMN_START 0x01
#define MT9V034_COLUMN_START_MIN 1
#define MT9V034_COLUMN_START_DEF 1
#define MT9V034_COLUMN_START_MAX 752

#define MT9V034_ROW_START 0x02
#define MT9V034_ROW_START_MIN 4
#define MT9V034_ROW_START_DEF 4
#define MT9V034_ROW_START_MAX 482

#define MT9V034_WINDOW_HEIGHT 0x03
#define MT9V034_WINDOW_HEIGHT_MIN 1
#define MT9V034_WINDOW_HEIGHT_DEF 64
#define MT9V034_WINDOW_HEIGHT_MAX 480

#define MT9V034_WINDOW_WIDTH 0x04
#define MT9V034_WINDOW_WIDTH_MIN 1
#define MT9V034_WINDOW_WIDTH_DEF 64
#define MT9V034_WINDOW_WIDTH_MAX 752

#define MINIMUM_HORIZONTAL_BLANKING 91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING 0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN 43
#define MT9V034_HORIZONTAL_BLANKING_MAX 1023

#define MT9V034_VERTICAL_BLANKING 0x06
#define MT9V034_VERTICAL_BLANKING_MIN 4
#define MT9V034_VERTICAL_BLANKING_MAX 3000

#define MT9V034_CHIP_CONTROL 0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL (1 << 8)

#define MT9V034_SHUTTER_WIDTH1 0x08
#define MT9V034_SHUTTER_WIDTH2 0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL 0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH 0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN 1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF 480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX 32767

#define MT9V034_RESET 0x0C

#define MT9V034_READ_MODE 0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT 0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT 2
#define MT9V034_READ_MODE_ROW_BIN_2 (1 << 0)
#define MT9V034_READ_MODE_ROW_BIN_4 (1 << 1)
#define MT9V034_READ_MODE_COL_BIN_2 (1 << 2)
#define MT9V034_READ_MODE_COL_BIN_4 (1 << 3)
#define MT9V034_READ_MODE_ROW_FLIP (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE 0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR (1 << 6)

#define MT9V034_V1_CTRL_REG_A 0x31
#define MT9V034_V2_CTRL_REG_A 0x32
#define MT9V034_V3_CTRL_REG_A 0x33
#define MT9V034_V4_CTRL_REG_A 0x34

#define MT9V034_ANALOG_GAIN 0x35
#define MT9V034_ANALOG_GAIN_MIN 16
#define MT9V034_ANALOG_GAIN_DEF 16
#define MT9V034_ANALOG_GAIN_MAX 64

#define MT9V034_MAX_ANALOG_GAIN 0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX 127

#define MT9V034_FRAME_DARK_AVERAGE 0x42
#define MT9V034_DARK_AVG_THRESH 0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT 0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT 8

#define MT9V034_ROW_NOISE_CORR_CONTROL 0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG (1 << 7)

#define MT9V034_PIXEL_CLOCK 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK (1 << 4)

#define MT9V034_TEST_PATTERN 0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT 0
#define MT9V034_TEST_PATTERN_USE_DATA (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP (1 << 14)

#define MT9V034_AEC_AGC_ENABLE 0xAF
#define MT9V034_AEC_ENABLE (1 << 0)
#define MT9V034_AGC_ENABLE (1 << 1)
#define MT9V034_THERMAL_INFO 0xc1
#define MT9V034_ANALOG_CTRL (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE (1 << 7)
#define MT9V034_MAX_GAIN (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A (0xD5)
#define MT9V034_HDR_ENABLE_REG 0x0F
#define MT9V034_ADC_RES_CTRL_REG 0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG 0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG 0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG 0xAB
#define MT9V034_MIN_EXPOSURE_REG 0xAC // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG 0xAD // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG 0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0

#endif
