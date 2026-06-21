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
#include "ipc/cy_ipc_pipe.h"
#include "cachel1_armv7.h"

/*
自定义一个结构体，结构体的第一元素一定是信道ID，用于标识数据来源
数据量可以自行添加  
*/
typedef struct
{
    uint32_t clientId;  
    uint32_t data1;     
    /*......用户可以继续添加数据*/
} custom_data_t;

/*定义一个结构体，用于M0核发送数据使用，
第一元素为发送的信道ID，剩下的元素用于存放数据
*/
custom_data_t pipeData_M0 = 
{
    .clientId = clientId_M0,
    .data1  = 0x02u
};

bool M0_released;  //传输完成标志位（自定义的）

/*M0的回调函数 当另一个端点释放与该端点对应的句柄时调用
（例如：M0->M70，当M70退出接收回调函数时，M0会执行这个回调函数）
 */
void M0_Callback(void)
{
    /* 通知核心已经得到了数据。*/
    /* 为下一次传输更新发送数据。 */
    M0_released = true;
}

/*************************************************************************
*  函数名称：void Test_ipc_M0(void)
*  功能说明：测试多核通信，将M0核的数据发送至M70核
*  参数说明：无
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*       此函数为发送数据，需要在M70核开始接收数据的测试函数，屏幕显示程序在M70核中
  当发送完成后LED0（核心板绿灯）会翻转，当M70接收到数据后会判断奇偶，
  偶数LED1（蓝灯）亮起，奇数熄灭。
*************************************************************************/
void Test_ipc_M0(void)
{
  //传输状态
  cy_en_ipc_pipe_status_t status;

  /*IPC配置*/
  cy_stc_ipc_pipe_config_t pipeConfig = 
  {
      .epIndexForThisCpu = CPU_M0,
      .epConfigData      = CY_IPC_PIPE_ENDPOINTS_DEFAULT_CONFIG,
  };  
  
  GPIO_LED_Init();
  /* Initialize PIPE */
  Cy_IPC_Pipe_Init(&pipeConfig);

  /* Already registered the handler to the system interrup structure.
   * So just enable corresponding IRQ channel.
   */
  NVIC_ClearPendingIRQ(pipeConfig.epConfigData[CPU_M0].ipcCpuIntIdx);
  NVIC_EnableIRQ(pipeConfig.epConfigData[CPU_M0].ipcCpuIntIdx);
 
    
  while(1)
  {
    /* Send data */
    M0_released = false;
    status = Cy_IPC_Pipe_SendMessage
    (
        CPU_M7_0,
        &pipeData_M0,
        M0_Callback
    );
    
    /* When the data successfully into the pipe */
    if (status == CY_IPC_PIPE_SUCCESS)
    {
        while(!M0_released)
        {
            /* wait until the data has been received by the other core */
        }
        /* Update the data to be sent */
        pipeData_M0.data1 = (pipeData_M0.data1 + 1) % 6;
        // Wait 0.5 [s]
        Cy_SysTick_DelayInUs(500000);
    }
    LED_Ctrl(LED0, RVS);
  }
}

/****************************M7_0*****************************************************************************************/
/****************************M7_0*****************************************************************************************/
/****************************M7_0*****************************************************************************************/
/*定义接收结构体，在不同回调函数中调用*/
custom_data_t pipeData_M0_M70;
custom_data_t pipeData_M71_M70;

/* 当M0发送数据到M70时调用此回调函数 */
void M0_To_M70_Callback(uint32_t *msgPtr)
{
    /* New data comes */
    custom_data_t * data = (custom_data_t *)msgPtr;
    pipeData_M0_M70.data1 = data->data1;
}
/* 当M71发送数据到M70时调用此回调函数 */
void M71_To_M70_Callback(uint32_t *msgPtr)
{
    /* New data comes */
    custom_data_t * data = (custom_data_t *)msgPtr;
    pipeData_M71_M70.data1 = data->data1;
}
/*************************************************************************
*  函数名称：void Test_ipc_M70(void)
*  功能说明：测试多核通信，接收M0或M71的数据或同时接收两个核的数据
*  参数说明：无
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*       此函数为接收数据，需要在M0核 或 M71核 或 同时 开启相应测试函数，屏幕显示程序在M70核中
  当发送完成后LED（M0核为绿灯，M71核为红灯）会翻转，当M70接收到M0的数据后会判断奇偶，
  偶数LED1（蓝灯）亮起，奇数熄灭。
*************************************************************************/
void Test_ipc_M70(void)
{
  char txt[32];
  
  SCB_DisableICache(); // Disables and invalidates instruction cache
  SCB_DisableDCache(); // Disables, cleans and invalidates data cache
    
  cy_stc_ipc_pipe_config_t pipeConfig = 
  {
      .epIndexForThisCpu = CPU_M7_0,
      .epConfigData      = CY_IPC_PIPE_ENDPOINTS_DEFAULT_CONFIG,
  };  
  
  TFTSPI_Init(3);        //LCD初始化  0:横屏  1：竖屏，2，3
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
  Cy_SysTick_DelayInUs(10000);         //延时等待
  
  /* 初始化 PIPE */
  Cy_IPC_Pipe_Init(&pipeConfig);


  /* 注册接收数据回调 */
  Cy_IPC_Pipe_RegisterCallback(M71_To_M70_Callback,clientId_M7_1);/* 实参：回调函数名 接收的信道ID */

  /* 注册接收数据回调 */
  Cy_IPC_Pipe_RegisterCallback(M0_To_M70_Callback,clientId_M0);/* 实参：回调函数名 接收的信道ID */

  /* Already registered the handler to the system interrup structure.
   * So just enable corresponding IRQ channel.
   */
  NVIC_ClearPendingIRQ(pipeConfig.epConfigData[CPU_M7_0].ipcCpuIntIdx);
  NVIC_EnableIRQ(pipeConfig.epConfigData[CPU_M7_0].ipcCpuIntIdx);
     
  while(1)
  {
    /*判断接收来自M0核的数据的奇偶性*/
    if(pipeData_M0_M70.data1 % 2 == 0)
    {
      sprintf(txt, "M0_M70 :%03d LED1:ON ", pipeData_M0_M70.data1);        //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
      LED_Ctrl(LED1, ON);
    }else
    {
      sprintf(txt, "M0_M70 :%03d LED1:OFF", pipeData_M0_M70.data1);        //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
      LED_Ctrl(LED1, OFF);
    }
    TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE);                //将txt中 内容显示出来
    /*显示M71的数据*/
    sprintf(txt, "M71_M70:%03d ", pipeData_M71_M70.data1);      //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
    TFTSPI_P8X16Str(0, 7, txt, u16RED, u16BLUE);                //将txt中 内容显示出来
    
  }
}

/****************************M7_1*****************************************************************************************/
/****************************M7_1*****************************************************************************************/
/****************************M7_1*****************************************************************************************/

/*定义用于发送数据的结构体*/
custom_data_t pipeData_M71 = 
{
    .clientId = clientId_M7_1,
    .data1  = 0x09u
};

bool M71_released;  //传输完成标志位

/* M71的回调，当另一个端点释放与该端点对应的句柄时调用 */
void M71_Callback(void)
{
    /* 通知核心已经得到了数据。*/
    /* 为下一次传输更新发送数据。 */
    M71_released = true;
}
/*************************************************************************
*  函数名称：void Test_ipc_M71(void)
*  功能说明：测试多核通信，将M71核的数据发送至M70核
*  参数说明：无
*  函数返回：无
*  修改时间：2023年12月5日
*  备    注：
*       此函数为发送数据，需要在M70核开始接收数据的测试函数，屏幕显示程序在M70核中
  当发送完成后LED2（核心板红灯）会翻转，当M70接收到数据后会显示在TFT1.8屏幕中
*************************************************************************/
void Test_ipc_M71(void)
{
  
  SCB_DisableICache(); // Disables and invalidates instruction cache
  SCB_DisableDCache(); // Disables, cleans and invalidates data cache
    
  //发送状态
  cy_en_ipc_pipe_status_t status;

  /*IPC配置*/
  cy_stc_ipc_pipe_config_t pipeConfig = 
  {
      .epIndexForThisCpu = CPU_M7_1,
      .epConfigData      = CY_IPC_PIPE_ENDPOINTS_DEFAULT_CONFIG,
  };  
  
  /* 初始化 PIPE */
  Cy_IPC_Pipe_Init(&pipeConfig);

  /* Already registered the handler to the system interrup structure.
   * So just enable corresponding IRQ channel.
   */
  NVIC_ClearPendingIRQ(pipeConfig.epConfigData[CPU_M7_1].ipcCpuIntIdx);
  NVIC_EnableIRQ(pipeConfig.epConfigData[CPU_M7_1].ipcCpuIntIdx);
 
    
  while(1)
  {
    /* 发送数据 */
    M71_released = false;
    status = Cy_IPC_Pipe_SendMessage
    (
        CPU_M7_0,
        &pipeData_M71,
        M71_Callback
    );
    
    /* When the data successfully into the pipe */
    if (status == CY_IPC_PIPE_SUCCESS)
    {
        while(!M71_released)
        {
            /* wait until the data has been received by the other core */
        }
        /* Update the data to be sent */
        pipeData_M71.data1 = (pipeData_M71.data1 + 1) % 12;
        // Wait 0.5 [s]
    }
    Cy_SysTick_DelayInUs(200000);
    LED_Ctrl(LED2, RVS);
  }
}



