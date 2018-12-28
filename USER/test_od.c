#include "include.h"


void OD_Visual_Check()
{
		TOTAL_DIS_NUM = 33;	
 	switch (DIS_NUM) 
	{
		case (0): 		
			FPGA_Info_Visible(INFO_NONE);
		  FPGA_DisPattern(88, 255, 255, 255); //white cross
			break;
		case (1): 
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO); 
			FPGA_DisPattern(0, 255, 255, 255); //white
			break;	
		case (2): 
			FPGA_Info_Visible(INFO_NONE);
			FPGA_DisPattern(0, 0, 0, 0); //black
			break;
		case (3): 
			FPGA_DisPattern(0, 255, 0, 0); //R
			break;
		case (4): 
			FPGA_DisPattern(0, 0, 255, 0); //G
			break;
		case (5): 
			FPGA_DisPattern(0, 0, 0, 255); //B
			break;
		case (6): 
			FPGA_DisPattern(24, 128, 128, 128); //flicker
			break;
		case (7): 
			FPGA_DisPattern(1, 127, 0, 0); //crosstalk 1/3 ºÚ¿ò
			break;
		case (8): 
			FPGA_DisPattern(7, 127, 0, 0); //crosstalk 1/3 °×¿ò
			break;
		case (9): 
			FPGA_DisPattern(0, 127, 127, 127);//gray127
			break;
		case (10): 
			FPGA_DisPattern(90, 0, 0, 0); //Color bar +gray bar
			break;
		case (11): 
			FPGA_DisPattern(18, 0, 0, 0); //gray bar-H
			break;	
		case (12): 
			FPGA_DisPattern(17, 0, 0, 0); //Gray bar-V
			break;
		case (13): 
			FPGA_DisPattern(128, 255, 0, 0); //²Ê¿òCrosstalk£¨red /G127)
			break;
		case (14): 
			FPGA_DisPattern(128, 0, 255, 0); //²Ê¿òCrosstalk£¨Green /G127)
			break;
		case (15): 
			FPGA_DisPattern(128, 0, 0, 255); //²Ê¿òCrosstalk£¨blue /G127)
			break;
		case (16): 
			FPGA_DisPattern(128, 0, 255, 255); //²Ê¿òCrosstalk£¨Çà/G127)
			break;
		case (17): 
			FPGA_DisPattern(128, 255, 255, 0); //²Ê¿òCrosstalk£¨yellow /G127)
			break;
		case (18): 
			FPGA_DisPattern(128, 255, 0, 255); //²Ê¿òCrosstalk£¨purple /G127)
			break;
		case (19): 
			FPGA_DisPattern(133, 255, 0, 0);//²Ê¿òCrosstalk£¨red +yellow)
			break;
 		case (20):
			FPGA_Info_Visible(INFO_NONE);			
 		  FPGA_DisPattern(71, 0, 0, 0);	//4*4 chessboard		
 			break;
 		case (21): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 127, 127, 127);//gray127	
 			break;
 		case (22):
			FPGA_Info_Visible(INFO_NONE);			
 		  FPGA_DisPattern(78, 0, 0, 0);	//6*8 chessboard		
 			break;
 		case (23): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 32, 32, 32);//gray32	
 			break;
		case (24): 
 		  FPGA_DisPattern(0, 64, 64, 64);//gray64
 			break;
		case (25): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 127, 127, 127);//gray127	
 			break;
		case (26):
			FPGA_Info_Visible(INFO_NONE);			
 		  FPGA_DisPattern(51, 0, 0, 0);	//8*8 chessboard		
 			break;
 		case (27): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 127, 127, 127);//gray127	
 			break;
		case (28):
			FPGA_Info_Visible(INFO_NONE);			
 		  FPGA_DisPattern(112, 0, 89, 113);	//12*19 chessboard		
 			break;
 		case (29): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 127, 127, 127);//gray127	
 			break;
		case (30):
			FPGA_Info_Visible(INFO_NONE);			
 		  FPGA_DisPattern(112, 0, 53, 107);	//20*20 chessboard		
 			break;
 		case (31): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 60, 60, 60);//gray60
 			break;
		case (32): 
			FPGA_Info_Visible(INFO_RGBVAL);
 		  FPGA_DisPattern(0, 127, 127, 127);//gray127	
 			break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}

}
/*********************************************************************************
* Function: Test_OD
* Description: OD test
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Test_OD(void)
{
	OD_Visual_Check();
}
