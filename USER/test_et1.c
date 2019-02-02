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
 	TOTAL_DIS_NUM =6;	

 	switch (DIS_NUM) 
	{
		case (0): 
			FPGA_DisPicture(0);	//picture
//		  Delay_ms(ET_DLY_LOCK * 1500);
			break;	
//		case (1): 
//			FPGA_Info_Visible(INFO_NONE);
//			FPGA_DisPattern(24, 127, 127, 127);	
//		  Delay_ms(ET_DLY_LOCK * 1200);
//		  SPEC_NORMAL_IOVCC_MAX =	50;
//			SPEC_NORMAL_VSP_MAX =	50;
//			SPEC_NORMAL_VSN_MAX	=	50;		
//		  Current_Check();	
//			break;			
//		case (2): 
//			ScanForward();
//			FPGA_DisPattern(0, 255, 255, 255);	
//		  Delay_ms(ET_DLY_LOCK * 2500);
//			SPEC_NORMAL_IOVCC_MAX =	44.0;
//			SPEC_NORMAL_VSP_MAX =	21.0;
//			SPEC_NORMAL_VSN_MAX	=	11.0;		
//		  Current_Check();	
//		break;		

//		case (3): 
//			FPGA_DisPattern(0, 127, 127, 127);		
//		  ScanBackward();
//		  Delay_ms(ET_DLY_LOCK * 2500);
//			break;
		
		case (1):		
      ScanForward();			
		  FPGA_DisPicture(1);	//picture
		  LCD_PWM(0xFF);
		  Delay_ms(ET_DLY_LOCK * 3000);
			break;
	  case (2):		
      ScanBackward();	
		  FPGA_DisPicture(7);	//picture
		  Delay_ms(ET_DLY_LOCK * 4000);
			break;
		case (3):	
     ScanForward();				
		  FPGA_DisPicture(8);	//picture
		 Delay_ms(ET_DLY_LOCK * 1000);
			break;
		case (4):				
		  FPGA_DisPicture(9);	//picture
			SPEC_NORMAL_IOVCC_MAX =	75;
			SPEC_NORMAL_VSP_MAX =	42;
			SPEC_NORMAL_VSN_MAX	=	21;	
	printf("CURRENT SPEC -> normal: IOVCC_MAX = %.2f; VSP_MAX = %.2f; VSN_MAX = %.2f;\r\n", SPEC_NORMAL_IOVCC_MAX ,SPEC_NORMAL_VSP_MAX , SPEC_NORMAL_VSN_MAX);
	printf("CURRENT SPEC -> normal: IOVCC_MIN = %.2f; VSP_MIN = %.2f; VSN_MIN = %.2f; \r\n", SPEC_NORMAL_IOVCC_MIN, SPEC_NORMAL_VSP_MIN ,SPEC_NORMAL_VSN_MIN);		
		  Current_Check();	
			break;
		case (5): 
	    LCD_SleepIn();
	    LCD_VideoMode_OFF();
	    LCD_LPMode();
	    MIPI_SleepMode_ON();	
			GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
      LCMPower_OFF();	
      GPIO_ResetBits(TEST17_GPIO_PORT, TEST17_PIN); 		
		break;
		default: 
			FPGA_DisPattern(0, 0, 0, 0); 
			break;
	}
	Delay_ms(ET_DLY_LOCK * 300);
 }
