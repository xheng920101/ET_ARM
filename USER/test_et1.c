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
	uint8_t *info=(uint8_t *)"F";
	uint8_t buf_ptr=(uint8_t)'D';
	TOTAL_DIS_NUM = 5;
	switch (DIS_NUM) 
	{
		case (0):
			FPGA_Reg_Test(ADDR_TEST,info,&buf_ptr);
			printf("YOU ARE %c\r\n",buf_ptr);
			FPGA_Info_Visible(INFO_STR);
		
//			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			ScanForward();	
 			FPGA_DisPattern(0, 0, 0, 0);	//black
			Delay_ms(ET_DLY_LOCK * 2700); 
			break;
		case (1): 	
			FPGA_Info_Visible(INFO_RGBVAL);
			ScanBackward();
 			FPGA_DisPattern(0, 127, 127, 127);	//gray
			Delay_ms(ET_DLY_LOCK * 3700); 
			break;
		case (2): 
			FPGA_Info_Visible(INFO_NONE);
			ScanForward();
			FPGA_DisPattern(0, 255, 255, 255);	//white
		  Delay_ms(ET_DLY_LOCK * 700); 
			break;
		case (3): 
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
