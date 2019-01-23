#include "include.h"

uint8_t OTP_TIMES = 0;				// OTP次数
uint16_t vcom_best = 0x007A;	// 最佳VCOM		
uint8_t MAIN_PORT = PORT1;		// 主MIPI port(作为回读的数据口),与转接线一致

unsigned short GAMMA_InitCode[]=
{
  //=======Gamma Code  Begin======//

  0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E100: R+
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E1,0x200,0x201,0x207,0x210,0x205,0x21c,0x224,0x22a,0x235,0x21d,0x23d,0x244,0x24a,0x24f,0x255,0x254,0x25c,0x263,0x26a,0x21a,0x271,0x278,0x280,0x288,0x252,0x291,0x297,0x29e,0x2a5,0x262,0x2ae,0x2b8,0x2c6,0x2cf,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E200: R-
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E2,0x200,0x201,0x207,0x210,0x205,0x21a,0x222,0x228,0x233,0x21d,0x23b,0x242,0x248,0x24d,0x255,0x252,0x25a,0x261,0x268,0x21a,0x26f,0x276,0x27e,0x286,0x252,0x28f,0x295,0x29c,0x2a3,0x262,0x2ac,0x2b6,0x2c4,0x2cd,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	
	0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E300: G+
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E3,0x200,0x201,0x207,0x210,0x205,0x21c,0x224,0x22a,0x235,0x21d,0x23d,0x244,0x24a,0x24f,0x255,0x254,0x25c,0x263,0x26a,0x21a,0x271,0x278,0x280,0x288,0x252,0x291,0x297,0x29e,0x2a5,0x262,0x2ae,0x2b8,0x2c6,0x2cf,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E400: G-
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E4,0x200,0x201,0x207,0x210,0x205,0x21a,0x222,0x228,0x233,0x21d,0x23b,0x242,0x248,0x24d,0x255,0x252,0x25a,0x261,0x268,0x21a,0x26f,0x276,0x27e,0x286,0x252,0x28f,0x295,0x29c,0x2a3,0x262,0x2ac,0x2b6,0x2c4,0x2cd,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	
	0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E500: B+
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E5,0x200,0x201,0x207,0x210,0x205,0x21c,0x224,0x22a,0x235,0x21d,0x23d,0x244,0x24a,0x24f,0x255,0x254,0x25c,0x263,0x26a,0x21a,0x271,0x278,0x280,0x288,0x252,0x291,0x297,0x29e,0x2a5,0x262,0x2ae,0x2b8,0x2c6,0x2cf,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	0x1BC, 0x202, 0x200,  
	0x1BF, 0x200, 0x200, //E600: B-
	0x1BC, 0x229, 0x200, 
	0x1BF, 0x2E6,0x200,0x201,0x207,0x210,0x205,0x21a,0x222,0x228,0x233,0x21d,0x23b,0x242,0x248,0x24d,0x255,0x252,0x25a,0x261,0x268,0x21a,0x26f,0x276,0x27e,0x286,0x252,0x28f,0x295,0x29c,0x2a3,0x262,0x2ac,0x2b6,0x2c4,0x2cd,0x265,0x2db,0x2eb,0x2f8,0x2ff,0x233,
	

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
	unsigned char wtBuf[1];

	wtBuf[0] = 0x40;
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0xE800); //0xC0E8	
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);
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

	wtBuf[0] = 0x10;
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0xE800); //0xC0E8	
	MIPI_GEN_Write((PORT0 | PORT1), 0xC0, 1, wtBuf);
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

	wtBuf[0] = (vcom >> 8) & 0x01;
	wtBuf[0] = wtBuf[0] + (wtBuf[0] << 1);
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0000); //0xD900: 	
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, wtBuf);

	wtBuf[0] = vcom;
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0100); //0xD901: VCOM_NORM 
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, wtBuf);
	
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0200); //0xD902: VCOM_IDLE
	MIPI_GEN_Write((PORT0 | PORT1), 0xD9, 1, wtBuf);
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
	unsigned short vcom;
	
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000);  //0xD900
	MIPI_GEN_Read(MAIN_PORT, 0xD9, 2, rdBuf);
	SSD_B7 &= (~SSD_CFGR_REN);	//write mode, it is necessary for next read circle
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7); 
  
	vcom = rdBuf[0] & 0x02;
	return ((vcom << 8) + rdBuf[1]);	
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
	
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0100); //0xF101
	MIPI_GEN_Read(MAIN_PORT, 0xF1, 2, rdBuf); 
	SSD_B7 &= (~SSD_CFGR_REN);	//write mode, it is necessary for next read circle
	WriteSSDReg(MAIN_PORT, SSD_CFGR, SSD_B7);  	

	OTPTimes = rdBuf[1] & 0xF0;
	if (OTPTimes == 0x00)
	{
		return 0;
	}
	else if (OTPTimes == 0x10)
	{
		return 1;
	}
	else if (OTPTimes == 0x30)
	{
		return 2;
	}
	else if (OTPTimes == 0x70)
	{
		return 3;
	}
	else if (OTPTimes == 0xF0)
	{
		return 4;
	}
	else
	{
		return 4;
	}
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
//	uint8_t buf[2];
//	
//	buf[0] = ID1;
//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0000); //0xD000
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD0, 1, buf);
//	
//	buf[0] = ID2;
//	buf[1] = ID3;
//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0000); //0xD100
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD1, 2, buf);	
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
	uint8_t buf[3];

//	if (OTP_TIMES > 0) //防止其他代码二次OTP
//	{
//		DriverIC_Reset();
//		IC_Init(ET2_InitCode);
//		VCOM_Set(vcom_best);
//	}
	
	ID_Chroma_OTP();

	debug = TIMESTAMP;
	LCD_SleepIn(); //28 10
	LCD_VideoMode_OFF(); //2828 video mode off
	LCD_LPMode(); //2828 LP mode
	
	//Turn on internal NVM write mode
	buf[0] = 0x5A;
	buf[1] = 0xA5;
	buf[2] = 0x01; 
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x1000); //0xEB10
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf); 
	Delay_ms(15);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011); //Sleep out
	Delay_ms(300); //delay to make sure internal NVMD is stable to 8.0V
	
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028); //Display off
	Delay_ms(15);
	
	//Start NVM program
	buf[0] = 0x01;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000); //0xEB00           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf); 
	Delay_ms(1000);
	
	//End NVM program
	buf[0] = 0x00;
	WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
	WriteSSDReg(MAIN_PORT, 0xBF, 0x0000); //0xEB00           
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 1, buf); 
	Delay_ms(15);
	
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010); //Sleep in
	Delay_ms(100);
	
	//Turn off internal NVM write mode
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0002);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x1000); //0xEB10
	MIPI_GEN_Write((PORT0 | PORT1), 0xEB, 3, buf); 
	Delay_ms(10);
	printf("\r\n===== OTP sequence time elapsed: %.3f(second)\r\n", TIMESTAMP - debug);

	DriverIC_Reset();	
  IC_Init(ET2_InitCode);
	
	buf[0] = OTP_TIMES;
	buf[1] = VCOM_Read();
	OTP_TIMES = OTPTimes_Read(); //20171221: HS下连续读会有问题，所以移到LP下回读	
	LCD_LitSquence();	
	
	if (OTP_TIMES <= buf[0])
	{
		printf("OTP_TIMES = %d, not change!\r\n", OTP_TIMES);
		printf("OTP NG\r\n");
		otpResult = ERROR;
	} 
  else if (vcom_best != buf[1]) 
  {	
		printf("vcom_best = %0x4X, otp vcom is %0x4X, different!\r\n", vcom_best, buf[1]);		
		printf("OTP NG\r\n");
		otpResult = ERROR;
	}
  else
  {
		printf("OTP OK\r\n");
		otpResult = SUCCESS;		
	}	
	
	return 	otpResult;
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
	 GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(5);
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
* Input: -data, PWM duty(0X00~0XFF)
* Output: none
* Return: none
* Call: external
*/
void LCD_PWM(uint16_t data)
{
	uint8_t wtBuf[2];

	//8bit pwm
	wtBuf[0] = data & 0x00FF;
	PWM_DUTY = (wtBuf[0]*PWM_T/0xFF);
	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);	
	MIPI_DCS_Write((PORT0 | PORT1), 0x51, 1, wtBuf);

	wtBuf[0] = 0x24; //0x2C
	MIPI_DCS_Write((PORT0 | PORT1), 0x53, 1, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x55, 1, wtBuf);
	Delay_ms(10);
}

void LEDA_DIM(void) //1mA
{
	LCD_PWM(0x000D);
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
	LCD_PWM(0x0FFF);
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
