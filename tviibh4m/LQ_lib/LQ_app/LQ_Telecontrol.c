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

char TXT[50];
char KEY_Value = 0,DSWA = 0,DSWB = 0,DSWC = 0,DSWD = 0;

int L_Around = 0, L_UpDown = 0;
int R_Around = 0, R_UpDown = 0;


void Telecontrol(void)
{
    char *start;
    char numbers[9][10];
    uint8_t count = 0;

    if((start =strstr((const char *)ReadBuff0,"A6A8K:"))!= NULL)
    {
         start +=6;
         while (count < 9)
         {
             if (*start == '-' || (*start >= '0' && *start <= '9'))
             {
                 int i = 0;
                 while ((*start >= '0' && *start <= '9') || *start == '-')
                 {
                     numbers[count][i++] = *start++;
                 }
                 numbers[count][i] = '\0';
                 count++;
             }
             else
             {
                 start++;
             }
         }
         KEY_Value = (char)atoi(numbers[0]);
         DSWA = (char)atoi(numbers[1]);
         DSWB = (char)atoi(numbers[2]);
         DSWC = (char)atoi(numbers[3]);
         DSWD = (char)atoi(numbers[4]);
         L_Around = (int)atoi(numbers[5]);
         L_UpDown = (int)atoi(numbers[6]);
         R_Around = (int)atoi(numbers[7]);
         R_UpDown = (int)atoi(numbers[8]);

         printf("K%d,A%d,B%d,C%d,D%d,L%d,R%d\r\n",KEY_Value,DSWA,DSWB,DSWC,DSWD,L_Around,R_Around);

    }
    else
    {
        printf("帧头未找到！\n");
    }
}
void Telecontrol_Test(void)
{
    TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏，2，3
    TFTSPI_CLS(u16BLUE);   //蓝色屏幕
    TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
    TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
    TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
    
    LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
    Cy_SysTick_DelayInUs(1000000);         //延时等待
    TFTSPI_CLS(u16BLACK);   //蓝色屏幕
    while(1)
    {
        if(Usart7_Rec_Fini_Flag == 1)
        {
            Usart7_Rec_Fini_Flag = 0;
            Telecontrol();
        }
        sprintf(TXT, "K:%1d", KEY_Value);
        TFTSPI_P8X16Str(1, 1, TXT, u16RED, u16BLUE);


        sprintf(TXT, "L:%4d R:%4d", L_UpDown,R_UpDown);
        TFTSPI_P8X16Str(1, 3, TXT, u16RED, u16BLUE);
        sprintf(TXT, "L:%4d R:%4d", L_Around,R_Around);
        TFTSPI_P8X16Str(1, 5, TXT, u16RED, u16BLUE);

        sprintf(TXT, "A:%1d B:%1d C:%1d D:%1d", DSWA,DSWB,DSWC,DSWD);
        TFTSPI_P8X16Str(1, 7, TXT, u16RED, u16BLUE);

        LQ_Printf("K%d,A%d,B%d,C%d,D%d,L%d,R%d\r\n",KEY_Value,DSWA,DSWB,DSWC,DSWD,L_Around,R_Around);

        LED_Ctrl(LED1, RVS); // 四个LED同时闪烁
        Cy_SysTick_DelayInUs(10000);         // 延时等待
    }
}