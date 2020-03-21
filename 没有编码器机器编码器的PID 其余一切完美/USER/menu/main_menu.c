//可供参考的菜单函数模版

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
    while(get_key_msg(&keymsg) == 1);      //清空按键消息

    LCD_clear(BLACK);                     //清屏

    main_menu_init();                       //菜单初始化
    while(1)
    {
        result = main_menu_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return ;      //直接退出后，会重新进来
        }
        else if(result == RESULT_TO_MENU )
        {
            goto MAIN_INIT;
        }
    }
}




/**********************************************       主程序       ************************************************/

result_e main_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置

    //uint32 t;
    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,main_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】

    while(1)
    {
        //任务循环执行内容【用户自行补充】
  //    pit_time_start(PIT3);
         camera_search();
         search_middleline();
      //
         SteerControl();
//t = pit_time_get_us(PIT3);
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】

              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

/**********************************************       轮胎PID控制       ************************************************/

result_e wheel_func(void)
{
   // Site_t site     = {0, 0};                           //显示图像左上角位置

    result_e result;
    KEY_MSG_t keymsg;
MAIN_INIT:
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
 //   LCD_FSTR_CH(site,wheel_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】
    pid_menu_init();                       //菜单初始化
    while(1)
    {
        //任务循环执行内容【用户自行补充】
        result = pid_menu_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
        }
        else if(result == RESULT_TO_MENU )
        {
           goto MAIN_INIT;
        }

        //任务按键处理
//        while(get_key_msg(&keymsg) == 1)               // 按键处理
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
//           {
//              //任务退出处理【用户自行补充】
//
//              return  RESULT_TO_MENU;
//          }
//          //其他的按键处理【用户自行补充】
//          
//          
//      }
   }
}

/**********************************************       总钻风摄像头       ************************************************/

result_e cam_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置

    //uint32 t;
    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,cam_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】

    while(1)
    {
        //任务循环执行内容【用户自行补充】
          //camera_show();
    //  pit_time_start(PIT3);
          camera_search();
          show_middleline();
     //   t = pit_time_get_us(PIT3);
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】

              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

//-------------------------------------------------------调节KP---------------------------------------------------------------
result_e KP_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = speed_KP;
 MAIN_INIT:     
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,KP_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】
 pid_deal_init();
    while(1)
    {
        //任务循环执行内容【用户自行补充】
        sprintf((char *)speed_buff_KP, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KP, WHITE,BLACK);
        
        
            result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
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
        
        
        
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】
              
              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

/**********************************************       调节KI       ************************************************/

result_e KI_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Site_t site_1     = {50, 30}; 
    result_e result; 

    KEY_MSG_t keymsg;
    PID = speed_KI;
 MAIN_INIT:     
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,KI_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】
 pid_deal_init();
    while(1)
    {
        //任务循环执行内容【用户自行补充】
          sprintf((char *)speed_buff_KI, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KI, WHITE,BLACK);
          result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
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
        
        
        
        
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】
              
              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

/**********************************************       调节KD       ************************************************/

result_e KD_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
   Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = speed_KD;
MAIN_INIT: 
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,KD_func_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】
 pid_deal_init();
    while(1)
    {
        //任务循环执行内容【用户自行补充】
             sprintf((char *)speed_buff_KD, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)speed_buff_KD, WHITE,BLACK);
          result = pid_deal();
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
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
        
        
        
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】
                
              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}
//-----------------------------------------------------转向KP------------------------------------------
result_e wheel_KP(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Site_t site_1     = {50, 30}; 
    result_e result; 
    
    KEY_MSG_t keymsg;
    PID = KP;
MAIN_INIT: 
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,wheel_KP_name,WHITE,BLACK);     //显示菜单文字
    
      //任务初始化【用户自行补充】
 pid_deal_init();
    while(1)
    {
      
      sprintf((char *)wheel_buff_KP, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)wheel_buff_KP, WHITE,BLACK);
        //任务循环执行内容【用户自行补充】
        result = pid_deal();
        
         
        
        
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
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
          
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】
                 
              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

/**********************************************       转向KD       ************************************************/

result_e wheel_KD(void)
{
    
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Site_t site_1     = {50, 30};                           //显示PID的 位置坐标
    result_e result; 
    
    KEY_MSG_t keymsg;
    
    PID = KD;
 MAIN_INIT:  
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息
    
    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,wheel_KD_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】
    pid_deal_init();
    while(1)
    {
        //任务循环执行内容【用户自行补充】

        sprintf((char *)wheel_buff_KD, "%4.1f",PID);
        LCD_Str_ENCH(site_1, (uint8 const *)wheel_buff_KD, WHITE,BLACK);
        result = pid_deal();
     
        if(result == RESULT_INIT )
        {
            //LCD_init();
            return RESULT_TO_MENU;      //直接退出后，会重新进来
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
        
        
        
        
        
        //任务按键处理
        while(get_key_msg(&keymsg) == 1)               // 按键处理
       {
            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
           {
              //任务退出处理【用户自行补充】
              
              return  RESULT_TO_MENU;
          }
          //其他的按键处理【用户自行补充】
          
          
      }
   }
}

/**********************************************       读取       ************************************************/

result_e read_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置


    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,read_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】

    while(1)
    {
        //任务循环执行内容【用户自行补充】
          return READ;

        //任务按键处理
//        while(get_key_msg(&keymsg) == 1)               // 按键处理
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
//           {
//              //任务退出处理【用户自行补充】
//
//              return  RESULT_TO_MENU;
//          }
//          //其他的按键处理【用户自行补充】
//          
//          
//      }
   }
}

/**********************************************       写入       ************************************************/

result_e write_func(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置


    KEY_MSG_t keymsg;
    while(get_key_msg(&keymsg) == 1);                   //清空按键消息

    LCD_clear(BLACK);                                 //清屏
    LCD_FSTR_CH(site,write_name,WHITE,BLACK);     //显示菜单文字

      //任务初始化【用户自行补充】

    while(1)
    {
        //任务循环执行内容【用户自行补充】
//          flash_erase_sector(SECTOR_KP);
//          flash_write(SECTOR_KP, 0, (int)(KP*100));
      return SAVE;

        //任务按键处理
//        while(get_key_msg(&keymsg) == 1)               // 按键处理
//       {
//            if((keymsg.status == KEY_DOWN) && (keymsg.key == KEY_A ))       //A 键 按下时退出
//           {
//              //任务退出处理【用户自行补充】
//
//              return  RESULT_TO_MENU;
//          }
//          //其他的按键处理【用户自行补充】
//          
//          
//      }
   }
}

