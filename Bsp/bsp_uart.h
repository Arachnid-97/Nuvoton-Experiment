#ifndef __BSP_UART_H
#define __BSP_UART_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define _UART_DEBUG				0
/*
#define DUBUG_PRINTF(fmt,arg...)			do{\
											if(_UART_DEBUG)\
												printf("<<-DEBUG DATA->> "fmt"\n",##arg);\
											}while(0)
*/

#define DUBUG_STRING(fmt,len)				do{\
											if(_UART_DEBUG)\
												UART0_SendString((fmt),len);\
											}while(0)

#define MAX_L				50				//接收缓冲区最大长度

extern uint8_t g_ReceiveBuf[MAX_L];			
extern uint16_t g_RxCnt;
extern bit g_Receiving_flag;							
extern bit g_Frame_flag;								
extern uint16_t g_Uart_time;

void UART0_Timer1_Init(void);
void UART0_SendByte( uint8_t c );
void UART0_SendString( uint8_t *pData, uint16_t Length );
void UART0_Printf( uint8_t *String );
uint8_t UART0_Receive(void);


#endif	/* __BSP_UART_H */


/*---------------------------- END OF FILE ----------------------------*/


