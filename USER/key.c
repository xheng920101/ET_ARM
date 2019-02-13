#include "include.h"

#define 	KEY_GROUP 	(GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_PIN)	\
						+ (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_PIN) << 1)	\
						+ (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_PIN) << 2)	\
						+ (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_PIN) << 3))

//debug add
#define		CTP_GROUP		(GPIO_ReadInputDataBit(TEST19_GPIO_PORT, TEST19_PIN)	\
						+ (GPIO_ReadInputDataBit(TEST18_GPIO_PORT, TEST18_PIN) << 1)	\
						+ (GPIO_ReadInputDataBit(TEST15_GPIO_PORT, TEST15_PIN) << 2))

#define SENSITIVE		5			//(default 8)smaller will effect Highly Sensitive!
#define SENSITIVE_CTP		8			//(default 8)smaller will effect Highly Sensitive!

uint8_t keyStateTemp  = KEY_IDLE;
uint8_t CTPStateTemp  = CTP_IDLE;

uint8_t TOTAL_DIS_NUM = 30;
uint8_t DIS_NUM = 0;
uint8_t DIS_NUM_OLD = 0xFF;

FlagStatus DIS_AUTO = RESET; //pattern auto switch is disable default.

/*********************************************************************************
* Function: KEY_Config
* Description: KEY I/O configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************************
* Function: KEY_GetState
* Description: KEY Detect,put this function in the 10mS interval loop!
* Input: none
* Output: none
* Return: KEY state
* Call: external
* modify by ywq ,2017-1-25 17:43
*/
uint8_t KEY_GetState(void)
{
	static uint8_t debounce = 0;
	static uint8_t key_old = KEY_IDLE;
	
	if (key_old != KEY_IDLE)
	{
		if (KEY_GROUP == key_old) 		
		{
			debounce++;
			if (debounce == SENSITIVE)				//effective input
			{
				return 	key_old;
			}
			else if(debounce > SENSITIVE)
			{
				  debounce = SENSITIVE + 3;					
			}		
		}
	  else
		{
				key_old = KEY_GROUP;
				debounce = 0;
				return 	KEY_IDLE;		
		}
	}
	else{
		key_old = KEY_GROUP;
		return 	KEY_IDLE;
	}
	return KEY_GROUP;
}

/*********************************************************************************
* Function: KeyProc
* Description: key funtion process
* Input: none
* Output: none
* Return: none
* Call: external
*/
void KeyProc(void)
{
	if (current_NG == SET || SDCard_NG == SET || TE_NG == SET || PWM_NG == SET ||  ID_NG == SET || FW_NG == SET || SD_MODE_ERROR == SET || FPGA_NG == SET)	
	{			
		if (keyStateTemp != KEY_IDLE)
		{
			printf("current_NG = %d\r\n", current_NG);
			printf("SDCard_NG = %d\r\n", SDCard_NG);
			printf("TE_NG = %d\r\n", TE_NG);
			printf("PWM_NG = %d\r\n", PWM_NG);
			printf("ID_NG = %d\r\n", ID_NG);
			printf("FW_NG = %d\r\n", FW_NG);
			printf("SD_MODE_ERROR = %d\r\n", SD_MODE_ERROR);
			printf("FPGA_NG = %d\r\n", FPGA_NG);
			KEY_SLEEPIN(); 			
		}
		keyStateTemp = KEY_IDLE;
		return;
	}
	
	switch (keyStateTemp)
	{
		case (KEY_IDLE):
#ifdef DSC_MODE
			if(LCM_POWER_STATE==RESET){
				LCM_Init();
				LCM_POWER_STATE=SET;
				printf("FUCK IT!\r\n");
			}
			else{
			}
#else
#endif
			break;
		case (KEY_AUTO): 
			if (TEST_MODE != TEST_MODE_OTP)	
			{
				if (DIS_AUTO == RESET){
					DIS_AUTO = SET;
				}
				else{
					DIS_AUTO = RESET;
				}
			}
			keyStateTemp = KEY_IDLE;
			break;
		case (KEY_UP): 
			if (DIS_AUTO) break;
			(DIS_NUM >= (TOTAL_DIS_NUM - 1)) ? (DIS_NUM = 0) : DIS_NUM++;
			printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM); 
			keyStateTemp = KEY_IDLE;
			break;
		case (KEY_DOWN):
			if (DIS_AUTO) break;
			(DIS_NUM == 0) ? (DIS_NUM = TOTAL_DIS_NUM - 1) : DIS_NUM--;
			printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM); 
			keyStateTemp = KEY_IDLE;
			break;
		case (KEY_SLEEP): 
			LCD_SleepIn();
			LCD_VideoMode_OFF();
			MIPI_SleepMode_ON();
			LCMPower_OFF();
			printf("\r\n*#*#E:SLEEP IN#*#*\r\n");		
			//	GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
			GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
			Delay_sec(120);
			GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new 			
			keyStateTemp = KEY_IDLE;
			break;
		case (KEY_TEST): //RESET DRIVER IC
#ifndef DSC_MODE
			printf("//--------Message from KeyProc(key.c): key control - reset Driver IC!\r\n//");
			LCM_Reset();
#else
			if(LCM_POWER_STATE==SET){
				LCD_SleepIn();
				LCD_VideoMode_OFF();
				MIPI_SleepMode_ON();
				LCMPower_OFF();
				DIS_NUM=0;
				LCM_POWER_STATE=RESET;
				printf("YOU ARE FUCKED!\r\n");
			}
			else{
			}
#endif
			keyStateTemp = KEY_TEST;
			break;
		default: 
			break;
	}
}

/*********************************************************************************
* Function: AutoSwitchPattern
* Description: key event process,must put this fun in 100mS interval loop
* Input: none
* Output: none
* Return: none
* Call: external
*/
void AutoSwitchPattern()
{
	static uint16_t auto_dly_cnt = 0;
	
	if (DIS_AUTO)
	{
		auto_dly_cnt++;
		if (auto_dly_cnt >= AUTOSWITCH_T)
		{
			auto_dly_cnt = 0;
			(DIS_NUM >= (TOTAL_DIS_NUM - 1)) ? (DIS_NUM = 0) : DIS_NUM++;
			printf("[%9.3f] TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TIMESTAMP, TOTAL_DIS_NUM, DIS_NUM);
		}
	}
}

/*********************************************************************************
* Function: KEY_SLEEPIN
* Description: key to sleep
* Input: none
* Output: none
* Return: none
* Call: external
*/
void KEY_SLEEPIN(void)
{
 	LCD_SleepIn();
	LCD_VideoMode_OFF();
 	MIPI_SleepMode_ON();
	LCMPower_OFF();
	
//	GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
	GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
	Delay_sec(120);
	GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
}

/*********************************************************************************
* Function: CTP_GetState
* Description: CTP Detect,put this function in the 10mS interval loop!
* Input: none
* Output: none
* Return: CTP state
* Call: external
* modify by ywq ,2017-3-3 16:56
*/
uint8_t CTP_GetState(void)
{
	static uint8_t debounce = 0;
	static uint8_t ctp_old = CTP_IDLE;
	
	if (ctp_old != CTP_IDLE)
	{
		if (CTP_GROUP == ctp_old) 		
		{
			debounce++;
			if (debounce == SENSITIVE_CTP)	//effective input
			{
				return ctp_old;
			}
			else if (debounce > SENSITIVE_CTP)
			{
				  debounce = SENSITIVE_CTP + 3;					
			}		
		}
	  else
		{
				ctp_old = CTP_GROUP;
				debounce = 0;
				return 	CTP_IDLE;		
		}
	}
	
	ctp_old = CTP_GROUP;
	return 	CTP_IDLE;          
}

/*********************************************************************************
* Function: CTPProc
* Description: CTP funtion process
* Input: none
* Output: none
* Return: none
* Call: external
*/
void CTPProc(void)
{	
#ifdef DIFFER2_DETECT
	uint8_t buf[3];
	uint8_t rbuf_C589[1], rbuf_C595[3];
	static FlagStatus reg_backup = RESET; 
	static FlagStatus reg_recovery = SET; 

	if (reg_backup == RESET)
	{
		WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
		WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
		MIPI_GEN_Read(MAIN_PORT, 0xC5, 1, rbuf_C589);	
		WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
		WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
		MIPI_GEN_Read(MAIN_PORT, 0xC5, 3, rbuf_C595);	
		reg_backup = SET; 
	}
#endif
		
	if (TEST_MODE != TEST_MODE_CTP) return;
//	printf("\r\n CTPStateTemp = %d ", CTPStateTemp);	
	switch (CTPStateTemp)
	{
		case (0): 
			printf("\r\nCTPState:IDLE.");
			break;		
		case (1): 	
			printf("\r\nCTPState:CTP fail.");
			DriverIC_Reset();
			IC_Init(ET1_InitCode);	
			LCD_LitSquence();
			FPGA_DisPattern(0, 255, 0, 0);	//red pattern indicate CTP fail
//			GPIO_SetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN); //Feedback to CTP kit
			break;
		case (2):
			printf("\r\nCTPState:CTP pass.");		
			DriverIC_Reset();
			IC_Init(ET1_InitCode);	
			LCD_LitSquence();		
			FPGA_DisPattern(0, 0, 255, 0); //green pattern indicate CTP pass	 
//			GPIO_SetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN); //Feedback to CTP kit
			break;
		case (3): 
			printf("\r\nCTPState:sleep in.");	
			LCD_SleepIn();
			LCD_VideoMode_OFF();
			LCD_LPMode();
			GPIO_SetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN);
			Delay_ms(150);	//Feedback to CTP kit	
			GPIO_ResetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN); 			
			break;
		case (4):
			printf("\r\nCTPState:sleep out.");
			LCD_LitSquence();	
			GPIO_SetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN);
			Delay_ms(150);	//Feedback to CTP kit	
			GPIO_ResetBits(CTP_ACK_GPIO_PORT, CTP_ACK_PIN); 				
			break;
		case (5): 
#ifdef DIFFER2_DETECT
			printf("\r\nCTPState:set voltage to 4V.");

			buf[0] = 0x06;
			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
			WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);
			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf);
			buf[0] = 0x11;
			buf[1] = 0x32;
			buf[2] = 0x50;
			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
			WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);
			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, buf); 
			reg_recovery = RESET;
#endif
			break;
		case (6):
#ifdef DIFFER2_DETECT
			printf("\r\nCTPState:set voltage to 3.5V.");	
			buf[0] = 0x05;
			WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
			WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
			MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, buf);
			reg_recovery = RESET;
#endif
			break;
		case (7): 
#ifdef DIFFER2_DETECT
			printf("\r\nCTPState:recovery voltage.");	
			if (reg_recovery == RESET)
			{
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x8900);           
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 1, rbuf_C589); 
				WriteSSDReg(MAIN_PORT, 0xBC, 0x0002);
				WriteSSDReg(MAIN_PORT, 0xBF, 0x9500);           
				MIPI_GEN_Write((PORT0 | PORT1), 0xC5, 3, rbuf_C595); 
				reg_recovery = SET;
			}
#endif
			break;
		default:
			printf("\r\nCTPState:IDLE.");
			break;
	}
	CTPStateTemp = CTP_IDLE;
}
