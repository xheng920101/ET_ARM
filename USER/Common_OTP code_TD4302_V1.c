#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP次数
uint16_t vcom_best = 0x002A;		// 最佳VCOM		
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1  // For TED use  PORT0


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
	unsigned char wtBuf[3];
	unsigned char wtBuf1[3];
  WriteSSDReg(MAIN_PORT, 0xB7, 0x0559);
	wtBuf1[0]=0x04;
  MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, wtBuf1);
	MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[2] &= 0xFE;
	MIPI_GEN_Write((PORT0 | PORT1), 0xC1, 3, wtBuf);
	
//	wtBuf[0] = 0x00;
//	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
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
	unsigned char wtBuf[3];
	unsigned char wtBuf1[3];
  WriteSSDReg(MAIN_PORT, 0xB7, 0x0559);
	wtBuf1[0]=0x04;
  MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, wtBuf1);
  MIPI_GEN_Read(MAIN_PORT, 0xC1, 3, wtBuf);
	wtBuf[2] |= 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xC1, 3, wtBuf);
	
//	wtBuf[0] = 0xC0;
//	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}

/*********************************************************************************
 * 函数名：Vcom_Set
 * 描述  ：设置VCOM
 * 输入  ：-VCOM值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
//TD4302
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[7];

	wtBuf[0] = 0x03; //0x03 For TD4302
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = vcom>>8;
	wtBuf[4] = vcom;
	wtBuf[5] = vcom>>8;
	wtBuf[6] = vcom;
	MIPI_GEN_Write((PORT0 | PORT1), 0xD5, 7, wtBuf);
//	Delay_ms(500);
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
	unsigned char rdBuf[7];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 7, rdBuf);

	return (rdBuf[3]<<8)+rdBuf[4];
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
	unsigned char rdBuf[6];

	MIPI_DCS_Read(MAIN_PORT, 0xA1, 6, rdBuf);

	return rdBuf[5];
}

/*********************************************************************************
 * 函数名：OTPSequence
 * 描述  ：OTP流程
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP执行结果
 * 调用  ：外部调用
 */
ErrorStatus OTPSequence(void)
{
	ErrorStatus otpResult;
	uint8_t buf[7];
	unsigned char Gamma_flag = 0;
	
	printf("\r\n=====OTP START!=====\r\n");
//-----------------------------------GAMMA OTP-----------------------------------//
	IC_Init(GAMMA_InitCode);
	Delay_ms(10); //How much delay?
	LCD_SleepIn();
	LCD_LPMode();
	LCD_VideoMode_OFF();
	
	WriteSSDReg(MAIN_PORT, 0xB7, 0x0510); //GCS
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);   
	WriteSSDReg(MAIN_PORT, 0xBF, 0x04B0); // Unlock Manufacture Command Protect (high-privilege).
	WriteSSDReg(MAIN_PORT, 0xBF, 0x10F4); //GCS Deletes tuned gamma from Flash.
	printf("\r\nStart erasing Gamma!\r\n");
	Delay_ms(300); //How much delay?
	WriteSSDReg(MAIN_PORT, 0xBF, 0x04B0); // Unlock Manufacture Command Protect (high-privilege).
	MIPI_GEN_Read(MAIN_PORT, 0xFB, 4, buf); //GCS read.

//	MIPI_DCS_Read(MAIN_PORT, 0xDA, 1, buf); //DCS read.
	if ((buf[3] & 0x30) == 0x30)
	{
		printf("\r\nErasing Gamma Successed!\r\n");
	}
	else
	{
		otpResult = ERROR;
		printf("\r\nErasing Gamma Failed!\r\n");
		return otpResult;
	}
	WriteSSDReg(MAIN_PORT, 0xB7, 0x0510); //GCS
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);	 
	WriteSSDReg(MAIN_PORT, 0xBF, 0x04B0); // Unlock Manufacture Command Protect (high-privilege).
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0FF4); //GCS Program tuned gamma to Flash.
	printf("\r\nStart programming Gamma!\r\n");
	Delay_ms(500); //NVM sequence set 300ms.
	WriteSSDReg(MAIN_PORT, 0xBF, 0x04B0); // Unlock Manufacture Command Protect (high-privilege).
	MIPI_GEN_Read(MAIN_PORT, 0xFB , 4, buf); //GCS read.
//	MIPI_DCS_Read(MAIN_PORT, 0xDA, 1, buf); //DCS read.
	if ((buf[3] & 0x10) == 0x10)
	{
		Gamma_flag = 1;
		printf("\r\nProgramming Gamma Successed!\r\n");
	}
	else
	{
		otpResult = ERROR;
		printf("\r\nProgramming Gamma Failed!\r\n");
		return otpResult;
	}
	printf("\r\n=====GAMMA OTP SUCCESSED!=====\r\n");
	
//-------------------------------VCOM&OTP_Time OTP-------------------------------//
	WriteSSDReg(MAIN_PORT, 0xB7, 0x0510); //GCS
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);   
	WriteSSDReg(MAIN_PORT, 0xBF, 0x11E8); //GCS Enable ID register access protect.
	Delay_ms(10); //Test
	//Write OTP_Time A1h 6th 
	MIPI_DCS_Read(MAIN_PORT, 0xA1, 6, buf);
	buf[5]++;
	MIPI_DCS_Write((PORT0 | PORT1), 0xA1, 6, buf);
	//Set OTP power-----change~~
//	POWER_OTP_Set();
  GPIO_SetBits(TEST25_GPIO_PORT, TEST25_PIN); //for vpp test		
	Delay_ms(150);
	//Start CPU to read display setting & write in NVM
	WriteSSDReg(MAIN_PORT, 0xB7, 0x0510); //GCS
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);   
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0EF4);  //GCS Start programming Vcom&ID to NVM memory.
	printf("\r\nStart programming Vcom&ID!\r\n");
	Delay_ms(350); //Delay for MTP request
	//Reset OTP power
	POWER_OTP_Reset();
	GPIO_ResetBits(TEST25_GPIO_PORT, TEST25_PIN); //for vpp test		

	Delay_ms(50);

//---------------------------------OTP check-------------------------------------//
//DAH: READ_ID1.v=Programmed_Slot;    // Slot id 0-7
//DBH: READ_ID2.v=7-Programmed_Slot;  // remaining slots
//DCH: READ_ID3.v=slot_address;       // Slot address 80-F0
//	MIPI_DCS_Read(MAIN_PORT, 0xA1, 6, buf);
	MIPI_DCS_Read(MAIN_PORT, 0xDA, 1, buf);
//	MIPI_DCS_Read(MAIN_PORT, 0xDB, 1, buf);
//	MIPI_DCS_Read(MAIN_PORT, 0xDC, 1, buf);
	OTP_TIMES = buf[0]+1;
	DriverIC_Reset();
	IC_Init(ET2_InitCode);
//	WriteSSDReg(MAIN_PORT, 0xB7, 0x0550); //DCS
//	LCD_PWM(0xFF);
	LCD_VideoMode_ON();
	LCD_HSMode();	
	Delay_ms(50);
	LCD_SleepOut();
//	MIPI_GEN_Read(MAIN_PORT, 0xD5, 7, buf); // Couldn't read back???
	MIPI_DCS_Read(MAIN_PORT, 0xA1, 6, buf);
	if(OTP_TIMES == buf[5] && Gamma_flag == 1)
	 {
		printf("\r\n=====OTP Successed!=====\r\n");
		otpResult = SUCCESS;
	 }
	 else 
	 {
		printf("\r\n=====OTP NG!=====\r\n");
		otpResult = ERROR;
	 }


	return 	otpResult;
}
///////
void MES()
{
	 uint8_t wr[2];
	 unsigned char wtBuf1[3];
	 int a;
	 FPGA_DisPattern(0, 255, 255, 255);
	 Delay_ms(500);
	 Meas_Yxy();
	 white = Lv;
	 chroma_Lv = Lv;
	 chroma_x = color_x;
	 chroma_y = color_y;
	 printf("white =%f \r\n",white);
   printf("white =%X \r\n",(int)white);
	 a=(int)(white-400);
	 printf("white =%X \r\n",a);
   wr[0]=a;
	
	 printf("white =%X \r\n",wr[0]);
//   WriteSSDReg(MAIN_PORT, 0xB7, 0x0559);
//	 wtBuf1[0]=0x04;
//   MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, wtBuf1);
	 wtBuf1[0]=0x11;
   MIPI_DCS_Write(MAIN_PORT, 0xE8, 1, wtBuf1);
	 wtBuf1[0]=0xD2;
   MIPI_DCS_Write(MAIN_PORT, 0xDA, 1, wtBuf1);
   MIPI_DCS_Write(MAIN_PORT, 0xDB, 1, wr);

}
