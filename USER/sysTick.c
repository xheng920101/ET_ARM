#include "sysTick.h"
#include "usart.h"

static __IO u32 DelayUsCnt;

/*********************************************************************************
* Function: SysTick_Init
* Description: system tickit configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms interrupt
	 * SystemFrequency / 100000	 10us interrupt
	 * SystemFrequency / 1000000 1us interrupt
	 */
	while (SysTick_Config(SystemCoreClock / 1000000))	// 120MHz
	{
		printf("\r\n Reload value impossible! \r\n");
	}
	printf("\r\nSystemCoreClock = %dHz\r\n", SystemCoreClock);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; 	// disable counter
}

/*********************************************************************************
* Function: Delay_us
* Description: delay, unit is us
* Input: -nUs, delay time
* Output: none
* Return: none
* Call: external
*/
void Delay_us(__IO u32 nUs)
{ 
	DelayUsCnt = nUs;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	 // enable counter
	while (DelayUsCnt != 0);	 //wait
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; 	// disable counter
}

/*********************************************************************************
* Function: Delay_ms
* Description: delay, unit is ms
* Input: -nMs, delay time
* Output: none
* Return: none
* Call: external
*/
void Delay_ms(uint16_t nMs)
{ 
	uint16_t DelayMsCnt;
	DelayMsCnt = nMs;
	while (DelayMsCnt != 0)
	{
		Delay_us(1000);
		DelayMsCnt--;
	}
}

/*********************************************************************************
* Function: Delay_sec
* Description: delay, unit is s
* Input: -nSec, delay time
* Output: none
* Return: none
* Call: external
*/
void Delay_sec(uint16_t nSec)
{ 
	uint16_t DelaySecCnt;
	DelaySecCnt = nSec;
	while (DelaySecCnt != 0)
	{
		Delay_ms(1000);
		DelaySecCnt--;
	}
}

/*********************************************************************************
* Function: Timer_Decrement
* Description: counter
* Input: none
* Output: none
* Return: none
* Call: call by SysTick_Handler()
*/
void Timer_Decrement(void)
{
  if (DelayUsCnt != 0x00)
  { 
    DelayUsCnt--;
  }
}
