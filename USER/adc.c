/*
 *			 ------------------------
 *			| PF3(ͨ��9)  - ADC3     |
 *         	 ------------------------
 */

#include "include.h"

#define SAMPLE_NUM 	500		// ��������
#define BUFFER_LEN	7		// �������峤�ȣ�����������һ������ȡ��ֵΪ��Чֵ����ֵ�˲���������ֵ���Ϊ����

__IO u16 ADC_ConvertedValue;
uint16_t ADC_ValueBuffer[BUFFER_LEN];
uint16_t maxVoltage;
uint16_t minVoltage;
uint16_t sampleCnt;
uint8_t bufferIndex;

/*********************************************************************************
 * ��������ADC3_Config
 * ����  ��ʹ��ADC3��DMA2��ʱ�ӣ���ʼ��PF.3������ADC3�Ĺ���ģʽΪDMAģʽ
 * ����  ����
 * ���  ����																			
 * ����  ����
 * ����  ���ⲿ����
 */
void ADC3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* ADC3 GPIO config */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
     
 	/* ADC3 common config */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; // ����ADCʱ��	ADCCLK = APB2CLK / 2 = 30M
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure); 	
	
	/* ADC3 mode config */
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	// ��������
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;  // ��ͨ��
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	// ����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ���ⲿ����
 //	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	 // �ⲿ����Դ 
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfConversion = 1;	 // ͨ����
  	ADC_Init(ADC3, &ADC_InitStructure);

	/* DMA stream configuration */
	DMA_DeInit(DMA2_Stream0);	// ��defaultֵ��ʼ��
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
   	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	  // ���ݴ��䷽������-> Memory  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;	// ��ʹ��FIFO
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		// Memory����λ��16λ (32λ��MCU��1��half-wordռ16 bits) 	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	 // Memory��ַ������
  	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_BASE + 0x4C;	// ADC_DR = 0x4C
 	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	   // ��������λ��16λ
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ������ 
  	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);	// Դ���ݱ仯ʱ����DMA����
  	DMA_Cmd(DMA2_Stream0, ENABLE);	 // ʹ��ADC��DMA����

  	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_144Cycles); //ADC3 regular channel9 configuration 
  	ADC_DMACmd(ADC3, ENABLE);  //Enable ADC3 DMA
	ADC_Cmd(ADC3, ENABLE);  // Enable ADC3

	ADC_SoftwareStartConv(ADC3);	// Start ADC3 Software Conversion
}

/*********************************************************************************
 * ��������BubbleSort
 * ����  ��ð������
 * ����  ��-data ����������
 *         -lenth ���鳤��
 * ���  ����																			
 * ����  ����
 * ����  ���ڲ�����
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
 * ��������ReadSensor
 * ����  ����ȡת�����Sensor��ѹֵ��������ֵ�˲�
 * ����  ����
 * ���  ����																			
 * ����  ����
 * ����  ���ڲ�����
 */
uint16_t ReadSensor(void)
 {
uint16_t tempVoltage;
bufferIndex = 0;
while(bufferIndex < BUFFER_LEN){
ADC_ValueBuffer[bufferIndex++] = ADC_ConvertedValue;

Delay_us(10);
// printf("ADC return data is %d\r\n",ADC_ValueBuffer[bufferIndex-1]);
}

if (bufferIndex == BUFFER_LEN)
{
BubbleSort(ADC_ValueBuffer, BUFFER_LEN);
tempVoltage = ADC_ValueBuffer[BUFFER_LEN >> 1];
if (maxVoltage < tempVoltage)
{
maxVoltage =  tempVoltage;
}
if (minVoltage > tempVoltage)
{
minVoltage =  tempVoltage;
}
bufferIndex = 0;
sampleCnt--;
}
bufferIndex =0;
while(bufferIndex < BUFFER_LEN){
printf("ADC return data is %d\r\n",ADC_ValueBuffer[bufferIndex++]);
}
 printf("voltage is %f\r\n",3.33*ADC_ValueBuffer[3]/0xFFF);
// printf("Reference is %f\r\n", (3.33-3.33*ADC_ValueBuffer[3]/0xFFF)/(3.33*ADC_ValueBuffer[3]/0xFFF/240));
return ADC_ValueBuffer[3];
 }



/******************************RESISTOR TEST***************************************************/
/******************************RESISTOR TEST***************************************************/
double Voltage(void)
{
double V_OTP;
uint16_t V_TEST;
V_TEST=ReadSensor();
if(V_TEST<6)
V_OTP=999;
else
{
V_OTP=((3.33*ADC_ValueBuffer[3]/0xFFF)+0.01)*3;
printf("V_OTP is %f\r\n",V_OTP);
}
return V_OTP;
}
//
void Sys_DET_Resistor(void)
{
uint16_t V_VPP;
double V_OTP;	
uint8_t redbuf[8];	
V_VPP=Voltage()*10;
V_OTP=Voltage();
//redbuf[0]=V_VPP/100+0x30;
//redbuf[1]=V_VPP/10+0x30;
//redbuf[2]=V_VPP%10+0x30;
redbuf[0]=V_VPP/10+0x30;
redbuf[1]='.';
redbuf[2]=V_VPP%10+0x30;
redbuf[3]=' ';
redbuf[4]='_';
redbuf[5]='V';
redbuf[6]='P';
redbuf[7]='P';		
if(V_OTP>=6.2&&V_OTP<=7)
{

FPGA_DisPattern(22, 127, 127, 127);
//FPGA_Info_Set(redbuf);
//FPGA_SPI3Write(ADDR_INFO_SHOW_EN);
//FPGA_SPI3Write(INFO_VERSION);
}	
else
{
FPGA_DisPicture(2);	
FPGA_Info_Set(redbuf);

}	
}

/////////////////////////////////////
void Con_Check(void)
{
	if ((GPIO_ReadInputDataBit(TEST24_GPIO_PORT, TEST24_PIN) == RESET))	
	{
					printf("*#*#E:0#*#*\r\n"); //zxj@20160526 for G6 ET
//					LCD_PWM(0x00);
//				  Delay_ms(10);
	        LCD_SleepIn();
          LCMPower_OFF();		
	        LCD_VideoMode_OFF();
	        LCD_LPMode();
	        MIPI_SleepMode_ON();	
				  GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
         	GPIO_ResetBits(TEST31_GPIO_PORT, TEST31_PIN); //new

	}	
}


