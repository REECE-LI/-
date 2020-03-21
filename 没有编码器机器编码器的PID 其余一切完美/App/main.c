//
//
//
//
//
//

#include "VCAN_key.h"
#include "common.h"
#include "include.h"
#include "camera_show.h"
#include "SEEKFREE_MT9V032.h"
#include "steer.h"
#include "key.h"
#include "stdio.h"
#include "MK60_flash.h"
#include "main_menu.h"

extern  void wheel_init(void);


void pit_hander(void)
{ 
    if(PIT_TFLG(PIT0) == 1 )        //判断是否 PIT0 进入中断
    {
        PIT_Flag_Clear(PIT0);                   //清中断标志位

        //下面由用户添加实现代码
        key_IRQHandler();                       //把按键扫描程序加入到定时中断复位函数里，定时执行

    }
}


 void  main(void)
{
 
////-------------------------flash的初始化----------------------------    
      flash_init();                                       //初始化flash   
                                                         
    
//---------------------按键的启动函数-----------------------------------------------------
    key_init(KEY_MAX);
    pit_init_ms(PIT0,10);                               // pit定时 10ms
    set_vector_handler(PIT0_VECTORn,pit_hander);         // 设置中断服务函数到中断向量表里
    enable_irq(PIT0_IRQn);                               // 使能pit中断
//---------------------------------------------------------------------

//-----------------摄像头的启动函数----------------------------------
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);               //设置LPTMR的中断服务函数为 PORTA_IRQHandler  
    camera_zhufei_init() ;
    enable_irq(PORTA_IRQn);
    
 //------------------------------------------------------------------------------
    LCD_init();
    wheel_init();

    while(1)
    {
        main_menu_task();       //菜单函数        
    }


}



