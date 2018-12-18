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
 * Parameters definition for SD FW file
 */
struct HX_bin_map {
uint32_t BIN_addr;
uint32_t SRAM_addr;
uint32_t size;
};
uint8_t support_hw_crc=1;
static struct HX_bin_map *bin_map;

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
void TCH_SPI_Config(void){
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
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_SCK_PIN; //TEST21
	GPIO_Init(TCH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT,TCH_SPI_SCK_PIN);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MOSI_PIN; //TEST18
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
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
		}	 
	  GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);    //SCK last low
		if(TEST_MODE != TEST_MODE_RA){
			Delay_us(10);
		}//ET delay
		else{
			Delay_ms(1);
		}//RA delay
	}
}

 /*********************************************************************************
* Function: TCH_SPI_RegWrite
* Description: IO simiulate SPI write,Send len(len<=256) byte
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
void TCH_SPI_RegWrite(uint8_t reg_addr, uint8_t length , uint8_t write_data){ 
  uint8_t i;
	uint8_t SPI_format_buf[16];
	SPI_format_buf[0] = 0xF2; // SPI Write ID
	SPI_format_buf[1] = reg_addr; // local register
	for (i=0; i<length; i++)
	{
		SPI_format_buf[i+2] = write_data;
	}
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(SPI_format_buf, length+2);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
  GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//CS=1,cancel chip selection
}

/*********************************************************************************
* Function:  Burst_Enable  
* Description: 
* Input: none
* Output: none
* Return: void
* Call: Internal
*/
void Himax_Burst_Enable(uint8_t auto_add_4_byte)
{
  uint8_t cmd_data = (0x10 | auto_add_4_byte);
  TCH_SPI_RegWrite(0x13, 1, 0x31);
	TCH_SPI_RegWrite(0x0D, 1, cmd_data);	
}

 /*********************************************************************************
* Function: TCH_SPI_WordWrite 
* Description: IO simiulate SPI write, Send a word
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
void TCH_SPI_WordWrite(uint32_t addr, uint16_t length , uint8_t* write_data){ 
	uint8_t DataLen = 6;
	uint8_t write_buf[6];
  Himax_Burst_Enable(1);
	write_buf[0] = 0xf2; // SPI Write ID
	write_buf[1] = 0x00; // Start from local address 0x00
	write_buf[2] = (uint8_t)addr;
	write_buf[3] = (uint8_t)(addr >> 8);
	write_buf[4] = (uint8_t)(addr >> 16);
	write_buf[5] = (uint8_t)(addr >> 24);
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(&write_buf[0], DataLen);
	TCH_SPI_WriteByte(write_data, length);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
  GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//CS=1,cancel chip selection
	TCH_SPI_RegWrite(0x0c, 1, 0x01); // Trigger AHB Write operation
	TCH_SPI_RegWrite(0x13, 1, 0x00); // Disable Continuous Read/Write
	TCH_SPI_RegWrite(0x0d, 1, 0x00); // Disable AutoWrite
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
	uint8_t bitPos;
	uint8_t dataRd = 0x00;
	 
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
			if (GPIO_ReadInputDataBit(TCH_SPI_MISO_GPIO_PORT,TCH_SPI_MISO_PIN) == SET){
				dataRd |= bitPos;	
			}	
			else{
				dataRd |= 0x00;
			}
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);	  	
			bitPos >>= 1;
			if(TEST_MODE != TEST_MODE_RA){
				Delay_us(10);
			}//ET delay
			else{
				Delay_ms(1);
			}//RA delay
		} 
		dataRdbuf[i] = dataRd;  
		dataRd = 0x00;
		GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);  	
		if(TEST_MODE != TEST_MODE_RA){
			Delay_us(10);
		}//ET delay
		else{
			Delay_ms(1);
		}//RA delay
	}
}

/*********************************************************************************
* Function:  FLASH_SPI_ReadByte  
* Description: IO simiulate SPI read,read a byte
* Input: none
* Output: none
* Return:  a read Byte
* Call: Internal
*/
void TCH_SPI_RegRead(uint8_t reg_addr, uint8_t length, uint8_t* read_buf)
{
	uint8_t i;
	uint8_t SPI_format_buf[16];
	SPI_format_buf[0] = 0xF3; // SPI Read ID
	SPI_format_buf[1] = reg_addr; // local register
	SPI_format_buf[2] = 0x00; // dummy read byte
	
  GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
  TCH_SPI_WriteByte(SPI_format_buf, 3);
	for (i=0; i<length; i++)
	{
		SPI_format_buf[i] = 0;
	}
	TCH_SPI_ReadByte(read_buf, length);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
}

/*********************************************************************************
* Function:  TCH_SPI_WordRead
* Description: IO simiulate SPI read,read a byte
* Input: none
* Output: none
* Return:   read n(n<=4) Byte
* Call: Internal
*/
void TCH_SPI_WordRead(uint32_t ahb_addr, uint16_t length, uint8_t* read_buf) 
{
	uint8_t DataLen;
	uint8_t write_buf_6[6];
	uint8_t Temp_buf[4];
	
  Himax_Burst_Enable(1); //Auto read
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	DataLen = 6;
	write_buf_6[0] = 0xf2; // SPI Write ID
	write_buf_6[1] = 0x00; // Start from local address 0x00
	write_buf_6[2] = (uint8_t)ahb_addr;
	write_buf_6[3] = (uint8_t)(ahb_addr >> 8);
	write_buf_6[4] = (uint8_t)(ahb_addr >> 16);
	write_buf_6[5] = (uint8_t)(ahb_addr >> 24);
  GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(&write_buf_6[0], DataLen); //write
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip diselection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_RegWrite(0x0c, 1, 0x00); // Trigger AHB Read operation
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
	Delay_us(4);
	Temp_buf[0] = 0xF3; // SPI Read ID
	Temp_buf[1] = 0x08; // Start from local address 0x08
	Temp_buf[2] = 0x00; // dummy read byte	
	TCH_SPI_WriteByte(Temp_buf, 3);	
	TCH_SPI_ReadByte(read_buf, length);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
  GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip diselection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_RegWrite(0x13, 1, 0x00); // Disable Continuous Read/Write
	TCH_SPI_RegWrite(0x0d, 1, 0x00); // Disable AutoWrite
}

 /*********************************************************************************
* Function: IC_Enter_SaveMode
* Description: IC Enter SaveMode configure
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
ErrorStatus IC_Enter_SafeMode(void)
{ 
  uint8_t read_buf[1];
	uint8_t cnt = 0;
	
	do
	{	
		printf("Try to Enter safe mode\r\n");	
		TCH_SPI_RegWrite(0x31, 1, 0x27);
		TCH_SPI_RegRead(0x31, 1, &read_buf[0]);
		printf("Read from reg 0x31 is 0x%02X\r\n", read_buf[0]);	
		TCH_SPI_RegWrite(0x32, 1, 0x95);
		TCH_SPI_RegRead(0x32, 1, &read_buf[0]);
		printf("Read from reg 0x32 is 0x%02X\r\n", read_buf[0]);	
		TCH_SPI_WordRead(0x900000A8, 1, read_buf);
		printf("Read from Himax 0x900000A8 is 0x%02X\r\n", read_buf[0]);	
		if (read_buf[0] == 0x0C)
		{ 
			printf("Has enter safe mode\r\n" );	 
			return SUCCESS;
		}
		else
		{
			printf("Not enter safe mode\r\n" );	 
			return ERROR;
		}
	} while(cnt++ < 15);
}

ErrorStatus  himax_system_reset(void)
{
	uint8_t tmp_data[4];
	
	//=====================================
	// System reset : 0x9000_0018 ==> 0x0000_0055
	//=====================================
	tmp_data[3] = 0x00; 
	tmp_data[2] = 0x00; 
	tmp_data[1] = 0x00; 
	tmp_data[0] = 0x55;
	TCH_SPI_WordWrite(90000018, 4, tmp_data);
	// for fw to setup
	Delay_ms(100);
	
	return SUCCESS;
}

ErrorStatus  himax_check_reload_state(void)
{
	uint8_t cnt = 0;
	uint8_t tmp_data[4];

	do
	{
		TCH_SPI_WordRead(0x900000A8, 4, tmp_data);
		if (tmp_data[0] == 0x05)
		{
			return SUCCESS;
		}	
		cnt++;
	} while (cnt <= 10);
	return ERROR;
}

ErrorStatus  himax_reset_touch_ic(void)
{
	ErrorStatus  result = SUCCESS;

	result = himax_system_reset();
	Himax_Burst_Enable(1);
	result &= himax_check_reload_state();
	
	return result;
}

/*********************************************************************************
* Function: IC_Enter_SafeMode
* Description: IC Enter SaveMode configure
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
ErrorStatus IC_Leave_SafeMode(void)
{ 
	ErrorStatus ret = SUCCESS;
	uint8_t  read_buf[4];
  uint8_t  write_buf[4];
	uint8_t  retry = 0;
	
	do
	{
		write_buf[3] = 0x00;
		write_buf[2] = 0x00;
		write_buf[1] = 0x00;
		write_buf[0] = 0x53;
		TCH_SPI_WordWrite(0x90000098, 4, write_buf);
		Delay_ms(1);
//		TCH_SPI_WordRead(0x90000098, 4, read_buf);
//		printf("\r\n Read from Himax 83112A IC SRAM 0x90000098 is %02x, %02x, %02x, %02x", read_buf[3], read_buf[2], read_buf[1], read_buf[0]);	
//	  if (read_buf[0] == 0x53)
//	 { 
//		printf("\r\n Has leave safe mode\r\n" );	 
//		ret = SUCCESS;
//	 }

		TCH_SPI_WordRead(0x900000E4,4,read_buf);
		printf("Read from Himax 83112A IC SRAM 0x900000E4 is 0x%02X, 0x%02X\r\n", read_buf[1], read_buf[0]);
	} while (((read_buf[1]!=0x01) || (read_buf[0]!=0x00)) && retry++ < 5);		

	if (retry >= 5)
	{
		ret = himax_reset_touch_ic();
	}
	else
	{
		printf("Has leave safe mode\r\n" );	
		TCH_SPI_RegWrite(0x31, 1, 0x00);
	
		TCH_SPI_RegWrite(0x32, 1, 0x00);

		write_buf[3] = 0x00;
		write_buf[2] = 0x00;
		write_buf[1] = 0x00;
		write_buf[0] = 0x00;
		TCH_SPI_WordWrite(0x90000098, 4, write_buf);
		ret = SUCCESS;
	}
	
	return ret;
 }

/*********************************************************************************
* Function: 
* Description:himax_sense_on
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
ErrorStatus himax_sense_on()
{
	ErrorStatus ret = ERROR;
	ret = IC_Leave_SafeMode();
	return ret;	
}

/*********************************************************************************
* Function: 
* Description: TCON_Reset
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
void TCON_Reset(void)
{
  uint8_t write_buf[4];
	
	write_buf[3] = 0x00;
	write_buf[2] = 0x00;
	write_buf[1] = 0x00;
	write_buf[0] = 0x00;
	TCH_SPI_WordWrite(0x80020020, 4, write_buf);
	
	write_buf[3] = 0x00;
	write_buf[2] = 0x00;
	write_buf[1] = 0x00;
	write_buf[0] = 0x01;
	TCH_SPI_WordWrite(0x80020020, 4, write_buf);
}

/*********************************************************************************
* Function: 
* Description: ADC_Reset
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
void ADC_Reset( void)
{
	uint8_t write_buf[4];
	
	write_buf[3] = 0x00;
	write_buf[2] = 0x00;
	write_buf[1] = 0x00;
	write_buf[0] = 0x00;
	TCH_SPI_WordWrite(0x80020094, 4, write_buf);
	
	write_buf[3] = 0x00;
	write_buf[2] = 0x00;
	write_buf[1] = 0x00;
	write_buf[0] = 0x01;
	TCH_SPI_WordWrite(0x80020094, 4, write_buf);
}

ErrorStatus  himax_check_sram(uint32_t start_addr, uint8_t* Fcontent, uint16_t length)
{
	uint16_t i;
  ErrorStatus ret = SUCCESS;
	uint8_t sram_data[1024];
	uint8_t* target_data = Fcontent;
	
	TCH_SPI_WordRead(start_addr, length, sram_data);
	for (i=0; i<length; i++)
	{
		if (target_data[i] != sram_data[i])
		{
			printf("data error at index = %d, 0x%02X, 0x%02X\r\n", i, target_data[i], sram_data[i]);	
			ret = ERROR;
		}
	}
 
	return ret;
}

/*********************************************************************************
* Function: 
* Description:himax_hw_crc
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
uint32_t himax_hw_crc(uint32_t start_addr, int32_t length)
{
	int32_t cnt = 0;
	uint32_t crc = 0xFF;
	uint8_t tmp_data[4];
	int32_t tmp_length = length / 4;
	
	tmp_data[3] = (uint8_t)(start_addr >> 24);
	tmp_data[2] = (uint8_t)(start_addr >> 16);
	tmp_data[1] = (uint8_t)(start_addr >> 8);
	tmp_data[0] = (uint8_t)start_addr;
	TCH_SPI_WordWrite(80050020, 4, tmp_data);
	
	tmp_data[3] = 0x00; 
	tmp_data[2] = 0x99; 
	tmp_data[1] = (uint8_t)(tmp_length >> 8); 
	tmp_data[0] = (uint8_t) tmp_length;	
	TCH_SPI_WordWrite(80050028, 4, tmp_data);
	
	cnt = 0;
	do
	{
		TCH_SPI_WordWrite(80050000, 4, tmp_data);
		if ((tmp_data[0] && 0x01) != 0x01)
		{
			TCH_SPI_WordRead(80050018, 4, tmp_data);
			crc = (uint32_t)(tmp_data[3] << 24) + (uint32_t)(tmp_data[2] << 16) + (uint32_t)(tmp_data[1] << 8) + (uint32_t)tmp_data[0];
			return crc;
		}	
		cnt++;
	} while (cnt < 100);
	
	return crc;
}

/*********************************************************************************
* Function: 
* Description:himax_sw_crc
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
uint32_t himax_sw_crc(uint8_t* file_array, int32_t length)
{
	uint32_t i;
	uint32_t j;
	uint32_t fw_data;
	uint32_t fw_data_2;
	uint32_t crc = 0xFFFFFFFF;
	uint32_t PolyNomial = 0x82F63B78;
	
	for (i=0; i<(length/4); i++)
	{
		fw_data = file_array[i*4];		
		for (j=1; j<4; j++)
		{
			fw_data_2 = file_array[i*4 + j];
			fw_data += (fw_data_2) << (8*j);
		}		
		crc = fw_data ^ crc;

		for (j=0; j<32; j++)
		{
			if ((crc % 2) != 0)
			{
				crc = ((crc >> 1) & 0x7FFFFFFF) ^ PolyNomial;
			}
			else
			{
				crc = (((crc >> 1) & 0x7FFFFFFF)/* & 0x7FFFFFFF*/);
			}
		}
	}
	return crc;
}

ErrorStatus himax_check_crc(uint32_t start_addr, uint8_t* file_array, uint32_t length)
{
	uint32_t crc1 = 0xFE; 
	uint32_t crc2 = 0xFE;
	
	crc1 = himax_hw_crc(start_addr, length);
	if (crc1 == 0)
	{
		return SUCCESS;
	}
	printf("HW CRC is not 0 = 0x%08X\r\n ",crc1);	

	crc2 = himax_sw_crc(file_array, length);	
	if (crc2 == crc1)
	{
		return SUCCESS;
	}
	printf("SW CRC is not the same with HW CRC = 0x%08X\r\n", crc2);	
	
	return SUCCESS;
}
/*********************************************************************************
* Function: 
* Description: WriteSram
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
void	WriteSram(uint8_t* fwCeil, uint32_t start_addr, uint32_t Len)
{   
		TCH_SPI_WordWrite(start_addr, Len, fwCeil);
}

static uint32_t byte_to_word(const uint8_t *data)
{
 return data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
}

/*********************************************************************************
*Description:
   Himax touchscreen Write_Partition function, to write firmware into each partition.
* Input: none
* Output: none
* return: ErrorStatus  
* Call: Internal
*/
ErrorStatus Write_Partition(void)
{
	uint32_t partition = 0;
	uint8_t FWContent[1024];	
	uint32_t list = 0;
	uint32_t BIN_addr, SRAM_addr, size;
	uint16_t len = 0;
	int32_t count = 0;
	uint32_t i = 0;
	ErrorStatus ret =SUCCESS; 
	uint32_t pos = 0x00;
		/* OPEN FW bin file */	
		if (SD_FW_OPEN() == ERROR)
		{
				printf("FW file open error!\r\n");
				ret = ERROR;	
			}
	/* Find the header size */		
	if(f_lseek(&fwfile,0x10000)==FR_OK)//64k以后位置放bin  map
		SD_FW_Read(FWContent,256); //read FW header 256BYTE
	else
	{
		printf("The FW File Not point to of file!\r\n");
		return ERROR;
	}
	/* parase bin tail addresss map */		
	printf("\r\n header 64k +0x12, 0x12=%d\n", FWContent[12]);
	//partition = fwdata[12+0] + (fwdata[12+1] << 8) + (fwdata[12+2] << 16) + (fwdata[12+3] << 24);
	partition = byte_to_word(&FWContent[12]);
	printf("\r\n header partition=%d\n", partition);

	bin_map = (struct HX_bin_map *)malloc((partition) * sizeof(struct HX_bin_map));
	 if(bin_map == NULL) 
	 {
	 printf("malloc for bin_map failed!");
	 return ERROR;
   }
  else
  printf("malloc for bin_map success!\n");
  for(list = 0; list < partition; list++)
  {
		pos=list*0x10;
		bin_map[list].SRAM_addr = byte_to_word(&FWContent[0 + pos]);
		bin_map[list].size = byte_to_word(&FWContent[4 + pos]);
		bin_map[list].BIN_addr = byte_to_word(&FWContent[8 + pos]);
	}
	/* write partion to IC SRAM */	
  for (list = 0; list < partition; list++) 
 {   
	/* initialize variable */
	SRAM_addr = bin_map[list].SRAM_addr;
	size = bin_map[list].size;
	BIN_addr = bin_map[list].BIN_addr;

	/* Check data size */
	if ((BIN_addr + size) > fileSize)
	{
	   printf("access range (0x%08X to 0x%08X) is larger than bin size!\n",
	   BIN_addr, BIN_addr + size);
	   ret =ERROR;
	   return ret;
	}

	/* ignore reserved partition (Reserved Partition size is zero) */
	if (!size)
		continue;
	else
		size = size ;// important
	if (size % 1024)
		count = (size / 1024) + 1;
	else
		count = (size / 1024);
	for (i = 0 ; i <count ; i++)
{
	 len = (size < 1024) ? size : 1024;
	 /*write data to SRAM */
	 if(f_lseek(&fwfile,BIN_addr)==FR_OK)
  {			 
	  SD_FW_Read(FWContent,len);
	  WriteSram(FWContent,SRAM_addr,len);
	  SRAM_addr += 1024;
	  BIN_addr += 1024;
	  size -= 1024;
	}
	 else
	 {
	  printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
	  ret=ERROR;
	 }
 } 
	if(himax_hw_crc(bin_map[list].SRAM_addr, bin_map[list].size)!= 0)
	printf("CRC fail!\n");
 }
 /* Close FW bin file */	
  SD_FW_Close();
   return ret;
}

/*********************************************************************************
* Function: 
* Description:
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
ErrorStatus Program_FW(void)
{
 
	ErrorStatus ret = SUCCESS;
#ifdef NO_FLASH_MODE
	uint8_t read_buf[1];
	uint8_t tmp_data[4];
	uint8_t retry = 0;
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
		
		if (IC_Enter_SafeMode() == ERROR)
		{
			ret = ERROR;
			continue;
		}
		printf("TCON Reset\r\n");
		TCON_Reset();
		printf("ADC Reset\r\n"); 
		ADC_Reset();
	  /* Start Write Firmware Process */
    ret = Write_Partition();
    if (!ret) 
    {
  	 printf("\r\n Write_Firmware failed.");
  	 continue;
    }
			tmp_data[3] = 0x00; 
			tmp_data[2] = 0x00; 
			tmp_data[1] = 0x00; 
			tmp_data[0] = 0x00;
			TCH_SPI_WordWrite(0x10007294, 4, tmp_data);
		/*himax modify end*/
		
		// Zero flash reload  0x1000_7F00 -> 0x9AA9
		tmp_data[3] = 0x00; 
		tmp_data[2] = 0x00; 
		tmp_data[1] = 0x9A; 
		tmp_data[0] = 0xA9;
		TCH_SPI_WordWrite(0x10007F00, 4, tmp_data);
		TCH_SPI_WordRead(0x900000A8, 1, read_buf);
		printf("Read from Himax 0x900000A8 is 0x%02X\r\n", read_buf[0]);	
		TCH_SPI_WordRead(0x900000F8, 1, read_buf); 
		printf("Read from Himax 0x900000F8 is 0x%02X\r\n", read_buf[0]);	
		ret = himax_sense_on();
		TCH_SPI_WordRead(0x900000A8, 1, read_buf);
	  if(read_buf[0]==0x05)
		printf("Read from Himax 0x900000A8 is 0x%02X,FW program Sucess\r\n", read_buf[0]);	
	  break; 
  }
 
	if (TEST_MODE != TEST_MODE_RA)
	{
		TCH_SPI_UNConfig();
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
