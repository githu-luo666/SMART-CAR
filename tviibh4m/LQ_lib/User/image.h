#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "include.h"

#define MAX_INTERPOLATE_GAP 20 // 可调参数，表示最多补多少行
#define NORMAL_RODE_WIDE 10
#define MT9V03X_W  94                                        // 图像宽度     (可修改 不得超过188)  当分辨率为188 * 120时，可采集到至多200帧的图像(可能有噪点)
#define MT9V03X_H  60  
//extern  uint8_t mt9v03x_image[MT9V03X_H][MT9V03X_W];
extern unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W];


uint8_t otsuThreshold(uint8_t *image, uint16_t width, uint16_t height);
uint8 otsuThreshold1(uint8 *image, uint16 col, uint16 row) ;
void Image_Binarization(void);
void findline(void);
uint8 find_mid_line_weight(void);
uint8 cross_guide_midline(void);
void get_error(void);
void drawkline(void);
void stright_angle(void);
void circular(void);
void cross(void);
void duanlu(void);
float Straight_Judge(uint8 dir, uint8 start, uint8 end);
void Add_Line(uint8 x1,uint8 y1,uint8 x2,uint8 y2);
void Get_image(uint8_t(*mt9v03x_image)[188]);
void image_filter(uint8 (*image_two_value)[94]);
void turn_to_bin(void);
void sobel(uint8 imageIn[MT9V03X_H][MT9V03X_W], uint8 imageOut[MT9V03X_H][MT9V03X_W], uint8 Threshold);
int  Camera_Error_Get(void);
extern float KP,KP2,KD,GKD;


extern uint8 image_two_value[MT9V03X_H][MT9V03X_W];
extern int16 image_error;
extern uint8 left_stright;
extern uint8 right_stright;
extern int Chasu_PWM;
extern int Camera_Error;
extern int zuan;

extern int16 duanlu111[10] ;
extern int16 left_circular_flag;
extern int16 right_circular_flag;
extern int16 cir_turn_left;
extern int16 cir_turn_right;
extern int16 left_circular_Cflag;
extern int16 right_circular_Cflag;
extern int16 flag_left;
extern int16 flag_right;
extern int16 Benzene_turn_flag_up;
extern int16 Benzene_turn_flag_down;
extern int b ;
extern int16 right_height;
extern int16 last_right_height;
extern int16 cir_wide;

extern uint8_t threshold;
extern uint8 open_flag ;
extern uint8 youxian;
extern uint8 is_duanlu() ;

extern int16 cir_out_encode;

extern int16 cross_flag;

;

#endif

