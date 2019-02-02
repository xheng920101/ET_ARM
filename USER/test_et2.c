#include "include.h"

/*********************************************************************************
* Function: Test_ET2
* Description: ET2 test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_ET2(void)
 {
 	TOTAL_DIS_NUM = 11;
 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(0, 0, 0, 0);	//waku+black crosstalk
		
			SPEC_NORMAL_IOVCC_MAX =	50.0;
			SPEC_NORMAL_VSP_MAX =	50.0;
			SPEC_NORMAL_VSN_MAX	=	50.0;		
			break;
		case (1): 
			FPGA_Info_Visible(INFO_NONE);
		 
			FPGA_DisPattern(22, 127, 127, 127);	//flicker
			break;
		case (2): 
			FPGA_DisPicture(0);	//picture
			break;
		case (3): 
			FPGA_DisPicture(1);	//picture
			break;
		case (4): 
			FPGA_DisPicture(2);	//picture
			break;
		case (5): 
			FPGA_DisPattern(0, 255, 255, 255);	
			break;
		case (6): 
			FPGA_DisPattern(1, 255, 255, 255);	
			break;
		case (7): 
			FPGA_DisPattern(2, 255, 255, 255);	
			break;
		case (8): 
			FPGA_DisPattern(3, 255, 255, 255);	
			break;
		case (9): 
			FPGA_DisPicture(3);	//picture
			break;
		case (10): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPicture(4);	//picture
			break;
		case (11): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPicture(5);	//picture
			break;
		case (12): 
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
		case (13): 
			FPGA_DisPattern(0, 0, 0, 0);
			KEY_SLEEPIN(); 
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
