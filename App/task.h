#ifndef __TASK_H
#define __TASK_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define REVERSE_BYTES_UINT32(A)		((A & 0x000000FFU) << 24 | (A & 0x0000FF00U) << 8 | \
									(A & 0x00FF0000U) >> 8 | (A & 0xFF000000U) >> 24)

#define REVERSE_BYTES_UINT16(A)		((A & 0x00FFU) << 8 | (A & 0xFF00U) >> 8)


#define SIGNAL_STATUS		12
#define WIFI1_STATUS		11
#define WIFI2_STATUS		10
#define WIFI3_STATUS		9
#define WIFI4_STATUS		8
#define BLUETOOTH_STATUS	5
#define G24_STATUS			13

#define REGION_NAME			"Guangzhou"

typedef struct
{
    uint8_t Hour;		// ʱ
    uint8_t Minute;		// ��
    uint8_t Seconds;	// ��
}OledTime_TypeDef;
extern OledTime_TypeDef OLEDtime;

typedef struct
{
	uint8_t Week;		// ����
    uint8_t Day;		// ��
    uint8_t Month;		// ��
    uint16_t Year;		// ��
}OledDate_TypeDef;
//extern OledDate_TypeDef OLEDDate;

typedef struct
{
    uint8_t Today;		// ����
    uint8_t Tomorrow;	// ����
    uint8_t Acquired;	// ����
}OledWeather_TypeDef;
//extern OledWeather_TypeDef OLEDWeather;

//#pragma pack (1) /* ָ���� 1�ֽڶ��� */
typedef struct
{
	uint8_t region_name[12];
	uint8_t weather_code[6];
	uint16_t current_date[3];
	uint8_t week;
	uint8_t current_time[3];
}weather_content;
//#pragma pack () /* ȡ��ָ�����룬�ָ�ȱʡ���� */

extern bit g_Send_flag;

void Main_Interface_APP(void);
void Menu_Interface_APP(void);
void Hygrothermograph_Updata(void);
void Weather_Updata(void);
void Car_Control(void);


#endif	/* __TASK_H */


/*---------------------------- END OF FILE ----------------------------*/

