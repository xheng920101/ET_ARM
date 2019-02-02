#ifndef __KEY_H
#define	__KEY_H

#include "board_conf.h"

#define		KEY_UP 		0x0E	// KEY1	
#define		KEY_DOWN	0x0D	// KEY2
#define		KEY_SLEEP 	0x0B	// KEY3	
#define		KEY_TEST 	0x07	// KEY4	
#define		KEY_AUTO	0x0C    // KEY1 & KEY2
#define 	KEY_IDLE 	0x0F

extern uint8_t TOTAL_DIS_NUM;
extern uint8_t DIS_NUM;

/*********************************************************************************
 * Function statement for external call
 */
void KEY_Config(void);
uint8_t KEY_GetState(void);
void DisState_Switch(void);
void KEY_SLEEPIN(void);

#endif /* __KEY_H */
