#include "include.h" 

uint8_t OTP_TIMES = 0;			// OTP����
uint16_t vcom_best = 0x6D;		// ���VCOM		
uint8_t MAIN_PORT = PORT0;	  	// ��MIPI port(��Ϊ�ض������ݿ�)	 WQXGA:PORT0 	HF/FHD:PORT1

// /*********************************************************************************
//  * ��������MIPI_DCS_WRITE
//  * ����  ��ͨ��2828дIC�Ĵ���
//  * ����  ��-reg   IC�Ĵ�����ַ
//  *         -nPara IC�Ĵ�����������
//  *         -para  IC�Ĵ�������ֵ
//  * ���  ����
//  * ����  ����
//  * ����  ���ڲ�����
//  */
//  void MIPI_DCS_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
//  {

// 	SSD_B7 |= SSD_CFGR_DCS;
// 	SSD_B7 &= ~SSD_CFGR_REN;
// 	WriteSSDReg(port, SSD_CFGR, SSD_B7);
// 	SSD_B7 = ReadSSDReg(port, SSD_CFGR);
// 	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);

// 	WriteDriverReg(port, reg, nPara, para);
//  }

// /*********************************************************************************
//  * ��������MIPI_GEN_Write
//  * ����  ��ͨ��2828дIC�Ĵ���
//  * ����  ��-reg   IC�Ĵ�����ַ
//  *         -nPara IC�Ĵ�����������
//  *         -para  IC�Ĵ�������ֵ
//  * ���  ����
//  * ����  ����
//  * ����  ���ڲ�����
//  */
//  void MIPI_GEN_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
//  {
// 	SSD_B7 &= ~SSD_CFGR_DCS;
// 	SSD_B7 &= ~SSD_CFGR_REN;
// 	WriteSSDReg(port, SSD_CFGR, SSD_B7);
// 	SSD_B7 = ReadSSDReg(port, SSD_CFGR);
// 	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);
// 		
// 	WriteDriverReg(port, reg, nPara, para);
//  }

// /*********************************************************************************
//  * ��������MIPI_DCS_Read
//  * ����  ��ͨ��2828��IC�Ĵ���
//  * ����  ��-reg   IC�Ĵ�����ַ
//  *         -nPara IC�Ĵ�����������
//  * ���  ��-para  IC�Ĵ�������ֵ
//  * ����  ����
//  * ����  ���ڲ�����
//  * --------------------------------------------------------------------------------
//  *						MIPI(SSD_RR)				  SPI(SSD_RRA)
//  *		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
//  * --------------------------------------------------------------------------------
//  */
//  void MIPI_DCS_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
//  {
// 	SSD_B7 |= (SSD_CFGR_DCS | SSD_CFGR_REN);
// 	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);

// 	ReadDriverReg(port, reg, nPara, para);			
//  }

// /*********************************************************************************
//  * ��������MIPI_GEN_Read
//  * ����  ��ͨ��2828��IC�Ĵ���
//  * ����  ��-reg   IC�Ĵ�����ַ
//  *         -nPara IC�Ĵ�����������
//  * ���  ��-para  IC�Ĵ�������ֵ
//  * ����  ����
//  * ����  ���ڲ�����
//  * --------------------------------------------------------------------------------
//  *						MIPI(SSD_RR)				  SPI(SSD_RRA)
//  *		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
//  * --------------------------------------------------------------------------------
//  */
//  void MIPI_GEN_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
//  {
// 	SSD_B7 &= ~SSD_CFGR_DCS;
// 	SSD_B7 |= SSD_CFGR_REN;
// 	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);

// 	ReadDriverReg(port, reg, nPara, para);	
//  }

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
 	uint8_t buf[10];

	LCD_SleepIn();
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
		FPGA_DisPattern(0, 255, 255, 0);
		otpResult = ERROR;
	}   
	else if ((buf[4] & 0x01) == 0x01)
	{
		if ((buf[4] & 0x02) == 0x02)
		{
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", VCOM_Read());
			printf("OTP OK\r\n");
			FPGA_DisPattern(22, 128, 128, 128);						
			otpResult = SUCCESS;
		}
		else
		{
			printf("Write failed!\r\n");
			printf("OTP NG\r\n");
			FPGA_DisPattern(0, 255, 255, 0);
			otpResult = ERROR;			
		}
	}
	else
	{
		printf("Erase failed!\r\n");
		printf("OTP NG\r\n");
		FPGA_DisPattern(0, 255, 255, 0);
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
	LCD_LPMode();
	LCD_SleepOut();
	LCD_HSMode();

	return 	otpResult;
}
