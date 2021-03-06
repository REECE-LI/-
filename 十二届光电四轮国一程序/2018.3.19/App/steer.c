#include "common.h"
#include "include.h"
#include "steer.h"

/*********define for SteerControl**********/

float  KP=15;//舵机方向比例系数，影响舵机的打角范围
float  KD=8;//10//7.5//舵机方向微分系数,影响舵机的打角反应
float  SteerPwmAdd=0.0;//舵机pwm增量
float  Error;//偏差值
float  LastError;//上次的偏差
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
float  J=0.0300;//调节p和偏差的关系，越大，作用越强
float  JD=0.0400;//调节p和偏差的关系，越大，作用越强
float  BasicP=3.0; //基本的P值
float  BasicD=6.5; //基本的P值
uint32 SteerPwm=0,LastSteerSwm=0;//舵机的pwm值和上次舵机的pwm值

//加权平均，权值的选取
 #if 0
float Weight[60]={ 
                    0,0,0,0,0,0,0,0,0,0,     //0-9行，基本用不到
                    0,0,0,0,0,0,2,2,2,2,  //0-19行，基本用不到
                   
                   3.8,3.8,1.8,1.8,2.8,3.3,2.3,3.3,3.3,3.3,//20-29行

                   //1.05,1.05,1.05,1.05,1.05,2,2,2,2,2,
                    
                   2,2,2,2,2,2.0,2.0,2.5,2.5,2.5,//30-39行
            
                   2.1,2.1,2.1,1,1,2,1,1,1,1,//40-49行
                  1,0,0,0,0,};//最近十行*///不压线，用于弯道
#endif

#if 1

float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,              //0-9行，基本用不到
                    
                     2,2,2,2,2,1.5,1,1.5,1,             //0-19行，基本用不到
                  
                     2.5,1,2.2,1,2.5,1,2,2,1,2,            //20-29行
                    
                     1,3,2,2,1,2,1,2,2,1,              //30-39行

                     3,1,2,3,3,2,3,1,1,             //40-49行
                   
                     1,1,1,1,1,1,1,1,1,1,};          //最近十行




#endif




/****************************************************** 
 *函数名：SteerInit
 *
 *功能：舵机初始化
 * 
 * 入口参数：无
 * 
 *返回参数：无
 *
 * 作者：XGL
 * 
 * 日期：2016-3-01(已测试)
 *******************************************************/


void SteerInit(void)//舵机初始化

{
  
    ftm_pwm_init(FTM2, FTM_CH1,50, SteerMidle);
}


#if 1



/****************************************************** 
 *函数名：CalculateError
 *
 *功能：计算误差
 * 
 * 入口参数：无
 * 
 *返回参数：无
 *
 * 作者：XGL
 * 
 * 日期：2016-3-01(已测试)
 *******************************************************/


void CalculateError(void)

{
  
            int i;
            
            CenterSum=0;
            
            CenterMeanValue=0;
            
            WeightSum=0; 
            
      if(Cross.LeftSideling) 
      {
        for(i=58;i>InflectionPointL.InflectionPointRow;i--)
        {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
        }
      }
      
      else if(Cross.RightSideling)
      {
       for(i=58;i>InflectionPointR.InflectionPointRow;i--)
        {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
        }
      
      }
      else if(StrightIntoCrossL)
      {
         for(i=58;i>InflectionPointL.InflectionPointRow;i--)
        {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
        }
      
      }
      
      else if(StrightIntoCrossR)
      {
        for(i=58;i>InflectionPointR.InflectionPointRow;i--)
        {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
        }
      
      }
      else if(StrightIntoCrossLR)
      {
        for(i=58;i>(InflectionPointR.InflectionPointRow+InflectionPointR.InflectionPointRow)/2;i--)
        {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
        }
      
      }
      

       else if(Cross.CrossFlag)
       {
         for(i=45;i>LastLine;i--)
         {
               CenterSum+=MiddleLine[i]*Weight[i];             
               WeightSum+=Weight[i];
         }
       
       }
      
      else
      {
            
        for(i=57;i>LastLine;i--)
          
        {      
               CenterSum+=MiddleLine[i]*Weight[i];
              
               WeightSum+=Weight[i];       
         }
      
      }
      
       if(WeightSum!=0)
         
       {
             CenterMeanValue=(CenterSum/WeightSum);//算出加权平均后中线的值
           
        }
            
             LastError=Error;
      
             Error=(40-CenterMeanValue);// 一场图像偏差值 
             
             if(Error>=30.0)//偏差限幅
                
               Error=30.0;
             
             if(Error<=-30.0)
               
               Error=-30.0; 
             
            KP=BasicP+(Error* Error)*J;//动态二次p模型
             
             if(KP>=13) KP=13;//p值限幅
             
             KD=BasicD+(Error* Error)*JD;
             
              if(KD>=20) KD=20;//p值限幅
                    
}
                     
#endif                
     



/****************************************************** 
 *函数名：SteerControl
 *
 *功能：舵机控制
 * 
 * 入口参数：无
 * 
 *返回参数：无
 *
 * 作者：XGL
 * 
 * 日期：2016-3-01(已测试)
 *******************************************************/



void SteerControl(void)
{
    
        CalculateError(); 
        
   
       SteerPwmAdd=(KP*Error)+KD*(Error-LastError);//舵机的pd控制器
       
        if(SteerPwmAdd>=120)
          
           SteerPwmAdd=120;
        
        if(SteerPwmAdd<=-120)
          
           SteerPwmAdd=-120;
            
        SteerPwm=(uint32)(SteerPwmAdd+SteerMidle);
        
          
        if(SteerPwm>=SteerMax)//限幅
         
              SteerPwm=SteerMax;
       
        if(SteerPwm<=SteerMin)
          
              SteerPwm=SteerMin;
          
           ftm_pwm_duty(FTM1,FTM_CH0,SteerPwm);//舵机pwm更新
           
           LastSteerSwm=SteerPwm;//记录pwm值
                    
}










