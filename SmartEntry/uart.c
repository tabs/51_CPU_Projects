#include"uart.h"


bit Uart4busy;
bit Uart1busy;
bit Uart3busy;
unsigned char xdata re_data_buffer[1295];
unsigned char S3_Re_Buffer[100];						//接收缓冲区
unsigned int Re_Cnt = 0;											//串口3接收计数器
unsigned int Rece_Cnt;								//接收数据计数器

/*****************************************/
/* 串口1、4初始化													 */
/* 程序功能：串口1、4初始化								 */
/* 入口参数:无												   */
/* 出口参数:无												   */
/*****************************************/
void init_Uart()
{
	P_SW2 &= ~S4_S0;            //S4_S0=0 (P0.2/RxD4, P0.3/TxD4)
	ACC = P_SW1;
	ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
	P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	SCON = 0x50;								//1010000
	S3CON = 0x10;
	S4CON = 0x10;             	//00001010
	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
	T2H = (65536 - (FOSC/4/BAUD))>>8;
	AUXR = 0x14;								//T2为1T模式, 并启动定时器2 10100
	AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
	ES = 1;                     //使能串口1中断
	IE2 = 0x18;                 //使能串口3,4中断
	EA = 1;
}


/*----------------------------
UART1 中断服务程序
-----------------------------*/
/*
void Uart1_ISR() interrupt 4 using 1
{
EA=0;
	if (RI)
	{
			RI = 0;                 //清除RI位
	}
	if (TI)
	{
			TI = 0;                 //清除TI位
			Uart1busy = 0;               //清忙标志
	}
EA=1;
}

*/

/*----------------------------
UART3 中断服务程序
-----------------------------*/
void Uart3_ISR() interrupt 17 using 1
{
EA =0;
	if (S3CON & 0x01 )
	{
		if(Re_Cnt < 100)
			S3_Re_Buffer[Re_Cnt++] = S3BUF;
			Uart3HaveData = 1; 				//标志串口3有数据进来
			S3CON &= ~0x01;         //清除S3RI位	
	}
	if (S3CON & 0x02)
	{
			S3CON &= ~0x02;         //清除S3TI位
			Uart3busy = 0;               //清忙标志
	}
	EA = 1;
}


/*----------------------------
UART4 中断服务程序
-----------------------------*/
/*
void Uart4_ISR() interrupt 18 using 1
{
unsigned char dat;
EA=0;
		if (S4CON & S4RI)
	{
			dat = S4BUF;
		if(Rece_Cnt < 1295)																	
		re_data_buffer[Rece_Cnt++] = dat;
			S4CON &= ~S4RI;        																				//串口接收中断标记需软件清零
	}
			
	if (S4CON & S4TI)
	{
		S4CON&= ~S4TI;
		Uart4busy=0;
	}
EA=1;
}
*/





/*----------------------------
发送串口数据
----------------------------*/
void Uart1SendData(unsigned char dat)
{
	while (Uart1busy);               //等待前面的数据发送完成
	Uart1busy = 1;
	SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void Uart1SendString(char *s)
{
	while (*s)                  //检测字符串结束标志
	{
			Uart1SendData(*s++);         //发送当前字符
	}
}
/*----------------------------
串口3发送串口数据
----------------------------*/

void Uart3SendData( unsigned char dat )
{
	while (Uart3busy);          
	Uart3busy = 1;
	S3BUF = dat;               
}

/*----------------------------
串口3发送字符串
----------------------------*/

void Uart3SendString(char *s)
{
	while (*s)               
	{
			Uart3SendData(*s++);        
	}
}




/*----------------------------
串口4发送数据
----------------------------*/
void Uart4SendData(unsigned char Udat)
{
while(Uart4busy);
ACC = Udat;
Uart4busy=1;
S4BUF = ACC;
}


void Uart4SendString(char *s)
{
	while (*s)                 
	{
			 Uart4SendData(*s++);         
	}
}


void USART_Send(unsigned char *dat,unsigned char len){
unsigned char i;
for(i=0;i<len;i++)	
	{
			 Uart4SendData(*dat++);         
	}
}


void USART3_Send(unsigned char *dat,unsigned char len){
unsigned char i;
for(i=0;i<len;i++)	
	{

			 Uart3SendData(*dat++);

	}
}
/***********************************************
函数名称：Delay_ms
功    能：STC15系列单片机1ms延时程序
入口参数：ms:延时的毫秒数
返 回 值：无	
备    注：示波器实测：0.997ms，内部时钟：11.0592MHz           
************************************************/
void Delay_ms(unsigned int ms)
{
	unsigned int i;
	while( (ms--) != 0)
	{
		for(i = 0; i < 580; i++); 
	}             
}






















































