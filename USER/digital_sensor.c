#include "include.h"

#define DSENSOR_I2C_ADDR1	0x42    // AD7994 bus address(7bit): 010 0001
#define DSENSOR_I2C_ADDR2	0x46    // AD7994 bus address(7bit): 010 0011

#define SAMPLE_RATE	800		//samples

uint8_t DSENSOR_I2C_ADDR = DSENSOR_I2C_ADDR1;

/*********************************************************************************
* Function: Check_ADCADDR
* Description: check ADC number
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void Check_ADCADDR(void)
{
	I2C_PORT_SDA = DSENSOR_I2C_SDA_GPIO_PORT;
	I2C_PIN_SDA = DSENSOR_I2C_SDA_PIN;
	I2C_PORT_SCL = DSENSOR_I2C_SCL_GPIO_PORT;
	I2C_PIN_SCL = DSENSOR_I2C_SCL_PIN;
	
	if (I2C_Check_Addr(DSENSOR_I2C_ADDR1) == SET)
	{
		DSENSOR_I2C_ADDR = DSENSOR_I2C_ADDR2;
	}	
	else
	{
		DSENSOR_I2C_ADDR = DSENSOR_I2C_ADDR1;
	}
	printf("\r\nDSENSOR_I2C_ADDR = 0x%02x\r\n", DSENSOR_I2C_ADDR);
}

/*********************************************************************************
* Function: DSENSOR_I2C_Config
* Description: digital sensor I2C interface configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void DSENSOR_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = DSENSOR_I2C_SCL_PIN;
	GPIO_Init(DSENSOR_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DSENSOR_I2C_SCL_GPIO_PORT, DSENSOR_I2C_SCL_PIN);
	
	GPIO_InitStructure.GPIO_Pin = DSENSOR_I2C_SDA_PIN;
	GPIO_Init(DSENSOR_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_SetBits(DSENSOR_I2C_SDA_GPIO_PORT, DSENSOR_I2C_SDA_PIN);
	
	Check_ADCADDR();
}

/*********************************************************************************
* Function: I2C_ReadSensor
* Description: read voltage value transform by digital sensor
* Input: -CHNX, sample channel
* Output: none
* Return: voltage value
* Call: internal
*/
uint16_t I2C_ReadSensor(uint8_t CHNX)
 {
	uint8_t data[2];
 
	if(I2C_Sequential_Read(CHNX, data, 2) == SET)
	{
		printf("Digital sensor read fail!\r\n");
		return 0xFFFF;    // error flag
	}

	return (uint16_t)(((data[0] & 0x0F) << 8) + data[1]);
 }

 /*********************************************************************************
* Function: DSENSOR_I2C_GetFlicker
* Description: calcultate flicker
* Input: -CHNX, sample channel
* Output: none
* Return: flicker(%)£¬0 indicate sample error
* Call: external
*/
 float DSENSOR_I2C_GetFlicker(uint8_t CHNX)
 {
	uint16_t maxVoltage;
	uint16_t minVoltage;
	uint16_t tempVoltage;
	uint16_t sampleCnt;

	I2C_PORT_SDA = DSENSOR_I2C_SDA_GPIO_PORT;
	I2C_PIN_SDA = DSENSOR_I2C_SDA_PIN;
	I2C_PORT_SCL = DSENSOR_I2C_SCL_GPIO_PORT;
	I2C_PIN_SCL = DSENSOR_I2C_SCL_PIN;
	I2C_ADDR = DSENSOR_I2C_ADDR;
	 
	maxVoltage = 0x0000;
	minVoltage = 0x0FFF;
	sampleCnt = SAMPLE_RATE;
	tempVoltage = I2C_ReadSensor(CHNX);	 // discard first data

	while (sampleCnt)
	{
		tempVoltage = I2C_ReadSensor(CHNX);
//		printf("tempVoltage = 0x%02x\r\n", tempVoltage);
		if (tempVoltage == 0xFFFF) return 0;
		if (tempVoltage > maxVoltage) maxVoltage = tempVoltage;
		if (tempVoltage < minVoltage) minVoltage = tempVoltage;
		sampleCnt--;
	}

	if (maxVoltage > 0xFF0 || minVoltage < 0x010)
	{
		printf("Waring: maxVoltage = 0x%04x, minVoltage = 0x%04x\r\n", maxVoltage, minVoltage);
		printf("Waring: The senor may be wrong! Please try again or replace the sensor!\r\n");
		return 0;
	}
	
	return ((maxVoltage - minVoltage) / ((maxVoltage + minVoltage) / 2.0) * 100);	//FMA
 }
 
 /*********************************************************************************
* Function: DSENSOR_Test
* Description: test the brightess range of panel
* Input: -CHNX, sample channel
* Output: none
* Return: none
* Call: external
*/
 void DSENSOR_Test(uint8_t CHNX)
 {
	 uint16_t i = 0;
	
	 I2C_PORT_SDA = DSENSOR_I2C_SDA_GPIO_PORT;
	 I2C_PIN_SDA = DSENSOR_I2C_SDA_PIN;
	 I2C_PORT_SCL = DSENSOR_I2C_SCL_GPIO_PORT;
	 I2C_PIN_SCL = DSENSOR_I2C_SCL_PIN;
	 I2C_ADDR = DSENSOR_I2C_ADDR;

	 for (; i < 256; i++)
	 {
		 FPGA_DisPattern(0, i, i, i); 
		 Delay_ms(500);
		 printf("gray%d: 0x%02x\r\n", i, I2C_ReadSensor(CHNX));
	 }
 }
