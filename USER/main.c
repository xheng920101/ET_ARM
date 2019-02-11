#include "include.h"

/* TEST_MODE can be a value of the list below */
//	TEST_MODE_ET1
//	TEST_MODE_ET2	  
//	TEST_MODE_ET3
//	TEST_MODE_OTP
//	TEST_MODE_OQC1
//	TEST_MODE_RA
//	TEST_MODE_ESD
//	TEST_MODE_OD
//	TEST_MODE_DEMO
//	TEST_MODE_CTP
//	TEST_MODE_DEBUG
uint8_t TEST_MODE = TEST_MODE_ET1;

#ifdef DSC_MODE
	uint8_t LCM_POWER_STATE;
#endif

/*********************************************************************************
* Function: main
* Description: main
* Input: none
* Output: none
* Return: none
* Call: system
*/
int main(void)
{
	/* STM32 initial */
	Board_Init(); 
	TEST_MODE_Init();
	Delay_ms(200);	// wait for FPGA ready!
	LED_ON(BLUE);

	/* Initial */		
	FPGA_Initial();
		
	if (!auto_line)
	{
		LCM_Init();
		
		/* CTP Sensor detect */
		if (TEST_MODE == TEST_MODE_CTP)
		{
			FPGA_Info_Visible(INFO_VERSION | INFO_PROJECT_NO);
			FPGA_DisPattern(83, 127, 127, 127);	//waku+black crosstalk
			CTP_Start();
			Differ2_Detect();	
		}
		
		/* picture loading for DPT AOI*/
		FPGA_SPI3Write(ADDR_PIC_WR_NUM);
		FPGA_SPI3Write(1);
		if (PIC_Load_BMP_DPT((uint8_t *)"DPT.bmp") == SUCCESS)
		{
			DIS_NUM_OLD = DIS_NUM; //DPT AOI not to enter test mode switch
		}			
		else //If DPT need more than one picture, picture should name as showed
		{
			FPGA_SPI3Write(ADDR_PIC_WR_NUM);
			FPGA_SPI3Write(2); 
			if (Lcd_Load_bmp(FPGA_porchPara.HACT, FPGA_porchPara.VACT, (uint8_t *)"DPT1.bmp") == SUCCESS)
			{
				DIS_NUM_OLD = DIS_NUM; //DPT AOI not to enter test mode switch
				Lcd_Load_bmp(FPGA_porchPara.HACT, FPGA_porchPara.VACT, (uint8_t *)"DPT2.bmp");
//					Lcd_Load_bmp(FPGA_porchPara.HACT, FPGA_porchPara.VACT, (uint8_t *)"DPT3.bmp");
			}			
		}			
		Pic_Load_Finish = RESET; //remain the flag for TM ET picture loading		
		
		if (current_NG == RESET && SDCard_NG == RESET && TE_NG == RESET && PWM_NG == RESET &&  ID_NG == RESET && FW_NG == RESET && FPGA_NG == RESET && OSC_TRIM_NG == RESET)
		{
			/* picture loading for TM manual line */ 
			if (PIC_NUM != 0)
			{
				debug = TIMESTAMP;	
				printf("\r\nPicture loading...\r\n");			
				PIC_Load_BMP(PIC_NUM);
				printf("\r\n===== Load %d picture time elapsed: %.3f(second)\r\n", PIC_NUM, TIMESTAMP - debug);
			}
#ifdef DSC_MODE
				LCM_POWER_STATE=SET;
				debug = TIMESTAMP;	
				printf("\r\nPicture loading...\r\n");			
				PIC_Load_BMP(PIC_NUM);
				printf("\r\n===== Load %d picture time elapsed: %.3f(second)\r\n", PIC_NUM, TIMESTAMP - debug);
#endif

			/* version setting */
			Version_Set();	
		}
		else if (current_NG == SET || FW_NG == SET)
		{
			//
		}
		else
		{
			FPGA_Info_Visible(INFO_STR);
			FPGA_DisPattern(114, 0, 0, 0);
			if (SDCard_NG == SET) FPGA_Info_Set((uint8_t *)"SDCARD ERROR");
			else if (TE_NG == SET) FPGA_Info_Set((uint8_t *)"TE NG");	
			else if (PWM_NG == SET) FPGA_Info_Set((uint8_t *)"PWM NG");	
			else if (ID_NG == SET)	FPGA_DisPattern(86, 0, 0, 0);
			else if (FPGA_NG == SET) FPGA_Info_Set((uint8_t *)"FPGA ERROR");
			else if (OSC_TRIM_NG == SET) FPGA_Info_Set((uint8_t *)"OSC TRIM OFF");				
		}
	} //end of 	if (!auto_line)
	else
	{
		PIC_Load_BMP(1); 
		DIS_NUM_OLD = DIS_NUM; //Auto AOI not to enter test mode switch
	}
	
#ifdef CURRENT_METER	
	SPEC_MAX_IOVCC =	SPEC_MAX_RED_IOVCC;
	SPEC_MAX_VSP =	SPEC_MAX_RED_VSP;
	SPEC_MAX_VSN	=	SPEC_MAX_RED_VSN;
	SPEC_LEDA_MIN = SPEC_MIN_LEDA_NORMAL;
	SPEC_LEDA_MAX	= SPEC_MAX_LEDA_NORMAL;
	printf("CURRENT SPEC -> normal: IOVCC_MAX = %.2f; VSP_MAX = %.2f; VSN_MAX = %.2f; LEDA_MAX = %.2f;\r\n", SPEC_MAX_IOVCC, SPEC_MAX_VSP, SPEC_MAX_VSN ,SPEC_LEDA_MAX);
	printf("CURRENT SPEC -> normal: IOVCC_MIN = %.2f; VSP_MIN = %.2f; VSN_MIN = %.2f; LEDA_MIN = %.2f; \r\n", SPEC_MIN_IOVCC, SPEC_MIN_VSP, SPEC_MIN_VSN, SPEC_LEDA_MIN);			
	printf("CURRENT SPEC -> dim: LEDA_MAX = %.2f;\r\n", SPEC_MAX_LEDA_DIM);
	printf("CURRENT SPEC -> dim: LEDA_MIN = %.2f;\r\n", SPEC_MIN_LEDA_DIM);
#endif
	
	/* Main loop */
	LED_OFF(BLUE);
	printf("\r\nMain loop...\r\n");
	printf("\r\n===== System on time elapsed: %.3f(second)\r\n", TIMESTAMP);	
  printf("\r\n*#*#SYSTEM OK#*#*\r\n");
//	
//	while (KEY_GetState() != KEY_UP);
//	Delay_ms(1000);		

	while (1)
	{	
		Test_Mode_Switch();
		Connect_Check();
		USART_EventProcess();
		task_list();
		
//		(DIS_NUM >= (TOTAL_DIS_NUM - 1)) ? (DIS_NUM = 0) : DIS_NUM++;
//		printf("TOTAL_DIS_NUM = %d, DIS_NUM = %d\r\n", TOTAL_DIS_NUM, DIS_NUM); 
//		Delay_ms(1000);		
	} 
}
