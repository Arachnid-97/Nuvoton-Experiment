#ifndef __OLED12864_DRV_H
#define __OLED12864_DRV_H


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#ifndef ENABLE
#define ENABLE              1
#endif /* ENABLE */

#ifndef DISABLE
#define DISABLE             0
#endif /* DISABLE */

#ifndef HIGH
#define HIGH                1
#endif /* HIGH */

#ifndef LOW
#define LOW                 0
#endif /* LOW */

/* �����С */
#define OLED_FONT_EIGHT     8
#define OLED_FONT_SIXTEEN   16

#define OLED_GRAM_MAX       30
#define OLED_MAX_COLUMN     128
#define OLED_MAX_ROW        64

#define X_WIDTH             128
#define Y_WIDTH             64

#define OLED_LEFT_ROLL      0x27
#define OLED_RIGHT_ROLL     0x26

#define OLED_CLS            0x00
#define OLED_SHOW           0xFF

//#define OLED_SCL_PORT       GPIOC
//#define OLED_SCL_PINS       GPIO_Pin_1
//#define OLED_SDA_PORT       GPIOC
//#define OLED_SDA_PINS       GPIO_Pin_0
//#define OLED_DC_PORT        GPIOB
//#define OLED_DC_PINS        GPIO_Pin_6
//#define OLED_CS_PORT        GPIOB
//#define OLED_CS_PINS        GPIO_Pin_5
//#define OLED_SCK_PORT       GPIOC
//#define OLED_SCK_PINS       GPIO_Pin_1
//#define OLED_RST_PORT       GPIOB
//#define OLED_RST_PINS       GPIO_Pin_7
//#define OLED_SDO_PORT       GPIOC
//#define OLED_SDO_PINS       GPIO_Pin_0

#define OLED_SDA_READ       P14

#define OLED_SCL(x)         P13 = x   // D0
#define OLED_SDA(x)         P14 = x   // D1/D2

#define OLED_DC(x)          ;     // D/C#

//#define OLED_CS(x)          GPIO_WriteBit(OLED_CS_PORT, OLED_CS_PINS, (BitAction)x)     // CS#
//#define OLED_SCK(x)         GPIO_WriteBit(OLED_SCK_PORT, OLED_SCK_PINS, (BitAction)x)   // D0
//#define OLED_RST(x)         GPIO_WriteBit(OLED_RST_PORT, OLED_RST_PINS, (BitAction)x)   // RES#
//#define OLED_SDO(x)         GPIO_WriteBit(OLED_SDO_PORT, OLED_SDO_PINS, (BitAction)x)   // D1

void OLED_Write_Cmd( uint8_t Cmd );
void OLED_Write_Data( uint8_t Data, uint8_t Inverse );
void OLED_Fill( uint8_t Mode );
void OLED_Row_Clear( uint8_t Row, uint8_t Amount ,uint8_t Mode );
void OLED_Coord( uint8_t X, uint8_t Y );
void OLED_ShowRoll( uint8_t Y, uint8_t Line, uint8_t Mode );
void OLED_ShowChar( uint8_t X, uint8_t Y, uint8_t Char, uint8_t Size, uint8_t Inverse );
void OLED_ShowString( uint8_t X, uint8_t Y, const uint8_t *pChar, uint16_t Len, uint8_t Size, uint8_t Inverse );
void OLED_ShowPrintf( uint8_t X, uint8_t Y, const uint8_t *pChar, uint8_t Size, bit Align, uint8_t Inverse);
uint32_t OLED_Power( uint8_t M, uint8_t N );
void OLED_ShowNum( uint8_t X, uint8_t Y, uint32_t Num, uint8_t Len, uint8_t Size, uint8_t Prefix, uint8_t Inverse );
void OLED_ShowHex( uint8_t X, uint8_t Y, uint32_t Num, uint8_t Size, uint8_t Prefix, uint8_t Inverse );

#if 1
void OLED_ShowLanguage( uint8_t X, uint8_t Y, const uint8_t *pChar, uint16_t Len, uint8_t Inverse );

#else
void OLED_ShowLanguage( uint8_t X, uint8_t Y, const uint8_t (*pArray)[16], uint16_t Len, uint8_t Inverse );

#endif
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Config(void);
void OLED_Init(void);

void OLED_ShowChinese( uint8_t X, uint8_t Y, const uint8_t *pArray, uint8_t Inverse );
void OLED_Draw_Font( uint8_t X, uint8_t Y, const uint8_t *pArray, uint8_t Inverse );


#endif /* __OLED12864_DRV_H */


/*---------------------------- END OF FILE ----------------------------*/


