#ifndef __BSP_BASE_H
#define __BSP_BASE_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

uint32_t Power( uint16_t Base, uint8_t Index );			
uint16_t HexToChar( uint8_t *Dest, uint8_t *Array, uint16_t Length );
uint16_t CharToHex( uint8_t *Dest, uint8_t *Array, uint16_t Length );
void ClrArray( uint8_t *Array, short BufCnt );	
uint8_t CheckDigit( uint8_t *pData, uint16_t Check_L );
uint16_t Data_Extract(uint8_t *Getbuf, uint8_t *Outbuf, uint8_t *Cmpack, uint8_t Caput, uint8_t End, uint16_t Len );


#endif	/* __BSP_BASE_H */


/*---------------------------- END OF FILE ----------------------------*/


