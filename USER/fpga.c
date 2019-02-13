#include "include.h"
#include <stdio.h>
#include "string.h"

#define ARMVERSION "V2P6"

uint16_t hsum;
uint16_t vsum;
uint16_t line_ram;
uint16_t frame_ram_h;
uint16_t frame_ram_l;

/*********************************************************************************
* Function: FPGA_FSMC_Config
* Description: FPGA to STM32 I/O configure, FSMC for picture data transfer
* Input: none
* Output: none
* Return: none
* Call: external
*/
void FPGA_FSMC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
  
	/* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
	
	/*-- GPIOs Configuration
	+-------------------+--------------------+------------------+------------------+
	+                        SRAM pins assignment                                   +
	+-------------------+--------------------+------------------+------------------+
	| PD0  <-> FSMC_D2  | PE7  <-> FSMC_D4                    
	| PD1  <-> FSMC_D3  | PE8  <-> FSMC_D5                      
	| PD8  <-> FSMC_D13 | PE9  <-> FSMC_D6    
	| PD9  <-> FSMC_D14 | PE10 <-> FSMC_D7
	| PD10 <-> FSMC_D15 | PE11 <-> FSMC_D8
	| PD14 <-> FSMC_D0  | PE12 <-> FSMC_D9     
	| PD15 <-> FSMC_D1  | PE13 <-> FSMC_D10 
	| PD5  <-> FSMC_WR  | PE14 <-> FSMC_D11
	|                   | PE15 <-> FSMC_D12
	+-------------------+--------------------+
	*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5|GPIO_Pin_8 |GPIO_Pin_9
															| GPIO_Pin_10 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;     
	
	/* FSMC Write Timing */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0x2;   /* Address Setup Time */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;       
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 0x4;         /* Data Setup Time */
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;    /* FSMC Access Mode */
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;           
	
	/* Enable FSMC Bank4_SRAM Bank */	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); 
}

/*********************************************************************************
* Function: FPGA_SPI_Config
* Description: FPGA to STM32 I/O configure, SPI for parameter setting
* Input: none
* Output: none
* Return: none
* Call: external
*/
void FPGA_SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	
	GPIO_InitStructure.GPIO_Pin = FPGA_SPI_SDO_PIN;
	GPIO_Init(FPGA_SPI_SDO_GPIO_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = FPGA_MCU_CSX_PIN;
	GPIO_Init(FPGA_MCU_CSX_GPIO_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = FPGA_SPI_nCS_PIN;
	GPIO_Init(FPGA_SPI_nCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FPGA_SPI_nCS_GPIO_PORT, FPGA_SPI_nCS_PIN);
	
	GPIO_InitStructure.GPIO_Pin = FPGA_SPI_SCK_PIN;
	GPIO_Init(FPGA_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);

	GPIO_InitStructure.GPIO_Pin = FPGA_SPI_SDI_PIN;
	GPIO_Init(FPGA_SPI_SDI_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(FPGA_SPI_SDI_GPIO_PORT, FPGA_SPI_SDI_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = FPGA_nRst_PIN;
	GPIO_Init(FPGA_nRst_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FPGA_nRst_GPIO_PORT, FPGA_nRst_PIN);		 
}

/*********************************************************************************
* Function: FPGA_SPI3Write
* Description: SPI protocol, send byte at SCK posetive edge
* Input: -data, data or command
* Output: none
* Return: none
* Call: internal
*/
 void  FPGA_SPI3Write(uint8_t dataWrt)
 {
	 uint8_t bitPos = 0x80;

	 GPIO_ResetBits(FPGA_SPI_nCS_GPIO_PORT, FPGA_SPI_nCS_PIN);
	 while (bitPos)
	{
		GPIO_ResetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);     
		if (dataWrt & bitPos)
		{
			GPIO_SetBits(FPGA_SPI_SDI_GPIO_PORT, FPGA_SPI_SDI_PIN);
		}
		else
		{
			GPIO_ResetBits(FPGA_SPI_SDI_GPIO_PORT, FPGA_SPI_SDI_PIN);
		}
		GPIO_SetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);		
		bitPos >>= 1;
	}
	GPIO_SetBits(FPGA_SPI_nCS_GPIO_PORT, FPGA_SPI_nCS_PIN);			
 }

/*********************************************************************************
* Function: FPGA_SPI3Read
* Description: SPI protocol, read byte at SCK posetive edge
* Input: none
* Output: none
* Return: read data
* Call: internal
*/
 uint8_t FPGA_SPI3Read(void)
 {
	 uint8_t bitPos = 0x80;
	 uint8_t dataRd = 0x00;

	 GPIO_ResetBits(FPGA_SPI_nCS_GPIO_PORT, FPGA_SPI_nCS_PIN);
	 GPIO_ResetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);
	 GPIO_SetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);
	 while (bitPos)
	{
		GPIO_ResetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);       
		if (GPIO_ReadInputDataBit(FPGA_SPI_SDO_GPIO_PORT, FPGA_SPI_SDO_PIN) == SET)
		{
			dataRd |= bitPos;	
		}
		else
		{
			dataRd |= 0x00;
		}
		GPIO_SetBits(FPGA_SPI_SCK_GPIO_PORT, FPGA_SPI_SCK_PIN);		
		bitPos >>= 1;
	} 
	GPIO_SetBits(FPGA_SPI_nCS_GPIO_PORT, FPGA_SPI_nCS_PIN);	

	FPGA_SPI3Write(0x00); //ensure that SPI write circle is complete and correct: first byte is address, second byte is parameter
	
	return dataRd;      
 }

/*********************************************************************************
* Function: FPGA_Reset
* Description: FPGA reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void FPGA_Reset(void)
 {
 	 GPIO_ResetBits(FPGA_nRst_GPIO_PORT, FPGA_nRst_PIN);	
	 Delay_ms(10);
	 GPIO_SetBits(FPGA_nRst_GPIO_PORT, FPGA_nRst_PIN);	
	 Delay_ms(10);
 }

/*********************************************************************************
* Function: FPGA_Porch_Set
* Description: porch setting
* Input: none
* Output: none
* Return: none
* Call: external
*/
void FPGA_Porch_Set(FPGAPORCH_TypeDef FPGA_porchPara)
{
	FPGA_SPI3Write(ADDR_HACT_1);
	FPGA_SPI3Write((FPGA_porchPara.HACT & 0xFF00) >> 8);
	FPGA_SPI3Write(ADDR_HACT_2);
	FPGA_SPI3Write(FPGA_porchPara.HACT & 0x00FF);

	FPGA_SPI3Write(ADDR_VACT_1);
	FPGA_SPI3Write((FPGA_porchPara.VACT & 0xFF00) >> 8);
	FPGA_SPI3Write(ADDR_VACT_2);
	FPGA_SPI3Write(FPGA_porchPara.VACT & 0x00FF);

	FPGA_SPI3Write(ADDR_HPW);
	FPGA_SPI3Write(FPGA_porchPara.HSA);

	FPGA_SPI3Write(ADDR_HBP_1);
	FPGA_SPI3Write(0x00);
	FPGA_SPI3Write(ADDR_HBP_2);
	FPGA_SPI3Write(FPGA_porchPara.HBP);

	FPGA_SPI3Write(ADDR_HFP_1);
	FPGA_SPI3Write(0x00);
	FPGA_SPI3Write(ADDR_HFP_2);
	FPGA_SPI3Write(FPGA_porchPara.HFP);

	FPGA_SPI3Write(ADDR_VPW);
	FPGA_SPI3Write(FPGA_porchPara.VSA);

	FPGA_SPI3Write(ADDR_VBP_1);
	FPGA_SPI3Write(0x00);
	FPGA_SPI3Write(ADDR_VBP_2);
	FPGA_SPI3Write(FPGA_porchPara.VBP);

	FPGA_SPI3Write(ADDR_VFP_1);
	FPGA_SPI3Write(0x00);
	FPGA_SPI3Write(ADDR_VFP_2);
	FPGA_SPI3Write(FPGA_porchPara.VFP);

#ifdef SINGLE_PORT	
{
	hsum = FPGA_porchPara.HACT;
	vsum = FPGA_porchPara.VACT;
}
#else
{
	hsum = (FPGA_porchPara.HACT << 1);
	vsum = FPGA_porchPara.VACT;
}
#endif
	
	line_ram = FPGA_porchPara.HACT * 3;	
	frame_ram_h = ((line_ram * FPGA_porchPara.VACT) >> 16) & 0x0000FFFF;
	frame_ram_l = (line_ram * FPGA_porchPara.VACT) & 0x0000FFFF;
}

/*********************************************************************************
* Function: FPGA_Display_Set
* Description: none
* Input: none
* Output: none
* Return: none
* Call: external
*/
void FPGA_Display_Set(void)
{
	FPGA_SPI3Write(ADDR_PIC_MASK);
	FPGA_SPI3Write(0);
	
#ifdef TE_DETECT
{
	FPGA_SPI3Write(ADDR_TE_DETECT);
	FPGA_SPI3Write(0);
}
#else
	{
		FPGA_SPI3Write(ADDR_TE_DETECT);
		FPGA_SPI3Write(0);
	}
#endif
	
	FPGA_SPI3Write(ADDR_INFO_Y_1);
	FPGA_SPI3Write((INFO_Y_AXIS & 0x0F00) >> 8);		
	FPGA_SPI3Write(ADDR_INFO_Y_2);
	FPGA_SPI3Write(INFO_Y_AXIS & 0x00FF);
	
	FPGA_SPI3Write(ADDR_PAT_RECT_XY);
	FPGA_SPI3Write(((RECT_START_X & 0x0F00) >> 4) + ((RECT_START_Y & 0x0F00) >> 8));
	FPGA_SPI3Write(ADDR_PAT_RECT_X);
	FPGA_SPI3Write(RECT_START_X & 0x00FF);
	FPGA_SPI3Write(ADDR_PAT_RECT_Y);
	FPGA_SPI3Write(RECT_START_Y & 0x00FF);
	FPGA_SPI3Write(ADDR_PAT_RECT_S_X);
	FPGA_SPI3Write(RECT_SIZE_X);
	FPGA_SPI3Write(ADDR_PAT_RECT_S_Y);
	FPGA_SPI3Write(RECT_SIZE_Y);
}

/*********************************************************************************
* Function: FPGA_Info_Set
* Description: set the first line display comment
* Input: -info
* Output: none
* Return: none
* Call: external
*/
 void FPGA_Info_Set(uint8_t * info)
 {
	uint8_t i = 0;
	
	while ((*(info + i)) != 0)
	{
		FPGA_SPI3Write(ADDR_INFO_CHAR_0 + i);
		FPGA_SPI3Write(*(info + i));
		i++;
		if (i > 13)	 break;
	}

	while (i < 14)
	{
		FPGA_SPI3Write(ADDR_INFO_CHAR_0 + i);
		FPGA_SPI3Write(0);
		i++;
	}
 }

/*********************************************************************************
* Function: FPGA_Project_Set
* Description: set the second line display comment: project number
* Input: -info
* Output: none
* Return: none
* Call: external
*/
 void FPGA_Project_Set(uint8_t * info)
 {
	uint8_t i = 0;
	
	while ((*(info + i)) != 0)
	{
		FPGA_SPI3Write(ADDR_PROJECT_CHAR_0 + i);
		FPGA_SPI3Write(*(info + i));
		i++;
		if (i > 21)	 break;
	}

	while (i < 22)
	{
		FPGA_SPI3Write(ADDR_PROJECT_CHAR_0 + i);
		FPGA_SPI3Write(0);
		i++;
	}
 }
 
 /*********************************************************************************
* Function: FPGA_Project_Check
* Description: check if FPGA project number is match the ARM
* Input: none
* Output: none
* Return: none
* Call: external
*/
void FPGA_Project_Check(void)
{
	uint8_t i = 0;
	uint8_t buf[18];
	
	while (i < 17)
	{
		FPGA_SPI3Write(ADDR_PROJECT_CHAR_0 + i);
		FPGA_SPI3Write(0x00);
		buf[i] = FPGA_SPI3Read(); 
		if (buf[i] != PROJECT_NO[i]) FPGA_NG = SET; 
		i++;
	}
	buf[i] = '\0'; //string end
	
	printf("ARM PROJECT_NO is %s\r\n", PROJECT_NO);
	printf("FPGA PROJECT_NO is %s\r\n", buf);
	if (FPGA_NG == SET)
	{
		printf("PROJECT_NO is not match!\r\n");
		printf("*#*#Connector is open!#*#*\r\n"); //for gammaexpert
	}
}
 
 /*********************************************************************************
* Function: FPGA_Version_Set
* Description: set the third line display comment: fw version
* Input: -info
* Output: none
* Return: none
* Call: external
*/
 void FPGA_Version_Set(uint8_t * info)
 {
	uint8_t i = 0;
	
	while ((*(info + i)) != 0)
	{
		FPGA_SPI3Write(ADDR_VERSION_CHAR_0 + i);
		FPGA_SPI3Write(*(info + i));
		i++;
		if (i > 12)	 break;
	}

	while (i < 13)
	{
		FPGA_SPI3Write(ADDR_VERSION_CHAR_0 + i);
		FPGA_SPI3Write(0);
		i++;
	}
 }
 
/*********************************************************************************
* Function: Version_Set
* Description: set the first line display comment: program version
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Version_Set(void)
{
#ifdef SDCARD_MODE
	FPGA_Info_Set((uint8_t *)VERSION_DISPLAY);
	printf("test mode is  %s\r\n", VERSION_DISPLAY);
#else
	switch (TEST_MODE) 
	{
		case (TEST_MODE_ET1):	FPGA_Info_Set((uint8_t *)VERSION_ET1);	printf("\r\nTest mode is ET1.\r\n");	break;
		case (TEST_MODE_ET2):	FPGA_Info_Set((uint8_t *)VERSION_ET2);	printf("\r\nTest mode is ET2.\r\n");	break;
		case (TEST_MODE_ET3):	FPGA_Info_Set((uint8_t *)VERSION_ET3);	printf("\r\nTest mode is ET3.\r\n");	break;
		case (TEST_MODE_OTP):	FPGA_Info_Set((uint8_t *)VERSION_OTP);	printf("\r\nTest mode is OTP.\r\n");	break;
		case (TEST_MODE_RA):	FPGA_Info_Set((uint8_t *)VERSION_RA);		printf("\r\nTest mode is RA.\r\n");		break;
		case (TEST_MODE_ESD):	FPGA_Info_Set((uint8_t *)VERSION_ESD);	printf("\r\nTest mode is ESD.\r\n");	break;
		case (TEST_MODE_OD):	FPGA_Info_Set((uint8_t *)VERSION_OD);		printf("\r\nTest mode is OD.\r\n");		break;
		case (TEST_MODE_DEMO):	FPGA_Info_Set((uint8_t *)VERSION_DEMO);	printf("\r\nTest mode is DEMO.\r\n");	break;
		case (TEST_MODE_CTP):	FPGA_Info_Set((uint8_t *)VERSION_CTP);	printf("\r\nTest mode is CTP.\r\n");	break;
		case (TEST_MODE_OQC1):	FPGA_Info_Set((uint8_t *)VERSION_OQC1);	printf("\r\nTest mode is OQC1.\r\n");	break;		
		default: 				FPGA_Info_Set((uint8_t *)VERSION_DEBUG);	printf("\r\nTest mode is DEBUG.\r\n");	break;
	}
#endif
	
#ifdef NO_FLASH_MODE
	FPGA_Version_Set((uint8_t *)FWVersion);
#endif
}

/*********************************************************************************
* Function: Project_Info_Upload
* Description: upload project information to PC
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Project_Info_Upload(void)
{
	char array[16];
	char *temp1, *temp2;

	switch (TEST_MODE) 
	{
		case (TEST_MODE_ET1):	sprintf(array, VERSION_ET1); break;
		case (TEST_MODE_ET2): sprintf(array, VERSION_ET2); break;
		case (TEST_MODE_ET3): sprintf(array, VERSION_ET3); break;
		case (TEST_MODE_OTP):	sprintf(array, VERSION_OTP); break;
		case (TEST_MODE_RA):	sprintf(array, VERSION_RA); break;
		case (TEST_MODE_ESD):	sprintf(array, VERSION_ESD); break;
		case (TEST_MODE_OD):	sprintf(array, VERSION_OD); break;
		case (TEST_MODE_DEMO):sprintf(array, VERSION_DEMO); break;
		case (TEST_MODE_CTP):	sprintf(array, VERSION_CTP); break;
		case (TEST_MODE_OQC1):sprintf(array, VERSION_OQC1); break;	
		default: sprintf(array, VERSION_DEBUG);	break;
	}
	temp1 = strtok(array, "_");
	temp2 = strtok(NULL, "_");
	
#ifndef SDCARD_MODE
	printf("\r\n*#*#1:%s_%s_ARM%s_%s#*#*\r\n", PROJECT_NO, temp1, ARMVERSION, temp2);
#else
//	if (!auto_line) 
	{
		printf("\r\n*#*#1:%s_ET_ARM%s_%s_%s#*#*\r\n", PROJECT_NO, ARMVERSION, VERSION_SDmode, VERSION_DISPLAY);
	}
//	else
//	{
//		printf("\r\n*#*#1:%s_%s_ARM%s_%s#*#*\r\n", PROJECT_NO, temp1, ARMVERSION, temp2);
//	}
#endif
}

/*********************************************************************************
* Function: FPGA_Info_Visible
* Description: select what informationg to show on the panel
* Input: -visible, 0 = unvisible, 1 = visible
*         bit0 = string set by FPGA_Info_Set(string)
*         bit1 = "PXXX"	
*         bit2 = (R;G;B)	
*         bit3 = OTP times
*         bit4 = "ET1_VXX FPGA_VXX"
*         bit5 = "ARM_VXX"	
*         bit6 = "TL055VDXP40-00"
*         bit7 = 
* Output: none
* Return: none
* Call: external
*/
void FPGA_Info_Visible(uint8_t visible)
{	
	char temp[16];
	sprintf(temp, "ARM_%s", ARMVERSION);
	 
	if ((visible & INFO_ARM_VERSION) == INFO_ARM_VERSION)
	{
		FPGA_Info_Set((uint8_t *)temp);
	}
	else
	{
		if ((visible & INFO_VERSION) == INFO_VERSION)
		{
			Version_Set();
		}
	}
	FPGA_SPI3Write(ADDR_INFO_SHOW_EN);
	FPGA_SPI3Write(visible);

	if ((visible & INFO_OTPTIMES) == INFO_OTPTIMES)
	{
		FPGA_SPI3Write(ADDR_OTP_TIMES);
		FPGA_SPI3Write(OTP_TIMES);
		sprintf(temp, "%02d", OTP_TIMES);
		FPGA_SPI3Write(ADDR_OTP_TIMES_1);
		FPGA_SPI3Write(temp[0]);
		FPGA_SPI3Write(ADDR_OTP_TIMES_2);
		FPGA_SPI3Write(temp[1]);
	}
 }

/*********************************************************************************
* Function: FPGA_DisPattern
* Description: display picture
* Input: -ptnNum, pattern number to be display
* Input: -rdata, R channel value, or background gray level
* Input: -rdata, G channel value
* Input: -rdata, B channel value
* Output: none
* Return: none
* Call: external
*/
void FPGA_DisPattern(uint8_t ptnNum, uint8_t rdata, uint8_t gdata, uint8_t bdata)
{
	char gray[4];
	
	if (ptnNum == 22)
	{
		SPEC_MAX_IOVCC =	SPEC_MAX_FLICKER_IOVCC;
		SPEC_MAX_VSP =	SPEC_MAX_FLICKER_VSP;
		SPEC_MAX_VSN	=	SPEC_MAX_FLICKER_VSN;	
	}
	else if (ptnNum == 23)
	{
		SPEC_MAX_IOVCC =	SPEC_MAX_CHECK_PIXEL_IOVCC;
		SPEC_MAX_VSP =	SPEC_MAX_CHECK_PIXEL_VSP;
		SPEC_MAX_VSN	= SPEC_MAX_CHECK_PIXEL_VSN;	
	}
	else if (ptnNum == 24)
	{
		SPEC_MAX_IOVCC =	SPEC_MAX_CHECK_DOT_IOVCC;
		SPEC_MAX_VSP =	SPEC_MAX_CHECK_DOT_VSP;
		SPEC_MAX_VSN	=	SPEC_MAX_CHECK_DOT_VSN;	
	}
	else
	{
		SPEC_MAX_IOVCC =	SPEC_MAX_RED_IOVCC;
		SPEC_MAX_VSP =	SPEC_MAX_RED_VSP;
		SPEC_MAX_VSN	=	SPEC_MAX_RED_VSN;
	}
	
	if (ptnNum == 128) 
	{
		FPGA_SPI3Write(ADDR_PAT_BG_R);
		FPGA_SPI3Write(HW_CRST_BG_R);     
		FPGA_SPI3Write(ADDR_PAT_BG_G);
		FPGA_SPI3Write(HW_CRST_BG_G);     
		FPGA_SPI3Write(ADDR_PAT_BG_B);
		FPGA_SPI3Write(HW_CRST_BG_B);	 
	}
	if (ptnNum == 133) 
	{
		FPGA_SPI3Write(ADDR_PAT_BG_R);
		FPGA_SPI3Write(HW_CROSS_BG_R);     
		FPGA_SPI3Write(ADDR_PAT_BG_G);
		FPGA_SPI3Write(HW_CROSS_BG_G);     
		FPGA_SPI3Write(ADDR_PAT_BG_B);
		FPGA_SPI3Write(HW_CROSS_BG_B);	 
	}
	if (ptnNum == 82) //bright dot
	{
		FPGA_Info_Visible(INFO_STR);
		if (rdata == 0) //CH1
		{
			FPGA_Info_Set((uint8_t *)"CH1");
			FPGA_SPI3Write(ADDR_PAT_R1);
			FPGA_SPI3Write(100);
			FPGA_SPI3Write(ADDR_PAT_G1);
			FPGA_SPI3Write(70);
			FPGA_SPI3Write(ADDR_PAT_B1);
			FPGA_SPI3Write(170);	
			FPGA_SPI3Write(ADDR_PAT_R2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_G2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_B2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_R3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_G3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_B3);
			FPGA_SPI3Write(0);	
			rdata = 70;
			gdata = 70;
			bdata = 70;
		}
		else if (rdata == 1) //CX1
		{
			FPGA_Info_Set((uint8_t *)"CX1");
			FPGA_SPI3Write(ADDR_PAT_R1);
			FPGA_SPI3Write(145);
			FPGA_SPI3Write(ADDR_PAT_G1);
			FPGA_SPI3Write(80);
			FPGA_SPI3Write(ADDR_PAT_B1);
			FPGA_SPI3Write(238);	
			FPGA_SPI3Write(ADDR_PAT_R2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_G2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_B2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_R3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_G3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_B3);
			FPGA_SPI3Write(0);	
			rdata = 0;
			gdata = 0;
			bdata = 0;
		}
		else if (rdata == 2) //CM1
		{
			FPGA_Info_Set((uint8_t *)"CM1");
			FPGA_SPI3Write(ADDR_PAT_R1);
			FPGA_SPI3Write(135);
			FPGA_SPI3Write(ADDR_PAT_G1);
			FPGA_SPI3Write(83);
			FPGA_SPI3Write(ADDR_PAT_B1);
			FPGA_SPI3Write(157);	
			FPGA_SPI3Write(ADDR_PAT_R2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_G2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_B2);
			FPGA_SPI3Write(0);
			FPGA_SPI3Write(ADDR_PAT_R3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_G3);
			FPGA_SPI3Write(0);     
			FPGA_SPI3Write(ADDR_PAT_B3);
			FPGA_SPI3Write(0);	
			rdata = 0;
			gdata = 0;
			bdata = 0;
		} 
	}

	FPGA_SPI3Write(ADDR_DIS_MODE);
	FPGA_SPI3Write(0);
	FPGA_SPI3Write(ADDR_DIS_SN);
	FPGA_SPI3Write(ptnNum);

	FPGA_SPI3Write(ADDR_PAT_R);
	FPGA_SPI3Write(rdata);
	FPGA_SPI3Write(ADDR_PAT_G);
	FPGA_SPI3Write(gdata);
	FPGA_SPI3Write(ADDR_PAT_B);
	FPGA_SPI3Write(bdata);
	
	FPGA_SPI3Write(ADDR_PAT_GRAY);
	FPGA_SPI3Write(rdata);

	sprintf(gray, "%d", rdata);
	FPGA_SPI3Write(ADDR_PAT_GRAY_1);
	FPGA_SPI3Write(gray[0]);
	FPGA_SPI3Write(ADDR_PAT_GRAY_2);
	FPGA_SPI3Write(gray[1]);
	FPGA_SPI3Write(ADDR_PAT_GRAY_3);
	FPGA_SPI3Write(gray[2]);
	 
#ifdef CMD_MODE
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg(PORT0|PORT1, SSD_CFGR, SSD_B7);
	WriteSSDReg(PORT0|PORT1, 0xBC, frame_ram_l);
	WriteSSDReg(PORT0|PORT1, 0xBD, frame_ram_h);
	WriteSSDReg(PORT0|PORT1, 0xBE, line_ram);
	WriteCMD2SSD(PORT0|PORT1, 0x2C);
	
	GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
	GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);	 
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0xDD);
	
	Delay_us(1);
	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA sent out all data to MIPI
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0x00);
	
	WriteSSDReg(PORT0|PORT1, 0xBC, 0x0000);
	WriteSSDReg(PORT0|PORT1, 0xBD, 0x0000);
	WriteSSDReg(PORT0|PORT1, 0xBE, 0x0000);
#endif
 }

/*********************************************************************************
* Function: FPGA_DisPicture
* Description: display picture
* Input: -picNum, picture number to be display
* Output: none
* Return: none
* Call: external
*/
void FPGA_DisPicture(uint8_t picNum)
{	
	SPEC_MAX_IOVCC =	200.0;
	SPEC_MAX_VSP =	200.0;
	SPEC_MAX_VSN	=	200.0;
	
	FPGA_SPI3Write(ADDR_DIS_MODE);
	FPGA_SPI3Write(1);
	FPGA_SPI3Write(ADDR_DIS_SN);
	FPGA_SPI3Write(picNum);

#ifdef CMD_MODE
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg(PORT0|PORT1, SSD_CFGR, SSD_B7);
	WriteSSDReg(PORT0|PORT1, 0xBC, frame_ram_l);
	WriteSSDReg(PORT0|PORT1, 0xBD, frame_ram_h);
	WriteSSDReg(PORT0|PORT1, 0xBE, line_ram);
	WriteCMD2SSD(PORT0|PORT1, 0x2C);
	
	GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
	GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);	 
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0xDD);
	
	Delay_us(1);
	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA sent out all data to MIPI
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0x00);
	
	WriteSSDReg(PORT0|PORT1, 0xBC, 0x0000);
	WriteSSDReg(PORT0|PORT1, 0xBD, 0x0000);
	WriteSSDReg(PORT0|PORT1, 0xBE, 0x0000);
#endif
}
 
/*********************************************************************************
* Function: FPGA_PIC_WR_CFG
* Description: parameters related to ARM sending picture data to FGPA
* Input: -wr_en: send enable
* Input: -wr_num: how many pictures to be sent
* Input: -bst_num: how mayn SDRAM bursts need to store 1 picture
* Input: -size_rsv: SDRAM size reserved for 1 picture
* Output: none
* Return: none
* Call: external
*/
 void FPGA_PIC_WR_CFG(uint8_t wr_en, uint8_t wr_num, uint16_t bst_num, uint8_t size_rsv, uint16_t sdramlastBurstUse)
 {
	 if (!auto_line)
	 {
#ifndef SDCARD_MODE
		 FPGA_SPI3Write(ADDR_PIC_WR_NUM);
		 FPGA_SPI3Write(wr_num);
#endif
	 }
	 else
	 {
		 FPGA_SPI3Write(ADDR_PIC_WR_NUM);
		 FPGA_SPI3Write(wr_num);
	 }

	 FPGA_SPI3Write(ADDR_PIC_BST_NUM_1);
	 FPGA_SPI3Write((bst_num & 0xFF00) >> 8);
	 FPGA_SPI3Write(ADDR_PIC_BST_NUM_2);
	 FPGA_SPI3Write(bst_num & 0x00FF);

	 FPGA_SPI3Write(ADDR_PIC_SIZE_RSV);
	 FPGA_SPI3Write(size_rsv);
	 
	 FPGA_SPI3Write(ADDR_PIC_LAST_BST_NUM_1);
	 FPGA_SPI3Write((sdramlastBurstUse & 0xFF00) >> 8);
	 FPGA_SPI3Write(ADDR_PIC_LAST_BST_NUM_2);
	 FPGA_SPI3Write(sdramlastBurstUse & 0x00FF);

	 FPGA_SPI3Write(ADDR_PIC_WR_EN);  //configured at last
	 FPGA_SPI3Write(wr_en);
 }
 
/*********************************************************************************
* Function: FPGA_INIT_END_INFO
* Description: tell FPGA IC initialization status
* Input: -init_end: 1'b1 - initialization complete; 1'b0 - not completed
* Output: none
* Return: none
* Call: external
*/
 void FPGA_INIT_END_INFO(uint8_t init_end)
 {
	 FPGA_SPI3Write(ADDR_INIT_END);
	 FPGA_SPI3Write(init_end);
 }
 
/*********************************************************************************
* Function: FPGA_PORT_MAP
* Description: set the FPGA PORT mapping
* Input: -port0_map: 1'b1 - PORT0 mapping to left; 1'b0 - PORT0 mapping to right
* Output: none
* Return: none
* Call: external
*/
 void FPGA_PORT_MAP(uint8_t port0_map)
 {
	 FPGA_SPI3Write(ADDR_PORT_MAP);
	 FPGA_SPI3Write(port0_map);
	 
	 FPGA_SPI3Write(ADDR_PORT_MAIN);
	 FPGA_SPI3Write(MAIN_PORT);
 }
 
 
 /*********************************************************************************
* Function: FPGA_Initial
* Description: Config FPGA to work
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void FPGA_Initial()
 {
#ifndef SDCARD_MODE
		char buf[] = PROJECT_NO;
		char *tmp;
#endif

		/* FPGA initial */
		printf("\r\nFPGA_Reset...\r\n");
		FPGA_Reset();
		printf("\r\nFPGA_Info_Visible...\r\n");
		FPGA_Info_Visible(INFO_NONE);
		printf("\r\nFPGA_Info_Set...\r\n");
		FPGA_Info_Set((uint8_t *)"");	
		printf("\r\nFPGA_Project_Set...\r\n");
//	  FPGA_Project_Check();
#ifdef SDCARD_MODE
	  FPGA_Project_Set((uint8_t *)PRO_DISPLAY);
#else
		tmp = strtok(buf, "_");
		FPGA_Project_Set((uint8_t *)tmp);	
#endif
		printf("\r\nFPGA_Porch_Set...\r\n");
		FPGA_Porch_Set(FPGA_porchPara);
	 	printf("\r\nFPGA_Display_Set...\r\n");
		FPGA_Display_Set();
		printf("\r\nFPGA_PORT_MAP...\r\n");
		FPGA_PORT_MAP(1);
		FPGA_INIT_END_INFO(1);
		FPGA_DisPattern(0, 0, 0, 0); 
 }
