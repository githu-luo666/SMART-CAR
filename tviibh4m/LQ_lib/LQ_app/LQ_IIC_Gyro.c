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
#include "systick/cy_systick.h"

#define TFT18

#define MPU6050_ADDR  0x68      //IIC写入时的地址字节数据，+1为读取

unsigned char IIC_ICM20602 = 0;
unsigned char IIC_ICM20689 = 0;
unsigned char IIC_MPU9250 = 0;
unsigned char IIC_ICM42605 = 0;
unsigned char IIC_MPU6050 = 0;
unsigned char IIC_errorid = 0;

unsigned char Gyro_ID = 0;

/*************************************************************************
*  函数名称：unsigned char Gyro_Chose(void)
*  功能说明：读取陀螺仪ID区
*  参数说明：无
*  函数返回：读取到的ID
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char Gyro_Chose(void)
{
    Gyro_ID = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
    switch(Gyro_ID)
    {
        case 0x12:IIC_ICM20602 = 1;break;
        case 0x71:IIC_MPU9250 = 1;break;
        case 0x98:IIC_ICM20689 = 1;break;
        case 0x42:IIC_ICM42605 = 1;break;
        case 0x68:IIC_MPU6050 = 1;break;
        default :IIC_errorid = 1;return 0;
    }
    return Gyro_ID;
}


signed short  aacx,aacy,aacz;            //加速度传感器原始数据
signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据
short magx, magy, magz;

/*************************************************************************
*  函数名称：void Test_IIC_Gyro(void)
*  功能说明：读取MPU6050,9250或者ICM20602,20689,42605原始数据 测试
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：IIC通信，42605需要将模块的AD0引脚接地
*************************************************************************/
void Test_IIC_Gyro(void)
{
    unsigned char res;
    char  txt[30];
    
    IIC_Init();                  //IIC初始化//IIC初始化
    Cy_SysTick_DelayInUs(100000);
    res = Gyro_Chose();                //判断陀螺仪型号
    sprintf(txt,"ID:0X%2x",res);
    LCD_ShowString(13,8,txt,WHITE,BLACK,16,0);
    LCD_Init();        //LCD初始化
    LCD_CLS(BLACK);
    
    if(IIC_MPU9250){
        sprintf(txt,"0X%2x",res);
        LCD_ShowString(2,0,"ID",WHITE,BLACK,16,0);
        LCD_ShowString(4,1,txt,WHITE,BLACK,16,0);
    }else
        LCD_ShowString(3,1,txt,WHITE,BLACK,16,0);
    if (IIC_ICM20602)
        LCD_ShowString(10,0,"LQ 20602 Test",WHITE,BLACK,16,0);
    if (IIC_MPU6050)
        LCD_ShowString(10,0,"LQ 6050 Test",WHITE,BLACK,16,0);
    if (IIC_ICM42605)
        LCD_ShowString(10,0,"LQ 42605 Test",WHITE,BLACK,16,0);
    if (IIC_MPU9250)
        LCD_ShowString(10,0,"LQ 9250 Test",WHITE,BLACK,16,0);
    if (IIC_ICM20689)
        LCD_ShowString(10,0,"LQ 20689 Test",WHITE,BLACK,16,0);
    if (IIC_errorid)
    {
        LCD_ShowString(10,0,"Gyro Test fail",WHITE,BLACK,16,0);
        while(1);
    }
    MPU6050_Init();
    while(1)
    {
      MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
//      if (IIC_MPU9250)
//          MPU9250_Get_Magnetometer(&magx, &magy, &magz);   
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

//    if (IIC_MPU9250)
//    {
//      sprintf((char*)txt,"mx:%06d",magx);
//      LCD_ShowString(10,9,"LQ 20602 Test",WHITE,BLACK,16,0);
//      sprintf((char*)txt,"my:%06d",magy);
//      LCD_ShowString(10,10,"LQ 20602 Test",WHITE,BLACK,16,0);
//      sprintf((char*)txt,"mz:%06d",magz);
//      LCD_ShowString(10,11,"LQ 20602 Test",WHITE,BLACK,16,0);
//    }
    
      /*上位机显示，将相关数据发送至匿名地面站，显示波形，视频参考B站龙邱科技视频*/
      //ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,0,0);
      Cy_SysTick_DelayInUs(100000);
  }

}

/*************************************************************************
*  函数名称：void delayms_mpu(uint16_t ms)
*  功能说明：不精确延时
*  参数说明：ms ：延时时间 单位：毫秒
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
void delayms_mpu(uint16_t ms)
{
  while(ms--)
  {
    uint16_t  i = 300;
    while(i--)
    {
      __NOP();
    }
  }
}

/*************************************************************************
*  函数名称：unsigned char MPU6050_Init(void)
*  功能说明：初始化陀螺仪
*  参数说明：无
*  函数返回：0：初始化成功   1：失败
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char MPU6050_Init(void)
{
    int  res;
    if (IIC_ICM42605)
    {
        MPU_Write_Byte(MPU6050_ADDR, device_config_reg,bit_soft_reset_chip_config);//chip soft reset
        Cy_SysTick_DelayInUs(10000);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x01);//Change to bank 1
        MPU_Write_Byte(MPU6050_ADDR,intf_config4,0x03);//4 wire spi mode  四线SPI  默认值0x03
        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
        MPU_Write_Byte(MPU6050_ADDR,fifo_config_reg,0x40);//Stream-to-FIFO Mode  陀螺仪Y轴陷波滤波器的频率选择

        res = MPU_Read_Byte(MPU6050_ADDR,int_source0_reg);
        MPU_Write_Byte(MPU6050_ADDR,int_source0_reg,0x00);
        MPU_Write_Byte(MPU6050_ADDR,fifo_config2_reg,0x00);// watermark
        MPU_Write_Byte(MPU6050_ADDR,fifo_config3_reg,0x02);// watermark
        MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);
        MPU_Write_Byte(MPU6050_ADDR,fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
        MPU_Write_Byte(MPU6050_ADDR,int_config_reg,0x36);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = (MPU_Read_Byte(MPU6050_ADDR,int_source0_reg)|bit_int_fifo_ths_int1_en);
        MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
        MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0xF0)|bit_accel_odr_50hz);
        MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));
        MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg,(unsigned char)res);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);
        MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg, (unsigned char)res);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
        MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg, (unsigned char)res);
        Cy_SysTick_DelayInUs(1000);

        MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
        res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
        MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg,(unsigned char)res);
        Cy_SysTick_DelayInUs(1000);
        return 0;
    }
    else
    {
        res = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);           //读取MPU6050的ID
        if(res != Gyro_ID)                                 //器件ID正确
            return 1;

        res = 0;
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
        delayms_mpu(100);  //延时100ms
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
        res += MPU_Set_Gyro_Fsr(3);					        	   //陀螺仪传感器,±2000dps
        res += MPU_Set_Accel_Fsr(1);					       	   //加速度传感器,±4g
        res += MPU_Set_Rate(1000);						       	   //设置采样率1000Hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //设置数字低通滤波器   98hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

        if(IIC_MPU9250)
        {
            MPU_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);  //关闭FIFO
            MPU_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式
            res = MPU_Read_Byte(AK8963_ADDR,MPU_MAG_WIA);   //读取MPU9250的ID
            if(res == AK8963_ID)                                //器件ID正确
            {
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL2_REG,0X01);//软件重置
                delayms_mpu(100);
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X16);//设置为单次模式，输出16bit
            }
        }
        return 0;
    }
}

/*************************************************************************
*  函数名称：unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
*  功能说明：设置陀螺仪测量范围
*  参数说明：fsr: 0,±250dps;  1,±500dps;  2,±1000dps;  3,±2000dps
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：MPU_Set_Gyro_Fsr(3);		  //陀螺仪传感器,±2000dps
*************************************************************************/
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}

/*************************************************************************
*  函数名称：unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
*  功能说明：设置加速度计测量范围
*  参数说明：fsr:  0,±2g;  1,±4g;  2,±8g;  3,±16g
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：MPU_Set_Accel_Fsr(1);		  //加速度传感器,±4g
*************************************************************************/
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}

/*************************************************************************
*  函数名称：unsigned char MPU_Set_LPF(uint16_t lpf)
*  功能说明：设置数字低通滤波
*  参数说明：lpf:数字低通滤波频率(Hz)
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：MPU_Set_LPF(100);
*************************************************************************/
unsigned char MPU_Set_LPF(uint16_t lpf)
{
	unsigned char  dat=0;
	if(lpf>=188)dat=1;
	else if(lpf>=98)dat=2;
	else if(lpf>=42)dat=3;
	else if(lpf>=20)dat=4;
	else if(lpf>=10)dat=5;
	else dat=6;
	return MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,dat);//设置数字低通滤波器
}

/*************************************************************************
*  函数名称：unsigned char MPU_Set_Rate(uint16_t rate)
*  功能说明：设置采样率
*  参数说明：lrate:4~1000(Hz)
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：MPU_Set_Rate(1000);       	   //设置采样率1000Hz
*************************************************************************/
unsigned char MPU_Set_Rate(uint16_t rate)
{
	unsigned char  dat;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	dat=1000/rate-1;
	MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,dat);	    //设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	                                //自动设置LPF为采样率的一半
}

/*************************************************************************
*  函数名称：short MPU_Get_Temperature(void)
*  功能说明：获取温度值
*  参数说明：无
*  函数返回：温度值(扩大了100倍)
*  修改时间：2023年11月22日
*  备    注：signed short temp = MPU_Get_Temperature();
*************************************************************************/
short MPU_Get_Temperature(void)
{
    unsigned char  buf[2];
    short raw;
	float temp;
	MPU_Read_Len(MPU6050_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=21+((double)raw)/333.87;
    return (short)temp*100;
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
*  功能说明：获取陀螺仪值
*  参数说明：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
*  函数返回：0：读取成功
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char MPU_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU6050_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];
		*gy=((uint16_t)buf[2]<<8)|buf[3];
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	}
    return res;
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
*  功能说明：获取加速度值
*  参数说明：ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char MPU_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
    unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];
		*ay=((uint16_t)buf[2]<<8)|buf[3];
		*az=((uint16_t)buf[4]<<8)|buf[5];
	}
    return res;
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
*  功能说明：获取 加速度值 角速度值
*  参数说明：ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
*            gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
*  函数返回：0：读取成功
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char MPU_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    if(IIC_ICM42605)
    {
        unsigned char  buf[14],res;
        res=MPU_Read_Len(MPU6050_ADDR,0X1F,12,buf);
        if(res==0)
        {
            *ax=((uint16_t)buf[0]<<8)|buf[1];
            *ay=((uint16_t)buf[2]<<8)|buf[3];
            *az=((uint16_t)buf[4]<<8)|buf[5];
            *gx=((uint16_t)buf[6]<<8)|buf[7];
            *gy=((uint16_t)buf[8]<<8)|buf[9];
            *gz=((uint16_t)buf[10]<<8)|buf[11];
        }
        return res;
    }
    else
    {
        unsigned char  buf[14],res;

        res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
        if(res==0)
        {
            *ax=((uint16_t)buf[0]<<8)|buf[1];
            *ay=((uint16_t)buf[2]<<8)|buf[3];
            *az=((uint16_t)buf[4]<<8)|buf[5];
            *gx=((uint16_t)buf[8]<<8)|buf[9];
            *gy=((uint16_t)buf[10]<<8)|buf[11];
            *gz=((uint16_t)buf[12]<<8)|buf[13];
        }
        return res;
    }
}

/*************************************************************************
*  函数名称：sunsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
*  功能说明：获取磁力计值
*  参数说明：mx,my,mz:磁力计仪x,y,z轴的原始读数(带符号)
*  函数返回：0：读取成功
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
unsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
{

  unsigned char  buf[6],res;
  res=MPU_Read_Len(AK8963_ADDR,MPU_MAG_XOUTL_REG,6,buf);
  if(res==0)
  {
    *mx=((uint16_t)buf[1]<<8)|buf[0];
    *my=((uint16_t)buf[3]<<8)|buf[2];
    *mz=((uint16_t)buf[5]<<8)|buf[4];
  }
  MPU_Read_Byte(AK8963_ADDR,MPU_MAG_ST2_REG);
  return res;
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
*  功能说明：IIC 连续读
*  参数说明：addr:器件地址
*            reg :要读取的寄存器地址
*            len :要读取的长度
*            buf :读取到的数据存储区
*  函数返回：0：读取成功
*  修改时间：2023年11月22日
*  备    注：底层驱动 移植时需要修改
*************************************************************************/
unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
*  功能说明：IIC 连续写
*  参数说明：addr:器件地址
*            reg :要写入的寄存器地址
*            len :要写入的长度
*            buf :写入到的数据存储区
*  函数返回：0：写入成功
*  修改时间：2023年11月22日
*  备    注：底层驱动 移植时需要修改
*************************************************************************/
unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
*  功能说明：IIC 写一个寄存器
*  参数说明：addr  :器件地址
*            reg   :寄存器地址
*            value :要写入的值
*  函数返回：0：读取成功
*  修改时间：2023年11月22日
*  备    注：底层驱动 移植时需要修改
*************************************************************************/
unsigned char MPU_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}

/*************************************************************************
*  函数名称：sunsigned char MPU_Read_Byte(unsigned char addr,unsigned char reg)
*  功能说明：IIC 读一个寄存器
*  参数说明：addr  :器件地址
*            reg   :寄存器地址
*            
*  函数返回：读取的值
*  修改时间：2023年11月22日
*  备    注：底层驱动 移植时需要修改
*************************************************************************/
unsigned char MPU_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    MPU_Read_Len(addr, reg, 1, value);
    return value[0];
}


