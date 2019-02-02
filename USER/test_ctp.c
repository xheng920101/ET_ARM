#include "include.h" 

/*********************************************************************************
* Function: Test_CTP
* Description: CTP test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_CTP(void)
 {
 	TOTAL_DIS_NUM = 13;
 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(83, 127, 127, 127);	//waku+black crosstalk
			MIPI_SleepMode_OFF();
			LCD_SleepOut();
#ifdef CMD_MODE
			LCD_VideoMode_OFF();
#else
			LCD_VideoMode_ON();
#endif
			LCD_HSMode();
			break;
		case (1): 
			FPGA_Info_Visible(INFO_NONE);
		  FPGA_DisPattern(22, 127, 127, 127);	//flicker
			break;
		case (2): 
			FPGA_DisPattern(23, 127, 127, 127);	//pixelcheck
			break;
		case (3): 
			FPGA_DisPattern(24, 127, 127, 127);	//dotcheck
			break;
		case (4): //18*30 chessboard
			FPGA_DisPattern(112, 0, 59, 63);	//FHD
//			FPGA_DisPattern(112, 0, 39, 42);	//HD
//			FPGA_DisPattern(112, 0, 79, 84);	//WQHD
			break;
		case (5): 
			FPGA_DisPattern(0, 0, 0, 0);	//black
			break;
		case (6):
			FPGA_Info_Visible(INFO_NONE);
		  ScanForward();			
			FPGA_DisPattern(0, 255, 255, 255);	//white
			break;
		case (7): 
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanBackward();
			FPGA_DisPattern(0, 127, 127, 127); 	//gray
			break;
		case (8): 
			FPGA_Info_Visible(INFO_NONE);
		  ScanForward();
			FPGA_DisPattern(90, 0, 0, 0);	//WRGB transtion		
			break;
		case (9): 
		  FPGA_DisPattern(0, 255, 0, 0);	//red			
			break;
		case (10): 
			FPGA_DisPattern(0, 0, 255, 0);	//green			
			break;
		case (11): 
			FPGA_DisPattern(0, 0, 0, 255);	//blue			
			break;
		case (12): 
			FPGA_Info_Visible(INFO_NONE);
		  FPGA_DisPattern(0, 0, 0, 0);
			LCD_SleepIn();
			LCD_VideoMode_OFF();
			LCD_LPMode();
			MIPI_SleepMode_ON();
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
 }
