#include "image.h"
#include "include.h"
#define white_pixel 255
#define black_pixel 0
uint8 image_two_value[MT9V03X_H][MT9V03X_W];//二值化图像
int zuan=20;
uint8_t threshold = 0;
//扫线
int16 mid = 47;
int16 left_line_list[60] = {0}; //左边界数组
int16 right_line_list[60] = {0}; //右边界数组
int16 left_line_list1[60] = {0}; //左边界数组
int16 right_line_list1[60] = {0}; //右边界数组
int16 mid_line_list[60] = {0}; //中线数组
int16 rode_wide[60] = {0};//赛宽

int16 left_lost_flag[60] = {0};//左边界丢线标志位数组
int16 right_lost_flag[60] = {0};//右边界丢线标志位数组

uint8 search_end_r = 1;//从左往右的搜索终点
uint8 search_end_l = MT9V03X_W-1;       //从右往左的搜索终点
uint8 search_start_l= 0;
uint8 search_start_r= MT9V03X_W-1;

//uint8 mid_weight_list[60] = 
//{
//  0,0,0,0,0,0,0,0,0,0,
//  0,0,0,0,0,0,0,0,0,0,
//  2,2,3,3,4,4,5,5,5,5,
//  6,6,7,7,8,8,10,10,10,10,
//  10,10,10,10,10,11,12,13,14,15,
//  16,17,18,19,20,21,23,24,25,26
//};

uint8 mid_weight_list[60] = 
{
  0,0,0,0,0,2,4,6,8,10,
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,3,3,3,3,3,
  4,4,4,4,4,4,5,5,6,6,
  7,7,8,8,9,10,11,12,13,14
  
};

uint8 mid_value = 47;//加权计算出的中线值
 

int16 image_error = 0;//中线误差

int16 last_valid_l_row = -1;//上一行左边界，-1则无效
int16 last_valid_l_col= -1;
int16 last_valid_r_row = -1;//上一行右边界，-1则无效
int16 last_valid_r_col= -1;

//直角
uint8 flag1 = 0;
uint8 flag2 = 0;
uint8 flag3 = 0;
uint8 youxian=0;
uint8 Benzene_point_left = 0;
uint8 Benzene_point_right = 0;
uint8 Benzene_point_on = 0;
uint8 left_stright = 0;
uint8 right_stright = 0;

//圆环
int16 flag_left= 0;
int16 flag_right= 0;
int16 Benzene_turn_flag_up= 0;
int16 Benzene_turn_flag_down= 0;
int16 Benzene_turn_flag_left= 0;
int16 Benzene_turn_flag_right= 0;
int16 Benzene_flag1= 0;
int16 Benzene_flag2= 0;
int16 Benzene_point_left1[10] = {0};
int16 Benzene_point_left2[10] = {0};
int16 Benzene_point_right1[10] = {0};
int16 Benzene_point_right2[10] = {0};
int16 duanlu111[10] = {0};
int16 first_pos= 0;
int16 second_pos= 0;
int16 left_circular_flag= 0;
int16 right_circular_flag= 0;
int16 left_point= 0;
int16 right_point= 0;
int16 left_height= 0;
int16 last_left_height= 0;
int16 right_height= 0;
int16 last_right_height= 0;
int16 cir_turn_left= 0;
int16 cir_turn_right= 0;
int16 left_circular_Cflag= 0;
int16 right_circular_Cflag= 0;
int b = 0;
int angle_cir = 328;//327
int16 cir_out_encoder= 0;

//十字
int16 cross_flag_l = 0;
int16 cross_flag_r = 0;
int16 cross_flag_on= 0;
int16 cross_flag = 0;
uint8 cross_left= 0;


//断路
uint8 check_row=0;
uint8 open_flag = 0;
uint8 start_check_row= MT9V03X_H-1;
uint8 end_check_row= 39;
float percent_lost= 0.f;
uint8 break_cont= 0;
uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];    

#define threshold_max  (255 * 5)  // 膨胀阈值
#define threshold_min  (255 * 2)  // 腐蚀阈值
void image_filter(uint8 (*bin_image)[94])
{
    uint16_t i, j;
    uint32_t num;

    for (i = 1; i < MT9V03X_H - 1; i++) {
        for (j = 1; j < MT9V03X_W - 1; j++) {
            // 计算8邻域像素和
            num = bin_image[i-1][j-1] + bin_image[i-1][j] + bin_image[i-1][j+1]
                + bin_image[i][j-1]   + bin_image[i][j+1]
                + bin_image[i+1][j-1] + bin_image[i+1][j] + bin_image[i+1][j+1];

            // 膨胀条件：周围白色像素多则填充
            if (num >= threshold_max && bin_image[i][j] == black_pixel) {
                bin_image[i][j] = white_pixel;
            }
            // 腐蚀条件：周围黑色像素多则清除
            if (num <= threshold_min && bin_image[i][j] == white_pixel) {
                bin_image[i][j] = black_pixel;
            }
        }
    }
}

void Get_image(uint8_t(*mt9v034Image)[188])
{
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < 120; i += 2)          
    {
        for (j = 0; j <188; j += 2)     
        {
            mt9v03x_image[row][line] = mt9v034Image[i][j];//这里的参数填写你的摄像头采集到的图像0
            line++;
        }
        line = 0;
        row++;
    }
}
int Chasu_PWM;//正常循迹差速PWM
int Camera_Error;
float KP=0.8,KP2=0.0010,KD=0,GKD=0.08;
//float KP=0,KP2=0,KD=0,GKD=0;
int XIANFU=120;
int Camera_Error_Get(void)
{
    float static Last_Camera_Error; 
    Camera_Error = 0;
    int Chasu_PWM = 0;
    Camera_Error = image_error;
    Chasu_PWM=(Camera_Error*KP+Camera_Error*abs(Camera_Error)*KP2+(Camera_Error-Last_Camera_Error)*KD-(int16_t)roundf(gyrox / 32.8f)*GKD);//获取转角值

  if(Chasu_PWM > 40 && Chasu_PWM>=0)Chasu_PWM = 40;
  if(Chasu_PWM < -40 && Chasu_PWM<=0)Chasu_PWM = -40;

    Last_Camera_Error = Camera_Error;//上一次图像偏差
    return Chasu_PWM;
}
/**************************************大津法*************************************************/
uint8_t otsuThreshold(uint8_t *image, uint16_t width, uint16_t height)
{
    #define GrayScale 256

    int pixelCount[GrayScale] = {0};       // 统计每个灰度值出现的次数
    float pixelPro[GrayScale] = {0.0f};    // 每个灰度值的概率
    int Sumpix = width * height;           // 总像素数
    uint8_t Threshold = 0;                 // 最终返回的阈值
    uint8_t* data = image;                 // 图像数据指针

    float u = 0.0f;                        // 图像全局平均灰度
    float maxVariance = 0.0f;              // 最大类间方差
    float w0 = 0.0f;                       // 前景区域比例
    float avgValue = 0.0f;                 // 前景区域的加权平均灰度

    // Step 1: 统计每个灰度值出现的次数
    for (int i = 0; i < Sumpix; ++i)
    {
        pixelCount[data[i]]++;
    }

    // Step 2: 计算每个灰度值的概率，并求整幅图像的平均灰度 u
    for (int i = 0; i < GrayScale; ++i)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;
        u += i * pixelPro[i];
    }

    // Step 3: 遍历所有可能的灰度值作为阈值，计算类间方差，找出最大值对应的阈值
    for (int i = 0; i < GrayScale; ++i)
    {
        w0 += pixelPro[i];         // 更新前景比例
        avgValue += i * pixelPro[i]; // 更新前景加权平均灰度

        if (w0 == 0 || w0 == 1) continue; // 跳过极端情况，防止除零错误

        // 计算类间方差
        float variance = powf((avgValue - u * w0), 2) / (w0 * (1 - w0));

        if (variance > maxVariance)
        {
            maxVariance = variance;
            Threshold = (uint8_t)i;
        }
    }
    return Threshold;
}

/***************************************二值化************************************************/
void Image_Binarization(void)
{
  uint16 i,j;
    
    //threshold= 60;
    threshold = otsuThreshold(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);
//    if(threshold > 90) threshold = 90;
//   else if(threshold < 70) threshold = 70;
    for(i=0;i<MT9V03X_H-1;i++)
    {
        for(j=0;j<MT9V03X_W-1;j++)//灰度图的数据只做判断，不进行更改，二值化图像放在了新数组中
        {
            if(mt9v03x_image[i][j]>=threshold)
                image_two_value[i][j]=255;//白
            else  
                image_two_value[i][j]=0;//黑
        }
    }
}
void turn_to_bin(void)
{
  unsigned char image_thereshold;
  uint8 i,j;
  image_thereshold = otsuThreshold1(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
  for(i = 0;i<MT9V03X_H;i++)
  {
      for(j = 0;j<MT9V03X_W;j++)
      {
         if(mt9v03x_image[i][j]>image_thereshold)
         {
             image_two_value[i][j] = white_pixel;
         }

         else
         {
             mt9v03x_image[i][j] = 0;
         }
      }
  }
}


/*---------------------------------------------------------------
 【函    数】otsuThreshold(uint8 *image, uint16 col, uint16 row)44功    能】大津法求阈值
 【参    数】进行大津法的图像数组，列数，行数
 【返 回 值】阈值大小
 【注意事项】
 ----------------------------------------------------------------*/
uint8 otsuThreshold1(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    static uint8 Last_Threshold;
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }



    for (MinValue = 0; HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MaxValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
    
//  if(50<Threshold && Threshold<160)//夜间
//     Last_Threshold = Threshold;
//   else
//     Threshold = Last_Threshold;
   
//   if(200<Threshold && Threshold<240)//14:25
//       Last_Threshold = Threshold;
//  else
     Threshold = Last_Threshold;
  
      if(threshold > 90) threshold = 90;
   else if(Threshold < 70) threshold = 70;
   
   return Threshold;
}
/**
 * @brief Sobel边缘检测（固定阈值）
 * @param imageIn 输入图像（original_image）
 * @param imageOut 输出二值图像（bin_image2）
 * @param Threshold 阈值（建议60-150）
 */
void sobel(uint8 imageIn[MT9V03X_H][MT9V03X_W], uint8 imageOut[MT9V03X_H][MT9V03X_W], uint8 Threshold)
{
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j;
    short temp[2];

    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算水平和垂直方向的梯度 */
            temp
[0] = -(short)imageIn[i - 1][j - 1] + (short)imageIn[i - 1][j + 1]  // 水平Sobel核
                    - (short)imageIn[i][j - 1]     + (short)imageIn[i][j + 1]
                    - (short)imageIn[i + 1][j - 1] + (short)imageIn[i + 1][j + 1];

            temp
[1] = -(short)imageIn[i - 1][j - 1] + (short)imageIn[i + 1][j - 1]  // 垂直Sobel核
                    - (short)imageIn[i - 1][j]     + (short)imageIn[i + 1][j]
                    - (short)imageIn[i - 1][j + 1] + (short)imageIn[i + 1][j + 1];

            temp
[0] = abs(temp[0]);  // 取绝对值
            temp
[1] = abs(temp[1]);

            /* 取两个方向的最大梯度 */
            if (temp[0] < temp[1])
                temp
[0] = temp[1];

            /* 阈值处理（边缘=黑，非边缘=白） */
            if (temp[0] > Threshold)
                imageOut
[i][j] = black_pixel;  // 边缘（黑）
            else
                imageOut
[i][j] = white_pixel;  // 非边缘（白）
        }
    }
}
/**************************************巡线************************************************/
uint8 l_border[MT9V03X_H];//左线数组
uint8 r_border[MT9V03X_H];//右线数组
uint8 center_line[MT9V03X_H];//中线数组
unsigned char in_flag = 0;
unsigned char zuoyou_xunxian(uint8 (*bin_image)[MT9V03X_W]) 
{
    int i, j;
    static int left_border = 0, right_border = 0, mid = MT9V03X_H / 2;
    static int last_left_border = 0, last_right_border = 0;
    
    // 第一次巡线，近大远小，近处按旧扫描方式
    for (i = MT9V03X_H - 1; i >= MT9V03X_H - 10; i--) // 底部10行
    {
        // 从左往右扫左边界
        for (j = 0; j < MT9V03X_W - 2; j++)
        {
            if (bin_image[i][j] == 0x00 && bin_image[i][j + 1] == 0xFF && bin_image[i][j + 2] == 0xFF)
            {
                left_border = j;
                break;
            }
        }
        if (j == MT9V03X_W - 2) left_border = last_left_border;

        // 从右往左扫右边界
        for (j = MT9V03X_W - 1; j > 1; j--)
        {
            if (bin_image[i][j - 2] == 0xFF && bin_image[i][j - 1] == 0xFF && bin_image[i][j] == 0x00)
            {
                right_border = j;
                break;
            }
        }
        if (j == 1) right_border = last_right_border;

        mid = (left_border + right_border) / 2;
        
        l_border[i] = (unsigned char)left_border;
        r_border[i] = (unsigned char)right_border;
        center_line[i] = (unsigned char)mid;
    }
    

    // 处理剩余的行(从第11行到顶部)
    for(i = MT9V03X_H - 11; i > 0; i--)
    {
        // 右边界搜索
        for (j = right_border - 15; j < MT9V03X_W - 2; j++)
        {
            if(j <= 0) j = 0;
            if (bin_image[i][j] == 0xFF && bin_image[i][j + 1] == 0x00 && abs(j - right_border) < 10)
            {
                right_border = j;
                break;
            }
        }
        if (j == MT9V03X_W - 2) right_border = last_right_border;

        // 左边界搜索
        for (j = left_border + 15; j > 1; j--)
        {
            if(j >= MT9V03X_W - 1) j = MT9V03X_W - 1;
            if (bin_image[i][j - 1] == 0x00 && bin_image[i][j] == 0xFF && abs(j - left_border) < 10)
            {
                left_border = j;
                break;
            }
        }
        if (j == 1) left_border = last_left_border;
        
        mid = (left_border + right_border) / 2;
        
        l_border[i] = (unsigned char)left_border;
        r_border[i] = (unsigned char)right_border;
        center_line[i] = (unsigned char)mid;
        
        last_left_border = left_border;
        last_right_border = right_border;
    }

    return in_flag;
}
/**************************************扫线循迹*****************************************************/
void findline(void)
{
    // 修改搜索范围以适应96像素高度
    search_end_l = MT9V03X_W - 20;
    search_end_r = 20;
  
    if(left_circular_flag == 1 || left_circular_Cflag == 1 || cir_turn_right == 0 || Benzene_turn_flag_up == 2)
    {
        search_end_r = mid - 5;
    }
    if(right_circular_flag == 1 || right_circular_Cflag == 1 || cir_turn_left == 0 || Benzene_turn_flag_up == 2)
    {
        search_end_l = mid + 5;
    }
    
  for(uint8 i=MT9V03X_H-1;i>4;i--)
  {
    left_lost_flag[i] = 0;
    right_lost_flag[i] = 0;//初始化默认左右边界丢线，只有找到边界才置标志位

    for(uint8 j=20;j<search_end_l;j++)
    {
      if(image_two_value[i][j]==0 && image_two_value[i][j+1]==255 && image_two_value[i][j+2]==255)
      {
        left_line_list1[i] = j;
        left_lost_flag[i] = 1;//丢线置0，没丢线置1
        break;
      }
    }
    
    for(uint8 j=MT9V03X_W-20;j>search_end_r;j--)
    {
      if(image_two_value[i][j]==0 && image_two_value[i][j-1]==255 && image_two_value[i][j-2]==255)
      {
        right_line_list1[i] = j;
        right_lost_flag[i] = 1;
        break;
      }
    }

    //虚线检测，插值补线算法 
    if(left_lost_flag[i]==1)//当前行左边界没丢线
    {
      if(last_valid_l_row== i)//防止除0
        continue;
      if(last_valid_l_row!=-1 && last_valid_l_row-1>i && last_valid_l_row-i<MAX_INTERPOLATE_GAP) //上一次有效边界距离这一次有效边界有一段距离但是不超过最大距离
      {
        //插值补线
        float delta_col= (float)(last_valid_l_col-left_line_list1[i])/(last_valid_l_row-i); //计算斜率
        for(int16 r=i+1;r<last_valid_l_row;r++)
        {
          left_line_list1[r]=(int16)(left_line_list1[i] + delta_col * (r - i));
          left_lost_flag[r]=1;//插值成功
        }
      }
      //更新上一次有效状态
      last_valid_l_row= i;
      last_valid_l_col= left_line_list1[i];      
    }
    else
    {
      if(last_valid_l_row-i>MAX_INTERPOLATE_GAP)
      {
        last_valid_l_row= -1; 
      }
    }
    
    
    if(right_lost_flag[i]==1)//当前行右边界没丢线
    {
      
      if(last_valid_r_row== i) //防止除0
        continue;
      if(last_valid_r_row!=-1 && last_valid_r_row-1>i && last_valid_r_row-i<MAX_INTERPOLATE_GAP)
      {
        //插值补线
        float delta_col= (float)(last_valid_r_col-right_line_list1[i])/(last_valid_r_row-i);
        for(int16 r=i+1;r<last_valid_r_row;r++)
        {
          right_line_list1[r]=(int16)(right_line_list1[i] + delta_col * (r - i));
          right_lost_flag[r]=1;//插值成功
        }
      }
      //更新上一次有效状态
      last_valid_r_row= i;
      last_valid_r_col= right_line_list1[i];      
    }
    else
    {
      if(last_valid_r_row-i>MAX_INTERPOLATE_GAP)
      {
        last_valid_r_row= -1; 
      }
    }
  }
    // 调用新的循迹算法
    in_flag = zuoyou_xunxian(image_two_value);
    
    // 将结果复制到原有变量中
    for(uint8 i = 0; i < MT9V03X_H; i++)
    {
        left_line_list[i] = l_border[i];
        right_line_list[i] = r_border[i];
        mid_line_list[i] = center_line[i];
 //       left_lost_flag[i] = (l_border[i] == 0) ? 0 : 1;
 //       right_lost_flag[i] = (r_border[i] == 0) ? 0 : 1;
        
   //圆环处理
    if(left_circular_flag== 1 )//检测到左环岛
    {
      Target_Speed1=50;
      if(cir_turn_left== 1)//准备进左环岛
      {
           mid_line_list[i] = center_line[i]-20;
      }
      else
      {
        mid_line_list[i] = center_line[i]-5;//巡右单线 
      }
    }
    else if(right_circular_flag== 1)//识别到右环岛
    {      
      Target_Speed1=50;
      if(cir_turn_right == 1)//准备进右环岛
      {
     mid_line_list[i] = center_line[i]+20;
      }
      else
      {
        mid_line_list[i] = center_line[i]+5;
      }
    }
    else
    {
      
        mid_line_list[i] = center_line[i];
    }
        
        // 环岛退出检测
        if(left_circular_Cflag == 1)
        {
            mid_line_list[i] = center_line[i];
            if(rode_wide[i] < 10 && rode_wide[i-1] < 10 && rode_wide[i-2] < 10) 
            {
                left_circular_Cflag = 0;
                search_end_l = MT9V03X_W - 3;
                search_end_r = 3;
                left_height = 0;
                last_left_height = 0;
                left_circular_flag=0;
                cir_turn_left=0;
                b = 0;
                Target_Speed1=70;
            }
        }
        else if(right_circular_Cflag == 1)
        {
            mid_line_list[i] = center_line[i];
            if(rode_wide[i] < 10 && rode_wide[i-2] < 10 && rode_wide[i-4] < 10)
            {
                right_circular_Cflag = 0;
                search_end_l = MT9V03X_W - 3;
                search_end_r = 3;
                right_height = 0;
                last_right_height = 0;
                right_circular_flag=0;
                cir_turn_right=0;
                b = 0;
                Target_Speed1=70;
            }
        }

        rode_wide[i] = abs((right_line_list1[i] - left_line_list1[i]));
    }
}

/****************************************加权中线计算****************************************************************/
uint8 find_mid_line_weight(void)
{
  uint8 mid_line = 47;
  static uint8 last_mid_line= 47;
  uint32 weight_middle_sum = 0; //加权中线累加值
  uint32 weight_sum = 0;        //权重累加值
  for(uint8 i = 59 ; i > 20 ; i--)
  {
    weight_middle_sum += mid_line_list[i] * mid_weight_list[i];
    weight_sum +=  mid_weight_list[i];
  }
  if(weight_sum== 0) return last_mid_line;
  mid_line =(uint8) (weight_middle_sum/ weight_sum);
  mid_value= (uint8)(last_mid_line*0.1 + mid_line*0.9); //互补滤波
  last_mid_line= mid_value;
  
  return mid_value;
}
/*用于十字引导，当识别到十字的时候，用上五行中线误差来引导*/
uint8 cross_guide_midline(void)
{
  uint8 mid_line = 47;
  static uint8 last_mid_line= 47;
  uint32 weight_middle_sum = 0;
  uint32 weight_sum = 0;
  for(uint8 i=9;i>4;i--)
  {
    weight_middle_sum += mid_line_list[i]*mid_weight_list[i];
    weight_sum += mid_weight_list[i];
  }
  if(weight_sum== 0) return last_mid_line;
  mid_line = (uint8) (weight_middle_sum/ weight_sum);
  mid_value= (uint8)(last_mid_line*0.1 + mid_line*0.9); //互补滤波
  last_mid_line= mid_value;
  
  return mid_value;
}

/**********************************************中线计算误差****************************************************************/
void get_error(void)
{
    //uint8 mid = find_mid_line_weight();
    if(cross_flag==1)
    {
      image_error = 44 - find_mid_line_weight();
    }
    else
    {
      image_error = 44 - find_mid_line_weight();
    }
    
    if(left_stright==1)
    {
      image_error+=zuan;
    }
    else if(right_stright==1)
    {
      image_error-=zuan;
    }
}


/****************************************************画线函数******************************************************************/
//void drawkline(void)
//{
//  for(uint8 i = MT9V03X_H-1 ; i > 1 ; i--)
//  {
//    if(left_line_list[i]>MT9V03X_W-1) left_line_list[i]= MT9V03X_W-1;
//    else if(left_line_list[i]<0) left_line_list[i] = 0;
//    ips200_draw_point((int16)left_line_list[i] , i , RGB565_YELLOW);
// 
//    if(right_line_list[i]>MT9V03X_W-1) right_line_list[i]= MT9V03X_W-1;
//    else if(right_line_list[i]<0) right_line_list[i] = 0;
//    ips200_draw_point((int16)right_line_list[i] , i , RGB565_RED);
//
//    if(mid_line_list[i]>MT9V03X_W-1) mid_line_list[i]= MT9V03X_W-1;
//    else if(mid_line_list[i]<0) mid_line_list[i] = 0;
//    ips200_draw_point((int16)mid_line_list[i] , i , RGB565_GREEN);
//  }
//}

/*****************直线判断******************/
float Straight_Judge(uint8 dir, uint8 start, uint8 end)     //返回结果小于1即为直线
{
    int i;
    float S = 0, Sum = 0, Err = 0, k = 0;
    switch (dir)
    {
    case 1:k = (float)(left_line_list[start] - left_line_list[end]) / (start - end);
        for (i = 0; i < end - start; i++)
        {
            Err = (left_line_list[start] + k * i -left_line_list[i + start]) * (left_line_list[start] + k * i - left_line_list[i + start]);
            Sum += Err;
        }
        S = Sum / (end - start);
        break;
    case 2:k = (float)(right_line_list[start] - right_line_list[end]) / (start - end);
        for (i = 0; i < end - start; i++)
        {
            Err = (right_line_list[start] + k * i - right_line_list[start + i]) * (right_line_list[start] + k * i - right_line_list[start + i]);
            Sum += Err;
        }
        S = Sum / (end - start);
        break;
    case 3:k = (float)(mid_line_list[start] - mid_line_list[end])/(start-end);
        for(i = 0;i<end-start;i++)
        {
            Err = (mid_line_list[start] + k * i - mid_line_list[start + i]) * (mid_line_list[start] + k * i - mid_line_list[start + i]);
            Sum += Err;
        }
        S = Sum / (end - start);
    }
    return S;
}
/****************************************************画线函数******************************************************************/
void drawkline(void)
{
  for(uint8 i = MT9V03X_H-1 ; i > 1 ; i--)
  {
    if(left_line_list[i]>MT9V03X_W-1) left_line_list[i]= MT9V03X_W-1;
    else if(left_line_list[i]<0) left_line_list[i] = 0;
   LCD_DrawPoint((int16)right_line_list1[i],i,RED_IPS);
 
    if(right_line_list[i]>MT9V03X_W-1) right_line_list[i]= MT9V03X_W-1;
    else if(right_line_list[i]<0) right_line_list[i] = 0;
    LCD_DrawPoint((int16)right_line_list[i],i,YELLOW);

    if(mid_line_list[i]>MT9V03X_W-1) mid_line_list[i]= MT9V03X_W-1;
    else if(mid_line_list[i]<0) mid_line_list[i] = 0;
    LCD_DrawPoint((int16)mid_line_list[i],i,BLUE);
  }
}

/****************************************************直角*************************************************************************/
void stright_angle(void) 
{
  //if(yh\\dl)retirn;
  for(uint8 j=MT9V03X_W-3;j>=mid+30;j--)  //右侧从下往上搜
  {
    for(uint8 i=MT9V03X_H-3;i>5;i--)
    {
      if(image_two_value[i][j]==0 && image_two_value[i-1][j]==255 && image_two_value[i-2][j]==255)
      {
        flag2 = 1;
        Benzene_point_right = i;
        break;
      }
      else
      {
        flag2 = 0;
      }
    }
  }
  
  for(uint8 j=0;j<=mid-30;j++)   //左侧从下往上搜
  {
    for(uint8 i=MT9V03X_H-3;i>5;i--)
    {
      if(image_two_value[i][j]==0 && image_two_value[i-1][j]==255 && image_two_value[i-2][j]==255)
      {
        flag1 = 1;
        Benzene_point_left = i;
        break;
      }
      else
      {
        flag1 = 0;
      }
    }
  }
  
  for(uint8 i=0;i<=20;i++)  //上方从左往右搜
  {
    for(uint8 j=3;j<=MT9V03X_W-3;j++)
    {
      if(image_two_value[i][j]==0 && image_two_value[i][j+1]==255 && image_two_value[i][j+2]==255)
      {
        flag3 = 1;
        youxian=1;
      duanlu111[0]=i;
      duanlu111[1]=j;

        break;
      }
      else
      {
        flag3 = 0;
        youxian=0;
      }
    }
  }
  
  if(flag1 == 1 && flag2 == 0 && flag3 == 0 && cir_turn_left==0 && cir_turn_right==0) //进入圆环后则不判断直角了，防止误判
  {
    left_stright = 1; //识别到左直角
    Add_Line(0,Benzene_point_left,mid_line_list[58],58);
    Add_Line(0,0,0,Benzene_point_left);
  }
  else if(flag1 == 0 && flag2 == 1 && flag3 == 0 && cir_turn_left==0 && cir_turn_right==0)
  {
    right_stright = 1;//识别到右直角
    Add_Line(93,Benzene_point_right,mid_line_list[58],58);
    Add_Line(93,0,93,Benzene_point_right);
  }
  else
  {
    left_stright = 0;
    right_stright = 0;
  }
}

/************************************圆环*************************************************/
int16 cir_wide=0;
void circular(void)
{
    for(uint8 i=0;i<5;i++) // 搜索结果顶行10行,自左向右搜索，记录一次和第二次黑白跳变点
      {
          Benzene_turn_flag_up = 0; // 初始化跳变标志
          Benzene_flag1 = 0;
          for(uint8 j=0;j<MT9V03X_W-3;j++) // 遍历每一行的像素
          {
              if(Benzene_turn_flag_up == 0) // 寻找第一次跳变
              {
                  if(image_two_value[i][j] == 0 && image_two_value[i][j+1] == 255 )
                  {
                      Benzene_point_left1[i]= j;//第一次跳变点坐标
                      Benzene_turn_flag_up = 1; // 标记已找到第一次跳变
                      Benzene_point_left2[0]=i;
                      Benzene_point_left2[1]=j;
                  }
                  
                  
              }
              else if(Benzene_turn_flag_up == 1) // 寻找第二次跳变
              {
                  if(image_two_value[i][j] == 0 && image_two_value[i][j+1] == 255 && image_two_value[i][j+2]==255)
                  {   Benzene_point_right1[i]= j;//第一次跳变点坐标
                      Benzene_turn_flag_up= 2;
                      Benzene_point_right2[0] = i;
                      Benzene_point_right2[1] = j;

                      break;
                  }
              }
          }
          if(Benzene_turn_flag_up== 2) break;
          
      }
    
    for(uint8 i=59;i>54;i--)
    {
      Benzene_turn_flag_down= 0;
      Benzene_flag2= 0;
      for(uint8 j=0;j<MT9V03X_W-3;j++)
      {
        if(Benzene_turn_flag_down== 0)
        {
            if(image_two_value[i][j-1]==0 && image_two_value[i][j] == 0 && image_two_value[i][j+1] == 255 && image_two_value[i][j+2] == 255)
            {
              Benzene_turn_flag_down = 1; // 标记已找到第一次跳变
            }
        }
        else if(Benzene_turn_flag_down== 1)
        {
            if(image_two_value[i][j-1]==0 && image_two_value[i][j] == 0 && image_two_value[i][j+1] == 255 && image_two_value[i][j+2] == 255)
            {
              Benzene_turn_flag_down= 2;
              break;
            }
        }
      }
      if(Benzene_turn_flag_down== 2) {break;}
    }
//  cir_wide=0;
//      for(uint8 i=MT9V03X_H-1;i>0;i--)
//      {
//        if(rode_wide[i] > NORMAL_RODE_WIDE)
//        {
//          cir_wide++;
//        }
//     }

      for (int j = mid + 30; j < mid + 40; j++) //右侧搜线由下到上
      {
        //Benzene_turn_flag_right= 0;
        for (int i = MT9V03X_H-1; i > 20; i--) 
        {
            // 检查像素是否符合黑-白-白的跳变
          if(image_two_value[i][j]==0 && image_two_value[i-1][j]==255 && image_two_value[i-2][j]==255)
          {
              flag_right= 1;
              break;
          }
          else {flag_right= 0;}
            
        }
        if(flag_right== 1) break;
     }

    // 检测左侧区域
    for (int j = mid - 30; j > mid - 40; j--) 
    {
     //Benzene_turn_flag_left=0;
        for (int i = MT9V03X_H-1; i > 20; i--) 
        {
            // 检查像素是否符合黑-白-白的跳变
          if(image_two_value[i][j]==0 && image_two_value[i-1][j]==255 && image_two_value[i-2][j]==255)
          {
            flag_left= 1;
            break;
          }
          else {flag_left= 0;}
        }
        if(flag_left== 1) break;
    }
  //左右环判断
  
      if( Benzene_turn_flag_up==2 && Benzene_turn_flag_down==2 && flag_left==0 && flag_right==1 && right_stright==0 && left_stright==0 && cross_flag==0 &&left_circular_flag==0 )//右环岛
     {
        right_circular_flag = 1;
     }
     else if( Benzene_turn_flag_up==2 && Benzene_turn_flag_down==2&& flag_left==1 && flag_right==0 && right_stright==0 && left_stright==0 && cross_flag==0 &&right_circular_flag==0)//左环岛
     {
        left_circular_flag = 1; 
     }
    
/*****判断入环*******/    
  if(left_circular_flag== 1)
  {
    for(uint8 j=mid-5;j>34;j--)
    {
      for(uint8 i=MT9V03X_H-3;i>0;i--)
      {
        if(image_two_value[i][j]==0 && image_two_value[i+1][j]==255 && image_two_value[i+2][j]==255)
        {
          left_point= i;
          left_height= MT9V03X_H-left_point;
          break;
        }
      }
      
      
    }
    if((abs)(left_height- last_left_height) >= 20)
    {
      b++;
      if(b == 2 )
      {
         cir_turn_left= 1;//左圆环入环标志 
      }
    }
    last_left_height= left_height;
  }
  else if(right_circular_flag== 1)
  {
    for(uint8 j=mid+5;j<75;j++)
    {
      for(uint8 i=MT9V03X_H-3;i>0;i--)
      {
        if(image_two_value[i][j]==0 && image_two_value[i+1][j]==255 && image_two_value[i+2][j]==255)
        {
          right_point= i;
          right_height= MT9V03X_H-right_point;
          break;
        }
      }
    }
    if((abs)(right_height- last_right_height) >= 20)
    {
      b++;
      if(b == 2 )
      {
         cir_turn_right= 1;//右圆环入环标志
      }
    }
    last_right_height= right_height;
    
  }
  
  
   if(Motor_control.yaw_accumulated  >170)
      { 
          Motor_control.yaw_accumulated = 0;
          left_circular_Cflag = 1;
          last_left_height=0;
      }
    else if(Motor_control.yaw_accumulated <- 160)
      {
          Motor_control.yaw_accumulated=0;
          right_circular_Cflag = 1;
          last_right_height= 0;
      }    
    
}

/**********************************十字*******************************************/
void cross(void)
{
  for(uint8 j = MT9V03X_W-1 ; j > MT9V03X_W-10 ; j--)
   {
     for(uint8 i = MT9V03X_H - 20 ; i > 2 ; i--) //右侧从下往上搜索
    {
     if(image_two_value[i][j] == 0 && image_two_value[i-1][j] == 255 && image_two_value[i-2][j] == 255 )
     {
        cross_flag_r = 1; //右侧从下往上找，找到跳变点置1 
        break;
     }
     else
     {
        cross_flag_r = 0;//未找到跳变点置0
     }
    }
    if(cross_flag_r== 1) {break;}
   }
  
  for(uint8 j = 0 ; j < 10 ; j++)
   {
     for(uint8 i = MT9V03X_H - 20 ; i > 2 ; i--) //左侧从下往上搜索
    {
     if(image_two_value[i][j] == 0 && image_two_value[i-1][j] == 255 && image_two_value[i-2][j] == 255 )
     {
        cross_flag_l = 1; //右侧从下往上找，找到跳变点置1 
        break;
     }
     else
     {
        cross_flag_l = 0;//未找到跳变点置0
     }
    }
    if(cross_flag_l== 1) {break;}
   }
  
  for(uint8 i = 0 ; i < 30 ; i++) //图像上方从左往右不断搜索
   {
      for(uint8 j = 0 ; j < MT9V03X_W-1 ; j++)
      {
        if(image_two_value[i][j] == 0 && image_two_value[i][j+1] == 255 && image_two_value[i][j+2] == 255)
        {
          cross_flag_on = 1; //找到跳变点置1，未找到置0
          cross_left = j;
          break;
        }
        else
        {
          cross_flag_on = 0;
        }
      }
      if(cross_flag_on== 1) {break;}
   }
  
  if(cross_flag_l== 1 && cross_flag_r== 1 && cross_flag_on== 1)
  {
    cross_flag = 1;
  }
  else
  {
    cross_flag = 0;
  }
}




/************************************断路**********************************************/
void duanlu(void)
{
  uint8 lost_count= 0;
  for(uint8 i = start_check_row;i>end_check_row;i--) 
  {
    if(right_lost_flag[i]==0 && left_lost_flag[i]==0)
    {
      lost_count++;//检测丢线行数
    }
  }
  
  check_row= start_check_row- end_check_row; //总检测行
  
  percent_lost= (lost_count*100/check_row); //丢线百分比
  
  if(percent_lost > 95)
  {
    open_flag= 1;
  }
}


uint8 is_duanlu() 
{
    bool left_has_line = false;  // 左侧有引导线
    bool right_has_line = false; // 右侧有引导线
    bool top_has_line = false;   // 顶部有引导线
    bool found;

    // ============== 条件1: 左侧0-36区域无引导线 ==============
    found = false;
    for (int y = MT9V03X_H-3; y >= 0 && !found; y--) {
        for (int x = 0; x <= 18; x++) { // 左区扫描
            if (image_two_value[y][x] == black_pixel && 
                image_two_value[y+1][x] == white_pixel && 
                image_two_value[y+2][x] == white_pixel) {
                left_has_line = true;
                found = true;
                break;
            }
        }
    }

    // ============== 条件2: 右侧77-114区域无引导线 ==============
    found = false;
    for (int y = MT9V03X_H-3; y >= 0 && !found; y--) {
        for (int x = 76; x <= 94; x++) { // 右区扫描
            if (image_two_value[y][x] == black_pixel && 
                image_two_value[y+1][x] == white_pixel && 
                image_two_value[y+2][x] == white_pixel) {
                right_has_line = true;
                found = true;
                break;
            }
        }
    }

    // ============== 条件3: 顶部0-36行无引导线 ==============
    found = false;
    for (int y = 0; y <= 18 && !found; y++) {    // 顶部行范围
        for (int x = 0; x < YS_WIDTH-3; x++) {   // 横向扫描
            if (image_two_value[y][x] == black_pixel && 
                image_two_value[y][x+1] == white_pixel && 
                image_two_value[y][x+2] == white_pixel) {
                top_has_line = true;
                found = true;
                break;
            }
        }
    }

    // ============== 联合判断 ==============
    // 三向均无引导线时返回5
    return (!left_has_line && !right_has_line && !top_has_line) ? 1 : 0;
}
/***********************************补线函数***********************************************/
void Add_Line(uint8 x1,uint8 y1,uint8 x2,uint8 y2)//补中线函数
{
    uint8 i,max,a1,a2;
    uint8 hx;
    if(x1>=MT9V03X_W-1)//起始点位置校正，排除数组越界的可能
       x1=MT9V03X_W-1;
    else if(x1<=0)
        x1=0;
     if(y1>=MT9V03X_H-1)
        y1=MT9V03X_H-1;
     else if(y1<=0)
        y1=0;
     if(x2>=MT9V03X_W-1)
        x2=MT9V03X_W-1;
     else if(x2<=0)
             x2=0;
     if(y2>=MT9V03X_H-1)
        y2=MT9V03X_H-1;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }

    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_W)
            hx=MT9V03X_W;
        else if(hx<=0)
            hx=0;
       mid_line_list[i]=hx;
    }
}





