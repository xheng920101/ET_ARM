#include "include.h"

uint8_t OTP_TIMES = 0;			    // OTP次数
uint16_t vcom_best = 0x006D;		    // 最佳VCOM		
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1

//-------------Sensor Detect------------------//
// uint8_t P1_4_n = RESET;
// uint8_t P1_5_n = RESET;
// uint8_t rbuf_C589[1];
// uint8_t rbuf_C595[3];
// FunctionalState SensorDetect = ENABLE;
//--------------------------------------------//

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

  WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0xB400);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xC0, 1, wtBuf);
	
	wtBuf[0] &= 0xE0;  //set SCAN_MIRROR_Y2 = 0 (bit[4])
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

  WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0xB400);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xC0, 1, wtBuf);
	
	wtBuf[0] |= 0x10;  //set SCAN_MIRROR_Y2 = 1 (bit[4])
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
	unsigned char wtBuf[1];
	wtBuf[0] = vcom;

 	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	
	wtBuf[0] = 0x80;
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  // REG_EN
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);

	wtBuf[0] = vcom;
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0100);  // VCOM_NORM_NOR
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);

//	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0200);  // VCOM_NORM_REV
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);
	
//	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0300);  // VCOM_IDLE_NOR
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);

//	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0400);  // VCOM_IDLE_REV
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, &wtBuf[0]);
	
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
	unsigned char rdBuf[1];

	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0100);  // Send sub address for Orise IC
	MIPI_GEN_Read(MAIN_PORT, 0xD9, 1, rdBuf);
	SSD_B7 &= (~SSD_CFGR_REN);	// write mode
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7);
  
	return (rdBuf[0]);	
	
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
 	uint8_t buf[3];

	DriverIC_Reset();							                  //Driver IC Reset
	LCD_VideoMode_OFF();		                        //2828 video mode off
	LCD_LPMode();									                  //2828 LP mode
  Delay_ms(100);

	if (OTP_TIMES == 0)
	{
		printf("\r\nOTP_TIMES = 0, must download initial, use ET1_InitCode and refresh Vcom!\r\n");
		IC_Init(ET1_InitCode);
		VCOM_Set(vcom_best);
	}
	else
	{
		printf("\r\nOTP_TIMES >= 1, mustn't download initial, refresh Vcom only!\r\n");
		IC_Init(ET2_InitCode);
		VCOM_Set(vcom_best);
	}

// EB10=0x5A,0xA5,0X01; Turn on internal NVM
	buf[0] = 0x5A;
	buf[1] = 0xA5;
	buf[2] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x1000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf);	// EB10=0x5A,0xA5,0X01; Turn on internal NVM
	Delay_ms(20);  // FT setting 10

//Display off
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);		  //Display off
//	Delay_ms(10);

//Sleep out
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011);		  //Sleep out
	Delay_ms(300); // FT setting 250

// EB00=0x01;OTP Start
	buf[0] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf);	// EB00=0x01;OTP Start
	Delay_ms(1000); // FT setting 850

// if EB00=0x00;OTP End
	buf[0] = 0x00;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf);	// EB00=0x00;OTP End
	Delay_ms(20); // FT setting 10

//Sleep in
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);		  //Sleep in
	Delay_ms(100); // FT setting 70

// Turn off internal NVM
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x1000);           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf);	// Turn off internal NVM

	DriverIC_Reset();		//Driver IC Reset
	
	Delay_ms(100); 
	
	buf[0] = OTPTimes_Read();						            // Read OTP times to check OTP status
	buf[1] = VCOM_Read();
	
  IC_Init(ET2_InitCode);		                      //ET2 Initial code
  
	Delay_ms(100); 
	buf[0] = OTPTimes_Read();						            // Read OTP times to check OTP status
	buf[1] = VCOM_Read();
  
	LCD_SleepOut();								                 	// Sleep out
	LCD_VideoMode_ON();							               	//2828 Video on
	LCD_HSMode();								                   	//2828 High Speed mode
	
	LCD_PWM(0xFF);
	
	buf[0] = OTPTimes_Read();						            // Read OTP times to check OTP status
	buf[1] = VCOM_Read();                           // Read vcom to check
	
	if (OTP_TIMES == buf[0])
	{
		//printf("NVM sequence doesn't finish!\r\n");
		printf("OTP NG\r\n");
		printf("******** OTP ERROR 5: otp times doesn't increase after NVM!\r\n");
		otpResult = ERROR;
	} 
  else if (buf[1] == vcom_best) 
  {		
	   if (buf[0] == 0x01)
	  {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", buf[1]);
			printf("OTP 1 Time\r\n");
			OTP_TIMES=0x01;					
			otpResult = SUCCESS;
	   }
	   else if (buf[0] == 0x02)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", buf[1]);
			printf("OTP 2 Time\r\n");
			OTP_TIMES=0x02;					
			otpResult = SUCCESS;
	   }
	   else if (buf[0] == 0x03)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", buf[1]);
			printf("OTP 3 Time\r\n");
			OTP_TIMES=0x03;						
			otpResult = SUCCESS;
	    }
	    else if (buf[0] == 0x04)
	   {
			printf("NVM write passed!\r\n");
			printf("NVM VCOM is: 0x%02X\r\n", buf[1]);
			printf("OTP 4 Time\r\n");
			OTP_TIMES=0x04;					
			otpResult = SUCCESS;
	    }
	    else
	    {
		   //printf("Erase failed!\r\n");
			 printf("******** OTP ERROR 6: otp times illegal after NVM!\r\n");
		   printf("OTP NG\r\n");
		   otpResult = ERROR;
	    }
		}
  else
  {
//		   printf("Erase failed!\r\n");
		   printf("******** OTP ERROR 7: vcom read-back failed!\r\n");
		   printf("OTP NG\r\n");
		   otpResult = ERROR;		
	}		
	return 	otpResult;
}

// /*********************************************************************************
//  * 函数名：Sensor_Detect
//  * 描述  ：检测Sensor连接
//  * 输入  ：无 
//  * 输出  ：无
//  * 返回  ：无
//  * 调用  ：外部调用
//  */
// void Sensor_Detect(void)
// {
// 	uint8_t P1_4, P1_5, P1_4_d, P1_5_d;
// 	uint8_t Detect_Status;
//  	uint8_t buf[3];

// 	P1_4 = GetStatus_TestPin(TEST1);
// 	P1_5 = GetStatus_TestPin(TEST2);
// 	printf("\r\nP1_4 is: %d\r\n", P1_4);
// 	printf("P1_5 is: %d\r\n", P1_5);
// 	printf("P1_4_n is: %d\r\n", P1_4_n);
// 	printf("P1_5_n is: %d\r\n", P1_5_n);
// 	printf("Sensor_Detect!\r\n");
// 	if (P1_4 == P1_4_n && P1_5 == P1_5_n) return;
// 	printf("\r\nChange OK!\r\n");
// 	P1_4_d = P1_4;
// 	P1_5_d = P1_5;
// 	Delay_ms(10);
// 	P1_4 = GetStatus_TestPin(TEST1);
// 	P1_5 = GetStatus_TestPin(TEST2);
// 	if (P1_4 != P1_4_d || P1_5 != P1_5_d) return;
// 	printf("\r\nJitter OK!\r\n");
// 	P1_4_n = P1_4;
// 	P1_5_n = P1_5;
// 	
// 	if (P1_4 == SET && P1_5 == SET) Detect_Status = 0;
// 	else if (P1_4 == RESET && P1_5 == RESET) Detect_Status = 1;
// 	else if (P1_4 == RESET && P1_5 == SET) Detect_Status = 2;
// 	else if (P1_4 == SET && P1_5 == RESET) Detect_Status = 3;
// 	
// 	switch (Detect_Status)
// 	{
// 		case 0:
// 			printf("\r\n0 status!\r\n");
// 			return;
// //		break;
// 		case 1:
// 			printf("\r\n1 status!\r\n");
// 			buf[0] = 0x06;
// 			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
// 			WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
// 			MIPI_GEN_Read(MAIN_PORT, 0xC5, 1, rbuf_C589);
// 			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf); //
// 			buf[0] = 0x11;
// 			buf[1] = 0x32;
// 			buf[2] = 0x50;
// 			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
// 			WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
// 			MIPI_GEN_Read(MAIN_PORT, 0xC5, 3, rbuf_C595);
// 			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, buf); //
// 		break;
// 		case 2:
// 			printf("\r\n2 status!\r\n");
// 			buf[0] = 0x05;
// 			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
// 			WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
// 			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf); //
// 		break;
// 		case 3:
// 			printf("\r\n3 status!\r\n");
// 			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
// 			WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
// 			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, rbuf_C589); //
// 			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
// 			WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
// 			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, rbuf_C595); //
// 		break;
// 		default:
// 			printf("\r\nDefault status!\r\n");
// 			return;
// //		break;
// 	}
// }
