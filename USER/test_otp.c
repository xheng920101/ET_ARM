#include "include.h" 

float flicker_best;
float white;
float black;
float chroma_x;
float chroma_y;
float chroma_Lv;

float chroma_u;
float chroma_v;

double Gamma, dif;
char OTPInfo_FilePath[32];
char OTP_FLAG = 0x80;

/*********************************************************************************
* Function: OTPInfo_SaveFile_Check
* Description: check if sdcard avaible, scan current avaible file to save OTP information
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void OTPInfo_SaveFile_Check(void)
{
	unsigned char num;
	FATFS fs[2];
	FIL fsrc; 
	
	if (disk_initialize(0)) //check SDCARD
	{
		FPGA_Info_Visible(INFO_STR);
		FPGA_Info_Set((uint8_t *)"SDCARD ERROR");
		FPGA_DisPattern(114, 0, 0, 0);
		printf("SDCARD is not available, or SDCARD maybe overflow!\r\n");
		SDCard_NG = SET;
		return;
	}
	
//	f_mount(0, &fs[0]);
	f_mount(&fs[0], (TCHAR*)"0:", 0);
	for (num = 0; num < 255; num++)
	{
		sprintf(OTPInfo_FilePath, "0:%s_%d.xls", (char *)PROJECT_NO, num);				
		if (f_open(&fsrc, OTPInfo_FilePath, FA_OPEN_EXISTING) == FR_OK)
		{
			if (f_size(&fsrc) > 10000000) //10M as upper limit for one file
			{
				f_close(&fsrc); 
				continue;
			}
			else 
			{
				f_close(&fsrc); 
				return;
			}
		}
		else
		{
			f_open(&fsrc, OTPInfo_FilePath, FA_OPEN_ALWAYS|FA_WRITE);
			f_lseek(&fsrc, f_size(&fsrc));
			f_puts("Result\tOTP times\tFlicker\tVCOM\tDB\tContrast\tLv_white\tLv_black\tChroma\tx\ty\tu\tv\tLv\tGamma\tindex\r\n", &fsrc);
			f_close(&fsrc); 
			return;
		}
	}
}

/*********************************************************************************
* Function: SD_Write_OTPInfo
* Description: write otp information into file that save in the SDCARD
* Input: -OTPInfo
* Output: none
* Return: none
* Call: internal
*/
void SD_Write_OTPInfo(char* OTPInfo)
{
	FATFS fs[2];
	FIL fsrc; 
	if (auto_line)
	{
			//nop
	}
	else
	{
//		f_mount(0, &fs[0]);
		f_mount(&fs[0], (TCHAR*)"0:", 0);
		f_open(&fsrc, OTPInfo_FilePath, FA_OPEN_ALWAYS|FA_WRITE);
		f_lseek(&fsrc, f_size(&fsrc));
		f_puts(OTPInfo, &fsrc);	
		f_close(&fsrc);	
	}
}

/*********************************************************************************
* Function: Test_OTP
* Description: OTP test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_OTP(void)
 {
 	TOTAL_DIS_NUM = 3;
 	switch (DIS_NUM) 
	{
		case (0):
			if (GAMMAEXPERT == SET)
			{
				printf("*#*#OTP Stop#*#*\r\n");
				if (OTP_TIMES >= OTP_TIMES_MAX)
				{
					if (OTP_TIMES < 3)
					{
						printf("*#*#OTP times is 1 or 2#*#*\r\n");
					}
					else
					{
						printf("*#*#OTP times is over MAX#*#*\r\n");			
					}	
//				printf("*#*#OTP times is over MAX#*#*\r\n");
//				FPGA_Info_Visible(INFO_NONE);	
//				FPGA_DisPattern(85, 0, 0, 0);
//				Delay_sec(1);	
//				LCD_SleepIn();
//				LCD_VideoMode_OFF();
//				MIPI_SleepMode_ON();
//				LCMPower_OFF();
//				LED_ON(RED);
//				LED_OFF(BLUE);
////					GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
//					GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
//				while(1);	
				}			
			}
			
			if (auto_line)
			{
				;//nop
			}
			else
			{
				FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO | INFO_OTPTIMES);
				FPGA_DisPattern(22, 127, 127, 127);
				OTPInfo_SaveFile_Check();
				
				SPEC_MAX_IOVCC =	SPEC_MAX_RED_IOVCC;
				SPEC_MAX_VSP =	SPEC_MAX_RED_VSP;
				SPEC_MAX_VSN	=	SPEC_MAX_RED_VSN;		
				SPEC_LEDA_MAX =	SPEC_MAX_LEDA_NORMAL;								//mA
				SPEC_LEDA_MIN	=	SPEC_MIN_LEDA_NORMAL;	
				printf("\r\nCURRENT SPEC -> Flicker: %.2f < LEDA < %.2f;\r\n", SPEC_LEDA_MIN, SPEC_LEDA_MAX);	
			}
			break;
		case (1):
//			GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
			GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
			FPGA_Info_Visible(INFO_NONE);
			LED_ON(BLUE);

			if (GAMMAEXPERT == SET)
			{
				printf("*#*#OTP Begin#*#*\r\n");
				Delay_ms(100);
			}
			else
			{
				if (OTP() == SUCCESS)
				{
					OTP_TIMES = OTPTimes_Read();
					FPGA_Info_Visible(INFO_OTPTIMES);					
					FPGA_DisPattern(0, 0, 255, 0);	//add green pattern indicate otp pass
					if (!auto_line)	Delay_ms(500);	//freeze display for 500mS 
					while (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_PIN) == RESET);		//wait key release.
					printf("\r\n*#*#E:TEST END#*#*\r\n");//2017-1-20 for sim request 				
					LCD_SleepIn();
					LCD_VideoMode_OFF();
					MIPI_SleepMode_ON();
					LCMPower_OFF();	
					LED_ON(GREEN);
					LED_OFF(BLUE);
	//				GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
					GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
					Delay_sec(120);
					GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
					while(1);	
				}
				else
				{	
					FPGA_Info_Visible(INFO_NONE);					
					FPGA_DisPattern(0, 255,0, 0);	//add RED pattern indicate otp NG
					if (!auto_line) Delay_ms(500);
					while (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_PIN) == RESET);		//wait key release.
					printf("\r\n*#*#E:SLEEP IN#*#*\r\n");
					LCD_SleepIn();
					LCD_VideoMode_OFF();
					MIPI_SleepMode_ON();
					LCMPower_OFF();
					LED_ON(RED);
					LED_OFF(BLUE);
	//				GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //old
					GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
					Delay_sec(120);
					GPIO_SetBits(TEST31_GPIO_PORT, TEST31_PIN); //new
					while(1);
				}	
			}
//			break;
		case (2):
			if (GAMMAEXPERT == SET)
			{
				printf("*#*#OTP Stop#*#*\r\n");
				DIS_NUM = 0;
				Delay_ms(100);
			}
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
 }

/*********************************************************************************
* Function: OTP
* Description: OTP test
* Input: none
* Output: none
* Return: OTP test result
* Call: external
*/
ErrorStatus OTP(void)
{
	ErrorStatus otp_result = ERROR;
	char Str_Temp[128];
	uint8_t auto_vcom_result;
			
	if (OTP_TIMES >= OTP_TIMES_MAX) //otp times check
	{
		FPGA_Info_Set((uint8_t *)"OTP TIMES NG");
		FPGA_Info_Visible(INFO_STR);
		FPGA_DisPattern(114, 0, 0, 0);
		printf("OTP times is over %d!\r\n", OTP_TIMES_MAX);
		Delay_ms(5);
		printf("\r\n*#*#OTP:NG#*#*\r\n");
		sprintf(Str_Temp, "NG\t%d\tOK\t0X%04X\t%5.2f\tNG\t%6.2f\t%4.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
		SD_Write_OTPInfo(Str_Temp);
		return ERROR;
	}
	
	if ((OTP_FLAG & 0x40) == 0x40) //constrast
	{
		if (Contrast_Check() == ERROR)
		{
			printf("\r\n*#*#OTP:NG#*#*\r\n");
			Delay_ms(5);					
			printf("\r\n*#*#OTP(0X%02X):NG$flicker OK 0X%04X %05.2f$contrast NG %06.2f %04.2f$xyLv OK %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
			sprintf(Str_Temp, "NG\t%d\tOK\t0X%04X\t%5.2f\tNG\t%6.2f\t%4.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
			SD_Write_OTPInfo(Str_Temp);
			return ERROR;
		}
	}
	
	if ((OTP_FLAG & 0x20) == 0x20) //chroma
	{
		if (Chroma_Check() == ERROR)
		{
			printf("\r\n*#*#OTP:NG#*#*\r\n");
			Delay_ms(5);					
			printf("\r\n*#*#OTP(0X%02X):NG$flicker OK 0X%04X %05.2f$contrast OK %06.2f %04.2f$xyLv NG %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
			sprintf(Str_Temp, "NG\t%d\tOK\t0X%04X\t%5.2f\tOK\t%6.2f\t%4.2f\tNG\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
			SD_Write_OTPInfo(Str_Temp);
			return ERROR;
		}
	}
		
	if ((OTP_FLAG & 0x10) == 0x10) //gamma
	{
		if (Gamma_Check() == ERROR)
		{
			printf("\r\n*#*#OTP:NG#*#*\r\n");
			Delay_ms(5);					
			printf("\r\n*#*#OTP(0X%02X):NG$flicker OK 0X%04X %05.2f$contrast OK %06.2f %04.2f$xyLv OK %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
			sprintf(Str_Temp, "NG\t%d\tOK\t0X%04X\t%5.2f\tOK\t%6.2f\t%4.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tNG\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
			SD_Write_OTPInfo(Str_Temp);
			Delay_ms(5);
			printf("\r\n*#*#GAMMA1:NG$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
#*#*\r\n");
			Delay_ms(5);
			printf("\r\n*#*#GAMMA2:NG$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
#*#*\r\n");
			return ERROR;
		}
	}	
	
	if ((OTP_FLAG & 0x80) == 0x80)
	{
		auto_vcom_result = AutoVcom();
		if (auto_vcom_result != 0)
		{
			printf("\r\n*#*#OTP:NG#*#*\r\n");
			Delay_ms(5);					
			printf("\r\n*#*#OTP(0X%02X):NG$flicker NG 0X%04X %05.2f$contrast OK %06.2f %04.2f$xyLv OK %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
			sprintf(Str_Temp, "NG\t%d\tNG(%d)\t0X%04X\t%5.2f\tOK\t%6.2f\t%4.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, auto_vcom_result, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
			SD_Write_OTPInfo(Str_Temp);
		  FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		  FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(114, 0, 0, 0);
			return ERROR;
		}
	}
	printf("[%9.3f] NVM PROGRAM START...\r\n",TIMESTAMP);
	otp_result = OTPSequence();
	printf("[%9.3f] NVM PROGRAM FINISH!\r\n",TIMESTAMP);
	if (otp_result == SUCCESS)
	{
		printf("\r\n*#*#OTP:OK#*#*\r\n");
		//2016-12-14 for sim manufacture log record
		printf("\r\n*#*#5:%d,0X%04X,%5.2f,%6.2f,%04.2f,%06.2f,%5.3f,%5.3f,%5.3f,%5.3f,%.2f#*#*\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_Lv, chroma_x, chroma_y, chroma_u, chroma_v, Gamma);
	
		Delay_ms(5);					
		printf("\r\n*#*#OTP(0X%02X):OK$flicker OK 0X%04X %05.2f$contrast OK %06.2f %04.2f$xyLv OK %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
		sprintf(Str_Temp, "OK\t%d\tOK\t0X%04X\t%5.2f\tOK\t%6.2f\t%04.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
		SD_Write_OTPInfo(Str_Temp);
	}
	else
	{
		printf("\r\n*#*#OTP:NG#*#*\r\n");
		Delay_ms(5);					
		printf("\r\n*#*#OTP(0X%02X):NG$flicker OK 0X%04X %05.2f$contrast OK %06.2f %04.2f$xyLv OK %05.3f %05.3f %05.3f %05.3f %06.2f#*#*\r\n", OTP_FLAG, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv);
		sprintf(Str_Temp, "NG\t%d\tOK\t0X%04X\t%5.2f\tOK\t%6.2f\t%4.2f\tOK\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.2f\tOK\t%.2f\r\n", OTP_TIMES, vcom_best, flicker_best, white, black, chroma_x, chroma_y, chroma_u, chroma_v, chroma_Lv, Gamma);
		SD_Write_OTPInfo(Str_Temp);
	}						

	if ((OTP_FLAG & 0x10) == 0x10)
	{
		Delay_ms(5);
		printf("\r\n*#*#GAMMA1:OK$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
#*#*\r\n");
		Delay_ms(5);
		printf("\r\n*#*#GAMMA2:OK$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
$G000 0.290 0.310 000.23$G016 0.290 0.310 001.25$G032 0.290 0.310 100.23$G255 0.290 0.310 500.23\
#*#*\r\n");				
	}
	
	return otp_result;
}

/*********************************************************************************
* Function: AutoVcom
* Description: find the best VCOM
* Input: none
* Output: none
* Return: result
* Call: external
*/
uint8_t AutoVcom(void)
{
	float flicker_max;
	float flicker_min;
	float flicker_temp;
	uint16_t vcom_old;
	uint16_t vcom_temp;
	uint8_t keepCnt;

	 flicker_max = 0;
	 flicker_min = 100;
	 vcom_old = vcom_best;
	 printf("[%9.3f] vcom_old = 0x%04X\r\n",TIMESTAMP, vcom_old);

	 FPGA_DisPattern(22, 127, 127, 127);
	 Delay_ms(100);

	 vcom_temp = vcom_old;
	 keepCnt = 0;
	 while (keepCnt < KEEPNUM)
	 {
//	 	FPGA_DisPattern(0, 0, 0, 0);
//	 	Delay_ms(40);
	 	VCOM_Set(vcom_temp);
//		FPGA_DisPattern(22, 127, 127, 127);
	 	Delay_ms(100);
		if (MSE_State == ERROR)
		{
			flicker_temp = DSENSOR_I2C_GetFlicker(CHN1);
		}
		else
		{
			Meas_Flicker();
			flicker_temp = Flicker;
		}
		printf("[%9.3f] vcom_temp = 0x%04X\r\n",TIMESTAMP, vcom_temp);
		printf("[%9.3f] flicker_temp = %f\r\n",TIMESTAMP,flicker_temp);
		if (flicker_temp == 0)
		{
			VCOM_Set(vcom_old);
			vcom_best = vcom_old;
		  FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		  FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(114, 0, 0, 0);
			return 1; //sensor no ack
		}
		if (flicker_max <= flicker_temp)
		{
			flicker_max = flicker_temp;
		}
		if (flicker_min >= flicker_temp)
		{
			flicker_min = flicker_temp;
			vcom_best = vcom_temp;
			keepCnt = 0;
		}
		else
		{
			keepCnt++;
		}
		vcom_temp++;
	 }

 	 vcom_temp = vcom_old;
	 keepCnt = 0;
	 while (keepCnt < KEEPNUM)
	 {
//	 	FPGA_DisPattern(0, 0, 0, 0);
//	 	Delay_ms(40);
	 	VCOM_Set(vcom_temp);
//		FPGA_DisPattern(22, 127, 127, 127);
	 	Delay_ms(100);
		 if (MSE_State == ERROR)
		 {
			 flicker_temp = DSENSOR_I2C_GetFlicker(CHN1);
		 }
		 else
		 {
			 Meas_Flicker();
			 flicker_temp = Flicker;
		 }
		 printf("[%9.3f] vcom_temp = 0x%04X\r\n", TIMESTAMP , vcom_temp);
		 printf("[%9.3f] flicker_temp = %f\r\n", TIMESTAMP , flicker_temp);
		 
		 if (flicker_temp == 0)
		 {
			 VCOM_Set(vcom_old);
			 vcom_best = vcom_old;
		   FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		   FPGA_Info_Visible(INFO_STR);
			 FPGA_DisPattern(114, 0, 0, 0);
			 return 1; //sensor no ack
		 }
		 if (flicker_max <= flicker_temp)
		 {
			 flicker_max = flicker_temp;
		 }
		 if (flicker_min >= flicker_temp)
		 {
			 flicker_min = flicker_temp;
			 vcom_best = vcom_temp;
			 keepCnt = 0;
		 }
		 else
		 {
			 keepCnt++;
		 }		

		 if (vcom_temp == 0) break;
		 vcom_temp--;
	 }

	 /////////////////////////zxj@20160322////////////////////////////////////////foolproof
	 if (vcom_best > 0x80) vcom_temp = vcom_best - 20;
	 else vcom_temp = vcom_best + 20;
	 VCOM_Set(vcom_temp);
	 Delay_ms(200);
	 if (MSE_State == ERROR)
	 {
		 flicker_max = DSENSOR_I2C_GetFlicker(CHN1);
	 }
	 else
	 {
		 Meas_Flicker();
		 flicker_max = Flicker;
	 }
	 printf("[%9.3f] vcom_temp = 0x%04X\r\n", TIMESTAMP , vcom_temp);
	 printf("[%9.3f] flicker_temp = %f\r\n", TIMESTAMP , flicker_max);
	 /////////////////////////zxj@20160322////////////////////////////////////////
	 
	 if (flicker_max - flicker_min < 5)
	 {
		 VCOM_Set(vcom_old);
		 printf("[%9.3f] Waring: flicker_max - flicker_min < 5\r\n",TIMESTAMP);
		 printf("[%9.3f] Waring: The  senor may be wrong! Please try again or replace the sensor!\r\n",TIMESTAMP);
		 FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 0, 0, 0);
		 return 2; //flicker almost no change
	 }

//	 FPGA_DisPattern(0, 0, 0, 0);
//	 Delay_ms(40);
	 VCOM_Set(vcom_best);
//	 FPGA_DisPattern(22, 127, 127, 127);
   Delay_ms(200);

	 if (MSE_State == ERROR)
	 {
		 flicker_temp = DSENSOR_I2C_GetFlicker(CHN1);
		 flicker_temp = DSENSOR_I2C_GetFlicker(CHN1);
		 flicker_best = flicker_temp;
		 printf("[%9.3f] vcom_temp = 0x%04X\r\n",TIMESTAMP, vcom_best);
		 printf("[%9.3f] flicker_temp = %f\r\n",TIMESTAMP, flicker_temp);
		 if ((flicker_temp > SPEC_FLICKER_PERCENT) || (flicker_temp - flicker_min) > 5) 
		 {
			 printf("[%9.3f] Waring: flicker drift too much or flicker value is too large!\r\n",TIMESTAMP);
		   FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		   FPGA_Info_Visible(INFO_STR);
			 FPGA_DisPattern(114, 0, 0, 0);
			 return 3; //flicker not meet the SPEC.
		 }

		 printf("[%9.3f] Best VCOM is 0x%04X(%f)\r\n",TIMESTAMP, vcom_best, flicker_temp);
		 return 0;
	 }
	 else
	 {
		 Meas_JEITA();
		 Meas_JEITA();
		 flicker_temp = Flicker;
		 flicker_best = flicker_temp;
		 printf("[%9.3f] vcom_temp = 0x%04X\r\n",TIMESTAMP, vcom_best);
		 printf("[%9.3f] flicker_temp = %f (dB) \r\n",TIMESTAMP, flicker_temp);
		 if ((flicker_temp > SPEC_FLICKER_DB)) 
		 {
			 printf("[%9.3f] Waring: flicker drift too much or flicker value is too large!\r\n",TIMESTAMP);
		   FPGA_Info_Set((uint8_t *)"FLICKER_NG");
		   FPGA_Info_Visible(INFO_STR);
			 FPGA_DisPattern(114, 0, 0, 0);
			 return 3; //flicker not meet the SPEC.
		 }
			
		 printf("[%9.3f] Best VCOM is 0x%04X(%f(dB))\r\n",TIMESTAMP, vcom_best, flicker_temp);
		 return 0;		 
	 }
 }

/*********************************************************************************
* Function: Gamma_Check
* Description: check if gamma meet the spec.
* Input: none
* Output: none
* Return: result
* Call: external
*/
 ErrorStatus Gamma_Check(void)
 {	 
	 uint8_t n, gray;
	 uint16_t gamma_l, gamma_r, gamma_m;
	 double gamma_side, dif_side;
	 double gamma_gray[9]; // 0 32 64 96 128 160 192 224 255
	 
	 if (MSE_State == ERROR)
	 {
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_Info_Set((uint8_t *)"MSE ERROR");
		 FPGA_DisPattern(114, 0, 0, 0);
		 printf("MSE error! Please check the MSE connection!\r\n");
		 return ERROR;
	 }
 
	 gray = 0;
	 for (n=0; n<8; n++)
	 {
		 FPGA_DisPattern(0, gray, gray, gray);
		 Delay_ms(100);
	   Meas_Yxy();
		 gamma_gray[n] = Lv;
		 gray = gray + 32;
	 }	 
	 FPGA_DisPattern(0, 255, 255, 255);
	 Delay_ms(200);
	 Meas_Yxy();
	 gamma_gray[8] = Lv;	 
	 
	 gamma_l = 160;
	 gamma_r = 280;	 	 
	 while(1)
	 {
		 gamma_m = (gamma_l + gamma_r) / 2;
		 Gamma = gamma_m / 100.0;
		 gamma_side = Gamma + 0.01;
		 dif = 0;
		 dif_side = 0;
		 gray = 0;
		 for (n=0; n<8; n++)
		 {
			 dif += pow(pow((gray/255.0), Gamma) - ((gamma_gray[n] - gamma_gray[0]) /(gamma_gray[8] - gamma_gray[0])), 2);
			 dif_side += pow(pow((gray/255.0), gamma_side) - ((gamma_gray[n] - gamma_gray[0]) /(gamma_gray[8] - gamma_gray[0])), 2);
			 gray = gray + 32;
		 }
		 printf("\r\ngamma_l = %0.2f, gamma_r = %f\r\n", gamma_l / 100.0, gamma_r / 100.0);
		 printf("gamma = %0.2f, dif = %f\r\n", Gamma, dif);
		 printf("gamma_side = %0.2f, dif_size = %f\r\n", gamma_side, dif_side);
		 
		 if (dif > dif_side)
		 {
			 gamma_l = gamma_m;
		 }
		 else
		 {
			 gamma_r = gamma_m;
		 } 
		 
		 if (gamma_r - gamma_l <= 1) break;
	 }
	 
	 if (dif < dif_side)
	 {
		 Gamma = Gamma;
		 dif = dif;
	 }
	 else
	 {
		 Gamma = gamma_side;
		 dif = dif_side;
	 }
	 printf("\r\ngamma_min = %0.2f, dif_min = %f\r\n", Gamma, dif);
	
	 if ((Gamma < SPEC_GAMMA - SPEC_GAMMA_RANGE) || (Gamma > SPEC_GAMMA + SPEC_GAMMA_RANGE))
	 {
		 printf("Gamma does not meet specifications!\r\n");
		 printf("Gamma specifications is (%0.2f, %0.2f)!\r\n", SPEC_GAMMA - SPEC_GAMMA_RANGE, SPEC_GAMMA + SPEC_GAMMA_RANGE);
		 FPGA_Info_Set((uint8_t *)"GAMMA_NG");
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 0, 0, 0);
		 return ERROR;
	 }
	 return SUCCESS;
}

/*********************************************************************************
* Function: Chroma_Check
* Description: check if chroma meet the spec.
* Input: none
* Output: none
* Return: result
* Call: external
*/
 ErrorStatus Chroma_Check(void)
 {		
	 if (MSE_State == ERROR)
	 {
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_Info_Set((uint8_t *)"MSE ERROR");
		 FPGA_DisPattern(114, 0, 0, 0);
		 printf("MSE error! Please check the MSE connection!\r\n");
		 return ERROR;
	 }
 	 
	 FPGA_DisPattern(0, 255, 255, 255);
	 Delay_ms(200);
	 Meas_Yxy();
	 chroma_Lv = Lv;
	 chroma_x = color_x;
	 chroma_y = color_y;
	 
	 Meas_Yuv();
	 chroma_u = color_u;
	 chroma_v = color_v;
	 
	 if (chroma_Lv < SPEC_Lv)
	 {
		 printf("Brightness does not meet specifications!\r\n");
		 printf("Brightness specifications is %0.2f!\r\n", SPEC_Lv);
		 FPGA_Info_Set((uint8_t *)"WHITE_NG");
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 255, 255, 255);
		 return ERROR;
	 }
	 if ((chroma_x < SPEC_x - SPEC_xy_RANGE) || (chroma_x > SPEC_x + SPEC_xy_RANGE) || (chroma_y < SPEC_y - SPEC_xy_RANGE) || (chroma_y > SPEC_y + SPEC_xy_RANGE))
	 {
		 printf("Chroma does not meet specifications!\r\n");
		 printf("Chroma specifications is (%0.3f, %0.3f) +- %0.3f!\r\n", SPEC_x, SPEC_y, SPEC_xy_RANGE);
		 FPGA_Info_Set((uint8_t *)"CHROMA_NG");
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 255, 255, 255);
		 return ERROR;
	 }
	 return SUCCESS;	
}
 
/*********************************************************************************
* Function: Contrast_Check
* Description: check if contrast meet the spec.
* Input: none
* Output: none
* Return: result
* Call: external
*/
ErrorStatus Contrast_Check(void)
{
	 float contrast;
	
	 if (MSE_State == ERROR)
	 {
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_Info_Set((uint8_t *)"MSE ERROR");
		 FPGA_DisPattern(114, 0, 0, 0);
		 printf("MSE error! Please check the MSE connection!\r\n");
		 return ERROR;
	 }
	 
	 FPGA_DisPattern(0, 255, 255, 255);
	 Delay_ms(200);
	 Meas_Yxy();
	 white = Lv;
	 FPGA_DisPattern(0, 0, 0, 0);
	 Delay_ms(200);
	 Meas_Yxy();
	 black = Lv;
	 
	 contrast = white / (black + 0.0001);
	 printf("Contrast is %0.1f!\r\n", contrast);
	 if (contrast < SPEC_CONTRAST)
	 {
		 printf("Contrast does not meet specifications!\r\n");
		 printf("Contrast specifications is %0.1f!\r\n", SPEC_CONTRAST);
		 FPGA_Info_Set((uint8_t *)"CONTRAST_NG");
		 FPGA_Info_Visible(INFO_STR);
		 FPGA_DisPattern(114, 0, 0, 0);
		 return ERROR;
	 }
	 return SUCCESS;
}
