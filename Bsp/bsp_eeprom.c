#include "bsp_eeprom.h"
#include "bsp_time.h"


//static bit s_EA_Save_bit;														
static bit BIT_TMP;			//EA�ݴ棨��Ӧ�ٷ��⣩

/************************************************
�������� �� IAP_Close
��    �� �� �ر�IAP
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void IAP_Close(void)
{
	clr_IAPEN;							//�ر�IAP
	clr_APUEN;							//�ر�APROM����
	IAPCN = 0x30;						//��λ����
	clr_IAPGO;							//�������
	clr_IAPFF;							//��������־
}

/************************************************
�������� �� EEPROM_ReadNByte
��    �� �� ��EEPROM�� N���ֽ�
��    �� �� pBuffer ---- ����������
            address ---- ��ַλ
						nByte ---- �ֽڳ���
�� �� ֵ �� ��
*************************************************/
void EEPROM_ReadNByte( uint8_t *pBuffer, uint16_t Address, uint16_t nByte )
{
    while(nByte--)
    {
        *pBuffer++ = EEPROM_ReadByte(Address++);
    }
}

/************************************************
�������� �� EEPROM_WriteNByte
��    �� �� ��EEPROMдN���ֽ�
��    �� �� pBuffer ---- д�������
            address ---- ��ַλ
			nByte ---- �ֽڳ���
�� �� ֵ �� ��
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
�������� �� EEPROM_Read
��    �� �� ��EEPROM��һ�ֽ�
��    �� �� Address ---- ��ַλ
�� �� ֵ �� rData ---- ��ȡ������
*************************************************/
uint8_t EEPROM_ReadByte(uint16_t Address)
{
	uint8_t rData;
	
	set_IAPEN;							//ʹ��IAP
	set_APUEN;							//APROM����ʹ��
	IAPCN = BYTE_READ_AP;				//����APROM�ֽڶ�ģʽ
	IAPAL = Address;
	IAPAH = Address >> 8;				//д���ַ
	set_IAPGO;							//IAP����
	SoftwareDelay_ms(10);
	rData = IAPFD;								
	IAP_Close();
	
	return	rData;
}

/************************************************
�������� �� EEPROM_Read
��    �� �� ��EEPROMдһ�ֽ�
��    �� �� Address ---- ��ַλ
			wData ---- �洢������
�� �� ֵ �� ��
*************************************************/
void EEPROM_WriteByte(uint16_t Address, uint8_t wData)
{
	set_IAPEN;							//ʹ��IAP
	set_APUEN;							//APROM����ʹ��
	IAPCN = BYTE_PROGRAM_AP;			//����APROM�ֽڱ��ģʽ
	IAPAL = Address;
	IAPAH = Address >> 8;				//д���ַ		
	IAPFD = wData;						//дEEPROM����
	set_IAPGO;							//IAP����
	SoftwareDelay_ms(10);
	IAP_Close();		
}

/************************************************
�������� �� EEPROM_ErasePage
��    �� �� ��address��ʼ����ҳ������ÿһҳ�� 128 �ֽڣ����Ե�ַӦ����Ŀ��ҳ�ĵ�ַ��
��    �� �� address ---- ��ַλ
�� �� ֵ �� ��
*************************************************/
void EEPROM_ErasePage(uint16_t address)
{
	set_IAPEN;							//ʹ��IAP
	set_APUEN;							//APROM����ʹ��
	IAPCN = PAGE_ERASE_AP;				//����APROMҳ����ģʽ
	IAPAL = address;
	IAPAH = address >> 8;				//д���ַ
	IAPFD = 0xFF;
	set_IAPGO;							//IAP����
	SoftwareDelay_ms(10);
	IAP_Close();
}



/*---------------------------- END OF FILE ----------------------------*/


