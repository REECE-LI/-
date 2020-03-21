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
    if(PIT_TFLG(PIT0) == 1 )        //�ж��Ƿ� PIT0 �����ж�
    {
        PIT_Flag_Clear(PIT0);                   //���жϱ�־λ

        //�������û����ʵ�ִ���
        key_IRQHandler();                       //�Ѱ���ɨ�������뵽��ʱ�жϸ�λ�������ʱִ��

    }
}


 void  main(void)
{
 
////-------------------------flash�ĳ�ʼ��----------------------------    
      flash_init();                                       //��ʼ��flash   
                                                         
    
//---------------------��������������-----------------------------------------------------
    key_init(KEY_MAX);
    pit_init_ms(PIT0,10);                               // pit��ʱ 10ms
    set_vector_handler(PIT0_VECTORn,pit_hander);         // �����жϷ��������ж���������
    enable_irq(PIT0_IRQn);                               // ʹ��pit�ж�
//---------------------------------------------------------------------

//-----------------����ͷ����������----------------------------------
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);               //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler  
    camera_zhufei_init() ;
    enable_irq(PORTA_IRQn);
    
 //------------------------------------------------------------------------------
    LCD_init();
    wheel_init();

    while(1)
    {
        main_menu_task();       //�˵�����        
    }


}



