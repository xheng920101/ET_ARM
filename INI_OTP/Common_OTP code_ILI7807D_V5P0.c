#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP次数
uint16_t vcom_best = 0x011A;		// 最佳VCOM	
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1

unsigned short GAMMA_InitCode[]=
{
  //=======Gamma Code  Begin======//

     //?????????Gamma code

 //=======Gamma Code   End======//

    0xF00   //end of initialization
};

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
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x36, 1,wtBuf);
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
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
	wtBuf[0] = 0xC0;
	MIPI_DCS_Write((PORT0 | PORT1), 0x36, 1,wtBuf);
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
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
	wtBuf[0] = 0x33;
	MIPI_GEN_Write((PORT0 | PORT1), 0xA1, 1, wtBuf); // Select Vcom setting before and after OTP, If the panel had already been OTP, 00: use OTP value, 33: use initial value.
	wtBuf[0] = vcom>>8;
	MIPI_GEN_Write((PORT0 | PORT1), 0xA2, 1, wtBuf);
	wtBuf[0] = vcom;
	MIPI_GEN_Write((PORT0 | PORT1), 0xA3, 1, wtBuf);
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
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
	unsigned char 	 wtBuf[3];
	unsigned short  currentVcom;
	
	if (OTP_TIMES == 0) 
	{
		wtBuf[0] = 0x78;
		wtBuf[1] = 0x07;
		wtBuf[2] = 0x01;
		MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
		MIPI_GEN_Read(MAIN_PORT, 0xA2, 1, wtBuf);
		currentVcom = (wtBuf[0]&0x01) << 8;
		MIPI_GEN_Read(MAIN_PORT, 0xA3, 1, wtBuf);
		currentVcom += wtBuf[0];
	}
	else
	{
		wtBuf[0] = 0x78;
		wtBuf[1] = 0x07;
		wtBuf[2] = 0x08;
		MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
		MIPI_GEN_Read(MAIN_PORT, 0x0C, 1, wtBuf);
		currentVcom = (wtBuf[0]&0x01) << 8;
		MIPI_GEN_Read(MAIN_PORT, 0x0D, 1, wtBuf);
		currentVcom += wtBuf[0];
	}
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
	
	return currentVcom;
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
	unsigned char wtBuf[3], OTP_time;
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
	MIPI_GEN_Read(MAIN_PORT, 0xED, 1,  wtBuf);
	
	switch(wtBuf[0]&0x0F) // Only calculate low 4bits, this is Vcom1 OTP times.
	{
		case (0):
			OTP_time = 0;
			break;
		case (1):
			OTP_time = 1;
			break;
		case (3):
			OTP_time = 2;
			break;
		case (7):
			OTP_time = 3;
			break;
		case (0x0F):
			OTP_time = 4;
			break;
		default:
			OTP_time = 0;
			break;
	}
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf);
	
  return OTP_time;
}

/*********************************************************************************
 * 函数名：Byte_OTP
 * 描述  ：单字节OTP流程
 * 输入  ：-data, 待OTP寄存器的参数值
 * 			   -addrl, 待OTP寄存器的OTP地址低字节
 * 				 -addrh, 待OTP寄存器的OTP地址高字节 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：内部调用
 */
void Byte_OTP(uint8_t data, uint8_t addrl, uint8_t addrh)
{
	uint8_t buf[1];
	unsigned char wtBuf[3];
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page1
	
	buf[0] = data;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE0, 1, buf); // DATA Set 
	buf[0] = addrl;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 1, buf); // ADDRESS LSB
	buf[0] = addrh;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE2, 1, buf); // ADDRESS MSB
	buf[0] = 0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE4, 1, buf); // NVM PROTECTION KEY
	buf[0] = 0xAA;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE5, 1, buf); // NVM PROTECTION KEY
	buf[0] = 0x66;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE6, 1, buf); // NVM PROTECTION KEY
	buf[0] = 0x33;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE7, 1, buf); // USE 1-BYTE OTP FOR VCOM
	Delay_ms(10);	
}

/*********************************************************************************
 * 函数名：ID_Chroma_OTP
 * 描述  ：OTP ID, chroma...
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：内部调用
 */
void ID_Chroma_OTP(void)
{
		//If need to OTP ID, please uncomment the following code
//	Byte_OTP(ID1, 0x01, 0x00);
//	Byte_OTP(ID2, 0x02, 0x00);
//	Byte_OTP(ID3, 0x03, 0x00);	
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
 	uint8_t buf[1];
  unsigned char wtBuf[3];
	uint16_t vcom_otp;

//OTP sequence start.	
	debug = TIMESTAMP;
	printf("\r\nOTP sequence start!\r\n");
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page0
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028); // Display off
	Delay_ms(10);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010); // sleep in
	Delay_ms(120);
	LCD_VideoMode_OFF();
	LCD_LPMode();
	Delay_ms(10);

//	DriverIC_Reset(); 
//	
//	wtBuf[0] = 0x78;
//	wtBuf[1] = 0x07;
//	wtBuf[2] = 0x01;
//	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page1
//	buf[0] = 0xBB;
//	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, buf); // FTE/FTE1=NVM BUSY
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x07;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page7
	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x45, 1, buf); // PROG_VOLTAGE(VGHO)_GATING
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page0	
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011); // sleep out
	Delay_ms(120);

	//*************************Auto flow FOR initial except VCOM/ID *************************************//	
//	if (OTP_TIMES == 0) 
	{
		printf("\r\nAuto flow FOR initial except VCOM/ID!\r\n");
	//	IC_Init(ET1_InitCode);		
		wtBuf[0] = 0x78;
		wtBuf[1] = 0x07;
		wtBuf[2] = 0x01;
		MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page1
		buf[0] = 0x55;
		MIPI_GEN_Write((PORT0 | PORT1), 0xE4, 1, buf); // NVM PROTECTION KEY
		buf[0] = 0xAA;
		MIPI_GEN_Write((PORT0 | PORT1), 0xE5, 1, buf); // NVM PROTECTION KEY
		buf[0] = 0x66;
		MIPI_GEN_Write((PORT0 | PORT1), 0xE6, 1, buf); // NVM PROTECTION KEY
		buf[0] = 0xCC;
		MIPI_GEN_Write((PORT0 | PORT1), 0xE7, 1, buf); // USE AUTO OTP ALL (1-BYTE OTP set 0xE7=0x33)		
		Delay_ms(1500);
	}
	
	//*************************1-BYTE flow FOR VCOM & ID*************************************//	
	printf("\r\n1-BYTE flow FOR VCOM & ID!\r\n");
	buf[0] = vcom_best>>8;
	Byte_OTP(buf[0], 0x04, 0x00);
	buf[0] = vcom_best;
	Byte_OTP(buf[0], 0x05, 0x00);

	ID_Chroma_OTP();
	
	//OTP sequence finish.
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page0
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010); // Sleep in
	Delay_ms(150);
	printf("\r\nOTP sequence finish!\r\n");
	printf("\r\n===== OTP sequence time elapsed: %.3f(second)\r\n", TIMESTAMP - debug);
	
	//OTP check!
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3, wtBuf); // Page1
	MIPI_GEN_Read(MAIN_PORT, 0xF9, 1, buf);
	if ((buf[0] & 0x01) == 0x00) 
	{
		otpResult = SUCCESS;
		printf("OTP SUCCESS!\r\n");
	}
	else 
	{
		otpResult = ERROR;
		printf("OTP FAIL!\r\n");
	}

	DriverIC_Reset();
	
	//check otp times and vcom
	if (otpResult)
	{
		buf[0] = OTP_TIMES;
		OTP_TIMES = OTPTimes_Read();
		vcom_otp = VCOM_Read();
		if (OTP_TIMES <= buf[0])
		{
			printf("OTP_TIMES = %d, not change!\r\n", OTP_TIMES);
			printf("OTP NG\r\n");
			otpResult = ERROR;
		} 
		else if (vcom_best != vcom_otp) 
		{
			printf("vcom_best = 0x%04X, otp vcom is 0x%04X, different!\r\n", vcom_best, vcom_otp);		
			printf("OTP NG\r\n");
			otpResult = ERROR;
		}
		else
		{
			printf("OTP OK\r\n");
			otpResult = SUCCESS;		
		}
	}
	
	if (otpResult)
	{
		IC_Init(ET2_InitCode);
	}
	else 
	{
		IC_Init(ET1_InitCode);
	}
	LCD_LitSquence();

	return otpResult;
}

/*********************************************************************************
 * 函数名：ID_Read
 * 描述  ：回读ID
 * 输入  ：无 
 * 输出  ：无
 * 返回  : ID
 * 调用  ：外部调用
 */
unsigned char ID_Read(void)
{
	unsigned char wtBuf[3];
	unsigned char id;
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
	MIPI_GEN_Read(MAIN_PORT, 0x00, 1,  wtBuf);
	id = wtBuf[0];
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
	
	return id;
}

unsigned char ID_Read1(void)
{
	unsigned char wtBuf[3];
	unsigned char id;
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
	MIPI_GEN_Read(MAIN_PORT, 0x01, 1,  wtBuf);
	id = wtBuf[0];
	
	wtBuf[0] = 0x78;
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 3,  wtBuf);
	
	return id;
}

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
	 Delay_ms(50);
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
* Input: -data, PWM duty

* Output: none
* Return: none
* Call: external
*/
void LCD_PWM(uint16_t data)
{
	uint8_t wtBuf[2];

	//10bit pwm
	data = data & 0x03FF;
	wtBuf[0] = data >> 6;
	wtBuf[1] = data << 2;
	PWM_DUTY = (data*PWM_T/0x03FF);
	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);
	MIPI_DCS_Write((PORT0 | PORT1), 0x51, 2, wtBuf);

	wtBuf[0] = 0x24; //0x2C
	MIPI_DCS_Write((PORT0 | PORT1), 0x53, 1, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x55, 1, wtBuf);
	Delay_ms(10);
}

void LEDA_DIM(void)
{
	LCD_PWM(0x0033); //1mA
}

void LEDA_NORM(void)
{
	LCD_PWM(0x03FF); 
}

/*********************************************************************************
* Function: LCD_LitSquence
* Description: 
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_LitSquence(void)
{	
	LEDA_NORM();
	LCD_SleepOut();
	LCD_HSMode();	
#ifdef CMD_MODE
	LCD_VideoMode_OFF();
#else
	LCD_VideoMode_ON();
#endif
}

/*********************************************************************************
* Function: LCMPower_ON
* Description: power on timing, low active
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCMPower_ON(void)
{	
 	POWER_IOVCC_Reset();
	Delay_ms(10);
 	POWER_VCI_Reset();
	Delay_ms(1);
	POWER_VSP_Reset();	
	Delay_ms(1);
	POWER_VSN_Reset();
	Delay_ms(1);
	
	printf("\r\nLCMPower_ON...\r\n");
}

/*********************************************************************************
* Function: LCMPower_OFF
* Description: power off timing, low active
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCMPower_OFF(void)
{	
 	LCD_PWM(0x0000);
	Delay_ms(1); 
	POWER_VSN_Set();
	Delay_ms(1);
	POWER_VSP_Set();
	Delay_ms(1);	
 	POWER_VCI_Set();
	Delay_ms(1);
	POWER_IOVCC_Set();
	Delay_ms(1);
	
	printf("\r\nLCMPower_OFF...\r\n");
}

/*********************************************************************************
 * 函数名：PreGamma_Set
 * 描述  ：download pre-gamma 
 * 输入  ：无 
 * 输出  ：无
 * 返回  : 无
 * 调用  ：外部调用
 */
void PreGamma_Set(void)
{
	//add your code according to your project

}

/*********************************************************************************
 * 函数名：OSC_trim_Check
 * 描述  ：判断OSC机制是否开启 
 * 输入  : 无
 * 输出  : 无
 * 返回  ：无
 * 调用  ：外部调用
 */
void OSC_trim_Check(void)
{
	
}
