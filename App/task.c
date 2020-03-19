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
�������� �� Data_Check
��    �� �� ����У��
��    �� �� Getbuf ---- ����Ĳ���
			Cmpack ---- У�������
			Len ---- ���볤��
�� �� ֵ �� 0 / 1
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
�������� �� Time_correct
��    �� �� ʱ��У׼
��    �� �� ��
�� �� ֵ �� ��
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

	/* ¼�� MCU��ʱʱ�� */
	g_Time_1min = OLEDtime.Minute;
	g_Time_1s = OLEDtime.Seconds;
}

/************************************************
�������� �� Status_display
��    �� �� ״̬��״̬ѡ����ʾ
��    �� �� Status ---- ״̬
�� �� ֵ �� ��
*************************************************/
static void Status_display( uint8_t Status )
{
	if(Status == SIGNAL_STATUS)
	{
//		OLED_P16x16(16*0, 0, 7, DISABLE, DISABLE);
//		OLED_P16x16(16*1, 0, 6, DISABLE, DISABLE);		
		OLED_P16x16(16*0, 0, 12, DISABLE, DISABLE);		// �ź���ʾ
	}
	if(Status == G24_STATUS)
	{
		OLED_P16x16(16*5, 0, 13, DISABLE, DISABLE);		// ��ʾ 2.4Gͼ��
	}
	if(Status == BLUETOOTH_STATUS)
	{
		OLED_P16x16(16*6, 0, 5, DISABLE, DISABLE);		// ��ʾ����ͼ��
	}
		
	switch(Status)
	{
		case WIFI1_STATUS:
			OLED_P16x16(16*4, 0, 11, DISABLE, DISABLE);		// ��ʾWIFIͼ��
			break;

		case WIFI2_STATUS:
			OLED_P16x16(16*4, 0, 10, DISABLE, DISABLE);		// ��ʾWIFIͼ��
			break;

		case WIFI3_STATUS:
			OLED_P16x16(16*4, 0, 9, DISABLE, DISABLE);		// ��ʾWIFIͼ��
			break;

		case WIFI4_STATUS:
			OLED_P16x16(16*4, 0, 8, DISABLE, DISABLE);		// ��ʾWIFIͼ��
			break;
		
		default:			
			break;		
	}
}

/************************************************
�������� �� Main_Interface_APP
��    �� �� ��ʾ������
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void Main_Interface_APP(void)
{	
	OLED_P16x16(16*7, 0, 4, DISABLE, DISABLE);		// ��ʾ����ͼ��
	
	OLED_Starting_Up();
	SoftwareDelay_ms(600);
	
	Time_correct();

	/* �������������Ϣ */		// �ĵ��������������룬�Խ�ʡʱ��
//	g_Frame_flag = 0;
//	g_RxCnt = 0;
//	UART0_Printf("GET 2");
	Weather_Updata();			// �鿴�Ƿ��л�ȡ�ɹ���
	
	OLED_ShowPrintf(15, 4, "Please wait..", OLED_FONT_EIGHT, DISABLE, DISABLE);
	
	Status_display(WIFI4_STATUS);
	
	OLED_Row_Clear(4, 4, OLED_CLS);
//	SoftwareDelay_ms(100);
	
	/* ��ʾ���·�Ĭ�Ͻ��ȵ� */
	OLED_P8x8(50, 7, 3, DISABLE, DISABLE);
	OLED_P8x8(60, 7, 4, DISABLE, DISABLE);
	OLED_P8x8(70, 7, 3, DISABLE, DISABLE);

	/* ���·�ʱ�� */
	OLED_ShowNum(2, 7, OLEDtime.Hour, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_ShowChar(18, 7, ':', OLED_FONT_EIGHT, DISABLE);
	OLED_ShowNum(26, 7, OLEDtime.Minute, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	
	/* ���·��¶� */
	Read_DHT11(&DHT11_Data);
	OLED_ShowNum(86, 7, DHT11_Data.temp_int, 2, OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_ShowChar(102, 7, '.', OLED_FONT_EIGHT, DISABLE);
	OLED_ShowNum(110 - 1, 7, DHT11_Data.temp_deci, 1, OLED_FONT_EIGHT, DISABLE, DISABLE);
	OLED_P8x8(118, 7, 5, DISABLE, DISABLE);

	/* ��ʾ��ʼ����ͼ�� */
	OLED_Period_Show(0*30 + 9, 2, OLEDDate.Week + 0, OLED_FONT_EIGHT, DISABLE);	// ����
//	OLED_P32x32(0*32 + 8, 3, 0, DISABLE, DISABLE);
	
	OLED_Period_Show(1*30 + 19, 2, OLEDDate.Week + 1, OLED_FONT_EIGHT, DISABLE);// ����
//	OLED_P32x32(1*32 + 16, 3, 3, DISABLE, DISABLE);
	
	OLED_Period_Show(2*30 + 29, 2, OLEDDate.Week + 2, OLED_FONT_EIGHT, DISABLE);// ����
//	OLED_P32x32(2*32 + 24, 3, 1, DISABLE, DISABLE);

	OLED_P32x32(0*32 + 8, 3, OLEDWeather.Today, ENABLE, DISABLE);
	OLED_P32x32(1*32 + 16, 3, OLEDWeather.Tomorrow, ENABLE, DISABLE);
	OLED_P32x32(2*32 + 24, 3, OLEDWeather.Acquired, ENABLE, DISABLE);	// ��Ӧ������ͼ��
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
�������� �� First_Menu
��    �� �� ��һ�˵�����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void First_Menu(void)
{
	/* �¶� */
	OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, ENABLE, DISABLE);
	OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, ENABLE, DISABLE);	
	OLED_P32x32(2*16 + 5, 3, 7, ENABLE, DISABLE);
	OLED_P8x8(2*16 + 5 + 2, 2, 5, ENABLE, DISABLE);

	/* ʪ�� */
	OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, ENABLE, DISABLE);
	OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, ENABLE, DISABLE);	
	OLED_P32x32(6*16, 3, 6, ENABLE, DISABLE);
	OLED_String_Move(6*16 + 2, 2,"%RH", OLED_FONT_EIGHT, ENABLE, DISABLE);	
	
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
�������� �� Second_Menu
��    �� �� �ڶ��˵�����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void Second_Menu(void)
{
	/* ���� */
	OLED_Period_Show(0*30 + 9, 2, OLEDDate.Week + 0, OLED_FONT_EIGHT, DISABLE);	// ����
	OLED_Period_Show(1*30 + 19, 2, OLEDDate.Week + 1, OLED_FONT_EIGHT, DISABLE);// ����
	OLED_Period_Show(2*30 + 29, 2, OLEDDate.Week + 2, OLED_FONT_EIGHT, DISABLE);// ����

	/* ����ͼ�� */
	OLED_P32x32(0*32 + 8, 3, OLEDWeather.Today, ENABLE, DISABLE);
	OLED_P32x32(1*32 + 16, 3, OLEDWeather.Tomorrow, ENABLE, DISABLE);
	OLED_P32x32(2*32 + 24, 3, OLEDWeather.Acquired, ENABLE, DISABLE);	// ��Ӧ������ͼ��
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
�������� �� Third_Menu
��    �� �� �����˵�����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void Third_Menu(void)
{
	/* ��� */
	OLED_Period_Show(0*6 + 18, 2, OLEDDate.Week, OLED_FONT_EIGHT, DISABLE);		// ����
	
	OLED_Num_Move(5*6 + 18, 2, OLEDDate.Day, 2, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);
	OLED_String_Move(7*6 + 18, 2,"/", OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_Num_Move(8*6 + 18, 2, OLEDDate.Month, 2, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);
	OLED_String_Move(10*6 + 18, 2,"/", OLED_FONT_EIGHT, ENABLE, DISABLE);
	OLED_Num_Move(11*6 + 18, 2, OLEDDate.Year, 4, OLED_FONT_EIGHT, ENABLE, ENABLE, DISABLE);	
	
	/* ʱ�� */
	OLED_P16x32(0*16 + 7, 3, OLEDtime.Hour / 10, ENABLE, DISABLE);
	OLED_P16x32(1*16 + 7, 3, OLEDtime.Hour % 10, ENABLE, DISABLE);	
	OLED_P16x32(2*16 + 7, 3, 10, ENABLE, DISABLE);		// " : "ʱ������
	OLED_P16x32(2*16 + 7 + 9, 3, OLEDtime.Minute / 10, ENABLE, DISABLE);
	OLED_P16x32(3*16 + 7 + 9, 3, OLEDtime.Minute % 10, ENABLE, DISABLE);	
	OLED_P16x32(4*16 + 7 + 9, 3, 10, ENABLE, DISABLE);	// " : "ʱ������
	OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, ENABLE, DISABLE);
	OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, ENABLE, DISABLE);
	
	OLED_Refresh(OLED_BUFFER_ROW);
}

/************************************************
�������� �� Menu_Interface_APP
��    �� �� ��ʾ�˵�����
��    �� �� ��
�� �� ֵ �� ��
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
			/* ����ת�� */
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
			case 1:						// ʵʱ��ʪ
				First_Menu();
				OLED_P8x8(50, 7, 4, DISABLE, DISABLE);
				OLED_P8x8(60, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(70, 7, 3, DISABLE, DISABLE);

				Menu_grade++;
				break;

			case 2:						// �������
				Second_Menu();
				OLED_P8x8(50, 7, 3, DISABLE, DISABLE);
				OLED_P8x8(60, 7, 4, DISABLE, DISABLE);
				OLED_P8x8(70, 7, 3, DISABLE, DISABLE);
			
				Menu_grade++;
				break;

			case 3:						// ��ǰʱ��
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
		/* ������ */
		OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, DISABLE, DISABLE);
		OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, DISABLE, DISABLE);

		OLED_P16x32(3*16 + 7 + 9, 3, OLEDtime.Minute % 10, ENABLE, DISABLE);			
		OLED_P16x32(4*16 + 7 + 18, 3, OLEDtime.Seconds / 10, ENABLE, DISABLE);
		OLED_P16x32(5*16 + 7 + 18, 3, OLEDtime.Seconds % 10, ENABLE, DISABLE);	// �����ƶ�ʱ����ʾ
	}
	else if(2 == Menu_grade)
	{
		/* �¶� */
		OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, DISABLE, DISABLE);
		OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, DISABLE, DISABLE);
		
		OLED_P16x32(0*16 + 5, 3, DHT11_Data.temp_int / 10, ENABLE, DISABLE);
		OLED_P16x32(1*16 + 5, 3, DHT11_Data.temp_int % 10, ENABLE, DISABLE);	


		/* ʪ�� */
		OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, DISABLE, DISABLE);
		OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, DISABLE, DISABLE);
		
		OLED_P16x32(4*16, 3, DHT11_Data.humi_int / 10, ENABLE, DISABLE);
		OLED_P16x32(5*16, 3, DHT11_Data.humi_int % 10, ENABLE, DISABLE);
	}
}

/************************************************
�������� �� Hygrothermograph_Updata
��    �� �� ��ʪ����
��    �� �� ��
�� �� ֵ �� ��
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
			DUBUG_PRINTF("Humidity = %bd.%bd ��RH\r\nTemperature = %bd.%bd �� \r\n",\
			DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
*/
		}
	}
}

/************************************************
�������� �� Weather_Updata
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
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

			if(OLEDtime.Hour >= 5 && OLEDtime.Hour <= 17)		// ����
			{
				OLEDWeather.Today = weather_data->weather_code[0];
//				OLEDWeather.Tomorrow = weather_data->weather_code[2];
//				OLEDWeather.Acquired = weather_data->weather_code[4];
			}
			else if((OLEDtime.Hour > 17 && OLEDtime.Hour < 24)
						|| (OLEDtime.Hour >= 0 && OLEDtime.Hour < 5))	// ���
			{
				OLEDWeather.Today = weather_data->weather_code[1];
//				OLEDWeather.Tomorrow = weather_data->weather_code[3];
//				OLEDWeather.Acquired = weather_data->weather_code[5];
			}
			OLEDWeather.Tomorrow = weather_data->weather_code[2];
			OLEDWeather.Acquired = weather_data->weather_code[4];

			/* ¼�� MUC��ʱʱ�� */
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
�������� �� Car_Control
��    �� �� С�����ݿ��ƴ���
��    �� �� ��
�� �� ֵ �� ��
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
			g_Gather_Time = 4;			// Լ 200ms�ؼ췢��һ��
		}
	}
}


/*---------------------------- END OF FILE ----------------------------*/


