#ifndef __WHEEL_H__
#define __WHEEL_H__

#define WHEEL_L  FTM0
#define WHEEL_R  FTM2

#define LEFT    FTM_CH6   
#define RIGHT    FTM_CH1     

#define wheel_freq  10000

#define wheel_duty_max 100      //速度最大时 PWM最小
#define wheel_duty_min  400     


#define wheel_duty_start 350


extern uint8  middleline[120];          //存储中线的数组

float  KP=1.0;                                       
float  KD=0.0;                                       
uint8  wheel_change = 0;                            
float  Error;                                      //偏差值
float  LastError;                                  //上次的偏差
uint8  WeightSum = 0;
uint8  CenterMeanValue = 0;
uint8  CenterSum=0;
uint16 wheel_duty_L = 350,Last_wheel_duty_L = 0 ;   //左轮的占空比
uint16 wheel_duty_R = 350,Last_wheel_duty_R = 0 ;   //右轮的占空比


//--------------------------------闭环时的PID变量命名-------------------------------
float speed_KP = 1.0;
float speed_KI = 1.0;
float speed_KD = 1.0;
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


void wheel_init(void);
void CalculateError(void);               //增加权重的Error
void SteerControl(void);


#endif 
