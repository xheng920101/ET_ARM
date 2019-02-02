#ifndef __TIMER_H
#define	__TIMER_H

#include "board_conf.h"

extern uint16_t PWM_DUTY;
extern uint16_t PWM_DDIC_LOW;
extern uint16_t PWM_DDIC_HIGH;
extern uint16_t delay_cnt;
extern uint16_t second;

/*********************************************************************************
 * 外部调用函数声明
 */
void TIM2_Config(void);
void TIM2_EventProcess(void);


void TIM3_Config(void);
void TIM3_EventProcess(void);

void TIM4_Config(void);
void TIM4_EventProcess(void);
#endif /* __TIMER_H */
