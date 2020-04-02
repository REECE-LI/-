#include "camera_show.h"

#include "OTSU.h"
#include "camera_show.h"
#include "common.h"
#include  "MK60_PIT.h" 

//摄像头的中断
//---------------------------------------------------------------------
void PORTA_IRQHandler()
{
    uint32 flag;
  
    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    VSYNC();
}
void DMA0_IRQHandler()
{ 
  
    DMA_IRQ_CLEAN(DMA_CH0);           //清除通道传输中断标志位  
    row_finished();
    
}


//-----------------------------------------------------------------------------


    Size_t size = {128,90};          //W = 128,H = 90
    Site_t site     = {0, 18};                           //显示图像左上角位置
    
    Size_t imgsize  = {188, 120};                       //图像大小
    
    uint8 LeftLose, RightLose, AllLose;
    uint8 LeftEdge[ROW], RightEdge[ROW];
    uint8  imagebuff[ROW][COL];      //图像二值化数组
    uint8  middleline[ROW];          //存储中线的数组
//------------------------------------------------------------------------------
void camera_search()
{
  
  uint8 i, j, k;
 // uint32 t;
  //pit_time_start(PIT3); 
  //DELAY_MS(50);
  
  k = MyOSTU(COL, ROW, *image);                                           //4.423ms
  //t = pit_time_get_us(PIT3);
 
     if( mt9v032_finish_flag )
     {
       for(i=0;i<ROW;i++)
        {             
           for(j=0;j<COL;j++)            
              {
                if(image[i][j] >= k)
                  {
                    imagebuff[i][j]=255;
                  }
                else
                  {
                      imagebuff[i][j]=0;
                  }
             
               }
             
          }
        mt9v032_finish_flag = 0;
    
//            LCD_Img_gray_Z(site, size, *imagebuff, imgsize);
//            show_middleline();
     }
}


void search_middleline(void)
{
  uint8 i, j, jj;
  uint8  Width[ROW];
  Site_t site_middle = {0, 0};
  uint8 middleLine = 94;
  

  LeftLose = 0;
  RightLose = 0;
  AllLose = 0;
  
  
  
   for(i=0; i < ROW; i++)//赋初值             
  { 
       LeftEdge[i] = 0;
       RightEdge[i] = COL-1;
       middleline[i] = COL/2;
       Width[i] = i*2-60;     //动态路宽
      
  }
  
for(i = ROW-1; i >= 90; i--)                                                //首先找前三十行，全行扫描 先扫画面的前1/4
  {
    if(i == ROW-1)                                                              //首行就以图像中心作为扫描起点
      {
         j = middleLine;                                                        //第一行以 94 为中点
      } 
    else
      {
          j = middleline[i+1];                                                  //否则就以上一行中点的位置作为本行扫描起点
      }     
    if(j <= 2)
      {
          j = 2;                                                                  //防止j溢出
      }      
    while(j >= 2)//j>=2有效范围内进行搜寻 
      {
          if(imagebuff[i][j] == 255 && imagebuff[i][j-1] == 255 && imagebuff[i][j-2] == 0)//从右向左找到白白黑跳变 
            {
                 LeftEdge[i] = j;                                               //找到则赋值 找不到保持原值0      
                 break;                                                         //跳出本行寻线
            }
               j--;                                                             //列数往左移动
       }
    
     if(i == ROW-1)                                                             //再找右边界
       {
          j = middleLine;                                                       //如果首行，从图像中心开始搜寻
       }          
     else
       {
          j = middleline[i+1];                                                  //否则从上一行中心位置开始搜寻
       }
    
    
     if(j >= COL-2)                                                             //j >=ColumnMax-2有效范围内搜寻右线
       {
          j = COL-2;
       }
    
     while(j <= COL-2)
       {
           
          if(imagebuff[i][j] == 255 && imagebuff[i][j+1] == 255 && imagebuff[i][j+2] == 0)//从左向右找到白白黑跳变点
          {
                 RightEdge[i] = j;                                                //找到则赋值   找不到保持原值
                 break;                                                           //跳出本行寻线
          }
                 j++;                                                             //列数往右移动
       }
    
    
    
    
//
//
//      判断是否丢线
//
     if(LeftEdge[i] != 0 && RightEdge[i] != COL)                                //中线判断，没有丢线
       {
            middleline[i] = (LeftEdge[i] + RightEdge[i])/2;  
       }
       
     else if(LeftEdge[i] != 0 && RightEdge[i] == COL)//丢了右线,没有丢左线
       {
            RightLose++;//记录只有右线丢的数量
            
            //如果当行的

          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//突变
            {
                    middleline[i] = middleline[i+1] + 10;
            }
          else 
          {
                  middleline[i] = LeftEdge[i] + Width[i]/2 + 10;//正常的话就用半宽补
          }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] != COL)//丢了左线
       {
             LeftLose++;
             
          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//突变      
            {
                    middleline[i] = middleline[i+1] - 10; 
            } 
          else 
            {
                    middleline[i] = RightEdge[i] - Width[i]/2 - 10;//线宽
            }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] == COL)//两边都丢了的话  
       {
           AllLose++;
               
          if(i == ROW-1)//如果是首行就以图像中心作为中点
            {
                    middleline[i] = middleLine;
            }       
          else 
            {
                    middleline[i] = middleline[i+5];//如果不是首行就用上一行的中线作为本行中点
            }             
        } 
    
    
    
     if(middleline[ROW-1] >= 126)
       {
           middleLine = 126;
       }          
     else if(middleline[ROW-1] <= 63)
       {
           middleLine = 63;
       }         
     else
       {
            middleLine = middleline[ROW-1];//记录本帧图像第59行的中线值，作为下一幅图像的59行扫描起始点
       }
    
//           site_middle.y = i * 3/4;
//           site_middle.x = (int)(middleline[i] * 0.68);
//           LCD_point(site_middle, RED);
    
    
  }   
  
  
  
//  
//  
//-----------------------沿边扫描----------------------------------------------------------------
//  
  
  
  for(i=ROW-30; i>0; i--)//查找剩余行
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=COL) //上一行两边都找到 启用边沿扫描     
   {
         j = ((LeftEdge[i+1]+13) >= COL-2) ? COL-2:(LeftEdge[i+1]+13);//先找左边界    
         jj = ((LeftEdge[i+1]-13) <= 1)? 1:(LeftEdge[i+1]-13);        
      while(j >= jj)       
      {       
          if(imagebuff[i][j]==255 && imagebuff[i][j-1]==255&& imagebuff[i][j-2]==0)  
          {
               LeftEdge[i] = j;
               break;
          }   
         j--;   
      }
         j = ((RightEdge[i+1]-13) <= 1)? 1:(RightEdge[i+1]-13); //在找右边界   
         jj = ((RightEdge[i+1]+13) >= COL-2)? COL-2:(RightEdge[i+1]+13);    
      while(j <= jj)             
      {
          if(imagebuff[i][j]==255 && imagebuff[i][j+1]==255 && imagebuff[i][j+2]==0) 
          {
               RightEdge[i] = j;
               break;    
          }
         j++;
      }
    }
    
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==COL)//上一行只找到左边界   
    {      
         j  = ((LeftEdge[i+1]+13) >=COL-2)? COL-2:(LeftEdge[i+1]+13);//左边界用边沿扫描   
         jj = ((LeftEdge[i+1]-13) <= 1)? 1:(LeftEdge[i+1]-13);              
      while(j >= jj)   
      {     
          if(imagebuff[i][j]==255 && imagebuff[i][j-1]==255 && imagebuff[i][j-2]==0)
          {
               LeftEdge[i] = j;
               break;
          }
         j--;   
      }     
         j = middleline[i+1];//上一行丢了右边界用全行扫描 
      if(j >= COL-2)
      {
           j = COL-2;
      }
      while(j <= COL-2)      
      {    
          if(imagebuff[i][j]==255 && imagebuff[i][j+1]==255&&imagebuff[i][j+2]==0)
          {
               RightEdge[i] = j;
               break;
          }
         j++;
      }  
      
    }
    
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=COL) //上一行只找到右边界      
    {
         j = ((RightEdge[i+1]-13) <= 1)?1:(RightEdge[i+1]-13);//边缘追踪找右边界 
         jj = ((RightEdge[i+1]+13) >= COL-2)? COL-2:(RightEdge[i+1]+13);            
       while(j <= jj)  
       {    
            if(imagebuff[i][j]==255 && imagebuff[i][j+1]==0 && imagebuff[i][j+2]==0)
            {
                 RightEdge[i] = j;
                 break;
            }
           j++;     
       }   
           j = middleline[i+1]; //全行扫描找左边界
        if(j < 2)
        {
             j = 2;
        }  
       while(j >= 2)  
       {  
            if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)       
            {
                 LeftEdge[i] = j;
                 break;
            }
           j--;      
       }           
     } 
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==COL)  //上一行没找到边界，可能是十字或者环形  
    {
        
          j = middleline[i+1];   //找全行找左边界
        while(j >= 2)  
        {
             if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)     
             {
                  LeftEdge[i] = j;
                  break;
             }
            j--;
        }
         j = middleline[i+1];   //全行找右边界   
       while(j <=COL-2)       
       {   
            if(imagebuff[i][j]==255&&imagebuff[i][j+1]==0&& imagebuff[i][j+2]==0)
            {
                  RightEdge[i] = j;  
                  break;
            }
           j++;
              
       }         
    }
    
    
    
    
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+13) )//不满足畸变 
    {      
          middleline[i] = middleline[i+5];//用上一行
    }
 
    
    else
    {
            if(LeftEdge[i]!=0 && RightEdge[i]!=COL)   
            {
                     middleline[i] = (LeftEdge[i] + RightEdge[i])/2 ;
                     //对斜出十字进行纠正
                     
//                 if( middleline[i]-middleline[i+1]>10&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)) )&& i>=60)//中线向右突变
//                 {
//                           uint8 ii = i;
//                    
//                      while(1)
//                      {
//                               middleline[ii+1] = middleline[ii]-3; 
//                               ii++; 
//                               
//                           if( ii>=100 || (middleline[ii]-middleline[ii+1]<=1) )
//                           {
//                                 break;
//                           }          
//                       }
//                 }
//                 if( (middleline[i+1]-middleline[i]>10)&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)))&& i>=60)
//                 {
//                           uint8 ii = i;
//                          
//                          while(1)
//                      {
//                               middleline[ii+1] = middleline[ii]+3;
//                               ii++;
//                               
//                           if( ii>=100 || (middleline[ii+1]-middleline[ii]<=1) )
//                           { 
//                                 break;
//                           }       
//                       }
//                  }
            }
            else if(LeftEdge[i]!=0 && RightEdge[i]==COL)//find left
            { 
                         RightLose++;
                                              
                 if(LeftEdge[i+1] != 0)
                 {
                       middleline[i] = middleline[i+1] + LeftEdge[i] - LeftEdge[i+1];
                 }        
                 else
                 {
                       middleline[i]  = middleline[i] + Width[i]/2+5;
                 }
            }
            
            else if(LeftEdge[i]==0 && RightEdge[i]!=COL)//find right
            {
                          LeftLose++;
                          
                      
                 if(RightEdge[i+1] !=COL)
                 {                  
                                 middleline[i] = middleline[i+1]+RightEdge[i]-RightEdge[i+1];
                 }
                 else
                 {
                 
                                 middleline[i] = RightEdge[i] - Width[i]/2-5;
                 }
            }
           else if(LeftEdge[i]==0 && RightEdge[i]==COL)//两边丢线    
           {      
                        
                         AllLose++;
                
                   
                        middleline[i] = middleline[i+5];
           }
            
       }
       
    
    
//           site_middle.y = i * 3/4;
//           site_middle.x = (int)(middleline[i] * 0.68);
//           LCD_point(site_middle, RED);
  
  }

}


void show_middleline(void)
{
  uint8 i, j, LeftLose, RightLose, AllLose, jj;
  uint8 LeftEdge[COL], RightEdge[COL], Width[ROW];
  Site_t site_middle = {0, 0};
  uint8 middleLine = 94;
  

  LeftLose = 0;
  RightLose = 0;
  AllLose = 0;
  
  
  LCD_Img_gray_Z(site, size, *imagebuff, imgsize);
   for(i=0; i < ROW; i++)//赋初值             
  { 
       LeftEdge[i] = 0;
       RightEdge[i] = COL;
       middleline[i] = COL/2;
       Width[i] = i*2-60;     //动态路宽
      
  }
  
for(i = ROW-1; i >= 90; i--)                                                //首先找前三十行，全行扫描 先扫画面的前1/4
  {
    if(i == ROW-1)                                                              //首行就以图像中心作为扫描起点
      {
         j = middleLine;                                                        //第一行以 94 为中点
      } 
    else
      {
          j = middleline[i+1];                                                  //否则就以上一行中点的位置作为本行扫描起点
      }     
    if(j <= 2)
      {
          j = 2;                                                                  //防止j溢出
      }      
    while(j >= 2)//j>=2有效范围内进行搜寻 
      {
          if(imagebuff[i][j] == 255 && imagebuff[i][j-1] == 0 && imagebuff[i][j-2] == 0)//从右向左找到白白黑跳变 
            {
                 LeftEdge[i] = j;                                               //找到则赋值 找不到保持原值0      
                 break;                                                         //跳出本行寻线
            }
               j--;                                                             //列数往左移动
       }
    
     if(i == ROW-1)                                                             //再找右边界
       {
          j = middleLine;                                                       //如果首行，从图像中心开始搜寻
       }          
     else
       {
          j = middleline[i+1];                                                  //否则从上一行中心位置开始搜寻
       }
    
    
     if(j >= COL-2)                                                             //j >=ColumnMax-2有效范围内搜寻右线
       {
          j = COL-2;
       }
    
     while(j <= COL-2)
       {
           
          if(imagebuff[i][j] == 255 && imagebuff[i][j+1] == 0 && imagebuff[i][j+2] == 0)//从左向右找到白白黑跳变点
          {
                 RightEdge[i] = j;                                                //找到则赋值   找不到保持原值
                 break;                                                           //跳出本行寻线
          }
                 j++;                                                             //列数往右移动
       }
    
    
    
    
//
//
//      判断是否丢线
//
     if(LeftEdge[i] != 0 && RightEdge[i] != COL)                                //中线判断，没有丢线
       {
            middleline[i] = (LeftEdge[i] + RightEdge[i])/2;  
       }
       
     else if(LeftEdge[i] != 0 && RightEdge[i] == COL)//丢了右线,没有丢左线
       {
            RightLose++;//记录只有右线丢的数量
            
            //如果当行的

          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//突变
            {
                    middleline[i] = middleline[i+1] + 10;
            }
          else 
          {
                  middleline[i] = LeftEdge[i] + Width[i]/2 + 10;//正常的话就用半宽补
          }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] != COL)//丢了左线
       {
             LeftLose++;
             
          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//突变      
            {
                    middleline[i] = middleline[i+1] - 10; 
            } 
          else 
            {
                    middleline[i] = RightEdge[i] - Width[i]/2 - 10;//线宽
            }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] == COL)//两边都丢了的话  
       {
           AllLose++;
               
          if(i == ROW-1)//如果是首行就以图像中心作为中点
            {
                    middleline[i] = middleLine;
            }       
          else 
            {
                    middleline[i] = middleline[i+5];//如果不是首行就用上一行的中线作为本行中点
            }             
        } 
    
    
    
     if(middleline[ROW-1] >= 126)
       {
           middleLine = 126;
       }          
     else if(middleline[ROW-1] <= 63)
       {
           middleLine = 63;
       }         
     else
       {
            middleLine = middleline[ROW-1];//记录本帧图像第59行的中线值，作为下一幅图像的59行扫描起始点
       }
    
           site_middle.y = (uint8)(i * 0.75) + 18;
           site_middle.x = (uint8)(middleline[i] * 0.68);
           LCD_point(site_middle, BLUE);
    
    
  }   
  
  
  
//  
//  
//-----------------------沿边扫描----------------------------------------------------------------
//  
  
  
  for(i=ROW-30; i>0; i--)//查找剩余行
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=COL) //上一行两边都找到 启用边沿扫描     
   {
         j = ((LeftEdge[i+1]+13) >= COL-2) ? COL-2:(LeftEdge[i+1]+13);//先找左边界    
         jj = ((LeftEdge[i+1]-13) <= 1)? 1:(LeftEdge[i+1]-13);        
      while(j >= jj)       
      {       
          if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0&& imagebuff[i][j-2]==0)  
          {
               LeftEdge[i] = j;
               break;
          }   
         j--;   
      }
         j = ((RightEdge[i+1]-13) <= 1)? 1:(RightEdge[i+1]-13); //在找右边界   
         jj = ((RightEdge[i+1]+13) >= COL-2)? COL-2:(RightEdge[i+1]+13);    
      while(j <= jj)             
      {
          if(imagebuff[i][j]==255 && imagebuff[i][j+1]==0 && imagebuff[i][j+2]==0) 
          {
               RightEdge[i] = j;
               break;    
          }
         j++;
      }
    }
    
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==COL)//上一行只找到左边界   
    {      
         j  = ((LeftEdge[i+1]+13) >=COL-2)? COL-2:(LeftEdge[i+1]+13);//左边界用边沿扫描   
         jj = ((LeftEdge[i+1]-13) <= 1)? 1:(LeftEdge[i+1]-13);              
      while(j >= jj)   
      {     
          if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)
          {
               LeftEdge[i] = j;
               break;
          }
         j--;   
      }     
         j = middleline[i+1];//上一行丢了右边界用全行扫描 
      if(j >= COL-2)
      {
           j = COL-2;
      }
      while(j <= COL-2)      
      {    
          if(imagebuff[i][j]==255 && imagebuff[i][j+1]==0&&imagebuff[i][j+2]==0)
          {
               RightEdge[i] = j;
               break;
          }
         j++;
      }  
      
    }
    
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=COL) //上一行只找到右边界      
    {
         j = ((RightEdge[i+1]-13) <= 1)?1:(RightEdge[i+1]-13);//边缘追踪找右边界 
         jj = ((RightEdge[i+1]+13) >= COL-2)? COL-2:(RightEdge[i+1]+13);            
       while(j <= jj)  
       {    
            if(imagebuff[i][j]==255 && imagebuff[i][j+1]==0 && imagebuff[i][j+2]==0)
            {
                 RightEdge[i] = j;
                 break;
            }
           j++;     
       }   
           j = middleline[i+1]; //全行扫描找左边界
        if(j < 2)
        {
             j = 2;
        }  
       while(j >= 2)  
       {  
            if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)       
            {
                 LeftEdge[i] = j;
                 break;
            }
           j--;      
       }           
     } 
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==COL)  //上一行没找到边界，可能是十字或者环形  
    {
        
          j = middleline[i+1];   //找全行找左边界
        while(j >= 2)  
        {
             if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)     
             {
                  LeftEdge[i] = j;
                  break;
             }
            j--;
        }
         j = middleline[i+1];   //全行找右边界   
       while(j <=COL-2)       
       {   
            if(imagebuff[i][j]==255&&imagebuff[i][j+1]==0&& imagebuff[i][j+2]==0)
            {
                  RightEdge[i] = j;  
                  break;
            }
           j++;
              
       }         
    }
    
    
    
    
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+13) )//不满足畸变 
    {      
          middleline[i] = middleline[i+5];//用上一行
    }
 
    
    else
    {
            if(LeftEdge[i]!=0 && RightEdge[i]!=COL)   
            {
                     middleline[i] = (LeftEdge[i] + RightEdge[i])/2 ;
                     //对斜出十字进行纠正
                     
//                 if( middleline[i]-middleline[i+1]>10&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)) )&& i>=60)//中线向右突变
//                 {
//                           uint8 ii = i;
//                    
//                      while(1)
//                      {
//                               middleline[ii+1] = middleline[ii]-3; 
//                               ii++; 
//                               
//                           if( ii>=100 || (middleline[ii]-middleline[ii+1]<=1) )
//                           {
//                                 break;
//                           }          
//                       }
//                 }
//                 if( (middleline[i+1]-middleline[i]>10)&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)))&& i>=60)
//                 {
//                           uint8 ii = i;
//                          
//                          while(1)
//                      {
//                               middleline[ii+1] = middleline[ii]+3;
//                               ii++;
//                               
//                           if( ii>=100 || (middleline[ii+1]-middleline[ii]<=1) )
//                           { 
//                                 break;
//                           }       
//                       }
//                  }
            }
            else if(LeftEdge[i]!=0 && RightEdge[i]==COL)//find left
            { 
                         RightLose++;
                                              
                 if(LeftEdge[i+1] != 0)
                 {
                       middleline[i] = middleline[i+1] + LeftEdge[i] - LeftEdge[i+1];
                 }        
                 else
                 {
                       middleline[i]  = middleline[i] + Width[i]/2+5;
                 }
            }
            
            else if(LeftEdge[i]==0 && RightEdge[i]!=COL)//find right
            {
                          LeftLose++;
                          
                      
                 if(RightEdge[i+1] !=COL)
                 {                  
                                 middleline[i] = middleline[i+1]+RightEdge[i]-RightEdge[i+1];
                 }
                 else
                 {
                 
                                 middleline[i] = RightEdge[i] - Width[i]/2-5;
                 }
            }
           else if(LeftEdge[i]==0 && RightEdge[i]==COL)//两边丢线    
           {      
                        
                         AllLose++;
                
                   
                        middleline[i] = middleline[i+5];
           }
            
       }
       
    
    
           site_middle.y = (uint8)(i * 0.75) + 18;
           site_middle.x = (uint8)(middleline[i] * 0.68);
           LCD_point(site_middle, BLUE);
  
  }

}

