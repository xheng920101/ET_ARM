#include "include.h" 



void Visual_check(void) 
{ 
		TOTAL_DIS_NUM = 36; 
		switch (DIS_NUM) 
		{ 
		case (0): //R 
		FPGA_DisPattern(0, 255, 0, 0); 
		FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO); 
		break; 
		case (1): //G 
		FPGA_DisPattern(0, 0,255, 0); 
		FPGA_Info_Visible(INFO_NONE); 
		break; 
		case (2): //B 
		FPGA_DisPattern(0, 0,0,255); 
		break; 
		case (3): //D 
		FPGA_DisPattern(0, 0,0,0); 
		break; 
		case (4): //W 
		FPGA_DisPattern(0, 255,255, 255); 
		break; 
		case (5): //L127 
		FPGA_DisPattern(0, 127,127,127); 
		break; 
		case (6): //Black Window(G127) 
		FPGA_DisPattern(1, 127,0,0); 
		break; 
		case (7): //White Window(G127) 
		FPGA_DisPattern(7, 127,255,0); 
		break; 
		case (8): //Chess Board (8*8) 
		FPGA_DisPattern(51, 0,0,0); //51  93
		break; 
		case (9): //Gary and RGB Color Bar 
		FPGA_DisPattern(90, 0,0,0); 
		break; 
		case (10): //8V Color Bar 
		FPGA_DisPattern(49, 0,0,0); 
		break; 
		case (11): //8H Color Bar 
		FPGA_DisPattern(50, 0,0,0); 
		break; 
		case (12): //Gray Bar1 
		FPGA_DisPattern(35, 0,0,0); 
		break; 
		case (13): //Gray Bar2 
		FPGA_DisPattern(36, 0,0,0); 
		break; 
		case (14): //Gray Bar3 
		FPGA_DisPattern(42, 0,0,0); 
		break; 
		case (15): //Gray Bar4 
		FPGA_DisPattern(43, 0,0,0); 
		break; 
		case (16): //Gray Bar5 
		FPGA_DisPattern(33, 0,0,0); 
		break; 
		case (17): //Gray Bar6 
		FPGA_DisPattern(34, 0,0,0); 
		break; 
		case (18): //´®¸ñ 
		FPGA_DisPattern(30,127,0,0); 
		break; 
		case (19): //1*1 Bar(0-255-0) 
		FPGA_DisPattern(32, 255,0,0); 
		break; 
		case (20): //Oblique Gray Bar 
		FPGA_DisPattern(73, 0,0,0); 
		break; 
		case (21): //LR Color and Gray Bar? 
		FPGA_DisPattern(92, 0,0,0); 
		break; 
		case (22): //Flicker-Pixel based(127) 
		FPGA_DisPattern(21, 127,0,0); 
		break; 
		case (23): //Flicker-Dot based(127) 
		FPGA_DisPattern(22, 127,0,0); 
		break; 
		case (24): //Pixel check (255) 
		FPGA_DisPattern(23, 255,0,0); 
		break; 
		case (25): //Pixel check (127) 
		FPGA_DisPattern(23, 127,0,0); 
		break; 
		case (26): //Dot Check (255) 
		FPGA_DisPattern(24, 255,0,0); 
		break; 
		case (27): //Dot Check (127) 
		FPGA_DisPattern(24, 127,0,0); 
		break; 
		case (28): //1 dot check_R 
		FPGA_DisPattern(57, 255,0,0); 
		break; 
		case (29): //1 dot check_G 
		FPGA_DisPattern(61, 255,0,0); 
		break; 
		case (30): //1 dot check_B 
		FPGA_DisPattern(65, 255,0,0); 
		break; 
		case (31): //Waku pattern 
		FPGA_DisPattern(79, 0, 0x77, 0x41); 
		break; 
		case (32): //G64 
		FPGA_DisPattern(0, 64,64,64); 
		break; 
		case (33): //G32 
		FPGA_DisPattern(0, 32,32,32); 
		break; 
		case (34): //Cross talk HW 
		FPGA_DisPattern(133, 255,0,0); 
		break; 
		case (35): //Pic 
		FPGA_DisPicture(0); 
		break; 
		case (36): 
		FPGA_DisPattern(0, 0, 0, 0); 
		FPGA_Info_Visible(INFO_NONE); 
// 		KEY_SLEEPIN(); 
// 		LCD_SleepIn(); 
		break; 
		default: 
		FPGA_DisPattern(0, 0, 0, 0); 
		break;  
		}

}

/*********************************************************************************
* Function: Test_DEBUG
* Description: DEBUG test
* Input: none
* Output: none
* Return: none
* Call: external
*/
//  void Test_DEBUG(void)
//  {
// // 	TOTAL_DIS_NUM = 85; 
// //	FPGA_DisPattern(DIS_NUM, 128, 128, 128);

// 	TOTAL_DIS_NUM = 7;
//  	switch (DIS_NUM) 
// 	{
// 		case (0): 
// 			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
// 		  FPGA_DisPattern(0, 255, 0, 0);
// 			break;
// 		case (1): 
// 			FPGA_Info_Visible(INFO_NONE);
// 		  FPGA_DisPattern(83, 128, 128, 128);			
// 			break;
// 		case (2): 
// 			FPGA_DisPattern(84, 0, 0, 0);
// 			break;
// 		case (3): 
// 			FPGA_DisPattern(90, 128, 128, 128);
// 			break;
// 		case (4): 
// 			FPGA_DisPicture(0);
// 			break;
// 		case (5): 
// 			FPGA_DisPicture(1);
// 			break;
// 		case (6): 	
// 			FPGA_Info_Visible(INFO_NONE);
// 		  FPGA_DisPicture(2);
// 			break;
// 		default: 
// 			FPGA_DisPattern(0, 0, 0, 0); 
// 			break;
// 	}
//  }



/*********************************************************************************
* Function: Test_DEBUG
* Description: DEBUG test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_DEBUG(void)
 {	
	 Visual_check();
 }















