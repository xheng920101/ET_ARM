#include "include.h"

FlagStatus Flag_Test_Current = SET;
FlagStatus current_NG = RESET;
FlagStatus SDCard_NG = RESET;
FlagStatus TE_NG = RESET;
FlagStatus PWM_NG = RESET;
FlagStatus ID_NG = RESET;
FlagStatus FW_NG = RESET;
FlagStatus FPGA_NG = RESET;
FlagStatus OSC_TRIM_NG = RESET;

uint16_t delay_cnt;
uint16_t PWM_detect_cnt;
uint16_t PWM_DDIC_LOW;
uint16_t PWM_DDIC_HIGH;
uint8_t PWM_pre, PWM_curr;

float	I_IOVCC;
float I_VSP;
float	I_VSN;
float	I_LEDA;

float SPEC_MAX_IOVCC =	200.0;							//mA
float SPEC_MAX_VSP =	100.0;								//mA
float SPEC_MAX_VSN	=	100.0;								//mA

float SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;			  //mA
float SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;				//mA

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

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = CTP_START_PIN;  //CTP START
	GPIO_Init(CTP_START_GPIO_PORT, &GPIO_InitStructure);
#ifdef DIFFER2_DETECT
	GPIO_SetBits(CTP_START_GPIO_PORT, CTP_START_PIN);
#else
	GPIO_ResetBits(CTP_START_GPIO_PORT, CTP_START_PIN);
#endif
	
	GPIO_InitStructure.GPIO_Pin = CTP_ACK_PIN;  //CTP ACK
	GPIO_Init(CTP_ACK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //PWM_OUTPUT
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST18_PIN; //POWER_I2C_SDA
	GPIO_Init(TEST18_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST19_PIN; //POWER_I2C_SCL
	GPIO_Init(TEST19_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TEST31_PIN; //solenoid valve 
	GPIO_Init(TEST31_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new    2017-1-6 12:17:25 change for G6 demond
//	GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //old  2017-1-6 12:17:25 change for G6 demond

	//2017-3-3 modify for TED current test, pull high will casue leakage
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	

	GPIO_InitStructure.GPIO_Pin = TEST20_PIN;	//TP_INT
	GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TEST21_PIN; //TP_SLC
	GPIO_Init(TEST21_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST22_PIN; //TP_SDA
	GPIO_Init(TEST22_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //CONNECTOR2 / TP_RST
	GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       

	GPIO_InitStructure.GPIO_Pin = TEST23_PIN; //I_IOVCC
	GPIO_Init(TEST23_GPIO_PORT, &GPIO_InitStructure);     
	
	GPIO_InitStructure.GPIO_Pin = TEST25_PIN; //CONNECTOR1
	GPIO_Init(TEST25_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TEST15_PIN; //CTP2
	GPIO_Init(TEST15_GPIO_PORT, &GPIO_InitStructure);	
	
#ifdef PWM_DETECT
	GPIO_InitStructure.GPIO_Pin = TEST17_PIN; //PWM_DETECT
	GPIO_Init(TEST17_GPIO_PORT, &GPIO_InitStructure);
#endif

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
		
	GPIO_InitStructure.GPIO_Pin = TE_PIN; //TE_DETECT
	GPIO_Init(TE_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST26_PIN; //AUTO SET
	GPIO_Init(TEST26_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************************
* Function: TEST_Config_CTP
* Description: test pin (on connector 60pin2panel) configure for CTP test 
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

	GPIO_InitStructure.GPIO_Pin = TEST15_PIN; //CTP2
	GPIO_Init(TEST15_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST18_PIN; //CTP1
	GPIO_Init(TEST18_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEST19_PIN; //CTP0
	GPIO_Init(TEST19_GPIO_PORT, &GPIO_InitStructure);
	
	//2017-3-3 modify for TED current test, pull high will casue leakage
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
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
* Description: test pin (on connector 60pin2panel) configure for power off  
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
* Function: CTP_Start
* Description: CTP start signal
* Input: none
* Output: none
* Return: none
* Call: external
*/
void CTP_Start(void)
{
//	uint8_t Reg_0A;
//	
//	MIPI_DCS_Read(MAIN_PORT, 0x0A, 1, &Reg_0A);
//	printf("The register 0x0A of DDIC is: 0x%02X\r\n", Reg_0A);
//	
//	if (Reg_0A == 0X9C)
//	{
//		GPIO_SetBits(CTP_START_GPIO_PORT, CTP_START_PIN);
//		Delay_ms(850); 
		GPIO_ResetBits(CTP_START_GPIO_PORT, CTP_START_PIN); 
		Delay_ms(850); //low active for focal CTP start
		GPIO_SetBits(CTP_START_GPIO_PORT, CTP_START_PIN); //positive edge for syna/novatek/himax CTP start
		printf("Notice test start has been sent out!\r\n");
// }
// else
// {
//	 ;
// }
}

/*********************************************************************************
* Function: Differ2_Detect
* Description: CTP detect sensot connection
* Input: none
* Output: none
* Return: none
* Call: external 
*/
void Differ2_Detect(void)//notice add test15m3 as ctp differ 2 read pin (J19 pin2)2017-2-28 
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
	
	while(1)
	{
		if (KEY_GetState() == KEY_SLEEP)
		{
			while (KEY_GetState() == KEY_SLEEP);
			return;	
		}

		P1_4 = GPIO_ReadInputDataBit(TEST18_GPIO_PORT, TEST18_PIN);
		P1_5 = GPIO_ReadInputDataBit(TEST19_GPIO_PORT, TEST19_PIN);
		printf("\r\nP1_4 is: %d\r\n", P1_4);
		printf("P1_5 is: %d\r\n", P1_5);
		printf("P1_4_n is: %d\r\n", P1_4_n);
		printf("P1_5_n is: %d\r\n", P1_5_n);
		printf("Sensor_Detect!\r\n");
		if (P1_4 == P1_4_n && P1_5 == P1_5_n)//machine status
			continue;
		printf("\r\nChange OK!\r\n");
		P1_4_d = P1_4;
		P1_5_d = P1_5;
		Delay_ms(10);
		P1_4 = GPIO_ReadInputDataBit(TEST18_GPIO_PORT, TEST18_PIN);
		P1_5 = GPIO_ReadInputDataBit(TEST19_GPIO_PORT, TEST19_PIN);
		if (P1_4 != P1_4_d || P1_5 != P1_5_d) 
			continue;
		printf("\r\nJitter OK!\r\n");
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
	 while (dly_cnt < 200000) //10 frame：16.7ms*10=167ms=167000us
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
		 TE_NG = SET;
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
		 PWM_NG = SET;
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
	if (auto_line) //2017-1-16
	{
	//NOP, auto line will not check the connector.
	}
  else
	{
	//	if ((GPIO_ReadInputDataBit(TEST24_GPIO_PORT, TEST24_PIN) == SET) || (GPIO_ReadInputDataBit(TEST25_GPIO_PORT, TEST25_PIN) == SET))	 // connector is open
		if ((GPIO_ReadInputDataBit(TEST25_GPIO_PORT, TEST25_PIN) == SET))	 // connector is open	
		{
			Delay_ms(20);
			if (GPIO_ReadInputDataBit(TEST25_GPIO_PORT, TEST25_PIN) == SET)
			{			
				LCMPower_OFF();
				printf("\r\nConnector is open!!!\r\n");
				while (1);
			}				
		}
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
	float resistor = 0;
	
	resistor = GetResValue();
	printf("Detect the AG resistor is %f !", resistor);
	if (auto_line)
	{
		if (resistor > 10000 || resistor < 0.5)	//modify base on test spec.
		{
			
		}
		else
		{
			
		}
	}
	else
	{
		if (resistor > 10000 || resistor < 0.5)	//modify base on test spec.
		{
			FPGA_DisPattern(114, 0, 0, 0);	//display NG
			while (1)
			{
				LED_ON(RED);
				Delay_ms(200);
				LED_OFF(RED);
				Delay_ms(200);
			}
		}
		else
		{
			while (KEY_GetState() != KEY_UP);	//display res value before new key in
			FPGA_DisPattern(139, 0, 0, 0);	//display "开盖"
			while (GetResValue() < 10000);	//modify base on test spec.
		}
	}
}

/*********************************************************************************
 * 函数名：IDCheck
 * 描述  ：Check panel ID
 * 输入  ：无 
 * 输出  ：无
 * 返回  : 无
 * 调用  ：外部调用
 */
void IDCheck(void)
{
	//change your ID check code according to your project
//	unsigned char rdBuf[1];	
//	MIPI_GEN_Read(MAIN_PORT, 0xDA, 1, rdBuf);	 
//	if ((rdBuf[0] & 0x00FF) == 0xD2) //"TL052VVXS04-00"
//	{
//		ID_NG = RESET; //panel ID is in accordance with the current project
//	}
//	else if ((rdBuf[0] & 0x00FF) == 0xE1)
//	{
//		ID_NG = SET;
//		FPGA_Info_Set((uint8_t *)"TL052VVXS05-00");	//the correct project number of the panel				
//	}
//	else if ((rdBuf[0] & 0x00FF) == 0x96)
//	{
//		ID_NG = SET;
//		FPGA_Info_Set((uint8_t *)"TL052VVXS07-00");	//the correct project number of the panel						
//	}		
//	else
//	{
//		ID_NG = SET;
//		FPGA_Info_Set((uint8_t *)"TL052VVXS06-00");	//the correct project number of the panel		 			
//	}
unsigned char rdBuf[4];
  ID_NG = RESET;
	rdBuf[0] = 0x04;
  MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, rdBuf); 
	Delay_ms(10);

	MIPI_DCS_Read(MAIN_PORT, 0xDA, 1, rdBuf);	 
	if ((rdBuf[0] & 0x00FF) != 0x0A) 
	{
		ID_NG = SET; 
	}
	
	if (ID_NG == SET)	printf("*#*#ID NG#*#*\r\n");
	else	printf("*#*#ID OK#*#*\r\n");
}

/*********************************************************************************
* Function: AOI_Current_Check_Normal
* Description: current check while AOI test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void AOI_Current_Check_Normal()
{
	if (auto_line)
	{
#ifdef CURRENT_METER			
		SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;
		SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;	
		
		Meas_Current_Normal();
		if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
		{
			printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		}
		else if (I_IOVCC < SPEC_MIN_IOVCC || I_IOVCC > SPEC_MAX_IOVCC)
		{
			printf("\r\n*#*#CURRENT_NG:IOVCC!#*#*\r\n");
		}
		else if (I_VSP < SPEC_MIN_VSP || I_VSP > SPEC_MAX_VSP)
		{
			printf("\r\n*#*#CURRENT_NG:VSP!#*#*\r\n");
		}
		else if (I_VSN < SPEC_MIN_VSN || I_VSN > SPEC_MAX_VSN)
		{
			printf("\r\n*#*#CURRENT_NG:VSN!#*#*\r\n");
		}
		else if ((TEST_MODE != TEST_MODE_ET1 && TEST_MODE != TEST_MODE_CTP) && (I_LEDA < SPEC_LEDA_MIN || I_LEDA > SPEC_LEDA_MAX))
		{
			printf("\r\n*#*#CURRENT_NG:LEDA!#*#*\r\n");
		}
		else//current ok
		{
			printf("\r\n*#*#CURRENT_OK#*#*\r\n");			
		}
		Current_Upload();
#endif
	}
}

/*********************************************************************************
* Function: AOI_Current_Check_Sleep
* Description: current check while AOI test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void AOI_Current_Check_Sleep()
{
	if (auto_line)
	{
#ifdef CURRENT_METER
		Meas_Current_Sleep();
		if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
		{
			printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		}
		else if (I_IOVCC > SPEC_SLEEP_IOVCC)
		{
			printf("\r\n*#*#CURRENT_NG:IOVCC!#*#*\r\n");
		}
		else if (I_VSP > SPEC_SLEEP_VSP)
		{
			printf("\r\n*#*#CURRENT_NG:VSP!#*#*\r\n");
		}
		else if (I_VSN > SPEC_SLEEP_VSN)
		{
			printf("\r\n*#*#CURRENT_NG:VSN!#*#*\r\n");
		}
		else
		{
			printf("\r\n*#*#CURRENT_OK#*#*\r\n");	
		}
		DIS_NUM = 255;
		Current_Upload();
		DIS_NUM = 0;
#endif
	}
}

/*********************************************************************************
* Function: AOI_Current_Check
* Description: current check while AOI test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void AOI_Current_Check(void)
{
#ifdef CURRENT_METER
	/* normal pattern current check */
	FPGA_DisPattern(0, 255, 0, 0); //red
	Meas_Current_Normal();
{	
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		return;
	}	
	if ((I_IOVCC < SPEC_MIN_IOVCC || I_IOVCC > SPEC_MAX_IOVCC)
	 || (I_VSP < SPEC_MIN_VSP || I_VSP > SPEC_MAX_VSP)
	 || (I_VSN < SPEC_MIN_VSN || I_VSN > SPEC_MAX_VSN)
	 || (I_LEDA < SPEC_MIN_LEDA_NORMAL || I_LEDA > SPEC_MAX_LEDA_NORMAL))
	{
		printf("\r\n*#*#CURRENT_NG#*#*\r\n");
		return;
	}
}	
	FPGA_DisPattern(0, 255, 255, 255); //white
	Meas_Current_Normal();
{	
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		return;
	}	
	if ((I_IOVCC < SPEC_MIN_IOVCC || I_IOVCC > SPEC_MAX_IOVCC)
	 || (I_VSP < SPEC_MIN_VSP || I_VSP > SPEC_MAX_VSP)
	 || (I_VSN < SPEC_MIN_VSN || I_VSN > SPEC_MAX_VSN)
	 || (I_LEDA < SPEC_MIN_LEDA_NORMAL || I_LEDA > SPEC_MAX_LEDA_NORMAL))
	{
		printf("\r\n*#*#CURRENT_NG#*#*\r\n");
		return;
	}
}	
	
	/* low current white check */
	LEDA_DIM();
	FPGA_DisPattern(0, 255, 255, 255);  							
	Delay_ms(100);	//should wait until BLU stable
	Meas_Current_Normal();	
{	
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		return;
	}	
	if (I_LEDA < SPEC_MIN_LEDA_DIM || I_LEDA > SPEC_MAX_LEDA_DIM) 
	{
		printf("\r\n*#*#CURRENT_NG#*#*\r\n");
		return;
	}
}
	
	/* sleep current check */
	LCD_PWM(0x0000); 	
	LCD_SleepIn();
	LCD_VideoMode_OFF();
	LCD_LPMode();
	MIPI_SleepMode_ON();	
	Meas_Current_Sleep();
{	
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
		return;
	}	
	if ((I_IOVCC > SPEC_SLEEP_IOVCC)
	 || (I_VSP > SPEC_SLEEP_VSP)
	 || (I_VSN > SPEC_SLEEP_VSN))
	{
		printf("\r\n*#*#CURRENT_NG#*#*\r\n");
		return;
	}
	MIPI_SleepMode_OFF();
	LCD_LitSquence();
}	
	
	/* current check ok */
	printf("\r\n*#*#CURRENT_OK#*#*\r\n");
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
			FPGA_Info_Set((uint8_t *)"DISPLAY");
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(123, 0, 0, 0); 
		}
		else if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET))
		{
			FPGA_Info_Set((uint8_t *)"DISPLAY");
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(99, 0, 0, 0); 
		}
		else if (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET)
		{
			FPGA_Info_Set((uint8_t *)"DISPLAY");
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(98, 0, 0, 0); 
		}
		else
		{
			return;
		}
	}
	
	printf("\r\nCurrent is abnormal!\r\n");
	current_NG = SET;	
#endif	
	
#ifdef CURRENT_METER	
	///////////////////////////////////////20160628///////////////////////////////
	Meas_Current_Normal();
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		FPGA_Info_Set((uint8_t *)"METER NO ACK");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(114, 0, 0, 0);
		printf("\r\n*#*#CURRENT_NG:METER NO ACK!#*#*\r\n");
	}
	else if (I_IOVCC < SPEC_MIN_IOVCC || I_IOVCC > SPEC_MAX_IOVCC)
	{
		FPGA_Info_Set((uint8_t *)"DISPLAY");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(123, 0, 0, 0); 
		printf("\r\n*#*#CURRENT_NG:IOVCC!#*#*\r\n");
	}
	else if (I_VSP < SPEC_MIN_VSP || I_VSP > SPEC_MAX_VSP)
	{
		FPGA_Info_Set((uint8_t *)"DISPLAY");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(99, 0, 0, 0); 
		printf("\r\n*#*#CURRENT_NG:VSP!#*#*\r\n");
	}
	else if (I_VSN < SPEC_MIN_VSN || I_VSN > SPEC_MAX_VSN)
	{ 
		FPGA_Info_Set((uint8_t *)"DISPLAY");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(98, 0, 0, 0);
		printf("\r\n*#*#CURRENT_NG:VSN!#*#*\r\n");
	}
	else if (TEST_MODE != TEST_MODE_ET1 && (I_LEDA < SPEC_LEDA_MIN || I_LEDA > SPEC_LEDA_MAX))
	{
		FPGA_Info_Set((uint8_t *)"I_LEDA");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(114, 0, 0, 0);//BL Error ,display NG pattern  
		printf("\r\n*#*#CURRENT_NG:LEDA!#*#*\r\n");		
	}
	else
	{
		printf("\r\n*#*#CURRENT_OK#*#*\r\n");
		Current_Upload();
		SPEC_MAX_IOVCC =	SPEC_MAX_RED_IOVCC;
		SPEC_MAX_VSP =	SPEC_MAX_RED_VSP;
		SPEC_MAX_VSN	=	SPEC_MAX_RED_VSN;		
		SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;
		SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;
		return;
	}
	
	Current_Upload();	
	SPEC_MAX_IOVCC =	SPEC_MAX_RED_IOVCC;
	SPEC_MAX_VSP =	SPEC_MAX_RED_VSP;
	SPEC_MAX_VSN	=	SPEC_MAX_RED_VSN;
	SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;
	SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;	
	
	printf("\r\nCurrent is abnormal!\r\n");
	current_NG = SET;	
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
#ifdef CURRENT_METER	
	uint8_t Num_Temp;
#endif
	
#ifdef CURRENT_CHECK	
	LCD_PWM(0x0000); 
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
			FPGA_Info_Set((uint8_t *)"SLEEP");
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(123, 0, 0, 0); 
			current_NG = SET;
			printf("\r\nIOVCC Sleep Current is abnormal!\r\n");
		}
		else if ((GPIO_ReadInputDataBit(IVSP_GPIO_PORT, IVSP_PIN) == RESET))
		{
			FPGA_Info_Set((uint8_t *)"SLEEP");
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(99, 0, 0, 0);
			current_NG = SET;			
			printf("\r\nVSP Sleep Current is abnormal!\r\n");
		}
		else if (GPIO_ReadInputDataBit(IVSN_GPIO_PORT, IVSN_PIN) == RESET)
		{
			FPGA_Info_Set((uint8_t *)"SLEEP");
			FPGA_Info_Visible(INFO_STR);
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
	LCD_LitSquence();
#endif
	
#ifdef CURRENT_METER		
	///////////////////////////////////////20160628///////////////////////////////
	LCD_PWM(0x0000); 	
	LCD_SleepIn();
	LCD_VideoMode_OFF();
	LCD_LPMode();
	MIPI_SleepMode_ON();
	
	Meas_Current_Sleep();
	if (I_IOVCC == 0.0 && I_VSP == 0.0 && I_VSN == 0.0 && I_LEDA == 0.0)
	{
		FPGA_Info_Set((uint8_t *)"METER NO ACK");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(114, 0, 0, 0);
		current_NG = SET;
		printf("\r\nThere is no ack from current meter, please check connection!\r\n");
	}
	else if (I_IOVCC > SPEC_SLEEP_IOVCC)
	{
		FPGA_Info_Set((uint8_t *)"SLEEP");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(123, 0, 0, 0); 
		current_NG = SET;
		printf("\r\nIOVCC Sleep Current is abnormal!\r\n");
	}
	else if (I_VSP > SPEC_SLEEP_VSP)
	{
		FPGA_Info_Set((uint8_t *)"SLEEP");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(99, 0, 0, 0); 
		current_NG = SET;
		printf("\r\nVSP Sleep Current is abnormal!\r\n");
	}
	else if (I_VSN > SPEC_SLEEP_VSN)
	{
		FPGA_Info_Set((uint8_t *)"SLEEP");
		FPGA_Info_Visible(INFO_STR);
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

	MIPI_SleepMode_OFF();
	LCD_LitSquence();
	
	printf("\r\nCURRENT SPEC -> sleep: IOVCC = %.2f; VSP = %.2f; VSN = %.2f\r\n", SPEC_SLEEP_IOVCC, SPEC_SLEEP_VSP, SPEC_SLEEP_VSN);
	Num_Temp = DIS_NUM;
	DIS_NUM = 255;
	Current_Upload();
	DIS_NUM = Num_Temp;
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

	printf("\r\nTry_Count = %d\r\n", Try_Count);
	if (Try_Count == 255)
	{
		I_IOVCC = 0.0;
		I_VSP = 0.0;
		I_VSN = 0.0;
		I_LEDA = 0.0;
		return;
	}
	
	Temp_IOVCC = strtok(&Admesy_RData[1] , " ");
	Temp_VSP = strtok(NULL , " ");
	Temp_VSN = strtok(NULL , " ");
	Temp_LEDA = strtok(NULL , " ");
	I_IOVCC = atof(Temp_IOVCC);
	I_VSP = atof(Temp_VSP);
	I_VSN = atof(Temp_VSN);
	I_LEDA = atof(Temp_LEDA);
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
		USART_printf(MSE_COM, (uint8_t *)"\r\n#SLEEP\r\n");		
		while (Admesy_RDataFlag == RESET && m < 2000)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] != '#' || m >= 2000)	//recieve data is not '#', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}
	
	printf("Try_Count = %d\r\n", Try_Count);
	if (Try_Count == 255)
	{
		I_IOVCC = 0.0;
		I_VSP = 0.0;
		I_VSN = 0.0;
		I_LEDA = 0.0;
		return;
	}
	
	Temp_IOVCC = strtok(&Admesy_RData[1] , " ");
	Temp_VSP = strtok(NULL , " ");
	Temp_VSN = strtok(NULL , " ");
	Temp_LEDA = strtok(NULL , " ");
	I_IOVCC = atof(Temp_IOVCC);
	I_VSP = atof(Temp_VSP);
	I_VSN = atof(Temp_VSN);
	I_LEDA = atof(Temp_LEDA);
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
	
	if (!auto_line)//Auto line will not save current log to SD card
	{
		sprintf(File_Name, "%s.xls", PROJECT_NO);
		sprintf(Str_Temp, "%d\t%f\t%f\t%f\t%f\r\n", DIS_NUM, I_IOVCC, I_VSP, I_VSN, I_LEDA);
		if (SD_Write_Str2File(File_Name, Str_Temp) == ERROR)
		{
			FPGA_Info_Visible(INFO_STR);
			FPGA_Info_Set((uint8_t *)"SDCARD ERROR");
			FPGA_DisPattern(114, 0, 0, 0);
			printf("SDCARD is not available, or SDCARD maybe overflow!\r\n");
			SDCard_NG = SET;
		}
	}
	printf("\r\n*#*#2:%d IOVCC %.2f ", DIS_NUM, I_IOVCC);	 
	printf("VSP %.2f ", I_VSP);
	printf("VSN %.2f ", I_VSN);
	printf("LEDA %.2f#*#*\r\n\r\n", I_LEDA);
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
	static uint8_t Reg_0A = 0x00;
	
	MIPI_DCS_Read(MAIN_PORT, 0x0A, 1, &Reg_0A);
	printf("The register 0x0A of DDIC is: 0x%02X\r\n", Reg_0A);
	if (Reg_0A == 0X1C || Reg_0A == 0X9C )//SYNA：DSC R0A == 1C,other 0x9C,normal! else need to HW reset!
	{
		printf("\r\nThe display is normal.\r\n");
		Reg_0A = 0x00;
	}
	else
	{
		printf("\r\nThe display is abnormal.R0A = %x\r\n",Reg_0A);
		printf("Recovering...\r\n");
		LCM_Reset();	
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

/*********************************************************************************
* Function: Test_Mode_Switch
* Description: check test mode &switch corresponding test pattern
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Test_Mode_Switch(void)
{
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
		FPGA_DisPattern(84, 0, 0, 0);	
		return;
	}
	else if (current_NG == SET || SDCard_NG == SET || TE_NG == SET || PWM_NG == SET || ID_NG == SET || FW_NG == SET || FPGA_NG == SET || OSC_TRIM_NG == SET)	
	{		
		return;
	}
		
//	if (TEST_MODE == TEST_MODE_RA && Timer4_Sec_Counter >= AUTORESET_T)
//	{
//		Timer4_Sec_Counter = 0;
//		printf("//--------RA auto reset Driver IC!\r\n//");
//		LCM_Reset();
//	}
	
	if (DIS_NUM != DIS_NUM_OLD)
	{
#ifndef SDCARD_MODE
		switch (TEST_MODE) 
		{
			case (TEST_MODE_ET1): Test_ET1(); break;
			case (TEST_MODE_ET2):	Test_ET2(); break;
			case (TEST_MODE_ET3):	Test_ET3(); break;
			case (TEST_MODE_OTP): Test_OTP(); break;
			case (TEST_MODE_RA): Test_RA(); break;
			case (TEST_MODE_ESD): Test_ESD(); break;
			case (TEST_MODE_OD): Test_OD(); break;
			case (TEST_MODE_DEMO): Test_DEMO(); break;
			case (TEST_MODE_CTP): Test_CTP(); break;
			case (TEST_MODE_OQC1): Test_OQC1();	break;	
			case (TEST_MODE_DEBUG): Test_DEBUG(); break;
			default: Test_DEBUG(); break;						
		}
#else	
		if (!auto_line) 
		{
			if (SD_MODE_ERROR == SET)
			{
				FPGA_DisPattern (114, 0, 0, 0);
				FPGA_Info_Visible(INFO_STR);
			}
			else		
			{
				Display_Pattern();
			}
		}
#endif
		
		if (!auto_line)
		{
			if (Flag_Test_Current && TEST_MODE != TEST_MODE_ET3 && TEST_MODE != TEST_MODE_CTP)
			{
				Current_Check();	
			}
		}
		Flag_Test_Current = SET;
		
		//for G6 ET OIC
		if (DIS_NUM == 2 || DIS_NUM == 3)
		{
			Project_Info_Upload();
			if (OTP_TIMES == 0)	printf("*#*#3:OTP NO#*#*\r\n"); 
			else printf("*#*#3:OTP YES#*#*\r\n");
			printf("*#*#4:0x%04X#*#*\r\n", vcom_best);
			printf("*#*#6:%d#*#*\r\n", OTP_TIMES);
		}
		if (DIS_NUM == TOTAL_DIS_NUM - 2 ) printf("\r\n*#*#E:TEST END#*#*\r\n");	
		if (DIS_NUM == 11) printf("\r\n*#*#E:TEST END#*#*\r\n");	
		DIS_NUM_OLD = DIS_NUM;			
	}
}
