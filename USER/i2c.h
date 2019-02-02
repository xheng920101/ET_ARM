#ifndef __I2C_H
#define	__I2C_H

#include "stm32f2xx.h"
#include "sysTick.h"

extern GPIO_TypeDef* I2C_PORT_SDA;
extern GPIO_TypeDef* I2C_PORT_SCL;
extern uint16_t I2C_PIN_SDA;
extern uint16_t I2C_PIN_SCL;
extern uint8_t I2C_ADDR;

void __INLINE I2C_Delay(void)
{
	Delay_us(5);
}
 
/*********************************************************************************
 * Function statement for external call
 */
FlagStatus I2C_Write_Byte(uint8_t Addr, uint8_t Data);
FlagStatus I2C_Write_Byte16(uint16_t Addr, uint8_t Data);
FlagStatus I2C_Read_Byte(uint8_t Addr, uint8_t Data);
FlagStatus I2C_Read_Byte16(uint16_t Addr, uint8_t Data);
FlagStatus I2C_Sequential_Write(uint8_t Addr, uint8_t* Data, uint8_t Data_Len);
FlagStatus I2C_Sequential_Write16(uint16_t Addr, uint8_t* Data, uint8_t Data_Len);
FlagStatus I2C_Sequential_Read(uint8_t Addr, uint8_t* Data, uint8_t Data_Len);
FlagStatus I2C_Sequential_Read16(uint16_t Addr, uint8_t* Data, uint8_t Data_Len);
FlagStatus I2C_Check_Addr(uint8_t Addr);

#endif /* __I2C_H */
