#include "bsp.h"
#include "bsp_time.h"


static bit BIT_TMP;

/************************************************
函数名称 ： Clk_Config
功    能 ： 时钟配置
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Clk_Config(void)
{
	set_HIRCST;          		// 打开16 MHz高速时钟
	CKDIV = 0x00;				// 不分频（fsys = 16MHz）
	set_HIRCEN;          		// 内部高速振荡器使能
}

/************************************************
函数名称 ： GPIO_Init
功    能 ： GPIO初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void GPIO_Init(void)
{
	Set_All_GPIO_Quasi_Mode;	// 设置所有Pin管脚为准双向模式
	P12_PushPull_Mode;			// LED
	P05_Quasi_Mode;				// Key
}

/************************************************
函数名称 ： Bsp_Init
功    能 ： 底层初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Bsp_Init(void)
{
	Clk_Config();
	GPIO_Init();	
//	set_CLOEN;					// Please measure P1.1 (CLO) pin for system clock output, it should as system clock;
}


/*---------------------------- END OF FILE ----------------------------*/


