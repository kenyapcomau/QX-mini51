/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)  http://qx-mcu.taobao.com/  ���ܵ꣩
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52RC
* �����뻷������ Keil ��Visio4	
* �������ܡ��� 			   			            			    
* ��ʹ��˵������ ʵ��ǰ�����J1��J2����ñ
				 ���ö�ʱ�����Ʋ���ռ�ձȿɱ�� PWM ��
				 ��S1��PWMֵ���ӣ���ռ�ձȼ�С,LED �ƽ���
				 ��S2��PWMֵ��С����ռ�ձ�����,LED �ƽ���
				 ��PWMֵ���ӵ����ֵ���С����Сֵʱ�������������� 
**********************************************************************************/

#include<reg51.h>
#include<intrins.h>

sbit  K1 =P3^4 ;           //PWMֵ���Ӽ�
sbit  K2 =P3^5;           //PWMֵ���ټ�
sbit  BEEP =P3^6;         //������
unsigned char PWM=0x7f ;   //����ֵ

void Beep();
void delayms(unsigned char ms);
void delay(unsigned char t);

/*********************************************************/
void main()
{   
    P1=0xff;
    TMOD=0x21 ;
	TH0=0xfc ;           //1ms��ʱ����
    TL0=0x66 ;           //Ƶ�ʵ���

    TH1=PWM ;            //��������
    TL1=0 ;

	EA=1;
	ET0=1;
	ET1=1;
    
    TR0=1 ;

   while(1)
   {
	do{
	    if(PWM!=0xff)
		  {PWM++ ;delayms(10);}
        else Beep() ; 
	  }
    while(K1==0);

	do{
      if(PWM!=0x02)
	    {PWM-- ;delayms(10);}
      else Beep() ; 
	  }
    while(K2==0);
  }
}

/*********************************************************/
// ��ʱ��0�жϷ������  ��Ƶ�ʣ�
/*********************************************************/
void timer0() interrupt 1 
{  
    TR1=0 ;
    TH0=0xfc ;
    TL0=0x66 ;
    TH1=PWM ;
    TR1=1 ;
    P1=0x00 ;      //�������
}

/*********************************************************/
// ��ʱ��1�жϷ������ ��������
/*********************************************************/
void timer1() interrupt 3 
{ 
    TR1=0 ;
    P1=0xff ;     //�������
}

/*********************************************************/
//�������ӳ���
/*********************************************************/

void Beep()     
  {
    unsigned char i  ;
    for (i=0  ;i<100  ;i++)
      {
        delay(100)  ;
        BEEP=!BEEP  ;                //Beepȡ��
      } 
    BEEP=1  ;                        //�رշ�����
	delayms(100);
  } 

/*********************************************************/
// ��ʱ�ӳ���
/*********************************************************/  
void delay(unsigned char t)
 { 
   while(t--)   ;
 }

/*********************************************************/
// ��ʱ�ӳ���
/*********************************************************/
void delayms(unsigned char ms) 

{
   unsigned char i ;
   while(ms--)
    {
      for(i = 0 ; i < 120 ; i++) ;
    }
}

/*********************************************************/