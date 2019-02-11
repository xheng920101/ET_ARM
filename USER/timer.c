#include "include.h"

FlagStatus Flag_1ms_Timeup = RESET;
unsigned long volatile Timer3_Ms_Counter = 0; //2017-2-3 19:45:47 ywq add
uint16_t Timer4_Sec_Counter;

/*********************************************************************************
* Function: TIM2_Config
* Description: timer2 configure, 10us interrupt once
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TIM2_Config(void)
{	
#ifdef PWM_DETECT
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 5; //APB1 = 60MHz, 60/(5+1) = 10M
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //div = 1
	TIM_TimeBaseInitStructure.TIM_Period = 9; //10M = 0.1us, 0.1*(9+1) = 1us
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //this parameter is valid only for TIM1 and TIM8.
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	//clear interrupt flag
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //enable overflow interrupt
	TIM_Cmd(TIM2, DISABLE);	//disable timer2
	
#else
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 59; //APB1 = 60MHz, 60/(59+1) = 1M
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //div = 1
	TIM_TimeBaseInitStructure.TIM_Period = 9; //1M = 1us, 1*(9+1) = 10us
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //this parameter is valid only for TIM1 and TIM8.
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	//clear interrupt flag
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //enable overflow interrupt
	TIM_Cmd(TIM2, ENABLE);	//enable timer2
	
#endif
}

/*********************************************************************************
* Function: TIM3_Config
* Description: timer3 configure, 1ms interrupt once
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
	TIM_DeInit(TIM3);

	TIM_TimeBaseInitStructure.TIM_Prescaler = 59; //APB1 = 60MHz, 60/(59+1) = 1M
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //div = 1
	TIM_TimeBaseInitStructure.TIM_Period = 999; //1M = 1us, 1*(999+1) = 1000us = 1ms
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //this parameter is valid only for TIM1 and TIM8.
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	//clear interrupt flag
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //enable overflow interrupt
	TIM_Cmd(TIM3, ENABLE);	//enable timer3
}

/*********************************************************************************
* Function: TIM4_Config
* Description: timer4 configure, 1s interrupt once
* Input: none
* Output: none
* Return: none
* Call: external
*/
void TIM4_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
	TIM_DeInit(TIM4);

	TIM_TimeBaseInitStructure.TIM_Prescaler = 59999; //APB1 = 60MHz, 60/(59999+1) = 1k
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //div = 1
	TIM_TimeBaseInitStructure.TIM_Period = 999; //1k = 1ms, 1*(999+1) = 1000ms = 1s
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //this parameter is valid only for TIM1 and TIM8.
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	//clear interrupt flag
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //enable overflow interrupt
	TIM_Cmd(TIM4, ENABLE);	//enable timer4
}

/*********************************************************************************
* Function: TIM2_EventProcess
* Description: interrupt handler
* Input: none
* Output: none
* Return: none
* Call: call by TIM2_IRQHandler
*/
void TIM2_EventProcess(void)
{
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET) 
  {
#ifdef PWM_DETECT
		ARM_PWM_Detect();
#endif	
		ARM_PWM_Control();

		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}

/*********************************************************************************
* Function: TIM3_EventProcess
* Description: interrupt handler
* Input: none
* Output: none
* Return: none
* Call: call by TIM3_IRQHandler
*/
void TIM3_EventProcess(void)
{
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == SET) 
  {
		Timer3_Ms_Counter++;
		Flag_1ms_Timeup = SET;	
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	}
}

/*********************************************************************************
* Function: TIM4_EventProcess
* Description: interrupt handler
* Input: none
* Output: none
* Return: none
* Call: call by TIM4_IRQHandler
*/
void TIM4_EventProcess(void)
{
	if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == SET) 
  {
		Timer4_Sec_Counter++;
		printf("\r\nTimer4_Sec_Counter = %d(second)\r\n", Timer4_Sec_Counter);	
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	}
}
