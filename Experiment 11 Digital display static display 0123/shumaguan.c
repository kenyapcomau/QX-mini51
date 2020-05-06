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
* 【程序功能】： 数码管静态显示-0123			   			            			    
* 【使用说明】： 跳帽短接数码管跳线J6
**********************************************************************************/

#include <reg52.h>
#define uchar unsigned char 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

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
  
   P0=table[0];
   P2_0 = 0;
   delay(5);
   P2_0 = 1;

   P0=table[1];
   P2_1 = 0;
   delay(5);
   P2_1 = 1;

   P0=table[2];
   P2_2 = 0;
   delay(5);
   P2_2 = 1;

    P0=table[3];
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}

/*演示主程序*/
void main(void)
{
    unsigned int  a;
   
 while(1)
 {
 
   for(a=100;a>0;a--)
   {
     display();
   }
 
 }
}