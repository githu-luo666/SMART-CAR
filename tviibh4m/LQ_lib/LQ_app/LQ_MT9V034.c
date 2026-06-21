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
 uint8_t mt9v034Image[MT9V034_IMAGE_H][MT9V034_IMAGE_W];
volatile uint8_t mt9v034Flag = 0;  
volatile uint32_t mt9v034Cnt = 0;  
int baoguang=360;
#pragma diag_suppress=Pe223 // never used variable
//===================================================DMA配置=======================================================//
#define BUFFER_SIZE         188
#define DW_CHANNEL          7
#define DW DW1     
static  cy_stc_pdma_descr_t         stcDescr;
const   cy_stc_pdma_chnl_config_t   chnlConfig      =   {
                                                        .PDMA_Descriptor=   &stcDescr,
                                                        .preemptable    =   0,
                                                        .priority       =   0,
                                                        .enable         =   1,  
                                                        .bufferable     =   0,
                                                        };
static  cy_stc_pdma_descr_config_t  stcDmaDescrConfig=  {
                                                        .deact          =   0,  
                                                        .intrType       =   CY_PDMA_INTR_DESCR_CMPLT,
                                                        .trigoutType    =   CY_PDMA_TRIGOUT_1ELEMENT_CMPLT,
                                                        .chStateAtCmplt =   CY_PDMA_CH_ENABLED,
                                                        .triginType     =   CY_PDMA_TRIGIN_1ELEMENT,
                                                        .dataSize       =   CY_PDMA_BYTE,
                                                        .srcTxfrSize    =   1,  
                                                        .destTxfrSize   =   0, 
                                                        .descrType      =   CY_PDMA_1D_TRANSFER,
                                                        .srcAddr        =   (uint32_t *)&GPIO_PRT18->unIN.u32Register,//(uint32_t *)&GPIO_PRT18->unIN.u32Register,//au8SrcBuffer,
                                                        .destAddr       =   (void*)mt9v034Image,
                                                        .srcXincr       =   0,
                                                        .destXincr      =   1,
                                                        .xCount         =   BUFFER_SIZE,
                                                        .srcYincr       =   0,
                                                        .destYincr      =   0,
                                                        .yCount         =   0,
                                                        .descrNext      =   &stcDescr
                                                        };
/*************************************************************************
*  函数名称：void camera_handler(void)
*  功能说明：DMA中断
*  参数说明：无
*  函数返回：无
*  修改时间：25年2月24日
*************************************************************************/
void camera_handler(void)
{   
    static uint8_t imgRow = 0;
    // 检查行中断
    if(Cy_GPIO_GetInterruptStatusMasked(GPIO_PRT17, 2)!= 0ul)
    {  
        stcDescr.u32PDMA_DESCR_DST = (uint32_t)(&mt9v034Image[imgRow++][0]);
        DW->CH_STRUCT[DW_CHANNEL].unCH_IDX.u32Register            = 0ul;
        Cy_GPIO_ClearInterrupt(GPIO_PRT17, 2);
        return;
    }
    // 检查帧中断
    if(Cy_GPIO_GetInterruptStatusMasked(GPIO_PRT17, 1)!= 0ul)
    {
        imgRow = 0;
        mt9v034Flag = 1;
        mt9v034Cnt++;         
        Cy_GPIO_ClearInterrupt(GPIO_PRT17, 1);
    }
}
/*************************************************************************
*  函数名称：void camera_init()
*  功能说明：摄像头初始化
*  参数说明：无
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
*************************************************************************/
void camera_init()
{
    // GPIO输入初始化
    LQ_GPIO_Init_Config(P18_0,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_2,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_1,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_3,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_4,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_5,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_6,GPIO_IN_DOWN);
    LQ_GPIO_Init_Config(P18_7,GPIO_IN_DOWN);           
    // 时钟信号GPIO初始化
    cy_stc_gpio_pin_config_t gpio_cfg1 = 
    { 
        .driveMode = CY_GPIO_DM_HIGHZ,
        .hsiom     = P17_3_PERI_TR_IO_INPUT26,
    };
    Cy_GPIO_Pin_Init(GPIO_PRT17, 3, &gpio_cfg1);

    // 连接触发信号
    Cy_TrigMux_Connect(TRIG_IN_MUX_2_HSIOM_IO_INPUT26, TRIG_OUT_MUX_2_PDMA1_TR_IN7, false, TRIGGER_TYPE_EDGE, false);   
    // 关闭cache通道
    SCB_DisableICache();
    SCB_DisableDCache();
    // 配置PDMA
    Cy_PDMA_Disable(DW); 
    Cy_PDMA_Chnl_DeInit(DW, DW_CHANNEL);
    Cy_PDMA_Descr_Init(&stcDescr, &stcDmaDescrConfig);
    Cy_PDMA_Chnl_Init(DW, DW_CHANNEL, &chnlConfig);
    Cy_PDMA_Chnl_SetInterruptMask(DW, DW_CHANNEL);
    Cy_PDMA_Chnl_Enable(DW, DW_CHANNEL);
    Cy_PDMA_Enable(DW);

    // 配置帧中断的GPIO
    cy_stc_gpio_pin_config_t gpio_cfg2= 
    {
        .driveMode = CY_GPIO_DM_HIGHZ,
        .hsiom     = P17_1_GPIO,
        .intEdge   = CY_GPIO_INTR_FALLING,// 下降沿触发中断
        .intMask   = 1ul,
    };
    Cy_GPIO_Pin_Init(GPIO_PRT17, 1, &gpio_cfg2);
    // 配置行中断的GPIO
    cy_stc_gpio_pin_config_t gpio_cfg3= 
    {
        .driveMode = CY_GPIO_DM_HIGHZ,
        .hsiom     = P17_2_GPIO,
        .intEdge   = CY_GPIO_INTR_FALLING,// 下降沿触发中断
        .intMask   = 1ul,
    };
    Cy_GPIO_Pin_Init(GPIO_PRT17, 2, &gpio_cfg3);
    // 配置中断和中断优先级
    cy_stc_sysint_irq_t irq_cfg1 =
    {
        .sysIntSrc  = ioss_interrupts_gpio_dpslp_17_IRQn, 
        .intIdx     = CPUIntIdx1_IRQn,
        .isEnabled  = true,
    };
    Cy_SysInt_InitIRQ(&irq_cfg1);
    NVIC_ClearPendingIRQ(CPUIntIdx1_IRQn);   
    NVIC_SetPriority(CPUIntIdx1_IRQn,0u);
    Cy_SysInt_SetSystemIrqVector(irq_cfg1.sysIntSrc, camera_handler);
    NVIC_EnableIRQ(CPUIntIdx1_IRQn);
}
/*************************************************************************
*  函数名称：int MT9V034_Init(unsigned char fps)
*  功能说明：MT9V034寄存器初始化
*  参数说明：fps 帧率
*  调用方法：MT9V034_Init(50);
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：
*************************************************************************/
int MT9V034_Init(unsigned char fps)
{

    // 初始化摄像头SCCB接口
    LQ_GPIO_Init_Config(MT9V034_SCL_PIN, GPIO_OUT_PP_UP);
    LQ_GPIO_Init_Config(MT9V034_SDA_PIN, GPIO_OUT_PP_UP);
    MT9V034_SCL_High;
    MT9V034_SDA_High;
    // 读取摄像头寄存器
    unsigned short data = 0;
    if (SCCB_RegRead(MT9V034_I2C_ADDR >> 1, MT9V034_CHIP_VERSION, &data) == 0) // 读取摄像头版本寄存器
    {
        if (data != MT9V034_CHIP_ID) // 芯片ID不正确，说明没有正确读取到数据，进入死循环
        {
            while (1);
        }
    }
    else
    {
        while (1);  
    }
    MT9V034_SetFrameResolution(MT9V034_IMAGE_H, MT9V034_IMAGE_W);
    MT9V034_SetFrameRate(fps*2);
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);                         
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303); 
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x13, 0x2D2E);                  
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x20, CAMERA_CONTRAST);     
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x24, 0x0010);                 
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2B, 0x0003);                  
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2F, 0x0003);                   
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_V2_CTRL_REG_A, 0x001A);  
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0103); 
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);
    // 初始化摄像头数据接口
    camera_init();
    return 0;
}
/*************************************************************************
*  函数名称：void Test_MT9V034()
*  功能说明：摄像头测试例程
*  参数说明：void
*  调用方法：void
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：使用FPC座直接插到母板上
*************************************************************************/
void Test_MT9V034()
{     
    char txt[32];
    LCD_Init();        
    LCD_CLS(u16BLACK);
    LCD_ShowStr_16(0, 0, "mt9v034 initializing", u16RED, u16BLACK); 
    //若屏幕一直显示 "mt9v034 initializing" 而不显示图像说明摄像头自检没过+
    MT9V034_Init(100);
    LCD_CLS(u16BLACK);  
    for(;;)
    {
        uint8_t img[60][94];
        if(mt9v034Flag == 1)
        {
            mt9v034Flag = 0;
            for(int y=0;y<60;y++)
            {
                for(int x=0;x<94;x++)
                {
                    img[y][x] = mt9v034Image[y*2][x*2];
                }
            }
            LCDSPI_Road(0, 0, 60, 94, (unsigned char *)&img[0][0]);  // 图像显示
            sprintf(txt, "mt9v034Cnt: %06d", mt9v034Cnt++);
            LCD_ShowStr_16(0, 4, txt, u16RED, u16BLACK);
        }
    }
}
/*************************************************************************
*  函数名称：void MT9V034_SetFrameRate(unsigned char fps)
*  功能说明：设置MT9V034摄像头的帧率
*  参数说明：帧率实际通过接口设置实现，帧率与设置值之间存在一定关系，具体如下：
               帧率        200    150    100    70.9    50    20    10
              设置值       39     92    199    330    515   1450   3000
*  调用方法：
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：
*************************************************************************/
void MT9V034_SetFrameRate(unsigned char fps)
{
    float vertical = 0;
    if (fps > 200)
    {
        vertical = 2;
    }
    else if (fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if (fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if (fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if (fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if (fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else
    {
        vertical = -155 * fps + 4550;
    }
    /* 垂直消隐时间 2 - 32288 */
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (unsigned short)vertical);
}

/*************************************************************************
*  函数名称：static void MT9V034_SetFrameResolution(unsigned short height, unsigned short width)
*  功能说明：设置图像分辨率大小
*  参数说明：height 高  width 宽
*  调用方法：MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW);
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：
*************************************************************************/
static void MT9V034_SetFrameResolution(unsigned short height, unsigned short width)
{
    unsigned short data = 0;

    if ((height * 4) <= MAX_IMAGE_HEIGHT) // 判断是否4倍采样
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if ((height * 2) <= MAX_IMAGE_HEIGHT) //  判断是否2倍采样
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }

    if ((width * 4) <= MAX_IMAGE_WIDTH) // 判断是否4倍采样
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if ((width * 2) <= MAX_IMAGE_WIDTH) // 判断是否2倍采样
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }
    // 水平翻转                     垂直翻转
    data |= (MT9V034_READ_MODE_ROW_FLIP | MT9V034_READ_MODE_COLUMN_FLIP); // 根据需求决定是否翻转
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);                          // 写入寄存器设置采样模式

    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH, width);   // 获取图像宽度  此设置也会影响图像分辨率大小，设置不当可能导致图像失真
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); // 获取图像高度  此设置也会影响图像分辨率大小，设置不当可能导致图像失真

    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN); // 列起始
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);       // 行起始
}

/*************************************************************************
*  函数名称：void MT9V034_SetAutoExposure(char enable)
*  功能说明：设置自动曝光
*  参数说明：enable为1开启自动曝光
*  调用方法：MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：自动曝光时间通过寄存器进行调整
*************************************************************************/
void MT9V034_SetAutoExposure(char enable)
{
    unsigned short reg = 0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, &reg);
    if (enable)
    {
        /* 开启自动曝光和自动增益 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg | MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE);
        /* 最大曝光时间  修改此值会影响图像亮度和曝光效果 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
         /* 最小曝光时间  修改此值会影响图像亮度和曝光效果 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
         /* 最大增益  图像亮度调整参数，取值范围0 - 60 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  设置AEC/AGC直接图像像素数量，22560 或44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);
    }
    else
    {
        /* 关闭自动曝光 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg & ~(MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE));
        /* 0xAB  设置固定增益值     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);
        /* 0x0B 设置总快门宽度，即曝光时间 0 - 32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_TOTAL_SHUTTER_WIDTH, CAMERA_EXPOSURE_TIME);
    }
}

/*************************************************************************
*  函数名称：void SCCB_Init(void)
*  功能说明：SCCB初始化
*  参数说明：无
*  调用方法：SCCB_Init();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在程序开始时进行初始化
*************************************************************************/
void SCCB_Init(void)
{
    LQ_GPIO_Init_Config(MT9V034_SCL_PIN, GPIO_OUT_PP_UP);
    LQ_GPIO_Init_Config(MT9V034_SDA_PIN, GPIO_OUT_PP_UP);
    MT9V034_SCL_High;
    MT9V034_SDA_High;
}
/*************************************************************************
*  函数名称：void MTV_IICWriteReg16(unsigned char reg, unsigned short val)
*  功能说明：SCCB写16位寄存器
*  参数说明：reg 为寄存器地址  val 为写入的值
*  调用方法：MTV_IICWriteReg16(0x20, 0x0003); //向MT9V034的0x20寄存器写入0x0003
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：
*************************************************************************/
void MTV_IICWriteReg16(unsigned char reg, unsigned short val)
{
    SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}

/*************************************************************************
*  函数名称：void SCCB_Wait(void)
*  功能说明：SCCB延时
*  参数说明：无
*  调用方法：SCCB_Wait();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在相关操作中使用
*************************************************************************/
void SCCB_Wait(void)
{
    int i;
    for (i = 0; i < 500; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*************************************************************************
*  函数名称：void SCCB_Star(void)
*  功能说明：SCCB开始信号
*  参数说明：无
*  调用方法：SCCB_Star();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在通信开始时使用
*************************************************************************/
void SCCB_Star(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    SCCB_Wait();
    MT9V034_SDA_High;
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}

/*************************************************************************
*  函数名称：void SCCB_Stop(void)
*  功能说明：SCCB结束信号
*  参数说明：无
*  调用方法：SCCB_Stop();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在通信结束时使用
*************************************************************************/
void SCCB_Stop(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    SCCB_Wait();
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SDA_High;
    SCCB_Wait();
}

/*************************************************************************
*  函数名称：unsigned char SCCB_SendByte(unsigned char Data)
*  功能说明：SCCB发送一个字节
*  参数说明：Data为要发送的字节
*  调用方法：SCCB_SendByte(0x01)
*  函数返回：ack应答值
*  修改时间：25年2月24日
*  备    注：在数据传输中使用
*************************************************************************/
unsigned char SCCB_SendByte(unsigned char Data)
{
    unsigned char i;
    unsigned char Ack;
    MT9V034_SDA_Out;
    for (i = 0; i < 8; i++)
    {
        if (Data & 0x80)
        {
            MT9V034_SDA_High;
        }
        else
        {
            MT9V034_SDA_Low;
        }
        Data <<= 1;
        SCCB_Wait();
        MT9V034_SCL_High;
        SCCB_Wait();
        MT9V034_SCL_Low;
        SCCB_Wait();
    }
    MT9V034_SDA_High;
    MT9V034_SDA_In;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    Ack = MT9V034_SDA_Data;
    MT9V034_SCL_Low;
    SCCB_Wait();
    return Ack;
}

/*************************************************************************
*  函数名称：unsigned char SCCB_ReadByte(void)
*  功能说明：SCCB读取一个字节
*  参数说明：无
*  调用方法：unsigned char data = SCCB_ReadByte();
*  函数返回：接收到的字节
*  修改时间：25年2月24日
*  备    注：在数据读取中使用
*************************************************************************/
unsigned char SCCB_ReadByte(void)
{
    unsigned char i;
    volatile unsigned char byte;
    MT9V034_SCL_Out;
    MT9V034_SDA_In; // 设置为输入
    for (i = 0; i < 8; i++)
    {
        MT9V034_SCL_Low;
        SCCB_Wait();
        MT9V034_SCL_High;
        SCCB_Wait();
        byte = (byte << 1) | (MT9V034_SDA_Data & 1);
    }
    MT9V034_SCL_Low;
    MT9V034_SDA_Out;
    SCCB_Wait();
    return byte;
}

/*************************************************************************
*  函数名称：static void SCCB_Ack(void)
*  功能说明：SCCB应答
*  参数说明：无
*  调用方法：SCCB_Ack();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在通信应答中使用
*************************************************************************/
static void SCCB_Ack(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    MT9V034_SCL_Low;
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}
/*************************************************************************
*  函数名称：static void SCCB_NAck(void)
*  功能说明：SCCB非应答
*  参数说明：无
*  调用方法：SCCB_NAck();
*  函数返回：无
*  修改时间：25年2月24日
*  备    注：在通信非应答中使用
*************************************************************************/
static void SCCB_NAck(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    MT9V034_SCL_Low;
    SCCB_Wait();
    MT9V034_SDA_High;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}

/*************************************************************************
*  函数名称：void SCCB_RegWrite(unsigned char Device, unsigned char reg, unsigned short Data)
*  功能说明：SCCB写寄存器
*  参数说明：Device: 设备地址  reg: 寄存器地址  Data: 写入的数据
*  调用方法：SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);
*  函数返回：ack应答值
*  修改时间：25年2月24日
*  备    注：在寄存器写入操作中使用
*************************************************************************/
void SCCB_RegWrite(unsigned char Device, unsigned char reg, unsigned short Data)
{
    unsigned char i;
    unsigned char Ack;

    for (i = 0; i < 3; i++)
    {
        SCCB_Star();
        Ack = SCCB_SendByte(Device);
        if (Ack == 1)
        {
            continue;
        }
        Ack = SCCB_SendByte(reg);
        if (Ack == 1)
        {
            continue;
        }
        SCCB_SendByte((unsigned char)(Data >> 8));
        Ack = SCCB_SendByte((unsigned char)Data);
        if (Ack == 1)
        {
            continue;
        }
        SCCB_Stop();
        if (Ack == 0)
            break;
    }
}

/*************************************************************************
*  函数名称：unsigned char SCCB_RegRead(unsigned char Device, unsigned char reg, unsigned short *Data)
*  功能说明：SCCB读寄存器
*  参数说明：Device: 设备地址  reg: 寄存器地址  Data: 读取的数据存储地址
*  调用方法：SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data);
*  函数返回：ack应答值
*  修改时间：25年2月24日
*  备    注：在寄存器读取操作中使用
*************************************************************************/
unsigned char SCCB_RegRead(unsigned char Device, unsigned char reg, unsigned short *Data)
{
    unsigned char Ack = 0;
    Device = Device << 1;
    SCCB_Star();
    Ack += SCCB_SendByte(Device);
    Ack += SCCB_SendByte(reg);
    SCCB_Star();
    Ack += SCCB_SendByte(Device + 1);
    *Data = SCCB_ReadByte();
    SCCB_Ack();
    *Data = *Data << 8;
    *Data += SCCB_ReadByte();
    SCCB_NAck();
    SCCB_Stop();
    return Ack;
}


