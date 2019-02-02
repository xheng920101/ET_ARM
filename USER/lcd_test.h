#ifndef __LCD_TEST_H
#define	__LCD_TEST_H

#include "stm32f2xx.h"

extern FlagStatus Delay_Enable;
extern uint16_t Delay_Time;
extern FlagStatus Auto_Switch;
extern uint16_t Auto_Time;
extern float SPEC_NORMAL_IOVCC_MAX;							//mA
extern float SPEC_NORMAL_VSP_MAX;								//mA
extern float SPEC_NORMAL_VSN_MAX;								//mA

/*********************************************************************************
 * Function statement for external call
 */
void TEST_Config_ON(void);
void TEST_Config_CTP(void);
void TEST_Config_OFF(void);
void Current_Check_Config(void);
void Delay_Lock(void);
void Auto_Switch_Delay(void);
void Test_XXX(void);
void CTP_Start(void);
void Differ2_Detect(void);
void TE_Detect(void);
void ARM_PWM_Detect(void);
void PWM_Detect(void);
void Connect_Check(void);
void Res_Check(void);
void Current_Check(void);
void SleepCurrent_Check(void);
void Meas_Current_Normal(void);
void Meas_Current_Sleep(void);
void Current_Upload(void);
void ESD_Fail_Recover(void);
void ON3sec_OFF3sec(void);

#endif /* __LCD_TEST_H */
