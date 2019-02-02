#include "include.h"

/* TEST_MODE can be a value of the list below */
//	TEST_MODE_ET1
//	TEST_MODE_ET2	  
//	TEST_MODE_ET3
//	TEST_MODE_OTP
//	TEST_MODE_RA
//	TEST_MODE_ESD
//	TEST_MODE_OD
//	TEST_MODE_DEMO
//	TEST_MODE_CTP
//	TEST_MODE_DEBUG
uint8_t TEST_MODE = TEST_MODE_ET1;	 		

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
	/* SSD2828 initial */		
	MIPI_Reset();
	printf("\r\nSSD_Init...\r\n");
	IC_Init(SSDInitCode);		
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
	DriverIC_Reset();	
	Delay_ms(500); 	
	printf("\r\nLCD_Init...\r\n");
	switch (TEST_MODE) 
	{
		case (TEST_MODE_ET1):	IC_Init(ET1_InitCode);	break;
		case (TEST_MODE_ET2):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_ET3):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_OTP):	IC_Init(ET1_InitCode); 	break;
		case (TEST_MODE_RA):	IC_Init(RA_InitCode);	break;
		case (TEST_MODE_ESD):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_OD):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_DEMO):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_CTP):	IC_Init(ET1_InitCode);	break;
		case (TEST_MODE_OQC1):	IC_Init(ET2_InitCode);	break;
		case (TEST_MODE_DEBUG):	IC_Init(ET2_InitCode);	break;		
		default: 				IC_Init(ET1_InitCode);	break;
	}
	
	/* ET2 OTP status check */
	if (TEST_MODE == TEST_MODE_ET2 || TEST_MODE == TEST_MODE_ET3 || TEST_MODE == TEST_MODE_ESD)
	{	
		OTP_TIMES = OTPTimes_Read();
		printf("\r\nOTP times is %d\r\n", OTP_TIMES);
		if (OTP_TIMES == 0)
		{
			DriverIC_Reset();	
			IC_Init(ET1_InitCode);
			vcom_best = VCOM_Read();	//zxj@20160526 for G6 ET2
			printf("*#*#3:OTP NO#*#*\r\n"); //zxj@20160526 for G6 ET2
			printf("*#*#4:0x%04X#*#*\r\n", vcom_best);//zxj@20160526 for G6 ET2
		}
		else
		{
			vcom_best = VCOM_Read();	//zxj@20160526 for G6 ET2
			printf("*#*#3:OTP YES#*#*\r\n"); //zxj@20160526 for G6 ET2
			printf("*#*#4:0x%04X#*#*\r\n", vcom_best);//zxj@20160526 for G6 ET2
		}
	}
//	if (TEST_MODE != TEST_MODE_RA)
//	{	
//	    LCD_SleepOut();
//	    LCD_VideoMode_ON();
//	    LCD_HSMode();
//	    FPGA_INIT_END_INFO(1);
//	    FPGA_DisPattern(0, 0, 0, 0);
//		  LCD_PWM(0xFF);
//      Delay_ms(500);				
//	}


	/* sleep in current check */
//	if (TEST_MODE == TEST_MODE_ET1)
//	{	
////		SleepCurrent_Check();
//	}
	/* display on */
	LCD_SleepOut();
//#ifdef CMD_MODE
//	LCD_VideoMode_OFF();
//#else
	LCD_VideoMode_ON();
//#endif
	LCD_HSMode();

	/* discharge */
	FPGA_INIT_END_INFO(1);
	FPGA_DisPattern(0, 0, 0, 0); 
	Delay_ms(300);
	LCD_PWM(0xFF);
#ifdef CMD_MODE
	FPGA_DisPattern(0, 1, 1, 1);
#endif
	if (TEST_MODE == TEST_MODE_CTP)
	{	
		TEST_Config_CTP();
	}
}

/*********************************************************************************
* Function: main
* Description: main
* Input: none
* Output: none
* Return: none
* Call: system
*/
int main(void)
{		
	/* STM32 initial */
	Board_Init(); 
	Delay_ms(200);	// wait for FPGA ready!
//	FPGA_INIT_END_INFO(0);
	LED_ON(BLUE);
	GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN);
	GPIO_ResetBits(TEST25_GPIO_PORT, TEST25_PIN);
	if (TEST_MODE != TEST_MODE_OTP)
	{
//		GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
		GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
	}
	
	/* CONNECTOR check */
//	Connect_Check();

	/* Silver resistance check */	
//	Res_Check();
		
	/* FPGA initial */
	printf("\r\nFPGA_Reset...\r\n");
	FPGA_Reset();
	printf("\r\nFPGA_Info_Visible...\r\n");
	FPGA_Info_Visible(INFO_NONE);
	printf("\r\nFPGA_Info_Set...\r\n");
	FPGA_Info_Set((uint8_t *)"");	
	printf("\r\nFPGA_Project_Set...\r\n");
	FPGA_Project_Set((uint8_t *)PROJECT_NO);	
	printf("\r\nFPGA_Porch_Set...\r\n");
	FPGA_Porch_Set(FPGA_porchPara);
	printf("\r\nFPGA_PORT_MAP...\r\n");
	FPGA_PORT_MAP(0);

#ifdef AUTO_LINE
#else
	LCM_Init();
#endif
	GPIO_SetBits(TEST17_GPIO_PORT, TEST17_PIN);
	/* CTP Sensor detect */
	if (TEST_MODE == TEST_MODE_CTP)
	{	
		FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
		FPGA_DisPattern(83, 127, 127, 127);	//waku+black crosstalk
//		CTP_Start();
		Differ2_Detect();	
	}
			
	/* TE detect */
	TE_Detect();
	LED_OFF(BLUE);
	printf("CURRENT SPEC -> sleep: IOVCC_MAX = %.2f; VSP_MAX = %.2f; VSN_MAX = %.2f;\r\n", SPEC_SLEEP_IOVCC ,SPEC_SLEEP_VSP , SPEC_SLEEP_VSN);	
	/* picture loading */ 
	if (PIC_NUM != 0)
	{
	  FPGA_DisPattern(0, 0, 0, 0);	
		printf("\r\n Picture loading...\r\n");			
		PIC_Load_BMP(PIC_NUM);
	}
	
	  FPGA_DisPicture(6);	//picture
		LCD_PWM(0xFF);
		Delay_ms(1000);
		SleepCurrent_Check();
  /* version setting */
	Version_Set();	
	
	/* debug */

	/* Main loop */
	printf("\r\nMain loop...\r\n");
	while (1)
	{		
		switch (TEST_MODE) 
		{
			case (TEST_MODE_ET1):	Test_ET1();		break;
			case (TEST_MODE_ET2):	if (OTP_TIMES == 0) 
									{ 
										FPGA_DisPattern(84, 0, 0, 0);
									}
									else  
									{
										Test_ET2();
									}	
									break;
			case (TEST_MODE_ET3):	if (OTP_TIMES == 0) 
									{ 
										FPGA_DisPattern(84, 0, 0, 0);
									}
									else  
									{
										Test_ET3();
									}			
									break;
			case (TEST_MODE_OTP):	Test_OTP();		break;
			case (TEST_MODE_RA):	Test_RA();		break;
			case (TEST_MODE_ESD):	if (OTP_TIMES == 0) 
									{ 
										FPGA_DisPattern(84, 0, 0, 0);
									}
									else  
									{
										Test_ESD();
									}	break;
			case (TEST_MODE_OD):	Test_OD();		break;
			case (TEST_MODE_DEMO):	Test_DEMO();	break;
			case (TEST_MODE_CTP): Test_CTP(); break;
			case (TEST_MODE_OQC1): Test_OQC1(); break;
			default: 				Test_DEBUG();	break;
		}

//		Current_Check();	
		
		DisState_Switch();
	} 
}
