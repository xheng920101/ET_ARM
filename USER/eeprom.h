#ifndef __EEPROM_H
#define	__EEPROM_H

#include "stm32f2xx.h"

/*********************************************************************************
 * Function statement for external call
 */
void EEPROM_I2C_Config(void);
FlagStatus EEPROM_I2C_Write(uint16_t Addr, uint8_t *Data, uint8_t Data_Len);
FlagStatus EEPROM_I2C_Read(uint16_t Addr, uint8_t *Data, uint8_t Data_Len);
void EEPROM_WR_Verify(void);
void Get_Test_Mode(void);
void Set_Test_Mode(void);
void Disp_Test_Mode(void);

#endif /* __EEPROM_H */
