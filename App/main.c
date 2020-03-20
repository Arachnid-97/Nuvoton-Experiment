#include "bsp.h"
#include "bsp_time.h"
#include "bsp_uart.h"
#include "bsp_adc.h"
#include "bsp_eeprom.h"
#include "rocker_comm.h"
#include "sensors_series.h"
#include "oled12864_drv.h"
#include "oled_extend.h"
#include "nrf24l01_drv.h"
#include "task.h"


/* main config */
#define _MAIN_INTERFACE			1
#define _MAIN_ROCKER			1
#define _MAIN_DHT11				1
#define _MAIN_WEATHER			1

/************************************************
�������� �� System_Start
��    �� �� ϵͳ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void System_Start(void)
{
	clr_EA;
	
    Bsp_Init();
	Timer3_Init();
	Timer0_Init();
    UART0_Timer1_Init();
	ADC_Config();
	Rocker_KeyConfig();
	OLED_Init();
	NRF24L_Init();
	
	set_EA;
}

/************************************************
�������� �� main
��    �� �� ���������
��    �� �� ��
�� �� ֵ �� int
*************************************************/
int main(void)
{
	uint8_t len = 0;

    System_Start();

//    LED_ON;
	SoftwareDelay_ms(100);
    LED_OFF;

//	DUBUG_PRINTF("Power Value = %d\n",VDD_Read());
	len = sizeof(weather_content);
	
#if _MAIN_INTERFACE
	Main_Interface_APP();
	g_OLED_Updata_flag = 0;
#endif /* _MAIN_INTERFACE */

    while(1)
    {

#if 1
		
	#if _MAIN_INTERFACE
		/* ����ˢ�� */
		Menu_Interface_APP();
	#endif /* _MAIN_INTERFACE */
		
	#if _MAIN_ROCKER
		/* С������ */
		Car_Control();
	#endif /* _MAIN_CAR */
		
	#if _MAIN_DHT11
		/* ʵʱ��ʪ���� */
		Hygrothermograph_Updata();
	#endif /* _MAIN_DHT11 */
		
	#if _MAIN_WEATHER
		/* �������ݴ��� */
		Weather_Updata();
		
	#endif /* _MAIN_UART */
		
#endif
    }
}


/*---------------------------- END OF FILE ----------------------------*/


