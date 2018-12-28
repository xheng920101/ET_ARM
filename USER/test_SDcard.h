#ifndef __TEST_SD_H
#define __TEST_SD_H

#include "stm32f2xx.h"

extern char 	VERSION_DISPLAY[];
extern char 	PRO_DISPLAY[];
extern char		Patten_Buf[];
extern unsigned  char LPatNu;
extern FlagStatus SD_MODE_ERROR;

/*********************************************************************************
 * Function statement for external call
 */
void TEST_MODE_Init(void);
void Display_Pattern(void);

#endif /* __TEST_SD_H */
