#include "include.h"

uint8_t OTP_TIMES = 3;			// OTP次数
//uint8_t  ID;
uint16_t vcom_best = 0x86;		// 最佳VCOM		
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1

/*********************************************************************************
 * 函数名：ScanForward
 * 描述  ：正扫
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */											
void ScanForward(void)
{
	BYTE t[4];
	
  t[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xCC, 1, t);

  t[0] = 0x77;
	MIPI_GEN_Write((PORT0 | PORT1), 0xEF, 1, t);

  t[0] = 0xE1;
  t[1] = 0x21;
  t[2] = 0x03;
	t[3] = 0x30;
	MIPI_DCS_Write((PORT0 | PORT1), 0xB1, 4, t);
	
  t[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xEF, 1, t);
	Delay_ms(ET_DLY_LOCK * 100);
}

/*********************************************************************************
 * 函数名：ScanBackward
 * 描述  ：反扫
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void ScanBackward(void)
{
	BYTE t[4];
	
  t[0] = 0x04;
	MIPI_GEN_Write((PORT0 | PORT1), 0xCC, 1, t);

  t[0] = 0x77;
	MIPI_GEN_Write((PORT0 | PORT1), 0xEF, 1, t);

  t[0] = 0xE1;
  t[1] = 0x21;
  t[2] = 0x23;
	t[3] = 0x30;
	MIPI_DCS_Write((PORT0 | PORT1), 0xB1, 4, t);
	
  t[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xEF, 1, t);
}

/*********************************************************************************
 * 函数名：Vcom_Set
 * 描述  ：设置VCOM
 * 输入  ：-VCOM值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[3];

	wtBuf[0] = vcom;
	wtBuf[1] = vcom;
	wtBuf[2] = 0x03;
	MIPI_DCS_Write((PORT0 | PORT1), 0xB6, 3, wtBuf); //GEN -> DCS
}

/*********************************************************************************
 * 函数名：Chroma_Set
 * 描述  ：设置Chroma
 * 输入  ：-u,v值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
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
 * 函数名：Date_Set
 * 描述  ：设置Date
 * 输入  ：-日期 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
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
 * 函数名：VCOM_Read
 * 描述  ：回读VCOM
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：VCOM值
 * 调用  ：外部调用
 */
unsigned short VCOM_Read(void)
{
	unsigned char rdBuf[4];

	MIPI_DCS_Read(MAIN_PORT, 0xB6, 4, rdBuf);

	return rdBuf[1];
}

/*********************************************************************************
 * 函数名：OTPTimes_Read
 * 描述  ：回读OTP次数
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP次数
 * 调用  ：外部调用
 */
unsigned char OTPTimes_Read(void)
{
	unsigned char rdBuf[4];
	BYTE iReadData;

	MIPI_DCS_Read(MAIN_PORT, 0xB6, 4, rdBuf);		
	iReadData = rdBuf[3];
	printf("**The data read from B6 is:");
	printf("%02X",iReadData);
	printf("\r\n");
	if(iReadData==0xE0)
	{
		OTP_TIMES=0;
		printf("The OTP 0-times!\r\n");
		return ERROR;
	}
	else	if(iReadData==0x60)
	{
		OTP_TIMES=1;
		printf("The OTP 1-times!\r\n");
	}
	else if(iReadData==0x20)
	{
		OTP_TIMES=2;
		printf("The OTP 2-times!\r\n");
	}
	else if(iReadData==0x00)
	{
		OTP_TIMES=3;
		printf("The OTP 3-times!\r\n");
	}	
	else 
	{
		printf("can not read OTP times!\r\n");
		return ERROR;
	}
return OTP_TIMES;
}

BYTE HX8396A_READID (void)
{
  BYTE ireg[6];
	BYTE iReadData;

//	SSD_B7 |= SSD_CFGR_REN;	 // READ MODE 
	MIPI_GEN_Read(MAIN_PORT, 0xC3, 4, ireg);
 	iReadData=ireg[3];
	printf("**The data read from RC3h[4] is:");
	printf("%02X",iReadData);
	printf("\r\n");
	if(iReadData==0xB3)   
	{
	  printf("ID right\r\n");
		return SUCCESS;
	}

	else 
	{

		printf("ID error\r\n");
		return ERROR;
	}

}
/*********************************************************************************
 * 函数名：OTPSequence
 * 描述  ：OTP流程
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP执行结果
 * 调用  ：外部调用
 *	------------------------------------------------------------------------------
 * 	0xE0寄存器
 *		-第四个参数bit6：NVMFTT指示 OTP操作 是否完成(0->1: start squence	1->0: squence finish)
 *		-第五个参数bit0：NVMVFFLGER指示 erase 是否成功
 *  	-第五个参数bit1：NVMVFFLGWR指示 write 是否成功
 *	------------------------------------------------------------------------------
 */
ErrorStatus OTPSequence(void)
{
 	ErrorStatus otpResult;
 	uint8_t buf[8];
	uint8_t VCOMValue;
	uint8_t VCOMValue1;
	
	MIPI_DCS_Read(MAIN_PORT, 0xB6, 4, buf);
	VCOMValue = buf[1];	
	printf("VCOMValue=%x\r\n",VCOMValue);
//	VCOMValue1 = buf[4];	
//	printf("VCOMValue1=%x\r\n",VCOMValue1);
	
	FPGA_DisPattern(0, 0, 0, 0);//black
//	HX8396A_BypassYYG();   //Cut1 IC需要，Cut2 IC不需要
	Delay_ms(20);
	LCD_LPMode();	
	LCD_VideoMode_OFF();
	Delay_ms(40);
	
//	printf("The OTP 0-times!\r\n");
	buf[0]=0xA0;
	buf[1]=0x00;
	buf[2]=0x00;
  buf[3]=0x00;
	buf[4]=0x00;
	buf[5]=0xAA;
	buf[6]=0x55;
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);

	
	buf[0]=0xA0;  
	buf[1]=0x00;  
	buf[2]=0x00;
  buf[3]=0x01; 

	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf);
  Delay_ms(5000);

////----------C7h / BFh/ C0h /D2h 20150930------------------
	buf[0]=0x00;
	buf[1]=0x00;
	buf[2]=0x00;
  buf[3]=0x00;
	buf[4]=0x00;
	buf[5]=0x00;
	buf[6]=0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);


//	buf[0]=0x85;
//	buf[1]=0xA9;
//	buf[2]=0x00;
//  buf[3]=0x01;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); //C7
//	Delay_ms(100);
	

	buf[0]=0x83;
	buf[1]=0x7D;
	buf[2]=0x00;
  buf[3]=0x00;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf);

//	buf[0]=0x85;
//	buf[1]=0xB5;
//	buf[2]=0x00;
 // buf[3]=0x00;
	buf[4]=0x00;
	buf[5]=0xAA;
	buf[6]=0x55;
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);

	buf[0]=0x83;
	buf[1]=0x7D;
	buf[2]=0x00;
  buf[3]=0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); //BF
	Delay_ms(100);
	
	buf[0]=0x83;
	buf[1]=0x7D;
	buf[2]=0x00;
  buf[3]=0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); 

//	buf[0]=0x85;
//	buf[1]=0xBA;
//	buf[2]=0x00;
//  buf[3]=0x00;
//	buf[4]=0x00;
//	buf[5]=0xAA;
//	buf[6]=0x55;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);

//	buf[0]=0x85;
//	buf[1]=0xBA;
//	buf[2]=0x00;
//  buf[3]=0x01;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); //C0
//	Delay_ms(100);
//	
//	buf[0]=0x85;
//	buf[1]=0xBA;
//	buf[2]=0x00;
//  buf[3]=0x00;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf);

//	buf[0]=0x85;
//	buf[1]=0xC6;
//	buf[2]=0x00;
//  buf[3]=0x00;
//	buf[4]=0x00;
//	buf[5]=0xAA;
//	buf[6]=0x55;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);

//	buf[0]=0x85;
//	buf[1]=0xC6;
//	buf[2]=0x00;
//  buf[3]=0x01;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); //D2
//	Delay_ms(100);
//	
//	buf[0]=0x85;
//	buf[1]=0xC6;
//	buf[2]=0x00;
//  buf[3]=0x00;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 4, buf); 

//----------C7h / BFh/ C0h /D2h 20150930------------------

 // buf[0]=0x00;
//	buf[1]=0x00;
//	buf[2]=0x00;
//  buf[3]=0x00;
//	buf[4]=0x00;
//	buf[5]=0x00;
//	buf[6]=0x00;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xE1, 7, buf);

	MIPI_DCS_Read(MAIN_PORT,0xB6, 4, buf); //read VCOM
	if (buf[1]  == VCOMValue )  //vcom
	{
		  printf("OTP SUCESS\r\n");
		  otpResult = SUCCESS;
		  LCD_SleepOut();
			LCD_VideoMode_ON();
		  LCD_PWM(0xFF);
	  	OTP_TIMES=OTPTimes_Read();
		  FPGA_Info_Visible(INFO_OTPTIMES|INFO_VERSION); 
		 		  
		  FPGA_DisPattern(109, 128, 0, 0);//
		  Delay_ms(20);
		  LCD_HSMode();
		 	Delay_ms(2000);
			LCD_SleepIn();
			LCD_VideoMode_OFF();
			LED_OFF(RED);
			LED_ON(GREEN);
			LED_OFF(BLUE);
			printf("SLEEP IN!\r\n");
	}   
	
	else
	{
		  //printf("OTP NG\r\n");
		  otpResult = ERROR;
			LED_ON(RED);
			LED_OFF(GREEN);
			LED_OFF(BLUE);
		  LCD_SleepOut();
		  LCD_PWM(0xFF);
		  FPGA_DisPattern(85, 0, 0, 0);  //NG
		  Delay_ms(20);
		  LCD_HSMode();
		  Delay_ms(20);
			printf("OTP fail !!!\r\n");
	}			
	  
	return 	otpResult;
}

void HX8396A_SETExtensionCommand (void)
{
	BYTE wtBuf[3];
	wtBuf[0]=0xff;
	wtBuf[1]=0x83;
	wtBuf[2]=0x96;

	MIPI_GEN_Write((PORT0 | PORT1), 0xB9, 3, wtBuf);
}

void HX8396A_EnterOTPCommand (void)
{
	BYTE wtBuf[7];
	wtBuf[0]=0xA0;//A0;
	wtBuf[1]=0x00;
	wtBuf[2]=0x00;
  wtBuf[3]=0x00;
	wtBuf[4]=0x00;
	wtBuf[5]=0xAA;
	wtBuf[6]=0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 7, wtBuf);
}

void HX8396A_VCOMEnterOTPCommand (void)
{
	BYTE wtBuf[7];
	wtBuf[0]=0x80;
	wtBuf[1]=0x0D;  //VCOM OTP index
	wtBuf[2]=0x00;
  wtBuf[3]=0x00;
	wtBuf[4]=0x00;
	wtBuf[5]=0xAA;
	wtBuf[6]=0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 7, wtBuf);
}

void HX8396A_IDEnterOTPCommand (void)
{
	BYTE wtBuf[7];
	wtBuf[0]=0x80;
	wtBuf[1]=0x00;
	wtBuf[2]=0x00;
  wtBuf[3]=0x00;
	wtBuf[4]=0x00;
	wtBuf[5]=0xAA;
	wtBuf[6]=0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 7, wtBuf);
}

void HX8396A_OTPEnableCommand (void)
{
	BYTE wtBuf[4];
	wtBuf[0]=0xA0;  //Set INTVPP_EN=1,Set OTP_PROG_ALL=1
	wtBuf[1]=0x00;  //
	wtBuf[2]=0x00;
  wtBuf[3]=0x01; //enable OTP program write,Set OTP_PROG=1

	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 4, wtBuf);
}

void HX8396A_VCOMOTPEnableCommand (void)
{
	BYTE wtBuf[4];
	wtBuf[0]=0x80;  //Set INTVPP_EN=1
	wtBuf[1]=0x0D;  //VCOM OTP index
	wtBuf[2]=0x00;
  wtBuf[3]=0x01; //enable OTP program write,Set OTP_PROG=1

	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 4, wtBuf);
}

void HX8396A_IDOTPEnableCommand (void)
{
	BYTE wtBuf[4];
	wtBuf[0]=0x80;  //Set INTVPP_EN=1
	wtBuf[1]=0x00;
	wtBuf[2]=0x00;
  wtBuf[3]=0x01;  //enable OTP program write,Set OTP_PROG=1

	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 4, wtBuf);
}

void HX8396A_QuitOTPCommand (void)
{
	BYTE wtBuf[7];
	wtBuf[0]=0x00;
	wtBuf[1]=0x00;
	wtBuf[2]=0x00;
  wtBuf[3]=0x00;
	wtBuf[4]=0x00;
	wtBuf[5]=0x00;
	wtBuf[6]=0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 7, wtBuf);
}

void HX8396A_BypassYYG(void)
{
  BYTE wtBuf[1];
	wtBuf[0]=0x00;
	
	MIPI_GEN_Write((PORT0 | PORT1), 0xB8, 1, wtBuf);
}

void HX8396A_YYG(void)
{
  BYTE wtBuf[1];
	wtBuf[0]=0xA5;
	
	MIPI_GEN_Write((PORT0 | PORT1), 0xB8, 1, wtBuf);
}
