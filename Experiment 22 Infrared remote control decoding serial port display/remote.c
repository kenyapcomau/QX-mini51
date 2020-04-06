/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ (直销店)  http://qx-mcu.taobao.com/  （总店）
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52RC
* 【编译环境】： Keil μVisio4	
* 【程序功能】： 串口显示红外键码值0-9 波特率 9600			   			            			    
* 【使用说明】： 使用红外接收时，应将红外接收头插到开发板指定位置，注意方向
**********************************************************************************/

/*预处理命令*/
#include<reg52.h>    //包含单片机寄存器的头文件
#include<intrins.h> //包含_nop_()函数定义的头文件
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

sbit IRIN=P3^2;         //红外接收器数据线



uchar IRCOM[7];
/***************延时函数*****************************/
void delay(unsigned char x)    //x*0.14MS
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++) {}
	}
}

void delay1(int ms)
{
	unsigned char y;
	while(ms--)
	{
		for(y = 0; y<250; y++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}

//************************************************************
void delay1ms()
{
 unsigned int i;
 for(i=8024;i>0;i--);  //延时124*8+10=1002us
}


 //*****串口初始化函数***********

//******************************
Initial_com()
{
 EA=1;        //开总中断
 ES=1;        //允许串口中断
 ET1=1;        //允许定时器T1的中断
 TMOD=0x20;   //定时器T1，在方式2中断产生波特率
 PCON=0x00;   //SMOD=0
 SCON=0x50;   // 方式1 由定时器控制
 TH1=0xfd;    //波特率设置为9600
 TL1=0xfd;
 TR1=1;       //开定时器T1运行控制位

}

/*******************************************************************/
void main()
{

	IRIN=1;                    //I/O口初始化
	 Initial_com();	
	
	IE=0x81;                 //允许总中断中断,使能 INT0 外部中断
	TCON=TCON|0x01;               //触发方式为脉冲负边沿触发
	
	 while(1)
	 {
		
	
		 
		
	  }


} 
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
	unsigned char j,k,N=0;
	unsigned char q=0;
	EX0 = 0;   
	delay(15);
	if (IRIN==1) 
	{ 
		EX0 =1;
		return;
	} 
	           //确认IR信号出现
	while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
		delay(1);
	
	for (j=0;j<4;j++)         //收集四组数据
	{ 
		for (k=0;k<8;k++)        //每组数据有8位
		{
			while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
			{delay(1);}
			while (!IRIN)          //等 IR 变为高电平
				delay(1);
			while (IRIN)           //计算IR高电平时长
			{
				delay(1);
				N++;           
				if (N>=30)
				{ 
					EX0=1;
					return;
				}                  //0.14ms计数过长自动离开。
			}                        //高电平计数完毕                
			IRCOM[j]=IRCOM[j] >> 1;     //数据最高位补“0”
			if(N>=8)
			{
				IRCOM[j] = IRCOM[j] | 0x80;		  //数据最高位补“1”
			}  
			N=0;
		}
	}
	
	if(IRCOM[2]!=~IRCOM[3])
	{ 
		EX0=1;
		return; 
	}
	
	IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
	IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位
	
	if(IRCOM[5]>9)
	{ 
		IRCOM[5]=IRCOM[5]+0x37;
	}
	else
	IRCOM[5]=IRCOM[5]+0x30;
	
	if(IRCOM[6]>9)
	{ 
		IRCOM[6]=IRCOM[6]+0x37;
	}
	else
	IRCOM[6]=IRCOM[6]+0x30;

	q= (((IRCOM[6]&0x0f)<<4) + (IRCOM[5]&0x0f));
    

	switch(q)    //判断按键键码值
	{
		case 0x16:SBUF=0x00;break; //串口发送0
		case 0x03:SBUF=0x01;break;  //串口发送01
		case 0x18:SBUF=0x02;break;  //串口发送02
		case 0x55:SBUF=0x03;break;  //串口发送03
		case 0x08:SBUF=0x04;break;  //串口发送04
		case 0x13:SBUF=0x05;break;  //串口发送05
		case 0x51:SBUF=0x06;break;  //串口发送06
		case 0x42:SBUF=0x07;break;  //串口发送07
		case 0x52:SBUF=0x08;break;  //串口发送08
		case 0x41:SBUF=0x09;break;  //串口发送09
	
		default :break; 
	}

	 EX0 = 1; 
}



