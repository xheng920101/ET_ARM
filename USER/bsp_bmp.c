#include "include.h" 

ErrorStatus Pic_Load = SUCCESS;
FlagStatus Pic_Load_Finish = RESET;
BYTE pColorData[8000];
FATFS bmpfs[2];
FIL bmpfsrc; 
FRESULT bmpres;

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5) | ((B)>>3)))  
#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)		//����24λ���ɫ ÿһ�е����ؿ�ȱ�����4�ı���  ����0����
#define GETR_FROM_RGB16(RGB565)  ((unsigned char)((((unsigned short int )RGB565) >> 11) << 3))		  	// R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)((((unsigned short int )(RGB565 & 0x7ff)) >> 5)<< 2)) 	// G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)((((unsigned short int )(RGB565 & 0x1f)) << 3)))       	// B
	
#define Bank1_LCD_D				((u32)0x6C000000) //Disp Data ADDR      
#define FPGA_FSMC_WR_Data(val)	((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val))) //write data to GRAM

/*********************************************************************************
* Function: showBmpHead
* Description: printf BMP file header, for debug
* Input: -pBmpHead, pointer to BMP file header
* Output: none
* Return: none
* Call: internal
*/
static void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
	Debug_Printf("λͼ�ļ�ͷ:\r\n");
	Debug_Printf("�ļ���С:%ld\r\n", (*pBmpHead).bfSize);
	Debug_Printf("������:%ld\r\n", (*pBmpHead).bfReserved1);
	Debug_Printf("������:%ld\r\n", (*pBmpHead).bfReserved2);
	Debug_Printf("ʵ��λͼ���ݵ�ƫ���ֽ���:%ld\r\n", (*pBmpHead).bfOffBits);
	Debug_Printf("\r\n");	
}

/*********************************************************************************
* Function: showBmpInforHead
* Description: printf BMP file information header, for debug
* Input: -pBmpInforHead, pointer to BMP file information header
* Output: none
* Return: none
* Call: internal
*/
static void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
	Debug_Printf("λͼ��Ϣͷ:\r\n");
	Debug_Printf("�ṹ��ĳ���:%d\r\n", (*pBmpInforHead).biSize);
	Debug_Printf("λͼ��:%d\r\n", (*pBmpInforHead).biWidth);
	Debug_Printf("λͼ��:%d\r\n", (*pBmpInforHead).biHeight);
	Debug_Printf("ƽ����:biPlanes = %d\r\n", (*pBmpInforHead).biPlanes);
	Debug_Printf("������ɫλ��:biBitCount = %d\r\n", (*pBmpInforHead).biBitCount);
	Debug_Printf("ѹ����ʽ:%d\r\n", (*pBmpInforHead).biCompression);
	Debug_Printf("ʵ��λͼ����ռ�õ��ֽ���:biSizeImage = %d\r\n", (*pBmpInforHead).biSizeImage);
	Debug_Printf("X����ֱ���:%d\r\n", (*pBmpInforHead).biXPelsPerMeter);
	Debug_Printf("Y����ֱ���:%d\r\n", (*pBmpInforHead).biYPelsPerMeter);
	Debug_Printf("ʹ�õ���ɫ��:%d\r\n", (*pBmpInforHead).biClrUsed);
	Debug_Printf("��Ҫ��ɫ��:%d\r\n", (*pBmpInforHead).biClrImportant);
	Debug_Printf("\r\n");
}

/*********************************************************************************
* Function: Lcd_Load_bmp
* Description: read picture from SD card, then transfer to FPGA by FSMC
* Input: -Bmp_Width
* Input: -Bmp_Hight
* Input: -Pic_Name, picture name, including file path
* Output: none
* Return: load result
* Call: external
*/
ErrorStatus Lcd_Load_bmp(LONG Bmp_Width, LONG Bmp_Hight, unsigned char *Pic_Name)
{
	unsigned char tmp_name[20];
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;
	LONG width, height;
	uint16_t num;
	unsigned int read_num;
	uint8_t i;
	
	//----------------------- open the file and check its type -----------------------
	sprintf((char*)tmp_name, "0:%s", Pic_Name);
	f_mount(0, &bmpfs[0]);
	Debug_Printf("File mount ok \r\n");  
	Debug_Printf("%s\r\n", tmp_name);  
	bmpres = f_open(&bmpfsrc, (char *)tmp_name, FA_OPEN_EXISTING | FA_READ);	

	if (bmpres == FR_OK)
	{
		Debug_Printf("Open file success\r\n");

		f_read(&bmpfsrc, &fileType, sizeof(WORD), &read_num); //read file head
		if (fileType != 0x4d42)	//BMP file type: "BM"
		{
			Debug_Printf("Picture is not a .bmp file!\r\n");
			f_close(&bmpfsrc);
			return ERROR;
		}
		else
		{
			Debug_Printf("Picture is a .bmp file!\r\n");	
		}        

		f_read(&bmpfsrc, &bitHead, sizeof(tagBITMAPFILEHEADER), &read_num);        
		showBmpHead(&bitHead);

		f_read(&bmpfsrc,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
		showBmpInforHead(&bitInfoHead);
		width = bitInfoHead.biWidth;
		height = bitInfoHead.biHeight;
	} 
	else
	{
		Debug_Printf("File open fail!\r\n");
		return ERROR;
	}
	
	if (bitInfoHead.biBitCount < 24) //24 bit true color picture
	{
		Debug_Printf("Picture is not 24 bit true color.\r\n");
		f_close(&bmpfsrc);
		return ERROR;
	}
	
#ifdef SINGLE_PORT	
	if ((width != Bmp_Width) || (height != Bmp_Hight)) //picture resolution
	{
		printf("Picture is not %d RGB x %d bmp pitcture.\r\n", Bmp_Width, Bmp_Hight);
		f_close(&bmpfsrc);
		return ERROR;
	}
#else
	if ((width != (Bmp_Width << 1)) || (height != Bmp_Hight)) //picture resolution
	{
		printf("Picture is not %d RGB x %d bmp pitcture.\r\n", (Bmp_Width << 1), Bmp_Hight);
		f_close(&bmpfsrc);
		return ERROR;
	}
#endif

	//----------------------- load data -----------------------	
	
	for (i = 0; height > 0; height--)
	{
		num = bitInfoHead.biWidth * 3;
		f_lseek(&bmpfsrc, (height - 1) * num + 0x36);
		f_read(&bmpfsrc, pColorData, num, &read_num);
		
#ifdef SINGLE_PORT
		for (width = 0; width < num; width += 2)
		{
			FPGA_FSMC_WR_Data((WORD)(pColorData[width] << 8) | (pColorData[width + 1]));
		}
#else
		num /= 2;//WQHD dul-port sent rgb data, num should be half
		for (width = 0; width < num; width += 3)
		{
			FPGA_FSMC_WR_Data((WORD)((pColorData[num + width + 2] << 8) | (pColorData[num + width + 1])));
			FPGA_FSMC_WR_Data(((WORD)(pColorData[num + width] << 8) | (pColorData[width + 2])));
			FPGA_FSMC_WR_Data((WORD)((pColorData[width + 1] << 8) | (pColorData[width])));
		}
#endif
	}
	
	//****************************************************************************
	//2014-7-25 luosijian  
	//For FHD picture, transfer extra padding bytes to form entire SDRAM burst
	//1 FHD picture size: 1080 * 1920 * 3 = 6220800 bytes
	//SDRAM: data bus width = 16 bits = 2 bytes
	//1 full page burst: 512 * 2 bytes = 1024 bytes
	//We use 2 SDRAM to store pictures, so 1 burst contains:   bytes
	//6220800 / 2048 = 3037.5
	//For each SDRAM, need 3037 and a half bursts. 
	//Need to transfer: 256(half burst) * 2 (bytes) * 2(SDRAM chips) = 1024 bytes
	//****************************************************************************
#ifdef SINGLE_PORT	
	num = (Bmp_Width * Bmp_Hight * 3 % 2048) / 2;
#else
	num = (Bmp_Width * Bmp_Hight * 2 % 1024) / 2;
#endif
	for (; num > 0; num--)
	{
			FPGA_FSMC_WR_Data(0x0000);
	}	
	
	f_close(&bmpfsrc); 
	return SUCCESS;   
}

/*********************************************************************************
* Function: PIC_Load_BMP
* Description: load several picture from sdcard to FPGA
* Input: -picNum, picture number to be loaded
* Output: none
* Return: load result
* Call: external
*/
ErrorStatus PIC_Load_BMP(uint8_t picNum)
{	
	uint8_t i;
	uint8_t otp_times = OTP_TIMES;
	uint8_t picName[6];
	uint8_t picSize;
	uint16_t sdramBurstNum;

	FPGA_Info_Set((uint8_t *)"OQC_V2P0 XS05");
	FPGA_Info_Visible(INFO_STR);

#ifdef SINGLE_PORT	
	{
		sdramBurstNum = ((FPGA_porchPara.HACT * FPGA_porchPara.VACT * 3.0) / 1024 / 2 + 0.99);  //����ȡ��
		picSize = ((FPGA_porchPara.HACT * FPGA_porchPara.VACT * 3.0) / 1024 / 1024 + 0.99);	  //����ȡ��		
	}
#else
	{
		sdramBurstNum = ((FPGA_porchPara.HACT * 2 * FPGA_porchPara.VACT * 3.0) / 1024 / 3 + 0.99);  //����ȡ��
		picSize = ((FPGA_porchPara.HACT * 2 * FPGA_porchPara.VACT * 3.0) / 1024 / 1024 / 3 + 0.99);	  //����ȡ��
	} 
#endif
	//para1: send enable
	//para2: how many pictures to be sent		 
	//para3: how mayn SDRAM bursts need to store 1 picture: (HSUM * VSUM * 24) / (512 * 16)	/ SDRAM_NUM
	//para4: SDRAM size reserved for 1 picture: (HSUM * VSUM * 3) / 1024 / 1024 / SDRAM_NUM	 (uint: MB)
	FPGA_PIC_WR_CFG(1, picNum, sdramBurstNum, picSize);
	
	for (i = 0; i < picNum; i++)
	{
		OTP_TIMES = picNum - i;
		FPGA_Info_Visible(INFO_STR | INFO_OTPTIMES);
		sprintf((char*)picName, "%d%s", i, ".bmp");
		printf("%s\r\n", picName);
		if (Lcd_Load_bmp(FPGA_porchPara.HACT, FPGA_porchPara.VACT, picName) == ERROR)
		{
			OTP_TIMES = otp_times;
			FPGA_Info_Set((uint8_t *)"LOAD PIC FAIL.");
			FPGA_Info_Visible(INFO_STR);
			return ERROR;
		}
	}

	OTP_TIMES = otp_times;
	FPGA_Info_Set((uint8_t *)"");
	FPGA_Info_Visible(INFO_NONE);
	return SUCCESS;
}

/*********************************************************************************
* Function: PIC_Load_BMP_ET
* Description: load a picture from sdcard to FPGA
* Input: -picName, picture name to be loaded
* Output: none
* Return: load result
* Call: external
*/
ErrorStatus PIC_Load_BMP_ET(uint8_t * picName)
{
	uint8_t picSize;
	uint16_t sdramBurstNum;

#ifdef SINGLE_PORT	
	{
		sdramBurstNum = ((FPGA_porchPara.HACT * FPGA_porchPara.VACT * 3.0) / 1024 / 2 + 0.99);  //����ȡ��
		picSize = ((FPGA_porchPara.HACT * FPGA_porchPara.VACT * 3.0) / 1024 / 1024 + 0.99);	  //����ȡ��		
	}
#else
	{
		sdramBurstNum = ((FPGA_porchPara.HACT * 2 * FPGA_porchPara.VACT * 3.0) / 1024 / 3 + 0.99);  //����ȡ��
		picSize = ((FPGA_porchPara.HACT * 2 * FPGA_porchPara.VACT * 3.0) / 1024 / 1024 / 3 + 0.99);	  //����ȡ��
	} 
#endif
	//para1: send enable
	//para2: how many pictures to be sent		 
	//para3: how mayn SDRAM bursts need to store 1 picture: (HSUM * VSUM * 24) / (512 * 16)	/ SDRAM_NUM
	//para4: SDRAM size reserved for 1 picture: (HSUM * VSUM * 3) / 1024 / 1024 / SDRAM_NUM	 (uint: MB)
	FPGA_PIC_WR_CFG(1, 1, sdramBurstNum, picSize);
	if (Lcd_Load_bmp(FPGA_porchPara.HACT, FPGA_porchPara.VACT, picName) == ERROR)
	{
		Pic_Load = ERROR;
		Pic_Load_Finish = SET;
//		FPGA_Info_Set((uint8_t *)"LOAD PIC FAIL.");
//		FPGA_Info_Visible(INFO_STR);
		return ERROR;
	}
	Pic_Load_Finish = SET;
	return SUCCESS;
}

/*********************************************************************************
* Function: SD_Write_Str2File
* Description: write string into a file witch is saved at sdcard
* Input: -File_Name, file name with extension
* Input: -Write_String
* Output: none
* Return: save result
* Call: external
*/
ErrorStatus SD_Write_Str2File(char *File_Name, char *Write_String)
{ 
	char File_Path[32];
	
	sprintf(File_Path, "0:%s", File_Name);
	f_mount(0, &bmpfs[0]);
	bmpres = f_open(&bmpfsrc, File_Path, FA_OPEN_ALWAYS|FA_WRITE);
	if (bmpres == FR_OK)
	{
		f_lseek(&bmpfsrc, f_size(&bmpfsrc));
		f_puts(Write_String, &bmpfsrc);
	}
	else
	{
		Debug_Printf("\r\n f_open(%s) Failed !\n", File_Path); 
		return ERROR;
	}
	
	f_close(&bmpfsrc);  
	Debug_Printf("f_close file %s\r\n", File_Path);
	return SUCCESS;	
}
