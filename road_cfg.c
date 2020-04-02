
#include "road_cfg.h"



#include "stdlib.h"


line_t line_limt;
point_t point_left;     
point_t point_right;                    //这个是正序 从上往下找的点 即远点

void line_config()
{
    uint8 i;
//-------------------------判断中止行----------------------
    for (i = ROW-2; i > 109; i--)                       //中止线判断10行
    {
        if( ((RightEdge[i] != 187 && RightEdge[i] != 0)&& 
              (RightEdge[i+1] != 187 && LeftEdge[i+1] != 0) &&
                (RightEdge[i]-LeftEdge[i] > i*2-70))&&            //在十字中央可能看到多余赛道
                    (RightEdge[i]-LeftEdge[i] < i*2-50）) ||


                        ((RightEdge[i] == 187 && LeftEdge[i] == 0)&&                //双丢线 只能在十字内部
                            (RightEdge[i+1] == 187 && LeftEdge[i+1] == 0)) )//判断底线是否存在
                {
                    line_limt.end = i;
                    line_limt.end_flag = 1;                   
                    break;                                  //底线存在 不丢线
                }

        else 
         {
             line_limt.end_flag = 0;
             break;                                 //底线不存在 丢线            
         }
    }

//----------------判断开始行----------------------------
        for (i = line_limt.start-3; i > 35; i--)                       //中止线判断10行
    {
        if((RightEdge[i] != 187 && RightEdge[i] != 0)&& 
              (RightEdge[i+1] != 187 && LeftEdge[i+1] != 0) &&
                (ABS(middleline[i], middleline[line_limt.end]) > 10) &&                 //这是判断当前底线时的中线 来判断 
                    (ABS(middleline[i+1], middleline[line_limt.end+1]) < 10))    
                {
                    line_limt.start = i;
                    line_limt.start_flag = 1;
                    break;                                  //首行存在 不丢线
                }

        else 
         {
             line_limt.start_flag = 0;
             break;                                 //首行不存在 丢线            
         }
    }
}


void find_point_R(point_t *p)
{
    uint8 i;
    for (i = line_limt.start+3, i <= line_limt.end, i++)
    {
        if((RightEdge[i]-RightEdge[i+1]<=0)&&
            (RightEdge[i-1]-RightEdge[i])>=1&&
                (RightEdge[i-2]-RightEdge[i])>=2)&&
                    (RightEdge[i-3]-RightEdge[i])>=2))
        {
            p->x = i;
            p->y = RightEdge[i];
            p->flag = 1;
            break
        }

        //可能还有其他情况
    }

}



void find_point_L(point_t *p)
{
    uint8 i;
    for (i = line_limt.start+3, i <= line_limt.end, i++)
    {
        if((LeftEdge[i]-LeftEdge[i+1]<=0)&&
            (LeftEdge[i-1]-LeftEdge[i])>=1&&
                (LeftEdge[i-2]-LeftEdge[i])>=2)&&
                    (LeftEdge[i-3]-LeftEdge[i])>=2))
        {
            p->x = i;
            p->y = LeftEdge[i];
            p->flag = 1;
            break
        }

        //可能还有其他情况
    }
}



road_e corss_config()
{
    line_config();
    find_point_R(point_right);
    find_point_L(point_left);
    if(AllLose > 15)           return CROSS_IN;
}