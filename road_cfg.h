#ifndef _ROAD_H_
#define _ROAD_H_

#include "common.h"
#include "include.h"


//---------十字识别-------------------------
typedef enum
{
    CROSS_IN,              //准备进入十字
    CROSS,               //在十字里面
    GARAGE_IN,              //出车库
              //入车库
    // LOOP_L,              //先不考虑 圆环
    // LOOP_R
}road_e;

//--------------巡点--------------------------
typedef struct {            //单侧断点的记录            
    uint8 number,           //记录当前画面的断点数
    // uint8 start_line;       //扫描起始行 从上往下扫
    // uint8 end_line;       //扫描结束行
    uint8 x,
    uint8 y,
    uint8 flag = 0,      //1: 存在； 2: 不存在 默认不存在
    uint8 count

} point_t;
//-------------巡线----------------------
typedef struct {                                //寻线
    uint8 start_flag = 0,      //正序 x i 
    uint8 end_flag = 0,
    uint8 start = 60,             //默认60
    uint8 end = 118             //默认118
    uint8 
} line_t;
//------------弯道识别-----------------
typedef struct {                                //寻线
    uint8 left_flag = 0,     
    uint8 right_flag = 0,
    uint8 left_count = 0,
    uint8 right_count = 0   
} curve_t;

void line_config();
void find_point_R(point_t *p);
void find_point_L(point_t *p);
road_e corss_config();
#endif