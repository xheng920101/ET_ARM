#ifndef __USART_H
#define	__USART_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f2xx.h"

extern USART_TypeDef* PC_COM;
extern USART_TypeDef* MSE_COM;

extern FlagStatus USART_ControlStatus;
extern char admesy_RData[];
extern uint32_t admesy_RDataCnt;
extern FlagStatus admesy_RDataFlag;

extern FlagStatus auto_line;
extern FlagStatus GAMMAEXPERT;
extern uint8_t Pnum;
extern float debug;

/*********************************************************************************
 * Function statement for external call
 */
void USART1_Config(void);
void UART4_Config(void);
void USART1_RXD(void);
void UART4_RXD(void);
void USART_EventProcess(void);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data, ...);

#endif /* __USART_H */
