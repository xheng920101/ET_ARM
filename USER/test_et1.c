#include "include.h"

/*********************************************************************************
* Function: Test_ET1
* Description: ET1 test
* Input: none
* Output: none
* Return: none
* Call: external
*/
		uint8_t BLU_ON;

void Test_ET1(void)
 {
 	TOTAL_DIS_NUM = 5;	
 	switch (DIS_NUM) 
	{
//		case (0): 
//			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
//			FPGA_DisPattern(24, 127, 127, 127);	//dotcheck
//			break;
//		case (1):	
//			FPGA_Info_Visible(INFO_NONE);
		case (0):			
			BLU_ON=RESET;
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			ScanForward();	
 			FPGA_DisPattern(0, 0, 0, 0);	//black
			Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (1):
			BLU_ON=SET;
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanBackward();
 			FPGA_DisPattern(0, 127, 127, 127);	//gray
			Delay_ms(ET_DLY_LOCK * 3700); 
			break;
		case (2): 
			BLU_ON=RESET;
			FPGA_Info_Visible(INFO_NONE);
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255);	//white
		  Delay_ms(ET_DLY_LOCK * 700); 
			break;
		case (3): 
			BLU_ON=SET;
			FPGA_DisPattern(127, 255, 255, 255);	//RGB
			break;
		case (4): 
			FPGA_DisPattern(0, 0, 0, 0);
			KEY_SLEEPIN(); 
		  Flag_Test_Current = RESET;				  //DO NOT test the current!
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
