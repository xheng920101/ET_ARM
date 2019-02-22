#ifndef __LCD_TEST_H
#define	__LCD_TEST_H

#include "stm32f2xx.h"

extern FlagStatus Flag_Test_Current;
extern FlagStatus current_NG;
extern FlagStatus SDCard_NG;
extern FlagStatus TE_NG;
extern FlagStatus PWM_NG;
extern FlagStatus ID_NG;
extern FlagStatus FW_NG;

extern float SPEC_MAX_IOVCC;							//mA
extern float SPEC_MAX_VSP;								//mA
extern float SPEC_MAX_VSN;								//mA

extern float SPEC_LEDA_MIN;							//mA
extern float SPEC_LEDA_MAX;							//mA

extern float	I_IOVCC;
extern float  I_VSP;
extern float	I_VSN;
extern float	I_LEDA;

/*********************************************************************************
 * Function statement for external call
 */
void TEST_Config_ON(void);
void TEST_Config_CTP(void);
void TEST_Config_OFF(void);
void Current_Check_Config(void);
void CTP_Start(void);
void Differ2_Detect(void);
void TE_Detect(void);
void ARM_PWM_Detect(void);
void PWM_Detect(void);
void Connect_Check(void);
void Res_Check(void);
void AOI_Current_Check_Normal(void);
void AOI_Current_Check_Sleep(void);
void AOI_Current_Check(void);
void Current_Check(void);
void SleepCurrent_Check(void);
void Meas_Current_Normal(void);
void Meas_Current_Sleep(void);
void Current_Upload(void);
void ESD_Fail_Recover(void);
void ON3sec_OFF3sec(void);
void Test_Mode_Switch(void);

#endif /* __LCD_TEST_H */
