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
* 【程序功能】： 串口中断程序实验——通过PC上位机软件控制流水灯实验			   			            			    
				 以下为16进制形式发送
				 发送 01 ， led1灯亮；   
				 发送 02 ， led2灯亮；
				 发送 03 ， led灯3亮；
				 发送 04 ， led灯4亮；
				 发送 05 ， led灯5亮；
				 发送 06 ， led灯6亮；
				 发送 07 ， led灯7亮；
				 发送 08 ， led灯8亮；			   			            			    
* 【使用说明】： 使用前请短接J2跳线帽，串口助手设置为比特率9600，无校验位，停止位1，
发送数据为16进制形式
**********************************************************************************/

/*预处理命令*/
#include <reg52.h> 		  //包含单片机寄存器的头文件
#define uchar unsigned char
#define uint unsigned int
#define LED P1
uchar dat,num;
void main()
{	
	TMOD=0x20;		   //用定时器设置串口波特率
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;          //串口初始化
	SM0=0;
	SM1=1;
	EA=1;           //开启总中断
	ES=1;			//开启串口中断
	while(1)
	{	
		if(num==1)
		{
			switch(dat)    //判断串口接收的数据
			{
				case 1:LED=0xfe;break; //点亮第1个灯
				case 2:LED=0xfd;break;  //点亮第2个灯
				case 3:LED=0xfb;break;  //点亮第3个灯
				case 4:LED=0xf7;break;  //点亮第4个灯
				case 5:LED=0xef;break;  //点亮第5个灯
				case 6:LED=0xdf;break;  //点亮第6个灯
				case 7:LED=0xbf;break;  //点亮第7个灯
				case 8:LED=0x7f;break;  //点亮第8个灯
				case 0:LED=0xff;break;  //清空所有的灯
			//	default :break; 
			}
			ES=1;		 //打开串口中断	
			num=0;
		}
		
	}	
}
void ser() interrupt 4
{
	RI=0;
	dat=SBUF;				//接收数据SBUF，即将计算机的数据接收。
	ES=0;				    //关闭串口中断
	num=1;
}