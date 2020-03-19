#include "bsp_eeprom.h"
#include "bsp_time.h"


//static bit s_EA_Save_bit;														
static bit BIT_TMP;			//EA暂存（对应官方库）

/************************************************
函数名称 ： IAP_Close
功    能 ： 关闭IAP
参    数 ： 无
返 回 值 ： 无
*************************************************/
void IAP_Close(void)
{
	clr_IAPEN;							//关闭IAP
	clr_APUEN;							//关闭APROM更新
	IAPCN = 0x30;						//复位命令
	clr_IAPGO;							//清除触发
	clr_IAPFF;							//清除错误标志
}

/************************************************
函数名称 ： EEPROM_ReadNByte
功    能 ： 从EEPROM读 N个字节
参    数 ： pBuffer ---- 读出的数据
            address ---- 地址位
						nByte ---- 字节长度
返 回 值 ： 无
*************************************************/
void EEPROM_ReadNByte( uint8_t *pBuffer, uint16_t Address, uint16_t nByte )
{
    while(nByte--)
    {
        *pBuffer++ = EEPROM_ReadByte(Address++);
    }
}

/************************************************
函数名称 ： EEPROM_WriteNByte
功    能 ： 从EEPROM写N个字节
参    数 ： pBuffer ---- 写入的数据
            address ---- 地址位
			nByte ---- 字节长度
返 回 值 ： 无
*************************************************/
void EEPROM_WriteNByte( uint8_t *pBuffer, uint16_t Address, uint16_t nByte )
{
//    uint8_t p;

//    p = &pBuffer;
	
	while(nByte--)
    {
        EEPROM_WriteByte(Address, *pBuffer++);
    }
}


/************************************************
函数名称 ： EEPROM_Read
功    能 ： 从EEPROM读一字节
参    数 ： Address ---- 地址位
返 回 值 ： rData ---- 读取的数据
*************************************************/
uint8_t EEPROM_ReadByte(uint16_t Address)
{
	uint8_t rData;
	
	set_IAPEN;							//使能IAP
	set_APUEN;							//APROM更新使能
	IAPCN = BYTE_READ_AP;				//设置APROM字节读模式
	IAPAL = Address;
	IAPAH = Address >> 8;				//写入地址
	set_IAPGO;							//IAP触发
	SoftwareDelay_ms(10);
	rData = IAPFD;								
	IAP_Close();
	
	return	rData;
}

/************************************************
函数名称 ： EEPROM_Read
功    能 ： 从EEPROM写一字节
参    数 ： Address ---- 地址位
			wData ---- 存储的数据
返 回 值 ： 无
*************************************************/
void EEPROM_WriteByte(uint16_t Address, uint8_t wData)
{
	set_IAPEN;							//使能IAP
	set_APUEN;							//APROM更新使能
	IAPCN = BYTE_PROGRAM_AP;			//设置APROM字节编程模式
	IAPAL = Address;
	IAPAH = Address >> 8;				//写入地址		
	IAPFD = wData;						//写EEPROM数据
	set_IAPGO;							//IAP触发
	SoftwareDelay_ms(10);
	IAP_Close();		
}

/************************************************
函数名称 ： EEPROM_ErasePage
功    能 ： 从address开始进行页擦除（每一页是 128 字节，所以地址应该是目标页的地址）
参    数 ： address ---- 地址位
返 回 值 ： 无
*************************************************/
void EEPROM_ErasePage(uint16_t address)
{
	set_IAPEN;							//使能IAP
	set_APUEN;							//APROM更新使能
	IAPCN = PAGE_ERASE_AP;				//设置APROM页擦除模式
	IAPAL = address;
	IAPAH = address >> 8;				//写入地址
	IAPFD = 0xFF;
	set_IAPGO;							//IAP触发
	SoftwareDelay_ms(10);
	IAP_Close();
}



/*---------------------------- END OF FILE ----------------------------*/


