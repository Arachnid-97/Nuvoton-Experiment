#include "bsp_uart.h"
#include "bsp_time.h"


#define BAUDRATE	9600						// 波特率设置     支持的波特率：115200,19200,9600,38400,57600,1200,2400,4800

bit g_Receiving_flag = 0;						// 串口接收标志
bit g_Frame_flag = 0;							// 一帧完成标志
uint8_t g_ReceiveBuf[MAX_L] = {0};				// 串口接收暂存缓冲区
uint16_t g_RxCnt = 0;							// 串口接收数据个数
uint16_t g_Uart_time = 0;						// 串口计时

/************************************************
函数名称 ： UART0_Timer1_Init
功    能 ： UART0初始化（用定时器 1溢出作为波特率时钟源）
参    数 ： 无
返 回 值 ： 无
*************************************************/
void UART0_Timer1_Init(void)
{
	P06_Quasi_Mode;		// Setting UART pin as Quasi mode for transmit
	P07_Quasi_Mode;		// Setting UART pin as Quasi mode for transmit
	
    SCON = 0x50;     	// UART0 Mode1,REN=1
    TMOD |= 0x20;    	// Timer1 Mode1	16位定时
    
    set_SMOD;        	// UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        	// Serial port 0 baud rate clock source = Timer1
	clr_TR1;
	
//    TH1 = 256 - (500000 / BAUDRATE + 1);               /* 8MHz */
    TH1 = 256 - (1000000 / BAUDRATE + 1);               /* 16MHz */
	set_TR1;

  	IPH |= 0x10;
	IP |= 0x10;			//中断优先级设置为 3
	set_ES;				//开启串口 0中断
	
	/* 注意，此操作会影响正常的发送,不需要 printf调试时注释掉 */
//	set_TI;				// For printf function must setting TI = 1
}

/************************************************
函数名称 ： UART0_SendByte
功    能 ： 串口0字符发送
参    数 ： c ---- 发送的数据
返 回 值 ： 无
*************************************************/
void UART0_SendByte(uint8_t c)
{     
	SBUF = c;
    while(!TI);
	clr_TI;
}

/************************************************
函数名称 ： UART0_SendString
功    能 ： 串口0字符串发送
参    数 ： pData ---- 字符串
			Length ---- 长度
返 回 值 ： 无
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
函数名称 ： UART0_Printf
功    能 ： 串口0打印输出
参    数 ： String	---- 字符串
返 回 值 ： 无
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
函数名称 ： UART0_SendString
功    能 ： 串口 0字符接收
参    数 ： 无
返 回 值 ： Receive ---- 接收的数据
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
函数名称 ： UART0_Isr
功    能 ： 串口0中断
参    数 ： 无
返 回 值 ： 无
*************************************************/
void UART0_ISR(void) interrupt 4
{
	uint8_t temp = 0;
	
	if(RI)
	{
		temp = UART0_Receive();
		
		g_Receiving_flag = 1;
		
		if(!g_Frame_flag)					// 判断帧处理完成与否
		{
			g_ReceiveBuf[g_RxCnt] = temp;	
			g_RxCnt++;
			if(g_RxCnt >= MAX_L)
			{
				g_Receiving_flag = 0;
				g_RxCnt = 0;				//为了防止数组溢出
			}
			g_Uart_time = 2;				//40ms(接收超时判定)
		}				
	}
//	if(TI);
//		clr_TI;
}


/*---------------------------- END OF FILE ----------------------------*/


