#ifndef __BSP_ADC_H
#define __BSP_ADC_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define	READ_UID		0x04		  // UID������

#define REF_VOLTAGE     1220UL        // ��׼��ѹǧ��
#define POWER_VOLTAGE   4950UL        // ��ѹǧ��
#define ADC_DIP(X)      (X)           // ADC�ֱ���

uint32_t VDD_Read(void);
uint32_t ADC_Read( const uint32_t Ref );
void ADC_Config(void);


#endif /* __BSP_ADC_H */


/*---------------------------- END OF FILE ----------------------------*/


