#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "include.h"
#define MAX_INTERPOLATE_GAP 20 // 可调参数，表示最多补多少行
#define NORMAL_RODE_WIDE 10


uint8_t otsuThreshold(uint8_t *image, uint16_t width, uint16_t height);
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



extern uint8 image_two_value[MT9V03X_H][MT9V03X_W];
extern int16 image_error;
extern uint8 left_stright;
extern uint8 right_stright;

extern int16 left_circular_flag;
extern int16 right_circular_flag;
extern int16 cir_turn_left;
extern int16 cir_turn_right;
extern int16 left_circular_Cflag;
extern int16 right_circular_Cflag;
extern int16 flag_left;
extern int16 flag_right;
extern int16 Benzene_turn_flag_up;
extern int b ;
extern int16 right_height;
extern int16 last_right_height;
extern int16 cir_wide;

extern uint8_t threshold;

extern int16 cir_out_encode;

extern int16 cross_flag;

;

#endif

