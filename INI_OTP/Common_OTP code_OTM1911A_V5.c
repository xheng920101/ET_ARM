
#include "include.h"

uint8_t OTP_TIMES = 0;			    // OTP����
uint16_t vcom_best = 0x00B2;		    // ���VCOM		
uint8_t MAIN_PORT = PORT1;	  	// ��MIPI port(��Ϊ�ض������ݿ�)	 WQXGA:PORT0 	HF/FHD:PORT1


/*********************************************************************************
* Function: DriverIC_Reset
* Description: DDIC reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void DriverIC_Reset(void)
 {
	 printf("\r\nDriver IC reset...\r\n");
	 GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(20);
	 GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(150);
 }
/*********************************************************************************
* Function: LCD_SoftReset
* Description: DDIC soft reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SoftReset(void)
{
	printf("\r\nDriver IC solfware reset...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0001);
	Delay_ms(150);	 //delay more than 120ms
}

/*********************************************************************************
* Function: LCD_SleepIn
* Description: DDIC sleep in and display off
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SleepIn(void)
{	
	printf("\r\nDriver IC display off then sleep in...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);
	Delay_ms(200);	  //delay more than 120ms
}

/*********************************************************************************
* Function: LCD_SleepOut
* Description: DDIC sleep out and display on
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SleepOut(void)
{
	printf("\r\nDriver IC display on then sleep out...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011);
	Delay_ms(200);	  //delay more than 120ms
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0029);	
}
/*********************************************************************************
* Function: LCD_PWM
* Description: BLU PWM duty setting
* Input: -data, PWM duty(0X00~0XFFF)
* Output: none
* Return: none
* Call: external
*/

void LCD_PWM(uint16_t data)
{
	uint8_t wtBuf[2];

	PWM_DUTY = (data*PWM_T/0xFFF);
	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);	
	wtBuf[1] = data & 0x000F;
  wtBuf[0] = (data & 0x0FF0) >> 4;	
	
  MIPI_DCS_Write((PORT0 | PORT1), 0x51, 2, wtBuf);  
	wtBuf[0] = 0x2C;
	MIPI_DCS_Write((PORT0 | PORT1), 0x53, 1, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x55, 1, wtBuf);
	Delay_ms(10);
}
/*********************************************************************************
* Function: LEDA_DIM()
* Description: BLU PWM duty setting for low current test
* Input: -data, PWM duty(12bit duty)
* Output: none
* Return: none
* Call: external
*/

void LEDA_DIM()
{
	LCD_PWM(0x66);
}
/*********************************************************************************
 * ��������LCD_LitSquence
 * ����  ����ɨ
 * ����  ���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */		
void LCD_LitSquence(void)
{	
	LCD_SleepOut();
	LCD_HSMode();
	#ifdef CMD_MODE
	LCD_VideoMode_OFF();
	#else
	LCD_VideoMode_ON();
	#endif
	LCD_PWM(0xfff);
}

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
	unsigned char wtBuf[1];

//	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[0] = 0x04;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8A00);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);

	
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
	unsigned char wtBuf[1];

 //  	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	
	wtBuf[0] = 0x05;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8A00);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);

	
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
	unsigned char wtBuf[1];
	wtBuf[0] = vcom;
	WriteSSDReg((PORT0 | PORT1), 0xB6, 0x00CB);
	WriteSSDReg((PORT0 | PORT1), 0xB7, 0x0219);
	//WriteSSDReg((PORT0 | PORT1), 0xB7, 0x0510);
	
 	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, wtBuf);

//	wtBuf[0] = vcom>>8;
//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0100);  // Send sub address for Orise IC
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, wtBuf);
//	
	WriteSSDReg((PORT0 | PORT1), 0xB7, 0x0519);
	
	
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
	unsigned char rdBuf[2];

	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xD9, 2, rdBuf);
	SSD_B7 &= (~SSD_CFGR_REN);	// write mode
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7);
  return (rdBuf[0]);	
//	return (((rdBuf[0] & 0x00FF)<<8) + rdBuf[1]);	
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
	unsigned char OTPTimes;
// 	SSD_B7 &= (~SSD_CFGR_HS);	//LP mode
//	SSD_B7 &= (~SSD_CFGR_VEN);  //video mode is disabled
  Delay_ms(10);	
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0200);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xF1, 1, rdBuf);
	SSD_B7 &= (~SSD_CFGR_REN);	// write mode
//	SSD_B7 |= SSD_CFGR_HS;	    //HS mode
//	SSD_B7 |= SSD_CFGR_VEN;     //video mode is enabled
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7);  	

	OTPTimes= rdBuf[0]& 0xF0;
	if(OTPTimes == 0x00)
	   return 0x00;
	else if (OTPTimes == 0x10)
	    return 0x01;
	else if (OTPTimes == 0x30)
	    return 0x02;
	else if (OTPTimes == 0x70)
	    return 0x03;
	else if (OTPTimes == 0xF0)
	    return 0x04;
	else
		return 0x04;
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
 	uint8_t buf[6];
	uint8_t buf_ID[3];
//	//~ for OTP tims >1
//	if(OTPTimes_Read() != 0x00)
//	{
//		DriverIC_Reset();							                  //Driver IC Reset	
//		IC_Init(ET2_InitCode);	                        //ET2 Initial code	
//	}
//	//~ 
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);		  //Display off
	Delay_ms(50);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);		  //Sleep in
	Delay_ms(150);

	LCD_VideoMode_OFF();		                        //2828 video mode off
	LCD_LPMode();									                  //2828 LP mode
  Delay_ms(10);
	 
	buf[0] = 0x19;
  buf[1] = 0x11;
	buf[2] = 0x01;	 
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, buf);	// Enable command 2 mdoe
	Delay_ms(10);

	buf[0] = 0x19;
  buf[1] = 0x11;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 2, buf);	// Enable orise mdoe
	Delay_ms(10);
	
	buf[0] = 0x5A;
  buf[1] = 0xA5;
	buf[2] = 0x01;	 
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x1000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf);	// EB10=0x01;To start VGH voltage
	Delay_ms(10);

	
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011);		  //Sleep out
	Delay_ms(120);
//	//~for OTP times >1
//	if(OTPTimes_Read()!=0x00){//write ID & Vcom
//	VCOM_Set(vcom_best);
//	buf_ID[0] = 0x5A;			//ID1													//set ID you want to save
//	buf_ID[1] = 0xA5;		  //ID2
//	buf_ID[2] = 0x5A;		  //ID3
//	
//  WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
//	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD0, 1, buf);	//Write ID1
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD1, 2, buf+1);//Write ID2 ID3			
//	}
//	//~for OTP times >1
	buf[0] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf);	// EB00=0x01; Start OTP

	Delay_ms(1000);	                                // waiting for more than 500ms

	buf[0] = 0x00;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf);	// EB00=0x00;	 End OTP
	Delay_ms(10);

	DriverIC_Reset();							                  //Driver IC Reset
  Delay_ms(10);

  IC_Init(ET2_InitCode);	                        //ET2 Initial code

	LCD_SleepOut();								                 	// Sleep out
	LCD_VideoMode_ON();							               	//2828 Video on
	LCD_HSMode();								                   	//2828 High Speed mode

	buf[0] = OTPTimes_Read();						            // Read OTP times to check OTP status
	buf[1] = VCOM_Read();                           // Read vcom to check
	
	if (OTP_TIMES == buf[0])
	{
		printf("NVM sequence doesn't finish!\r\n");
		printf("OTP NG\r\n");
		otpResult = ERROR;
	} 
  else if (buf[1] == vcom_best) 
  {		
	   if (buf[0] == 0x01)
	  {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", vcom_best);
			printf("OTP 1 Time\r\n");
			OTP_TIMES=0x01;					
			otpResult = SUCCESS;
	   }
	   else if (buf[0] == 0x02)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", vcom_best);
			printf("OTP 2 Time\r\n");
			OTP_TIMES=0x02;					
			otpResult = SUCCESS;
	   }
	   else if (buf[0] == 0x03)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", vcom_best);
			printf("OTP 3 Time\r\n");
			OTP_TIMES=0x03;						
			otpResult = SUCCESS;
	    }
	    else if (buf[0] == 0x04)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", vcom_best);
			printf("OTP 4 Time\r\n");
			OTP_TIMES=0x04;					
			otpResult = SUCCESS;
	    }
	    else
	    {
		   printf("Erase failed!\r\n");
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

	return 	otpResult;
}






