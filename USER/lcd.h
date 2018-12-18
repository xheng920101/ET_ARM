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

extern uint16_t				TCH_SPI_CSN1_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN1_GPIO_PORT;

extern uint16_t				TCH_SPI_SCK_PIN;
extern GPIO_TypeDef *	TCH_SPI_SCK_GPIO_PORT;

extern uint16_t				TCH_SPI_MISO_PIN;
extern GPIO_TypeDef *	TCH_SPI_MISO_GPIO_PORT;

extern uint16_t				TCH_SPI_MOSI_PIN;
extern GPIO_TypeDef *	TCH_SPI_MOSI_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN2_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN2_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN3_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN3_GPIO_PORT;

extern uint16_t				TCH_RST_PIN;
extern GPIO_TypeDef *	TCH_RST_GPIO_PORT;

//YWB
extern uint16_t				TCH_SPI_CSN1_2_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN1_2_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN2_2_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN2_2_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN3_2_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN3_2_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN1_3_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN1_3_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN2_3_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN2_3_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN3_3_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN3_3_GPIO_PORT;

extern uint16_t				TCH_SPI_CSN_PIN;
extern GPIO_TypeDef *	TCH_SPI_CSN_GPIO_PORT;

#endif /* __LCD_H */
