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
    uint8_t Hour;		// 时
    uint8_t Minute;		// 分
    uint8_t Seconds;	// 秒
}OledTime_TypeDef;
extern OledTime_TypeDef OLEDtime;

typedef struct
{
	uint8_t Week;		// 星期
    uint8_t Day;		// 日
    uint8_t Month;		// 月
    uint16_t Year;		// 年
}OledDate_TypeDef;
//extern OledDate_TypeDef OLEDDate;

typedef struct
{
    uint8_t Today;		// 今天
    uint8_t Tomorrow;	// 明天
    uint8_t Acquired;	// 后天
}OledWeather_TypeDef;
//extern OledWeather_TypeDef OLEDWeather;

//#pragma pack (1) /* 指定按 1字节对齐 */
typedef struct
{
	uint8_t region_name[12];
	uint8_t weather_code[6];
	uint16_t current_date[3];
	uint8_t week;
	uint8_t current_time[3];
}weather_content;
//#pragma pack () /* 取消指定对齐，恢复缺省对齐 */

extern bit g_Send_flag;

void Main_Interface_APP(void);
void Menu_Interface_APP(void);
void Hygrothermograph_Updata(void);
void Weather_Updata(void);
void Car_Control(void);


#endif	/* __TASK_H */


/*---------------------------- END OF FILE ----------------------------*/

