#include "include.h"

void ESD_Visual_Check(void) 
{ 
		TOTAL_DIS_NUM = 13; 
		switch (DIS_NUM) 
		{ 		
		case (0): //waku
		FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
		FPGA_DisPattern(79, 0, 0x77, 0x41); 
		break; 	
		case (1): //dark
		FPGA_Info_Visible(INFO_NONE); 
		FPGA_DisPattern(0, 0,0,0); 
		break; 
		case (2): //W 
		FPGA_DisPattern(0, 255,255, 255); 
		break; 
		case (3): //R 
		FPGA_DisPattern(0, 255, 0, 0); 
		break; 
		case (4): //G 
		FPGA_DisPattern(0, 0,255, 0); 
		break; 
		case (5): //B 
		FPGA_DisPattern(0, 0,0,255); 
		break; 
		case (6): //L64 
		FPGA_DisPattern(0, 64,64,64); 
		break; 
		case (7): //gray127 
		FPGA_DisPattern(0, 127,127,127); 
		break; 
		case (8): //crosstalk
		FPGA_DisPattern(7, 127,127,127); 
		break; 
		case (9): //crosstalk
		FPGA_DisPattern(1, 127,127,127); 
		break; 
		case (10): //Flicker
		FPGA_DisPattern(22, 127, 0, 0);	
		break; 
		case (11): //WRGB transtion
		FPGA_DisPattern(90, 0,0,0); 
		break; 
		case (12): //dot check 
		FPGA_DisPattern(24, 127,0,0); 
		FPGA_Info_Visible(INFO_NONE); 
		break; 
		default: 
		FPGA_DisPattern(0, 0, 0, 0); 
		break;  
		}

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
