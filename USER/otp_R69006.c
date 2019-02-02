#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP����
uint16_t vcom_best = 0x0125;		// ���VCOM		
uint8_t MAIN_PORT = PORT0;	  	// ��MIPI port(��Ϊ�ض������ݿ�)	 WQXGA:PORT0 	HF/FHD:PORT1
#define D_Value 		  0x0115	// VCOM1 - VCOM2 for R69006 TES solution Sivan add 20150417

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
	
//	wtBuf[0] = 0x00;
//	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
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
	
//	wtBuf[0] = 0xC0;
//	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}

/*********************************************************************************
 * ��������Vcom_Set
 * ����  ������VCOM
 * ����  ��-VCOMֵ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
//void VCOM_Set(unsigned short vcom)
//{
//	unsigned char wtBuf[7];

//	wtBuf[0] = 0x06;
//	wtBuf[1] = 0x00;
//	wtBuf[2] = 0x00;
//	wtBuf[3] = vcom>>8;
//	wtBuf[4] = vcom;
//	wtBuf[5] = vcom>>8;
//	wtBuf[6] = vcom;
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD5, 7, wtBuf);
//}

//For R69006 - 20150407
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[11];

	wtBuf[0] = 0x66;
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = vcom>>8;
//	wtBuf[3] = 0x01;
	wtBuf[4] = vcom;
	wtBuf[5] = vcom>>8;
//	wtBuf[3] = 0x01;
	wtBuf[6] = vcom;
//	wtBuf[7] = 0x00;
//	wtBuf[8] = 0x15;
//	wtBuf[9] = 0x00;
//	wtBuf[10] = 0x15;
	wtBuf[7] = (vcom-D_Value)>>8;
	wtBuf[8] = (vcom-D_Value);
	wtBuf[9] = (vcom-D_Value)>>8;
	wtBuf[10] = (vcom-D_Value);

	MIPI_GEN_Write((PORT0 | PORT1), 0xD5, 11, wtBuf);
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
	unsigned char wtBuf[6];

	wtBuf[0] = 0x01;
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = u;			  
	wtBuf[4] = v;
	wtBuf[5] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE2, 6, wtBuf);
}

/*********************************************************************************
 * ��������Date_Set
 * ����  ������Date
 * ����  ��-���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void Date_Set(unsigned char year, unsigned char month, unsigned char day)
{
	unsigned char wtBuf[10];

	wtBuf[0] = 0x01;
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = 0x20 | (day & 0x1F); 
	wtBuf[4] = 0x59;
	wtBuf[5] = 0x00;
	wtBuf[6] = (year << 4) + (month & 0x0F);			  
	wtBuf[7] = 0x07;
	wtBuf[8] = 0x14;
	wtBuf[9] = 0xFF;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 9, wtBuf);
	
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
	unsigned char rdBuf[11];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 11, rdBuf);

	return (((rdBuf[5] & 0x00FF) << 8) | rdBuf[6]);
}
/*********************************************************************************
 * // 20150416 Sivan add for R69006 
 * ��������VCOM2_Read  
 * ����  ���ض�VCOM
 * ����  ���� 
 * ���  ����
 * ����  ��VCOM2ֵ
 * ����  ���ⲿ����
 */
unsigned short VCOM2_Read(void)
{
	unsigned char rdBuf[11];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 11, rdBuf);

	return (((rdBuf[7] & 0x00FF) << 8) | rdBuf[8]);
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
	unsigned char rdBuf[10];

	MIPI_GEN_Read(MAIN_PORT, 0xE1, 10, rdBuf);

	return rdBuf[8];
}

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
 	uint8_t buf[10];

	MIPI_GEN_Read(MAIN_PORT, 0xE1, 10, buf);
	buf[0] = 0x03;	// enable access OxE1
	buf[8]++;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 10, buf);

//	LCD_SleepIn();
//	LCD_VideoMode_OFF();
//	LCD_LPMode();

	LCD_SleepIn();
//	buf[0] = 0x01;
	buf[0] = 0x81; //Sivan for R69006 - 20150407
	MIPI_GEN_Write((PORT0 | PORT1), 0xD6, 1, buf);
	buf[0] = 0xFF; 
	MIPI_GEN_Write((PORT0 | PORT1), 0xE3, 1, buf);
	buf[0] = 0x11;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x40;
	buf[4] = 0x00;
//	buf[5] = 0x00; //Sivan for R69006 - 20150407
	MIPI_GEN_Write((PORT0 | PORT1), 0xE0, 5, buf);
	MIPI_GEN_Read(MAIN_PORT, 0xE0, 5, buf);

	Delay_sec(2);	// wait more than 1.7s[1.4s for R69006], write & verify oparation

	MIPI_GEN_Read(MAIN_PORT, 0xE0, 5, buf);
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
//	buf[5] = 0x00; //Sivan for R69006 - 20150407
	MIPI_GEN_Write((PORT0 | PORT1), 0xE0, 5, buf);

	LCD_SoftReset();
	LCD_PWM(0x80);
	LCD_SleepOut();
	LCD_VideoMode_ON();
	LCD_HSMode();

	return 	otpResult;
}
