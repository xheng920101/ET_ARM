#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP����
uint16_t vcom_best = 0x12;		// ���VCOM		
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
	unsigned char wtBuf[1];
	wtBuf[0]=0x00;	
	MIPI_DCS_Write(MAIN_PORT, 0xFF, 1, wtBuf);//(PORT0 | PORT1)
	Delay_ms(5);	
	wtBuf[0] = 0x00;
	MIPI_DCS_Write(MAIN_PORT, 0x36, 1, wtBuf);
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
	wtBuf[0]=0x00;	
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	wtBuf[0] = 0x80;
	MIPI_DCS_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}
/*********************************************************************************
 * ��������Pagesel0
 * ����  ��Page select
 * ����  ���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void ChangeToCMD1(void)   //�л�CMD1
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);
}
/*********************************************************************************
 * ��������Pagesel1
 * ����  ��Page select
 * ����  ���� 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void ChangeToCMD2Page0(void)
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);
}

/*********************************************************************************
 * ��������Vcom_Set
 * ����  ������VCOM
 * ����  ��-VCOMֵ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 * ��ע  ��CMD2 PAGE0
 */
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[1];	
	
//	SSD_B7 = 0x0550;
//	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
//	Delay_ms(5);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);//MIPI_GEN_Write  //add 20151221
	Delay_ms(5);
	wtBuf[0] = 0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);//MIPI_GEN_Write
	Delay_ms(5);
	wtBuf[0] = vcom;	
	MIPI_DCS_Write((PORT0 | PORT1), 0x11, 1, wtBuf);//MIPI_DCS_Write
	Delay_ms(50); //20151221	5-->50
//	SSD_B7 = 0x0559;
//	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
//	Delay_ms(5);
}

/*********************************************************************************
 * ��������Chroma_Set    ɫ��Ũ��
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
	MIPI_DCS_Write((PORT0 | PORT1), 0xE2, 6, wtBuf);
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
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 9, wtBuf);
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
	unsigned char wtBuf[1];	
	unsigned char rdBuf[1];
	wtBuf[0]=0x01;	
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);
	MIPI_DCS_Read(MAIN_PORT, 0x11, 1, rdBuf);

	return rdBuf[0];
}

/*********************************************************************************
 * ��������NVM_Flag  OTP���
 * ����  ���ض�NVM_Flag
 * ����  ���� 
 * ���  ����
 * ����  ��NVM_Flag
 * ����  ���ⲿ����
 */
unsigned NVM_Flag(void)
{
 	ErrorStatus NVM_Status;	
	unsigned char wtBuf[1];
	unsigned char rdBuf[1];
	wtBuf[0]=0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	MIPI_DCS_Read(MAIN_PORT, 0x3F, 1, rdBuf);

  if(rdBuf[0]&0x01)
	{
		NVM_Status=SUCCESS;			//OTP success
	}
	
  else
	{
		NVM_Status=ERROR;      // unsuccess	
	}	
	return NVM_Status;
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
	unsigned char wtBuf[1];
	unsigned char rdBuf[1];
	unsigned char OTPTime[1];
	wtBuf[0]=0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);  
	Delay_ms(5);	
	MIPI_DCS_Read(MAIN_PORT, 0x3F, 1, rdBuf);
  switch(rdBuf[0])
   {
		case 0x00:  OTPTime[0]=0x00;
					break;
		case 0x10:  OTPTime[0]=0x01;
					break;
		case 0x30:  OTPTime[0]=0x02;
					break;
		case 0x70:  OTPTime[0]=0x03;
					break;
		case 0xF0:	OTPTime[0]=0x04;
					break;  
		default:    OTPTime[0]=0x00; 
   } 
	
  ChangeToCMD1();
	return OTPTime[0];
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
 	uint8_t buf[1];
	 
	POWER_OTP_Reset();  //Disable 7.5V Power 
		
	ChangeToCMD1();
	Delay_ms(200);		 	 
	LCD_DisplayOff();
	Delay_ms(50);	 
	LCD_VideoMode_OFF();
	Delay_ms(50);	 
	LCD_LPMode();
	Delay_ms(50);
 
	POWER_OTP_Set();//Enable 7.5V Power    //SSD_B7 = 0x0250
  Delay_ms(1300);
	 
	buf[0] = 0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, buf);	 
	Delay_ms(50);	 
	buf[0] = 0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0x3D, 1, buf); //MTP WRITE ENABLE
	Delay_ms(50);
	 
	buf[0] = 0x55;
	MIPI_DCS_Write((PORT0 | PORT1), 0x40, 1, buf);	//START OTP COMMAND  
	Delay_ms(50);		
	buf[0] = 0xAA;
	MIPI_DCS_Write((PORT0 | PORT1), 0x41, 1, buf);
	Delay_ms(20);
	buf[0] = 0x66;
	MIPI_DCS_Write((PORT0 | PORT1), 0x42, 1, buf);

	printf("Start OTP\r\n");
	Delay_ms(1500);
	
  if(NVM_Flag())
	{
		buf[0] = 0x01; 
		MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, buf);
		Delay_ms(50);	
		buf[0] = 0x00; 
		MIPI_DCS_Write((PORT0 | PORT1), 0x3D, 1, buf); //Disable MTP Write
		Delay_ms(50);  
		POWER_OTP_Reset();  //Disable 7.5V Power 
		Delay_ms(200);			
		
		DriverIC_Reset();	
		Delay_ms(60);
				
		buf[0] = 0x01; 
		MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, buf);
		Delay_ms(10);	
		buf[0] = 0x20; 
		MIPI_DCS_Write((PORT0 | PORT1), 0x43, 1, buf);		
		Delay_ms(100);		
								
//		MIPI_Reset();
//		Delay_ms(200);			
		IC_Init(SSDInitCode);	
//		Delay_ms(20);		
//		DriverIC_Reset();	
		Delay_ms(200);	
		IC_Init(ET2_InitCode);	
		Delay_ms(200);	
    OTP_TIMES = OTPTimes_Read();
    if(OTP_TIMES==0)	
    { otpResult=ERROR;		
      printf("OTP NOT SUCCESS!\n");
		}	
    printf("OTP SUCCESS!\n");		
	  ChangeToCMD1();		
		LCD_SleepOut();
		LCD_VideoMode_ON();
		LCD_HSMode();
	}
	
  else
	{
		otpResult=ERROR;;// 	
	} 
	
	return 	otpResult;
}
/*********************************************************************************
 * ��������VcomCheck
 * ����  ��check vcom
 * ����  ��-
 *         -
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
 void VcomCheck(void)
 {
     if(vcom_best != VCOM_Read())
   {
 	    LCMPower_OFF();
		  LED_ON(RED);
		  LED_OFF(BLUE);
   }
 }

/*********************************************************************************
 * ��������ICRegCheck
 * ����  ��check IC reg
 * ����  ��-reg   MIPI�ض���������ַ
 *         -nPara IC�Ĵ�����������
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
 ErrorStatus ICRegCheck(uint8_t nPara,uint8_t *ICReg,uint8_t *para)
 {
  ErrorStatus checkflag;
	uint8_t rdCnt ;
	unsigned char rdBuf[1];
    for(rdCnt = 0;rdCnt < nPara;rdCnt++)
	{
		MIPI_DCS_Read(MAIN_PORT, *(ICReg + rdCnt), 0x01, rdBuf);	
	  if(rdBuf[0] != *(para + rdCnt))
	  {
	   	checkflag=ERROR;
	    printf("\r\n IC reg check wrong 0x%02X : ", *(ICReg + rdCnt));
	    printf("\n Read back data is 0x%02X :", rdBuf[0]);			
	    printf("\n");			
		  break;
	  }
	  else
	  {
	  	checkflag=SUCCESS; 			
	  }
	}
   if(checkflag==ERROR)
   {
// 	      LCMPower_OFF();
//		 		LED_ON(RED);
//		    LED_OFF(BLUE);
   }
	 else
	 {
		 printf("Gamma check OK!\n");
	 }
	 return checkflag;
 }


/*********************************************************************************
 * ��������ICCheck_Gamma
 * ����  ��check IC Gamma R+  after OTP 
 * ����  ��-
 *         -
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
  */
 ErrorStatus ICCheck_Gamma(void)
{
	 	 ErrorStatus checkflag;
	   uint8_t buf[1],rdBuf[1];
	  	
    unsigned char RegBuf[60] =
    {
			0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,
			0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,
		  0x9D,0x9E,0x9F,0xA0,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,0xB2		
		};			                                                    	
    unsigned char DataBuf[60]=
		{
			0x00,0x39,0x00,0x54,0x00,0x7A,0x00,0x93,0x00,0xAA,0x00,0xBF,0x00,0xCA,0x00,0xDA,0x00,0xE9,0x01,0x12,
			0x01,0x38,0x01,0x6F,0x01,0x9D,0x01,0xE7,0x02,0x22,0x02,0x24,0x02,0x5B,0x02,0x98,0x02,0xBF,0x02,0xF4,
			0x03,0x15,0x03,0x41,0x03,0x4F,0x03,0x5D,0x03,0x6C,0x03,0x80,0x03,0x9A,0x03,0xB5,0x03,0xB9,0x03,0xBB		
		};
		
		LCD_DisplayOff();  //For except otp
		Delay_ms(50);	 
		
	  ChangeToCMD2Page0();					
	  checkflag=ICRegCheck(60,RegBuf,DataBuf);
	  ChangeToCMD1();	
		
		return checkflag;	
}
