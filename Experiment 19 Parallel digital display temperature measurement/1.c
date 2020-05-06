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
* 【程序功能】： 数码管显示温度			   			            			    
* 【使用说明】： 注意：在接18B20温度传感器时，半圆面向开发板外，平面向开发板内
 				 不能插反，插反18B20表面会热量会急剧上升，此时应立即取下18B20
				 跳线帽短接数码管J6跳线
**********************************************************************************/

#include <reg51.h>                 //DS18B20温度传感器程序 
code unsigned char seg7code[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,
                               0x82,0xf8,0x80,0x90,0xff}; //不带小数点的共阳数码管段码
code unsigned char seg7codeB[]={0x40,0x79,0x24,0x30,0x19,0x12,
                               0x02,0x78,0x00,0x10,0xff}; //带小数点的共阳数码管段码

sbit DQ=P3^7; //数据传输线接单片机的相应的引脚 
unsigned char tempL=0; //设全局变量
unsigned char tempH=0; 
unsigned int sdata;//测量到的温度的整数部分
unsigned char xiaoshu1;//小数第一位
unsigned char xiaoshu2;//小数第二位
unsigned char xiaoshu;//两位小数
bit  fg=1;        //温度正负标志



//******************延时子程序 *******************************

//这个延时程序的具体延时时间是time=i*8+10,适用于小于2ms的延时

//************************************************************
void delay(unsigned char i)
{
 for(i;i>0;i--);
}


//***********************************************************

//                     延时子程序

//************************************************************
void delay1ms()
{
 unsigned char i;
 for(i=124;i>0;i--);  //延时124*8+10=1002us
}



//*****************************初始化程序 *********************************//

void Init_DS18B20(void) 
{
 unsigned char x=0;
 DQ=1; //DQ先置高 
 delay(8); //稍延时
 DQ=0; //发送复位脉冲 
 delay(80); //延时（>480us) 
 DQ=1; //拉高数据线 
 delay(5); //等待（15~60us) 
 x=DQ; //用X的值来判断初始化有没有成功，18B20存在的话X=0，否则X=1 
 delay(20); 
} 


//**********读一个字节************//

ReadOneChar(void)  //主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
unsigned char i=0; //每个读周期最短的持续时间为60us，各个读周期之间必须有1us以上的高电平恢复期
unsigned char dat=0; 
for (i=8;i>0;i--) //一个字节有8位 
{
 DQ=1; 
 delay(1); 
 DQ=0;
 dat>>=1; 
 DQ=1; 
 if(DQ) 
 dat|=0x80; 
 delay(4);
} 
return(dat);
} 



//*********************** **写一个字节**************************//

void WriteOneChar(unsigned char dat) 
{ 
  unsigned char i=0; //数据线从高电平拉至低电平，产生写起始信号。15us之内将所需写的位送到数据线上，
  for(i=8;i>0;i--) //在15~60us之间对数据线进行采样，如果是高电平就写1，低写0发生。 
  {
   DQ=0; //在开始另一个写周期前必须有1us以上的高电平恢复期。 
   DQ=dat&0x01; 
   delay(5); 
   DQ=1; 
   dat>>=1;
  } 
  delay(4);
} 


//读温度值（低位放tempL;高位放tempH;）
void ReadTemperature(void) 
{ 
 Init_DS18B20(); //初始化
 WriteOneChar(0xcc); //跳过读序列号的操作
 WriteOneChar(0x44); //启动温度转换
 delay(125); //转换需要一点时间，延时 
 Init_DS18B20(); //初始化
 WriteOneChar(0xcc); //跳过读序列号的操作 
 WriteOneChar(0xbe); //读温度寄存器（头两个值分别为温度的低位和高位） 
 tempL=ReadOneChar(); //读出温度的低位LSB
 tempH=ReadOneChar(); //读出温度的高位MSB 
  
		if(tempH>0x7f)      //最高位为1时温度是负
		{
		 tempL=~tempL;         //补码转换，取反加一
		 tempH=~tempH+1;       
		 fg=0;      //读取温度为负时fg=0
       }
		sdata = tempL/16+tempH*16;      //整数部分
		xiaoshu1 = (tempL&0x0f)*10/16; //小数第一位
		xiaoshu2 = (tempL&0x0f)*100/16%10;//小数第二位
		xiaoshu=xiaoshu1*10+xiaoshu2; //小数两位
}


//********************显示函数****************
void Led(unsigned int date)
{ 
 if(fg==1)
 {
	 P2=0xfe;     //P1.0=0，选通第一位
	 P0=seg7code[date/10];  //十位数，查表，输出
	 delay1ms();
	 P0=0xff;        //消隐
	
	 P2=0xfd;     //P1.1=0,选通第二位，个位数
	 P0=seg7codeB[date%10];
	 delay1ms();
	 P0=0xff;       //消隐
	
	 P2=0xfb;     //P1.3=0,选通第三位，小数点第一位
	 P0=seg7code[xiaoshu1];
	 delay1ms();
	 P0=0xff;         //消隐
	
	 P2=0xf7;     //P1.3=0,选通第四位，小数点第二位
	 P0=seg7code[xiaoshu2];
	 delay1ms();
	 P0=0xff;       //消隐
 }

   if(fg==0)  //温度为负时显示的数据
   {
	 P2=0xfe;     //P1.0=0，选通第一位
	 P0=seg7code[11];  //显示负号
	 delay1ms();
	 P0=0xff;        //消隐
	
	 P2=0xfd;     //P1.1=0,选通第二位，十位数
	 P0=seg7code[date/10];
	 delay1ms();
	 P0=0xff;       //消隐
	
	 P2=0xfb;     //P1.3=0,选通第三位，个位数
	 P0=seg7codeB[date%10];
	 delay1ms();
	 P0=0xff;         //消隐
	
	 P2=0xf7;     //P1.3=0,选通第四位，小数点第一位
	 P0=seg7code[xiaoshu1];
	 delay1ms();
	 P0=0xff;       //消隐
   }
}


//**************************** //主程序 


main()
{
 while(1)
 {
  ReadTemperature();
  Led(sdata);
 }
}