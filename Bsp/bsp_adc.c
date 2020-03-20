#include "bsp_adc.h"
#include "bsp_time.h"


#define _BENCHMARK_VOLTAGE      // ����׼��ѹУ���� define

static bit BIT_TMP;
static double s_Bandgap_voleage = 0;

/************************************************
�������� �� Read_Bandgap
��    �� �� ��϶��ѹ��ȡ(�ڲ���׼��ѹ)
��    �� �� ��
�� �� ֵ �� bandgap_value ---- �ڲ���϶��ѹ����
*************************************************/
#ifdef _BENCHMARK_VOLTAGE
static double Read_Bandgap(void)
{
	uint8_t bandgapHigh,bandgapLow;
    double bandgap_value = 0;

	set_IAPEN;							// ʹ��IAP
	IAPCN = READ_UID;					// ���� UID��ģʽ
	IAPAL = 0x0C;
	IAPAH = 0x00;						// д���ַ
	set_IAPGO;							// IAP����
	SoftwareDelay_ms(2);
	bandgapHigh = IAPFD;
	
	IAPCN = READ_UID;					// ���� UID��ģʽ
	IAPAL = 0x0D;
	IAPAH = 0x00;						// д���ַ
	set_IAPGO;							// IAP����
	SoftwareDelay_ms(2);
	bandgapLow = IAPFD;
	bandgapLow &= 0x0F;

	clr_IAPEN;

	bandgap_value = (bandgapHigh << 4) + bandgapLow;
	bandgap_value = 3072 / (0x1000 / bandgap_value);	// �����ǧ���ĵ�ѹֵ

    return bandgap_value;
}

#endif /* _BENCHMARK_VOLTAGE */

/************************************************
�������� �� VDD_Read
��    �� �� VDD���ݶ�ȡ(�����ڲ���׼��ѹ)
��    �� �� ��
�� �� ֵ �� VDD_value ---- �ڲ���Դ���ݣ��Ŵ�1000����
*************************************************/
#ifdef _BENCHMARK_VOLTAGE
double VDD_Read(void)
{
	uint8_t i;
    double VDD_value = 0;

	Enable_ADC_BandGap;			// �� BandGap����
	for(i = 0;i < 4;i++)
	{
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0);
	}
	VDD_value = ADCRL;
	VDD_value += (ADCRH << 4);
	VDD_value = 0x1000 / VDD_value * s_Bandgap_voleage;		// �����ǧ���ĵ�ѹֵ
	ADCCON1 &= ~0x01;			// �ر� ADC
	
    return VDD_value;
}

#endif /* _BENCHMARK_VOLTAGE */

/************************************************
�������� �� ADC_Read
��    �� �� ADC���ݶ�ȡ
��    �� �� Ref ---- �ο�ֵ
�� �� ֵ �� ADC_value ---- ���ݣ��Ŵ�1000����
*************************************************/
uint32_t ADC_Read( const double Ref )
{
    double ADC_value = 0;

	clr_ADCF;
	set_ADCS;
    while(ADCF == 0);                    // �ȴ�ת������
    ADC_value = ADCRL;
    ADC_value += (ADCRH << 4);           //��ȡת�����

    ADC_value = ADC_value * Ref / 0x1000;  // �����ǧ���ĵ�ѹֵ

    return (uint32_t)ADC_value;
}

/************************************************
�������� �� ADC_Config
��    �� �� ADC����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void ADC_Config(void)
{
	P17_Input_Mode;
	P30_Input_Mode;
	
	s_Bandgap_voleage = Read_Bandgap();
}


/*---------------------------- END OF FILE ----------------------------*/


