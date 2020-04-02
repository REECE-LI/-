#include "camera_show.h"

#include "OTSU.h"
#include "camera_show.h"
#include "common.h"
#include  "MK60_PIT.h" 

//����ͷ���ж�
//---------------------------------------------------------------------
void PORTA_IRQHandler()
{
    uint32 flag;
  
    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    VSYNC();
}
void DMA0_IRQHandler()
{ 
  
    DMA_IRQ_CLEAN(DMA_CH0);           //���ͨ�������жϱ�־λ  
    row_finished();
    
}


//-----------------------------------------------------------------------------


    Size_t size = {128,90};          //W = 128,H = 90
    Site_t site     = {0, 18};                           //��ʾͼ�����Ͻ�λ��
    
    Size_t imgsize  = {188, 120};                       //ͼ���С
    
    uint8 LeftLose, RightLose, AllLose;
    uint8 LeftEdge[ROW], RightEdge[ROW];
    uint8  imagebuff[ROW][COL];      //ͼ���ֵ������
    uint8  middleline[ROW];          //�洢���ߵ�����
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
  
  
  
   for(i=0; i < ROW; i++)//����ֵ             
  { 
       LeftEdge[i] = 0;
       RightEdge[i] = COL-1;
       middleline[i] = COL/2;
       Width[i] = i*2-60;     //��̬·��
      
  }
  
for(i = ROW-1; i >= 90; i--)                                                //������ǰ��ʮ�У�ȫ��ɨ�� ��ɨ�����ǰ1/4
  {
    if(i == ROW-1)                                                              //���о���ͼ��������Ϊɨ�����
      {
         j = middleLine;                                                        //��һ���� 94 Ϊ�е�
      } 
    else
      {
          j = middleline[i+1];                                                  //���������һ���е��λ����Ϊ����ɨ�����
      }     
    if(j <= 2)
      {
          j = 2;                                                                  //��ֹj���
      }      
    while(j >= 2)//j>=2��Ч��Χ�ڽ�����Ѱ 
      {
          if(imagebuff[i][j] == 255 && imagebuff[i][j-1] == 255 && imagebuff[i][j-2] == 0)//���������ҵ��װ׺����� 
            {
                 LeftEdge[i] = j;                                               //�ҵ���ֵ �Ҳ�������ԭֵ0      
                 break;                                                         //��������Ѱ��
            }
               j--;                                                             //���������ƶ�
       }
    
     if(i == ROW-1)                                                             //�����ұ߽�
       {
          j = middleLine;                                                       //������У���ͼ�����Ŀ�ʼ��Ѱ
       }          
     else
       {
          j = middleline[i+1];                                                  //�������һ������λ�ÿ�ʼ��Ѱ
       }
    
    
     if(j >= COL-2)                                                             //j >=ColumnMax-2��Ч��Χ����Ѱ����
       {
          j = COL-2;
       }
    
     while(j <= COL-2)
       {
           
          if(imagebuff[i][j] == 255 && imagebuff[i][j+1] == 255 && imagebuff[i][j+2] == 0)//���������ҵ��װ׺������
          {
                 RightEdge[i] = j;                                                //�ҵ���ֵ   �Ҳ�������ԭֵ
                 break;                                                           //��������Ѱ��
          }
                 j++;                                                             //���������ƶ�
       }
    
    
    
    
//
//
//      �ж��Ƿ���
//
     if(LeftEdge[i] != 0 && RightEdge[i] != COL)                                //�����жϣ�û�ж���
       {
            middleline[i] = (LeftEdge[i] + RightEdge[i])/2;  
       }
       
     else if(LeftEdge[i] != 0 && RightEdge[i] == COL)//��������,û�ж�����
       {
            RightLose++;//��¼ֻ�����߶�������
            
            //������е�

          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//ͻ��
            {
                    middleline[i] = middleline[i+1] + 10;
            }
          else 
          {
                  middleline[i] = LeftEdge[i] + Width[i]/2 + 10;//�����Ļ����ð��
          }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] != COL)//��������
       {
             LeftLose++;
             
          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//ͻ��      
            {
                    middleline[i] = middleline[i+1] - 10; 
            } 
          else 
            {
                    middleline[i] = RightEdge[i] - Width[i]/2 - 10;//�߿�
            }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] == COL)//���߶����˵Ļ�  
       {
           AllLose++;
               
          if(i == ROW-1)//��������о���ͼ��������Ϊ�е�
            {
                    middleline[i] = middleLine;
            }       
          else 
            {
                    middleline[i] = middleline[i+5];//����������о�����һ�е�������Ϊ�����е�
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
            middleLine = middleline[ROW-1];//��¼��֡ͼ���59�е�����ֵ����Ϊ��һ��ͼ���59��ɨ����ʼ��
       }
    
//           site_middle.y = i * 3/4;
//           site_middle.x = (int)(middleline[i] * 0.68);
//           LCD_point(site_middle, RED);
    
    
  }   
  
  
  
//  
//  
//-----------------------�ر�ɨ��----------------------------------------------------------------
//  
  
  
  for(i=ROW-30; i>0; i--)//����ʣ����
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=COL) //��һ�����߶��ҵ� ���ñ���ɨ��     
   {
         j = ((LeftEdge[i+1]+13) >= COL-2) ? COL-2:(LeftEdge[i+1]+13);//������߽�    
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
         j = ((RightEdge[i+1]-13) <= 1)? 1:(RightEdge[i+1]-13); //�����ұ߽�   
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
    
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==COL)//��һ��ֻ�ҵ���߽�   
    {      
         j  = ((LeftEdge[i+1]+13) >=COL-2)? COL-2:(LeftEdge[i+1]+13);//��߽��ñ���ɨ��   
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
         j = middleline[i+1];//��һ�ж����ұ߽���ȫ��ɨ�� 
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
    
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=COL) //��һ��ֻ�ҵ��ұ߽�      
    {
         j = ((RightEdge[i+1]-13) <= 1)?1:(RightEdge[i+1]-13);//��Ե׷�����ұ߽� 
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
           j = middleline[i+1]; //ȫ��ɨ������߽�
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
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==COL)  //��һ��û�ҵ��߽磬������ʮ�ֻ��߻���  
    {
        
          j = middleline[i+1];   //��ȫ������߽�
        while(j >= 2)  
        {
             if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)     
             {
                  LeftEdge[i] = j;
                  break;
             }
            j--;
        }
         j = middleline[i+1];   //ȫ�����ұ߽�   
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
    
    
    
    
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+13) )//��������� 
    {      
          middleline[i] = middleline[i+5];//����һ��
    }
 
    
    else
    {
            if(LeftEdge[i]!=0 && RightEdge[i]!=COL)   
            {
                     middleline[i] = (LeftEdge[i] + RightEdge[i])/2 ;
                     //��б��ʮ�ֽ��о���
                     
//                 if( middleline[i]-middleline[i+1]>10&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)) )&& i>=60)//��������ͻ��
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
           else if(LeftEdge[i]==0 && RightEdge[i]==COL)//���߶���    
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
   for(i=0; i < ROW; i++)//����ֵ             
  { 
       LeftEdge[i] = 0;
       RightEdge[i] = COL;
       middleline[i] = COL/2;
       Width[i] = i*2-60;     //��̬·��
      
  }
  
for(i = ROW-1; i >= 90; i--)                                                //������ǰ��ʮ�У�ȫ��ɨ�� ��ɨ�����ǰ1/4
  {
    if(i == ROW-1)                                                              //���о���ͼ��������Ϊɨ�����
      {
         j = middleLine;                                                        //��һ���� 94 Ϊ�е�
      } 
    else
      {
          j = middleline[i+1];                                                  //���������һ���е��λ����Ϊ����ɨ�����
      }     
    if(j <= 2)
      {
          j = 2;                                                                  //��ֹj���
      }      
    while(j >= 2)//j>=2��Ч��Χ�ڽ�����Ѱ 
      {
          if(imagebuff[i][j] == 255 && imagebuff[i][j-1] == 0 && imagebuff[i][j-2] == 0)//���������ҵ��װ׺����� 
            {
                 LeftEdge[i] = j;                                               //�ҵ���ֵ �Ҳ�������ԭֵ0      
                 break;                                                         //��������Ѱ��
            }
               j--;                                                             //���������ƶ�
       }
    
     if(i == ROW-1)                                                             //�����ұ߽�
       {
          j = middleLine;                                                       //������У���ͼ�����Ŀ�ʼ��Ѱ
       }          
     else
       {
          j = middleline[i+1];                                                  //�������һ������λ�ÿ�ʼ��Ѱ
       }
    
    
     if(j >= COL-2)                                                             //j >=ColumnMax-2��Ч��Χ����Ѱ����
       {
          j = COL-2;
       }
    
     while(j <= COL-2)
       {
           
          if(imagebuff[i][j] == 255 && imagebuff[i][j+1] == 0 && imagebuff[i][j+2] == 0)//���������ҵ��װ׺������
          {
                 RightEdge[i] = j;                                                //�ҵ���ֵ   �Ҳ�������ԭֵ
                 break;                                                           //��������Ѱ��
          }
                 j++;                                                             //���������ƶ�
       }
    
    
    
    
//
//
//      �ж��Ƿ���
//
     if(LeftEdge[i] != 0 && RightEdge[i] != COL)                                //�����жϣ�û�ж���
       {
            middleline[i] = (LeftEdge[i] + RightEdge[i])/2;  
       }
       
     else if(LeftEdge[i] != 0 && RightEdge[i] == COL)//��������,û�ж�����
       {
            RightLose++;//��¼ֻ�����߶�������
            
            //������е�

          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//ͻ��
            {
                    middleline[i] = middleline[i+1] + 10;
            }
          else 
          {
                  middleline[i] = LeftEdge[i] + Width[i]/2 + 10;//�����Ļ����ð��
          }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] != COL)//��������
       {
             LeftLose++;
             
          if((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i+1] - LeftEdge[i+1] + 13))//ͻ��      
            {
                    middleline[i] = middleline[i+1] - 10; 
            } 
          else 
            {
                    middleline[i] = RightEdge[i] - Width[i]/2 - 10;//�߿�
            }
       }
    
     else if(LeftEdge[i] == 0 && RightEdge[i] == COL)//���߶����˵Ļ�  
       {
           AllLose++;
               
          if(i == ROW-1)//��������о���ͼ��������Ϊ�е�
            {
                    middleline[i] = middleLine;
            }       
          else 
            {
                    middleline[i] = middleline[i+5];//����������о�����һ�е�������Ϊ�����е�
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
            middleLine = middleline[ROW-1];//��¼��֡ͼ���59�е�����ֵ����Ϊ��һ��ͼ���59��ɨ����ʼ��
       }
    
           site_middle.y = (uint8)(i * 0.75) + 18;
           site_middle.x = (uint8)(middleline[i] * 0.68);
           LCD_point(site_middle, BLUE);
    
    
  }   
  
  
  
//  
//  
//-----------------------�ر�ɨ��----------------------------------------------------------------
//  
  
  
  for(i=ROW-30; i>0; i--)//����ʣ����
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=COL) //��һ�����߶��ҵ� ���ñ���ɨ��     
   {
         j = ((LeftEdge[i+1]+13) >= COL-2) ? COL-2:(LeftEdge[i+1]+13);//������߽�    
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
         j = ((RightEdge[i+1]-13) <= 1)? 1:(RightEdge[i+1]-13); //�����ұ߽�   
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
    
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==COL)//��һ��ֻ�ҵ���߽�   
    {      
         j  = ((LeftEdge[i+1]+13) >=COL-2)? COL-2:(LeftEdge[i+1]+13);//��߽��ñ���ɨ��   
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
         j = middleline[i+1];//��һ�ж����ұ߽���ȫ��ɨ�� 
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
    
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=COL) //��һ��ֻ�ҵ��ұ߽�      
    {
         j = ((RightEdge[i+1]-13) <= 1)?1:(RightEdge[i+1]-13);//��Ե׷�����ұ߽� 
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
           j = middleline[i+1]; //ȫ��ɨ������߽�
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
    
    
    
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==COL)  //��һ��û�ҵ��߽磬������ʮ�ֻ��߻���  
    {
        
          j = middleline[i+1];   //��ȫ������߽�
        while(j >= 2)  
        {
             if(imagebuff[i][j]==255 && imagebuff[i][j-1]==0 && imagebuff[i][j-2]==0)     
             {
                  LeftEdge[i] = j;
                  break;
             }
            j--;
        }
         j = middleline[i+1];   //ȫ�����ұ߽�   
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
    
    
    
    
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+13) )//��������� 
    {      
          middleline[i] = middleline[i+5];//����һ��
    }
 
    
    else
    {
            if(LeftEdge[i]!=0 && RightEdge[i]!=COL)   
            {
                     middleline[i] = (LeftEdge[i] + RightEdge[i])/2 ;
                     //��б��ʮ�ֽ��о���
                     
//                 if( middleline[i]-middleline[i+1]>10&&((ABS(LeftEdge[i]-LeftEdge[i+1]>6)||ABS(RightEdge[i]-RightEdge[i+1]>6)) )&& i>=60)//��������ͻ��
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
           else if(LeftEdge[i]==0 && RightEdge[i]==COL)//���߶���    
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

