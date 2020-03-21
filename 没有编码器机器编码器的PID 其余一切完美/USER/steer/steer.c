 #include "common.h"
#include "include.h"
#include "steer.h"

float  KP=1.0;//舵机方向比例系数，影响舵机的打角范围
float  KD=0.0;//10//7.5//舵机方向微分系数,影响舵机的打角反应
float  SteerPwmAdd=0.0;//舵机pwm增量
float  Error;//偏差值
float  LastError;//上次的偏差
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
//float  J=0.02;//调节p和偏差的关系，越大，作用越强
//float  JD=0;//=0.0400;/ /调节p和偏差的关系，越大，作用越强
//float  BasicP=0.43; //基本的P值
//float  BasicD=0; //基本的D值
uint32 SteerPwm = 155,LastSteerSwm = 0 ;//舵机的pwm值和上次舵机的pwm值
uint8 Weight[120]={ 
   0,0,0,0,0,0,0,0,0,0,                 //1
    0,0,0,0,0,0,0,0,0,0,                //2
     0,0,0,0,0,0,0,0,0,0,               //3
      2,2,2,2,2,2,2,2,2,2,              //4
      
      
      // 5,5,5,5,5,5,5,5,5,5,             //5
       2,2,2,2,3,3,3,3,3,3,
         3,3,3,3,5,5,5,5,5,5,
        8,8,8,8,8,8,8,8,8,8,            //6
       //  8,8,8,8,8,8,8,8,8,8,          //7
          8,8,8,8,8,8,8,8,8,8,   
           8,8,8,8,8,8,8,8,8,8,//8
           // 5,5,5,5,5,5,5,5,5,5,       //9
             8,8,8,5,5,5,5,5,3,3,       //10
              3,3,3,3,3,3,2,2,2,2,        //11
              2,2,2,2,1,1,1,1,1,1,       //12
  
};








void SteerInit(void)//舵机初始化
{
        ftm_pwm_init(S3010_FTM, S3010_CH, S3010_HZ, SteerMidle);
}

void CalculateError(void)

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
      
          Error=(94-CenterMeanValue);// 一场图像偏差值 
             
             if(Error>=94.0)//偏差限幅
                
               Error=94.0;
             
             if(Error<=-94.0)
               
               Error=-94.0; 
             
//            KP = BasicP + (Error * Error) * J;//动态二次p模型
//             
//            if(KP >= 1) KP = 1;//p值限幅
//             
//            KD = BasicD+(Error * Error) * JD;
//             
//            if(KD >= 10) KD = 10;//p值限幅
//                    
}


                              
    void SteerControl(void)
{
    
        CalculateError(); 
        
   
       SteerPwmAdd=(KP*Error)+KD*(Error-LastError);//舵机的pd控制器
       
//        if(SteerPwmAdd>=60)
//          
//           SteerPwmAdd=60;
//        
//        if(SteerPwmAdd<=-60)
//          
//           SteerPwmAdd=-60;
            
        SteerPwm = (uint32)(SteerPwmAdd+SteerMidle);
        
          
        if(SteerPwm >= SteerMax)//限幅
         
              SteerPwm = SteerMax;
       
        if(SteerPwm<=SteerMin)
          
              SteerPwm = SteerMin;
          
           
//           if (Error <= 3 && Error >= -3)
//             SteerPwm = SteerMidle;
           
           ftm_pwm_duty(FTM2,FTM_CH1,SteerPwm);                                 //舵机pwm更新
           
           LastSteerSwm=SteerPwm;                                               //记录pwm值
          
}
             
            