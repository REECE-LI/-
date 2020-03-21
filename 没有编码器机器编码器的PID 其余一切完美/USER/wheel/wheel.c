#include "common.h"
#include "include.h"
#include "wheel.h"

void wheel_init(void)
  {
      ftm_pwm_init(WHEEL_L, LEFT, wheel_freq, wheel_duty_start);         //PTD6，右电机
      ftm_pwm_init(WHEEL_R, RIGHT, wheel_freq, wheel_duty_start);         //PTD7,左电机  
  }

void CalculateError(void)               //增加权重的Error
  {
    uint8 i;            
    CenterSum=0;            
    CenterMeanValue=0;           
    WeightSum=0;    
    for(i=119; i>30; i--)       
      {      
        CenterSum += middleline[i] * Weight[i];
        WeightSum += Weight[i];       
      }
     if(WeightSum!=0)   
      {
        CenterMeanValue = (CenterSum / WeightSum);//算出加权平均后中线的值
      }
     LastError=Error;
     Error=(94-CenterMeanValue);          // 一场图像偏差值 左转error>0
     if(Error>=94.0) Error=94.0;          //偏差限
     if(Error<=-94.0) Error=-94.0; 
                 
  }
      
//            KP = BasicP + (Error * Error) * J;//动态二次p模型
//             
//            if(KP >= 1) KP = 1;//p值限幅
//             
//            KD = BasicD+(Error * Error) * JD;
//             
//            if(KD >= 10) KD = 10;//p值限幅
//                    



//------------------------开环状态---------------------------------------------------                              
void SteerControl(void)
{
    
        CalculateError();         
       wheel_change = (int)((KP*Error) + KD*(Error-LastError));//
       
//        if(SteerPwmAdd>=60)
//          
//           SteerPwmAdd=60;
//        
//        if(SteerPwmAdd<=-60)
//          
//           SteerPwmAdd=-60;

           wheel_duty_L = (int)(wheel_duty_start + wheel_change);
           wheel_duty_R = (int)(wheel_duty_start - wheel_change);
//----------------------------------------------------------------------------        
        //Error = 0需要比较Error-LastError
         if((Error == 0)&&(Error-LastError<5)&&(Error-LastError>-5))
           wheel_duty_L = wheel_duty_R = wheel_duty_start;
       
//------------------------------------------------------------------          
        if(wheel_duty_L >= wheel_duty_min) wheel_duty_L = wheel_duty_min;                    
        else if(wheel_duty_L <= wheel_duty_max) wheel_duty_L = wheel_duty_max;
       
          
        if(wheel_duty_R >= wheel_duty_min) wheel_duty_R = wheel_duty_min;                    
        else if(wheel_duty_R <= wheel_duty_max) wheel_duty_R = wheel_duty_max;
//--------------------------------------------------------------------             
          
           
//           if (Error <= 3 && Error >= -3)
//             SteerPwm = SteerMidle;
           
           ftm_pwm_duty(WHEEL_R,RIGHT,wheel_duty_R);
           ftm_pwm_duty(WHEEL_L,LEFT,wheel_duty_L);
           
           
           Last_wheel_duty_L=wheel_duty_L;                                               //记录pwm值
           Last_wheel_duty_R=wheel_duty_R;
}
             
//-----------------------------------闭环状态------------------------------------------------     






