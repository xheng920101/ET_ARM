#include "include.h"

/*********************************************************************************
 * Interface definition for all IC, not to change!
 */
#define    TCH_SPI_CSN1_PIN					GPIO_Pin_9		//TEST22: TP_SDA --> SPI_CSN
#define    TCH_SPI_CSN1_GPIO_PORT		GPIOB

#define    TCH_SPI_SCK_PIN					GPIO_Pin_8		//TEST23: TP_SCL --> SPI_SCK
#define    TCH_SPI_SCK_GPIO_PORT		GPIOB

#define    TCH_SPI_MISO_PIN					GPIO_Pin_6		//TEST19: POWER_I2C_SCL --> SPI_MISO
#define    TCH_SPI_MISO_GPIO_PORT		GPIOB

#define    TCH_SPI_MOSI_PIN					GPIO_Pin_5		// TEST18: POWER_I2C_SDA --> SPI_MOSI
#define    TCH_SPI_MOSI_GPIO_PORT		GPIOB

#define    TCH_SPI_CSN2_PIN					GPIO_Pin_7		//TEST20: TP_INT --> SPI_CSN
#define    TCH_SPI_CSN2_GPIO_PORT		GPIOB

#define    TCH_SPI_CSN3_PIN					GPIO_Pin_2		//TEST21: TP_SCL --> SPI_CSN
#define    TCH_SPI_CSN3_GPIO_PORT		GPIOC

#define    TCH_RST_PIN							GPIO_Pin_3		//TEST24: TP_RST --> TP_RST
#define    TCH_RST_GPIO_PORT				GPIOC

//YWB
#define    TCH_SPI_CSN1_2_PIN				GPIO_Pin_4	
#define    TCH_SPI_CSN1_2_GPIO_PORT	GPIOE

#define    TCH_SPI_CSN2_2_PIN				GPIO_Pin_5		
#define    TCH_SPI_CSN2_2_GPIO_PORT	GPIOE

#define    TCH_SPI_CSN3_2_PIN				GPIO_Pin_6		
#define    TCH_SPI_CSN3_2_GPIO_PORT	GPIOE

#define    TCH_SPI_CSN1_3_PIN				GPIO_Pin_6	
#define    TCH_SPI_CSN1_3_GPIO_PORT	GPIOF

#define    TCH_SPI_CSN2_3_PIN				GPIO_Pin_7		
#define    TCH_SPI_CSN2_3_GPIO_PORT	GPIOF

#define    TCH_SPI_CSN3_3_PIN				GPIO_Pin_8		
#define    TCH_SPI_CSN3_3_GPIO_PORT	GPIOF

uint16_t				TCH_SPI_CSN_PIN				=	TCH_SPI_CSN1_PIN;
GPIO_TypeDef *	TCH_SPI_CSN_GPIO_PORT	=	TCH_SPI_CSN1_GPIO_PORT;
/* 
 * End of interface definition
*********************************************************************************/

/*********************************************************************************
 * Parameters definition for FT8719
 */
#define T			10	//FT BYTE send interval, unit is us
#define t			35 //FT package send inteval, unit is us
#define ROMBOOT_ID	0x8719

/*********************************************************************************
 * Parameters definition for SD FW file
 */
FIL fwfile; 
DIR filedir;
uint32_t fileSize;
char FWVersion[12] = "FW";
char FW_project[14];
/* 
 * End of parameters definition
*********************************************************************************/

/*********************************************************************************
* Function: SD_FW_Open
* Description: Check the FW file, then open it
* Input: none
* Output: none
* Return: FW file open result
* Call: internal
*/
ErrorStatus SD_FW_OPEN(void)
{ 
	char file_name[30];
	TCHAR *path = (TCHAR*)"0:\\";
	FATFS filefs;
	FRESULT frs;
	static FILINFO fileInfo;
	uint8_t cntFile = 0;
	uint8_t k = 0;
	
	while (k < 14)
	{
		FW_project[k] = PROJECT_NO[k];
		k++;
	}
	printf("FW_project No is %s\r\n", FW_project);

	sprintf(file_name, "%s*.bin", FW_project);
	frs = f_mount(&filefs, path, 0);
	frs = f_opendir(&filedir,path);
	frs =	f_findfirst(&filedir, &fileInfo, path, (TCHAR*)file_name); //查询第一个匹配条件的文件
	for ( ; ; )
	{
		frs = f_findnext(&filedir, &fileInfo);
		if (frs != FR_OK || fileInfo.fname[0] == 0) break;
		cntFile++ ;
		printf("%s\r\n", fileInfo.fname);
	}
	
	frs =	f_findfirst(&filedir, &fileInfo, path, (TCHAR*)file_name); //查询第一个匹配条件的文件
	if (cntFile != 0)
	{
		printf("There are %d FW files match the project %s\r\n", (cntFile+1), FW_project);
		return ERROR;
	}
	else if (frs == FR_OK)
	{		 
		printf("The FW File name: %s\r\n", file_name);
		fileSize = fileInfo.fsize;
		printf("The FW File size: %d bytes\r\n", fileSize);
		cntFile = strlen(FW_project) + strlen(Info_IC_Type) + 4;
		strncpy(&FWVersion[2], &fileInfo.fname[cntFile], 5);
		printf("The FW File version: %s\r\n", FWVersion);
		if (f_open(&fwfile, (TCHAR *)fileInfo.fname, FA_OPEN_EXISTING | FA_READ) == FR_OK)
		{
			f_lseek(&fwfile, fileInfo.fsize); //important
			printf("The FW File has open!\r\n");
			return SUCCESS;			
		}
		else
		{
			printf("The FW file open fail!\r\n");
			return ERROR;
		}
	}	
	else
	{
		printf("There is no FW file match the project %s\r\n", FW_project);
		return ERROR;
	}
}

/*********************************************************************************
* Function: SD_FW_Read
* Description: 
* Input: -pbuf, pointer of data buffer to save data read back
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void SD_FW_Read(uint8_t * pbuf, UINT Len)
{	
	UINT* br;
	f_read(&fwfile, pbuf, Len, br);
}

/*********************************************************************************
* Function: SD_FW_Close
* Description: 
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void SD_FW_Close(void)
{	
	f_close(&fwfile);
	f_closedir(&filedir);
}

/*********************************************************************************
* Function: TCH_SPI_Config
* Description: TCH SPI interface configure
* Input: none
* Output: none
* Return: none
* Call: internal
*/
static void TCH_SPI_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_InitStructure.GPIO_Pin = TCH_RST_PIN; //TEST24 TP_RST
	GPIO_Init(TCH_RST_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_RST_GPIO_PORT, TCH_RST_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN_PIN;
	GPIO_Init(TCH_SPI_CSN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT,TCH_SPI_CSN_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_SCK_PIN;
	GPIO_Init(TCH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT,TCH_SPI_SCK_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MOSI_PIN;
	GPIO_Init(TCH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);	
	
	GPIO_InitStructure.GPIO_Mode  =	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType =	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin		= TCH_SPI_MISO_PIN ;
	GPIO_Init(TCH_SPI_MISO_GPIO_PORT , &GPIO_InitStructure);
}

/*********************************************************************************
* Function: TCH_SPI_UNConfig
* Description: TCH SPI interface recovery to original settings
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void TCH_SPI_UNConfig(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN1_PIN;
	GPIO_Init(TCH_SPI_CSN1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN1_GPIO_PORT, TCH_SPI_CSN1_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN2_PIN;
	GPIO_Init(TCH_SPI_CSN2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN2_GPIO_PORT, TCH_SPI_CSN2_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN3_PIN;
	GPIO_Init(TCH_SPI_CSN3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN3_GPIO_PORT, TCH_SPI_CSN3_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN1_2_PIN;
	GPIO_Init(TCH_SPI_CSN1_2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN1_2_GPIO_PORT, TCH_SPI_CSN1_2_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN2_2_PIN;
	GPIO_Init(TCH_SPI_CSN2_2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN2_2_GPIO_PORT, TCH_SPI_CSN2_2_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN3_2_PIN;
	GPIO_Init(TCH_SPI_CSN3_2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN3_2_GPIO_PORT, TCH_SPI_CSN3_2_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN1_3_PIN;
	GPIO_Init(TCH_SPI_CSN1_3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN1_3_GPIO_PORT, TCH_SPI_CSN1_3_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN2_3_PIN;
	GPIO_Init(TCH_SPI_CSN2_3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN2_3_GPIO_PORT, TCH_SPI_CSN2_3_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN3_3_PIN;
	GPIO_Init(TCH_SPI_CSN3_3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN3_3_GPIO_PORT, TCH_SPI_CSN3_3_PIN);
}

/*********************************************************************************
* Function: TCH_SPI_WriteByte
* Description: IO simiulate SPI write
* Input: -dataWrt, pointer of data buffer that save the data to be write
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void TCH_SPI_WriteByte(uint8_t* dataWrt, uint16_t len)
{    
	uint16_t i; 
	uint8_t bitPos;

	for (i=0; i<len; i++)
	{ 
		bitPos = 0x80;
		while (bitPos)
		{
			GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);    
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
			if (dataWrt[i] & bitPos)
			{
				GPIO_SetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
			else
			{
				GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);		
			bitPos >>= 1;
//			Delay_us(2); //bit delay
		}	 
		Delay_us(1); //BYTE delay
		GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); //SCK last low
	}	
}

/*********************************************************************************
* Function:  FLASH_SPI_ReadByte  
* Description: IO simiulate SPI read
* Input: -len, data length, range is 0~65535
* Output: -dataRdbuf, pointer of data buffer to save data read back
* Return: node
* Call: internal
*/
void TCH_SPI_ReadByte(uint8_t* dataRdbuf, uint16_t len) 
{
	uint16_t i; 
	uint8_t j;
	uint8_t bitPos;
	uint8_t dataRd = 0x00;		
	
  for (i=0; i<len; i++)
  {
		bitPos = 0x80;  	
		while (bitPos)
		{
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);	
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
			GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); 		
			if (GPIO_ReadInputDataBit(TCH_SPI_MISO_GPIO_PORT,TCH_SPI_MISO_PIN) == SET)
			{
				dataRd |= bitPos;	
			}	
			else
			{
				dataRd |= 0x00;
			} 	
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
			bitPos >>= 1;
		} 
		GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); 		// SCK last low
		dataRdbuf[i] = dataRd;  
		dataRd = 0x00;	
		if(TEST_MODE != TEST_MODE_RA){
			Delay_us(10);
		}//ET delay
		else{
			Delay_ms(1);
		}//RA delay
	}	
}

/*********************************************************************************
* Function: FT_Check_Status
* Description: 
* Input: none
* Output: none
* Return: status
* Call: internal
*/
uint8_t FT_Check_Status(void) 
{ 
	uint8_t buf = 0x05;
	
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf, 1);
	TCH_SPI_ReadByte(&buf, 1);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
		
	return (buf);
}

/*********************************************************************************
* Function: FT_CMD_Write
* Description: 
* Input: -CMD
* Input: -para
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void FT_CMD_Write(uint8_t WRCL, uint8_t CMD, uint8_t* para, uint16_t len) 
{
	uint8_t buf; 

	while ((FT_Check_Status() & 0x80) == 0x80);
	
	buf = 0xC0;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf, 1);
	Delay_us(T);
	
	buf = WRCL;
	TCH_SPI_WriteByte(&buf, 1);
	buf = CMD;
	TCH_SPI_WriteByte(&buf, 1);
	TCH_SPI_WriteByte(para, len);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
}

/*********************************************************************************
* Function: FT_DATA_Write
* Description: 
* Input: -data
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void FT_DATA_Write(uint8_t* data, uint16_t len) 
{
	uint8_t buf; 

	while ((FT_Check_Status() & 0x80) == 0x80);
	
	buf = 0x3F;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf, 1);
	Delay_us(T);
	TCH_SPI_WriteByte(data, len);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
}

/*********************************************************************************
* Function: FT_DATA_Read
* Description: 
* Input: -data
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void FT_DATA_Read(uint8_t* data, uint16_t len) 
{
	uint8_t buf; 

	while ((FT_Check_Status() & 0x80) == 0x80);
	
	buf = 0x3F;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf, 1);
	Delay_us(T);
	TCH_SPI_ReadByte(data, len);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
}

/*********************************************************************************
* Function: FT_FWReg_Write
* Description: 
* Input: -data
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void FT_FWReg_Write(uint8_t CMD, uint16_t len, uint8_t *pdata) 
{
	uint8_t buf[6]; 

	while ((FT_Check_Status() & 0x80) == 0x80);
	buf[0]= 0xC0;
	buf[1]= 0x03;
	buf[2]= CMD;
	buf[3]= len >> 8;
	buf[4]= len;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf[0], 1);
	Delay_us(T);
	TCH_SPI_WriteByte(&buf[1], 4);

	while ((FT_Check_Status() & 0x80) == 0x80);
	FT_DATA_Write(pdata, len);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
}

/*********************************************************************************
* Function: FT_FWReg_Read
* Description: 
* Input: -data
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal
*/
void FT_FWReg_Read(uint8_t CMD, uint16_t len, uint8_t *pdata) 
{
	uint8_t buf[6]; 

	while ((FT_Check_Status() & 0x80) == 0x80);
	buf[0]= 0xC0;
	buf[1]= 0x83;
	buf[2]= CMD;
	buf[3]= len >> 8;
	buf[4]= len;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(&buf[0], 1);
	Delay_us(T);
	TCH_SPI_WriteByte(&buf[1], 4);

	while ((FT_Check_Status() & 0x80) == 0x80);
	FT_DATA_Read(pdata, len);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(t);
}

/*********************************************************************************
* Function: FT_IDC_HWRST
* Description: reset IDC
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void FT_IDC_HWRST(void) 
{
	GPIO_ResetBits(TCH_RST_GPIO_PORT, TCH_RST_PIN);
	Delay_ms(8);
	GPIO_SetBits(TCH_RST_GPIO_PORT, TCH_RST_PIN);
}

/*********************************************************************************
* Function: FT_IDC_SWRST
* Description: reset IDC
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void FT_IDC_SWRST(void) 
{
	uint8_t para;
	
	para = 0xAA;
	FT_CMD_Write(0x02, 0xFC, &para, 1);
	Delay_ms(10);
	para = 0x55;
	FT_CMD_Write(0x02, 0xFC, &para, 1);
	Delay_ms(10);	
}

/*********************************************************************************
* Function: FT_BOOT_ID
* Description: Read ROMBOOT ID
* Input: none
* Output: none
* Return: BOOT ID
* Call: internal
*/
ErrorStatus FT_BOOT_ID(void) 
{
	uint8_t buf[2];
	uint16_t ID;
	uint8_t retry = 0;
	
	FT_CMD_Write(0x81, 0x90, buf, 0);
	FT_DATA_Read(buf, 2);
	ID = ((buf[0] << 8) + buf[1]);
	printf("read FT_BOOT_ID: 0x%04X\r\n", ID);
	while (ID != ROMBOOT_ID)
  {
		printf("ROMBOOT ID read wrong ,is not 0x%04X\r\n", ROMBOOT_ID);	
		retry++;
		if (retry > 3)
		{
			return ERROR;
		}
	}
	return SUCCESS;	
}

/*********************************************************************************
* Function: FT_BOOT_ECC
* Description: BOOT calculate ECC
* Input: none
* Output: none
* Return: BOOT ECC
* Call: internal
*/
uint16_t FT_BOOT_ECC(void) 
{
	uint8_t buf[6];
	uint16_t ECC;
	
	buf[0] = 0x00; //ADD_H
	buf[1] = 0x00; //ADD_M
	buf[2] = 0x00; //ADD_L
	buf[3] = fileSize >> 16; //LEN_H
	buf[4] = fileSize >> 8; //LEN_M
	buf[5] = fileSize; //LEN_L
	FT_CMD_Write(0x07, 0xCC, buf, 6);
	Delay_ms(3);//wait for ECC calculate finish
	while (1) //wait for ECC calculate finish
	{
		FT_CMD_Write(0x81, 0xCE, buf, 0);
		FT_DATA_Read(buf, 1);
		if (buf[0] == 0) break;
	}
	
	FT_CMD_Write(0x81, 0xCD, buf, 0);
	FT_DATA_Read(buf, 2);
	ECC = (buf[0] << 8) + buf[1];
	
	return ECC;
}

/*********************************************************************************
* Function: FT_BIN_ECC
* Description: calculate FW.bin ECC
* Input: none
* Output: none
* Return: FW.bin ECC
* Call: internal
*/
uint16_t FT_BIN_ECC(uint16_t oldECC, uint8_t* data, uint16_t dataLen) 
{
	uint16_t i, j;
	uint16_t temp;
	uint16_t newECC = oldECC;
	uint16_t AL2_FCS_COEF = ((1 << 15) + (1 << 10) + (1 << 3));
	
	for (i=0; i < dataLen; i+=2)
	{
		temp = (data[i] << 8) + data[i+1];
		newECC ^= temp;
		for (j=0; j<16; j++)
		{
			if (newECC & 0x01)
			{
				newECC = (uint16_t)((newECC >> 1) ^ AL2_FCS_COEF);
			}
			else
			{
				newECC >>= 1;
			}
		}
	}	
	
	return newECC;
}

/*********************************************************************************
* Function: 
* Description: WriteSram
* Input: uint8_t* fwCeil, uint32_t start_addr, uint32_t Len
* Output: none
* Return: none
* Call: Internal
*/
static void	WriteSram(uint8_t* fwCeil, uint32_t start_addr, uint32_t Len)
{   
	uint8_t buf[6];
	buf[0] = start_addr >> 16; //ADD_H
	buf[1] = start_addr >> 8; //ADD_M
	buf[2] = start_addr; //ADD_L
	buf[3] = Len >> 8; //LEN_H
	buf[4] = Len; //LEN_L
	FT_CMD_Write(0x06, 0xAE, buf, 5);
	FT_DATA_Write(fwCeil, Len);
}

/*********************************************************************************
* Function: FT_FW_Write
* Description: write FW.bin to PRAM
* Input: none
* Output: -M_ECC, ECC of FW.bin
* Return: PRAM write result
* Call: internal
*/
ErrorStatus FT_PRAM_Write(uint16_t* M_ECC) 
{
	uint32_t BIN_addr, SRAM_addr, size;
	uint16_t len = 0;
	int32_t count = 0;
	uint32_t i = 0;
	uint8_t FWContent[1024];
	ErrorStatus ret = SUCCESS;
     
	if (SD_FW_OPEN() == ERROR)
	{
		printf("FW file open error!\r\n");
		return ERROR;
	}
	
	SRAM_addr = 0x00; //sram start address
	size = fileSize;
	BIN_addr =0;
	*M_ECC = 0;
	if (fileSize % 1024)
	{
		count = (size / 1024) + 1;
	}
	else 
	{
		count = (size / 1024);
	}
	
	for (i=0 ; i <count ; i++)
  {
		len = (size < 1024) ? size : 1024;
		/*write data to SRAM */
		if (f_lseek(&fwfile, BIN_addr) == FR_OK)
    {			 
			SD_FW_Read(FWContent, len);
			WriteSram(FWContent, SRAM_addr, len);
			*M_ECC = FT_BIN_ECC(*M_ECC, FWContent, len);
			SRAM_addr += 1024;
			BIN_addr += 1024;
			size -= 1024;
		}
		else
		{
			printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
			ret = ERROR;
		}
	}
	
	SD_FW_Close();
	return ret;
}

/*********************************************************************************
* Function: Program_FW 
* Description: FW Upgrate
* Input: none
* Output: none
* Return: FW upgrate result
* Call: external
*/
ErrorStatus Program_FW(void)
{
	ErrorStatus ret = SUCCESS;
#ifdef NO_FLASH_MODE
	uint8_t buf[6];
	uint8_t retry = 0;
	uint8_t i = 0;
	uint16_t M_ECC, ECC;

	printf("\r\n");
	TCH_SPI_Config();
	while (1)
	{
		retry++;
    if (retry > 3)
    {
			printf("error, retry = %d\r\n", retry);
			break;
    }
		
		/* step1: IDC reset */
    FT_IDC_HWRST(); //SW reset only effective when PRAM having been programed
    
    /* step2: ROMBOOT enter upgrade mode */
    Delay_ms(10); //SW:8~25ms, HW:5~25ms
    FT_CMD_Write(0x01, 0x55, buf, 0);
    
    /* step3: read ROMBOOT ID */ 
		Delay_ms(8); //Dealy 8ms
		if (TEST_MODE != TEST_MODE_RA && (FT_BOOT_ID() == ERROR))
		{
			ret = ERROR;
			continue;
		} 
		
		/* step4: write/read FW and calculate ECC of FW.bin */
		if (FT_PRAM_Write(&M_ECC) == ERROR)
		{
			ret = ERROR;
			continue;
		}		
		
		/* step5: check ECC */
		if (TEST_MODE != TEST_MODE_RA)
		{
			ECC = FT_BOOT_ECC();
			while (!ECC)
			{ 
				ECC = FT_BOOT_ECC();
				printf("ECC = 0x%04X\r\n", ECC);	
				i++;
				if (i > 30)
				break;
			}	
			if (M_ECC == ECC)
			{
				printf("M_ECC = 0x%04X, ECC = 0x%04X\r\n", M_ECC, ECC);	
			}
			else
			{
				ret = ERROR;
				printf("M_ECC(0x%04X) != ECC(0x%04X)\r\n", M_ECC, ECC);
				continue;
			}
		}
    /* step6: start ROMBOOT upgrade */
    FT_CMD_Write(0x01, 0x08, buf, 0);
    Delay_ms(2);
		printf("ROMBOOT upgrade finish!\r\n");		
    /* step7: read FW Vertion */
    FT_FWReg_Read(0xA6, 1, buf);
    printf("FW version = 0x%02X\r\n", buf[0]);
	  Delay_ms(2); 
		ret = SUCCESS;
	  break; 
	}
	printf("\r\n");
#endif	
	return ret;
}

/*********************************************************************************
* Function: RA Program_FW 
* Description: RA FW Upgrate
* Input: none
* Output: none
* Return: FW upgrate result
* Call: external
*/
ErrorStatus RA_Program_FW(void){
	uint8_t i=0;
	uint8_t k=0;
	ErrorStatus ret=SUCCESS;
	printf("\r\n Begin FW Program !\r\n");
	TCH_SPI_UNConfig();
	for (i=0;i<9;i++){
		if (i==0){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN1_2_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN1_2_GPIO_PORT;
		}
		if (i==1){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN2_2_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN2_2_GPIO_PORT;
		}
		if (i==2){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN3_2_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN3_2_GPIO_PORT;
		}
		if (i==3){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN1_3_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN1_3_GPIO_PORT;
		}
		if (i==4){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN2_3_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN2_3_GPIO_PORT;
		}
		if (i==5){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN3_3_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN3_3_GPIO_PORT;
		}
		if (i==6){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN1_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN1_GPIO_PORT;
		}
		if (i==7){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN2_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN2_GPIO_PORT;
		}
		if (i==8){
			TCH_SPI_CSN_PIN=TCH_SPI_CSN3_PIN;
			TCH_SPI_CSN_GPIO_PORT=TCH_SPI_CSN3_GPIO_PORT;
		}
	  if(Program_FW()){
			k=0;
			printf("\r\n FW Program success!, Panel NO =%d\n", i+1);
		}
	  else{
			printf("\r\n FW Program error!, Panel NO =%d\n", i+1);
			i--;
			k++;
			if (k==10){
				ret=ERROR;
				break;
			}
		}
		Delay_ms(200);
		TCH_SPI_UNConfig();
	}
  return ret;
}

/*********************************************************************************
* Function: FT_FWReg_Write
* Description: 
* Input: -data
* Input: -len, data length, range is 0~65535
* Output: none
* Return: none
* Call: internal

*/
void FT_TP_STOP(void) 
{
	uint8_t buf = 0x03; //0x00: TP STAR, write twice
	
	TCH_SPI_Config();
	FT_FWReg_Write(0xA5, 1, &buf);
  printf("TP STOP!\r\n");
	Delay_ms(120); //at least 120 
	TCH_SPI_UNConfig();
}
