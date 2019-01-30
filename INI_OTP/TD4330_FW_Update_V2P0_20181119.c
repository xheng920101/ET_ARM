#include "include.h"

/*********************************************************************************
 * Interface definition for all IC, not to change!
 */
#define    TCH_SPI_CSN1_PIN                GPIO_Pin_9		//TEST22: TP_SDA --> SPI_CSN1
#define    TCH_SPI_CSN1_GPIO_PORT          GPIOB

#define    TCH_SPI_SCK_PIN                GPIO_Pin_8		//TEST21: TP_SCL --> SPI_SCK
#define    TCH_SPI_SCK_GPIO_PORT          GPIOB

#define    TCH_SPI_MISO_PIN                GPIO_Pin_6		//TEST19: POWER_I2C_SCL --> SPI_MISO
#define    TCH_SPI_MISO_GPIO_PORT          GPIOB

#define    TCH_SPI_MOSI_PIN                GPIO_Pin_5		// TEST18: POWER_I2C_SDA --> SPI_MOSI
#define    TCH_SPI_MOSI_GPIO_PORT          GPIOB

#define    TCH_SPI_CSN2_PIN                GPIO_Pin_7		//TEST20: TP_INT --> SPI_CSN2
#define    TCH_SPI_CSN2_GPIO_PORT          GPIOB

#define    TCH_SPI_CSN3_PIN                GPIO_Pin_2		//TEST23: SPI_CSN3
#define    TCH_SPI_CSN3_GPIO_PORT          GPIOC

#define    TCH_RST_PIN				             GPIO_Pin_3		//TEST24: TP_RST --> TP_RST
#define    TCH_RST_GPIO_PORT		           GPIOC

uint16_t			TCH_SPI_CSN_PIN = TCH_SPI_CSN1_PIN;
GPIO_TypeDef *TCH_SPI_CSN_GPIO_PORT = TCH_SPI_CSN1_GPIO_PORT;
/* 
 * End of interface definition
*********************************************************************************/

/*********************************************************************************
 * Parameters definition for TD43xx
 */
//Standard Commands
#define		IDENTIFYS										0x02
#define		RESET												0x04

//Bootloader Commands
#define		GET_BOOT_INFO								0x10
#define 	ERASE_FLASH									0x11
#define		WRITE_FLASH									0x12
#define		READ_FLASH									0x13
#define		RUN_APPLICATION_FIRMWARE		0x14
#define   SPI_MASTER_WRITE_THEN_READ	0x15
#define 	REBOOT_TO_ROM_BOOTLOADER		0x16
#define		ENTER_BOOTLOADER_MODE				0x1F
#define 	GET_APP_INFO								0x20
#define		GET_STATIC_CONFIG						0x21
#define		SET_STATIC_CONFIG						0x22
#define		GET_DYNAMIC_CONFIG					0x23
#define 	SET_DYNAMIC_CONFIG					0x24
#define		GET_REPORT_CONFIG						0x25

//Format, Do not change!
#define 	HEADER_IMG									0x4818472B
#define 	HEADER_EREA									0x7C05E516
#define  	DATA_OFFSET									36	

//download fail retry time
#define		ERROR_RETRY_TIMES						2

//#define		b		12	//Syna bit send interval, every 0.0375uS count
//#define		B		106	//Syan BYTE send interval, every 0.0375uS count

//RA mode recomad speed 
#define		b		18	//Syna bit send interval, every 0.0375uS count
#define		B		32	//Syan BYTE send interval, every 0.0375uS count

//unmark to printf APP CODE RAW DATA, Warning:Debug only!
//#define		PRINT_RAW	

uint8_t men = 0x00; //initial must be 00

extern ErrorStatus OTP_HDL_MODE(void);
extern void DriverIC_RST_TP(void);

/*********************************************************************************
 * Parameters definition for SD FW file
 */
FIL fwfile; 
DIR filedir;
uint32_t fileSize;
char FWVersion[12] = "FW";
char FW_project[14];

struct rmi_info
{
	unsigned char intr_src_count:3;
	unsigned char reserved_1:2;
	unsigned char fn_version:2;
	unsigned char reserved_2:1;
}_packed_info;

struct rmi_pdt_entry 
{
	unsigned char query_base_addr;
	unsigned char command_base_addr;
	unsigned char control_base_addr;
	unsigned char data_base_addr;
	unsigned char info;
	unsigned char fn_number;
}_packed_rmi;

struct img_contents
{
	uint32_t id;
	uint32_t block;
	uint32_t area_0;//BOOT_ONFIG *
  uint32_t area_1;//F35_APP_CODE
	uint32_t area_2;//APP_CONFIG
	uint32_t area_3;//DISPLAY
	uint32_t area_4;//JSON *
	uint32_t Preambl;
}_img_contents;

struct block_contents
{
	uint32_t id;
	uint8_t  str[16];
	uint32_t flags;
  uint32_t destination;
	uint32_t len;
	uint32_t crc;
}_block_contents;

struct IDENTIFY 
{
	uint8_t begin;
	uint8_t	RTcode;
	uint16_t len;
}identify;
/* 
 * End of parameters definition
*********************************************************************************/

/*********************************************************************************
* Function: nop() 
* Description: RA FW Upgrate
* Input: none
* Output: none
* Return: FW upgrate result
* Call: external
*/
void nop(unsigned int cnt)//count 10000 = delay = 0.375mS ;cnt 13.3 equal about 2M speed
{
	while(cnt--);
}

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

	sprintf(file_name, "%s*.img", FW_project);
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
void TCH_SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin = TCH_RST_PIN; //TEST24
	GPIO_Init(TCH_RST_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_RST_GPIO_PORT, TCH_RST_PIN);

	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN2_PIN; //TEST20
	GPIO_Init(TCH_SPI_CSN2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN2_GPIO_PORT, TCH_SPI_CSN2_PIN);	

	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN1_PIN; //TEST22
	GPIO_Init(TCH_SPI_CSN1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN1_GPIO_PORT, TCH_SPI_CSN1_PIN);	

	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN3_PIN; //TEST23
	GPIO_Init(TCH_SPI_CSN3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_CSN3_GPIO_PORT, TCH_SPI_CSN3_PIN);	
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_SCK_PIN; //TEST21
	GPIO_Init(TCH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); //mode0/1: keep low when idle
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MOSI_PIN; //TEST18
	GPIO_Init(TCH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MISO_PIN ; //TEST19: 
	GPIO_Init(TCH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);	
}

/*********************************************************************************
* Function: TCH_SPI_UNConfig
* Description: TCH SPI interface recovery to original settings
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void TCH_SPI_UNConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MOSI_PIN; //TEST18: POWER_I2C_SDA
	GPIO_Init(TCH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_MISO_PIN ; //TEST19: POWER_I2C_SCL
	GPIO_Init(TCH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN2_PIN; //TEST20: TP_INT
	GPIO_Init(TCH_SPI_CSN2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN1_PIN; //TEST22: TP_SDA
	GPIO_Init(TCH_SPI_CSN1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TCH_RST_PIN; //TEST24: TP_RST
	GPIO_Init(TCH_RST_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TCH_SPI_SCK_PIN; //TEST21: TP_SCL
	GPIO_Init(TCH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;	

	GPIO_InitStructure.GPIO_Pin = TCH_SPI_CSN3_PIN; //TEST23: I_IOVCC
	GPIO_Init(TCH_SPI_CSN3_GPIO_PORT, &GPIO_InitStructure);
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
//			if (TEST_MODE == TEST_MODE_RA) 
//			{
//			      Delay_us(2);//RA
//			}
			if (dataWrt[i] & bitPos)
			{
				GPIO_SetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
			else
			{
				GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
//			nop(b);	
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);		
			bitPos >>= 1;
//			nop(b);					

		}	 

		GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); //SCK last High
//		nop(b);		
		GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);				
		nop(25);	
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
		GPIO_SetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
		while (bitPos)
		{
			GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);	
			nop(b);
			if (TEST_MODE == TEST_MODE_RA)
			{
				Delay_us(2); //RA_delay 10us, else delay 1ns
			}
			else
			{
				for(j=0; j<3; j++); //delay_ns
			}
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); 
		  nop(b);
			if (TEST_MODE == TEST_MODE_RA)	
			{
				Delay_us(2);//RA_delay 10us, else no delay
			}
			if (GPIO_ReadInputDataBit(TCH_SPI_MISO_GPIO_PORT,TCH_SPI_MISO_PIN) == SET)
			{
				dataRd |= bitPos;	
			}	
			else
			{
				dataRd |= 0x00;
			} 	
//			if (TEST_MODE == TEST_MODE_RA)	
//			{
//				Delay_us(25);//RA_delay 10us, else no delay
//			}
			bitPos >>= 1;
		} 
		GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN); 		// SCK last high
		nop(b);
		GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
		nop(B);
		dataRdbuf[i] = dataRd;  
		dataRd = 0x00;	
		if (TEST_MODE == TEST_MODE_RA)//RA_delay 10us,else no delay
		{
			Delay_us(10);
		}
	}	
}
 
/*********************************************************************************
* Function: Detect INT/ATTN 
* Description: ATTN pin is connect to main board TEST 20
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void Detect_ATTN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t timeout = 0;
	
	printf("waiting ATTN/INT.........\r\n");		
	if (TEST_MODE != TEST_MODE_RA)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = TEST20_PIN; // ATTEN
		GPIO_Init(TEST20_GPIO_PORT, &GPIO_InitStructure);	

		while (GPIO_ReadInputDataBit(TEST20_GPIO_PORT, TEST20_PIN) == SET)
		{
			Delay_ms(1);
			if ((timeout++) > 1000)	break;
		}	
		printf(">>>>>>timeout count is %d>>>>>>\r\n", timeout);
	}
	else
	{
		Delay_ms(1000);
		printf(">>>>>>wait atten pin active 1000mS>>>>>>\r\n");	
	}
}

/*********************************************************************************
* Function: Read_TouchCom()
* Description: read from IC
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void Read_TouchCom(uint8_t reg)
{
	uint8_t buf[32], i, len;
	
	printf("\r\n/*============ read %02xH ============*/\r\n", reg);
	buf[0] = reg;
	buf[1] = 0x00;
	buf[2] = 0x00;	
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(buf, 3);
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	Delay_us(10);
	
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_ReadByte(buf, 4);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(1);
	
	printf("\r\n");	
	for (i=0; i<4; i++)
	{
		printf("0x%02x\t", buf[i]);	
	}
	printf("\r\n");
	
	len = buf[2];
	Delay_us(10);
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_ReadByte(buf, len+3);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(1);
	
	printf("\r\n");	
	for (i=0; i<len+3; i++)
	{
		printf("0x%02x\t", buf[i]);
	}
	printf("\r\n");	
}

/*********************************************************************************
* Function: Scan_PDT
* Description: read from IC
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void Scan_PDT()
{
	uint8_t buf[6];
	
	printf("\r\n/*============ Scan_PDT ============*/\r\n");
	buf[0] = 0x80;
	buf[1] = 0xe9;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(buf, 2);
	TCH_SPI_ReadByte(buf, 6);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	Delay_us(1);

	_packed_rmi.query_base_addr = buf[0];
//	printf("_packed_rmi.query_base_addr = 0x%02x\r\n", _packed_rmi.query_base_addr);
	_packed_rmi.command_base_addr = buf[1];	
//	printf("_packed_rmi.command_base_addr = 0x%02x\r\n", _packed_rmi.command_base_addr);
	_packed_rmi.control_base_addr = buf[2];	
//	printf("_packed_rmi.control_base_addr = 0x%02x\r\n", _packed_rmi.control_base_addr);
	_packed_rmi.data_base_addr = buf[3];	
//	printf("_packed_rmi.data_base_addr = 0x%02x\r\n", _packed_rmi.data_base_addr);
	_packed_rmi.info = buf[4];	 
//	printf("_packed_rmi.info = 0x%02x\r\n", _packed_rmi.info);
	_packed_rmi.fn_number=buf[5];
//	printf("_packed_rmi.fn_number = 0x%02x\r\n", _packed_rmi.fn_number);
}	

/*********************************************************************************
* Function: SYNA_FW_GET_CONTENTS()
* Description: get content info form f/w.img
* Input: none
* Output: 
* Return: 
* Call: internal
*/
ErrorStatus SYNA_FW_GET_CONTENTS() 
{
	uint32_t BIN_addr;
	uint8_t FWContent[36];
	ErrorStatus ret = SUCCESS;
	
	printf("\r\n");     
	if (SD_FW_OPEN() == ERROR)
	{
		printf("FW file open error!\r\n");
		return ERROR;
	}

	BIN_addr = 0;
	if (f_lseek(&fwfile, BIN_addr) == FR_OK)
	{			 
		SD_FW_Read(FWContent, 32);

		_img_contents.id = (uint32_t) FWContent[0];
		_img_contents.id += (uint32_t) FWContent[1]<<8;				
		_img_contents.id += (uint32_t) FWContent[2]<<16;				
		_img_contents.id += (uint32_t) FWContent[3]<<24;				
		if (_img_contents.id == HEADER_IMG) //check ID ok
		{
			printf("Check f/w ID OK!\r\n" );
			_img_contents.block = (uint32_t) FWContent[4];				
			_img_contents.block += (uint32_t) FWContent[5]<<8;					
			_img_contents.block += (uint32_t) FWContent[6]<<16;				
			_img_contents.block += (uint32_t) FWContent[7]<<24;				
//			printf("_img_contents.block = 0x%x\r\n", _img_contents.block);
			
			_img_contents.area_0 = (uint32_t) FWContent[8];				
			_img_contents.area_0 += (uint32_t) FWContent[9]<<8;					
			_img_contents.area_0 += (uint32_t) FWContent[10]<<16;				
			_img_contents.area_0 += (uint32_t) FWContent[11]<<24;				
//			printf("_img_contents.area_0 = 0x%x\r\n", _img_contents.area_0);				

			_img_contents.area_1 = (uint32_t) FWContent[12];				
			_img_contents.area_1 += (uint32_t) FWContent[13]<<8;					
			_img_contents.area_1 += (uint32_t) FWContent[14]<<16;				
			_img_contents.area_1 += (uint32_t) FWContent[15]<<24;				
//			printf("_img_contents.area_1 = 0x%x\r\n", _img_contents.area_1);

			_img_contents.area_2 = (uint32_t) FWContent[16];				
			_img_contents.area_2 += (uint32_t) FWContent[17]<<8;					
			_img_contents.area_2 += (uint32_t) FWContent[18]<<16;				
			_img_contents.area_2 += (uint32_t) FWContent[19]<<24;				
//			printf("_img_contents.area_2 = 0x%x\r\n", _img_contents.area_2);			

			_img_contents.area_3 = (uint32_t) FWContent[20];				
			_img_contents.area_3 += (uint32_t) FWContent[21]<<8;					
			_img_contents.area_3 += (uint32_t) FWContent[22]<<16;				
			_img_contents.area_3 += (uint32_t) FWContent[23]<<24;				
//			printf("_img_contents.area_3 = 0x%x\r\n", _img_contents.area_3);

			_img_contents.area_4 = (uint32_t) FWContent[24];				
			_img_contents.area_4 += (uint32_t) FWContent[25]<<8;					
			_img_contents.area_4 += (uint32_t) FWContent[26]<<16;				
			_img_contents.area_4 += (uint32_t) FWContent[27]<<24;				
//			printf("_img_contents.area_4 = 0x%x\r\n", _img_contents.area_4);

			_img_contents.Preambl = (uint32_t) FWContent[28];				
			_img_contents.Preambl += (uint32_t) FWContent[29]<<8;					
			_img_contents.Preambl += (uint32_t) FWContent[30]<<16;				
			_img_contents.Preambl += (uint32_t) FWContent[31]<<24;				
//			printf("_img_contents.Preambl = 0x%x\r\n", _img_contents.Preambl);
		}			
		else
		{		
			printf("Error:_img_contents.id = 0x%x ID NG!\r\n", _img_contents.id);
			ret = ERROR;
		}
	}
	else
	{
		printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
		ret = ERROR;
	}
	
	SD_FW_Close();
	return ret;		
}
	
/*********************************************************************************
* Function: Check_F35_UBOOT_Query2
* Description: 
* Input: none
* Output: none
* Return: F35_UBOOT_Query2 status
* Call: internal
*/
uint8_t Check_F35_UBOOT_Query2()
{
	uint8_t buf[8];
	
	buf[0] = 0x80;	
	buf[1] = _packed_rmi.query_base_addr + 2;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(buf, 2);		
	TCH_SPI_ReadByte(buf, 1);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	printf("\r\n Check F35_UBOOT_Query2 = 0x%2X\r\n", buf[0]);

	//check if F35_UBOOT_Query2[1:0]=2'b11
	return buf[0];
}

/*********************************************************************************
* Function: WR_F35_UBOOT_Ctrl3
* Description: 
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void WR_F35_UBOOT_Ctrl3()
{
	uint8_t buf[4];
			
	buf[0] = 0x00;	
	buf[1] = _packed_rmi.control_base_addr + 18;
	buf[2] = 0x04;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(buf, 3);		
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
//	printf("\r\n WR_F35_UBOOT_Ctrl3[0x%02x] = 0x%x\r\n", buf[1], buf[2]);
}

/*********************************************************************************
* Function: WR_F35_UBOOT_Ctrl7
* Description: 
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void WR_F35_UBOOT_Ctrl7()
{
	uint8_t buf[2];
	
	buf[0]= 0x00;	
	buf[1]= _packed_rmi.control_base_addr+22;//0x1c
	TCH_SPI_WriteByte(buf, 2);	
//	printf("\r\n Load WR_F35_UBOOT_Ctrl7[0x%02x]\r\n", buf[1]);
}

/*********************************************************************************
* Function: Check_F35_UBOOT_Data0
* Description: 
* Input: none
* Output: none
* Return: F35_UBOOT_Data0 status
* Call: internal
*/
uint8_t Check_F35_UBOOT_Data0()
{
	uint8_t buf[8];
	
	buf[0] = 0x80;	
	buf[1] = _packed_rmi.data_base_addr;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	TCH_SPI_WriteByte(buf, 2);		
	TCH_SPI_ReadByte(buf, 1);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
	printf("\r\n F35_UBOOT_Data0 = 0x%x\r\n", buf[0]);
	
	return buf[0];
}

/*********************************************************************************
* Function: Get_Identify_Report()
* Description: 
* Input: none
* Output: none
* Return: F35_UBOOT_Data0 status
* Call: internal
*/
uint8_t Get_Identify_Report()
{
	uint8_t buf[64];
//	uint8_t len;

	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	Delay_us(10);
	TCH_SPI_ReadByte(buf, 4);	
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	Delay_ms(2);	

	identify.begin = buf[0];
	identify.RTcode = buf[1];
	identify.len = buf[2] + ((uint16_t)buf[3] << 8);
	
//	printf("\r\n============APP_CODE=============\r\n");
//	for (len=0; len<4; len++)
//	{
//		printf("%02x\t", buf[len]);
//	}
	if (identify.RTcode != 0x10)
	{
		printf("error:read identify report error!\r\n");			
		return ERROR;
	}
	else
	{
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);
		Delay_us(10);			
		TCH_SPI_ReadByte(buf, identify.len+3);	
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		
//		printf("\r\n===========APP_CODE==============\r\n");
//		for (len=0; len<identify.len+3; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}
		
		if (buf[3] == 0x02)
		{
			printf("Info:download F35_APP_CODE SUCCESS!\r\n");
		}
	}
	
	return SUCCESS;
}

/*********************************************************************************
* Function: 
* Description: WriteSram
* Input: uint8_t* fwCeil, uint32_t start_addr, uint32_t Len
* Output: none
* Return: none
* Call: Internal
*/
static void	WriteSram(uint8_t* fwCeil, uint8_t start_addr, uint32_t Len)
{ 
#ifdef PRINT_RAW  
	uint32_t i, cnt = 0;
#endif	
	
	if (men == 0)
	{		
		if (TEST_MODE != TEST_MODE_RA) //RA mode TP_RST cant not be reset!
		{
			DriverIC_RST_TP();
		}
			
		WR_F35_UBOOT_Ctrl3();
		Delay_ms(1);
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);		
		WR_F35_UBOOT_Ctrl7();
		men = 0x55; //excute once
	}
	TCH_SPI_WriteByte(fwCeil, Len);
	
#ifdef PRINT_RAW
	for (i=0; i<Len; i++)
	{
		printf("%02x\t", fwCeil[i]);
		cnt++;
		if (cnt%16 == 0)
		{
			printf("\r\n");
		}
	}
#endif
}

/*********************************************************************************
* Function: SYNA_FW_Write_Block_F35_APP_CODE()
* Description: write F35_APP_CODE to PRAM
* Input: none
* Output: 
* Return: PRAM write result
* Call: internal
*/
ErrorStatus SYNA_FW_Write_Block_F35_APP_CODE(uint32_t area) 
{
	uint32_t BIN_addr, SRAM_addr, size;
	uint16_t len = 0;
	int32_t count = 0;
	uint32_t i = 0;
	uint8_t FWContent[1024];
//	uint8_t buf[32];	
	
  men = 0x00;
//	//check if F35_UBOOT_Query2[1:0]=2'b11
//	if (((Check_F35_UBOOT_Query2()) & 0x03) != 0x03)
//	{
//		printf("\r\n error end return!\r\n");
//		return ERROR;
//	}
//	//Verify F35_UBOOT_Data0
//	buf[0] = Check_F35_UBOOT_Data0();
//	if ((buf[0] & 0x1F) != 0x0b)	
//	{
//		printf("Error :F35_UBOOT_Data0 = 0x%x\r\n", buf[0]);
//		return ERROR;
//	}
//	printf("\r\nRequesting FW\r\n");	

	if (SD_FW_OPEN() == ERROR)
	{
		printf("FW file open error!\r\n");
		return ERROR;
	}
	
	SRAM_addr = 0; //sram start address
	size = fileSize;
	BIN_addr = area;
	if (f_lseek(&fwfile, BIN_addr) == FR_OK)
	{			 
		SD_FW_Read(FWContent, 36);

		_block_contents.id = (uint32_t) FWContent[0];
		_block_contents.id += (uint32_t) FWContent[1]<<8;				
		_block_contents.id += (uint32_t) FWContent [2]<<16;				
		_block_contents.id += (uint32_t) FWContent[3]<<24;				
		if (_block_contents.id ==HEADER_EREA) //check ID ok
		{
			printf("Check Block ID OK!\r\n" );
			for(len=0; len<16; len++)
			{
				_block_contents.str[len] = FWContent[4+len];
//				printf("%c", _block_contents.str[len]);
			}

			_block_contents.flags = (uint32_t) FWContent[20];				
			_block_contents.flags += (uint32_t) FWContent[21]<<8;					
			_block_contents.flags += (uint32_t) FWContent[22]<<16;					
			_block_contents.flags += (uint32_t) FWContent[23]<<24;					
//			printf("\r\n_block_contents.flags = 0x%x\r\n", _block_contents.flags);			

			_block_contents.destination = (uint32_t) FWContent[24];				
			_block_contents.destination += (uint32_t) FWContent[25]<<8;							
			_block_contents.destination += (uint32_t) FWContent[26]<<16;						
			_block_contents.destination += (uint32_t) FWContent[27]<<24;						
//			printf("_block_contents.destination = 0x%x\r\n", _block_contents.destination);

			_block_contents.len = (uint32_t) FWContent[28];				
			_block_contents.len += (uint32_t) FWContent[29]<<8;					
			_block_contents.len += (uint32_t) FWContent[30]<<16;				
			_block_contents.len += (uint32_t) FWContent[31]<<24;				
//			printf("_block_contents.len = 0x%x\r\n", _block_contents.len);
			
			_block_contents.crc = (uint32_t) FWContent[32];				
			_block_contents.crc += (uint32_t) FWContent[33]<<8;					
			_block_contents.crc += (uint32_t) FWContent[34]<<16;				
			_block_contents.crc += (uint32_t) FWContent[35]<<24;				
//			printf("_block_contents.crc = 0x%x\r\n", _block_contents.crc);
		}			 
		else
		{		
			printf("Error:_block_contents.id = 0x%x Block ID NG!\r\n", _block_contents.id);
			GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN); //reset untill app code sent over!
			return ERROR;
		}
	}
	else
	{
		printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN); //reset untill app code sent over!
		return ERROR;
	}
	
	//write data
	size = _block_contents.len;		
	BIN_addr = area + DATA_OFFSET;
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
			SRAM_addr += 1024;
			BIN_addr += 1024;
			size -= 1024;				
		}
		else
		{
			printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
			GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN); //reset untill app code sent over!
			return ERROR;
		}	
	}		
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN); //reset untill app code sent over!
	SD_FW_Close();
	printf("~~~~~~F35_APP_CODE f/w has been sent!~~~~~~\r\n");
		
	return SUCCESS;		
}

/*********************************************************************************
* Function: SYNA_FW_Write_Block_F35_APP_CODE()
* Description: write F35_APP_CODE to PRAM
* Input: none
* Output: 
* Return: PRAM write result
* Call: internal
*/
ErrorStatus SYNA_FW_Write_Block_APP_CODE(uint32_t area) 
{
	uint32_t BIN_addr, SRAM_addr, size;
	uint16_t len = 0;
	int32_t count = 0;
	uint32_t i = 0, j;
	uint8_t FWContent[1024];	
//	static unsigned char temp = 0;
	uint8_t buf[32], cnt;
	unsigned char one = 0x55;   
	
	if (SD_FW_OPEN() == ERROR)
	{
		printf("FW file open error!\r\n");
		return ERROR;
	}
	
	SRAM_addr = 0; //sram start address
	size = fileSize;
	BIN_addr = area;
	if (f_lseek(&fwfile, BIN_addr) == FR_OK)
	{			 
		SD_FW_Read(FWContent, 36);

		_block_contents.id = (uint32_t) FWContent[0];
		_block_contents.id += (uint32_t) FWContent[1]<<8;				
		_block_contents.id += (uint32_t) FWContent[2]<<16;				
		_block_contents.id += (uint32_t) FWContent[3]<<24;				
		if(_block_contents.id == HEADER_EREA) //check ID ok
		{
			printf("Check Block ID OK!\r\n" );
			for(len=0; len<16 ;len++)
			{
				_block_contents.str[len] = FWContent[4+len];
				printf("%c", _block_contents.str[len]);
			}

			_block_contents.flags = (uint32_t) FWContent[20];				
			_block_contents.flags += (uint32_t) FWContent[21]<<8;					
			_block_contents.flags += (uint32_t) FWContent[22]<<16;					
			_block_contents.flags += (uint32_t) FWContent[23]<<24;					
//			printf("\r\n_block_contents.flags = 0x%x\r\n", _block_contents.flags);			

			_block_contents.destination = (uint32_t) FWContent[24];				
			_block_contents.destination += (uint32_t) FWContent[25]<<8;							
			_block_contents.destination += (uint32_t) FWContent[26]<<16;						
			_block_contents.destination += (uint32_t) FWContent[27]<<24;						
//			printf("_block_contents.destination = 0x%x\r\n", _block_contents.destination);

			_block_contents.len = (uint32_t) FWContent[28];				
			_block_contents.len += (uint32_t) FWContent[29]<<8;					
			_block_contents.len += (uint32_t) FWContent[30]<<16;				
			_block_contents.len += (uint32_t) FWContent[31]<<24;				
//			printf("_block_contents.len = 0x%x\r\n", _block_contents.len);
			
			_block_contents.crc = (uint32_t) FWContent[32];				
			_block_contents.crc += (uint32_t) FWContent[33]<<8;					
			_block_contents.crc += (uint32_t) FWContent[34]<<16;				
			_block_contents.crc += (uint32_t) FWContent[35]<<24;				
//			printf("_block_contents.crc = 0x%x\r\n", _block_contents.crc);
		}			
		else
		{		
			printf("Error:_block_contents.id = 0x%x Block ID NG!\r\n" ,_block_contents.id );
			return ERROR;
		}
	}
	else
	{
		printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
		return ERROR;
	}
	
	if (one != 0) //only run once in this function
	{
		one = 0;
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		Delay_us(10);
		TCH_SPI_ReadByte(buf, 4);	
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);			
		if (buf[1] == 0x1B)
		{
			printf("Receive STATUS report Touchcomm_SPI_Read(RTcode=0x1B)\r\n");	
		}			
		
//		printf("\r\n=========================\r\n");
//		for (len=0; len<4; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}

		Delay_us(10);			
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		Delay_us(10);
		TCH_SPI_ReadByte(buf, 5);	
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		
//		printf("\r\n=========================\r\n");
//		for (len=0; len<5; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}			
	}
//~~~debug 256

	//Write host download command 0x30
	if (area ==_img_contents.area_2)	
	{
		_block_contents.len += 4;
	}
	else if (area == _img_contents.area_3)	
	{
		_block_contents.len += 2;
	}			
	FWContent[0] = 0x30;
	FWContent[1] = (_block_contents.len & 0xff);
	FWContent[2] = ((_block_contents.len>>8) & 0xff);
	FWContent[3] = 0x01;
	if (area ==_img_contents.area_2)	FWContent[4] = 0x01;	
	else if (area ==_img_contents.area_3)	FWContent[4] = 0x02;	
	BIN_addr = area + DATA_OFFSET;						
	if (f_lseek(&fwfile, BIN_addr) == FR_OK)
	{			 
		SD_FW_Read(FWContent+5, 256);
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		Delay_ms(1);
		TCH_SPI_WriteByte(FWContent, 256);
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		Delay_ms(5);	
//		for (j=0; j<256; j++)
//		{
//			printf("%02x ",FWContent[j]);
//			cnt++;
//			if(cnt%16 == 0) printf("\r\n");
//		}						
	}
	else
	{
		printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
		return ERROR;
	}						
//~~~debug 256		
	
	//write data
	if (area ==_img_contents.area_2)	
	{
		_block_contents.len -= 4;	
	}			
	else if (area ==_img_contents.area_3)	
	{
		_block_contents.len -= 2;	
	}			
	size = _block_contents.len - (256 - 5);		
	BIN_addr = area+DATA_OFFSET + (256 - 5);
	if (size % 255)
	{
		count = (size / 255) + 1;
	}
	else 
	{
		count = (size / 255);
	}

	for (i=0 ; i<count; i++)
	{
		len = (size < 255) ? size : 255;
		/*write data to SRAM */
		if (f_lseek(&fwfile, BIN_addr) == FR_OK)
		{			 
			SD_FW_Read(FWContent, len);
			GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
			Delay_ms(1);
			buf[0] = 0x01;					
			TCH_SPI_WriteByte(buf, 1);		
			TCH_SPI_WriteByte(FWContent, len);
			if (len < 255 && (area ==_img_contents.area_2))
			{
				buf[0] = 0x00;
				buf[1] = 0x00;					
				TCH_SPI_WriteByte(buf, 2);					
			}
			GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
			Delay_ms(5);

//			for (j=0; j<len; j++)
//			{
//				printf("%02x ", FWContent[j]);
//				cnt++;
//				if (cnt%16 == 0) printf("\r\n");
//			}
				
			SRAM_addr += 255;
			BIN_addr += 255;
			size -= 255;				
		}
		else
		{
			printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
			return ERROR;
		}	
	}	
	
	SD_FW_Close();
	if (area ==_img_contents.area_2)
	{
		printf("\r\n~~~~~~f/w APP_CONFIG has been sent!~~~~~~\r\n");
	}
	else if (area ==_img_contents.area_3)	
	{
		printf("\r\n~~~~~~f/w DISPLAY has been sent!~~~~~~\r\n");	
	}
	
	return SUCCESS;
}	

/*********************************************************************************
* Function: SYNA_HDL_Sequence
* Description: 
* Input: none
* Output: none
* Return: status
* Call: internal
*/
ErrorStatus SYNA_HDL_Sequence(void) 
{ 
	uint8_t buf[32];
	uint8_t	ack, len;
	uint8_t retry = 0;
	uint8_t tolerant = 0x00;
	
	Scan_PDT();
	if (SYNA_FW_GET_CONTENTS() == ERROR)
	{
		return ERROR;
	}
	
//	//check if F35_UBOOT_Query2[1:0] = 2'b11
//	if (((Check_F35_UBOOT_Query2()) & 0x03) != 0x03)
//	{
//		printf("\r\n error end return!\r\n");
//		return ERROR;
//	}
	
//FW_DOWNLOAD:
	while (1)
	{	
		if (tolerant++ > ERROR_RETRY_TIMES)
		{
			printf("\r\n\r\n/*============== FW DOWNLOAD: ERROR ================*/\r\n\r\n");	
			return ERROR;
		}
	
		//step 1
		printf("\r\n[%.3f]============FW DOWNLOAD: F35_APP_CODE=============\r\n", TIMESTAMP);
		SYNA_FW_Write_Block_F35_APP_CODE(_img_contents.area_1); //F35_APP_CODE write			
		Detect_ATTN();
		retry = 0;
		while (Get_Identify_Report() != SUCCESS)		
		{
			if (retry > ERROR_RETRY_TIMES)
			{
				printf("\r\n\r\n/*============== FW DOWNLOAD: ERROR ================*/\r\n\r\n");	
				return ERROR;					
			}			
			printf("\r\n[warning]: F35_APP_CODE download fail, start to retry %d time!\r\n", (++retry));
			if (TEST_MODE != TEST_MODE_RA) //RA mode TP_RST cant not be reset!
			{
				DriverIC_RST_TP();
			}
			SYNA_FW_Write_Block_F35_APP_CODE(_img_contents.area_1);				
			Detect_ATTN();
		}
		
		//step 2	
		printf("\r\n[%.3f]============FW DOWNLOAD: CONFIG=============\r\n", TIMESTAMP);		
		SYNA_FW_Write_Block_APP_CODE(_img_contents.area_2);		 //CONFIG
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
	//	Detect_ATTN();
		Delay_ms(1);
		TCH_SPI_ReadByte(buf, 4);	//A5 addr Len 00
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);				
//		printf("\r\n============APP_CONFIG ACK1=============\r\n");
//		for (len=0; len<4; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}
		Delay_us(10);
		if (buf[1] != 0x01) //if APP_CONFIG download error
		{
			printf("\r\n[ERROR]:APP_CONFIG Download NG!\r\n ");
			continue;
		}
		ack = buf[2] + 3;	
		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
		Delay_ms(1);
		TCH_SPI_ReadByte(buf, ack);	
		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);				
//		printf("\r\n=============APP_CONFIG ACK2 ============\r\n");
//		for (len=0; len<ack; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}
		Delay_us(10);	
		
		//step 3
//		printf("\r\n[%.3f]============FW DOWNLOAD: DISPLAY=============\r\n", TIMESTAMP);		
//		SYNA_FW_Write_Block_APP_CODE(_img_contents.area_3);			 //DISPLAY 
//		Delay_ms(10);
//		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
//		Delay_us(10);
//		TCH_SPI_ReadByte(buf, 4);	
//		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
//	//	Detect_ATTN();
//		printf("\r\n=============DISPLAY ACK1============\r\n");
//		for (len=0; len<4; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}
//		if (buf[1] != 0x01) //if DISPLAY download error
//		{
//			printf("\r\n[ERROR]:DISPLAY Download NG!\r\n ");
//			continue;
//		}
//		ack = buf[2] + 3;
//		GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
//		Delay_us(10);
//		TCH_SPI_ReadByte(buf, ack);	//A5 03 xx xx 5A
//		GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);	
//		printf("\r\n=============DISPLAY ACK2============\r\n");
//		for(len=0; len<ack; len++)
//		{
//			printf("%02x\t", buf[len]);
//		}
		
		break;
	}

	printf("\r\n\r\n[%.3f]/*============== FW DOWNLOAD: SUCCESS ================*/\r\n\r\n", TIMESTAMP);		
	return SUCCESS;
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
////	if (TEST_MODE != TEST_MODE_RA)
////	{
////		printf("\r\n/*============ OTP HDL mode check ============*/\r\n");
////		OTP_HDL_MODE();
////	}	
if (TEST_MODE == TEST_MODE_CTP ||
	  TEST_MODE == TEST_MODE_ET1 || 
	  TEST_MODE == TEST_MODE_OTP )
	{
		printf("\r\n/*============ OTP HDL mode check ============*/\r\n");
		OTP_HDL_MODE();
	}	
	TCH_SPI_Config();
	if (SYNA_HDL_Sequence() != SUCCESS)
	{
		ret = ERROR;
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
ErrorStatus RA_Program_FW(void)
{
  ErrorStatus ret = SUCCESS;

	printf("\r\nBegin FW Program!\r\n");
	TCH_SPI_CSN_PIN = TCH_SPI_CSN1_PIN;
	TCH_SPI_CSN_GPIO_PORT = TCH_SPI_CSN1_GPIO_PORT;
	if (Program_FW())
	{
		printf("Panel 1 FW Program success!\r\n");
	}
	else
	{
		printf("Panel 1 FW Program error!\r\n");
		ret = ERROR;
	}
//	Read_TouchCom(IDENTIFYS);
//	Delay_ms(10);
//	Read_TouchCom(GET_APP_INFO);		
	
	TCH_SPI_CSN_PIN = TCH_SPI_CSN2_PIN;
	TCH_SPI_CSN_GPIO_PORT = TCH_SPI_CSN2_GPIO_PORT;
	if (Program_FW())
	{
		printf("Panel 2 FW Program success!\r\n");
	}
	else
	{
		printf("Panel 2 FW Program error!\r\n");
		ret = ERROR;
	}
//	Read_TouchCom(IDENTIFYS);
//	Delay_ms(10);
//	Read_TouchCom(GET_APP_INFO);	
	
	TCH_SPI_CSN_PIN = TCH_SPI_CSN3_PIN;
	TCH_SPI_CSN_GPIO_PORT = TCH_SPI_CSN3_GPIO_PORT;
	if (Program_FW())
	{
		printf("Panel 3 FW Program success!\r\n");
	}
	else
	{
		printf("Panel 3 FW Program error!\r\n");
		ret = ERROR;
	}
//	Read_TouchCom(IDENTIFYS);
//	Delay_ms(10);
//	Read_TouchCom(GET_APP_INFO);	
//	Delay_ms(200);
//	IC_Init(RA_InitCode);
	
	TCH_SPI_UNConfig();
	return ret;
}
