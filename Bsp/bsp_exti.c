#include "bsp_exti.h"
#include "bsp.h"
#include "bsp_time.h"
#include "rocker_comm.h"
#include "oled_extend.h"
#include "task.h"


/************************************************
�������� �� PinInterrupt_ISR
��    �� �� �ܽ��жϴ���
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void PinInterrupt_ISR(void) interrupt 7
{
	if(0x20 == PIF)
	{
		SoftwareDelay_ms(30);
		if(0 == SW_PIN)
		{
			if(0x00 == g_Car_Mode)
			{
				g_Car_Mode = 0x28;
				g_OLED_Updata_flag = 1;		// �����л�����ʱҪ��ʾ�Ľ���
				LED_ON;
				set_ES;				// �������� 0�ж�
			}
			else
			{
				clr_ES;				// �رմ��� 0�ж�
				g_Car_Mode = 0x00;
				g_Send_flag = 1;
				LED_OFF;
			}
			Protocol[2] = g_Car_Mode;
		}		
		PIF &= ~0x20;
	}
}


/*---------------------------- END OF FILE ----------------------------*/


