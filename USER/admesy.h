#ifndef __ADMESY_H
#define __ADMESY_H

#include "stm32f2xx.h"
#include "admesy_colour.h"
#include "_kiss_fft_guts.h"

extern char Admesy_RData[];
extern uint16_t	Admesy_RDataCnt;
extern FlagStatus Admesy_RDataFlag;
extern ErrorStatus MSE_State;
extern float Flicker;
extern float color_x;
extern float color_y;
extern float Lv;

extern float color_u;
extern float	color_v;

extern int CCT;

/*********************************************************************************
 * Function statement for external call
 */
void Admesy_Init(void);
void Meas_Yxy(void);
void Meas_XYZ(void);
void Meas_CCT(void);
void Meas_Flicker(void);
void Meas_JEITA(void);
void Meas_Yuv(void);

#endif /* __ADMESY_H */
