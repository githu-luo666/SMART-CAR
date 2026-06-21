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
#define GYRO_CS_PIN     P20_13
#define GYRO_CS_H       PIN_Write(GYRO_CS_PIN,1)
#define GYRO_CS_L       PIN_Write(GYRO_CS_PIN,0)

// SPI陀螺仪
enum//SPI通信陀螺仪类型
{
    error,
    SPI_ICM20602,
    SPI_ICM20689,
    SPI_MPU9250,
    SPI_ICM42605,
    SPI_ICM42688,
    SPI_LSM6DSR,
    SPI_LQ9AGMV
}GyroType;

/*************************************************************************
*  函数名称：void Test_SPI_Gyro(void)
*  功能说明：测试SPI陀螺仪
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：测试SPI通信陀螺仪，ICM20602、20689、42605、42688、LSM6DSR、LQ9AGM
*************************************************************************/
void Test_SPI_Gyro(void)
{
    char txt[30];
    short aacx, aacy, aacz;    // 定义加速度计传感器原始数据
    short gyrox, gyroy, gyroz; // 定义陀螺仪传感器原始数据
    unsigned char res;
    // LCD初始化
    LCD_Init();
    LCD_CLS(u16BLACK);
    // SPI初始化
    Soft_SPI_Init();
    // 读陀螺仪ID
    res = SPI_Gryo_Init();
    
    if(res==0x12)
    {
        sprintf(txt,"TEST_20602 ID:%2x",res);
        LCD_ShowStr_16(0, 0, txt, u16WHITE, u16BLACK);
    }
    if(res==0x98)
    {
        sprintf(txt,"TEST_20689 ID:%2x",res);
        LCD_ShowStr_16(0, 0, txt, u16WHITE, u16BLACK);
    }
    if(res==0x42)
    {
        sprintf(txt,"TEST_42605 ID:%2x",res);
        LCD_ShowStr_16(0, 0, txt, u16WHITE, u16BLACK);
    }
    if(res==0x47)
    {
        sprintf(txt,"TEST_42688 ID:%2x",res);
        LCD_ShowStr_16(0, 0, txt, u16WHITE, u16BLACK);
    }
    while (1)
    {
      ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
      
      sprintf((char*)txt,"ax:%06d",aacx);
      LCD_ShowString(10,16,txt,WHITE,BLACK,16,0);
      sprintf((char*)txt,"ay:%06d",aacy);
      LCD_ShowString(10,32,txt,WHITE,BLACK,16,0);
      sprintf((char*)txt,"az:%06d",aacz);
      LCD_ShowString(10,48,txt,WHITE,BLACK,16,0);
      sprintf((char*)txt,"gx:%06d",gyrox);
      LCD_ShowString(10,64,txt,WHITE,BLACK,16,0);
      sprintf((char*)txt,"gy:%06d",gyroy);
      LCD_ShowString(10,80,txt,WHITE,BLACK,16,0);
      sprintf((char*)txt,"gz:%06d",gyroz);
      LCD_ShowString(10,96,txt,WHITE,BLACK,16,0);
    }
}


/*************************************************************************
*  函数名称：unsigned char SPI_Gryo_Init(void)
*  功能说明：陀螺仪Gryo初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char SPI_Gryo_Init(void)
{
    unsigned char res,add;
    // 读取ICM的ID
    res = ICM_Read_Byte(WHO_AM_I); 
    add = res;
    switch(res)
    {
        case 0x12:
            GyroType = SPI_ICM20602;
            break;
        case 0x71:
            GyroType = SPI_MPU9250;
            break;
        case 0x98:
            GyroType = SPI_ICM20689;
            break;
        case 0x42://42
            GyroType = SPI_ICM42605;
            break;
        case 0x47:
            GyroType = SPI_ICM42688;
            break;
        default :   
            // 读取LSM6DSR_LQ9AGM的ID
            res = Gyro_LSM6DSR_LQ9AGMV_Check();
            add = res;
            if(res == DRV_ID_LSM6D) GyroType = SPI_LSM6DSR;
            else if(res == Mag_ID) GyroType = SPI_LQ9AGMV;
            else GyroType = error;   
            break;
    }

    if(GyroType == SPI_ICM42605 || GyroType == SPI_ICM42688)
    {
        
        ICM_Write_Byte(reg_bank_sel,0x00);//Set to bank 0
        ICM_Write_Byte(reg_bank_sel,0x00);//Set to bank 0   
        ICM_Write_Byte(device_config_reg,bit_soft_reset_chip_config);
        delay_ms(100);
        ICM_Write_Byte(reg_bank_sel,0x01);//Change to bank 1
        ICM_Write_Byte(intf_config4,0x02);//4 wire spi mode
        ICM_Write_Byte(reg_bank_sel,0x00);
        ICM_Write_Byte(fifo_config_reg,0x40);//Stream-to-FIFO Mode

        res = ICM_Read_Byte(int_source0_reg);
        ICM_Write_Byte(int_source0_reg,0x00);
        ICM_Write_Byte(fifo_config2_reg,0x00);// watermark
        ICM_Write_Byte(fifo_config3_reg,0x02);// watermark
        ICM_Write_Byte(int_source0_reg, res);
        ICM_Write_Byte(fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO

        ICM_Write_Byte(reg_bank_sel,0x00);
        ICM_Write_Byte(int_config_reg,0x36);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = (ICM_Read_Byte(int_source0_reg)|bit_int_fifo_ths_int1_en);
        ICM_Write_Byte(int_source0_reg, res);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ((ICM_Read_Byte(accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
        ICM_Write_Byte(accel_config0_reg, res);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ((ICM_Read_Byte(accel_config0_reg)&0xF0)|bit_accel_odr_100hz);
        ICM_Write_Byte(accel_config0_reg, res);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ((ICM_Read_Byte(gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));//1000
        ICM_Write_Byte(gyro_config0_reg,res);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ((ICM_Read_Byte(gyro_config0_reg)&0xF0)|bit_gyro_odr_100hz);
        ICM_Write_Byte(gyro_config0_reg, res);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ICM_Read_Byte(pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
        ICM_Write_Byte(pwr_mgmt0_reg, res);
        delay_ms(1);

        ICM_Write_Byte(reg_bank_sel, 0x00);
        res = ICM_Read_Byte(pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
        ICM_Write_Byte(pwr_mgmt0_reg, res);
        delay_ms(1);


    }
    else if((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        ICM_Write_Byte(LSM6DSR_CTRL1_XL,0X20);      //加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
        ICM_Write_Byte(LSM6DSR_CTRL9_XL,0X38);      //使能加速度计x,y,z轴
        ICM_Write_Byte(LSM6DSR_CTRL6_C,0X40|0x10);  //陀螺仪电平触发，加速度计高性能使能
        ICM_Write_Byte(LSM6DSR_CTRL7_G,0X80);       //陀螺仪高性能使能
        ICM_Write_Byte(LSM6DSR_INT2_CTRL,0X03);     //加速度计INT2引脚失能,陀螺仪数据INT2使能
        ICM_Write_Byte(LSM6DSR_CTRL2_G,0X1C);       //陀螺仪12.5hz  2000dps
        ICM_Write_Byte(LSM6DSR_CTRL10_C,0X38);      //使能陀螺仪x,y,z轴
        if(GyroType == SPI_LQ9AGMV)
        {
            ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x40);
            ICM_Write_Byte(0x14, 0x80);
            delay_ms(1);
            ICM_Write_Byte(0x14, 0x00);
            delay_ms(1);
            SPI_Mag_Write_Reg(0x21,0x04);   //陀螺仪复位
            delay_ms(5);
            SPI_Mag_Write_Reg(0x21,0x00);
            delay_ms(5);
            SPI_Mag_Write_Reg(0x20,0x7f);
            SPI_Mag_Write_Reg(0x23,0x0c);
            SPI_Mag_Write_Reg(0x22,0x00);
            SPI_Mag_Write_Reg(0x24,0x00);
            SPI_Mag_Write_Reg(0x30,0x02);
            ICM_Write_Byte(0x17, 0x06);
            ICM_Write_Byte(0x15, 0x39);
            ICM_Write_Byte(0x16, 0x28);
            ICM_Write_Byte(0x14, 0x4c);
            while((ICM_Read_Byte(0x22) & 0x01) == 0);
            ICM_Write_Byte(0x14, 0x6c);
            ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x00);
        }


    }

    else
    {
        res = 0;
        res += ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X80); // 复位
        delay_ms(100);
        res += ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X00); // 唤醒ICM20602
        res += ICM_Set_Gyro_Fsr(3);                     // 陀螺仪传感器，±2000dps
        res += ICM_Set_Accel_Fsr(1);                    // 加速度传感器,±4g
        res += ICM_Set_Rate(1000);                      // 设置采样率1000Hz
        res += ICM_Write_Byte(ICM_CFG_REG, 0x02);       // 设置数字低通滤波器   98hz
        res += ICM_Write_Byte(ICM_INT_EN_REG, 0X00);    // 关闭所有中断
        res += ICM_Write_Byte(ICM_USER_CTRL_REG, 0X00); // I2C主模式关闭
        res += ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
        res += ICM_Write_Byte(ICM_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
    }
    delay_ms(50);
    return add;
}

/*************************************************************************
*  函数名称：unsigned char ICM_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf)
*  功能说明：从 ICM 设备指定寄存器开始读取指定长度的数据，并将数据存于指定缓冲区。
*  参数说明：reg：起始存储设备读取数据的寄存器地址
             len: 要读取的数据长度
             buf：存储读取数据的缓冲区指针
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf)
{
    // ���Ϊ����������������ʼ�Ĵ���
    buf[0] = reg | 0x80;
    // ִ�� SPI ��д����
    Soft_SPI_ReadWriteNbyte(buf,len+1);
    return buf[0];
}

/*************************************************************************
*  函数名称：unsigned char ICM_Write_Byte(unsigned char reg, unsigned char value)
*  功能说明：向存储设备写入数据
*  参数说明：reg：寄存器地址，用于指定要写入数据的位置，类型为无符号8位整数
             value: 要写入寄存器的数据
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Write_Byte(unsigned char reg, unsigned char value)
{
    unsigned char buf[2];
    buf[0] = reg;                                    
    buf[1] = value;                                 
    Soft_SPI_ReadWriteNbyte(buf,2);
    return buf[1];
}
/*************************************************************************
*  函数名称：unsigned char ICM_Read_Byte(unsigned char reg)
*  功能说明：向存储设备读取数据
*  参数说明：reg：寄存器地址，用于指定要读取数据的位置，类型为无符号8位整数
             value: 读取寄存器的数据
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Read_Byte(unsigned char reg)
{
    unsigned char buf[2]={0x00,0x00};
    buf[0] = reg | 0x80;
    Soft_SPI_ReadWriteNbyte(buf,2);
    return buf[1];
}
/*************************************************************************
*  函数名称：uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t data)
*  功能说明：通过SPI向磁力计设备的指定寄存器写入数据
*  参数说明：addr 要写入数据的目标寄存器地址
             data 要写入到目标寄存器的数据
*  函数返回：0表示操作成功，1表示操作超时
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t data)
{
    uint32_t count = 0;
    ICM_Write_Byte(0x17, 0x00);
    ICM_Write_Byte(0x15, 0x38);
    ICM_Write_Byte(0x16, addr);
    ICM_Write_Byte(0x21, data);
    ICM_Write_Byte(0x14, 0x4c);
    while((ICM_Read_Byte(0x22) & 0x80) == 0)
    {
        if(1000 < count++){
            return 1;
        }
        delay_us(100);
    }
    return 0;
}
/*************************************************************************
*  函数名称：uint8_t SPI_Mag_Read_Reg(uint8_t addr)
*  功能说明：通过SPI从哪个磁力计设备的指定寄存器读取数据
*  参数说明：addr 要读取数据的目标寄存器地址
*  函数返回：从寄存器读取到的数据
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
uint8_t SPI_Mag_Read_Reg(uint8_t addr)
{
    uint16_t count = 0;
    // 配置设备的控制寄存器 0x17，写入 0x01
    ICM_Write_Byte(0x17, 0x01);
    //  配置设备的控制寄存器 0x15，写入 0x39
    ICM_Write_Byte(0x15, 0x39);
    // 将要读取数据的目标寄存器地址写入 0x16 寄存器
    ICM_Write_Byte(0x16, addr);
    // 触发读取操作，向 0x14 寄存器写入 0x4c
    ICM_Write_Byte(0x14, 0x4c);
    // 等待操作完成，检查状态寄存器（地址 0x22）的最低位（第 0 位）
    while((ICM_Read_Byte(0x22) & 0x01) == 0)
    {
        if(1000 < count++){
            break;
        }
        delay_us(100);
    }
    // 每次等待 100 微秒
    return ICM_Read_Byte(0x02);
}
/*************************************************************************
*  函数名称：uint8_t Gyro_LSM6DSR_LQ9AGMV_Check(void)
*  功能说明：检查 LSM6DSR 陀螺仪和 LQ9AGMV 磁力计设备
*  参数说明：无
*  函数返回：读取磁力计 ID
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
uint8_t Gyro_LSM6DSR_LQ9AGMV_Check(void)
{
    uint8_t res;
    // 初始化功能配置寄存器，将其值设为 0x00
    ICM_Write_Byte(LSM6DSR_FUNC_CFG,0x00); 
    // 向 LSM6DSR 的 CTRL3_C 寄存器写入 0x05
    ICM_Write_Byte(LSM6DSR_CTRL3_C, 0x05);
    delay_ms(2);
    ICM_Write_Byte(LSM6DSR_FUNC_CFG,0x00);
    // ��ȡ LSM6DSR �� WHO_AM_I �Ĵ���
    res = ICM_Read_Byte(WHO_AM_LSM6D);
    if(res == DRV_ID_LSM6D){
        for(int i = 0;i <= 3; i++)
        {
            // 向 0x14 寄存器写入 0x80
            ICM_Write_Byte(0x14, 0x80);
            delay_ms(5);
            // 向 0x14 寄存器写入 0x00
            ICM_Write_Byte(0x14, 0x00);
            delay_ms(5);
            // 向 LSM6DSR 的 CTRL1_XL 寄存器写入 0x20
            ICM_Write_Byte(LSM6DSR_CTRL1_XL,0X20);
            ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x40);
            // 读取 0x0f 寄存器的值，判断是否为磁力计的 ID
            if(SPI_Mag_Read_Reg(0x0f) == Mag_ID){
                res = Mag_ID;
                break;
            }
        }
        ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x00);
    }

    return res;
}

/*************************************************************************
*  函数名称：void Gyro_Get_Mag_data(short *magx,short *magy,short *magz)
*  功能说明：读取九轴陀螺仪的地磁数据
*  参数说明：magx X轴地磁数据；magy Y轴地磁数据；magz Z轴地磁数据
*  函数返回：X、Y、Z轴数据
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void Gyro_Get_Mag_data(short *magx,short *magy,short *magz)
{
    unsigned char buf[6];
    if(GyroType == SPI_LQ9AGMV){
        ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x40);
        ICM_Read_Len(0x02,6,buf);
        *magx=(short)(((uint16_t)buf[1]<<8)|buf[0]);
        *magy=(short)(((uint16_t)buf[3]<<8)|buf[2]);
        *magz=(short)(((uint16_t)buf[5]<<8)|buf[4]);
        ICM_Write_Byte(LSM6DSR_FUNC_CFG, 0x00);
    }
}


/*************************************************************************
*  函数名称：unsigned char ICM_Set_Gyro_Fsr(unsigned char fsr)
*  功能说明：设置陀螺仪满量程范围
*  参数说明：fsr 满量程取值
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Set_Gyro_Fsr(unsigned char fsr)
{
    
    return ICM_Write_Byte(ICM_GYRO_CFG_REG, fsr << 3);
}



/*************************************************************************
*  函数名称：unsigned char ICM_Set_Accel_Fsr(unsigned char fsr)
*  功能说明：设置陀螺仪加速度计满量程范围
*  参数说明：fsr 满量程取值
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Set_Accel_Fsr(unsigned char fsr)
{
    return ICM_Write_Byte(ICM_ACCEL_CFG_REG, fsr << 3);
}


/*************************************************************************
*  函数名称：unsigned char ICM_Set_LPF(uint16_t lpf)
*  功能说明：设置低通滤波器
*  参数说明：lpf 低通滤波器的频率 单位为hz
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Set_LPF(uint16_t lpf)
{
    unsigned char data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return ICM_Write_Byte(ICM_CFG_REG, data); 
}


/*************************************************************************
*  函数名称：unsigned char ICM_Set_Rate(uint16_t rate)
*  功能说明：设置采样频率
*  参数说明：rate 采样频率 范围为4hz~1000hz
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Set_Rate(uint16_t rate)
{
    unsigned char data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    ICM_Write_Byte(ICM_SAMPLE_RATE_REG, data); 
    return ICM_Set_LPF(rate / 2);              
}

/*************************************************************************
*  函数名称：short ICM_Get_Temperature(void)
*  功能说明：读取温度数据并放大100倍
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
short ICM_Get_Temperature(void)
{
    unsigned char buf[3];
    short raw;
    float temp;
    ICM_Read_Len(ICM_TEMP_OUTH_REG, 2, buf);
    raw = ((uint16_t)buf[1] << 8) | buf[2];
    temp = 21 + ((double)raw) / 333.87;
    return (short)temp * 100;
}


/*************************************************************************
*  函数名称：unsigned char ICM_Get_Gyroscope(short *gx, short *gy, short *gz)
*  功能说明：读取陀螺仪gyro三轴值
*  参数说明：gx、gy、gz 对应X Y Z轴
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    unsigned char buf[7], res;
    res = ICM_Read_Len(ICM_GYRO_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *gx = ((uint16_t)buf[1] << 8) | buf[2];
        *gy = ((uint16_t)buf[3] << 8) | buf[4];
        *gz = ((uint16_t)buf[5] << 8) | buf[6];
    }
    return res;
}


/*************************************************************************
*  函数名称：unsigned char ICM_Get_Accelerometer(short *ax, short *ay, short *az)
*  功能说明：读取陀螺仪Accel三轴值
*  参数说明： ax、ay、az 对应加速度计X Y Z轴
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char ICM_Get_Accelerometer(short *ax, short *ay, short *az)
{
    unsigned char buf[7], res;
    res = ICM_Read_Len(ICM_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((uint16_t)buf[1] << 8) | buf[2];
        *ay = ((uint16_t)buf[3] << 8) | buf[4];
        *az = ((uint16_t)buf[5] << 8) | buf[6];
    }
    return res;
}

/*************************************************************************
*  函数名称：unsigned char ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
*  功能说明：读取三轴加速度计和三轴陀螺仪值
*  参数说明：gx、gy、gz 对应陀螺仪X Y Z轴  ax、ay、az 对应加速度计X Y Z轴
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
float imu660ra_transition_factor[2] = {16384, 32.8};                             // 转换实际值的比例
unsigned char ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
{
    unsigned char buf[15], res;
    if(GyroType == SPI_ICM42605|| GyroType == SPI_ICM42688)
    {
        res = ICM_Read_Len(ICM_ACCEL_DATA_X1,12,buf);
        *ax=((uint16_t)buf[1]<<8)|buf[2];
        *ay=((uint16_t)buf[3]<<8)|buf[4];
        *az=((uint16_t)buf[5]<<8)|buf[6];
        *gx=((uint16_t)buf[7]<<8)|buf[8];
        *gy=((uint16_t)buf[9]<<8)|buf[10];
        *gz=((uint16_t)buf[11]<<8)|buf[12];
    }
    else if((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        res = ICM_Read_Len(LSM6DSR_OUTX_L_GYRO,12,buf);
        *gx=(uint16_t)(((uint16_t)buf[1]<<8)|buf[0]);
        *gy=(uint16_t)(((uint16_t)buf[3]<<8)|buf[2]);
        *gz=(uint16_t)(((uint16_t)buf[5]<<8)|buf[4]);
        *ax=(uint16_t)(((uint16_t)buf[7]<<8)|buf[6]);
        *ay=(uint16_t)(((uint16_t)buf[9]<<8)|buf[8]);
        *az=(short)(((uint16_t)buf[11]<<8)|buf[10]);
    }
    else
    {
        res = ICM_Read_Len(ICM_ACCEL_XOUTH_REG, 14, buf);
        *ax = ((uint16_t)buf[1] << 8) | buf[2];
        *ay = ((uint16_t)buf[3] << 8) | buf[4];
        *az = ((uint16_t)buf[5] << 8) | buf[6];
        *gx = ((uint16_t)buf[9] << 8) | buf[10];
        *gy = ((uint16_t)buf[11] << 8) | buf[12];
        *gz = ((uint16_t)buf[13] << 8) | buf[14];
    }

    return res;
}