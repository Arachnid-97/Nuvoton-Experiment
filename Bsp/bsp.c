#include "bsp.h"
#include "bsp_time.h"


static bit BIT_TMP;

/************************************************
�������� �� Clk_Config
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void Clk_Config(void)
{
	set_HIRCST;          		// ��16 MHz����ʱ��
	CKDIV = 0x00;				// ����Ƶ��fsys = 16MHz��
	set_HIRCEN;          		// �ڲ���������ʹ��
}

/************************************************
�������� �� GPIO_Init
��    �� �� GPIO��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void GPIO_Init(void)
{
	Set_All_GPIO_Quasi_Mode;	// ��������Pin�ܽ�Ϊ׼˫��ģʽ
	P12_PushPull_Mode;			// LED
	P05_Quasi_Mode;				// Key
}

/************************************************
�������� �� Bsp_Init
��    �� �� �ײ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void Bsp_Init(void)
{
	Clk_Config();
	GPIO_Init();	
//	set_CLOEN;					// Please measure P1.1 (CLO) pin for system clock output, it should as system clock;
}


/*---------------------------- END OF FILE ----------------------------*/


