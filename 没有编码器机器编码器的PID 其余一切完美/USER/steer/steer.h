#ifndef __STEER_H__
#define __STEER_H__

#define S3010_FTM   FTM2       //FTM2ģ��
#define S3010_CH    FTM_CH1   //ͨ����
#define S3010_HZ    100        //�����Ƶ��

#define SteerMax 1700//180     //������ֵ
#define SteerMin  1400//130    //�����Сֵ
#define SteerMidle 1550//155   //�����ֵ


extern float  LastError;
extern float  Error;
extern uint8  middleline[120];


void SteerInit(void);//�����ʼ��
void CalculateError(void);
extern void SteerControl(void);


#endif 
