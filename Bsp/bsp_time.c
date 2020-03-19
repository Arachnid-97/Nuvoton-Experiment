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
�������� �� SoftwareDelay_ms
��    �� �� �����ʱ��������
��    �� �� Cnt ---- ��ʱ����
�� �� ֵ �� ��
*************************************************/
void SoftwareDelay_ms( uint32_t Cnt )
{
	uint16_t x,y;
	
	for(x = 0;x < Cnt;x++)
		for(y = 1240;y > 0;y--);
}

/************************************************
�������� �� Delay_30us
��    �� �� Timer0��ʱ����30΢��
��    �� �� CNT ---- ��ʱ����
�� �� ֵ �� ��
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
�������� �� Timer0_Init
��    �� �� Timer0��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void Timer0_Init(void)
{
	TMOD &= ~0x02;
	TMOD |= 0x01;						// 16λ��ʱ
	TMOD &= ~0x08;
	CKCON |= 0x08;						// ѡ��ϵͳʱ��
	TH0 = RELOAD_VALUE_H;
	TL0 = RELOAD_VALUE_L;
}

/************************************************
�������� �� Timer3_Init
��    �� �� Timer3��ʼ��
��    �� �� void
�� �� ֵ �� ��
*************************************************/
void Timer3_Init(void)
{
	T3CON = 0x04;       // Timer3 Clock = Fsys / 16
	RH3 = 0x3C;
	RL3 = 0xB0;			// 50ms
	EIPH1 |= 0x02;
	EIP1 &= ~0x02;		// �ж����ȼ�����Ϊ 2
	set_ET3;			// ʹ�ܶ�ʱ�� 3�ж�
	set_TR3;			// ʹ�ܶ�ʱ�� 3
}

/************************************************
�������� �� Timer3_ISR
��    �� �� Timer3��ʱ����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void Timer3_ISR(void) interrupt 16
{	
	g_TCount++;
	if(g_TCount >= 20)
	{
		g_TCount = 0;
		OLEDtime.Seconds = ++g_Time_1s;			// ��
		if(60 == g_Time_1s)						// ��
		{
			g_Time_1s = 0;
			OLEDtime.Seconds = g_Time_1s;
			OLEDtime.Minute = ++g_Time_1min;
			if(60 == g_Time_1min)				// ʱ
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
				g_Send_flag = 1;			// ÿ ��Сʱ����һ��������Ϣ
			}
		}
		if(0 == g_Time_1s % 3)
		{
			g_DHT11_Read_flag = 1;	// 3���һ��
		}
		if(0 == g_Time_1s % 15)		// ����ˢ��
		{
			g_OLED_Updata_flag = 1;
		}
	}
	if(g_Gather_Time)				// ң�˲ɼ�����
	{
		g_Gather_Time--;
	}
	if(g_Uart_time)					// ���ڳ�ʱ����
	{
		g_Uart_time--;
		if(!g_Uart_time)
		{
			g_Frame_flag = 1;
		}
	}
}


/*---------------------------- END OF FILE ----------------------------*/


