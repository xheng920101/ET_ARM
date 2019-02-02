#include "include.h"

void ESD_Visual_Check(void) 
{
	TOTAL_DIS_NUM = 20; 
	switch (DIS_NUM) 
	{ 		
		case (0):
			LEDA_NORM();
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(79, 0, 0x77, 0x41); //waku
			break; 	
		case (1): 
			FPGA_Info_Visible(INFO_NONE); 
			FPGA_DisPattern(0, 0, 0, 0); //black
			break; 
		case (2):  
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanForward();
			FPGA_DisPattern(0, 64, 64, 64); //gray64
			Delay_ms(ET_DLY_LOCK * 2700); 
			break; 
		case (3): 
			FPGA_Info_Visible(INFO_RGBVAL);			
			ScanBackward();
			FPGA_DisPattern(0, 127, 127, 127); //gray127
			Delay_ms(ET_DLY_LOCK * 1700); 
			break; 
		case (4):  
			FPGA_Info_Visible(INFO_NONE); 
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255); //white
			if (Pic_Load_Finish == SET || PIC_Load_BMP_ET((uint8_t *)"0.bmp") == ERROR)
			{
				Delay_ms(ET_DLY_LOCK * 3700); 
			}
			break;
		case (5): 
			FPGA_DisPattern(18, 0, 0, 0); //gray bar-H
			break;	
		case (6): 
			FPGA_DisPattern(17, 0, 0, 0); //Gray bar-V
			break;
		case (7): 
			FPGA_DisPattern(22, 127, 127, 127); //Flicker	
			break;
		case (8): 
			FPGA_DisPattern(1, 127, 0, 0); //crosstalk 1/3 ºÚ¿ò
			break;
		case (9): 
			FPGA_DisPattern(7, 127, 0, 0); //crosstalk 1/3 °×¿ò
			break;
		case (10): 
			FPGA_DisPattern(133, 255, 0, 0); //²Ê¿òCrosstalk£¨red +yellow)
			break; 
		case (11): 
			FPGA_DisPattern(126, 0, 255, 0); //ºÚ°×Ìõ
			break; 
		case (12): 
			FPGA_DisPattern(126, 255, 0, 255); //ºÚ°×Ìõ
			break;
		case (13):  
			FPGA_DisPattern(0, 255, 0, 0); //R
			break; 
		case (14): 
			FPGA_DisPattern(0, 0, 255, 0); //G 
			break; 
		case (15):
			FPGA_DisPattern(0, 0, 0, 255); //B  
			break; 
		case (16): 
			FPGA_DisPattern(92, 0, 0, 0);	//WRGB transtion
			break;
		case (17): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(24, 127, 127, 127); //dot check 
			break;
		case (18): 
			LEDA_NORM();
			if (Pic_Load == ERROR)
			{
				FPGA_Info_Visible(INFO_STR);
				FPGA_Info_Set((uint8_t *)"LOAD PIC FAIL.");
			}
			FPGA_DisPicture(0);	//picture
			break;
		case (19): 
			FPGA_Info_Visible(INFO_NONE); 
			LEDA_DIM();
			FPGA_DisPattern(0, 255, 255, 255);	//DIM (low current white)
			break; 
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;  
	}
	Delay_ms(ET_DLY_LOCK * 300);
}

/*********************************************************************************
* Function: Test_ESD
* Description: ESD test
* Input: none
* Output: none
* Return: none
* Call: external
*/

void Test_ESD(void)
{
	ESD_Visual_Check();
	ESD_Fail_Recover();
}
