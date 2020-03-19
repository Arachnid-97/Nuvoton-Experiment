#include "bsp_time.h"
#include "bsp_uart.h"
#include "oled_extend.h"
#include "rocker_comm.h"
#include "sensors_series.h"
#include "task.h"


uint8_t g_TCount = 0;
uint8_t g_Time_1s = 0;
uint8_t g_Time_1min = 0;

/************************************************
函数名称 ： SoftwareDelay_ms
功    能 ： 软件延时——毫秒
参    数 ： Cnt ---- 延时次数
返 回 值 ： 无
*************************************************/
void SoftwareDelay_ms( uint32_t Cnt )
{
	uint16_t x,y;
	
	for(x = 0;x < Cnt;x++)
		for(y = 1240;y > 0;y--);
}

/************************************************
函数名称 ： Delay_30us
功    能 ： Timer0延时——30微秒
参    数 ： CNT ---- 延时次数
返 回 值 ： 无
*************************************************/
void Delay_30us( uint32_t Cnt )
{
	set_TR0;                             // Trigger Timer0
	while (Cnt != 0)
	{
		TH0 = RELOAD_VALUE_H;
		TL0 = RELOAD_VALUE_L;	
		while(!TF0);	
		clr_TF0;
		Cnt --;
	}
	clr_TR0;                            // Stop Timer0
}

/************************************************
函数名称 ： Timer0_Init
功    能 ： Timer0初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Timer0_Init(void)
{
	TMOD &= ~0x02;
	TMOD |= 0x01;						// 16位定时
	TMOD &= ~0x08;
	CKCON |= 0x08;						// 选择系统时钟
	TH0 = RELOAD_VALUE_H;
	TL0 = RELOAD_VALUE_L;
}

/************************************************
函数名称 ： Timer3_Init
功    能 ： Timer3初始化
参    数 ： void
返 回 值 ： 无
*************************************************/
void Timer3_Init(void)
{
	T3CON = 0x04;       // Timer3 Clock = Fsys / 16
	RH3 = 0x3C;
	RL3 = 0xB0;			// 50ms
	EIPH1 |= 0x02;
	EIP1 &= ~0x02;		// 中断优先级设置为 2
	set_ET3;			// 使能定时器 3中断
	set_TR3;			// 使能定时器 3
}

/************************************************
函数名称 ： Timer3_ISR
功    能 ： Timer3定时计数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Timer3_ISR(void) interrupt 16
{	
	g_TCount++;
	if(g_TCount >= 20)
	{
		g_TCount = 0;
		OLEDtime.Seconds = ++g_Time_1s;			// 秒
		if(60 == g_Time_1s)						// 分
		{
			g_Time_1s = 0;
			OLEDtime.Seconds = g_Time_1s;
			OLEDtime.Minute = ++g_Time_1min;
			if(60 == g_Time_1min)				// 时
			{
				g_Time_1min = 0;
				OLEDtime.Minute = g_Time_1min;
				OLEDtime.Hour++;
				if(24 == OLEDtime.Hour)
				{
					OLEDtime.Hour = 0;
					OLEDtime.Minute = 0;
				}
			}
			if(0 == OLEDtime.Minute % 30)
			{
				g_Send_flag = 1;			// 每 半小时更新一次天气信息
			}
		}
		if(0 == g_Time_1s % 3)
		{
			g_DHT11_Read_flag = 1;	// 3秒测一次
		}
		if(0 == g_Time_1s % 15)		// 界面刷新
		{
			g_OLED_Updata_flag = 1;
		}
	}
	if(g_Gather_Time)				// 遥杆采集处理
	{
		g_Gather_Time--;
	}
	if(g_Uart_time)					// 串口超时处理
	{
		g_Uart_time--;
		if(!g_Uart_time)
		{
			g_Frame_flag = 1;
		}
	}
}


/*---------------------------- END OF FILE ----------------------------*/


