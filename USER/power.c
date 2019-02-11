#include "include.h"

#define POWER_I2C_ADDR	0x7C    // TCS65312 bus address(7bit): 010 0001 *	//*=bit0 0:write,1:read

/*********************************************************************************
* Function: POWER_EN_Config
* Description: POWER enable pin configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void POWER_EN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_InitStructure.GPIO_Pin = POWER_VCI_PIN;	
	GPIO_Init(POWER_VCI_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = POWER_IOVCC_PIN;	
	GPIO_Init(POWER_IOVCC_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = POWER_OTP_PIN;	
	GPIO_Init(POWER_OTP_GPIO_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = POWER_LED_PIN;	
	GPIO_Init(POWER_LED_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(POWER_LED_GPIO_PORT, POWER_LED_PIN);

	GPIO_InitStructure.GPIO_Pin = POWER_VSN_PIN;	
	GPIO_Init(POWER_VSN_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = POWER_VSP_PIN;	
	GPIO_Init(POWER_VSP_GPIO_PORT, &GPIO_InitStructure);	
}

/*********************************************************************************
* Function: POWER_I2C_Config
* Description: POWER I2C interface configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void POWER_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = POWER_I2C_SCL_PIN;
	GPIO_Init(POWER_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(POWER_I2C_SCL_GPIO_PORT, POWER_I2C_SCL_PIN);
	
	GPIO_InitStructure.GPIO_Pin = POWER_I2C_SDA_PIN;
	GPIO_Init(POWER_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_SetBits(POWER_I2C_SDA_GPIO_PORT, POWER_I2C_SDA_PIN);
}

/*********************************************************************************
* Function: POWER_I2C_SETTING
* Description: set vsp/vsn voltage
* Input: -vsp_val, vsp setting value
* Input: -vsn_val, vsn setting value
* Output: none
* Return: none
* Call: external
*/
 void POWER_I2C_SETTING(float vsp_val, float vsn_val)
{
	uint8_t VSP_New;
	uint8_t VSN_New;
	uint8_t data[2];
	
	printf("\r\nPower VSP VSN setting...\r\n");	
	POWER_I2C_Config();
	
	I2C_PORT_SDA = POWER_I2C_SDA_GPIO_PORT;
	I2C_PIN_SDA = POWER_I2C_SDA_PIN;
	I2C_PORT_SCL = POWER_I2C_SCL_GPIO_PORT;
	I2C_PIN_SCL = POWER_I2C_SCL_PIN;
	
	///////////////////FOR eDP power board///////////////////
	I2C_ADDR = 0xA0;
	data[0] = 0x2F;//VCI
	data[1] = 0x17;//OTP
	I2C_Sequential_Write(0x00, data, 2);
	Delay_ms(1);
	
	I2C_ADDR = 0xA6;
	data[0] = 0x80;//VSP
	data[1] = 0x80;//VSN
	I2C_Sequential_Write(0x00, data, 2);
	///////////////////FOR eDP power board///////////////////
	
	I2C_ADDR = POWER_I2C_ADDR;
	VSP_New = vsp_val * 10 - 50 + 0x0A;
	VSN_New = vsn_val * 10 - 50 + 0x0A;
	
	if (I2C_Sequential_Read(0x00, data, 2) == SET)
	{
		printf("POWER setting read fail!\r\n");
		return;
	}	
	printf("target setting reg[vsp]=%d reg[vsn]=%d ,read reg[vsp]=%d, reg[vsn]=%d\r\n", VSP_New, VSN_New, data[0], data[1]);
	
	if ((VSP_New == data[0]) && (VSN_New == data[1])) //reduce erase times, only write when setting change
	{
		printf("Power VSP VSN setting is already meet!\r\n");
		return;
	}
	
	data[0] = VSP_New;
	data[1] = VSN_New;
	if (I2C_Sequential_Write(0x00, data, 2) == SET) //VSP VSN
	{
		printf("Power VSP setting fail!\r\n");
		return;
	}	
	Delay_ms(1);
	
	if (I2C_Write_Byte(0x03, 0x43) == SET) //current£º80mA + discharge active 	//0x03 £º40mA 
	{
		printf("Power current setting fail!\r\n");
		return;
	}
	Delay_ms(1);
	
	if (I2C_Write_Byte(0xFF, 0x80) == SET) // Write to NVM 
	{
		printf("Power write to NVM !\r\n");
		return;
	}	
}

/*********************************************************************************
* Function: LCMPower_Init
* Description: set power initial state
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCMPower_Init(void)
{	
	LCD_PWM(0x0000);
	POWER_VCI_Set();
	POWER_IOVCC_Reset(); //prevent lit NG probabilistic, ensure IOVCC is stable when 2828 start to work
	POWER_OTP_Reset();	//PET12 high active 
	POWER_VSN_Set();
	POWER_VSP_Set();
}

///*********************************************************************************
//* Function: LCMPower_ON
//* Description: power on timing, low active
//* Input: none
//* Output: none
//* Return: none
//* Call: external
//*/
void LCMPower_ON(void)
{	
 	POWER_IOVCC_Reset();
	Delay_ms(10);
 	POWER_VCI_Reset();
	Delay_ms(1);
	POWER_VSP_Reset();	
	Delay_ms(1);
	POWER_VSN_Reset();
	Delay_ms(1);
	
	printf("\r\nLCMPower_ON...\r\n");
}

///*********************************************************************************
//* Function: LCMPower_OFF
//* Description: power off timing, low active
//* Input: none
//* Output: none
//* Return: none
//* Call: external
//*/
void LCMPower_OFF(void)
{	
 	LCD_PWM(0x0000);
	Delay_ms(1); 
	POWER_VSN_Set();
	Delay_ms(1);
	POWER_VSP_Set();
	Delay_ms(1);	
 	POWER_VCI_Set();
	Delay_ms(1);
	POWER_IOVCC_Set();
	Delay_ms(1);	
	
	printf("\r\nLCMPower_OFF...\r\n");
}

/*********************************************************************************
* Function: LCMPower_ON
* Description: power on timing, high active
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCMPower_ON1(void)
{	
 	POWER_IOVCC_Set();
	Delay_ms(10);
 	POWER_VCI_Set();
	Delay_ms(1);
	POWER_VSP_Set();	
	Delay_ms(1);
	POWER_VSN_Set();
	Delay_ms(1);
	
	printf("\r\nLCMPower_ON...\r\n");
}

/*********************************************************************************
* Function: LCMPower_OFF
* Description: power off timing, high active
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCMPower_OFF1(void)
{	
 	LCD_PWM(0x0000);
	Delay_ms(1); 
	POWER_VSN_Reset();
	Delay_ms(1);
	POWER_VSP_Reset();
	Delay_ms(1);	
 	POWER_VCI_Reset();
	Delay_ms(1);
	POWER_IOVCC_Reset();
	Delay_ms(1);	
	
	printf("\r\nLCMPower_OFF...\r\n");
}
