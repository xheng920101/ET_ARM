#include "include.h"

/*********************************************************************************
* Function: Test_ET3
* Description: ET3 test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_ET3(void)
 {
 	TOTAL_DIS_NUM = 11;
 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(83, 127, 127, 127);	//waku+black crosstalk		
			break;
		case (1): 
			FPGA_Info_Visible(INFO_NONE);
		  ScanForward();
		  FPGA_DisPattern(0, 0, 0, 0);	//black
		  Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (2): 
		  LCD_PWM(0xFF);
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanBackward();
			FPGA_DisPattern(0, 127, 127, 127);	//gray127
			Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (3): 
			LCD_PWM(0x0D);	//1mA
		  FPGA_Info_Visible(INFO_NONE);
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255);	//low current white
			break;
		case (4): 
			LCD_PWM(0xFF);
			FPGA_DisPattern(0, 255, 255, 255);	//white
			if (Pic_Load_Finish == SET || PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
			{
				Delay_ms(ET_DLY_LOCK * 2700); 
			}
			break;
		case (5): 
			FPGA_DisPattern(90, 0, 0, 0);	//WRGB transtion
			break;
		case (6): 
			FPGA_DisPattern(0, 255, 0, 0); 	//red
			break;
		case (7): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(127, 255, 255, 255);	//RGB
			break;
		case (8): 
			if (Pic_Load == ERROR)
			{
				FPGA_Info_Visible(INFO_STR);
				FPGA_Info_Set((uint8_t *)"LOAD PIC FAIL.");
			}
			FPGA_DisPicture(0);	//picture
			break;
		case (9): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(22, 127, 127, 127);	//flicker
//			LCD_SleepIn();
//			LCD_VideoMode_OFF();
//			MIPI_SleepMode_ON();
			break;
//		case (10): 
//			FPGA_DisPattern(24, 127, 127, 127);	//dotcheck
//			LCD_SleepIn();
//			LCD_VideoMode_OFF();
//			MIPI_SleepMode_ON();
//			break;
		/////////////////////////////////add pattern here for different customer///////////////
		/////////////////////////////////change the TOTAL_DIS_NUM value and last case num after add///////////////
//		case (11): 
//			FPGA_DisPattern(0, 255, 0, 0);	//red
//			MIPI_SleepMode_OFF();
//			LCD_SleepOut();
//			LCD_VideoMode_ON();
//			LCD_HSMode();
//			break;		
		
		/////////////////////////////////add pattern here for different customer///////////////
		case (10): 
			FPGA_DisPattern(0, 0, 0, 0);
			KEY_SLEEPIN(); 
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
