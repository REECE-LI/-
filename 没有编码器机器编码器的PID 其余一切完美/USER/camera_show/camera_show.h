#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"
#include "SEEKFREE_MT9V032.h"   
    
//extern   Size_t size = {128,90};          //W = 128,H = 90
//extern   Site_t site     = {0, 0};                           //显示图像左上角位置
//extern   Size_t imgsize  = {188, 120};                       //图像大小
//    
//    
    
extern uint8  middleline[120];          //存储中线的数组
extern uint8  imagebuff[ROW][COL];      //图像二值化数组
void PORTA_IRQHandler();
void DMA0_IRQHandler();

void camera_search(void);
void search_middleline(void);
void show_middleline(void);

#endif 
