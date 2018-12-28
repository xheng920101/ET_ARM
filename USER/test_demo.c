#include "include.h" 

/*********************************************************************************
* Function: Test_DEMO
* Description: DEMO test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_DEMO(void)
 {
	TOTAL_DIS_NUM = 14;
 	switch (DIS_NUM) 
	{
		case (0): 		
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
		  FPGA_DisPattern(0, 255, 0, 0);
			break;
		case (1): 
			FPGA_Info_Visible(INFO_NONE);
		  FPGA_DisPattern(0, 0, 255, 0);			
			break;
		case (2): 
			FPGA_DisPattern(0, 0, 0, 255);
			break;
		case (3): 
			FPGA_DisPattern(0, 0, 0, 0);
			break;
		case (4): 
			FPGA_DisPattern(0, 255, 255, 255);
			break;
		case (5): 
			FPGA_DisPattern(13, 0, 0, 0);
			break;
		case (6): 
			FPGA_DisPattern(16, 0, 0, 0);
			break;
		case (7): 
			FPGA_DisPattern(1, 128, 128, 128);
			break;
		case (8): 
			FPGA_DisPicture(0);
			break;
		case (9): 
			FPGA_DisPicture(1);
			break;
		case (10): 
			FPGA_DisPicture(2);
			break;
		case (11): 
			FPGA_DisPicture(3);
			break;
		case (12): 
			FPGA_DisPicture(4);
			break;
		case (13): 			
			FPGA_Info_Visible(INFO_NONE);
		  FPGA_DisPicture(5);
			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
 }
