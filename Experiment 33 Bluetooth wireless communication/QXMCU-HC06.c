/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ (直销店)  http://qx-mcu.taobao.com/  （总店）
* 【实验平台】： QX-MINI51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【程序功能】： 蓝牙数码管显示加LED指示灯			   			            			    
* 【使用说明】： 

STC89c52与HC-06蓝牙连线：
	RXD    TXD
	TXD    RXD
	蓝牙连接方式请参照本文件下的图片
手机采用蓝牙串口通信助手
最后已改用Android自编蓝牙通信软件
蓝牙模块安装方法请参照本文件夹下的图片01
蓝牙模块购买地址：http://item.taobao.com/item.htm?id=27378740053

**********************************************************************************/

#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
uchar tmp;
uint c=0;
sbit led1=P1^0;		    //指示灯0
sbit led2=P1^1;			//指示灯1
sbit led3=P1^2;			//指示灯3
sbit led7=P1^7;			//指示灯3

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};

void delay1(uint z)//1ms
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);	
}
void display(uchar value)
{
    uchar shu1,value1;
	value1=value-48;
	//bai=value1/16;
	shu1=value1%16;


   P0=table[shu1];
   P2_0 = 0;
   delay1(1);
   P2_0 = 1;
}	
	  
void init();   //串口初始化
void send(unsigned char a);	//单字节发送函数
void ctrl(); //接收处理函数

void main()
{
		init();
          while(1)
          {         
                  display(SBUF);
				  if(RI==1)                     // 是否有数据到来
                  {
                   RI = 0;
				  ctrl();
				  }       
          }
}

 void init()	//串口初始化
 {
 		  ES=0;								  //关中断
		  SCON = 0x50;                        // REN=1允许串行接受状态，串口工作模式1,
											  //10位UART（1位起始位，8位数据位，1位停止位，无奇偶校验），波特率可变

		  TMOD = 0x20;                        // 定时器1工作于方式2，8位自动重载模式, 用于产生波特率
          TH1=TL1=0xFD;                       // 波特率9600 （本次测试采用晶振为11.0592）
          
          PCON &= 0x7f;                       // 波特率不倍增
          TR1 = 1;							  //定时器1开始工作，产生波特率
											  //发送标志位置0
		  TI=0;								  //接收标志位置0
		  RI=0;
		  
		  //EA=0;
	      ES=1;
 }
   
void send(unsigned char a)	//单字节数据发送
{							//注意：若单片机TXD（P3.1）无上拉能力，必须在P3.1端接上拉电阻。本次测试需要接上拉电阻
	TI=0;	
	SBUF=a;
	while(TI==0);
	TI=0;

	if(c%2)					//发送指示灯标志，每接收一次，此灯亮灭交替
	 led7=1;
	else
	 led7=0;
	c++;
} 

void ctrl()		            //接收处理函数
{
  				  switch(tmp)
				  {
				  	case '1':
						led1=1;		       	    //收到字符1，指示灯0灭
						send(tmp);
						break;					
					case '2':					 //收到字符2，指示灯1灭
						 led2=1; 
						 send(tmp);
						 break;
					case '3':				    //收到字符3，指示灯0、1亮
						 led1=0;
						 led2=0;
						 send(tmp);
						 break;
					case '4':					//收到字符4，指示灯0、1灭
						  led1=1;
						  led2=1;
						  send(tmp);
						  break;
					case '5':					//收到字符5，指示灯3亮
						led3=0;
						send(tmp);
						break;
					default:                   //其他，灯全灭
						  led1=1;
						  led2=1;
						  led3=1;
						  send(tmp);
				  }
}