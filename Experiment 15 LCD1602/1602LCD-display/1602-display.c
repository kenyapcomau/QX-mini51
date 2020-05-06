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
* 【程序功能】： 1602液晶显示			   			            			    
* 【使用说明】： 1.使用1602液晶前先拔掉数码管J6跳帽，数码管与液晶不能共用
                 2.将1602液晶接到L2排母上，液晶面向开发板外部
				 3.若程序烧写完成后，1602液晶无显示，请调节开发板右下方的电位器RV1				 
**********************************************************************************/

# include <reg51.h>
# include <intrins.h>
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL;

sbit rs=P0^7;
sbit rw=P0^6;
sbit ep=P0^5;

BYTE code dis1 []={"     QXMCU     "};
BYTE code dis2 []={"HTTP://QXMCU.COM"};

void delay (int m)
{
 unsigned char i,j;
 for (i=0;i<m;i++)
 for (j=0;j<253;j++);
}

 BOOL lcd_bz()
{
  BOOL result;
  rs=0;           // 读忙信号
  rw=1;
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (BOOL)(P2&0x80);
  ep=0;
  return result;
}

void lcd_wcmd (BYTE cmd)
{
  while (lcd_bz());
  rs=0;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=cmd ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_pos (BYTE pos)
{
  lcd_wcmd (pos|0x80);
}

void lcd_wdat (BYTE dat)
{
   while (lcd_bz());
  rs=1;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=dat ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_init ()
{
  lcd_wcmd (0x38);
  delay (1);
  lcd_wcmd (0x0c);
  delay (1);
  lcd_wcmd (0x06);
  delay (1);
  lcd_wcmd (0x01);
  delay (1);
}

main ()
{
  BYTE i;
  lcd_init ();
  delay (10);

  lcd_pos (0);
  i=0;
  while (dis1 [i]!='\0')
   {
     lcd_wdat (dis1 [i]);
     i++;	
   } 

   lcd_pos (0x40);
  i=0;
  while (dis2 [i]!='\0')
   {
     lcd_wdat (dis2 [i]);
     i++;	
   } 
   while (1);
} 