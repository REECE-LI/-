#ifndef __STEER_H__
#define __SYEER_H__

#define S3010_FTM   FTM2       //FTM1ģ��
#define S3010_CH    FTM_CH1   //ͨ����PTA8
#define S3010_HZ    50 
//�����Ƶ��


//#if 0
//
//#define SteerMax 660     //������ֵ
//#define SteerMin  370    //�����ֵ
//#define SteerMidle 515   //�����Сֵ

//#endif

#if 1

#define SteerMax 860    //������ֵ
#define SteerMin  700    //�����Сֵ
#define SteerMidle 780 //�����ֵ

#endif



extern float  LastError;
extern float  Error;


void SteerInit(void);//�����ʼ��
void CalculateError(void);
extern void SteerControl(void);


#endif
