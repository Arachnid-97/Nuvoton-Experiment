#include "rocker_comm.h"
#include "bsp_time.h"
#include "bsp_adc.h"
#include "bsp_uart.h"
#include "nrf24l01_drv.h"


uint8_t g_Gather_Time = 0;			// 采集时间
uint8_t g_Car_Mode = 0x00;			// 对应小车的远程 or 初始模式
volatile uint8_t Protocol[7] = {0x06, 0x5A, 0x00, 0x00, 0x04, 0x00, 0x00};		// 发送协议(注：第一位为字节数，只是为了迎合NRF24l串口模块)

/************************************************
函数名称 ： Voltage_Check
功    能 ： 电压检测处理
参    数 ： 无
返 回 值 ： 0 / 1
*************************************************/
bit Voltage_Check(void)
{
	uint16_t left_scope,right_scope;
	uint32_t temp = 0;
	uint32_t x_value = 0, y_value = 0;
	static uint8_t Count = 0;

#if _UART_DEBUG
    uint8_t temp_buf[6] = {0};
	temp_buf[4] = '\r';
	temp_buf[5] = '\n';

#endif /* _UART_DEBUG */

//    SoftwareDelay_ms(1);          // 稍微等待一下 ref电压稳定
#if 0
	temp = VDD_Read();
	
#else
	temp = MEAS_VOL;
	
#endif

	Enable_ADC_AIN0;
//    ADC_Read(temp);                     // 先转换一次（第一次读值不准）
	x_value = ADC_Read(temp);
	
	Enable_ADC_AIN1;
//    ADC_Read(temp);                     // 先转换一次（第一次读值不准）
	y_value = ADC_Read(temp);;

	ADCCON1 &= ~0x01;				// 关闭 ADC

#if _UART_DEBUG
//    temp_buf[0] = (temp / 1000) % 10 + '0';
//    temp_buf[1] = (temp / 100) % 10 + '0';
//    temp_buf[2] = (temp / 10) % 10 + '0';
//    temp_buf[3] = (temp) % 10 + '0';
//    DUBUG_STRING(temp_buf, 6);

    temp_buf[0] = (x_value / 1000) % 10 + '0';
    temp_buf[1] = (x_value / 100) % 10 + '0';
    temp_buf[2] = (x_value / 10) % 10 + '0';
    temp_buf[3] = (x_value) % 10 + '0';
    DUBUG_STRING(temp_buf, 6);
	
    temp_buf[0] = (y_value / 1000) % 10 + '0';
    temp_buf[1] = (y_value / 100) % 10 + '0';
    temp_buf[2] = (y_value / 10) % 10 + '0';
    temp_buf[3] = (y_value) % 10 + '0';
    DUBUG_STRING(temp_buf, 6);

#endif /* _UART_DEBUG */

	/* 浮动误差 ±30 */
	left_scope = 0 + 30;
	right_scope = temp - 30;
	
	if(x_value < left_scope && (y_value < right_scope && y_value > left_scope))
	{
		Protocol[3] = UP;
		Count = 0;
//		UART0_Printf("UP");
		DUBUG_STRING("UP",2);
	}
	else if(x_value > right_scope && (y_value < right_scope && y_value > left_scope))
	{
		Protocol[3] = DOWN;
		Count = 0;
//		UART0_Printf("DOWN");
		DUBUG_STRING("DOWN",4);
	}	
	else if(y_value > right_scope && (x_value < right_scope && x_value > left_scope))
	{
		Protocol[3] = LEFT;
		Count = 0;
//		UART0_Printf("LEFT");
		DUBUG_STRING("LEFT",4);
	}
	else if(y_value < left_scope && (x_value < right_scope && x_value > left_scope))
	{
		Protocol[3] = RIGHT;
		Count = 0;
//		UART0_Printf("RIGHT");
		DUBUG_STRING("RIGHT",5);
	}
	
	/* 对角方向没实现 */
	else if(y_value > right_scope && x_value < left_scope)
	{
		Protocol[3] = LEFT_UP;
		Count = 0;
//		UART0_Printf("LEFT_UP");
		DUBUG_STRING("LEFT_UP",7);
	}
	else if(y_value > right_scope && x_value > right_scope)
	{
		Protocol[3] = LEFT_DOWN;
		Count = 0;
//		UART0_Printf("LEFT_DOWN");
		DUBUG_STRING("LEFT_DOWN",9);
	}
	else if(y_value < left_scope && x_value < left_scope)
	{
		Protocol[3] = RIGHT_UP;
		Count = 0;
//		UART0_Printf("RIGHT_UP");
		DUBUG_STRING("RIGHT_UP",8);
	}
	else if(y_value < left_scope && x_value > right_scope)
	{
		Protocol[3] = RIGHT_DONW;
		Count = 0;
//		UART0_Printf("RIGHT_DONW");
		DUBUG_STRING("RIGHT_DONW",10);
	}
	else
	{
		Protocol[3] = CENTER;
//		UART0_Printf("CENTER");
//		DUBUG_STRING("CENTER",6);
		if(Count < 8)
		{
			Count++;
		}
	}
	
	if(8 == Count)
	{
		return 0;		// 无动作，发送 7次停止
	}
	DUBUG_STRING("\r\n",2);
	
	return 1;
}

/************************************************
函数名称 ： Data_Verify
功    能 ： 校验
参    数 ： 无
返 回 值 ： temp ---- 校验值
*************************************************/
uint8_t Data_Verify( uint8_t *Buf )
{
	uint8_t i;
	uint16_t temp = 0;
	
	for(i = 1;i < 5;i++)
	{
		temp += *(Buf + i);
	}
	temp = (uint8_t)(~temp);
	
	return temp;
}

/************************************************
函数名称 ： Rocker_KeyConfig
功    能 ： 遥杆按键配置
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Rocker_KeyConfig(void)
{
	P15_Quasi_Mode;			// SW
	
	/* 使能 P1.5管脚中断 */
	Enable_INT_Port1;
	Enable_BIT5_FallEdge_Trig;
	
	EIPH &= ~0x02;
	EIP |= 0x02;			// 中断优先级设置为 1
	
	set_EPI;				// 开总管脚中断
}


/*---------------------------- END OF FILE ----------------------------*/


