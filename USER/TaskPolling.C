/**
  ******************************************************************************
  * @file    ..\V1.9\STM_DEMO_V1P9\USER\TaskPolling.C
  * @author  wenqiang_yang@tianma.cn
  * @version V1P9
  * @date    25-January-2017
  * @brief   Task polling function
  ******************************************************************************
  * @attention
  *
	*
  ******************************************************************************
  */  

#include "include.h"

void task_list(void)
{
	static uint8_t c0, c1, c2, c0_flag, c1_flag, c2_flag;

//======================================	
	if (Flag_1ms_Timeup == SET)								
	{
		Flag_1ms_Timeup = RESET;
		c0_flag = 1;		  //10ms处理一次的任务
		c0++;
		if (c0 >= 10)			  
		{
			c0 = 0;
			c1_flag = 1;	 //100ms处理一次的任务
			c1++;			
			if (c1 >= 10)		 
			{
				c1 = 0;
				c2_flag = 1; //300ms处理一次的任务
				c2++;
				if (c2 >= 3)
					c2 = 0;
			}
		}
	}
	
//======================================
	if (c0_flag)				 //10mS Polling
	{
		c0_flag = 0;
		switch (c0)
		{
			case 0:keyStateTemp = KEY_GetState();
						 break; 
			case 1:KeyProc();
						 break;
			case 2:CTPStateTemp = CTP_GetState();
						 break;
			case 3:CTPProc();
						 break;
			case 4:break;
			case 5:break;
			case 6:break;
			case 7:break;
			case 8:break;
			case 9:break;	 
			default:break;
		}
	}

	if (c1_flag)				 //100mS Polling
	{
		c1_flag = 0;
		switch (c1)
		{
			case 0:
							AutoSwitchPattern();				
						 break;
			case 1:break;
			case 2:break;
			case 3:break;
			case 4:break;
			case 5:break;
			case 6:break;
			case 7:break;
			case 8:break;
			case 9:break;
			default:break;
		}
	}
	
	if (c2_flag)				 //300mS Polling
	{
		c2_flag = 0;		
		switch (c2)
		{
			case 0:break;  
			case 1:break;	
			case 2:break;											  
			default:break;
		}
	}
}
