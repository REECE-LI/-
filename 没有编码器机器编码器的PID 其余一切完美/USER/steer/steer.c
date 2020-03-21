 #include "common.h"
#include "include.h"
#include "steer.h"

float  KP=1.0;//����������ϵ����Ӱ�����Ĵ�Ƿ�Χ
float  KD=0.0;//10//7.5//�������΢��ϵ��,Ӱ�����Ĵ�Ƿ�Ӧ
float  SteerPwmAdd=0.0;//���pwm����
float  Error;//ƫ��ֵ
float  LastError;//�ϴε�ƫ��
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
//float  J=0.02;//����p��ƫ��Ĺ�ϵ��Խ������Խǿ
//float  JD=0;//=0.0400;/ /����p��ƫ��Ĺ�ϵ��Խ������Խǿ
//float  BasicP=0.43; //������Pֵ
//float  BasicD=0; //������Dֵ
uint32 SteerPwm = 155,LastSteerSwm = 0 ;//�����pwmֵ���ϴζ����pwmֵ
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








void SteerInit(void)//�����ʼ��
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
          CenterMeanValue = (CenterSum / WeightSum);//�����Ȩƽ�������ߵ�ֵ
         }
        
          LastError=Error;
      
          Error=(94-CenterMeanValue);// һ��ͼ��ƫ��ֵ 
             
             if(Error>=94.0)//ƫ���޷�
                
               Error=94.0;
             
             if(Error<=-94.0)
               
               Error=-94.0; 
             
//            KP = BasicP + (Error * Error) * J;//��̬����pģ��
//             
//            if(KP >= 1) KP = 1;//pֵ�޷�
//             
//            KD = BasicD+(Error * Error) * JD;
//             
//            if(KD >= 10) KD = 10;//pֵ�޷�
//                    
}


                              
    void SteerControl(void)
{
    
        CalculateError(); 
        
   
       SteerPwmAdd=(KP*Error)+KD*(Error-LastError);//�����pd������
       
//        if(SteerPwmAdd>=60)
//          
//           SteerPwmAdd=60;
//        
//        if(SteerPwmAdd<=-60)
//          
//           SteerPwmAdd=-60;
            
        SteerPwm = (uint32)(SteerPwmAdd+SteerMidle);
        
          
        if(SteerPwm >= SteerMax)//�޷�
         
              SteerPwm = SteerMax;
       
        if(SteerPwm<=SteerMin)
          
              SteerPwm = SteerMin;
          
           
//           if (Error <= 3 && Error >= -3)
//             SteerPwm = SteerMidle;
           
           ftm_pwm_duty(FTM2,FTM_CH1,SteerPwm);                                 //���pwm����
           
           LastSteerSwm=SteerPwm;                                               //��¼pwmֵ
          
}
             
            