#include "include.h"

char Admesy_RData[ADMESY_RDATA_LEN];
uint16_t Admesy_RDataCnt = 0;
FlagStatus Admesy_RDataFlag = RESET;
ErrorStatus MSE_State = ERROR;
float	Flicker;
float color_x;
float	color_y;
float	Lv;
int CCT;

float	color_u;
float	color_v;

/*********************************************************************************
* Function: Admesy_Init
* Description: initial MSE
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Admesy_Init(void)
{
	uint8_t Try_Count = 3;	
	uint16_t m = 0;
	
	/* Set for Brontes
	*/	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":SENSe:INT 200000\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
	if (MSE_State == ERROR) return;
	
	/* Set auto-range parameters
	* freq: frequency of display to measure. (Hz)	
	* frames: number of frames of display to measure.  
	* adjmin: minimal percentage level for measurement to be executed. (%)
	* command:
	* :SENSe:AUTOPARMS freq,frames,adjmin\n
	*/	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
//		USART_printf(MSE_COM, (uint8_t *)":SENSe:AUTOPARMS 60,2,6\n");
		USART_printf(MSE_COM, (uint8_t *)":SENSe:AUTOPARMS 60,1,15\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
	if (MSE_State == ERROR) return;
	
  /* set the auto-range to on. with using the above parameters: 
	* On: 1, Off: 0
	*/
	Try_Count = 3;	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":SENSe:AUTORANGE 1\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
	if (MSE_State == ERROR) return;
	
	/* Set the maximum integration time. 
	* If set too low, it is not possible to measure dark levels well 
	* :SENSe:MAXINT maxint\n  
	* range: 1000 ¨C 5000000 uS
	*/
	Try_Count = 3;	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":SENSe:MAXINT 500000\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
	if (MSE_State == ERROR) return;
	
	/* averaging set to 1 */
	Try_Count = 3;	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":sens:aver 1\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E)  //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
	if (MSE_State == ERROR) return;
	
	/* Set the user calibration matrix */
	Try_Count = 3;	
	while (Try_Count--)
	{	
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":sens:sbw user2\n");
		while (Admesy_RDataFlag == RESET && m < 100)
		{
			Delay_ms(1);
			m++;
		}
		if (Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E)  //recieve data is '>>', which is MSE ack
		{
			MSE_State = SUCCESS;
			break;
		}
		MSE_State = ERROR;		
	}
}

/*********************************************************************************
* Function: Meas_Flicker
* Description: measure flicker in %
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_Flicker(void)
{
	uint8_t Try_Count = 3;
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":meas:Flicker 4000\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if ((Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) || m >= 1000)	//recieve data is '>>', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}

	Flicker = atof(Admesy_RData);
	printf("The Flicker is %f\r\n", Flicker);
	printf("Try_Count = %d\r\n", Try_Count);
}

/*********************************************************************************
* Function: Meas_JEITA
* Description: measure flicker in DB
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_JEITA(void)
{
	float dt ;		//delta t (1/frequency)
	double *fsamples = malloc((ADMESY_FLICKER_SAMPLES + 10) * sizeof(double));	//alloc of sample Y data 
	char *temp1 = NULL;	
	uint16_t m;
	
	if(fsamples == NULL)
	{
		printf("There are NOT more RAM!\r\n");
	}
	else
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":sample:y %d,%d\n", ADMESY_FLICKER_SAMPLES, 3);
		while (Admesy_RDataFlag == RESET && m < 2000)
		{
			Delay_ms(1);
			m++;
		}
		
		temp1 = strtok(Admesy_RData ,"\t");
		dt = atof(temp1);
		temp1 = strtok(NULL , "\t");
		temp1 = strtok(NULL , "\t");
		temp1 = strtok(NULL , "\t");
		m = 0;
		while (temp1)
		{
			fsamples[m++] = atof(temp1);
			temp1 = strtok(NULL , "\t");
		}
		
		/* calculate JEITA flicker from sample Y signal (dB) */
		Flicker = calc_JEITA_flicker(&fsamples[0], dt, ADMESY_FLICKER_SAMPLES);
		printf("flicker JEITA = %fdB \r\n", Flicker);
		free(fsamples);
		fsamples = NULL;
	}
}

/*********************************************************************************
* Function: Meas_CCT_MSE
* Description: measure CCT, only for MSE, not support Brontes-LL
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_CCT_MSE(void)
{
	uint8_t Try_Count = 3;
	uint16_t m = 0;
	
	FPGA_DisPattern(0, 255, 255, 255);
	Delay_ms(500);	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":meas:CCT\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if ((Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) || m >= 1000)	//recieve data is '>>', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}
	CCT = atoi(Admesy_RData);
	
	printf("The CCT is %d\r\n", CCT);
	printf("Try_Count = %d\r\n", Try_Count);
}

/*********************************************************************************
* Function: Meas_XYZ
* Description: measure XYZ
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_XYZ(void)
{
	uint8_t Try_Count = 3;	
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":meas:XYZ\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if ((Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) || m >= 1000)	//recieve data is '>>', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}

	printf("The XYZ return from sensor is %s", Admesy_RData);
	printf("Try_Count = %d\r\n", Try_Count);
}

/*********************************************************************************
* Function: Meas_Yxy
* Description: measure xyLv
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_Yxy(void)
{
	char *tempY, *tempx, *tempy;
	uint8_t Try_Count = 3;	
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":meas:Yxy\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if ((Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) || m >= 1000)	//recieve data is '>>', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}
	
	printf("The Yxy return from sensor is %s ;\r\n", Admesy_RData);
	tempY = strtok(Admesy_RData , ",");
	tempx = strtok(NULL , ",");
	tempy = strtok(NULL , ",");
	Lv = atof(tempY);
	color_x = atof(tempx);
	color_y = atof(tempy);
	printf("The Lv = %f ; x = %f ; y = %f \r\n",Lv, color_x, color_y);
	printf("Try_Count = %d\r\n", Try_Count);
}

/*********************************************************************************
* Function: Meas_CCT
* Description: measure CCT
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_CCT(void)
{
	float f;

	Meas_Yxy();
	f = -(color_x - 0.3366) / (color_y - 0.1735);
	CCT = -949.86315 + 6253.80338 * exp(f / 0.92159) + 28.70599 * exp(f / 0.20039) + 0.00004 * exp(f / 0.07125); 
	printf("Caculate the CCT is %d \r\n", CCT);
}

/*********************************************************************************
* Function: Meas_Yuv
* Description: measure Yuv
* Input: none
* Output: none
* Return: none
* Call: external
*/
void Meas_Yuv(void)
{
	char *tempY, *tempx, *tempy;
	uint8_t Try_Count = 3;	
	uint16_t m;
	
	while(Try_Count--)
	{
		Admesy_RDataCnt = 0;
		Admesy_RDataFlag = RESET;
		m = 0;
		USART_printf(MSE_COM, (uint8_t *)":meas:Yuv\n");
		while (Admesy_RDataFlag == RESET && m < 1000)
		{
			Delay_ms(1);
			m++;
		}
		if ((Admesy_RData[0] == 0x3E && Admesy_RData[1] == 0x3E) || m >= 1000)	//recieve data is '>>', witch is not effetive data, try agagin
			continue;
		else
			break;	
	}
	
	printf("The Yuv return from sensor is %s ;\r\n", Admesy_RData);
	tempY = strtok(Admesy_RData , ",");
	tempx = strtok(NULL , ",");
	tempy = strtok(NULL , ",");
	Lv = atof(tempY);
	color_u = atof(tempx);
	color_v = atof(tempy);
	printf("The Lv = %f ; u = %f ; v = %f \r\n",Lv, color_u, color_v);
	printf("Try_Count = %d\r\n", Try_Count);
}
