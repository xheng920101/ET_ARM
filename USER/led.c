#include "led.h"

GPIO_TypeDef*  LED_PORT[3] = {
	LED1_GPIO_PORT,
	LED2_GPIO_PORT,
	LED3_GPIO_PORT
};
const uint16_t LED_PIN[3] = {
	LED1_PIN,
	LED2_PIN,
	LED3_PIN
};

/*********************************************************************************
* Function: LED_Config
* Description: LED I/O configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	 
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	 
	GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	 
}

/*********************************************************************************
* Function: LED_ON
* Description: lit on the led
* Input: -Led
* Output: none
* Return: none
* Call: external
*/
void LED_ON(LED_TypeDef Led)
{
	GPIO_ResetBits(LED_PORT[Led], LED_PIN[Led]);
}

/*********************************************************************************
* Function: LED_OFF
* Description: turn off the led
* Input: -Led
* Output: none
* Return: none
* Call: external
*/
void LED_OFF(LED_TypeDef Led)
{
	GPIO_SetBits(LED_PORT[Led], LED_PIN[Led]);
}

/*********************************************************************************
* Function: LED_Toggle
* Description: toggle the led
* Input: -Led
* Output: none
* Return: none
* Call: external
*/
void LED_Toggle(LED_TypeDef Led)
{
    GPIO_ToggleBits(LED_PORT[Led], LED_PIN[Led]);
}
