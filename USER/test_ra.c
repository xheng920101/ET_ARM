#include "include.h" 


void RA_Mode_AutoReset()
{
	if(TEST_MODE == TEST_MODE_RA)
	{
			if(second>=TAUTORESET)
			{
				printf("\r\n AutoReset time is up! do reset the LCM!\r\n");
					second =0;
					MIPI_Reset();
				IC_Init(SSDInitCode);	
				DriverIC_Reset();
				Delay_ms(20);
				DriverIC_Reset();
				IC_Init(RA_InitCode);
				LCD_SleepOut();
		#ifdef CMD_MODE
				LCD_VideoMode_OFF();
		#else
				LCD_VideoMode_ON();
		#endif
				LCD_HSMode();
				LCD_PWM(0xFF);			
			}
	}
}

/*********************************************************************************
* Function: Test_RA
* Description: RA test, bist mode
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Test_RA(void)
{

}
