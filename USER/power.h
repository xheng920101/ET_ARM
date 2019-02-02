#ifndef __POWER_H
#define	__POWER_H

#include "board_conf.h"

void __INLINE POWER_VCI_Set(void)
{
	GPIO_SetBits(POWER_VCI_GPIO_PORT, POWER_VCI_PIN);
}

void __INLINE POWER_VCI_Reset(void)
{
	GPIO_ResetBits(POWER_VCI_GPIO_PORT, POWER_VCI_PIN);
}

void __INLINE POWER_IOVCC_Set(void)
{
	GPIO_SetBits(POWER_IOVCC_GPIO_PORT, POWER_IOVCC_PIN);
}

void __INLINE POWER_IOVCC_Reset(void)
{
	GPIO_ResetBits(POWER_IOVCC_GPIO_PORT, POWER_IOVCC_PIN);
}

void __INLINE POWER_OTP_Set(void)
{
	GPIO_SetBits(POWER_OTP_GPIO_PORT, POWER_OTP_PIN);
}

void __INLINE POWER_OTP_Reset(void)
{
	GPIO_ResetBits(POWER_OTP_GPIO_PORT, POWER_OTP_PIN);
}

void __INLINE POWER_VSN_Set(void)
{
	GPIO_SetBits(POWER_VSN_GPIO_PORT, POWER_VSN_PIN);
}

void __INLINE POWER_VSN_Reset(void)
{
	GPIO_ResetBits(POWER_VSN_GPIO_PORT, POWER_VSN_PIN);
}

void __INLINE POWER_VSP_Set(void)
{
	GPIO_SetBits(POWER_VSP_GPIO_PORT, POWER_VSP_PIN);
}

void __INLINE POWER_VSP_Reset(void)
{
	GPIO_ResetBits(POWER_VSP_GPIO_PORT, POWER_VSP_PIN);
}

void __INLINE POWER_LED_Set(void)
{
}

void __INLINE POWER_LED_Reset(void)
{
}

/*********************************************************************************
 * Function statement for external call
 */
void POWER_I2C_Config(void);
void POWER_I2C_SETTING(float vsp_val, float vsn_val);
void POWER_EN_Config(void);
void LCMPower_Init(void);
void LCMPower_ON(void);
void LCMPower_OFF(void);
void LCMPower_ON1(void);
void LCMPower_OFF1(void);

#endif /* __POWER_H */
