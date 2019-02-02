#ifndef __LED_H
#define	__LED_H

#include "board_conf.h"

/*********************************************************************************
 * Function statement for external call
 */
void LED_Config(void);
void LED_ON(LED_TypeDef Led);
void LED_OFF(LED_TypeDef Led);
void LED_Toggle(LED_TypeDef Led);

#endif /* __LED_H */
