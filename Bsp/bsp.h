#ifndef __BSP_H
#define __BSP_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define LED_ON		clr_P12
#define LED_OFF		set_P12

void Clk_Config(void);
void GPIO_Init(void);
void Bsp_Init(void);


#endif	/* __BSP_H */


/*---------------------------- END OF FILE ----------------------------*/

