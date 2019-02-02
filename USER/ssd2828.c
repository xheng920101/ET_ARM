#include "include.h"

uint16_t SSD_B7;

/*********************************************************************************
* Function: SSD2828_SPI_Config
* Description: SSD2828 SPI interface configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void SSD2828_SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	
	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_SDO_PIN;
	GPIO_Init(SSD2828_SPI_SDO_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_SCK_PIN;
	GPIO_Init(SSD2828_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);

	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_SDI_PIN;
	GPIO_Init(SSD2828_SPI_SDI_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SSD2828_SPI_SDI_GPIO_PORT, SSD2828_SPI_SDI_PIN);	

	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_SDC_PIN;
	GPIO_Init(SSD2828_SPI_SDC_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SSD2828_SPI_SDC_GPIO_PORT, SSD2828_SPI_SDC_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = SSD2828_nRST_PIN;
	GPIO_Init(SSD2828_nRST_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SSD2828_nRST_GPIO_PORT, SSD2828_nRST_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_nCS0_PIN;
	GPIO_Init(SSD2828_SPI_nCS0_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = SSD2828_SPI_nCS1_PIN;
	GPIO_Init(SSD2828_SPI_nCS1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);	
}

/*********************************************************************************
* Function: MIPI_Reset
* Description: SSD2828 reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void MIPI_Reset(void)
 {
	 printf("\r\nMIPI reset...\r\n");
 	 GPIO_ResetBits(SSD2828_nRST_GPIO_PORT, SSD2828_nRST_PIN);
	 Delay_ms(20);				//20 ->15RESET "low" pulse width Min 10mS
 	 GPIO_SetBits(SSD2828_nRST_GPIO_PORT, SSD2828_nRST_PIN);
 	 Delay_ms(20);					//20->5 2016-8-12
 }
 
/*********************************************************************************
* Function: IC_Init
* Description: download initial code
* Input: -initCode, pointer to the initial code
* Output: none
* Return: none
* Call: external
*/
void IC_Init(uint16_t * initCode)
{
	uint8_t cmd;
	uint8_t data;
	uint8_t initEnd = 0;
	uint16_t * p = 	initCode;

	while (initEnd == 0)
	{
		data = (*p);
		cmd = (*p++) >> 8;
		switch (cmd)
		{
			case (0x01): WriteCMD2SSD((PORT0 | PORT1), data); break;	
			case (0x02): WriteDATA2SSD((PORT0 | PORT1), data); break;
			case (0x03): Delay_ms(data); break;
			case (0x10): WriteCMD2SSD(PORT0, data); break;
			case (0x20): WriteDATA2SSD(PORT0, data); break;	
			case (0x11): WriteCMD2SSD(PORT1, data); break;
			case (0x21): WriteDATA2SSD(PORT1, data); break;
			case (0x07): switch (data)
						{
							case (0x00): LED_OFF(RED); break;	
							case (0x01): LED_OFF(GREEN); break;
							case (0x02): LED_OFF(BLUE); break;	
							case (0x10): LED_ON(RED); break;
							case (0x11): LED_ON(GREEN); break;	
							case (0x12): LED_ON(BLUE); break;
							case (0xA0): POWER_IOVCC_Reset(); break;	
							case (0xA1): POWER_IOVCC_Set(); break;
							case (0xB0): POWER_VCI_Reset(); break;
							case (0xB1): POWER_VCI_Set(); break;
							case (0xC0): POWER_VSP_Reset(); break;
							case (0xC1): POWER_VSP_Set(); break;
							case (0xD0): POWER_VSN_Reset(); break;
							case (0xD1): POWER_VSN_Set(); break;
							case (0xE0): LCD_PWM(0xFF); break;
							case (0xE1): LCD_PWM(0x00); break;
							case (0xF0): POWER_OTP_Reset(); break;
							case (0xF1): POWER_OTP_Set(); break;
							default: break;
						}
						break;
			case (0x0F): initEnd = 1; break;
			default: initEnd = 1; printf("Command invalid! Please check your code.\r\n"); break;
		}			
	}

	SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
	printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
}

/*********************************************************************************
* Function: SPI3Write
* Description: SPI protocol, send byte at SCK posetive edge
* Input: -data, low 8 bit is data, 9th bit is indicate the data type, 0 is data, 1 is command
* Output: none
* Return: none
* Call: internal
*/
 void SPI3Write(uint16_t data)
 {
	 uint16_t bitPos = 0x0100;
	 
	 while (bitPos)
	{	
		GPIO_ResetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);     
		if (data & bitPos)
		{
			GPIO_SetBits(SSD2828_SPI_SDI_GPIO_PORT, SSD2828_SPI_SDI_PIN);			
		}
		else
		{
			GPIO_ResetBits(SSD2828_SPI_SDI_GPIO_PORT, SSD2828_SPI_SDI_PIN);
		}
		Delay_us(10);	      
		GPIO_SetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);  		
		bitPos >>= 1;
	} 
	
	GPIO_ResetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);
	GPIO_ResetBits(SSD2828_SPI_SDI_GPIO_PORT, SSD2828_SPI_SDI_PIN);
 }

/*********************************************************************************
* Function: SPI3Read
* Description: SPI protocol, read byte at SCK posetive edge
* Input: -port, MIPI port to be read
* Output: none
* Return: read data
* Call: internal
*/
 uint16_t SPI3Read(uint8_t port)
 {
	 uint16_t bitPos = 0x8000;
	 uint16_t dataRd = 0x0000;

	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	
	 while (bitPos)
	{          
		GPIO_SetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);; 
		if (GPIO_ReadInputDataBit(SSD2828_SPI_SDO_GPIO_PORT, SSD2828_SPI_SDO_PIN) == SET)	  // 下降沿变化，上升沿采样
		{
			dataRd |= bitPos;;	
		}
		else
		{
			;;
		} 
		GPIO_ResetBits(SSD2828_SPI_SCK_GPIO_PORT, SSD2828_SPI_SCK_PIN);;        		  
		bitPos >>= 1;
	} 

	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	dataRd = (dataRd << 8) | (dataRd >> 8);
	return dataRd;      
 }
 
/*********************************************************************************
* Function: WriteCMD2SSD
* Description: write command to SSD2828
* Input: -port, MIPI port to be write
* Input: -CMD, command
* Output: none
* Return: none
* Call: external
*/
 void WriteCMD2SSD(uint8_t port, uint8_t CMD)
 {
#ifdef CMD_MODE	
	 
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0xAA);
	 
	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	
	FPGA_SPI3Write(ADDR_INI_DCX); FPGA_SPI3Write(0x00); 
	FPGA_SPI3Write(ADDR_INI_DATA); FPGA_SPI3Write(CMD);

	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA sent out all data to MIPI
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0x00);
	
#else
	
	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	SPI3Write(CMD & 0x00FF);
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);

#endif
 }

/*********************************************************************************
* Function: WriteDATA2SSD
* Description: write data to SSD2828
* Input: -port, MIPI port to be write
* Input: -DATA, data
* Output: none
* Return: none
* Call: external
*/
 void WriteDATA2SSD(uint8_t port, uint8_t DATA)
 {
#ifdef CMD_MODE	
	 
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0xAA);
	 
	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	
	FPGA_SPI3Write(ADDR_INI_DCX); FPGA_SPI3Write(0x01); 
	FPGA_SPI3Write(ADDR_INI_DATA); FPGA_SPI3Write(DATA);

	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA sent out all data to MIPI
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	FPGA_SPI3Write(ADDR_OP_TYPE);
	FPGA_SPI3Write(0x00);
	
#else
	
	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	SPI3Write(DATA | 0x0100);
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
 
#endif
 }
 
/*********************************************************************************
* Function: WriteSSDReg
* Description: set the register of SSD2828
* Input: -port, MIPI port to be write
* Input: -reg, register to be set
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
*/
 void WriteSSDReg(uint8_t port, uint8_t reg, uint16_t para)
 {
	 WriteCMD2SSD(port, reg);
	 WriteDATA2SSD(port, (para & 0x00FF));
	 WriteDATA2SSD(port, ((para >> 8) & 0x00FF));
 }

/*********************************************************************************
* Function: ReadSSDReg
* Description: read the register setting of SSD2828
* Input: -port, MIPI port to be read
* Input: -reg, register to be read
* Output: none
* Return: setting value
* Call: external
* --------------------------------------------------------------------------------
*						MIPI(SSD_RR)				  SPI(SSD_RRA)
*		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
* --------------------------------------------------------------------------------
*/
 uint16_t ReadSSDReg(uint8_t port, uint8_t reg)
 {	 
#ifdef CMD_MODE 
	 	
	 uint16_t	rdata;
	 uint8_t rdata1; 
	 
	 WriteCMD2SSD(port, reg);	// register to be read
	 
	 switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	
	FPGA_SPI3Write(ADDR_OP_TYPE);//enter read state
	FPGA_SPI3Write(0xBB);
	
	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA finish read data from MIPI slave	
	FPGA_SPI3Write(ADDR_READ_FINISH);
	FPGA_SPI3Write(1);  //FPGA start to sent data to ARM
	Delay_ms(1); //wait some time for FPGA to get ready
	rdata1 = FPGA_SPI3Read(); //ARM read data
	
	FPGA_SPI3Write(ADDR_READ_FINISH);
	FPGA_SPI3Write(0);	//clear flag for next read
	while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA finish read data from MIPI slave	
	FPGA_SPI3Write(ADDR_READ_FINISH);
	FPGA_SPI3Write(1);  //FPGA start to sent data to ARM
	Delay_ms(1); //wait some time for FPGA to get ready
	rdata = FPGA_SPI3Read(); //ARM read data
	
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN);
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);
	
	FPGA_SPI3Write(ADDR_OP_TYPE); //exit read state
	FPGA_SPI3Write(0x00);	
	FPGA_SPI3Write(ADDR_READ_FINISH);
	FPGA_SPI3Write(0);	//clear flag for next read
	
	return ((rdata << 8) + rdata1);
	
#else
	 
	 uint16_t rdata;
	 
	 WriteCMD2SSD(port, 0xD4);
	 WriteDATA2SSD(port, 0xFA);	// set SPI read address
	 WriteDATA2SSD(port, 0x00);
	 WriteCMD2SSD(port, reg);	// register to be read
	 WriteCMD2SSD(port, 0xFA);	// write data buffer address for read data 
	 rdata = SPI3Read(port);
	 
	 return rdata;
	 
#endif
}					  

/*********************************************************************************
* Function: WriteDriverReg
* Description: write the DDIC register by MIPI
* Input: -port, MIPI port to be write
* Input: -reg, DDIC register to be set
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
*/
 void WriteDriverReg(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
 {
	uint8_t wrtCnt = 0;
	 
	WriteSSDReg(port, 0xBC, (nPara + 1)); 	
	WriteCMD2SSD(port, 0xBF);
	WriteDATA2SSD(port, reg);	//DDIC register to be set
	while (wrtCnt < nPara)
	{
		 WriteDATA2SSD(port, *(para + wrtCnt));  // parameter
		 wrtCnt++;
	}
 }

 /*********************************************************************************
* Function: ReadDriverReg
* Description: read the DDIC register by MIPI
* Input: -port, MIPI port to be read
* Input: -reg, DDIC register to be read
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
* --------------------------------------------------------------------------------
*						MIPI(SSD_RR)				  SPI(SSD_RRA)
*		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
* --------------------------------------------------------------------------------
*/
void ReadDriverReg(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
{
 	uint16_t rdDataTemp;
	uint8_t rdCnt = 0;

  do
	{
		WriteSSDReg(port, SSD_CFGR, SSD_B7);
		WriteSSDReg(port, 0xC1, (nPara & 0x00FF));
		WriteSSDReg(port, 0xBC, 0x0001);
		WriteSSDReg(port, 0xBF, (reg & 0x00FF));
		Delay_ms(50);	 //	wait for MIPI read
		rdDataTemp = ReadSSDReg(port, 0xC6);
		printf("The register 0xC6 of SSD2828 is: 0x%04X\r\n", rdDataTemp);
		rdCnt++;
	} while (((rdDataTemp & 0x0001) ==  0x0000) && (rdCnt < 20));

	rdDataTemp = ReadSSDReg(port, 0xC2);
	printf("The data lenth of returning from driver IC is: 0x%04X\r\n", rdDataTemp);

	printf("The parameters of driver IC register 0x%02X is: ", reg);
  rdCnt = 0;

#ifdef CMD_MODE 

	WriteCMD2SSD(port, 0xFF);	// register to be read
	
	switch (port)
	{
		case (PORT0): GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); break;
		case (PORT1): GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);break;
		default: GPIO_ResetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
						GPIO_ResetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN); break;
	}
	
	FPGA_SPI3Write(ADDR_OP_TYPE);//enter read DDIC state
	FPGA_SPI3Write(0xBB);

	while (rdCnt < nPara)
	{
		FPGA_SPI3Write(ADDR_READ_FINISH);
		FPGA_SPI3Write(0);	//clear flag for next read
		while (GPIO_ReadInputDataBit(FPGA_MCU_CSX_GPIO_PORT, FPGA_MCU_CSX_PIN) == (uint8_t)Bit_RESET);//wait until FPGA finish read data from MIPI slave	
		FPGA_SPI3Write(ADDR_READ_FINISH);
		FPGA_SPI3Write(1);  //FPGA start to sent data to ARM
		Delay_ms(1); //wait some time for FPGA to get ready
		rdDataTemp = FPGA_SPI3Read(); //ARM read data
		*(para + rdCnt) = rdDataTemp & 0x00FF;
		rdCnt++;
	}	
	
	GPIO_SetBits(SSD2828_SPI_nCS0_GPIO_PORT, SSD2828_SPI_nCS0_PIN); 
	GPIO_SetBits(SSD2828_SPI_nCS1_GPIO_PORT, SSD2828_SPI_nCS1_PIN);

	FPGA_SPI3Write(ADDR_OP_TYPE); //exit read DDIC state
	FPGA_SPI3Write(0x00);
	FPGA_SPI3Write(ADDR_READ_FINISH);
	FPGA_SPI3Write(0);	//clear flag for next read

	rdCnt = 0;
	while (rdCnt < nPara)
	{
		printf("0x%02X\t", *(para + rdCnt));
		rdCnt++;
	}
	printf("\r\n");
	
	/////////////////////////////////////////////////////////////////////////////
	// cover command mode read bug
	rdDataTemp = ReadSSDReg(MAIN_PORT, SSD_CFGR); //backup B7 status
	MIPI_Reset();
	IC_Init(SSDInitCode);
	SSD_B7 = rdDataTemp;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7); //reset B7 status
	//////////////////////////////////////////////////////////////////////////////
	
#else
	
	WriteCMD2SSD(port, 0xFF);	  // MIPI read data buffer address
	while (rdCnt < nPara)
	{
		WriteCMD2SSD(port, 0xFA);	// SPI read address	
		rdDataTemp = SPI3Read(port); 
		*(para + rdCnt) = rdDataTemp & 0x00FF;
		rdCnt++;
		*(para + rdCnt) = (rdDataTemp >> 8) & 0x00FF;
		rdCnt++;		
	}
		
	rdCnt = 0;
	while (rdCnt < nPara)
	{
		printf("0x%02X\t", *(para + rdCnt));
		rdCnt++;
	}
	printf("\r\n");

#endif	
}

/*********************************************************************************
* Function: MIPI_DCS_Write
* Description: write the DDIC register by MIPI
* Input: -port, MIPI port to be write
* Input: -reg, register to be set
* Input: -nPara, parameter number
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
*/
 void MIPI_DCS_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
 {
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg(port, SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(port, SSD_CFGR);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteDriverReg(port, reg, nPara, para);
 }

/*********************************************************************************
* Function: MIPI_GEN_Write
* Description: write the DDIC register by MIPI
* Input: -port, MIPI port to be write
* Input: -reg, register to be set
* Input: -nPara, parameter number
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
*/
 void MIPI_GEN_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
 {
	SSD_B7 &= ~SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg(port, SSD_CFGR, SSD_B7);
	SSD_B7 = ReadSSDReg(port, SSD_CFGR);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);
		
	WriteDriverReg(port, reg, nPara, para);
 }

/*********************************************************************************
* Function: MIPI_DCS_Read
* Description: read the DDIC register by MIPI
* Input: -port, MIPI port to be read
* Input: -reg, register to be read
* Input: -nPara, parameter number
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
* --------------------------------------------------------------------------------
*						MIPI(SSD_RR)				  SPI(SSD_RRA)
*		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
* --------------------------------------------------------------------------------
*/
 void MIPI_DCS_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
 {
	SSD_B7 |= (SSD_CFGR_DCS | SSD_CFGR_REN);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	ReadDriverReg(port, reg, nPara, para);			
 }

/*********************************************************************************
* Function: MIPI_DCS_Read
* Description: read the DDIC register by MIPI
* Input: -port, MIPI port to be read
* Input: -reg, register to be read
* Input: -nPara, parameter number
* Input: -para, parameter
* Output: none
* Return: none
* Call: external
* --------------------------------------------------------------------------------
*						MIPI(SSD_RR)				  SPI(SSD_RRA)
*		| Driver IC | <<------------>>  | SSD2828 | <<------------>> | STM32 |
* --------------------------------------------------------------------------------
*/
 void MIPI_GEN_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para)
 {
	SSD_B7 &= ~SSD_CFGR_DCS;
	SSD_B7 |= SSD_CFGR_REN;
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	ReadDriverReg(port, reg, nPara, para);	
 }

/*********************************************************************************
* Function: SSD2828_Init
* Description: SSD2828 initial
* Input: none
* Output: none
* Return: none
* Call: external
*/
void SSD2828_Init(void)
{		
	MIPI_Reset();
	printf("\r\nSSD2828 initial...\r\n");
	IC_Init(SSDInitCode);		
	Debug_Printf("SSD_B1 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB1));
	Debug_Printf("SSD_B2 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB2));
	Debug_Printf("SSD_B3 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB3));
	Debug_Printf("SSD_B4 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB4));
	Debug_Printf("SSD_B5 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB5));
	Debug_Printf("SSD_B6 = 0x%04X\r\n", ReadSSDReg(MAIN_PORT, 0xB6));
}
