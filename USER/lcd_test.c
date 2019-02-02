#include "include.h"

FlagStatus Delay_Enable = RESET;
uint16_t Delay_Time = 0;	//uint is 10ms

FlagStatus Auto_Switch = RESET;
uint16_t Auto_Time = AUTO_TIME;

uint16_t delay_cnt;
uint16_t PWM_detect_cnt;
uint16_t PWM_DDIC_LOW;
uint16_t PWM_DDIC_HIGH;
uint8_t PWM_pre, PWM_curr;

float	I_IOVCC;
float I_VSP;
float	I_VSN;
float	I_LEDA;

float SPEC_NORMAL_IOVCC_MAX =	200.0;							//mA
float SPEC_NORMAL_VSP_MAX =	100.0;								//mA
float SPEC_NORMAL_VSN_MAX	=	100.0;								//mA

/*********************************************************************************
* Function: TEST_Config_ON
* Description: test pin configure for power on
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TEST_Config_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	
//	GPIO_InitStructure.GPIO_Pin = TEST23_PIN; //I_IOVCC
//	GPIO_Init(TEST23_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //FOR_LEDA
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST1_PIN; //CTP START(Focal)
	GPIO_Init(TEST1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST18_PIN; //POWER_I2C_SDA
	GPIO_Init(TEST18_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST19_PIN; //POWER_I2C_SCL
	GPIO_Init(TEST19_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TEST20_PIN;
//	GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TEST21_PIN;
//	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TEST22_PIN;
//	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST31_PIN; //solenoid valve 
	GPIO_Init(TEST31_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //PWM_OUTPUT
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST25_PIN; //CONNECTOR1
	GPIO_Init(TEST25_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

//	GPIO_InitStructure.GPIO_Pin = TEST23_PIN; //I_IOVCC
//	GPIO_Init(TEST23_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = TEST21_PIN;
//	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TEST22_PIN;
//	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //CONNECTOR2
//	GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin = TEST4_PIN; //RES_TEST
	GPIO_Init(TEST4_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TE_PIN; //TE_DETECT
	GPIO_Init(TE_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = TEST1_PIN; 
//	GPIO_Init(TEST1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST2_PIN; 
	GPIO_Init(TEST2_GPIO_PORT, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //I_IOVCC
	GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			
	GPIO_InitStructure.GPIO_Pin = TEST20_PIN;
	GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST21_PIN;
	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST22_PIN;
	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
#ifdef PWM_DETECT
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //PWM_DETECT
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
#endif
}

/*********************************************************************************
* Function: TEST_Config_CTP
* Description: test pin configure for CTP test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TEST_Config_CTP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = TEST18_PIN; //Focal p1.4
	GPIO_Init(TEST18_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST19_PIN; //Focal p1.4
	GPIO_Init(TEST19_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST20_PIN; //TP_INT
	GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST21_PIN; //TP_SCL
	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST22_PIN; //TP_SDA
	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //TP_RST
	GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************************
* Function: TEST_Config_OFF
* Description: test pin configure for power off
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TEST_Config_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //PWM_DETECT or PWM_OUTPUT
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST18_PIN; //POWER_I2C_SDA
	GPIO_Init(TEST18_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST19_PIN; //POWER_I2C_SCL
	GPIO_Init(TEST19_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST20_PIN;
	GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST21_PIN;
	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST22_PIN;
	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = TEST23_PIN; //I_IOVCC
//	GPIO_Init(TEST23_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //CONNECTOR2
	GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST25_PIN; //CONNECTOR1
	GPIO_Init(TEST25_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************************
* Function: Current_Check_Config
* Description: test pin configure for current check
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Current_Check_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_InitStructure.GPIO_Pin = IVSP_PIN;	 	
	GPIO_Init(IVSP_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = IVSN_PIN;	 	
	GPIO_Init(IVSN_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = IIOVCC_PIN;	 	
	GPIO_Init(IIOVCC_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = EN_uA_PIN;	 	
	GPIO_Init(EN_uA_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EN_0_ohm_PIN;	 	
	GPIO_Init(EN_0_ohm_GPIO_PORT, &GPIO_InitStructure);
} 

/*********************************************************************************
* Function: Delay_Lock
* Description: display pattern switch delay lock
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Delay_Lock(void)
{
	if (Delay_Enable == SET)
	{
		if (Delay_Time > 0)
		{
			Delay_Time--;
		}
		else
		{
			Delay_Enable = RESET;
		}
	}
}

/*********************************************************************************
* Function: Auto_Switch_Delay
* Description: display pattern auto switch delay
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Auto_Switch_Delay(void)
{
	if (Auto_Switch == RESET)
	{
		if (Auto_Time > 0)
		{
			Auto_Time--;
		}
		else
		{
			Auto_Switch = SET;
		}
	}
}

/*********************************************************************************
* Function: Test_XXX
* Description: test mode switch
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Test_XXX(void)
{	
	switch (TEST_MODE) 
	{
		case (TEST_MODE_DEBUG): Test_DEBUG(); break;
		case (TEST_MODE_ET1): Test_ET1(); break;
		case (TEST_MODE_ET2): Test_ET2(); break;
		case (TEST_MODE_ET3): Test_ET3(); break;
		case (TEST_MODE_OTP): Test_OTP(); break;
		case (TEST_MODE_RA): Test_RA(); break;
		case (TEST_MODE_ESD): Test_ESD(); break;
		case (TEST_MODE_OD): Test_OD(); break;
		case (TEST_MODE_DEMO): Test_DEMO(); break;
		case (TEST_MODE_CTP): Test_CTP(); break;
		default: Test_DEBUG(); break;
	}
}

/*********************************************************************************
* Function: CTP_Start
* Description: CTP start signal
* Input: none
* Output: none
* Return: none
* Call: external
*/
void CTP_Start(void)
{
	uint8_t Reg_0A;
	
	MIPI_DCS_Read(MAIN_PORT, 0x0A, 1, &Reg_0A);
	printf("The register 0x0A of DDIC is: 0x%02X\r\n", Reg_0A);
	
	if (Reg_0A == 0X9C)
	{
		GPIO_ResetBits(TEST1_GPIO_PORT, TEST1_PIN); // Start notice.
		Delay_ms(150);
		GPIO_SetBits(TEST1_GPIO_PORT, TEST1_PIN); // End notice.
		Delay_ms(20);
		printf("Notice test start has been sent out!\r\n");
 }
 else
 {
	 while (1);
 }
}

/*********************************************************************************
* Function: Differ2_Detect
* Description: CTP detect sensot connection
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Differ2_Detect(void)
{
#ifdef DIFFER2_DETECT
	uint8_t P1_4, P1_5, P1_4_d, P1_5_d;
	uint8_t Detect_Status;
 	uint8_t buf[3];
	uint16_t B7_Temp;
	uint8_t P1_4_n = RESET;
	uint8_t P1_5_n = RESET;
	uint8_t rbuf_C589[1];
	uint8_t rbuf_C595[3];

	while (1)
	{
		if (KEY_GetState() == KEY_SLEEP)
		{
			while (KEY_GetState() == KEY_SLEEP);
			return;	
		}

		P1_4 = GPIO_ReadInputDataBit(TEST18_GPIO_PORT, TEST18_PIN);
		P1_5 = GPIO_ReadInputDataBit(TEST19_GPIO_PORT, TEST19_PIN);
		Debug_Printf("\r\nP1_4 is: %d\r\n", P1_4);
		Debug_Printf("P1_5 is: %d\r\n", P1_5);
		Debug_Printf("P1_4_n is: %d\r\n", P1_4_n);
		Debug_Printf("P1_5_n is: %d\r\n", P1_5_n);
		Debug_Printf("Sensor_Detect!\r\n");
		if (P1_4 == P1_4_n && P1_5 == P1_5_n) continue;
		Debug_Printf("\r\nChange OK!\r\n");
		P1_4_d = P1_4;
		P1_5_d = P1_5;
		Delay_ms(10);
		P1_4 = GPIO_ReadInputDataBit(TEST18_GPIO_PORT, TEST18_PIN);
		P1_5 = GPIO_ReadInputDataBit(TEST19_GPIO_PORT, TEST19_PIN);
		if (P1_4 != P1_4_d || P1_5 != P1_5_d) continue;
		Debug_Printf("\r\nJitter OK!\r\n");
		P1_4_n = P1_4;
		P1_5_n = P1_5;
		
		if (P1_4 == SET && P1_5 == SET) Detect_Status = 0;
		else if (P1_4 == RESET && P1_5 == RESET) Detect_Status = 1;
		else if (P1_4 == RESET && P1_5 == SET) Detect_Status = 2;
		else if (P1_4 == SET && P1_5 == RESET) Detect_Status = 3;
		
		switch (Detect_Status)
		{
			case 0:
				printf("\r\n0 status!\r\n");
				break;
			case 1:
				printf("\r\n1 status!\r\n");
				buf[0] = 0x06;
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
				MIPI_GEN_Read(MAIN_PORT, 0xC5, 1, rbuf_C589);
//				B7_Temp = ReadSSDReg(MAIN_PORT, SSD_CFGR); //backup B7 status
//				MIPI_Reset();
//				IC_Init(SSDInitCode);
//				SSD_B7 = B7_Temp;
//				WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7); //reset B7 status
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf); //
				buf[0] = 0x11;
				buf[1] = 0x32;
				buf[2] = 0x50;
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
				MIPI_GEN_Read(MAIN_PORT, 0xC5, 3, rbuf_C595);
//				B7_Temp = ReadSSDReg(MAIN_PORT, SSD_CFGR); //backup B7 status
//				MIPI_Reset();
//				IC_Init(SSDInitCode);
//				SSD_B7 = B7_Temp;
//				WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7); //reset B7 status
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, buf); //
			break;
			case 2:
				printf("\r\n2 status!\r\n");
				buf[0] = 0x05;
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf); //
			break;
			case 3:
				printf("\r\n3 status!\r\n");
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, rbuf_C589); //
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, rbuf_C595); //
			break;
			default:
				printf("\r\nDefault status!\r\n");
			break;
		}
	}
#endif
}

/*********************************************************************************
* Function: TE_Detect
* Description: detect TE signal
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TE_Detect(void)
{
#ifdef TE_DETECT
	 uint32_t dly_cnt = 0;
	 uint16_t TE_detect_cnt = 0;
	 uint8_t TE_pre, TE_curr;
	 
	 TE_curr = GPIO_ReadInputDataBit(TE_GPIO_PORT, TE_PIN);
	 while (dly_cnt < 200000) //10 frame£º16.7ms*10=167ms=167000us
	 {
		 Delay_us(1);
		 dly_cnt++;
		 TE_pre = TE_curr;
		 TE_curr = GPIO_ReadInputDataBit(TE_GPIO_PORT, TE_PIN);
		 if ((TE_pre == RESET) && (TE_curr == SET))
		 {
		  	TE_detect_cnt = TE_detect_cnt + 1;
		 }
	 }
		 
	 if (TE_detect_cnt < 10) //TE abnormal 
	 {
		 printf("TE is abnormal!\r\n");
		 printf("TE_detect_cnt=%d\r\n", TE_detect_cnt);
		 FPGA_Info_Set((uint8_t *)"TE NG");	
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 0, 0, 0); 
		 while (KEY_GetState() == KEY_IDLE);
	   KEY_SLEEPIN();	
	   while (1);
	 }	
	 printf("TE is normal!\r\n");	 
	 printf("TE_detect_cnt=%d\r\n", TE_detect_cnt);
#endif
}

/*********************************************************************************
* Function: ARM_PWM_Detect
* Description: ARM detect PWM singal control
* Input: none
* Output: none
* Return: none
* Call: external
*/
void ARM_PWM_Detect(void)
{
	delay_cnt++;
	PWM_pre = PWM_curr;
	PWM_curr = GPIO_ReadInputDataBit(TEST17_GPIO_PORT, TEST17_PIN);
	if (PWM_curr == PWM_pre)
	{
		PWM_detect_cnt = PWM_detect_cnt + 1;
	}
	else if (PWM_pre == SET)
	{		
		PWM_DDIC_HIGH = PWM_detect_cnt;
		PWM_detect_cnt = 0;
	}
	else
	{
		PWM_DDIC_LOW = PWM_detect_cnt;
		PWM_detect_cnt = 0;
	}
}

/*********************************************************************************
* Function: PWM_Detect
* Description: detect PWM signal
* Input: none
* Output: none
* Return: none
* Call: external
*/
void PWM_Detect(void)
{
#ifdef PWM_DETECT	 
	 float pwm_duty;

	 TIM_Cmd(TIM2, ENABLE);
	 delay_cnt = 0;
	 while (delay_cnt < 10000); //sample rate is 10MHz, detect 10ms
	 TIM_Cmd(TIM2, DISABLE);
	 
	 pwm_duty = (PWM_DDIC_HIGH * 1.0 / (PWM_DDIC_HIGH + PWM_DDIC_LOW) * 100.0);
	 printf("PWM_DDIC_HIGH = %d\r\n", PWM_DDIC_HIGH);
	 printf("PWM_DDIC_LOW = %d\r\n", PWM_DDIC_LOW);
	 printf("PWM_DUTY = %f%%\r\n", pwm_duty);
	 if (pwm_duty < 0.05)
	 {
		 printf("PWM is abnormal!\r\n");
		 FPGA_Info_Set((uint8_t *)"PWM NG");	
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 0, 0, 0); 
		 while (KEY_GetState() == KEY_IDLE);
	   KEY_SLEEPIN();	
	   while (1);
	 }
#endif
}

/*********************************************************************************
* Function: Connect_Check
* Description: FPC connection check
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Connect_Check(void)
{
#ifdef HOT_PLUG
//	if ((GPIO_ReadInputDataBit(TEST24_GPIO_PORT, TEST24_PIN) == SET) || (GPIO_ReadInputDataBit(TEST25_GPIO_PORT, TEST25_PIN) == SET))	 // connector is open
	if ((GPIO_ReadInputDataBit(TEST25_GPIO_PORT, TEST25_PIN) == SET))	 // connector is open	
	{
		LCMPower_OFF();
		printf("\r\nConnector is open!\r\n");
		while(1);
	}
#endif
}

/*********************************************************************************
* Function: Res_Check
* Description: Silver resistance check
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Res_Check(void)
{
#ifdef RES_TEST
	Delay_sec(2);	
	if (GPIO_ReadInputDataBit(TEST4_GPIO_PORT, TEST4_PIN) == SET)
	{
		printf("\r\nSilver resistance is abnormal!\r\n");
		while(1);
	}
#endif
}

/*********************************************************************************
* Function: Current_Measure
* Description: current check while normal display
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Current_Check(void)
{
#ifdef CURRENT_CHECK
	if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET) 
		|| (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET) 
	|| (GPIO_ReadInputDataBit(IIOVCC_GPIO_PORT, IIOVCC_PIN) == RESET))
	{
		Delay_ms(1);
		if ((GPIO_ReadInputDataBit(IIOVCC_GPIO_PORT, IIOVCC_PIN) == RESET))
		{
				FPGA_DisPattern(123, 0, 0, 0); 
		}
		else if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET))
		{
				FPGA_DisPattern(99, 0, 0, 0); 
		}
		else if (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET)
		{
				FPGA_DisPattern(98, 0, 0, 0); 
		}
		else
		{
			return;
		}
	
		printf("\r\nCurrent is abnormal!\r\n");
		while (KEY_GetState() == KEY_IDLE);
	  KEY_SLEEPIN();
		while (1);
	}
#endif	
	
#ifdef CURRENT_METER	
	///////////////////////////////////////20160628///////////////////////////////
	Meas_Current_Normal();
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0)
	{
//		FPGA_DisPattern(114, 0, 0, 0);
//		FPGA_Info_Set((uint8_t *)"METER NO ACK");
//		FPGA_Info_Visible(INFO_STR);
	Current_Upload();	
	SPEC_NORMAL_IOVCC_MAX =	200.0;
	SPEC_NORMAL_VSP_MAX =	100.0;
	SPEC_NORMAL_VSN_MAX	=	100.0;	
	printf("\r\nCurrent is abnormal!\r\n");
	ScanForward();
	FPGA_DisPicture(2);
	Delay_ms(4000);
  KEY_SLEEPIN();
	}
	else if (I_IOVCC < SPEC_NORMAL_IOVCC_MIN || I_IOVCC > SPEC_NORMAL_IOVCC_MAX)
	{
//		FPGA_DisPattern(123, 0, 0, 0); 
	Current_Upload();	
	SPEC_NORMAL_IOVCC_MAX =	200.0;
	SPEC_NORMAL_VSP_MAX =	100.0;
	SPEC_NORMAL_VSN_MAX	=	100.0;	
	printf("\r\nCurrent is abnormal!\r\n");
	ScanForward();
	FPGA_DisPicture(3);
	Delay_ms(4000);
  KEY_SLEEPIN();
	}
	else if (I_VSP < SPEC_NORMAL_VSP_MIN || I_VSP > SPEC_NORMAL_VSP_MAX)
	{
//		FPGA_DisPattern(99, 0, 0, 0); 
	Current_Upload();	
	SPEC_NORMAL_IOVCC_MAX =	200.0;
	SPEC_NORMAL_VSP_MAX =	100.0;
	SPEC_NORMAL_VSN_MAX	=	100.0;	
	printf("\r\nCurrent is abnormal!\r\n");
	ScanForward();
	FPGA_DisPicture(4);
	Delay_ms(4000);
  KEY_SLEEPIN();
	}
	else if (I_VSN < SPEC_NORMAL_VSN_MIN || I_VSN > SPEC_NORMAL_VSN_MAX)
	{
//		FPGA_DisPattern(98, 0, 0, 0); 
	Current_Upload();	
	SPEC_NORMAL_IOVCC_MAX =	200.0;
	SPEC_NORMAL_VSP_MAX =	100.0;
	SPEC_NORMAL_VSN_MAX	=	100.0;	
	printf("\r\nCurrent is abnormal!\r\n");
	ScanForward();
	FPGA_DisPicture(5);
	Delay_ms(4000);
  KEY_SLEEPIN();
	}
	else
	{
		Current_Upload();
		SPEC_NORMAL_IOVCC_MAX =	200.0;
		SPEC_NORMAL_VSP_MAX =	100.0;
		SPEC_NORMAL_VSN_MAX	=	100.0;	
		return;
	}
	
//	Current_Upload();	
//	SPEC_NORMAL_IOVCC_MAX =	200.0;
//	SPEC_NORMAL_VSP_MAX =	100.0;
//	SPEC_NORMAL_VSN_MAX	=	100.0;	
//	printf("\r\nCurrent is abnormal!\r\n");
//	while (KEY_GetState() == KEY_IDLE);
//	KEY_SLEEPIN();
//	while (1);

	///////////////////////////////////////20160628///////////////////////////////
#endif	 
}


/*********************************************************************************
* Function: SleepCurrent_Check
* Description: current check while sleep in
* Input: none
* Output: none
* Return: none
* Call: external
*/
void SleepCurrent_Check(void)
{
	FlagStatus current_NG = RESET;
	FlagStatus current_NG_I = RESET;
	FlagStatus current_NG_P = RESET;
	FlagStatus current_NG_N = RESET;

	uint8_t Num_Temp;
	
#ifdef CURRENT_CHECK	
	LCD_SleepIn();
	LCD_VideoMode_OFF();
	LCD_LPMode();
	MIPI_SleepMode_ON();
 
	GPIO_ResetBits(EN_uA_GPIO_PORT, EN_uA_PIN); 
	Delay_ms(50);
	GPIO_SetBits(EN_0_ohm_GPIO_PORT, EN_0_ohm_PIN);
	Delay_ms(500); //wait for relay switch to complete
	 
	if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET) 
		|| (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET) 
	|| (GPIO_ReadInputDataBit(IIOVCC_GPIO_PORT, IIOVCC_PIN) == RESET))
	{
		Delay_ms(1);
		if ((GPIO_ReadInputDataBit(IIOVCC_GPIO_PORT, IIOVCC_PIN) == RESET))
		{
			FPGA_DisPattern(123, 0, 0, 0); 
			current_NG = SET;
			printf("\r\nIOVCC Sleep Current is abnormal!\r\n");
		}
		else if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET))
		{
			FPGA_DisPattern(99, 0, 0, 0); 
			current_NG = SET;
			printf("\r\nVSP Sleep Current is abnormal!\r\n");
		}
		else if (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET)
		{
			FPGA_DisPattern(98, 0, 0, 0); 
			current_NG = SET;
			printf("\r\nVSN Sleep Current is abnormal!\r\n");
		}
		else
		{
			FPGA_DisPattern(0, 0, 0, 0); 
			current_NG = RESET;
			printf("\r\nSleep Current is normal!\r\n");	
		}
	}

	GPIO_ResetBits(EN_0_ohm_GPIO_PORT, EN_0_ohm_PIN);
	Delay_ms(50);
	GPIO_SetBits(EN_uA_GPIO_PORT, EN_uA_PIN);;
	Delay_ms(50);
	MIPI_SleepMode_OFF();
	LCD_SleepOut();
#ifdef CMD_MODE
	LCD_VideoMode_OFF();
#else
	LCD_VideoMode_ON();
#endif
	LCD_HSMode();
	FPGA_INIT_END_INFO(1);
	LCD_PWM(0x00);
	
	if (current_NG == SET)	
	{		
		while (KEY_GetState() == KEY_IDLE);
	  KEY_SLEEPIN();
		while (1);
	}
#endif
	
#ifdef CURRENT_METER		
	///////////////////////////////////////20160628///////////////////////////////	 
	LCD_SleepIn();
	LCD_VideoMode_OFF();
	LCD_LPMode();
	MIPI_SleepMode_ON();
	Delay_ms(500);

	Meas_Current_Sleep();
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0)
	{
//		FPGA_DisPattern(114, 0, 0, 0);
//		FPGA_Info_Set((uint8_t *)"METER NO ACK");
//		FPGA_Info_Visible(INFO_STR);
		current_NG = SET;
	}
	else if (I_IOVCC > SPEC_SLEEP_IOVCC)
	{
//		FPGA_DisPattern(123, 0, 0, 0); 

		current_NG_I = SET;
	}
	else if (I_VSP > SPEC_SLEEP_VSP)
	{
//		FPGA_DisPattern(99, 0, 0, 0); 

		current_NG_P = SET;
	}
	else if (I_VSN > SPEC_SLEEP_VSN)
	{
//		FPGA_DisPattern(98, 0, 0, 0); 

		current_NG_N = SET;
	}
	else
	{
		FPGA_DisPattern(0, 0, 0, 0); 
		current_NG = RESET;
		printf("\r\nSleep Current is normal!\r\n");	
	}

//	MIPI_SleepMode_OFF();
//	LCD_SleepOut();
////#ifdef CMD_MODE
////	LCD_VideoMode_OFF();
////#else
//	LCD_VideoMode_ON();
////#endif
//	LCD_HSMode();
//	FPGA_INIT_END_INFO(1);
//	LCD_PWM(0xFF);
		MIPI_Reset();
		IC_Init(SSDInitCode);	
		GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	  Delay_ms(10);
		DriverIC_Reset();
		IC_Init(ET1_InitCode);
		LCD_SleepOut();
		LCD_VideoMode_ON();
		LCD_HSMode();
		FPGA_INIT_END_INFO(1);
	  LCD_PWM(0xFF);
		
	Num_Temp = DIS_NUM;
	DIS_NUM = 255;
	Current_Upload();
	DIS_NUM = Num_Temp;
	//
	if (current_NG == SET)	
	{ 
		FPGA_DisPicture(2);
		printf("\r\nThere is no ack from current meter, please check connection!\r\n");
		Delay_ms(4000);
    KEY_SLEEPIN();

	}
	else if (current_NG_I == SET)	
	{
    ScanBackward();
		FPGA_DisPicture(3);
		printf("\r\nIOVCC Sleep Current is abnormal!\r\n");
		Delay_ms(4000);
    KEY_SLEEPIN();
	}
	else if (current_NG_P == SET)	
	{
		ScanBackward();
		FPGA_DisPicture(4);
		printf("\r\nVSP Sleep Current is abnormal!\r\n");
		Delay_ms(4000);
    KEY_SLEEPIN();
	}
	else if (current_NG_N == SET)	
	{
		ScanBackward();
		FPGA_DisPicture(5);
		printf("\r\nVSN Sleep Current is abnormal!\r\n");
		Delay_ms(4000);
    KEY_SLEEPIN();
	}

	///////


	
//	if (current_NG == SET)	
//	{		
//		while (KEY_GetState() == KEY_IDLE);
//	  KEY_SLEEPIN();
//		while (1);

//	}
	///////////////////////////////////////20160628///////////////////////////////
#endif
}

/*********************************************************************************
* Function: Meas_Current_Normal
* Description: measure normal display current
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_Current_Normal(void)
{
	char *Temp_IOVCC, *Temp_VSP, *Temp_VSN, *Temp_LEDA;
	uint8_t Try_Count = 2;
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)"#NORMAL\r\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] != '#' || m >= 1000)	//recieve data is not '#', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}

	Temp_IOVCC = strtok(&Admesy_RData[1] , " ");
	Temp_VSP = strtok(NULL , " ");
	Temp_VSN = strtok(NULL , " ");
	Temp_LEDA = strtok(NULL , " ");
	I_IOVCC = atof(Temp_IOVCC);
	I_VSP = atof(Temp_VSP);
	I_VSN = atof(Temp_VSN);
	I_LEDA = atof(Temp_LEDA);
	Debug_Printf("Try_Count = %d", Try_Count);
}

/*********************************************************************************
* Function: Meas_Current_Sleep
* Description: measure sleep in current
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_Current_Sleep(void)
{
	char *Temp_IOVCC, *Temp_VSP, *Temp_VSN, *Temp_LEDA;
	uint8_t Try_Count = 2;
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
//		USART_printf(MSE_COM, (uint8_t *)"#SLEEP\r\n");
//		USART_printf(MSE_COM, (uint8_t *)"#SLEEP\r\n");
			USART_printf(MSE_COM, (uint8_t *)"\r\n#NORMAL\r\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] != '#' || m >= 1000)	//recieve data is not '#', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}

	Temp_IOVCC = strtok(&Admesy_RData[1] , " ");
	Temp_VSP = strtok(NULL , " ");
	Temp_VSN = strtok(NULL , " ");
	Temp_LEDA = strtok(NULL , " ");
	I_IOVCC = atof(Temp_IOVCC);
	I_VSP = atof(Temp_VSP);
	I_VSN = atof(Temp_VSN);
	I_LEDA = atof(Temp_LEDA);
	Debug_Printf("Try_Count = %d", Try_Count);
}

/*********************************************************************************
* Function: Current_Upload
* Description: upload current information to PC
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Current_Upload(void)
{
#ifdef CURRENT_METER	
	char Str_Temp[64];
	char File_Name[32];
	
	sprintf(File_Name, "%s.xls", PROJECT_NO);
	sprintf(Str_Temp, "%d\t%f\t%f\t%f\t%f\r\n", DIS_NUM, I_IOVCC, I_VSP, I_VSN, I_LEDA);
	if (SD_Write_Str2File(File_Name, Str_Temp) == ERROR)
	{
		FPGA_DisPattern(114, 0, 0, 0);
		FPGA_Info_Visible(INFO_STR);
		FPGA_Info_Set((uint8_t *)"SDCARD ERROR");
		printf("SDCARD is not available, or SDCARD maybe overflow!\r\n");
		while (KEY_GetState() == KEY_IDLE);
	  KEY_SLEEPIN();
		while (1);
	}
	
	printf("\r\n*#*#2:%d IOVCC %f ", DIS_NUM, I_IOVCC);	 
	printf("VSP %f ", I_VSP);
	printf("VSN %f ", I_VSN);
	printf("LEDA %f#*#*\r\n\r\n", I_LEDA);
#endif
}

/*********************************************************************************
* Function: ESD_Fail_Recover
* Description: Lit on the LCM once detected ESD fail
* Input: none
* Output: none
* Return: none
* Call: external
*/
void ESD_Fail_Recover(void)
{
	uint8_t Reg_0A;
	
	MIPI_DCS_Read(MAIN_PORT, 0x0A, 1, &Reg_0A);
	printf("The register 0x0A of DDIC is: 0x%02X\r\n", Reg_0A);
	
	if (Reg_0A == 0X1C || Reg_0A == 0X9C )//SYNA£ºDSC R0A == 1C,other 0x9C,normal! else need to HW reset!
	{
		printf("\r\nThe display is normal.\r\n");
	}
	else
	{
		printf("\r\nThe display is abnormal.R0A = %x\r\n",Reg_0A);
		printf("Recovering...\r\n");
		
		MIPI_Reset();
		IC_Init(SSDInitCode);	
		DriverIC_Reset();
		Delay_ms(20);
		DriverIC_Reset();
		IC_Init(ET2_InitCode);
		LCD_SleepOut();
#ifdef CMD_MODE
		LCD_VideoMode_OFF();
#else
		LCD_VideoMode_ON();
#endif
		LCD_HSMode();
		LCD_PWM(0xFF);		
	}
}

/*********************************************************************************
* Function: ON3sec_OFF3sec
* Description: 3sec ON and 3sec OFF, measure 64gray flicker
* Input: none
* Output: none
* Return: none
* Call: external
*/
void ON3sec_OFF3sec(void)
{
	FlagStatus Test_Status = RESET;
	uint16_t dlyCnt = 0;

	FPGA_Info_Visible(INFO_NONE);
	FPGA_DisPattern(0, 64, 64, 64);

	while (KEY_GetState() != KEY_TEST);
	Test_Status = SET;
	FPGA_DisPattern(0, 255, 255, 255);
	while (Test_Status == SET)
	{		 
		while (dlyCnt < 30)
		{
		 	Delay_ms(50);
		 	dlyCnt++;
		 	if (KEY_GetState() == KEY_TEST)	
			{
				Test_Status	 = RESET;
			}
		}
		if (Test_Status	== RESET) break;
		dlyCnt = 0;

		POWER_LED_Set();
		FPGA_DisPattern(0, 0, 0, 0); 
		Delay_ms(30);
		LCD_LPMode();
		LCD_VideoMode_OFF();
		LCD_SleepIn();	
		LCMPower_OFF();

		while (dlyCnt < 26)
		{
		 	Delay_ms(50);
		 	dlyCnt++;
		 	if (KEY_GetState() == KEY_TEST)	
			{
				Test_Status	 = RESET;
			}
		}
		if (Test_Status	== RESET) break;
		dlyCnt = 0;

		FPGA_DisPattern(0, 255, 255, 255);
		LCMPower_ON();
		DriverIC_Reset();
		IC_Init(ET2_InitCode);
		LCD_SleepOut();
		LCD_VideoMode_ON();	
		LCD_HSMode();
		POWER_LED_Reset();		
	}

	FPGA_DisPattern(0, 64, 64, 64);
	while (1);          
}
