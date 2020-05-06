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
* 【程序功能】： 数码管显示红外键码值0-9 			   			            			    
* 【使用说明】： 使用红外接收时，应将红外接收头插到开发板指定位置，注意方向，
				 开发板上有弧面提示	 ,红外接收头圆弧面面向开发板外部
**********************************************************************************/

/*预处理命令*/
#include<reg52.h>    //包含单片机寄存器的头文件
#include<intrins.h> //包含_nop_()函数定义的头文件
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

sbit IRIN=P3^2;         //红外接收器数据线


sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};
uchar IRCOM[7];
uchar m;


void delay1ms();
void delay(unsigned char x) ;
void display();
void delay1(int ms);



/*******************************************************************/
void main()
{
	 unsigned int  a;
   
	IRIN=1;                    //I/O口初始化	
	IE=0x81;                 //允许总中断中断,使能 INT0 外部中断
	TCON=TCON|0x01;               //触发方式为脉冲负边沿触发
	
	 while(1)
	 {
		
	    for(a=100;a>0;a--)
   {
     display();
   }
		 
		
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
		case 0x16:m=0;break; //串口发送0
		case 0x03:m=1;break;  //串口发送01
		case 0x18:m=2;break;  //串口发送02
		case 0x55:m=3;break;  //串口发送03
		case 0x08:m=4;break;  //串口发送04
		case 0x13:m=5;break;  //串口发送05
		case 0x51:m=6;break;  //串口发送06
		case 0x42:m=7;break;  //串口发送07
		case 0x52:m=8;break;  //串口发送08
		case 0x41:m=9;break;  //串口发送09
	
		default :break; 
	}

	 EX0 = 1; 
}


/***************延时函数*****************************/
void delay(unsigned char x)    //x*0.14MS
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++) {}
	}
}


void display()
{
   
   P0=table[m];
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
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

