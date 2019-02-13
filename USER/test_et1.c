#include "include.h"

/*********************************************************************************
* Function: Test_ET1
* Description: ET1 test
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void Test_ET1(void)
 {
 	TOTAL_DIS_NUM = 30;	
 	switch (DIS_NUM) 
	{
		case (0):
			FPGA_DisPicture(0);
			break;
		case (1):
			FPGA_DisPicture(1);
		break;
		case (2):
			FPGA_DisPicture(2);
		break;
		case (3):
			FPGA_DisPicture(3);
			break;
		case (4):
			FPGA_DisPicture(4);
			break;
		case (5):
			FPGA_DisPicture(5);
			break;
		case (6):
			FPGA_DisPicture(6);
			break;
		case (7):
			FPGA_DisPicture(7);
			break;
		case (8):
			FPGA_DisPicture(8);
			break;
		case (9):
			FPGA_DisPicture(9);
			break;
		case (10):
			FPGA_DisPicture(10);
			break;
		case (11):
			FPGA_DisPicture(11);
			break;
		case (12):
			FPGA_DisPicture(12);
			break;
		case (13):
			FPGA_DisPicture(13);
			break;
		case (14):
			FPGA_DisPicture(14);
			break;
		case (15):
			FPGA_DisPicture(15);
			break;
		case (16):
			FPGA_DisPicture(16);
			break;
		case (17):
			FPGA_DisPicture(17);
			break;
		case (18):
			FPGA_DisPicture(18);
			break;
		case (19):
			FPGA_DisPicture(19);
			break;
		case (20):
			FPGA_DisPicture(20);
			break;
		case (21):
			FPGA_DisPicture(21);
			break;
		case (22):
			FPGA_DisPicture(22);
			break;
		case (23):
			FPGA_DisPicture(23);
			break;
		case (24):
			FPGA_DisPicture(24);
			break;
		case (25):
			FPGA_DisPicture(25);
			break;
		case (26):
			FPGA_DisPicture(26);
			break;
		case (27):
			FPGA_DisPicture(27);
			break;
//		case (28):
//			FPGA_DisPicture(28);
//			break;
//		case (29):
//			FPGA_DisPicture(29);
//			break;
		default: 
			FPGA_DisPicture(0);
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
