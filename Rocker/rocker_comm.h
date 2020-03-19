#ifndef __ROCKER_COMM_H
#define __ROCKER_COMM_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define SW_PIN			P15

#define MEAS_VOL        1600UL         // ºÏ≤‚µÁ—π

#define UP				0xF0
#define DOWN			0x0F
#define LEFT			0xAA
#define RIGHT			0x55
#define LEFT_UP			0xFA
#define LEFT_DOWN		0xAF
#define RIGHT_UP		0xF5
#define RIGHT_DONW		0x5F
#define CENTER			0x00

extern uint8_t g_Gather_Time;
extern uint8_t g_Car_Mode;
extern uint8_t Protocol[7];

bit Voltage_Check(void);
uint8_t Data_Verify( uint8_t *Buf );
void Rocker_KeyConfig(void);


#endif /* __ROCKER_COMM_H */


/*---------------------------- END OF FILE ----------------------------*/


