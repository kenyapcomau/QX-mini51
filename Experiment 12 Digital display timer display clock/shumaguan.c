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
* 【程序功能】： 数码管模拟数字时钟显示				   			            			    
* 【使用说明】： 跳线帽短接数码管J6跳线
**********************************************************************************/

#include <reg52.h>
#define uchar unsigned char 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

static unsigned char second,minute,hour;
unsigned int tcount;
unsigned char m;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};
void delay(uchar i)
{
  uchar j,k; 
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display()
{
  
   P0=table[minute/10];
   P2_0 = 0;
   delay(5);
   P2_0 = 1;

   P0=(table[minute%10]& 0x7f);
   P2_1 = 0;
   delay(5);
   P2_1 = 1;

   P0=table[second/10];
   P2_2 = 0;
   delay(5);
   P2_2 = 1;

    P0=table[second%10];
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}

/*演示主程序*/
void main(void)
{
    unsigned int  a;
	TMOD=0x02;
    TH0=0x06;
    TL0=0x06;
    TR0=1;
    ET0=1;
    EA=1;
   
 while(1)
 {
 
   for(a=100;a>0;a--)
   {
     display();
   }
 
 }
}


/*
********************************************************************************
** 函数名称 ： t0(void) interrupt 1 using 0
** 函数功能 ： 定时器中断
********************************************************************************
*/

 void t0(void) interrupt 1 using 0 
  {	tcount++;

    if(tcount==4000)
	  {tcount=0;
	    second++;

		 if(second==60)
		  {second=0;
		   minute++;

		    if(minute==60)
			 {minute=0;
			  hour++;

			  if(hour==24)
			   {hour=0;
			   }
			 }
		  }
	  }
	}
