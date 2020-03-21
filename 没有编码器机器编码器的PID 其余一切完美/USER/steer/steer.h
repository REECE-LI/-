#ifndef __STEER_H__
#define __STEER_H__

#define S3010_FTM   FTM2       //FTM2模块
#define S3010_CH    FTM_CH1   //通道号
#define S3010_HZ    100        //舵机的频率

#define SteerMax 1700//180     //舵机最大值
#define SteerMin  1400//130    //舵机最小值
#define SteerMidle 1550//155   //舵机中值


extern float  LastError;
extern float  Error;
extern uint8  middleline[120];


void SteerInit(void);//舵机初始化
void CalculateError(void);
extern void SteerControl(void);


#endif 
