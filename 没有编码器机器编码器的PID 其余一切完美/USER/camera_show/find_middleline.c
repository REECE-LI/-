#include "find.h"
#include "common.h"
#include "road_cfg.h"

    uint8 LeftLose, RightLose, AllLose;     //记录丢线次数
    uint8 LeftEdge[ROW], RightEdge[ROW];
    uint8  imagebuff[ROW][COL];      //图像二值化数组
    uint8  middleline[ROW];          //存储中线的数组
    point_t point_right, point_left, left_point, right_point;
    curve_t curve_right, curve_left;
    LeftLose = RightLose = AllLose = 0;

 void find_middleline()
 {
    uint8 i, j, ii, middle_start, right_start, left_start, line_temp;
//-----------------初始化变量-----------------------
    for (i = ROW-1; i > 0; i--)
    {
    LeftEdge[i] = 0;            //起始的左边线
    RightEdge[i] = COL-1;       //起始的右边线
    middleline[i] = COL/2;
    }
//----------------前十行进行左右巡线------------------
    for (i = ROW-1; i > 110; i--)
    {
        if (i == ROW-1)
        {
            for(j = 94; j > 0; j--)
            {
                if(imagebuff[ROW-1][j] == 255 && imagebuff[ROW-1][j-1] == 255 && imagebuff[ROW-1][j-2] == 0)
                {
                    left_start = j-2;
                    break;
                }
            }
            for (j = 94; j < COL; j++)
            {
                if(imagebuff[ROW-1][j] == 255 && imagebuff[ROW-1][j+1] == 255 && imagebuff[ROW-1][j+2] == 0)
                {
                    right_start = j+2;
                    break;
                }
            }
            middle_start == (right_start + left_start)/2;
        }
        
        else middle_start == middleline[i+1];         //确定扫描的起点

        //------------------------首先寻找左边线-----------------------
        j = MAX(middle_start, 2);
        while (j >= 2)
        {
            if(imagebuff[i][j] == 255 && imagebuff[i][j-1] == 255 && imagebuff[i][j-2] == 0)        //白白黑
            {
                LeftEdge[i] = j-2;
                break;  
            }
            j--; 
        }

        //------------------------再寻找右边线------------------------
         j = MIN(middle_start, COL-2);
          while(j <= COL-2)
       {
           
        if(imagebuff[i][j] == 255 && imagebuff[i][j+1] == 255 && imagebuff[i][j+2] == 0)      //从左向右找到白白黑跳变点
          {
             RightEdge[i] = j+2;                                                
             break;                                                           
          }
         j++;                                                             
       }    

       if (LeftEdge[i] != 0 && RightEdge[i] != COL-1)   middleline[i] = (LeftEdge[i] + RightEdge[i])/2;        //这边 总钻风 视野广 可能在十字里面时会误判！！！ 需要结合拐点判断！！！
       else if (LeftEdge[i] != 0 && RightEdge[i] == COL-1 && RightEdge[i+1] != COL-1)
       {
           //检测不到右线 环岛 右弯？右斜入十字 
           //如果是弯道的话没有断点
           //十字有断点 但是可以忽略
           //可能是误差
           //总钻风可以不补半宽
           line_temp = i+1;
           for(ii = ROW-1; ii > line_temp; i--)
           {
               if(RightEdge[ii+3]-RightEdge[ii+2]>0&&(RightEdge[ii+1]-RightEdge[ii]>=0||RightEdge[ii+2]-RightEdge[ii]>=0))
               {
                   point_right.x = ii+1;
                   point_right.y = RightEdge[ii+1];
                   point_right.flag = 1;
                   break;
               }
           }           
           RightLose++;
       }
       else if (LeftEdge[i] == 0 && RightEdge[i] != COL-1 && LeftEdge[i+1] != 0)
       {
           //同上
           line_temp = i+1;
           for(ii = ROW-1; ii > line_temp; i--)
           {
               if(LeftEdge[ii+3]-LeftEdge[ii+2]>0&&(LeftEdge[ii+1]-LeftEdge[ii]>=0||LeftEdge[ii+2]-LeftEdge[ii]>=0))
               {
                   point_left.x = ii+1;
                   point_left.y = LeftEdge[ii+1];
                   point_left.flag = 1;
                   break;
               }
           }           
           LeftLose++;
       }
       else if(LeftEdge[i] < 10 && RightEdge[i] > COL-11)
       {
           AllLose++;
           if (i == ROW-1) middleline[i] = middle_start;//如果是首行就以图像中心作为中点     
          else middleline[i] = middleline[i+1];                    //如果不是首行就用上一行的中线作为本行中点        
       }       
    }
//-------------------以上 前十行 左右扫描结束--------------------------------
//-------------------以下边缘判断----------------------------
    if(point_left.flag == 0 && point_right.flag = 0)            //直道 底线没有丢失
    {
        for(i = 111; i > 1; i--)
        {   
            //在近拐点 没有找到的前提下 寻找近拐点
            if (point_left.flag == 0)
            {
                            left_start =  LeftEdge[i];
                            while( (imagebuff[i][left_start+1] == 0) || (imagebuff[i-1][left_start+1] == 0) || (imagebuff[i-1][left_start] == 0) || (imagebuff[i-1][left_start] == 0) ||
                                        (imagebuff[i][left_start-1] == 0) || (imagebuff[i+1][left_start-1] == 0))
                            {
                                if (imagebuff[i][left_start+1] == 0)                //右
                                {
                                    while(left_start <  LeftEdge[i]+10)                 //系数待定
                                    {
                                        if(imagebuff[i][left_start+1] == 0 && imagebuff[i][left_start+2] == 255 && imagebuff[i][left_start+3] == 255)
                                        {
                                            LeftEdge[i] = left_start+1;
                                            break;
                                        }
                                        
                                        left_start++;
                                        
                                    }
                                    
                                    point_left.count = 0;
                                    break;
                                }
                                else if (imagebuff[i-1][left_start+1] == 0)         //右·上
                                {
                                    LeftEdge[i-1] = left_start+1;
                                    point_left.count = 0;
                                    break;
                                } 
                                else if (imagebuff[i-1][left_start] == 0)           //上
                                {
                                    LeftEdge[i-1] = left_start;
                                    point_left.count = 0;
                                    break;
                                }
                                else if (imagebuff[i-1][left_start-1] == 0)            //左上
                                {
                                    LeftEdge[i-1] = left_start-1;
                                    if(point_left.count == 0) 
                                    {
                                        //point_left.flag = 1;
                                        point_left.x = i;
                                        point_left.y = left_start;
                                    }
                                    point_left.count++;
                                    break;
                                }
                                else if (imagebuff[i][left_start-1] == 0)               //左
                                {
                                    LeftEdge[i] = left_start;
                                    if(point_left.count == 0) 
                                    {
                                        //point_left.flag = 1;
                                        point_left.x = i;
                                        point_left.y = left_start;
                                    }
                                    point_left.count++;
                                    break;
                                }
                                else if (imagebuff[i+1][left_start-1] == 0)             //左下
                                {
                                    point_left.flag = 1;
                                    point_left.x = i;
                                    point_left.y = left_start;
                                    break;
                                }
                                else if (point_left.count>5)    point_left.flag = 1;
                              
                            }

                            
            }
            // 在近拐点没有找到的前提下寻找近拐点
            if (point_right.flag == 0)
            {
                    right_start = RightEdge[i];
                        while( (imagebuff[i][right_start-1] == 0) || (imagebuff[i-1][right_start+1] == 0) || (imagebuff[i-1][right_start] == 0) || (imagebuff[i-1][right_start+1] == 0) ||
                                    (imagebuff[i][right_start-1] == 0) || (imagebuff[i+1][right_start-1] == 0))
                        {
                            if (imagebuff[i][right_start-1] == 0)                //左
                            {
                                while(1)
                                {
                                    if(imagebuff[i][right_start-1] == 0 && imagebuff[i][right_start-2] == 255 && imagebuff[i][right_start-3] == 255)
                                    {
                                        RightEdge[i] = right_start-1;
                                        break;
                                    }
                                    
                                    right_start--;
                                    
                                }
                                RightEdge[i] = right_start-1;
                                break;
                            }
                            else if (imagebuff[i-1][right_start-1] == 0)         //左·上
                            {
                                RightEdge[i-1] = right_start-1;
                                point_right.count = 0;
                                break;
                            } 
                            else if (imagebuff[i-1][right_start] == 0)           //上
                            {
                                RightEdge[i-1] = right_start;
                                point_right.count = 0;
                                break;
                            }
                            else if (imagebuff[i-1][right_start+1] == 0)            //右上
                            {
                                RightEdge[i-1] = right_start-1;
                                if(point_right.count == 0) 
                                {
                                // right_point.flag = 1;
                                    point_right.x = i;
                                    point_right.y = right_start;
                                }
                                point_right.count++;
                                break;
                            }
                            else if (imagebuff[i][right_start+1] == 0)               //右
                            {
                                RightEdge[i] = right_start+1;
                                if(point_right.count == 0) 
                                {
                                    //point_right.flag = 1;
                                    point_right.x = i;
                                    point_right.y = right_start;
                                }
                                point_right.count++;
                                break;
                            }
                            else if (imagebuff[i+1][right_start+1] == 0)             //右下
                            {
                                point_right.flag = 1;
                                point_right.x = i;
                                point_right.y = right_start+1;
                                break;                                                      //这边拐点判断 感觉有问题 需要 实地测量！！！！！！！
                            }
                            else if (point_right.count > 5) point_right.flag = 1;
                        }
            }
        }
            //由于  找到近拐点 一边会停止寻找 所以 middleline[MAX(point_right.x, point_left.x)] 开始缺失
            
//-------------------------------------寻找远拐点 出现左右近拐点 进入十字 寻找远拐点-----------------------------------------------------
            if (point_right.flag ==1 && point_left.flag == 1)                   //近端双拐点 遇到十字 
            {
               line_temp = MAX(point_right.x, point_left.x);                                 //必须把车身弄正
               //middle_start = (LeftEdge[line_temp] + RightEdge[line_temp])/2;
               for (j = line_temp; j > 10; j--)
               {
                  if(imagebuff[j][LeftEdge[line_temp]] == 255 && imagebuff[j-1][LeftEdge[line_temp]] == 255 && imagebuff[j-2][LeftEdge[line_temp]] == 0) 
                  {
                      left_point.x = j-2;                       //往上进几格  看时机情况 要多不要少
                      left_point.y = LeftEdge[line_temp];
                      break;
                  }
               } 
//----------------------------------远左拐点----------------------//这个不是到可不可以  不可以的话需要增加附加条件
               while(imagebuff[left_point.x][left_point.y] == 0 && imagebuff[left_point.x][left_point.y-1] == 0)
               {                   
                    if (imagebuff[left_point.x][left_point.y+1] == 255 && imagebuff[left_point.x-1][left_point.y+5] == 255）
                    {
                        left_point.flag = 1;
                        break;
                    }
                    left_point.x--;
                    left_point.y++;
               }

  //-------------------------------------------------------------------------------------              
               for (j = line_temp; j > 10; j--)
               {
                  if(imagebuff[j][RightEdge[line_temp]] == 255 && imagebuff[j-1][RightEdge[line_temp]] == 255 && imagebuff[j-2][RightEdge[line_temp]] == 0) 
                  {
                      right_point.x = j+2;
                      right_point.y = RightEdge[line_temp];
                      break;
                  }
               } 

               while(imagebuff[right_point.x][right_point.y] == 0 && imagebuff[right_point.x][right_point.y+1] == 0)
               {                   
                    if (imagebuff[right_point.x][right_point.y-1] == 255 && imagebuff[right_point.x-1][right_point.y-5] == 255）
                    {
                        left_point.flag = 1;
                        break;
                    }
                    left_point.x--;
                    left_point.y--;
               } 
                line_temp = MIN(left_point.x, right_point.x);                     
            }

            //line_temp = MIN(left_point.x, right_point.x);

           


            



                   
        
        
    }

 }