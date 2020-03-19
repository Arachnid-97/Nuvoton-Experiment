#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define PAGE_ERASE_AP					0x22					// APROM Ò³²Á³ýÃüÁî
#define BYTE_PROGRAM_AP					0x21					// APROM ×Ö½Ú±à³ÌÃüÁî
#define BYTE_READ_AP 					0x00					// APROM ×Ö½Ú¶ÁÃüÁî

static bit s_EA_Save_bit;

void IAP_Close(void);
void EEPROM_ReadNByte( uint8_t *pBuffer, uint16_t Address, uint16_t nByte );
void EEPROM_WriteNByte( uint8_t *pBuffer, uint16_t Address, uint16_t nByte );
uint8_t EEPROM_ReadByte( uint16_t address );
void EEPROM_WriteByte( uint16_t address, uint8_t wData );
void EEPROM_ErasePage( uint16_t address );

#endif /* __BSP_EEPROM_H */


/*---------------------------- END OF FILE ----------------------------*/


