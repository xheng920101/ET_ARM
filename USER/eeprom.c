#include "include.h"

#define	EEPROM_I2C_ADDR	 0xA0
#define	EEPROM_PAGE		8

/*********************************************************************************
* Function: EEPROM_I2C_Config
* Description: EEPROM I2C interface configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void EEPROM_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_SetBits(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_PIN);
	
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_PIN);

	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_WP_PIN;
	GPIO_Init(EEPROM_I2C_WP_GPIO_PORT, &GPIO_InitStructure);
  GPIO_SetBits(EEPROM_I2C_WP_GPIO_PORT, EEPROM_I2C_WP_PIN);
}

/*********************************************************************************
* Function: EEPROM_I2C_Write
* Description: write several datas to eeprom
* Input: -Addr, start address to be write
* Input: -Data, point to the data to be write
* Input: -Data_Len, how many data to be write
* Output: none
* Return: write result, 0 is success, 1 is fail
* Call: external
*/
FlagStatus EEPROM_I2C_Write(uint16_t Addr, uint8_t *Data, uint8_t Data_Len)
{
	uint8_t Write_Len;
	uint8_t Curr_Len = Data_Len;
	uint8_t *Curr_Data = Data;
	uint16_t Curr_Addr = Addr; 
	
	I2C_PORT_SDA = EEPROM_I2C_SDA_GPIO_PORT;
	I2C_PIN_SDA = EEPROM_I2C_SDA_PIN;
	I2C_PORT_SCL = EEPROM_I2C_SCL_GPIO_PORT;
	I2C_PIN_SCL = EEPROM_I2C_SCL_PIN;
	I2C_ADDR = EEPROM_I2C_ADDR;
	
	Write_Len = (Addr / EEPROM_PAGE + 1) * EEPROM_PAGE - Addr;
	if (Data_Len < Write_Len)
	{
		Write_Len = Data_Len;
	}
	
	GPIO_ResetBits(EEPROM_I2C_WP_GPIO_PORT, EEPROM_I2C_WP_PIN); //write protect disable
	
	while (Curr_Len)
	{
		Delay_ms(2); // wait for I2C bus idle
		if (I2C_Sequential_Write16(Curr_Addr, Curr_Data, Write_Len) == SET)
		{
			printf("EEPROM write fail!\r\n");
			printf("Curr_Len = %d\r\n", Curr_Len);
			GPIO_SetBits(EEPROM_I2C_WP_GPIO_PORT, EEPROM_I2C_WP_PIN);
			return SET;
		}		
		
		Curr_Addr = Curr_Addr + Write_Len;
		Curr_Data = Curr_Data + Write_Len;
		Curr_Len = Curr_Len - Write_Len;
		if (Curr_Len > EEPROM_PAGE)
		{
			Write_Len = EEPROM_PAGE;
		}
		else
		{
			Write_Len = Curr_Len;
		}		
	}	
	
	GPIO_SetBits(EEPROM_I2C_WP_GPIO_PORT, EEPROM_I2C_WP_PIN);
	Delay_ms(2);	// wait for I2C bus idle
	
	return RESET;
}

/*********************************************************************************
* Function: EEPROM_I2C_Read
* Description: read several datas to eeprom
* Input: -Addr, start address to be read
* Input: -Data, buffer pointer to save read data
* Input: -Data_Len, how many data to be read
* Output: none
* Return: read result, 0 is success, 1 is fail
* Call: external
*/
FlagStatus EEPROM_I2C_Read(uint16_t Addr, uint8_t *Data, uint8_t Data_Len)
{
	I2C_PORT_SDA = EEPROM_I2C_SDA_GPIO_PORT;
	I2C_PIN_SDA = EEPROM_I2C_SDA_PIN;
	I2C_PORT_SCL = EEPROM_I2C_SCL_GPIO_PORT;
	I2C_PIN_SCL = EEPROM_I2C_SCL_PIN;
	I2C_ADDR = EEPROM_I2C_ADDR;
	
	if (I2C_Sequential_Read16(Addr, Data, Data_Len) == SET)
	{
		printf("EEPROM read fail!\r\n");
		return SET;
	}
	
	return RESET;
}

/*********************************************************************************
* Function: EEPROM_WR_Verify
* Description: EEPROM read and wtrite test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void EEPROM_WR_Verify(void)
{
	uint8_t wdata[] = {1, 0, 2, 3, 4, 55, 6, 77, 8, 99, 23, 100};
	uint8_t rdata[12];
	uint8_t i;
	
	EEPROM_I2C_Write(0x0000, wdata, 12);
	EEPROM_I2C_Write(0x0007, wdata, 12);
	EEPROM_I2C_Read(0x0000, rdata, 12);
	EEPROM_I2C_Read(0x0007, rdata, 12);
	EEPROM_I2C_Write(0x0008, wdata, 12);
	EEPROM_I2C_Read(0x0008, rdata, 12);
	EEPROM_I2C_Read(0x0009, rdata, 1);
	
	for (i = 0; i < 12; i++)
	{
		printf("%d ", rdata[i]);
	}
	printf("\r\n");
}

/*********************************************************************************
* Function: Get_Test_Mode
* Description: Get test mode
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Get_Test_Mode(void)
{
	EEPROM_I2C_Read(0x0000, &TEST_MODE, 1);
	
//	switch (Test_Mode)
//	{
//		case (TEST_MODE_ET1): 
//			Init_Code = ET1_InitCode;
//		  printf("\r\nTest mode is ET1\r\n");
//			break;
//		case (TEST_MODE_ET2):
//			Init_Code = ET2_InitCode;
//			printf("\r\nTest mode is ET2\r\n");
//			break;
//		case (TEST_MODE_ET3): 
//			Init_Code = ET2_InitCode;
//		  printf("\r\nTest mode is ET3\r\n");
//			break;
//		case (TEST_MODE_OTP):
//			Init_Code = ET1_InitCode;
//			printf("\r\nTest mode is OTP\r\n");
//			break;
//		case (TEST_MODE_RA):
//			Init_Code = RA_InitCode;
//			printf("\r\nTest mode is RA\r\n");
//			break;
//		case (TEST_MODE_ESD):
//			Init_Code = ET2_InitCode;
//			printf("\r\nTest mode is ESD\r\n");
//			break;
//		case (TEST_MODE_OD):
//			Init_Code = ET2_InitCode;
//			printf("\r\nTest mode is OD\r\n");
//			break;
//		case (TEST_MODE_DEMO):
//			Init_Code = ET2_InitCode;
//			printf("\r\nTest mode is DEMO\r\n");
//			break;
//		case (TEST_MODE_FTP):
//			Init_Code = ET2_InitCode;
//			printf("\r\nTest mode is FTP\r\n");
//			break;
//		default:
//			Test_Mode = TEST_MODE_DEBUG;
//			Init_Code = ET1_InitCode;
//		  printf("\r\nTest mode is DEBUG\r\n");
//			break;
//	}
}

/*********************************************************************************
* Function: Set_Test_Mode
* Description: Set test mode
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Set_Test_Mode(void)
{
	EEPROM_I2C_Write(0x0000, &TEST_MODE, 1);
}

/*********************************************************************************
* Function: Disp_Test_Mode
* Description: display current test mode on the panel
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Disp_Test_Mode(void)
{
	FPGA_Info_Visible(INFO_STR);
	switch (TEST_MODE)
	{
		case (TEST_MODE_ET1): 
			FPGA_Info_Set((uint8_t *)"ET1");
		  printf("Test mode is ET1\r\n");
			break;
		case (TEST_MODE_ET2):
			FPGA_Info_Set((uint8_t *)"ET2");
			printf("Test mode is ET2\r\n");
			break;
		case (TEST_MODE_ET3): 
			FPGA_Info_Set((uint8_t *)"ET3");
		  printf("Test mode is ET3\r\n");
			break;
		case (TEST_MODE_OTP):
			FPGA_Info_Set((uint8_t *)"OTP");
			printf("Test mode is OTP\r\n");
			break;
		case (TEST_MODE_RA):
			FPGA_Info_Set((uint8_t *)"RA");
			printf("Test mode is RA\r\n");
			break;
		case (TEST_MODE_ESD):
			FPGA_Info_Set((uint8_t *)"ESD");
			printf("Test mode is ESD\r\n");
			break;
		case (TEST_MODE_OD):
			FPGA_Info_Set((uint8_t *)"OD");
			printf("Test mode is OD\r\n");
			break;
		case (TEST_MODE_DEMO):
			FPGA_Info_Set((uint8_t *)"DEMO");
			printf("Test mode is DEMO\r\n");
			break;	
		default:
			FPGA_Info_Set((uint8_t *)"DEBUG");
		  printf("Test mode is DEBUG\r\n");
			break;
	}
}
