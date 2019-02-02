/*
 *			 ----------------------------
 *			| PF3(channel 9)  - ADC3     |
 *       ----------------------------
 */

#include "include.h"

#define BUFFER_LEN	7		//buffer length, for medium filter, it is best to set odd number

__IO u16 ADC_ConvertedValue;


/*********************************************************************************
* Function: ADC3_Config
* Description: ADC3 configure
* Input: none
* Output: none
* Return: none
* Call: external
*/
void ADC3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* ADC3 GPIO config */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
     
 	/* ADC3 common config */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; //ADCCLK = APB2CLK / 2 = 30M
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure); 	
	
	/* ADC3 mode config */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 //	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

	/* DMA stream configuration */
	DMA_DeInit(DMA2_Stream0);	
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		//32bit MCU: 1 half-word = 16bit 	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_BASE + 0x4C;	// ADC_DR = 0x4C
 	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	   
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);	// source data change will start DMA transfer
  DMA_Cmd(DMA2_Stream0, ENABLE);	 

  ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_144Cycles); //ADC3 regular channel9 configuration 
  ADC_DMACmd(ADC3, ENABLE);  //Enable ADC3 DMA
	ADC_Cmd(ADC3, ENABLE);  // Enable ADC3

	ADC_SoftwareStartConv(ADC3);	// Start ADC3 Software Conversion
}

/*********************************************************************************
* Function: BubbleSort
* Description: bubble sort
* Input: -data
* Input: -lenth
* Output: none
* Return: none
* Call: internal
*/
void BubbleSort(uint16_t * data, uint8_t lenth)
{
	uint8_t i;
	uint8_t j;
	uint16_t dataTemp;

	for (i = 0; i < lenth; i++)
	{
		for (j = 1; j < (lenth - i); j++)
		{
			if (data[j] < data[j-1])
			{
				dataTemp = data[j];
				data[j] = data[j-1];
				data[j-1] = dataTemp;
			}
		}
	}
}

/*********************************************************************************
* Function: Get_ADC3_Voltage
* Description: bubble sort
* Input: none
* Output: none
* Return: none
* Call: internal
*/
uint16_t Get_ADC3_Voltage(void)
{
	uint16_t ADC_ValueBuffer[BUFFER_LEN];
	uint8_t bufferIndex = 0;
	
	while (bufferIndex < BUFFER_LEN)
	{
		Delay_us(20);
		ADC_ValueBuffer[bufferIndex++] = ADC_ConvertedValue;
	}

	return (ADC_ValueBuffer[BUFFER_LEN >> 1]);
}



/*********************************************************************************
* Function: GetResValue
* Description: adc get voltage value to calc the resistor 
* Input:  none
* Output: none
* Return: float tempRes,calc resistor value
* Call: external
*/
#define SAMPLE_NUM 128
#define R_PULL_HIGH 100000	//define pull high resistor value ,according to board circuit. 
float GetResValue(void)
{ 
	char str[12];
	uint32_t total_sample =0;
	uint16_t avg_sample = 0;
	float tempVoltage =0.0, tempRes =0.0;
	uint16_t sampleCnt = SAMPLE_NUM;

	while (sampleCnt--) 
 {
		Delay_us(20);
		total_sample += ADC_ConvertedValue;
//	printf("ADC_ConvertedValue is :%d\r\n",ADC_ConvertedValue);
 }
	avg_sample = total_sample / SAMPLE_NUM +0.5;
//	printf("get %d times adc ,calc the average adc value is: %d\r\n",SAMPLE_NUM,avg_sample);
	tempVoltage = 3.3 * avg_sample / 0x0FFF;
	if(tempVoltage> 3.2)
//  notice : this value is base on the hardware pull high resistor, spec_min< x/(x+pullhigh) <spec_max
	{
		tempRes = 9999999;
		sprintf(str,"AG_OPEN");
		printf("error:resistor of silver paste is larger than spec.!\r\n");
	}
	else
	{
		tempRes = tempVoltage /(3.3 - tempVoltage) * R_PULL_HIGH;
		printf("calc Voltage is %0.2fV\t tempRes is %0.1fK\r\n",tempVoltage,tempRes/1000);
		sprintf(str,"AG_%0.1fR",tempRes);
	}
	FPGA_Info_Set((uint8_t *)str);
	FPGA_Info_Visible(INFO_STR);

	return tempRes;
}
