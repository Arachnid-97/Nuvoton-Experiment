#include "task.h"
#include "bsp_uart.h"
#include "bsp_time.h"
#include "oled12864_drv.h"
#include "oled_extend.h"
#include "sensors_series.h"
#include "nrf24l01_drv.h"
#include "rocker_comm.h"


OledTime_TypeDef OLEDtime;
OledDate_TypeDef OLEDDate;
OledWeather_TypeDef OLEDWeather;

bit g_Send_flag = 0;

/************************************************
函数名称 ： Data_Check
功    能 ： 数据校验
参    数 ： Getbuf ---- 输入的参数
			Cmpack ---- 校验的数据
			Len ---- 输入长度
返 回 值 ： 0 / 1
*************************************************/
static uint8_t Data_Check( const uint8_t *Getbuf, char *Cmpack, uint16_t Len )
{
	uint16_t q;
	uint16_t i = 0;	

	do
	{
		for(q = 0;*(Getbuf + i + q) == *(Cmpack + q);q++)
		{
			if(q + 1 == Len)
			{
				return 1;
			}
		}		
		i++;
	}while(i < Len);
	
	return 0;
}

/************************************************
函数名称 ： Time_correct
功    能 ： 时间校准
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void Time_correct(void)
{
	OLEDtime.Hour = 5;
	OLEDtime.Minute = 0;
	OLEDtime.Seconds = 0;
	
	OLEDDate.Week = 1;	
	OLEDDate.Day = 28;
	OLEDDate.Month = 5;
	OLEDDate.Year = 1997;

	OLEDWeather.Today = 8;
	OLEDWeather.Tomorrow = 8;
	OLEDWeather.Acquired = 8;

	/* 录入 MCU计时时间 */
	g_Time_1min = OLEDtime.Minute;
	g_Time_1s = OLEDtime.Seconds;
}

/************************************************
函数名称 ： Status_display
功    能 ： 状态栏状态选择显示
参    数 ： Status ---- 状态
返 回 值 ： 无
*************************************************/
static void Status_display( uint8_t Status )
{
	if(Status == SIGNAL_STATUS)
	{
//		OLED_P16x16(16*0, 0, 7, DISABLE, DISABLE);
//		OLED_P16x16(16*1, 0, 6, DISABLE, DISABLE);		
		OLED_P16x16(16*0, 0, 12, DISABLE, DISABLE);		// 信号显示
	}
	if(Status == G24_STATUS)
	{
		OLED_P16x16(16*5, 0, 13, DISABLE, DISABLE);		// 显示 2.4G图标
	}
	if(Status == BLUETOOTH_STATUS)
	{
		OLED_P16x16(16*6, 0, 5, DISABLE, DISABLE);		// 显示蓝牙图标
	}
		
	switch(Status)
	{
		case WIFI1_STATUS:
			OLED_P16x16(16*4, 0, 11, DISABLE, DISABLE);		// 显示WIFI图标
			break;

		case WIFI2_STATUS:
			OLED_P16x16(16*4, 0, 10, DISABLE, DISABLE);		// 显示WIFI图标
			break;

		case WIFI3_STATUS:
			OLED_P16x16(16*4, 0, 9, DISABLE, DISABLE);		// 显示WIFI图标
			break;

		case WIFI4_STATUS:
			OLED_P16x16(16*4, 0, 8, DISABLE, DISABLE);		// 显示WIFI图标
			break;
		
		default:			
			break;		
	}
}

/************************************************
函数名称 ： Main_Interface_APP
功    能 ： 显示主界面
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Main_Interface_APP(void)
{	
	OLED_P16x16(16*7, 0, 4, DISABLE, DISABLE);		// 显示电量图标
	
	OLED_Starting_Up();
	SoftwareDelay_ms(600);
	
	Time_correct();

	/* 申请更新天气信息 */		// 改到开机过程中申请，以节省时间
//	g_Frame_flag = 0;
//	g_RxCnt = 0;
//	UART0_Printf("GET 2");
	Weather_Updata();			// 查看是否有获取成功了
	
	OLED_ShowPrintf(15, 4, "Please wait..", OLED_FONT_EIGHT, DISABLE, DISABLE);
	
	Status_display(WIFI4_STATUS);
	
	OLED_Row_Clear(4, 4, OLED_CLS);
//	SoftwareDelay_ms(100);
	
	/* 显示最下方默认进度点 */
	OLED_P8x8(50, 7, 3, DISABLE, DISABLE);
	OLED_P8x8(60, 7, 4, DISABLE, DISABLE);
	OLED_P8x8(70, 7, 3, DISABLE, DISABLE);

	/* 最下方时间 */
	OLED_ShowNum(2, 7, OLEDtime.Hour, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_ShowChar(18, 7, ':', OLED_FONT_EIGHT, DISABLE);
	OLED_ShowNum(26, 7, OLEDtime.Minute, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	
	/* 最下方温度 */
	Read_DHT11(&DHT11_Data);
	OLED_ShowNum(86, 7, DHT11_Data.temp_int, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_ShowChar(102, 7, '.', OLED_FONT_EIGHT, DISABLE);
	OLED_ShowNum(110 - 1, 7, DHT11_Data.temp_deci, 1, OLED_FONT_EIGHT, DISABLE, DISABLE);
	OLED_P8x8(118, 7, 5, DISABLE, DISABLE);

	/* 显示初始界面图像 */
	OLED_Period_Show(0*30 + 9, 2, OLEDDate.Week + 0, OLED_FONT_EIGHT, DISABLE);	// 今天
//	OLED_P32x32(0*32 + 8, 3, 0, DISABLE, DISABLE);
	
	OLED_Period_Show(1*30 + 19, 2, OLEDDate.Week + 1, OLED_FONT_EIGHT, DISABLE);// 明天
//	OLED_P32x32(1*32 + 16, 3, 3, DISABLE, DISABLE);
	
	OLED_Period_Show(2*30 + 29, 2, OLEDDate.Week + 2, OLED_FONT_EIGHT, DISABLE);// 后天
//	OLED_P32x32(2*32 + 24, 3, 1, DISABLE, DISABLE);

	OLED_P32x32(0*32 + 8, 3, OLEDWeather.Today, ENABLE, DISABLE);
	OLED_P32x32(1*32 + 16, 3, OLEDWeather.Tomorrow, ENABLE, DISABLE);
	OLED_P32x32(2*32 + 24, 3, OLEDWeather.Acquired, ENABLE, DISABLE);	// 对应的天气图像
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
函数名称 ： First_Menu
功    能 ： 第一菜单界面
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void First_Menu(void)
{
	/* 温度 */
	OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, ENABLE, DISABLE);
	OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, ENABLE, DISABLE);	
	OLED_P32x32(2*16 + 5, 3, 7, ENABLE, DISABLE);
	OLED_P8x8(2*16 + 5 + 2, 2, 5, ENABLE, DISABLE);

	/* 湿度 */
	OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, ENABLE, DISABLE);
	OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, ENABLE, DISABLE);	
	OLED_P32x32(6*16, 3, 6, ENABLE, DISABLE);
	OLED_String_Move(6*16 + 2, 2,"%RH", OLED_FONT_EIGHT, ENABLE, DISABLE);	
	
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
函数名称 ： Second_Menu
功    能 ： 第二菜单界面
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void Second_Menu(void)
{
	/* 星期 */
	OLED_Period_Show(0*30 + 9, 2, OLEDDate.Week + 0, OLED_FONT_EIGHT, DISABLE);	// 今天
	OLED_Period_Show(1*30 + 19, 2, OLEDDate.Week + 1, OLED_FONT_EIGHT, DISABLE);// 明天
	OLED_Period_Show(2*30 + 29, 2, OLEDDate.Week + 2, OLED_FONT_EIGHT, DISABLE);// 后天

	/* 天气图像 */
	OLED_P32x32(0*32 + 8, 3, OLEDWeather.Today, ENABLE, DISABLE);
	OLED_P32x32(1*32 + 16, 3, OLEDWeather.Tomorrow, ENABLE, DISABLE);
	OLED_P32x32(2*32 + 24, 3, OLEDWeather.Acquired, ENABLE, DISABLE);	// 对应的天气图像
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
函数名称 ： Third_Menu
功    能 ： 第三菜单界面
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void Third_Menu(void)
{
	/* 年份 */
	OLED_Period_Show(0*6 + 18, 2, OLEDDate.Week, OLED_FONT_EIGHT, DISABLE);		// 星期
	
	OLED_Num_Move(5*6 + 18, 2, OLEDDate.Day, 2, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);
	OLED_String_Move(7*6 + 18, 2,"/", OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_Num_Move(8*6 + 18, 2, OLEDDate.Month, 2, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);
	OLED_String_Move(10*6 + 18, 2,"/", OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_Num_Move(11*6 + 18, 2, OLEDDate.Year, 4, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);	
	
	/* 时间 */
	OLED_P16x32(0*16 + 7, 3, OLEDtime.Hour / 10, ENABLE, DISABLE);
	OLED_P16x32(1*16 + 7, 3, OLEDtime.Hour % 10, ENABLE, DISABLE);	
	OLED_P16x32(2*16 + 7, 3, 10, ENABLE, DISABLE);		// " : "时间点符号
	OLED_P16x32(2*16 + 7 + 9, 3, OLEDtime.Minute / 10, ENABLE, DISABLE);
	OLED_P16x32(3*16 + 7 + 9, 3, OLEDtime.Minute % 10, ENABLE, DISABLE);	
	OLED_P16x32(4*16 + 7 + 9, 3, 10, ENABLE, DISABLE);	// " : "时间点符号
	OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, ENABLE, DISABLE);
	OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, ENABLE, DISABLE);
	
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
函数名称 ： Menu_Interface_APP
功    能 ： 显示菜单界面
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Menu_Interface_APP(void)
{
	uint8_t i,j,n = 128;
	static uint8_t Menu_grade = 3;
	static bit lock = 0;

#if 1	
	if(g_OLED_Updata_flag)
	{
		g_OLED_Updata_flag = 0;

		OLED_ShowNum(2, 7, OLEDtime.Hour, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
//		OLED_ShowChar(18, 7, ':', OLED_FONT_EIGHT, DISABLE);
		OLED_ShowNum(26, 7, OLEDtime.Minute, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);

		if(0 == lock || 0x28 != g_Car_Mode)
		{
			/* 画面转屏 */
			while(n--)
			{
				for(i = 0;i < OLED_BUFF_ROW_SIZE;i++)
				{
					for(j = 1;j < 128;j++)
					{
	//					OLED_GRAM[j - 1][i] = OLED_GRAM[j][i];
						OLED_GRAM[i][j - 1] = OLED_GRAM[i][j];
						
					}
				}
				if(n % 16 == 0)
					OLED_Refresh(OLED_BUFFER_ROW);
			}
			lock = 0;
		}		
		if(0x28 == g_Car_Mode)
		{
			lock = 1;
			Menu_grade = 1;
		}

		switch(Menu_grade)
		{
			case 1:						// 实时温湿
				First_Menu();
				OLED_P8x8(50, 7, 4, DISABLE, DISABLE);
				OLED_P8x8(60, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(70, 7, 3, DISABLE, DISABLE);

				Menu_grade++;
				break;

			case 2:						// 天气情况
				Second_Menu();
				OLED_P8x8(50, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(60, 7, 4, DISABLE, DISABLE);
				OLED_P8x8(70, 7, 3, DISABLE, DISABLE);
			
				Menu_grade++;
				break;

			case 3:						// 当前时间
				Third_Menu();
				OLED_P8x8(50, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(60, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(70, 7, 4, DISABLE, DISABLE);
			
				Menu_grade = 1;
				break;
			
			default:
				break;		
		}
	}

#endif
	
	if(1 == Menu_grade)
	{
		/* 更新秒 */
		OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, DISABLE, DISABLE);
		OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, DISABLE, DISABLE);

		OLED_P16x32(3*16 + 7 + 9, 3, OLEDtime.Minute % 10, ENABLE, DISABLE);			
		OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, ENABLE, DISABLE);
		OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, ENABLE, DISABLE);	// 更新移动时的显示
	}
	else if(2 == Menu_grade)
	{
		/* 温度 */
		OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, DISABLE, DISABLE);
		OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, DISABLE, DISABLE);
		
		OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, ENABLE, DISABLE);
		OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, ENABLE, DISABLE);	


		/* 湿度 */
		OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, DISABLE, DISABLE);
		OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, DISABLE, DISABLE);
		
		OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, ENABLE, DISABLE);
		OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, ENABLE, DISABLE);
	}
}

/************************************************
函数名称 ： Hygrothermograph_Updata
功    能 ： 温湿更新
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Hygrothermograph_Updata(void)
{
	if(g_DHT11_Read_flag)
	{
		g_DHT11_Read_flag = 0;
		if(Read_DHT11(&DHT11_Data))
		{				
			OLED_ShowNum(86, 7, DHT11_Data.temp_int, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
//				OLED_ShowChar(102, 7, '.', OLED_FONT_EIGHT, DISABLE);
			OLED_ShowNum(110 - 1, 7, DHT11_Data.temp_deci, 1, OLED_FONT_EIGHT, DISABLE, DISABLE);
//				OLED_P8x8(118, 7, 5, DISABLE);

/*
			DUBUG_PRINTF("Humidity = %bd.%bd ％RH\r\nTemperature = %bd.%bd ℃ \r\n",\
			DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
*/
		}
	}
}

/************************************************
函数名称 ： Weather_Updata
功    能 ： 天气更新
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Weather_Updata(void)
{
	weather_content *weather_data = NULL;
	
	if(g_Frame_flag)
	{
//		UART0_SendString(g_ReceiveBuf, g_RxCnt);
		g_Frame_flag = 0;
		g_RxCnt = 0;
		weather_data = (weather_content *)g_ReceiveBuf;
		if(Data_Check(weather_data->region_name, REGION_NAME, sizeof(REGION_NAME) / sizeof(uint8_t) - 1))
		{
			OLEDtime.Hour = weather_data->current_time[0];
			OLEDtime.Minute = weather_data->current_time[1];
			OLEDtime.Seconds = weather_data->current_time[2];
			
			OLEDDate.Week = weather_data->week;	
			OLEDDate.Day = REVERSE_BYTES_UINT16(weather_data->current_date[2]);
			OLEDDate.Month = REVERSE_BYTES_UINT16(weather_data->current_date[1]);
			OLEDDate.Year = REVERSE_BYTES_UINT16(weather_data->current_date[0]);

			if(OLEDtime.Hour >= 5 && OLEDtime.Hour <= 17)		// 白天
			{
				OLEDWeather.Today = weather_data->weather_code[0];
//				OLEDWeather.Tomorrow = weather_data->weather_code[2];
//				OLEDWeather.Acquired = weather_data->weather_code[4];
			}
			else if((OLEDtime.Hour > 17 && OLEDtime.Hour < 24)
						|| (OLEDtime.Hour >= 0 && OLEDtime.Hour < 5))	// 晚间
			{
				OLEDWeather.Today = weather_data->weather_code[1];
//				OLEDWeather.Tomorrow = weather_data->weather_code[3];
//				OLEDWeather.Acquired = weather_data->weather_code[5];
			}
			OLEDWeather.Tomorrow = weather_data->weather_code[2];
			OLEDWeather.Acquired = weather_data->weather_code[4];

			/* 录入 MUC计时时间 */
			g_Time_1min = OLEDtime.Minute;
			g_Time_1s = OLEDtime.Seconds;
		}
	}
	if(g_Send_flag)
	{
		g_Send_flag = 0;
		UART0_Printf("GET 2");	
	}
}

/************************************************
函数名称 ： Car_Control
功    能 ： 小车数据控制处理
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Car_Control(void)
{
	if(0x28 == g_Car_Mode)
	{
		if(!g_Gather_Time)
		{
			if(0 != Voltage_Check())
			{
				Protocol[6] = Data_Verify(&Protocol[1]);
				DUBUG_STRING(&Protocol[1], 6);
				NRF24L_SendPacket(&Protocol, TX_PLOAD_WIDTH);
			}
			g_Gather_Time = 4;			// 约 200ms回检发送一次
		}
	}
}


/*---------------------------- END OF FILE ----------------------------*/


