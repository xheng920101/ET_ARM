#include "include.h" 

void Visual_check(void) 
{ 
	TOTAL_DIS_NUM = 44; 
	switch (DIS_NUM) 
	{  
		case (0): //R 
		FPGA_DisPattern(0, 255, 0, 0); 
		FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO); 
		break; 
		case (1): //G 
		FPGA_DisPattern(0, 0, 255, 0); 
		FPGA_Info_Visible(INFO_NONE); 
		break; 
		case (2): //B 
		FPGA_DisPattern(0, 0, 0, 255); 
		break; 
		case (3): //D 
		FPGA_DisPattern(0, 0, 0, 0); 
		break; 
		case (4): //W 
		FPGA_DisPattern(0, 255, 255, 255); 
		break; 
		case (5): //L127 
		FPGA_DisPattern(0, 127, 127, 127); 
		break; 
		case (6): //Black Window(G127) 
		FPGA_DisPattern(1, 127, 0, 0); 
		break; 
		case (7): //White Window(G127) 
		FPGA_DisPattern(7, 127, 255, 0); 
		break; 
		case (8): //Chess Board (8*8) 
		FPGA_DisPattern(51, 0, 0, 0); //51  93
		break; 
		case (9): //Gary and RGB Color Bar 
		FPGA_DisPattern(90, 0, 0, 0); 
		break; 
		case (10): //8V Color Bar 
		FPGA_DisPattern(49, 0, 0, 0); 
		break; 
		case (11): //8H Color Bar 
		FPGA_DisPattern(50, 0, 0, 0); 
		break; 
		case (12): //Gray Bar1: H 0-255
		FPGA_DisPattern(18, 0, 0, 0); 
		break; 
		case (13): //Gray Bar2: V 0-255 
		FPGA_DisPattern(17, 0, 0, 0); 
		break; 
		case (14): //Gray Bar3: H 255-0
		FPGA_DisPattern(36, 0, 0, 0); 
		break; 
		case (15): //Gray Bar4: V 255-0 
		FPGA_DisPattern(35, 0, 0, 0); 
		break;  
		case (16): //Gray Bar5: H16
		FPGA_DisPattern(34, 0, 0, 0); 
		break; 
		case (17): //Gray Bar6: V16 
		FPGA_DisPattern(33, 0, 0, 0); 
		break; 
		case (18): //´®¸ñ 
		FPGA_DisPattern(30, 127, 0, 0); 
		break; 
		case (19): //1*1 Bar(0-255-0) 
		FPGA_DisPattern(32, 255, 0, 0); 
		break; 
		case (20): //Oblique Gray Bar 
		FPGA_DisPattern(73, 0, 0, 0); 
		break; 
		case (21): //LR Color and Gray Bar
		FPGA_DisPattern(92, 0, 0, 0); 
		break; 
		case (22): //Flicker-Pixel based(127) 
		FPGA_DisPattern(21, 127, 0, 0); 
		break; 
		case (23): //Flicker-Dot based(127) 
		FPGA_DisPattern(22, 127, 0, 0); 
		break; 
		case (24): //Pixel check (255) 
		FPGA_DisPattern(23, 255, 0, 0); 
		break; 
		case (26): //Pixel check (127) 
		FPGA_DisPattern(23, 127, 0, 0); 
		break; 
		case (25): //Dot Check (255) 
		FPGA_DisPattern(24, 255, 0, 0); 
		break; 
		case (27): //Dot Check (127) 
		FPGA_DisPattern(24, 127, 0, 0); 
		break; 
		case (28): //1 dot check_R 
		FPGA_DisPattern(57, 255, 0, 0); 
		break; 
		case (29): //1 dot check_G 
		FPGA_DisPattern(61, 255, 0, 0); 
		break; 
		case (30): //1 dot check_B 
		FPGA_DisPattern(65, 255, 0, 0); 
		break; 
		case (31): //Waku pattern 
		FPGA_DisPattern(79, 0, 0x77, 0x41); 
		break; 
		case (32): //G64 
		FPGA_DisPattern(0, 64, 64, 64); 
		break; 
		case (33): //G32 
		FPGA_DisPattern(0, 32, 32, 32); 
		break; 
		case (35): //Cross talk HW 
		FPGA_DisPattern(133, 255, 0, 0); 
		break;
		case (34): //hw color bar gradients 
		FPGA_DisPattern(138, 0, 0, 0); 
		break; 
		case (36): //hw crosstalk red
		FPGA_DisPattern(128, 255, 0, 0); 
		break; 		
		case (37): //hw crosstalk green
		FPGA_DisPattern(128, 0, 255, 0); 
		break; 
		case (38): //hw crosstalk blue
		FPGA_DisPattern(128, 0, 0, 255); 
		break;
		case (39): //hw crosstalk yellow
		FPGA_DisPattern(128, 255, 255, 0); 
		break;
		case (40): //hw crosstalk purple
		FPGA_DisPattern(128, 255, 0, 255); 
		break;
		case (41): //hw crosstalk cyan
		FPGA_DisPattern(128, 0, 255, 255); 
		break;
		case (42): //Pic 
		FPGA_DisPicture(0); 
		FPGA_Info_Visible(INFO_NONE);
		break; 
		case (43): 
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

void Power_check(void) 
{ 
	TOTAL_DIS_NUM = 13; 
	switch (DIS_NUM) 
	{ 
		case (0): //R
		FPGA_DisPattern(0, 255, 0, 0); 
		FPGA_Info_Visible(INFO_NONE); 
		break; 
		case (1): //G 
		FPGA_DisPattern(0, 0, 255, 0); 
		break; 
		case (2): //B 
		FPGA_DisPattern(0, 0, 0, 255); 
		break; 
		case (3): //D 
		FPGA_DisPattern(0, 0, 0, 0); 
		break; 
		case (4): //W 
		FPGA_DisPattern(0, 255, 255, 255); 
		break;
		case (5): //chessboard 8x8
		FPGA_DisPattern(20, 0, 0, 0); 
		break;
		case (6): //v-colorbar
		FPGA_DisPattern(13, 0, 0, 0); 
		break;
		case (7): //h-colorbar
		FPGA_DisPattern(14, 0, 0, 0); 
		break;
		case (8): //flicker
		FPGA_DisPattern(22, 255, 0, 0); 
		break;
		case (9): //1x1 bar
		FPGA_DisPattern(32, 255, 0, 0); 
		break;
		case (10): //pixel check
		FPGA_DisPattern(23, 255, 0, 0); 
		break;
		case (11): //dot check
		FPGA_DisPattern(24, 255, 0, 0); 
		break;
		case (12): //picture
		FPGA_DisPicture(0);
		break;
		default: 
 			FPGA_DisPattern(0, 0, 0, 0); 
 		break;
	}
}
		
void Pattern_check(void)
{
// 	TOTAL_DIS_NUM = 140; 
//  FPGA_DisPattern(DIS_NUM, 164, 0x77, 0x41);
//	return;
	
	TOTAL_DIS_NUM = PIC_NUM; 
	FPGA_DisPicture(DIS_NUM);
	return;

 	TOTAL_DIS_NUM = 6;
  switch (DIS_NUM) 
 	{
		case (0): //6*8 chessboard
			FPGA_DisPattern(78, 0, 0, 0);	
			FPGA_Info_Visible(INFO_NONE); 
		break;
 		case (1): //8*8 chessboard
 		  FPGA_DisPattern(51, 0, 0, 0);		
 			break;
 		case (2): //20*20 chessboard 
 			FPGA_DisPattern(112, 0, 53, 116);	
 			break;
 		case (3): //32 gray 	 
 			FPGA_DisPattern(0, 32, 32, 32);
 			break;
 		case (4): //60 gray 	
 			FPGA_DisPattern(0, 60, 60, 60);	
 			break;
 		case (5): //127 gray 	 
 			FPGA_DisPattern(0, 127, 127, 127);	
 			break;
 		case (6): 	
 		  FPGA_DisPicture(1);
			break;
 		case (7): 
 			FPGA_DisPicture(2);
 			break;
 		case (8): 	
 		  FPGA_DisPicture(3);
			break;
 		case (9): 
 			FPGA_DisPicture(4);
 			break;
 		case (10): 	
 		  FPGA_DisPicture(5);
			break;
 		case (11): 	
 			FPGA_DisPattern(22, 127, 0, 0);	
 			break;
 		default: 
 			FPGA_DisPattern(0, 0, 0, 0); 
 			break;
 	}
}

void ACD_test(void)
{
 	TOTAL_DIS_NUM = 7;
  switch (DIS_NUM) 
 	{
		case (0): //Cross talk HW 
			FPGA_DisPattern(128, 255, 0, 0); 
			FPGA_Info_Visible(INFO_NONE); 
		break;
 		case (1): //4*4 chessboard
 		  FPGA_DisPattern(71, 0, 0, 0);			
 			break;
 		case (2): //8*8 chessboard
 			FPGA_DisPattern(51, 0, 0, 0);
 			break;
 		case (3): //12*20 chessboard 
 			FPGA_DisPattern(93, 0, 0, 0);
 			break;
 		case (4): //20*20 chessboard
 			FPGA_DisPattern(112, 0, 53, 116);	
 			break;
 		case (5): //128 gray
 			FPGA_DisPattern(0, 128, 128, 128);
 			break;
 		case (6): //60 gray 	
 		  FPGA_DisPattern(0, 60, 60, 60);
			break;
 		default: 
 			FPGA_DisPattern(0, 0, 0, 0); 
 			break;
 	}
}

void pattern24_test(void)
{
 	TOTAL_DIS_NUM = 30;
  switch (DIS_NUM) 
 	{
		case (0): //dark skin
			FPGA_DisPattern(0, 115, 82, 68); 
			FPGA_Info_Visible(INFO_NONE); 
		break;
 		case (1): //light skin
 		  FPGA_DisPattern(0, 194, 150, 130); 		
 			break;
 		case (2): //blue sky
 		  FPGA_DisPattern(0, 98, 122, 157); 	
 			break;
 		case (3): //foliage
 		  FPGA_DisPattern(0, 87, 108, 67); 	
 			break;
 		case (4): //blue flower
 		  FPGA_DisPattern(0, 133, 128, 177); 	
 			break;
 		case (5): //bluish green
 		  FPGA_DisPattern(0, 103, 189, 170); 	
 			break;
 		case (6): //oragne
 		  FPGA_DisPattern(0, 214, 126, 44); 	
			break;
		case (7): //purplish blue
 		  FPGA_DisPattern(0, 80, 91, 166); 	
			break;
		case (8): //moderate red
 		  FPGA_DisPattern(0, 193, 90, 99); 	
			break;
		case (9): //purple
 		  FPGA_DisPattern(0, 94, 60, 108); 	
			break;
		case (10): //yellow green
 		  FPGA_DisPattern(0, 157, 188, 64); 	
			break;
		case (11): //orange yellow
 		  FPGA_DisPattern(0, 224, 163, 46); 	
			break;
		case (12): //blue
 		  FPGA_DisPattern(0, 56, 61, 150); 	
			break;
		case (13): //green
 		  FPGA_DisPattern(0, 70, 148, 73); 	
			break;
		case (14): //red
 		  FPGA_DisPattern(0, 175, 54, 60); 	
			break;
		case (15): //yellow
 		  FPGA_DisPattern(0, 231, 199, 31); 	
			break;
		case (16): //magenta
 		  FPGA_DisPattern(0, 187, 86, 149); 	
			break;
		case (17): //cyan
 		  FPGA_DisPattern(0, 8, 133, 161); 	
			break;
		case (18): //white
 		  FPGA_DisPattern(0, 243, 243, 242); 	
			break;
		case (19): //neutral 8
 		  FPGA_DisPattern(0, 200, 200, 200); 	
			break;
		case (20): //neutral 6.5
 		  FPGA_DisPattern(0, 160, 160, 160); 	
			break;
		case (21): //neutral 5
 		  FPGA_DisPattern(0, 122, 122, 122); 	
			break;
		case (22): //neutral 3.5
 		  FPGA_DisPattern(0, 85, 85, 85); 	
			break;
		case (23): //black
 		  FPGA_DisPattern(0, 52, 52, 52); 	
			break;
 		case (24): 
 			FPGA_DisPicture(0);
 			break;
 		case (25): 	
 		  FPGA_DisPicture(1);
			break;
 		case (26): 
 			FPGA_DisPicture(2);
 			break;
 		case (27): 	
 		  FPGA_DisPicture(3);
			break;
 		case (28): 
 			FPGA_DisPicture(4);
 			break;
 		case (29): 	
 		  FPGA_DisPicture(5);
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
 void Test_DEBUG(void)
 {	
//	 Visual_check();
//	 Power_check();
//	 ACD_test();
	 Pattern_check();
//	 pattern24_test();
 }















