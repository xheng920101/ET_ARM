#include "include.h"

uint8_t OTP_TIMES = 0;			    // OTP次数
uint16_t vcom_best = 0x00B2;		    // 最佳VCOM		
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
	unsigned char wtBuf[1];

//	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[0] = 0x80;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0xB400);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);

	wtBuf[0] = 0x01;                       // FB00=0x01
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xFB, 1, wtBuf);	
	Delay_ms(50);
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
	unsigned char wtBuf[1];

 //  	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	
	wtBuf[0] = 0x90;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0xB400);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);

	wtBuf[0] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);  // FB00=0x01
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xFB, 1, wtBuf);	
	Delay_ms(50);	
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
	unsigned char wtBuf[7];
	wtBuf[0] = vcom>>8;
	wtBuf[1] = vcom;
 	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);

 	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0100);  // Send sub address for Orise IC
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[1]);	
	
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
	unsigned char rdBuf[2];

	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xD9, 2, rdBuf);
	SSD_B7 &= (~SSD_CFGR_REN);	// write mode
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7);
  
	return (((rdBuf[0] & 0x00FF)<<8) + rdBuf[1]);	
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
 	uint8_t buf[6];
	 
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);		  //Display off
	Delay_ms(10);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);		  //Sleep in
	Delay_ms(150);

	LCD_VideoMode_OFF();		                        //2828 video mode off
	LCD_LPMode();									                  //2828 LP mode
  Delay_ms(10);
	 
	buf[0] = 0x19;
  buf[1] = 0x01;
	buf[2] = 0x01;	 
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, buf);	// Enable command 2 mdoe
	Delay_ms(10);

	buf[0] = 0x19;
  buf[1] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 2, buf);	// Enable orise mdoe
	Delay_ms(10);
	
	buf[0] = 0x5A;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8200);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xF6, 1, buf);	// F682=0x5A; unlock
	Delay_ms(1);

	buf[0] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf);	// EB00=0x01;To start VGH voltage
	Delay_ms(1);

	buf[0] = 0x14;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0xD100);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf);	// C5D1=0x14; Use to set VGH voltage when NVM program 14=8.0V
	Delay_ms(1);

	buf[0] = 0x20;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x8500);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xF6, 1, buf);	// F685=0x20; connect VGH to OTP circuit
	Delay_ms(10);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011);		  //Sleep out
	Delay_ms(150);
	
	buf[0] = 0x01;
  buf[1] = 0x5A;
	buf[2] = 0xA5;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf);	// EB00=0x01,0x5A,0xA5; Start OTP

	Delay_ms(1000);	                                // waiting for more than 850ms

	buf[0] = 0x00;
  buf[1] = 0x00;
	buf[2] = 0x00;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf);	// EB00=0x00,0x00,0x00;	 End OTP
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
