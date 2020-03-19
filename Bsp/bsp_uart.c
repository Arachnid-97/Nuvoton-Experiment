#include "bsp_uart.h"
#include "bsp_time.h"


#define BAUDRATE	9600						// ����������     ֧�ֵĲ����ʣ�115200,19200,9600,38400,57600,1200,2400,4800

bit g_Receiving_flag = 0;						// ���ڽ��ձ�־
bit g_Frame_flag = 0;							// һ֡��ɱ�־
uint8_t g_ReceiveBuf[MAX_L] = {0};				// ���ڽ����ݴ滺����
uint16_t g_RxCnt = 0;							// ���ڽ������ݸ���
uint16_t g_Uart_time = 0;						// ���ڼ�ʱ

/************************************************
�������� �� UART0_Timer1_Init
��    �� �� UART0��ʼ�����ö�ʱ�� 1�����Ϊ������ʱ��Դ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void UART0_Timer1_Init(void)
{
	P06_Quasi_Mode;		// Setting UART pin as Quasi mode for transmit
	P07_Quasi_Mode;		// Setting UART pin as Quasi mode for transmit
	
    SCON = 0x50;     	// UART0 Mode1,REN=1
    TMOD |= 0x20;    	// Timer1 Mode1	16λ��ʱ
    
    set_SMOD;        	// UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        	// Serial port 0 baud rate clock source = Timer1
	clr_TR1;
	
//    TH1 = 256 - (500000 / BAUDRATE + 1);               /* 8MHz */
    TH1 = 256 - (1000000 / BAUDRATE + 1);               /* 16MHz */
	set_TR1;

  	IPH |= 0x10;
	IP |= 0x10;			//�ж����ȼ�����Ϊ 3
	set_ES;				//�������� 0�ж�
	
	/* ע�⣬�˲�����Ӱ�������ķ���,����Ҫ printf����ʱע�͵� */
//	set_TI;				// For printf function must setting TI = 1
}

/************************************************
�������� �� UART0_SendByte
��    �� �� ����0�ַ�����
��    �� �� c ---- ���͵�����
�� �� ֵ �� ��
*************************************************/
void UART0_SendByte(uint8_t c)
{     
	SBUF = c;
    while(!TI);
	clr_TI;
}

/************************************************
�������� �� UART0_SendString
��    �� �� ����0�ַ�������
��    �� �� pData ---- �ַ���
			Length ---- ����
�� �� ֵ �� ��
*************************************************/
void UART0_SendString(uint8_t *pData, uint16_t Length)
{
	while(Length--)
	{
		UART0_SendByte(*pData);
		pData++;
	}
}

/************************************************
�������� �� UART0_Printf
��    �� �� ����0��ӡ���
��    �� �� String	---- �ַ���
�� �� ֵ �� ��
*************************************************/
void UART0_Printf(uint8_t *String)
{
	while((*String) != '\0')
	{
		UART0_SendByte(*String);
		String++;
	}
}

/************************************************
�������� �� UART0_SendString
��    �� �� ���� 0�ַ�����
��    �� �� ��
�� �� ֵ �� Receive ---- ���յ�����
*************************************************/
uint8_t UART0_Receive(void)
{
    uint8_t Receive = 0;
	
    while(!RI);
    Receive = SBUF;
    clr_RI;                          		/* clear reception flag for next reception */
    return Receive;
}

/************************************************
�������� �� UART0_Isr
��    �� �� ����0�ж�
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void UART0_ISR(void) interrupt 4
{
	uint8_t temp = 0;
	
	if(RI)
	{
		temp = UART0_Receive();
		
		g_Receiving_flag = 1;
		
		if(!g_Frame_flag)					// �ж�֡����������
		{
			g_ReceiveBuf[g_RxCnt] = temp;	
			g_RxCnt++;
			if(g_RxCnt >= MAX_L)
			{
				g_Receiving_flag = 0;
				g_RxCnt = 0;				//Ϊ�˷�ֹ�������
			}
			g_Uart_time = 2;				//40ms(���ճ�ʱ�ж�)
		}				
	}
//	if(TI);
//		clr_TI;
}


/*---------------------------- END OF FILE ----------------------------*/


