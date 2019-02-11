#include "include.h"

/*********************************************************************************
* Function: Board_Init
* Description: system board initial
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Board_Init(void)
{
#ifdef CURRENT_METER
	char Str_Temp[64];
	char File_Name[32];
#endif
	
//	uint32_t  CPU_ID[3];
	
	OTP_FLAG = OTP_FLAG_MAN; //load defaut otp option
	if (TEST_MODE == TEST_MODE_OTP)
	{
		PC_COM = UART4;
		MSE_COM = USART1;
	}
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	USART1_Config();
 	UART4_Config();
	printf("\r\nUSART configuration is ready.\r\n");
		
	//STM32F103系列
////	CPU_ID[0] = *(__IO u32*)(0x1FFFF7E8);
////	CPU_ID[1] = *(__IO u32*)(0x1FFFF7EC);
////	CPU_ID[2] = *(__IO u32*)(0x1FFFF7F0);
//	//STM32F207/STM32F40X系列
//	CPU_ID[0] = *(__IO u32*)(0x1FFF7A10);
//	CPU_ID[1] = *(__IO u32*)(0x1FFF7A14);
//	CPU_ID[2] = *(__IO u32*)(0x1FFF7A18);
//	printf("CPU_ID[0] = 0x%08X\r\n", CPU_ID[0]);
//	printf("CPU_ID[1] = 0x%08X\r\n", CPU_ID[1]);
//	printf("CPU_ID[2] = 0x%08X\r\n", CPU_ID[2]);
	
 	TIM3_Config();
 	printf("\r\nTimer tick start.\r\n[%.3fs]\r\n", TIMESTAMP);
	
	SysTick_Init();
	printf("\r\nSystem Tick configuration is ready.\r\n");

	NVIC_Config();
	printf("\r\nNVIC configuration is ready.\r\n");
	
	TIM2_Config();
	printf("\r\nTIMER2 configuration is ready.\r\n");
	
	ADC3_Config();
	printf("\r\nADC3 configuration is ready.\r\n");
	
//	if(TEST_MODE == TEST_MODE_RA)	//counter for RA test auto reset!
//	{
//		TIM4_Config();
//		printf("\r\nTIM4 configuration is ready.\r\n");	 
//	}
	
	TEST_Config_ON();
	if ((GPIO_ReadInputDataBit(TEST26_GPIO_PORT, TEST26_PIN) == SET)) 
	{
		printf("\r\n*#*#AUTOLINE_ENABLE#*#*\r\n");
		auto_line = SET;
	}
	else
	{
		printf("\r\n*#*#AUTOLINE_DISABLE#*#*\r\n");
		auto_line = RESET;
	}
	printf("\r\nTEST configuration is ready.\r\n");

 	LED_Config();
	LED_OFF(RED);
	LED_OFF(GREEN);
	LED_OFF(BLUE);
	printf("\r\nLED configuration is ready.\r\n");

	KEY_Config();
	printf("\r\nKEY configuration is ready.\r\n");
	
	POWER_EN_Config();
	LCMPower_Init();
	printf("\r\nPower enable configuration is ready.\r\n");

	POWER_I2C_Config();
	printf("\r\nI2C for TPS65312 configuration is ready.\r\n");

	EEPROM_I2C_Config();
	printf("\r\nI2C for EEPROM configuration is ready.\r\n");

	DSENSOR_I2C_Config();
	printf("\r\nI2C for AD7994 configuration is ready.\r\n");
 
	Admesy_Init();
	printf("\r\nAdmesy configuration is ready.\r\n");

	FPGA_FSMC_Config();
	FPGA_SPI_Config();
	printf("\r\nInterface between STM32F2xx and FPGA configuration is ready.\r\n");

	SSD2828_SPI_Config();
	printf("\r\nInterface between STM32F2xx and SSD2828 configuration is ready.\r\n");

	LCD_RST_Config();
	printf("\r\nInterface between STM32F2xx and LCD configuration is ready.\r\n");

#ifdef CURRENT_METER	
	sprintf(File_Name, "%s.xls", (char *)PROJECT_NO);
	sprintf(Str_Temp, "\r\nNo\tIOVCC\tVSP\tVSN\tLEDA\r\n");
	SD_Write_Str2File(File_Name, Str_Temp);
#endif
}

/*********************************************************************************
* Function: NVIC_Config
* Description: Nested Vectored Interrupt Controller configure
* Input: none
* Output: none
* Return: none
* Call: external
**********************************************************************************
* 4bit for priority setting, 0 is the highest priority
*                       preemption		sub
* NVIC_PriorityGroup_0:		0					4
* NVIC_PriorityGroup_1:		1					3
* NVIC_PriorityGroup_2:		2					2
* NVIC_PriorityGroup_3:		3 				1
* NVIC_PriorityGroup_4:		4 				0
* group priority: 0>1>2>3>4
* group > preemption > sub
* The same group cannot be interrupted
* Different groups can interrupt nesting 
*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // TIMER2: PWM output
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // TIMER3: system tick counter:ms++
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	SDDMA_VIC_Init();
}
