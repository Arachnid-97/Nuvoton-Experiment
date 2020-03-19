#ifndef __BSP_TIME_H
#define __BSP_TIME_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define RELOAD_VALUE_H  0xFE
#define RELOAD_VALUE_L  0x20

extern uint8_t g_TCount;
extern uint8_t g_Time_1s;
extern uint8_t g_Time_1min;

void SoftwareDelay_ms(uint32_t Cnt);
void Timer3_Init(void);
void Delay_30us( uint32_t Cnt );
void Timer0_Init(void);


#endif	/* __BSP_TIME_H */


/*---------------------------- END OF FILE ----------------------------*/

