#include "bsp_adc.h"
#include "bsp_time.h"


#define _BENCHMARK_VOLTAGE      // 开基准电压校验则 define

static bit BIT_TMP;
static double s_Bandgap_voleage = 0;

/************************************************
函数名称 ： Read_Bandgap
功    能 ： 带隙电压读取(内部基准电压)
参    数 ： 无
返 回 值 ： bandgap_value ---- 内部带隙电压数据
*************************************************/
#ifdef _BENCHMARK_VOLTAGE
static double Read_Bandgap(void)
{
	uint8_t bandgapHigh,bandgapLow;
    double bandgap_value = 0;

	set_IAPEN;							// 使能IAP
	IAPCN = READ_UID;					// 设置 UID读模式
	IAPAL = 0x0C;
	IAPAH = 0x00;						// 写入地址
	set_IAPGO;							// IAP触发
	SoftwareDelay_ms(2);
	bandgapHigh = IAPFD;
	
	IAPCN = READ_UID;					// 设置 UID读模式
	IAPAL = 0x0D;
	IAPAH = 0x00;						// 写入地址
	set_IAPGO;							// IAP触发
	SoftwareDelay_ms(2);
	bandgapLow = IAPFD;
	bandgapLow &= 0x0F;

	clr_IAPEN;

	bandgap_value = (bandgapHigh << 4) + bandgapLow;
	bandgap_value = 3072 / (0x1000 / bandgap_value);	// 换算成千倍的电压值

    return bandgap_value;
}

#endif /* _BENCHMARK_VOLTAGE */

/************************************************
函数名称 ： VDD_Read
功    能 ： VDD数据读取(基于内部基准电压)
参    数 ： 无
返 回 值 ： VDD_value ---- 内部电源数据（放大1000倍）
*************************************************/
#ifdef _BENCHMARK_VOLTAGE
double VDD_Read(void)
{
	uint8_t i;
    double VDD_value = 0;

	Enable_ADC_BandGap;			// 打开 BandGap测量
	for(i = 0;i < 4;i++)
	{
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0);
	}
	VDD_value = ADCRL;
	VDD_value += (ADCRH << 4);
	VDD_value = 0x1000 / VDD_value * s_Bandgap_voleage;		// 换算成千倍的电压值
	ADCCON1 &= ~0x01;			// 关闭 ADC
	
    return VDD_value;
}

#endif /* _BENCHMARK_VOLTAGE */

/************************************************
函数名称 ： ADC_Read
功    能 ： ADC数据读取
参    数 ： Ref ---- 参考值
返 回 值 ： ADC_value ---- 数据（放大1000倍）
*************************************************/
uint32_t ADC_Read( const double Ref )
{
    double ADC_value = 0;

	clr_ADCF;
	set_ADCS;
    while(ADCF == 0);                    // 等待转换结束
    ADC_value = ADCRL;
    ADC_value += (ADCRH << 4);           //读取转换结果

    ADC_value = ADC_value * Ref / 0x1000;  // 换算成千倍的电压值

    return (uint32_t)ADC_value;
}

/************************************************
函数名称 ： ADC_Config
功    能 ： ADC配置
参    数 ： 无
返 回 值 ： 无
*************************************************/
void ADC_Config(void)
{
	P17_Input_Mode;
	P30_Input_Mode;
	
	s_Bandgap_voleage = Read_Bandgap();
}


/*---------------------------- END OF FILE ----------------------------*/


