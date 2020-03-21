
#include "key.h"


void pid_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab)
{
    pid_display(site,menu,max_tab ,new_tab,max_tab);
}


void pid_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab)
{
    //定义背景延时
#define SELECT_BG         GRED   //当前选中，而且没有改变
#define NO_SELECT_BG      BLACK     //没有选中，而且没有改变（普通的就是这样）

    //定义文字颜色
#define SELECT_FC            WHITE    //当前选中，而且没有改变
#define NO_SELECT_FC         WHITE   //没有选中，而且改变了

    uint8  i = 0;
    uint16 bkColor;
    uint16 Color;
    Site_t  sitetmp;
    sitetmp.y =  site.y;
  
    if(tab == max_tab)      //显示全部
    {
        i = max_tab - 1;    //循环的次数
        tab = 0;
    }

    do
    {
        if(tab == new_tab)
        {

            Color   =  SELECT_FC;
            bkColor =  SELECT_BG;

        }
        else
        {
            Color   =  NO_SELECT_FC;
            bkColor =  NO_SELECT_BG;
        }


        //显示文字
        //LCD_num_C(VAR_SITE(tab), VAR_VALUE(tab), Color, bkColor);
        sitetmp.x = site.x +  tab * 96 ;
        LCD_FSTR_CH_FUNC(sitetmp,menu[tab].strbuff,menu[tab].strnum,Color,bkColor);

        tab++;
    }
    while(i--);         //tab != VAR_MAX 的时候，执行一次就跳出
}

float PID = 4.1f;
result_e     pid_deal_s(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab)
{
    KEY_MSG_t keymsg;
    
    while(get_key_msg(&keymsg) == 1)
    {
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_D:
                {
                  PID += 0.1;
                    break;
                }
            case KEY_U:
                {
                  PID -= 0.1;
                    break;
                }
            case KEY_A:
                {
                    return RESULT_INIT;
                }
            case KEY_B:
                {
                    return menu[*new_tab].pfunc();
                }
            case KEY_L:
                {
                   if(*new_tab == 0)
                    {
                        *new_tab = max_tab - 1;
                        pid_display(site,menu,0 ,*new_tab,max_tab);
                    }
                    else
                    {
                        (*new_tab)--;
                        pid_display(site,menu,*new_tab + 1 ,*new_tab,max_tab);
                    }
                    pid_display(site,menu,*new_tab ,*new_tab,max_tab);
                   break;
                }
             case KEY_R:
               {
                  if(*new_tab < max_tab-1)
                    {
                        (*new_tab)++;
                        pid_display(site,menu,*new_tab - 1 ,*new_tab,max_tab);
                    }
                    else
                    {
                        *new_tab=0;
                        pid_display(site,menu,max_tab - 1 ,*new_tab,max_tab);
                    }
                    pid_display(site,menu,*new_tab ,*new_tab,max_tab);
                  break;
               }
                

            default:
                break;

            }

        }
    }

    return RESULT_NULL;
}







































//#include "key.h"
//#include "VCAN_key.h"
//
//#include "common.h"
//#include "VCAN_LCD.h"                                         //上面的 #include "common.h" 一定要带着不然会报错
//#include "stdio.h"
//#include "VCAN_LCD_CH.h"  
//#include "MK60_flash.h"
//
//
//#include "steer.h"
////extern float  BasicP; //基本的P值
////extern float  BasicD; //基本的D值
////extern float  J;//调节p和偏差的关系，越大，作用越强
////extern float  JD;//=0.0400;//调节p和偏差的关系，越大，作用越强
//
//char buff_KD[10];
//char buff_KP[10];
//
//
//
//uint8 flag = 1;
//
//   Site_t sign_1 = {5, 95};
//   Site_t sign_2 = {5, 110};
//   Site_t sign_3 = {80, 95};
//   Site_t sign_4 = {80, 110};
//   Site_t sign_5 = {85, 95};
//   Site_t sign_6 = {85, 110};
//   Site_t KP_s = {25, 95};
//   Site_t KD_s = {25, 110};
//   Site_t kp = {40,95};
//   Site_t kd = {40, 110};
// 
//void changePID()
//{ 
////   KP = flash_read(SECTOR_KP, 0, uint16) * 0.01;
////   KD = flash_read(SECTOR_KD, 0, uint16);  
//   sprintf((char *)buff_KP, "%4.1f",KP);
//   sprintf((char *)buff_KD, "%4.1f",KD);
//   if (flag == 1) 
//   {
//     //LCD_clear(BLACK);
//     LCD_str(sign_1, ">", WHITE,BLACK);
//     LCD_str(sign_2, " ", WHITE,BLACK);
//     LCD_str(sign_3, " ", WHITE,BLACK);
//     LCD_str(sign_4, " ", WHITE,BLACK);
//   }
//   
//   if (flag == 2)
//   {
//     //LCD_clear(BLACK);
//     LCD_str(sign_1, " ", WHITE,BLACK);
//     LCD_str(sign_2, ">", WHITE,BLACK);
//     LCD_str(sign_3, " ", WHITE,BLACK);
//     LCD_str(sign_4, " ", WHITE,BLACK);
//   }
//   
//   if (flag == 3)
//   {
//     LCD_str(sign_1, ">", WHITE,BLACK);
//     LCD_str(sign_2, " ", WHITE,BLACK);
//     LCD_str(sign_3, "<", WHITE,BLACK);
//     LCD_str(sign_4, " ", WHITE,BLACK);
//   }
//   
//   
//   if (flag == 4)
//   {
//     LCD_str(sign_1, " ", WHITE,BLACK);
//     LCD_str(sign_2, ">", WHITE,BLACK);
//     LCD_str(sign_3, " ", WHITE,BLACK);
//     LCD_str(sign_4, "<", WHITE,BLACK);
//   }
//   
//   
//   
//   LCD_str(KP_s, "KP", WHITE,BLACK);
//   LCD_str(KD_s, "KD", WHITE,BLACK);
//   LCD_Str_ENCH(kp, (uint8 const *)buff_KP, WHITE,BLACK);
//   LCD_Str_ENCH(kd, (uint8 const *)buff_KD, WHITE,BLACK);
//}
//
//
//
//void PORTD_IRQHandler(void)
//{
//    //PORT_FUNC(D,7,key_save);
//    PORT_FUNC(D,10,key_up);
//    PORT_FUNC(D,11,key_left);
//    PORT_FUNC(D,12,key_right);
//    PORT_FUNC(D,13,key_middle);
//    PORT_FUNC(D,14,key_down);
//}
//
//
//void key_up()
//{
////   flash_erase_sector(SECTOR_KP);
////   KP += 0.10;
////   flash_write(SECTOR_KP, 0, (int)(KP*100));
//  
//  if (flag == 2)
//  {
//    flag = 1;
//  }
//  
//  if (flag == 3)
//  {
//    flash_erase_sector(SECTOR_KP);
//    KP += 0.10;
//    flash_write(SECTOR_KP, 0, (int)(KP*100));
//     
//  }
//  
//  if (flag == 4)
//  {
//    flash_erase_sector(SECTOR_KD);
//    KD += 1.0;
//     flash_write(SECTOR_KD, 0, (int)KD);
//  }
//}
//
//void key_left()
//{
//  if (flag == 3)
//  {
//    flag = 1;
//  }
//  
//  if (flag == 4)
//  {
//    flag = 2;
//  }
//}
//
//void key_right()
//{
//  
////  if (flag == 3)
////  {
////    flash_erase_sector(SECTOR_KP);
////    flash_write(SECTOR_KP, 0, (int)(KP*100));
////    LCD_str(sign_5, "<", WHITE,BLACK);
////    DELAY_MS(100);
////    LCD_str(sign_5, " ", WHITE,BLACK);
////  }
////  
////  if (flag == 4)
////  {
////    flash_erase_sector(SECTOR_KD);
////    flash_write(SECTOR_KD, 0, (int)(KP*100));
////    LCD_str(sign_6, "<", WHITE,BLACK);
////    DELAY_MS(100);
////    LCD_str(sign_6, " ", WHITE,BLACK);
////  }
//  if (flag == 1)
//  {
//    flag = 3;
//  }
//  
//  if (flag == 2)
//  {
//    flag = 4;
//  }
//}
//
//void key_down()
//{
//  if (flag == 1)
//  {
//    flag += 1;
//  }
//  
//  
//  
//  if (flag == 3)
//  {
//    flash_erase_sector(SECTOR_KP);
//    KP -= 0.10;
//     flash_write(SECTOR_KP, 0, (int)(KP*100));
//  }
//  
//  if (flag == 4)
//  {
//    flash_erase_sector(SECTOR_KD);
//    KD -= 1.0;
//     flash_write(SECTOR_KD, 0, (int)KD);
//  }
//  
//
//}
//
//
//void key_middle()
//{
////   flash_erase_sector(SECTOR_KP);
////   flash_erase_sector(SECTOR_KD);
////   flash_write(SECTOR_KP, 0, (int)(KP*100));
////   flash_write(SECTOR_KD, 0, (int)KD);
//  
//   KP = flash_read(SECTOR_KP, 0, uint16) * 0.01;
//   KD = flash_read(SECTOR_KD, 0, uint16);
//}
//
////void key_save()
////{
////   flash_erase_sector(SECTOR_KP);
////   flash_erase_sector(SECTOR_KD);
////   flash_write(SECTOR_KP, 0, (int)(KP*100));
////   flash_write(SECTOR_KD, 0, (int)KD);
////}