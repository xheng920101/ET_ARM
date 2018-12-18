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
* NT36672A defination
 */
typedef enum {
	RESET_STATE_INIT = 0xA0,// IC reset
	RESET_STATE_MAX  = 0xAF
} RST_COMPLETE_STATE;

#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)
#define BLD_CRC_EN(a)	    (a | 0x80)
#define EVENT_BUF_ADDR           0x21C00
#define EVENT_MAP_RESET_COMPLETE 0x21C60
#define EVENT_MAP_FWINFO         0x21C78
#define EVENT_MAP_PROJECTID      0x21C9A
#define RAW_PIPE0_ADDR           0x20000
#define RAW_PIPE0_Q_ADDR         0
#define RAW_PIPE1_ADDR           0x23000
#define RAW_PIPE1_Q_ADDR         0
#define BASELINE_ADDR            0x20BFC
#define BASELINE_Q_ADDR          0
#define BASELINE_BTN_ADDR        0x23BFC
#define BASELINE_BTN_Q_ADDR      0
#define DIFF_PIPE0_ADDR          0x206DC
#define DIFF_PIPE0_Q_ADDR        0
#define DIFF_PIPE1_ADDR          0x236DC
#define DIFF_PIPE1_Q_ADDR        0
#define RAW_BTN_PIPE0_ADDR       0x206C4
#define RAW_BTN_PIPE0_Q_ADDR     0
#define RAW_BTN_PIPE1_ADDR       0x236C4
#define RAW_BTN_PIPE1_Q_ADDR     0
#define DIFF_BTN_PIPE0_ADDR      0x20BF0
#define DIFF_BTN_PIPE0_Q_ADDR    0
#define DIFF_BTN_PIPE1_ADDR      0x23BF0
#define DIFF_BTN_PIPE1_Q_ADDR    0
#define READ_FLASH_CHECKSUM_ADDR 0x24000
#define RW_FLASH_DATA_ADDR       0x24002
/* Phase 2 Host Download */   
#define BOOT_RDY_ADDR            0x3F10D
/* BLD CRC */
#define DMA_LENGTH_ADDR          0x3F10E	//0x3F10E ~ 0x3F10F	(2 bytes)
#define ILM_LENGTH_ADDR          0x3F118	//0x3F118 ~ 0x3F119	(2 bytes)
#define DLM_LENGTH_ADDR          0x3F130	//0x3F130 ~ 0x3F131	(2 bytes)
#define DMA_DES_ADDR             0x3F114	//0x3F114 ~ 0x3F116	(3 bytes)
#define ILM_DES_ADDR             0x3F128	//0x3F128 ~ 0x3F12A	(3 bytes)
#define DLM_DES_ADDR             0x3F12C	//0x3F12C ~ 0x3F12E	(3 bytes)
#define ILM_CHECKSUM_ADDR        0x3F100	//0x3F100 ~ 0x3F103	(4 bytes)
#define DMA_CHECKSUM_ADDR        0x3F104	//0x3F104 ~ 0x3F107	(4 bytes)
#define R_ILM_CHECKSUM_ADDR      0x3F120	//0x3F120 ~ 0x3F123 (4 bytes)
#define R_DLM_CHECKSUM_ADDR      0x3F124	//0x3F124 ~ 0x3F127 (4 bytes)
#define DMA_CRC_EN_ADDR          0x3F132
#define BLD_ILM_DLM_CRC_EN_ADDR  0x3F30E
#define BLD_ILM_DLM_CRC_ADDR     0x3F133
#define DMA_CRC_FLAG_ADDR        0x3F134
uint32_t ENG_RST_ADDR  = 0x7FFF80;
uint32_t SWRST_N8_ADDR = 0x03F0C6; 

#define FW_BIN_VER_OFFSET		(0x1A000)
#define FW_BIN_VER_BAR_OFFSET	(0x1A001)

struct nvt_ts_bin_map {
char name[12];
uint32_t BIN_addr;
uint32_t SRAM_addr;
uint32_t size;
uint32_t crc;
};
uint8_t support_hw_crc=1;
static struct nvt_ts_bin_map *bin_map;


FIL fwfile; 
DIR filedir;
uint32_t fileSize;
uint8_t FWContent[1024]; 
uint8_t fwbuf[20];
char FWVersion[12]= "FW";
char FW_project[14];

ErrorStatus SD_FW_OPEN(void){ 
	ErrorStatus ret;
	char file_name[30];
	TCHAR *path = (TCHAR*)"0:\\";
	FATFS filefs;
	FRESULT frs;
	static FILINFO fileInfo;
	uint8_t cntFile = 0;
	uint8_t kk = 0;
	uint8_t jj = 0;
	for ( ; ; ){
		if (kk>13) break;
	  FW_project[jj] = FW_NO[jj];
		jj++ ;
		kk++ ;
	}
	printf("\r\n");
	printf("FW_project No is %s\r\n", FW_project);
	sprintf(file_name, "%s*.bin", FW_project);
	frs = f_mount(&filefs, path, 0);
	frs = f_opendir(&filedir,path);
	frs =	f_findfirst(&filedir, &fileInfo, path, (TCHAR*)file_name);		//????????????
	for ( ; ; ){
		frs = f_findnext(&filedir, &fileInfo);
		if (frs != FR_OK || fileInfo.fname[0] == 0) break;
		cntFile++ ;
		printf("%s\r\n", fileInfo.fname);
	}
	frs =	f_findfirst(&filedir, &fileInfo, path, (TCHAR*)file_name);		//????????????
	if (cntFile != 0){
		printf("There are %d FW files match the project %s\r\n", (cntFile+1), FW_project);
		return ERROR;
	}
	else if (frs == FR_OK){
		printf("\r\n The FW File name:%s",file_name);
		fileSize=fileInfo.fsize;
		printf("\r\n The FW File size:%dbytes",fileSize);
		cntFile = strlen(FW_project) +strlen(Info_IC_Type) + 4;
		strncpy(&FWVersion[2], &fileInfo.fname[cntFile], 5);
		printf("\r\n The FW File vertion:%s",FWVersion);
		if (f_open(&fwfile, (TCHAR *)fileInfo.fname, FA_OPEN_EXISTING | FA_READ) == FR_OK){// FA_OPEN_EXISTING | FA_READ) == FR_OK)
			f_lseek(&fwfile, fileInfo.fsize); //important
			printf("\r\n The FW File has open!\r\n");
			ret = SUCCESS;
			return ret;
		}
		else{
			printf("FW file open fail!\r\n");
			ret = ERROR;
			return ret;
		}
	}else{
		printf("There is no FW file match the project %s\r\n", FW_project);
		ret = ERROR;
		return ret;
	}
}

static void SD_FW_Read(uint8_t * pbuf,UINT Len){
	UINT* br;
	f_read(&fwfile,pbuf,Len,br);
}

void SD_FW_Close(void){
	f_close(&fwfile);
	f_closedir(&filedir);
}
 
/*************************************************/


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
* Function: TCH_SPI Write length Byte mode 0
* Description: IO simiulate SPI write,Send n(n<=256) byte
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
static void TCH_SPI_WriteByte(uint8_t* dataWrt, uint16_t len){
	uint16_t i;
	uint8_t bitPos;
	for(i=0;i<len;i++){
		bitPos = 0x80;
		while (bitPos){
			GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);
			if (dataWrt[i] & bitPos){
				GPIO_SetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
			else{
				GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
			}
			GPIO_SetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);
			bitPos >>= 1;
		}	 
    GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);    //SCK last low
    GPIO_ResetBits(TCH_SPI_MOSI_GPIO_PORT, TCH_SPI_MOSI_PIN);
		Delay_us(1);
	}
}
/*********************************************************************************
* Function:  FLASH_SPI_ReadByte  mode0
* Description: IO simiulate SPI read,read a byte
* Input: none
* Output: none
* Return:  a read Byte
* Call: Internal
*/

static void TCH_SPI_ReadByte(uint8_t* dataRdbuf, uint16_t len){//fall edge
	uint16_t i;
	uint8_t bitPos;
	uint8_t dataRd = 0x00;
	for(i=0;i<len;i++){
		bitPos = 0x80;
		while (bitPos){
			Delay_ms(5);
			GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);
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
		GPIO_ResetBits(TCH_SPI_SCK_GPIO_PORT, TCH_SPI_SCK_PIN);// SCK last low
    dataRdbuf[i]=dataRd;  
    dataRd=0x00;	
		if(TEST_MODE != TEST_MODE_RA){
			Delay_us(10);
		}//ET delay
		else{
			Delay_ms(1);
		}//RA delay
	}
}

 /*********************************************************************************
* Function: TCH_SPI_Write 
* Description: IO simiulate SPI write,Send a byte
* Input: uint32_t addr, uint16_t length , uint8_t* write_data
* Output: none
* Return: none
* Call: Internal
*/
static void TCH_SPI_WordWrite(uint32_t addr, uint16_t length , uint8_t* write_data){
	uint8_t write_buf[4];
	write_buf[0] = 0xFF;  //set index/page/addr command 
	write_buf[1] =(addr >> 15) & 0xFF; //NT36672A addr:23bit
	write_buf[2] =(addr >> 7) & 0xFF;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(write_buf,3);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//CS=1,cancel chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	//---write data to index---
	write_buf[0] = addr&(0x7F);
	write_buf[0] = SPI_WRITE_MASK(write_buf[0]); //bit8 R/W=0/1;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection 
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(write_buf,1);
	TCH_SPI_WriteByte(write_data,length);
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	GPIO_SetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//CS=1,cancel chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
}

/*********************************************************************************
* Function:  TCH_SPI_WordRead
* Description: IO simiulate SPI read,read a byte
* Input: uint32_t ahb_addr, uint16_t length, uint8_t* read_buf
* Output: none
* Return:   read n() Byte
* Call: Internal
*/

static void TCH_SPI_WordRead(uint32_t ahb_addr, uint16_t length, uint8_t* read_buf){
	uint8_t Temp_buf[4];
	Temp_buf[0]=0xFF; // Set index/page/addr command 
	Temp_buf[1] =(ahb_addr >> 15) & 0xFF; //NT36672A addr:23bit
	Temp_buf[2] =(ahb_addr >> 7) & 0xFF;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(Temp_buf,3); //write
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
//---write data to index---
	Temp_buf[0]= ahb_addr&(0x7F);
	Temp_buf[1]= 0x00;
	Temp_buf[0]= SPI_READ_MASK(Temp_buf[0]); //bit8 R/W=0/1;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(Temp_buf,2);
	TCH_SPI_ReadByte(read_buf,length);
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
}

/*********************************************************************************
* Function: 
* Description: WriteSram
* Input: uint8_t* fwCeil, uint32_t start_addr, uint32_t Len
* Output: none
* Return: none
* Call: Internal
*/
static void	WriteSram(uint8_t* fwCeil, uint32_t start_addr, uint32_t Len){
	TCH_SPI_WordWrite(start_addr,Len,fwCeil);
}

/*********************************************************************************
*Description: Novatek touchscreen set index/page/addr address.
* Input: uint32_t addr
* Output: none
* Return: none
* Call: Internal
*/
static void nvt_set_page(uint32_t addr){
	uint8_t buf[4] = {0};
	buf[0] = 0xFF;	//set index/page/addr command
	buf[1] = (addr >> 15) & 0xFF;
	buf[2] = (addr >> 7) & 0xFF;
	GPIO_ResetBits(TCH_SPI_CSN_GPIO_PORT, TCH_SPI_CSN_PIN);//chip selection
	if(TEST_MODE != TEST_MODE_RA){
		Delay_us(10);
	}//ET delay
	else{
		Delay_ms(1);
	}//RA delay
	TCH_SPI_WriteByte(buf,3); //write
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
}
/*********************************************************************************
*Description: Novatek ILM_DLM CRC check enable
* Input: uint32_t addr
* Output: none
* Return: none
* Call: Internal
*/
static void  nvt_bld_crc_en(void){
	uint8_t buf[1];
	//---read  BLD_ILM_DLM_CRC_EN_ADDR status cmds---
	TCH_SPI_WordRead( BLD_ILM_DLM_CRC_EN_ADDR,1,buf);
	buf[0]=BLD_CRC_EN(buf[0]);
	TCH_SPI_WordWrite( BLD_ILM_DLM_CRC_EN_ADDR,1,buf);
	Delay_ms(5);
}
/*********************************************************************************
*Description: Novatek touchscreen set boot ready function.
* Input: uint8_t ready
* Output: none
* Return: none
* Call: Internal
*/
static void nvt_boot_ready(){
	//---write BOOT_RDY status cmds---
	uint8_t buf[1];
	buf[0]=0x01;
	TCH_SPI_WordWrite(BOOT_RDY_ADDR,1,buf);
	Delay_ms(5);
}

/*********************************************************************************
* Description: 	Novatek touchscreen reset MCU (boot) function.
* Input: none
* Output: none
* Return: none
* Call: Internal
*/
static void nvt_bootloader_reset(void){
	uint8_t buf[4];
	buf[0]=0x69;
	TCH_SPI_WordWrite(SWRST_N8_ADDR,1,buf);//NT36672A:0X3F0C6
}

/*********************************************************************************
* Description: 	Novatek touchscreen check FW reset state function.
* Input: check_reset_state
* Output: none
* Return: ErrorStatus
* Call: Internal
*/

static ErrorStatus  nvt_check_fw_reset_state(RST_COMPLETE_STATE check_reset_state){
	uint8_t buf[5] = {0};
	ErrorStatus  ret =SUCCESS;
	int32_t retry = 0;
	while (1){
		Delay_ms(10);
		//---read reset state---
		TCH_SPI_WordRead(EVENT_MAP_RESET_COMPLETE,5,buf);
		printf("read from  rest_state__reg:0x%02X=0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n",EVENT_MAP_RESET_COMPLETE,buf[0], buf[1], buf[2], buf[3], buf[4]);
		if ((buf[0] >= check_reset_state) && (buf[0] <= RESET_STATE_MAX)){
			ret = SUCCESS;
			break;
		}
		retry++;
		if(retry > 100){
			printf("error, retry=%d, rest_state__reg:0x%02X=0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", retry,EVENT_MAP_RESET_COMPLETE,buf[0], buf[1], buf[2], buf[3], buf[4]);
			ret =ERROR;
			break;
		}
	}
	return ret;
}

/*********************************************************************************
*Description:
	Novatek touchscreen checksum function. Calculate bin file checksum for comparison.
* Input: const u8 *data, size_t len
* Output: none
* return: uint32_t data 
* Call: Internal
*/
static uint32_t CheckSum(const uint8_t *data, uint16_t len){
	uint32_t i = 0;
	uint32_t checksum = 0;
	for (i = 0 ; i < len+1 ; i++){
		checksum += data[i];
	}
	checksum += len;
	checksum = ~checksum +1;
	return checksum;
}

static uint32_t byte_to_word(const uint8_t *data){
	return data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
}

/*********************************************************************************
*Description:
	Novatek touchscreen checksum function. Calculate bin file checksum for comparison.
* Input: const u8 *fwdata, size_t fwsize
* Output: none
* return: ErrorStatus  
* Call: Internal
*/
static uint32_t partition = 0;
static uint8_t ilm_dlm_num = 2;
ErrorStatus nvt_bin_header_parser(const uint8_t *fwdata, uint16_t fwsize){
	uint32_t list = 0;
	uint32_t pos = 0x00;
	uint32_t end = 0x00;
	uint8_t info_sec_num = 0;
	uint8_t ovly_sec_num = 0;
	uint8_t ovly_info = 0;
	/* Find the header size */
	printf("\r\n header 0x28, 0x28=%d\n", fwdata[28]);
	end = fwdata[0] + (fwdata[1] << 8) + (fwdata[2] << 16) + (fwdata[3] << 24);
	pos = 0x30;	// info section start at 0x30 offset
	while (pos < end){
		info_sec_num ++;
		pos += 0x10;	/* each header info is 16 bytes */
	}
/*
 * Find the DLM OVLY section
 * [0:3] Overlay Section Number
 * [4]   Overlay Info
 */
	ovly_info = (fwdata[0x28] & 0x10) >> 4;
	ovly_sec_num = (ovly_info) ? (fwdata[0x28] & 0x0F) : 0;
/*
 * calculate all partition number
 * ilm_dlm_num (ILM & DLM) + ovly_sec_num + info_sec_num
 */
	partition = ilm_dlm_num + ovly_sec_num + info_sec_num;
	printf("\r\n ovly_info = %d, ilm_dlm_num = %d, ovly_sec_num = %d, info_sec_num = %d, partition = %d\n",	ovly_info, ilm_dlm_num, ovly_sec_num, info_sec_num, partition);
/* allocated memory for header info */
	bin_map = (struct nvt_ts_bin_map *)malloc((partition+1) * sizeof(struct nvt_ts_bin_map));
	if(bin_map == NULL){
		printf("malloc for bin_map failed!");
		return ERROR;
	}
	else{
		printf("malloc for bin_map success!\n");
	}
  for(list = 0; list < partition; list++){
	/*
	 * [1] parsing ILM & DLM header info
	 * BIN_addr : SRAM_addr : size (12-bytes)
	 * crc located at 0x18 & 0x1C
	 */
		if (list < ilm_dlm_num){
			bin_map[list].BIN_addr = byte_to_word(&fwdata[0 + list*12]);
			bin_map[list].SRAM_addr = byte_to_word(&fwdata[4 + list*12]);
			bin_map[list].size = byte_to_word(&fwdata[8 + list*12]);
			if (support_hw_crc){
				bin_map[list].crc = byte_to_word(&fwdata[0x18 + list*4]);
			}
			else{//support_hw_crc
				if ((bin_map[list].BIN_addr + bin_map[list].size) < fwsize){
					bin_map[list].crc = CheckSum(&fwdata[bin_map[list].BIN_addr], bin_map[list].size);
				}
				else{
					printf("access range (0x%08X to 0x%08X) is larger than bin size!\n",bin_map[list].BIN_addr, bin_map[list].BIN_addr + bin_map[list].size);
					return ERROR;
				}
			} //support_hw_crc
			if (list == 0){
				printf(bin_map[list].name, "ILM");
			}
			else if (list == 1){
				printf(bin_map[list].name, "DLM");
			}
		}
	/*
	 * [2] parsing others header info
	 * SRAM_addr : size : BIN_addr : crc (16-bytes)
	 */
		if ((list >= ilm_dlm_num) && (list < (ilm_dlm_num + info_sec_num))){
		/* others partition located at 0x30 offset */
			pos = 0x30 + (0x10 * (list - ilm_dlm_num));
			bin_map[list].SRAM_addr = byte_to_word(&fwdata[pos]);
			bin_map[list].size = byte_to_word(&fwdata[pos+4]);
			bin_map[list].BIN_addr = byte_to_word(&fwdata[pos+8]);
			if (support_hw_crc){
				bin_map[list].crc = byte_to_word(&fwdata[pos+12]);
			}
			else{
				if((bin_map[list].BIN_addr + bin_map[list].size) < fwsize){
					bin_map[list].crc = CheckSum(&fwdata[bin_map[list].BIN_addr], bin_map[list].size);
				}
				else{ //ts->support_hw_crc
					printf("access range (0x%08X to 0x%08X) is larger than bin size!\n",bin_map[list].BIN_addr, bin_map[list].BIN_addr + bin_map[list].size);
					return ERROR;
				}
			} //ts->support_hw_crc
			/* detect header end to protect parser function */
			if ((bin_map[list].BIN_addr == 0) && (bin_map[list].size != 0)){
				printf(bin_map[list].name, "Header");
			}
			else{
				printf(bin_map[list].name, "Info-%d", (list - ilm_dlm_num));
			}
		}

	/*
	 * [3] parsing overlay section header info
	 * SRAM_addr : size : BIN_addr : crc (16-bytes)
	 */
		if (list >= (ilm_dlm_num + info_sec_num)){
		/* overlay info located at DLM (list = 1) start addr */
			pos = bin_map[1].BIN_addr + (0x10 * (list- ilm_dlm_num - info_sec_num));
			bin_map[list].SRAM_addr = byte_to_word(&fwdata[pos]);
			bin_map[list].size = byte_to_word(&fwdata[pos+4]);
			bin_map[list].BIN_addr = byte_to_word(&fwdata[pos+8]);
			if (support_hw_crc){
				bin_map[list].crc = byte_to_word(&fwdata[pos+12]);
			}
			else{ //ts->support_hw_crc
				if ((bin_map[list].BIN_addr + bin_map[list].size) < fileSize){
					bin_map[list].crc = CheckSum(&fwdata[bin_map[list].BIN_addr], bin_map[list].size);
				}
				else{
					printf("access range (0x%08X to 0x%08X) is larger than bin size!\n",bin_map[list].BIN_addr, bin_map[list].BIN_addr + bin_map[list].size);
					return ERROR;
				}
			} //ts->support_hw_crc
			sprintf(bin_map[list].name, "Overlay-%d", (list- ilm_dlm_num - info_sec_num));
		}
	}
	return SUCCESS;
}

/*********************************************************************************
*Description:
   Novatek touchscreen Write_Partition function, to write firmware into each partition.
* Input: none
* Output: none
* return: ErrorStatus  
* Call: Internal
*/
ErrorStatus Write_Partition(void){
	uint32_t list = 0;
	uint32_t BIN_addr, SRAM_addr, size;
	uint16_t len = 0;
	int32_t count = 0;
	uint32_t i = 0;
	ErrorStatus ret =SUCCESS; 
	char name[12];
	for (list = 0; list < partition; list++){
		/* initialize variable */
		SRAM_addr = bin_map[list].SRAM_addr;
		size = bin_map[list].size;
		BIN_addr = bin_map[list].BIN_addr;
		strcpy(name,bin_map[list].name);
		/* Check data size */
		if ((BIN_addr + size) > fileSize){
			printf("access range (0x%08X to 0x%08X) is larger than bin size!\n",BIN_addr, BIN_addr + size);
			ret =ERROR;
			return ret;
		}
		/* ignore reserved partition (Reserved Partition size is zero) */
		if (!size){
			continue;
		}
		else{
			size = size +1;// important
		}
		if(size % 1024){
			count = (size / 1024) + 1;
		}
		else{
			count = (size / 1024);
		}
		for (i = 0 ; i <count ; i++){
			len = (size < 1024) ? size : 1024;
			/*write data to SRAM */
			if(f_lseek(&fwfile,BIN_addr)==FR_OK){
				SD_FW_Read(FWContent,len);
				WriteSram(FWContent,SRAM_addr,len);
				SRAM_addr += 1024;
				BIN_addr += 1024;
				size -= 1024;
			}
			else{
				printf("\r\n The file pointer  Not  point to the FW File  !\r\n");
				ret=ERROR;
			}
		}
	}
	return ret;
}

void nvt_set_bld_crc(uint32_t DES_ADDR, uint32_t SRAM_ADDR,uint32_t LENGTH_ADDR,uint32_t size,uint32_t G_CHECKSUM_ADDR, uint32_t crc){
	/* write destination address */
	fwbuf[0] = (SRAM_ADDR) & 0xFF;
	fwbuf[1] = (SRAM_ADDR >> 8) & 0xFF;
	fwbuf[2] = (SRAM_ADDR >> 16) & 0xFF;
	TCH_SPI_WordWrite(DES_ADDR,3,fwbuf);
	/* write length */
	fwbuf[0] = (size) & 0xFF;
	fwbuf[1] = (size >> 8) & 0xFF;
	TCH_SPI_WordWrite(LENGTH_ADDR,2,fwbuf);
	/* write golden dlm checksum */
	fwbuf[0] = (crc) & 0xFF;
	fwbuf[1] = (crc >> 8) & 0xFF;
	fwbuf[2] = (crc >> 16) & 0xFF;
	fwbuf[3] = (crc >> 24) & 0xFF;
	TCH_SPI_WordWrite(G_CHECKSUM_ADDR,4,fwbuf);
}

/*********************************************************************************
*Description:
  Novatek touchscreen check hw crc function.
This function will check hw crc result is pass or not.
* Input: uint8_t is_ilm_dlm
* Output: none
* return: ErrorStatus  
* Call: Internal
*/
ErrorStatus Check_HW_CRC(uint8_t is_ilm_dlm){
	uint32_t list = 0;
	uint8_t retry = 0, crc_flag = 0;
	ErrorStatus ret = SUCCESS;
	if (is_ilm_dlm){
		/* polling for ilm & dlm crc check finish */
		while (1){
			TCH_SPI_WordRead(BLD_ILM_DLM_CRC_ADDR,2,fwbuf);
			printf("\r\n Read ilm&&dlm crc flag  %04x is  %02x",BLD_ILM_DLM_CRC_ADDR,fwbuf[0]);// DMA_CRC_FLAG_ADDR//
			/*
			* [0] ILM_CRC_FLAG: 0-ILM CRC pass, 1-ILM CRC error
			* [1] DLM_CRC_FLAG: 0-DLM CRC pass, 1-DLM CRC error
			* [2] CRC_DONE    : 0-CRC checking, 1-finish
			*/
			crc_flag = (fwbuf[0] & 0x07);
			if (crc_flag == 0x04){
				break;
			}
			Delay_ms(1);
			retry++;
			if(retry>20){
				TCH_SPI_WordRead(R_ILM_CHECKSUM_ADDR,8,fwbuf);
				printf("\r\n ilm dlm crc error 0x%02X\n", crc_flag);
				printf("\r\n ILM_BIN_CRC=0x%08X, ILM_HW_CRC=0x%08X\n",bin_map[0].crc, byte_to_word(&fwbuf[0]));
				printf("\r\n DLM_BIN_CRC=0x%08X, DLM_HW_CRC=0x%08X\n",bin_map[1].crc, byte_to_word(&fwbuf[4]));
				return ERROR;
			}
		}
	}
	else{
		for (list = ilm_dlm_num; list < partition; list++){
			/* ignore reserved partition (Reserved Partition size is zero) */
			if (!bin_map[list].size){
				continue;
			}
			/* Detect Header (Header size - 4) */
			if ((bin_map[list].BIN_addr == 0) && (bin_map[list].size != 0)){
				bin_map[list].size = bin_map[list].size - 4;
			}
			/* write register bank */
			nvt_set_bld_crc(DMA_DES_ADDR, bin_map[list].SRAM_addr,DMA_LENGTH_ADDR, bin_map[list].size,DMA_CHECKSUM_ADDR, bin_map[list].crc);
			/* bld dma crc enable */
			fwbuf[0] = 0x01;	//enable
      TCH_SPI_WordWrite(DMA_CRC_EN_ADDR,1,fwbuf);
			/* polling for dma crc check finish */
			while(1){
				TCH_SPI_WordRead(DMA_CRC_FLAG_ADDR,1,fwbuf);
				/*
				* [0] DMA_CRC_FLAG: 0-DMA CRC pass, 1-DMA CRC error
				* [1] DMA_CRC_DONE: 0-CRC checking, 1-finish
				*/
				crc_flag = (fwbuf[0] & 0x03);
				if (crc_flag == 0x02){
					break;
				}
				Delay_ms(1);
				retry++;
				if (retry > 20){
					TCH_SPI_WordRead(R_DLM_CHECKSUM_ADDR,4,fwbuf);
					printf("\r\n partition %d dma crc error 0x%02X\n", list-1, crc_flag);
					printf("\r\n partition %d BIN_CRC=0x%08X, HW_CRC=0x%08X\n",list-1, bin_map[list].crc, byte_to_word(&fwbuf[0]));
					ret=ERROR;
					break;
				}
			}
		}

/* [0] ILM */
/* write register bank */
//nvt_set_bld_crc(ILM_DES_ADDR, bin_map[0].SRAM_addr,ILM_LENGTH_ADDR, bin_map[0].size,G_ILM_CHECKSUM_ADDR, bin_map[0].crc);
		nvt_set_bld_crc(ILM_DES_ADDR, bin_map[0].SRAM_addr,ILM_LENGTH_ADDR, bin_map[0].size,ILM_CHECKSUM_ADDR, bin_map[0].crc);

/* [1] DLM */
/* write register bank */
//nvt_set_bld_crc(DLM_DES_ADDR, bin_map[1].SRAM_addr,DLM_LENGTH_ADDR, bin_map[1].size,G_DLM_CHECKSUM_ADDR, bin_map[1].crc);
		nvt_set_bld_crc(DLM_DES_ADDR, bin_map[1].SRAM_addr,DLM_LENGTH_ADDR, bin_map[1].size,DMA_CHECKSUM_ADDR, bin_map[1].crc);
	}
	return ret;
}

/*********************************************************************************
*Description:
  Novatek touchscreen check checksum function.
This function will compare file checksum and fw checksum.
* Input: none
* Output: none
* return: ErrorStatus  
* Call: Internal
*/
ErrorStatus Check_CheckSum(void){
	uint32_t fw_checksum = 0;
	uint32_t len = partition*4;
	uint32_t list = 0;
	ErrorStatus ret =SUCCESS;
	memset(fwbuf, 0, (len+1));
	/* read checksum */
	TCH_SPI_WordRead(R_ILM_CHECKSUM_ADDR, len+1 , fwbuf);
	/*
	* Compare each checksum from fw
	* ILM + DLM + Overlay + Info
	* ilm_dlm_num (ILM & DLM) + ovly_sec_num + info_sec_num
	*/
	for (list = 0; list < partition; list++){
		fw_checksum = byte_to_word(&fwbuf[1+list*4]);
		/* ignore reserved partition (Reserved Partition size is zero) */
		if(!bin_map[list].size){
			continue;
		}
		if (bin_map[list].crc != fw_checksum){
			printf("[%d] BIN_checksum=0x%08X, FW_checksum=0x%08X\n",list, bin_map[list].crc, fw_checksum);
			printf("\r\n firmware checksum not match!!\n");
			ret=ERROR;
			break;
		}
	}
	//---set xdata index to EVENT BUF ADDR---
	nvt_set_page(EVENT_BUF_ADDR);
	return ret;
}

ErrorStatus Program_FW(void){//Download_Firmware_HW_CRC(void)//Download_Firmware_HW_CRC
	ErrorStatus ret =SUCCESS;
	#ifdef NO_FLASH_MODE
	uint8_t retry = 0;
	TCH_SPI_Config();//spi_config
	if(SD_FW_OPEN()==ERROR)// open fw file
		return ERROR;
	/* Parse Firmware header  Process */
	if(f_lseek(&fwfile,0x00)==FR_OK)
		SD_FW_Read(FWContent,256); //read FW header 256BYTE
	ret =nvt_bin_header_parser(FWContent,256);// parse bin file headre
	if (!ret)
		printf("\r\n parse bin file headre failed.");
	while (1){
		GPIO_SetBits(TEST24_GPIO_PORT, TEST24_PIN);// TP_RST
		Delay_us(5);	//wait tRSTA2BRST after TP_RST
		nvt_bootloader_reset();
		/* Start Write Firmware Process */
		ret = Write_Partition();// send fw to sram
		if (!ret){
			printf("\r\n Write_Firmware failed.");
			goto fail;
		}
    /* clear fw reset status */
//	buf[0]=0xAA;
//	while(1){
  TCH_SPI_WordWrite(EVENT_MAP_RESET_COMPLETE,1, 0x00);
//	TCH_SPI_WordRead(EVENT_MAP_RESET_COMPLETE,1, buf);
//	}
	
   ret = Check_HW_CRC(0);	//Check overlay & Info (also name Check DMA CRC)

  if (!ret) 
  {
  	printf("\r\n check hw crc failed, retry=%d\n", retry);
  	goto fail;
  }
  /* Set BLD_CRC_EN Bit to 1*/
   nvt_bld_crc_en();
  /* Set Boot Ready Bit */
  nvt_boot_ready();
  
  ret = Check_HW_CRC(1);	//Check ILM & DLM
  if (!ret) 
  {
  	printf("\r\n check hw crc failed, retry=%d\n", retry);
  	goto fail;
  } 
  ret = nvt_check_fw_reset_state(RESET_STATE_INIT);
  if (!ret) {
  	printf("\r\n nvt_check_fw_reset_state failed.\n");
  	goto fail;
  }
  else
  { 
  	printf("\r\n nvt_check_fw_reset_state SUCCESS.\n");
  	break;
  }
  
fail:
  retry++;
//  if(retry >3) {
  	printf("\r\n error, retry=%d\n", retry);
  	ret=ERROR;
  	break;
//  }
	}
SD_FW_Close();
TCH_SPI_UNConfig();
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
