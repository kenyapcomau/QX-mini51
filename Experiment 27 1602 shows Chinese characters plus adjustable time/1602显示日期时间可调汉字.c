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
				 S1选择调节时分秒，S2加，S3减，S4确认。				 
**********************************************************************************/

#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int 
sbit lcdrs  = P0^7;	//液晶引脚定义
sbit led_cs = P0^6;
sbit lcden  = P0^5;

sbit key1 = P3^5;
sbit key2 = P3^4;
sbit key3 = P3^3;
sbit keyE = P3^2;
uchar num,key1num,count,hour,min,sec;
uchar code cg_1602[]={ 0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,
0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,
0x0f,0x09,0x09,0x0f,0x09,0x09,0x0f,0x00};//"年月日"
/*延时函数*/
void delay(uint xms)
{
	uint i,j; 
	for(i=xms;i>0;i--)
	for(j=114;j>0;j--);
}
/*1602液晶*/
void write_com(uchar com)//写命令
{
	lcdrs=0;
	lcden=0;
	P2=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar date)//写数据
{
	lcdrs=1;
	lcden=0;
	P2=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_by_xy(uchar x,uchar y)//坐标定位
{
	uchar address;
	if(y==0) 
	address=0x80+x;//y=0,写第一行
	else 
	address=0xc0+x;//y=1,写第二行
	write_com(address); 
}
/*void write_char(uchar x,uchar y,uchar date)//坐标定位显示
{
	write_by_xy(x,y);//先定位 
	write_data(date);//后写数
}*/
void write_string(uchar x,uchar y,uchar *s)//写字符串（这个指针还不大懂啊）
{
	write_by_xy(x,y);//设定显示位置 
	while(*s)//write character
	{
	P2=*s;
	write_data(*s); 
	s++;
	}
}
void write_sfm(uchar add,uchar date)//写时分秒函数
{
	uchar hour,ge;
	hour=date/10;//分解一个2位数的十位和个位
	ge=date%10;
	write_com(0x80+0x40+add);//设置显示位置
	write_data(0x30+hour);//送去液晶显示十位
	write_data(0x30+ge); //送去液晶显示个位
}
void write_cg()//写入固定汉字和符号
{
	write_com(0x40); //设定CGRAM地址 
	for(num=0;num<24;num++) //将自定义字型码写入CGRAM中 
	{ 
		write_data(cg_1602[num]); 
	}
	write_com(0x84);//年 
	{ 
		write_data(0); 
	} 
	write_com(0x87);//月 
	{ 
		write_data(1); 
	}
	write_com(0x8a);//日 
	{ 
		write_data(2); 
	}
	write_string(2,1,":");
	write_string(5,1,":");
	write_string(9,1,"QX-MCU");
}
void init_1602()//初始化
{
	led_cs=0;
	hour=22;min=33;sec=55;key1num=0;count=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_string(0,0,"2014");
	write_string(5,0,"07");
	write_string(8,0,"12");
	write_sfm(0,hour);
	write_sfm(3,min);
	write_sfm(6,sec);
	write_cg();
	TMOD=0x01;//设置定时器0工作模式1
	TH0=(65536-50000)/256;//定时器装初值
	TL0=(65536-50000)%256;
	EA=1; //开总中断
	ET0=1; //开定时器0中断
	TR0=1; //启动定时器0
}
/*键盘扫描函数*/
void keyscan()//按键扫描函数
{
	keyE =0;
	if(key1==0) 
	{
		delay(5);
		if(key1==0)//确认功能键被按下
		{ 	
			key1num++;//功能键按下次数记录
			while(!key1);//释放确认
			if(key1num==1)//第一次被按下时
			{
				TR0=0; //关闭定时器
				write_com(0x80+0x40+7);//光标定位到秒位置
				write_com(0x0f); //光标开始闪烁
			}
			if(key1num==2)//第二次按下光标闪烁定位到分钟位置
			{
				write_com(0x80+0x40+4);
			}
			if(key1num==3)//第三次按下光标闪烁定位到小时位置
			{
				write_com(0x80+0x40+1);
			}
			if(key1num==4)//第四次按下
			{
				key1num=0;//记录按键数清零
				write_com(0x0c);//取消光标闪烁
				TR0=1; //启动定时器使时钟开始走
			} 
		}
	}
	if(key1num!=0)//只有功能键被按下后，增加和减小键才有效
	{
		if(key2==0)
		{
			delay(5);
			if(key2==0)//增加键确认被按下
			{
				while(!key2);//按键释放
				if(key1num==1)//若功能键第一次按下
				{
					sec++; //则调整秒加1
					if(sec==60)//若满60后将清零
					sec=0;
					write_sfm(6,sec);//每调节一次送液晶显示一下
					write_com(0x80+0x40+7);//显示位置重新回到调节处
				}
				if(key1num==2)//若功能键第二次按下
				{
					min++;//则调整分钟加1
					if(min==60)//若满60后将清零
					min=0;
					write_sfm(3,min);//每调节一次送液晶显示一下
					write_com(0x80+0x40+4);//显示位置重新回到调节处
				}
				if(key1num==3)//若功能键第三次按下
				{
					hour++;//则调整小时加1
					if(hour==24)//若满24后将清零
					hour=0;
					write_sfm(0,hour);;//每调节一次送液晶显示一下
					write_com(0x80+0x40+1);//显示位置重新回到调节处
				}
			}
		}
		if(key3==0)
		{
			delay(5);
			if(key3==0)//确认减小键被按下
			{
				while(!key3);//按键释放
				if(key1num==1)//若功能键第一次按下
				{
					sec--;//则调整秒减1
					if(sec==-1)//若减到负数则将其重新设置为59
						sec=59;
					write_sfm(6,sec);//每调节一次送液晶显示一下
					write_com(0x80+0x40+7);//显示位置重新回到调节处
				}
				if(key1num==2)//若功能键第二次按下
				{
					min--;//则调整分钟减1
					if(min==-1)//若减到负数则将其重新设置为59
						min=59;
					write_sfm(3,min);//每调节一次送液晶显示一下
				write_com(0x80+0x40+4);//显示位置重新回到调节处
				}
				if(key1num==3)//若功能键第二次按下
				{
					hour--;//则调整小时减1
					if(hour==-1)//若减到负数则将其重新设置为23
						hour=23;
					write_sfm(0,hour);//每调节一次送液晶显示一下
					write_com(0x80+0x40+1);//显示位置重新回到调节处
				}
			}
		}
	}
}
/*主函数*/
void main()
{
	led_cs=0;
	init_1602();
	while(1)
	{
		keyscan();
	} 
}
/*中断函数*/
void timer0() interrupt 1//定时器0中断服务程序
{
	TH0=(65536-50000)/256;//再次装定时器初值
	TL0=(65536-50000)%256;
	count++; //中断次数累加
	if(count==20) //20次50毫秒为1秒
	{
		count=0;
		sec++;
		if(sec==60)//秒加到60则进位分钟
		{
			sec=0;//同时秒数清零
			min++;
			if(min==60)//分钟加到60则进位小时
			{
				min=0;//同时分钟数清零
				hour++;
				if(hour==24)//小时加到24则小时清零
				{
					hour=0;
				}
				write_sfm(0,hour);//小时若变化则重新写入
			}
			write_sfm(3,min);//分钟若变化则重新写入
		}
		write_sfm(6,sec);//秒若变化则重新写入 
	} 
}