#ifndef __KEY_H__
#define __KEY_H__


#include "common.h"
#include "include.h"
#include "VCAN_menu.h"

extern result_e read_func(void);
extern result_e write_func(void);


static unsigned char const read_name[2*2*16]= //读取  
{
    //共2个字节，一行占用(16+7)/8=2个字节，共16行。
    /* 读 */
    0x00,0x20,0x20,0x20,0x11,0xFC,0x10,0x20,0x00,0x20,0x03,0xFE,0xF0,0x02,0x10,0x94,
    0x10,0x50,0x11,0x10,0x10,0x90,0x13,0xFE,0x14,0x28,0x18,0x44,0x10,0x82,0x03,0x02,
    /* 取 */
    0x00,0x00,0xFF,0x80,0x22,0xFC,0x22,0x44,0x3E,0x44,0x22,0x44,0x22,0x44,0x3E,0x44,
    0x22,0x28,0x22,0x28,0x27,0xA8,0xFA,0x10,0x42,0x10,0x02,0x28,0x02,0x44,0x02,0x82,
};

static unsigned char const write_name[2*2*16]= //写入  
{
    //共2个字节，一行占用(16+7)/8=2个字节，共16行。
    /* 写 */
    0x00,0x00,0x7F,0xFE,0x40,0x02,0x90,0x04,0x10,0x00,0x1F,0xF8,0x10,0x00,0x20,0x00,
    0x3F,0xF8,0x00,0x08,0x00,0x08,0xFF,0xC8,0x00,0x08,0x00,0x08,0x00,0x50,0x00,0x20,
    /* 入 */
    0x04,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x02,0x80,0x02,0x80,0x02,0x80,
    0x04,0x40,0x04,0x40,0x08,0x20,0x08,0x20,0x10,0x10,0x20,0x10,0x40,0x08,0x80,0x06,
};

static menu_t   pid_menu_deal_f[] =   //菜单函数数组
{
    VCAN_MENU(read_name,read_func),
    VCAN_MENU(write_name,write_func),
   
};


extern float  KP;
extern float  KD;
extern float PID;
extern float speed_KP ;
extern float speed_KI ;
extern float speed_KD ;
//void changePID(void);
//void PORTD_IRQHandler(void);        //PORTD端口中断服务函数
//void key_up(void);  
//void key_left(void);
//void key_right(void);
//void key_middle(void);
//void key_down(void);
//void key_save(void);
//Site_t site_1     = {50, 30};                           //显示PID的 位置坐标

static Site_t pid_site = {0,112};
static uint8 pid_max =    2;
static uint8 pid_new =  0;


extern result_e    pid_deal(void);


void pid_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);

static void pid_deal_init(void)
{
    pid_init(pid_site,pid_menu_deal_f,pid_new,pid_new,pid_max);
}


static result_e pid_deal()
{
    return  pid_deal_s(pid_site,pid_menu_deal_f,pid_new,&pid_new,pid_max);
}



extern char wheel_buff_KP[5] ;
extern char wheel_buff_KD[5] ;
extern char speed_buff_KP[5] ;
extern char speed_buff_KI[5] ;
extern char speed_buff_KD[5] ;


#define wheel_char_KP  (FLASH_SECTOR_NUM-1)                             //wheel_KP 
#define wheel_char_KD  (FLASH_SECTOR_NUM-2)                             //wheel_KD
#define speed_char_KP  (FLASH_SECTOR_NUM-3)                             //KP 加上编码器时 用的PID
#define speed_char_KI  (FLASH_SECTOR_NUM-4)                             //KI 
#define speed_char_KD  (FLASH_SECTOR_NUM-5)                             //KD

#endif