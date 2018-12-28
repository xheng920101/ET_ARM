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
 	TOTAL_DIS_NUM = 12;
 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(81, 127, 0x77, 0x41);	//waku+black
			ScanForward();		
			break;
		case (1): 		
			FPGA_Info_Visible(INFO_NONE);
		  ScanForward();
		  FPGA_DisPattern(0, 0, 0, 0);			//black
		  Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (2): 
		  LEDA_NORM();
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanBackward();
			FPGA_DisPattern(0, 127, 127, 127);	//gray127
			Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (3): 
		  FPGA_Info_Visible(INFO_NONE);
			LEDA_DIM();
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255);	//DIM (low current white)
			Delay_ms(100);		  
		  SPEC_LEDA_MIN = SPEC_MIN_LEDA_DIM;
		  SPEC_LEDA_MAX	= SPEC_MAX_LEDA_DIM;
			break;
		case (4): 
			LEDA_NORM();
			FPGA_DisPattern(0, 255, 255, 255);	//white
			if (Pic_Load_Finish == SET || PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
			{
				Delay_ms(ET_DLY_LOCK * 2700); 
			}
			break;
		case (5): 
			FPGA_DisPattern(138, 0, 0, 0);	//WRGB transtion
			break;
		case (6): 			
			FPGA_DisPattern(0, 255, 0, 0); 	//R
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
			break;
		case (10): 
//			FPGA_DisPattern(24, 127, 127, 127);	//dotcheck
//			break;
//		case (11): 
			FPGA_DisPattern(1, 127, 127, 127);	//crosstalk
			break;
//		case (12): 
//			FPGA_DisPattern(0, 0, 0, 0);
//			FPGA_Info_Visible(INFO_NONE);
// 	    LCD_SleepIn();
//	    LCD_VideoMode_OFF();
//		  MIPI_SleepMode_ON();			
//			break;		
//		/////////////////////////////////add pattern here for different customer///////////////
//		case (13):
//			MIPI_SleepMode_OFF();		
//			LCD_SleepOut();
//			LCD_HSMode();		
//			LCD_VideoMode_ON();
//			FPGA_DisPattern(1, 127, 0, 0); 	//crosstalk
//			break;
//		case (14): 		
//			FPGA_DisPattern(0, 255, 0, 0); 	//green
//			break;
//		case (15): 
//			FPGA_DisPattern(0, 0, 0, 255); 	//blue
//			break;
//		case (16): 
//			FPGA_DisPattern(17, 0, 0, 0); 	//»Ò½×bar
//			break;
//		case (17): 
//			FPGA_DisPattern(7, 127, 0, 0); 	//crosstalk
//			break;
//		case (18): 
//			FPGA_DisPattern(133, 255, 0, 0); 	//HUAWEI TP
//			break;
//		case (19): 
//			FPGA_DisPattern(136, 255, 0, 0); 	//HUAWEI S1
//			break;
//		case (20): 
//			FPGA_DisPattern(137, 255, 0, 0); 	//HUAWEI S1
//			break;
		case (11): 
			FPGA_DisPattern(0, 0, 0, 0);
			FPGA_Info_Visible(INFO_NONE);
			KEY_SLEEPIN(); 		
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
