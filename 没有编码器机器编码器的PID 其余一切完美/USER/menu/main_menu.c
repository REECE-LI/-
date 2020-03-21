//�ɹ��ο��Ĳ˵�����ģ��

#include "main_menu.h"
#include "camera_show.h"
#include "common.h"


#include "VCAN_lcd.h"

#include "key.h"
extern void SteerControl(void);

char wheel_buff_KP[5] ;
char wheel_buff_KD[5] ;
char speed_buff_KP[5] ;
char speed_buff_KI[5] ;
char speed_buff_KD[5] ;
void main_menu_task(void)
{
    result_e result;
    KEY_MSG_t keymsg;
MAIN_INIT:
    while(get_key_msg(&keymsg) == 1);      //��հ�����Ϣ

    LCD_clear(BLACK);                     //����

    main_menu_init();                       //�˵���ʼ��
    while(1)
    {
        result = main_menu_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return ;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
            goto MAIN_INIT;
        }
    }
}




/**********************************************       ������       ************************************************/

result_e main_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��

    //uint32 t;
    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,main_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿

    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
  //    pit_time_start(PIT3);
         camera_search();
         search_middleline();
      //
         SteerControl();
//t = pit_time_get_us(PIT3);
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿

              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

/**********************************************       ��̥PID����       ************************************************/

result_e wheel_func(void)
{
   // Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��

    result_e result;
    KEY_MSG_t keymsg;
MAIN_INIT:
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
 //   LCD_FSTR_CH(site,wheel_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿
    pid_menu_init();                       //�˵���ʼ��
    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
        result = pid_menu_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }

        //���񰴼�����
//        while(get_key_msg(&keymsg) == 1)               // ��������
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
//           {
//              //�����˳������û����в��䡿
//
//              return  RESULT_TO_MENU;
//          }
//          //�����İ��������û����в��䡿
//          
//          
//      }
   }
}

/**********************************************       ���������ͷ       ************************************************/

result_e cam_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��

    //uint32 t;
    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,cam_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿

    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
          //camera_show();
    //  pit_time_start(PIT3);
          camera_search();
          show_middleline();
     //   t = pit_time_get_us(PIT3);
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿

              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

//-------------------------------------------------------����KP---------------------------------------------------------------
result_e KP_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = speed_KP;
 MAIN_INIT:     
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,KP_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿
 pid_deal_init();
    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
        sprintf((char *)speed_buff_KP, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KP, WHITE,BLACK);
        
        
            result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }
        
        
         else if (result == SAVE)
        {
          flash_erase_sector(speed_char_KP);
          flash_write(speed_char_KP, 0, (int)(PID*100));
          goto MAIN_INIT;
        }
        else if (result == READ)
        {
          PID = flash_read(speed_char_KP, 0, uint16) * 0.01;
          speed_KP = PID;
          goto MAIN_INIT;
          
        }
        
        
        
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿
              
              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

/**********************************************       ����KI       ************************************************/

result_e KI_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Site_t site_1     = {50, 30}; 
    result_e result; 

    KEY_MSG_t keymsg;
    PID = speed_KI;
 MAIN_INIT:     
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,KI_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿
 pid_deal_init();
    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
          sprintf((char *)speed_buff_KI, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KI, WHITE,BLACK);
          result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }
        
        
        
             else if (result == SAVE)
        {
          flash_erase_sector(speed_char_KI);
          flash_write(speed_char_KI, 0, (int)(PID*100));
          goto MAIN_INIT;
        }
        else if (result == READ)
        {
          PID = flash_read(speed_char_KI, 0, uint16) * 0.01;
           speed_KI = PID;
          goto MAIN_INIT;
          
        }
        
        
        
        
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿
              
              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

/**********************************************       ����KD       ************************************************/

result_e KD_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
   Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = speed_KD;
MAIN_INIT: 
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,KD_func_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿
 pid_deal_init();
    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
             sprintf((char *)speed_buff_KD, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KD, WHITE,BLACK);
          result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }
        
        
         else if (result == SAVE)
        {
          flash_erase_sector(speed_char_KD);
          flash_write(speed_char_KD, 0, (int)(PID*100));
          goto MAIN_INIT;
        }
        else if (result == READ)
        {
          PID = flash_read(speed_char_KD, 0, uint16) * 0.01;
          speed_KD = PID;
          goto MAIN_INIT;
          
        }
        
        
        
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿
                
              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}
//-----------------------------------------------------ת��KP------------------------------------------
result_e wheel_KP(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = KP;
MAIN_INIT: 
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,wheel_KP_name,WHITE,BLACK);     //��ʾ�˵�����
    
      //�����ʼ�����û����в��䡿
 pid_deal_init();
    while(1)
    {
      
      sprintf((char *)wheel_buff_KP, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)wheel_buff_KP, WHITE,BLACK);
        //����ѭ��ִ�����ݡ��û����в��䡿
        result = pid_deal();
        
         
        
        
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }
        
         else if (result == SAVE)
        {
          flash_erase_sector(wheel_char_KP);
          flash_write(wheel_char_KP, 0, (int)(PID*100));
          goto MAIN_INIT;
        }
        else if (result == READ)
        {
          PID = flash_read(wheel_char_KP, 0, uint16) * 0.01;
          KP = PID;
          goto MAIN_INIT;
          
        }
          
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿
                 
              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

/**********************************************       ת��KD       ************************************************/

result_e wheel_KD(void)
{
    
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Site_t site_1     = {50, 30};                           //��ʾPID�� λ������
    result_e result; 
    
    KEY_MSG_t keymsg;
    
    PID = KD;
 MAIN_INIT:  
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ
    
    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,wheel_KD_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿
    pid_deal_init();
    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿

        sprintf((char *)wheel_buff_KD, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)wheel_buff_KD, WHITE,BLACK);
        result = pid_deal();
     
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //ֱ���˳��󣬻����½���
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }
        else if (result == SAVE)
        {
          flash_erase_sector(wheel_char_KD);
          flash_write(wheel_char_KD, 0, (int)(PID*100));
          goto MAIN_INIT;
        }
        else if (result == READ)
        {
          PID = flash_read(wheel_char_KD, 0, uint16) * 0.01;
          KD = PID;
          goto MAIN_INIT;
          
        }
        
        
        
        
        
        //���񰴼�����
        while(get_key_msg(&keymsg) == 1)               // ��������
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
           {
              //�����˳������û����в��䡿
              
              return  RESULT_TO_MENU;
          }
          //�����İ��������û����в��䡿
          
          
      }
   }
}

/**********************************************       ��ȡ       ************************************************/

result_e read_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��


    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,read_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿

    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
          return READ;

        //���񰴼�����
//        while(get_key_msg(&keymsg) == 1)               // ��������
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
//           {
//              //�����˳������û����в��䡿
//
//              return  RESULT_TO_MENU;
//          }
//          //�����İ��������û����в��䡿
//          
//          
//      }
   }
}

/**********************************************       д��       ************************************************/

result_e write_func(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��


    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //��հ�����Ϣ

    LCD_clear(BLACK);                                 //����
    LCD_FSTR_CH(site,write_name,WHITE,BLACK);     //��ʾ�˵�����

      //�����ʼ�����û����в��䡿

    while(1)
    {
        //����ѭ��ִ�����ݡ��û����в��䡿
//          flash_erase_sector(SECTOR_KP);
//          flash_write(SECTOR_KP, 0, (int)(KP*100));
      return SAVE;

        //���񰴼�����
//        while(get_key_msg(&keymsg) == 1)               // ��������
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A �� ����ʱ�˳�
//           {
//              //�����˳������û����в��䡿
//
//              return  RESULT_TO_MENU;
//          }
//          //�����İ��������û����в��䡿
//          
//          
//      }
   }
}

