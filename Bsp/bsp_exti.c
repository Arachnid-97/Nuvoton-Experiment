#include "bsp_exti.h"
#include "bsp.h"
#include "bsp_time.h"
#include "rocker_comm.h"
#include "oled_extend.h"
#include "task.h"


/************************************************
函数名称 ： PinInterrupt_ISR
功    能 ： 管脚中断处理
参    数 ： 无
返 回 值 ： 无
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
				g_OLED_Updata_flag = 1;		// 立即切换控制时要显示的界面
				LED_ON;
				set_ES;				// 开启串口 0中断
			}
			else
			{
				clr_ES;				// 关闭串口 0中断
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


