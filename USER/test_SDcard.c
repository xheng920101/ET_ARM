#include "include.h"

char 	VERSION_DISPLAY[10];
char 	PRO_DISPLAY[20];
char	Patten_Buf[1024] = {0};
unsigned  char  LPatNum = 0;
FlagStatus SD_MODE_ERROR = RESET;
FlagStatus SD_MODE_AG = RESET;
double resist=0;

ErrorStatus get_RGBGray(uint8_t DisNum,	const char *InFile,	uint8_t *PatNum, uint8_t *Rgray, uint8_t *Ggray, uint8_t *Bgray, int *DeTime, uint16_t *SetPwm)
{
	char StrBuf[8], PatBuf[30];
	char *FiPa ;
	
	sprintf(StrBuf, ">>%d:", DisNum);
	FiPa = strstr(InFile, StrBuf);
//	sprintf(StrBuf,">>%d:",DisNum+1);
	if (FiPa != NULL)
	{
		FiPa += strlen(StrBuf);
		strncpy(PatBuf, FiPa, 30);
//		strtok(PatBuf,",");
		*PatNum = (uint8_t)atoi(strtok(PatBuf, ","));
//		strtok(NULL,",");
		*Rgray = (uint8_t)atoi(strtok(NULL, ","));
//		strtok(NULL,",");
		*Ggray = (uint8_t)atoi(strtok(NULL, ","));
//		strtok(NULL,";");
		*Bgray = (uint8_t)atoi(strtok(NULL, ";"));
//		strtok(NULL,";");
		*DeTime =(int)(1000 * atof(strtok(NULL, ";")));
		FiPa = strtok(NULL, "\r");
		if (*FiPa == 'L') 
    {
		  SPEC_LEDA_MIN = SPEC_MIN_LEDA_DIM;
		  SPEC_LEDA_MAX	= SPEC_MAX_LEDA_DIM;

			*SetPwm = 0x00C6;	//1mA for 12bit
//		  *SetPwm = 0x000D;	//1mA for 8bit
			LEDA_DIM();
		}
		else
		{
			 SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;
	     SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;	
			*SetPwm = 0xFFF;
			LEDA_NORM();
		}
		
		if (*FiPa == 'B') 
    {
       ScanBackward();
		}		
		return SUCCESS;
	}
	else
	{
		sprintf(StrBuf, ">>%d:", DisNum - 1);		
		FiPa = strstr(InFile, StrBuf);
		sprintf(StrBuf, ">>#end");
		
		if ((strstr(InFile, StrBuf) - FiPa) < 30)
		{
			*PatNum = 254;
			*Rgray = 0;
			*Ggray = 0;
			*Bgray = 0;
			return SUCCESS;
		}
		return ERROR;
	}
}

void TEST_MODE_Init(void)
{	
#ifdef	SDCARD_MODE
	char *Pattern_P = NULL;
	char txt_name[30];
//	char *nameBuf = NULL,*tempBuf2=NULL;
	char temp_Cha[5]={0}, temp_Mod[5]={0}, temp_Ver[5]={0},temp_num[5]={0};//temp_str[20]={0},temp_Pro[15]={0},
//	unsigned short readBit;
//	uint16_t cnt;
	TCHAR *path = (TCHAR*)"0:\\";
	FATFS txtfs;
	FIL txtfsrc; 
	FRESULT frs;
	DIR my_dir;
	UINT* br;
	static FILINFO MyInfo;
	uint8_t cntFile = 0;
	
	sprintf(txt_name, "%s*.DAT", PROJECT_NO);
	frs = f_mount(&txtfs, path, 0);
	frs = f_opendir(&my_dir,path);
//	for ( ; ; )	//查询当前目录下的所有文档并打印到串口
//	{
//		frs = f_readdir(&my_dir, &MyInfo);
//		if (frs != FR_OK || MyInfo.fname[0] == 0)	break;
//		printf("%s\r\n", MyInfo.fname);
//	}
	frs =	f_findfirst(&my_dir, &MyInfo, path, (TCHAR*)txt_name);		//查询第一个匹配条件的文件
	for ( ; ; )
	{
		frs = f_findnext(&my_dir, &MyInfo);
		if (frs != FR_OK || MyInfo.fname[0] == 0) break;
		cntFile++ ;
		printf("%s\r\n", MyInfo.fname);
	}
	frs =	f_findfirst(&my_dir, &MyInfo, path, (TCHAR*)txt_name);		//查询第一个匹配条件的文件
//	f_closedir(&my_dir);
	if (cntFile != 0)
	{
		printf("There are more than one File!\r\n");
		sprintf(VERSION_DISPLAY, "TWO FILES");
		SD_MODE_ERROR = SET;
	}
	else if (frs == FR_OK)
	{
		if (f_open(&txtfsrc, (TCHAR *)MyInfo.fname, FA_OPEN_EXISTING | FA_READ) == FR_OK)
		{
//			f_lseek(&txtfsrc, MyInfo.fsize);
//			memcpy(Patten_Buf, txtfsrc.buf, MyInfo.fsize);
			f_read(&txtfsrc,Patten_Buf,1024,br);
			memcpy(PRO_DISPLAY, PROJECT_NO, 14);
			Pattern_P = strstr(Patten_Buf, PRO_DISPLAY);
			if(Pattern_P != NULL)
			{
				Pattern_P += (strlen(PRO_DISPLAY) + strlen(","));
				memcpy(temp_Cha, Pattern_P, (strchr(Pattern_P, ',') - Pattern_P));
				Pattern_P += (strlen(temp_Cha)+strlen(","));
				memcpy(temp_Mod, Pattern_P, (strchr(Pattern_P, ',') - Pattern_P));
				Pattern_P += (strlen(temp_Mod)+strlen(","));
				memcpy(temp_Ver, Pattern_P, (strchr(Pattern_P, ',') - Pattern_P));
				Pattern_P += (strlen(temp_Ver)+strlen(","));
				memcpy(temp_num, Pattern_P, (strchr(Pattern_P, '\r') - Pattern_P));
				Pattern_P = strstr(Patten_Buf, "#begin");
				sprintf(VERSION_DISPLAY, "%s_%s", temp_Mod, temp_Ver);
				sprintf(PRO_DISPLAY, "%s_%s", PRO_DISPLAY, VERSION_SDmode);
		    TOTAL_DIS_NUM=atoi(temp_num)+1;
				printf("TOTAL_DIS_NUM=%d\r\n", TOTAL_DIS_NUM);
				sprintf(txt_name, "%s_%s_%s.dat", PROJECT_NO, temp_Mod, temp_Ver);
				
				if (strcmp(txt_name, MyInfo.fname) == 0)
				{
					
					if (strcmp(temp_Mod, "ET1") == 0)	
					{						
						SD_MODE_AG = SET;	
						TEST_MODE = TEST_MODE_ET1;
					}
					else if (strcmp(temp_Mod, "ET2") == 0)	TEST_MODE = TEST_MODE_ET2;
					else if (strcmp(temp_Mod, "ET3") == 0)	TEST_MODE = TEST_MODE_ET3;
					else if (strcmp(temp_Mod, "OQC2") == 0)	TEST_MODE = TEST_MODE_ET2;	
					else if (strcmp(temp_Mod, "CTP") == 0)	TEST_MODE = TEST_MODE_CTP;	
					else	TEST_MODE = TEST_MODE_ET2;	
//				Pattern_P += strlen("#begin\r\n");
				}
				else
				{
					printf("Read File of version is ERROR!\r\n");
					sprintf(VERSION_DISPLAY, "VERSION_ERROR");
					SD_MODE_ERROR = SET;
				}
			}
			else
			{
				printf("Read Project Name is ERROR!\r\n");
				sprintf(VERSION_DISPLAY, "FILE_ERROR!");
				SD_MODE_ERROR = SET;
			}		
		}
		else 
		{
			sprintf(VERSION_DISPLAY, "FILE_ERROR!");
			SD_MODE_ERROR = SET;
		}
	}
	else
	{
		sprintf(VERSION_DISPLAY, "SDCARD ERROR");
		SD_MODE_ERROR = SET;
	}
	f_close(&txtfsrc);
	f_closedir(&my_dir);
#else
//		TEST_MODE = TEST_MODE_ET1;
#endif	
}

void Display_Pattern(void)
{
	uint8_t PatNum = 0, Rgray = 0, Ggray = 0, Bgray = 0;
	uint16_t BL_PWM = 0xFFF;
	int DelayTime = 0;
	
	if (get_RGBGray(DIS_NUM, Patten_Buf, &PatNum, &Rgray, &Ggray, &Bgray, &DelayTime, &BL_PWM) == SUCCESS)
	{
		if (PatNum == 255)
		{
			if (Rgray == Ggray && Ggray == Bgray && Rgray ==0)
			{
				FPGA_DisPattern(0, 0, 0, 0);
//				LCD_SleepIn();
//				LCD_LPMode();
				LCD_SleepIn();
	      LCD_VideoMode_OFF();
		    MIPI_SleepMode_ON();
				Delay_ms(DelayTime);
				Flag_Test_Current = RESET;
			}
//			else 
//			{
//				printf("LCM power off");
//				KEY_SLEEPIN();
//			}
		}
		else if (PatNum == 254 && Rgray == 0x11)
		{
			if (Pic_Load_Finish == RESET)	
			{
				FPGA_Info_Set((uint8_t *)"LOAD PIC");
				FPGA_DisPattern(0, 0, 0, 0);
				FPGA_Info_Visible(INFO_STR);
				PIC_Load_BMP_ET((uint8_t *)"0.bmp");
				FPGA_Info_Visible(INFO_NONE);
			}
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPicture(1);
			Delay_ms(DelayTime);
		}
		else if (PatNum == 254 && Rgray == 0x12)
		{
			if (Pic_Load_Finish == RESET)	
			{
				FPGA_Info_Set((uint8_t *)"LOAD PIC");
				FPGA_DisPattern(0, 0, 0, 0);
				FPGA_Info_Visible(INFO_STR);
				PIC_Load_BMP_ET((uint8_t *)"1.bmp");
				FPGA_Info_Visible(INFO_NONE);
			}
			ScanForward();
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPicture(0);
			Delay_ms(DelayTime);
		}
		else if (PatNum == 254 && Rgray == 0)
		{
			FPGA_DisPattern(0, 0, 0, 0);
			KEY_SLEEPIN();
		}
		else
		{
			if (LPatNum == 255) 
			{
//				LCD_SleepOut();
//				LCD_HSMode();
//				LCD_VideoMode_ON();
				MIPI_SleepMode_OFF();		
			  LCD_SleepOut();
			  LCD_HSMode();
			  LCD_VideoMode_ON();
			}
			if (DIS_NUM == 0)	
			{
				ScanForward(); 
				FPGA_Info_Visible(INFO_VERSION|INFO_PROJECT_NO);
			}
			else if(PatNum == 0 && Rgray == 127 && Ggray == 127 && Bgray == 127)
			{
//				ScanBackward();
				FPGA_Info_Visible(INFO_RGBVAL);	
			}
//			else if(PatNum == 0 && Rgray == 64 && Ggray == 64 && Bgray == 64)
//			{
//        FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);			
//			}
			else
			{
				ScanForward();
				FPGA_Info_Visible(INFO_NONE);
			}	
			////////////Ag resistor measurement/////////////
#ifdef AG_TEST			
      if(SD_MODE_AG == SET	)
			{
				if(DIS_NUM == 1) // 
			  {
			  	Delay_ms(100);
			  	resist = GetResValue();				
			    printf("Detect the AG resistor is %f !",resist);
		  	  if( resist >10000 || resist < 0.5)//modify base on test spec.
			    {
			  	 FPGA_DisPattern(114, 0, 0, 0);     //display NG
			  	 while(1)
			  	 {
			  	 	 LED_ON(RED);
			  		 Delay_ms(200);
				  	 LED_OFF(RED);
				  	 Delay_ms(200);
				   }
			    }
	    	}
		  	if(DIS_NUM > 1 && DIS_NUM < TOTAL_DIS_NUM)
		  	{
			  	while(GetResValue() < 10000)//modify base on test spec.
			    {
			  	  FPGA_DisPattern(139, 0, 0, 0);     //开盖
			  	  FPGA_Info_Set((uint8_t *)"OPEN_AG");
			    }
				  FPGA_Info_Visible(INFO_NONE);
			  }
			 }
#endif			
			FPGA_DisPattern(PatNum, Rgray, Ggray, Bgray);
//			LCD_PWM(BL_PWM);
			FPGA_SPI3Write(ADDR_PIC_WR_NUM);
      FPGA_SPI3Write(SD_MODE_PIC);
			if (PatNum == 0 && Rgray == 255 && Ggray == 255 && Bgray == 255 && BL_PWM == 0xFFF)
			{
//				if (Pic_Load_Finish == SET || PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
				if (PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
				{
					Delay_ms(DelayTime); 
				}
			}			
			else if (PatNum == 0 && Rgray == 0 && Ggray == 0 && Bgray == 0 )
			{
//				if (Pic_Load_Finish == SET || PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
				if (PIC_Load_BMP_ET((uint8_t *)"1.bmp") == ERROR)
				{
					Delay_ms(DelayTime); 
				}
			}
			else
			{
				Delay_ms(DelayTime);
			}
		}

		LPatNum = PatNum;
	}
}
