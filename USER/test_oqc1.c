#include "include.h"

/*********************************************************************************
* Function: Test_OQC1
* Description: OQC1 test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_OQC1(void)
 {
 	TOTAL_DIS_NUM = 11;
 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(81, 127, 0x77, 0x41);	//waku+black
			break;
		case (1):
			FPGA_Info_Visible(INFO_NONE);
		  ScanForward();
		  FPGA_DisPattern(0, 0, 0, 0);	//black
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
			LEDA_DIM();
		  FPGA_Info_Visible(INFO_NONE);
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255);	//low current white
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
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(90, 0, 0, 0);	//WRGB transtion
			break;
		case (6): 
			if (Pic_Load == ERROR)
			{
				FPGA_Info_Visible(INFO_STR);
				FPGA_Info_Set((uint8_t *)"LOAD PIC FAIL.");
			}
			FPGA_DisPicture(0);	//picture
			break;
		case (7): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(22, 127, 127, 127);	//flicker
			break;
		case (8): 
			FPGA_DisPattern(24, 127, 127, 127);	//dotcheck
			break;
		case (9): 
			FPGA_DisPattern(1, 127, 127, 127);	//crosstalk
			break;
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
