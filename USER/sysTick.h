#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f2xx.h"

/*********************************************************************************
 * Function statement for external call
 */
void SysTick_Init(void);
void Delay_us(__IO u32 nUs);
void Delay_ms(uint16_t nMs);
void Delay_sec(uint16_t nSec);
void Timer_Decrement(void);

#endif /* __SYSTICK_H */
