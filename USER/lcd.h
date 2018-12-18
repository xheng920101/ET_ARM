#ifndef __LCD_H
#define __LCD_H

#include "include.h"

/*********************************************************************************
 * Function statement for external call
 */
void LCD_RST_Config(void);
void DriverIC_Reset(void);
void LCD_SleepIn(void);
void LCD_SleepOut(void);
void LCD_DisplayOff(void);
void LCD_DisplayOn(void);
void LCD_SoftReset(void);
void LCD_LPMode(void);
void LCD_HSMode(void);
void LCD_VideoMode_ON(void);
void LCD_VideoMode_OFF(void);
void LCD_PWM(uint16_t data);
void LEDA_DIM(void);
void LEDA_NORM(void);
void MIPI_SleepMode_ON(void);
void MIPI_SleepMode_OFF(void);
void ARM_PWM_Control(void);
void LCM_Init(void);
void LCM_Reset(void);
void LCD_LitSquence(void);

#endif /* __LCD_H */
