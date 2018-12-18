#include "include.h"

uint16_t PWM_T_cnt = 0;
uint16_t PWM_DUTY = 0;

/*********************************************************************************
* Function: LCD_RST_Config
* Description: DDIC reset pin configure
* Input: none
* Output: none
* Return: none
* Call: external
 */
void LCD_RST_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LCD_nRST_PIN;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LCD_nRST_GPIO_PORT, &GPIO_InitStructure);	
	GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);	
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
	 Delay_ms(20);
	 GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(150);
 }

///*********************************************************************************
//* Function: LCD_SoftReset
//* Description: DDIC soft reset
//* Input: none
//* Output: none
//* Return: none
//* Call: external
//*/
//void LCD_SoftReset(void)
//{
//	printf("\r\nDriver IC solfware reset...\r\n");
//	SSD_B7 |= SSD_CFGR_DCS;
//	SSD_B7 &= ~SSD_CFGR_REN;
//	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
//	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0001);
//	Delay_ms(150);	 //delay more than 120ms
//}

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

	
//	 	uint8_t D3_buf[26], D3_P1, D3_P4;

//	printf("\r\n//==========R63350 sleep in start==========//\r\n");
//	SSD_B7 |= SSD_CFGR_DCS;
//	SSD_B7 &= ~SSD_CFGR_REN;
//	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
//	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);
//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);

//	MIPI_GEN_Read(MAIN_PORT, 0xD3, 26, D3_buf);
//	D3_P1 = D3_buf[0];
//	D3_P4 = D3_buf[3];
//	D3_buf[0] = 0x13; // D3 parameter 1 set 0x13
//	D3_buf[3] = 0xB3; // D3 parameter 4 set 0xB3
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD3, 26, D3_buf);
//	Delay_ms(5);	

//	SSD_B7 |= SSD_CFGR_DCS;
//	SSD_B7 &= ~SSD_CFGR_REN;
//	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
//	printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);
//	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
//	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);
//	Delay_ms(200);	 // ????120ms

//	D3_buf[0] = D3_P1; // D3 parameter 1 data recover.
//	D3_buf[3] = D3_P4; // D3 parameter 4 data recover.
//	MIPI_GEN_Write((PORT0 | PORT1), 0xD3, 26, D3_buf);
//	Delay_ms(5);	
////	MIPI_GEN_Read(MAIN_PORT, 0xD3, 26, D3_buf);

//	printf("\r\n//==========R63350 sleep in end==========//\r\n");
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
* Function: LCD_DisplayOff
* Description: DDIC display off
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_DisplayOff(void)
{
	printf("\r\nDriver IC display off...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);
	Delay_ms(10); //150 -> 10 @2018 V2P4
}

/*********************************************************************************
* Function: LCD_DisplayOn
* Description: DDIC display on
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_DisplayOn(void)
{
	printf("\r\nDriver IC display on...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0029);
	Delay_ms(10); //150 -> 10 @2018 V2P4
}

/*********************************************************************************
* Function: LCD_LPMode
* Description: MIPI enter low power mode
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_LPMode(void)
{
	printf("\r\nLCM enter low power mode...\r\n");
	SSD_B7 &= ~SSD_CFGR_HS;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: LCD_HSMode
* Description: MIPI enter high speed mode
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_HSMode(void)
{
	printf("\r\nLCM enter high speed mode...\r\n");
	SSD_B7 |= SSD_CFGR_HS;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: LCD_VideoMode_ON
* Description: MIPI vedeo mode enable
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_VideoMode_ON(void)
{
	printf("\r\nMIPI video mode on...\r\n");
	SSD_B7 |= SSD_CFGR_VEN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: LCD_VideoMode_OFF
* Description: MIPI vedeo mode disable
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_VideoMode_OFF(void)
{
	printf("\r\nMIPI video mode off...\r\n");
	SSD_B7 &= ~SSD_CFGR_VEN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: MIPI_SleepMode_ON
* Description: MIPI Sleep mode enable
* Input: none
* Output: none
* Return: none
* Call: external
*/
void MIPI_SleepMode_ON(void)
{
	printf("\r\nMIPI sleep mode on...\r\n");
	SSD_B7 |= SSD_CFGR_SLP;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: MIPI_SleepMode_OFF
* Description: MIPI Sleep mode disable
* Input: none
* Output: none
* Return: none
* Call: external
*/
void MIPI_SleepMode_OFF(void)
{
	printf("\r\nMIPI sleep mode off...\r\n");
	SSD_B7 &= ~SSD_CFGR_SLP;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
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
	LCD_PWM(0xFFF);
	LCD_SleepOut();
	LCD_HSMode();	
#ifdef CMD_MODE
	LCD_VideoMode_OFF();
#else
	LCD_VideoMode_ON();
#endif
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

	//8bit pwm
	wtBuf[0] = data & 0x00FF;
	PWM_DUTY = (wtBuf[0]*PWM_T/0xFF);
	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);	
	MIPI_DCS_Write((PORT0 | PORT1), 0x51, 1, wtBuf);
	
	//more than 8bit, for example: 12 bit pwm
//	wtBuf[0] = (data >> 8) & 0x000F;
//	wtBuf[1] = data & 0x00FF;
//	PWM_DUTY = (data*PWM_T/0x0FFF);
//	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);
//	MIPI_DCS_Write((PORT0 | PORT1), 0x51, 2, wtBuf);

	wtBuf[0] = 0x24; //0x2C
	MIPI_DCS_Write((PORT0 | PORT1), 0x53, 1, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x55, 1, wtBuf);
	Delay_ms(10);
}

void LEDA_DIM(void) //1mA
{
	LCD_PWM(0x000D); //8bit
//	LCD_PWM(0x00CC); //12bit
}

/*********************************************************************************
* Function: ARM_PWM_Control
* Description: ARM output PWM singal control
* Input: none
* Output: none
* Return: none
* Call: external
*/
void ARM_PWM_Control(void)
{
	if (PWM_T_cnt >= PWM_T - 1) //calculate form 0
	{
		PWM_T_cnt = 0;	
	}
	else
	{
		PWM_T_cnt = PWM_T_cnt + 1;	
	}
	
	if (PWM_T_cnt == PWM_DUTY) 
	{
#ifdef PWM_DETECT
#else		
		GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN); //test17 connect to DDIC output PWM pin (R8) on FET, high active
#endif
		GPIO_SetBits(POWER_LED_GPIO_PORT, POWER_LED_PIN); //enable pin on power board (R9), low active
	}
	else if (PWM_DUTY != 0 && PWM_T_cnt == 0)
	{
#ifdef PWM_DETECT
#else	
		GPIO_SetBits(TEST17_GPIO_PORT, TEST17_PIN);
#endif
		if (TEST_MODE == TEST_MODE_ET1 || TEST_MODE == TEST_MODE_ET2 || TEST_MODE == TEST_MODE_ET3 
			|| TEST_MODE == TEST_MODE_OTP || TEST_MODE == TEST_MODE_CTP)
		{
			GPIO_SetBits(POWER_LED_GPIO_PORT, POWER_LED_PIN); // 防止非PWM控制方式的power板点亮背光
		}
		else
		{
			GPIO_ResetBits(POWER_LED_GPIO_PORT, POWER_LED_PIN);
		}
	} 
}

/*********************************************************************************
* Function: LCM_Init
* Description: Initial SSD2828 and DDIC to lit on LCM
* Input: none
* Output: none
* Return: none
* Call: extern
*/
void LCM_Init(void)
{
	/* connection check */
	Connect_Check();
	
	/* SSD2828 initial */		
	MIPI_Reset();
	printf("\r\nSSD_Init...\r\n");
	debug = TIMESTAMP;
	IC_Init(SSDInitCode);
	printf("\r\n===== SSD2828 initial time elapsed: %.3f(second)\r\n", TIMESTAMP - debug);
// 	printf("SSD_B1 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB1));
// 	printf("SSD_B2 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB2));
// 	printf("SSD_B3 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB3));
// 	printf("SSD_B4 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB4));
// 	printf("SSD_B5 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB5));
// 	printf("SSD_B6 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB6));	
	
	/* DDIC initial */
	LCMPower_ON();
	Delay_ms(20);		
	POWER_I2C_SETTING(5.5, 5.5);
	Delay_ms(50);		
//	DriverIC_Reset();	
//	Delay_ms(20); 	
	DriverIC_Reset();
	printf("\r\nLCD_Init...\r\n");
	debug = TIMESTAMP;
	switch (TEST_MODE) 
	{
		case (TEST_MODE_ET1):	IC_Init(ET1_InitCode);	break;
		case (TEST_MODE_ET2):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_ET3):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_OTP):	IC_Init(ET1_InitCode); 	break;
		case (TEST_MODE_RA):	IC_Init(RA_InitCode);		break;
		case (TEST_MODE_ESD):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_OD):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_DEMO):IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_CTP):	IC_Init(ET1_InitCode);	break;
		case (TEST_MODE_OQC1):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_DEBUG):	IC_Init(ET1_InitCode);	break;	
		default: 				IC_Init(ET1_InitCode);	break;
	}
	printf("\r\n===== Driver IC initial time elapsed: %.3f(second)\r\n", TIMESTAMP - debug);

	if (TEST_MODE != TEST_MODE_RA)
	{
		/* OTP status check */
		debug = TIMESTAMP;	
		OTP_TIMES = OTPTimes_Read();
		if (OTP_TIMES == 0 && ( 
													TEST_MODE == TEST_MODE_ET2 || 
													TEST_MODE == TEST_MODE_ET3 ||
	//												TEST_MODE == TEST_MODE_RA ||
													TEST_MODE == TEST_MODE_ESD ||
													TEST_MODE == TEST_MODE_OD ||
													TEST_MODE == TEST_MODE_DEMO ||
													TEST_MODE == TEST_MODE_OQC1
													)
				) 
		{
			DriverIC_Reset();	
			IC_Init(ET1_InitCode);
		}	
		vcom_best = VCOM_Read();
		if (OTP_TIMES == 0)	printf("*#*#3:OTP NO#*#*\r\n"); 
		else printf("*#*#3:OTP YES#*#*\r\n");
		printf("*#*#4:0x%04X#*#*\r\n", vcom_best);
		printf("*#*#6:%d#*#*\r\n", OTP_TIMES);
		printf("\r\n===== OTP status check time elapsed: %.3f(second)\r\n", TIMESTAMP - debug);

		/* ID check */	
		IDCheck();
 	}
	
	/* discharge */
	LCD_LitSquence();
	FPGA_DisPattern(0, 0, 0, 0); 
	Delay_ms(100);
#ifdef CMD_MODE
	FPGA_DisPattern(0, 1, 1, 1);
#endif
	
#ifdef NO_FLASH_MODE
	if (TEST_MODE == TEST_MODE_CTP)
	{
		sprintf(FWVersion, "");
	}
	else if (TEST_MODE == TEST_MODE_RA)
	{	
		if (RA_Program_FW() == ERROR) 
		{
			FW_NG = SET;
			FPGA_Info_Visible(INFO_STR);
			FPGA_Info_Set((uint8_t *)"FW ERROR");	
			FPGA_DisPattern(114, 0, 0, 0);
			LED_ON(RED);
		}
	}
	else
	{
		if (Program_FW() == ERROR) 
		{
			FW_NG = SET;
			FPGA_Info_Visible(INFO_STR);
			FPGA_Info_Set((uint8_t *)"FW ERROR");	
			FPGA_DisPattern(114, 0, 0, 0);
			LED_ON(RED);
		}
	}
#endif
	
	if (TEST_MODE == TEST_MODE_CTP)
	{	
		TEST_Config_CTP();
	}
	
	if (FW_NG == RESET && TEST_MODE != TEST_MODE_OTP && TEST_MODE != TEST_MODE_ET3 && auto_line != SET) //OTP Mode NO sleep in current test! 2017-3-22 ywq add
	{
		SleepCurrent_Check();
	}
	
	/* TE detect */
	TE_Detect();
}

/*********************************************************************************
* Function: LCM_Reset
* Description: Reset the LCM
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCM_Reset()
{
//	LCD_SleepIn();
//	LCD_VideoMode_OFF();
//	MIPI_SleepMode_ON();
	MIPI_Reset();
	IC_Init(SSDInitCode);	
//	DriverIC_Reset();
//	Delay_ms(20);
	DriverIC_Reset();
	if (TEST_MODE == TEST_MODE_RA)
	{
			IC_Init(RA_InitCode);
	}
	else
	{
			IC_Init(ET2_InitCode);				
	}
	LCD_LitSquence();

#ifdef NO_FLASH_MODE
	if (TEST_MODE == TEST_MODE_CTP)
	{
		sprintf(FWVersion, "");
	}
	else if (TEST_MODE == TEST_MODE_RA)
	{	
		if (RA_Program_FW() == ERROR) 
		{
			FW_NG = SET;
			FPGA_Info_Visible(INFO_STR);
			FPGA_Info_Set((uint8_t *)"FW ERROR");	
			FPGA_DisPattern(114, 0, 0, 0);
			LED_ON(RED);
		}
	}
	else
	{
		if (Program_FW() == ERROR) 
		{
			FW_NG = SET;
			FPGA_Info_Visible(INFO_STR);
			FPGA_Info_Set((uint8_t *)"FW ERROR");	
			FPGA_DisPattern(114, 0, 0, 0);
			LED_ON(RED);
		}
	}
#endif
}
