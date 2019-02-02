#include "include.h"

#define 	KEY_GROUP 	(GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_PIN)	\
						+ (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_PIN) << 1)	\
						+ (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_PIN) << 2)	\
						+ (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_PIN) << 3))

uint8_t TOTAL_DIS_NUM = 10;
uint8_t DIS_NUM =0;
uint16_t auto_dly_cnt = 0;
FlagStatus DIS_AUTO = RESET;
FlagStatus Current_Error = RESET;

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
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
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
* Description: KEY Detect
* Input: none
* Output: none
* Return: KEY state
* Call: external
*/
uint8_t KEY_GetState(void)
{
 	uint8_t keyStateTemp = KEY_GROUP;

	if (keyStateTemp != KEY_IDLE)
	{
		Delay_ms(50);  						//delay for jitter
		if (KEY_GROUP == keyStateTemp) 		//effective input
		{
				if (TEST_MODE == TEST_MODE_OTP)
				{
					Delay_ms(300);
				}
				else
				{
					while (KEY_GROUP != KEY_IDLE);	//wait for key up
				}
			
			return 	keyStateTemp;
		} 
	}
	return 	KEY_IDLE;          
}

/*********************************************************************************
* Function: DisState_Switch
* Description: key event process
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void DisState_Switch(void)
 {
 	uint8_t keyStateTemp  = KEY_IDLE;

 	switch (DIS_AUTO)
	{
		/* KEY control	*/
		case (RESET):
		{
			while (keyStateTemp == KEY_IDLE)
			{
		 		USART_EventProcess(); 	
//				Connect_Check();
//				Current_Check();
				RA_Mode_AutoReset();//for RA auto reset
				keyStateTemp = KEY_GetState(); 
				Con_Check();

			}
			switch (keyStateTemp)
			{																			
				case (KEY_AUTO): 
					if (TEST_MODE != TEST_MODE_OTP)	DIS_AUTO = SET; 
					break;
				case (KEY_UP): 
					(DIS_NUM >= (TOTAL_DIS_NUM - 1)) ? (DIS_NUM = 0) : DIS_NUM++;
					printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM); 
					break;
				case (KEY_DOWN):
					(DIS_NUM == 0) ? (DIS_NUM = TOTAL_DIS_NUM - 1) : DIS_NUM--;
					printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM); 
					break;
				case (KEY_SLEEP): 	
					printf("*#*#E:0#*#*\r\n"); //zxj@20160526 for G6 ET	
	        LCD_SleepIn();
	        LCD_VideoMode_OFF();
	        LCD_LPMode();
	        MIPI_SleepMode_ON();	
				  GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
          LCMPower_OFF();		
					GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN);
//         	GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
//	        Delay_ms(10);
//	        GPIO_ResetBits(TEST25_GPIO_PORT, TEST25_PIN); //for vpp test	
					break;
		case (KEY_TEST): //RESET DRIVER IC
		GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN);
		LCMPower_ON();
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
		GPIO_SetBits(TEST17_GPIO_PORT, TEST17_PIN);		
	  FPGA_DisPicture(6);	//picture
		LCD_PWM(0xFF);
		Delay_ms(1000);
		DIS_NUM = 0;
		SleepCurrent_Check();
					break;
				default: 
					break;
			}
			break;
		}

		/* auto run	*/				  
		case (SET):
		{
			USART_EventProcess(); 	
			Connect_Check();
//			Current_Check();
			keyStateTemp = KEY_GetState();
			switch (keyStateTemp)
			{
				case (KEY_AUTO): 
					DIS_AUTO = RESET;
					break;
				case (KEY_SLEEP): 
					printf("*#*#E:0#*#*\r\n"); //zxj@20160526 for G6 ET
					KEY_SLEEPIN(); 
					break;
				case (KEY_TEST): //RESET DRIVER IC
					DriverIC_Reset();	
					IC_Init(ET2_InitCode);	
					LCD_SleepOut();
					LCD_VideoMode_ON();
					LCD_HSMode();				
					break;
				default: 
					break;
			}
			Delay_ms(100);
			auto_dly_cnt++;
			if (auto_dly_cnt >= AUTO_TIME)
			{
				auto_dly_cnt = 0;
				(DIS_NUM >= (TOTAL_DIS_NUM - 1)) ? (DIS_NUM = 0) : DIS_NUM++;
				printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM);
			}
			break;
	   }
	   default:
			break;			
	}
	if (DIS_NUM == TOTAL_DIS_NUM - 1) 
	 {
		printf("*#*#E:1#*#*\r\n"); //zxj@20160526 for G6 ET
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
//	POWER_OTP_Reset();
//	Delay_ms(10);
//	GPIO_ResetBits(TEST25_GPIO_PORT, TEST25_PIN); //for vpp test		
	    LCD_SleepIn();
	    LCD_VideoMode_OFF();
	    LCD_LPMode();
	    MIPI_SleepMode_ON();	
			GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
      LCMPower_OFF();	
      GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN); 

//	GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
//	GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
 
 }
