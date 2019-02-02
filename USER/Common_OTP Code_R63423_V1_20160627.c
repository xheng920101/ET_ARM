#include "include.h"


uint8_t OTP_TIMES = 0;			// OTP����
uint16_t vcom_best = 0x6D;		// ���VCOM		
uint8_t MAIN_PORT = PORT1;	  	// ��MIPI port(��Ϊ�ض������ݿ�)	 WQXGA:PORT0 	HF/FHD:PORT1

/*********************************************************************************
 * ��������ScanForward
 * ����  ����ɨ
 * ����  ���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */											
void ScanForward(void)
{
	unsigned char wtBuf[3];

	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[2] &= 0xFE;
	MIPI_GEN_Write((PORT0 | PORT1), 0xC1, 3, wtBuf);
}

/*********************************************************************************
 * ��������ScanBackward
 * ����  ����ɨ
 * ����  ���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void ScanBackward(void)
{
	unsigned char wtBuf[3];

   	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[2] |= 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xC1, 3, wtBuf);
}

/*********************************************************************************
 * ��������Vcom_Set
 * ����  ������VCOM
 * ����  ��-VCOMֵ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[7];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 7, wtBuf);
	wtBuf[4] = vcom;
	wtBuf[6] = vcom;
	MIPI_GEN_Write((PORT0 | PORT1), 0xD5, 7, wtBuf);
}

/*********************************************************************************
 * ��������Chroma_Set
 * ����  ������Chroma
 * ����  ��-u,vֵ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void Chroma_Set(unsigned char u, unsigned char v)
{
	MIPI_GEN_Write((PORT0 | PORT1), 0xDA, 1, &u);
	MIPI_GEN_Write((PORT0 | PORT1), 0xDB, 1, &v);
}

/*********************************************************************************
 * ��������VCOM_Read
 * ����  ���ض�VCOM
 * ����  ���� 
 * ���  ����
 * ����  ��VCOMֵ
 * ����  ���ⲿ����
 */
unsigned short VCOM_Read(void)
{
	unsigned char rdBuf[7];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 7, rdBuf);

	return rdBuf[6];
}

/*********************************************************************************
 * ��������OTPTimes_Read
 * ����  ���ض�OTP����
 * ����  ���� 
 * ���  ����
 * ����  ��OTP����
 * ����  ���ⲿ����
 */
unsigned char OTPTimes_Read(void)
{
	unsigned char rdBuf[2];

	MIPI_GEN_Read(MAIN_PORT, 0xE8, 2, rdBuf);

	return rdBuf[1];
}

// /*********************************************************************************
//  * ��������Connect_Check
//  * ����  ���ϵ����
//  * ����  ���� 
//  * ���  ����
//  * ����  ��IC����״̬
//  * ����  ���ⲿ����
//  */
// ErrorStatus Connect_Check(void)
// {
// 	unsigned char rdBuf[1];

// 	MIPI_GEN_Read(MAIN_PORT, 0xDC, 1, rdBuf);
// 	if (rdBuf[0] == 0x00)
// 	{
// 		return 	SUCCESS;
// 	}
// 	return 	ERROR;
// }

/*********************************************************************************
 * ��������OTPSequence
 * ����  ��OTP����
 * ����  ���� 
 * ���  ����
 * ����  ��OTPִ�н��
 * ����  ���ⲿ����
 *	------------------------------------------------------------------------------
 * 	0xE0�Ĵ���
 *		-���ĸ�����bit6��NVMFTTָʾ OTP���� �Ƿ����(0->1: start squence	1->0: squence finish)
 *		-���������bit0��NVMVFFLGERָʾ erase �Ƿ�ɹ�
 *  	-���������bit1��NVMVFFLGWRָʾ write �Ƿ�ɹ�
 *	------------------------------------------------------------------------------
 */
 ErrorStatus OTPSequence(void)
 {
 	ErrorStatus otpResult;
 	uint8_t buf[6];

	LCD_SleepIn();
	LCD_VideoMode_OFF();
	LCD_LPMode();

	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xD6, 1, buf);
	buf[0] = 0xFF; 
	MIPI_GEN_Write((PORT0 | PORT1), 0xE8, 1, buf);
	buf[0] = 0x11;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x40;
	buf[4] = 0x00;
	buf[5] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE6, 6, buf);

	Delay_sec(2);	// wait more than 1.7s, write & verify oparation

	MIPI_GEN_Read(MAIN_PORT, 0xE6, 6, buf);
	if ((buf[3] & 0x40) == 0x40)
	{
		printf("NVM sequence doesn't finish!\r\n");
		printf("OTP NG\r\n");
		otpResult = ERROR;
	}   
	else if ((buf[4] & 0x01) == 0x01)
	{
		if ((buf[4] & 0x02) == 0x02)
		{
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", VCOM_Read());
			printf("OTP OK\r\n");					
			otpResult = SUCCESS;
		}
		else
		{
			printf("Write failed!\r\n");
			printf("OTP NG\r\n");
			otpResult = ERROR;			
		}
	}
	else
	{
		printf("Erase failed!\r\n");
		printf("OTP NG\r\n");
		otpResult = ERROR;
	}							

	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE6, 6, buf);

	LCD_SoftReset();
	LCD_SleepOut();
	LCD_VideoMode_ON();
	LCD_HSMode();

	return 	otpResult;
}
