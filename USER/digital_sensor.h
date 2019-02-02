#ifndef __DIGITAL_SENSOR_H
#define	__DIGITAL_SENSOR_H

#include "stm32f2xx.h"

#define CHN1	0x10	// Digital Sample for VIN1
#define CHN2	0x20	// Digital Sample for VIN2
#define CHN3	0x40	// Digital Sample for VIN3
#define CHN4	0x80	// Digital Sample for VIN4

/*********************************************************************************
 * Function statement for external call
 */
void DSENSOR_I2C_Config(void);
float DSENSOR_I2C_GetFlicker(uint8_t CHNX);
void DSENSOR_Test(uint8_t CHNX);

#endif /* __DIGITAL_SENSOR_H */
