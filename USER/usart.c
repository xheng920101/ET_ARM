/*
 * 	J14		 --------------------
 *			| PA9  - USART1(Tx)  |
 *          | PA10 - USART1(Rx)  |
 *         	 --------------------
 *	J7		 --------------------
 *			| PA0  - UART4(Tx)   |
 *          | PA1  - UART4(Rx)   |
 *         	 --------------------
 *	J7		 --------------------
 *	(NC)	| PC6  - USART6(Tx)  |
 *          | PC7  - USART6(Rx)  |
 *         	 --------------------
 */

#include "include.h"

uint16_t USART_RData[CODE_MAX_LEN];
uint16_t USART_RDataCnt = 0;
FlagStatus USART_EvenStatus = SET;
FlagStatus USART_ControlStatus = RESET;
FlagStatus USART_CMD_FLAG = RESET;
FlagStatus Debug = RESET;

/*********************************************************************************
* Function: USART1_Config
* Description: USART1 configure, 115200 8-N-1
* Input: none
* Output: none
* Return: none
* Call: external
*/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* USART1 GPIO config */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure USART1 Tx (PA.9) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable the USART1 Interrupt */
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //clear interrupt flag
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	  //enable RXNE interrupt
	USART_Cmd(USART1, ENABLE);
}
 
/*********************************************************************************
* Function: UART4_Config
* Description: UART4 configure, 115200 8-N-1
* Input: none
* Output: none
* Return: none
* Call: external
*/
void UART4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config UART4 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* UART4 GPIO config */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	/* Configure UART4 Tx (PA.0) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure UART4 Rx (PA.1) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* UART4 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	/* Enable the UART4 Interrupt */
	USART_ClearITPendingBit(UART4, USART_IT_RXNE);   //clear interrupt flag  
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); 	 //enable RXNE interrupt
	USART_Cmd(UART4, ENABLE);
}

/*********************************************************************************
* Function: fputc
* Description: relocate printf function
* Input: -ch
* Input: -f
* Output: none
* Return: ch
* Call: call by printf
*/
int fputc(int ch, FILE *f)
{
	USART_SendData(PC_COM, (unsigned char) ch);
	while (USART_GetFlagStatus(PC_COM, USART_FLAG_TXE) == RESET);

	return (ch);
}

/*********************************************************************************
* Function: PC_Data_Reciever
* Description: 
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void PC_Data_Reciever(void)
{
	uint8_t rdataTemp;
  if(USART_GetFlagStatus(PC_COM, USART_IT_RXNE) == SET) 
  {          
    rdataTemp = USART_ReceiveData(PC_COM); 	   //read data
		if (USART_CMD_FLAG == RESET)
		{
			if (USART_EvenStatus == SET)
			{
				USART_RData[USART_RDataCnt]= rdataTemp;
				USART_RData[USART_RDataCnt]<<= 8;
				USART_EvenStatus = RESET;
			}
			else
			{
				USART_RData[USART_RDataCnt++] |= rdataTemp;
				USART_EvenStatus = SET;
			}
			
			if (USART_RData[USART_RDataCnt - 1] == 0x1248 && USART_RData[USART_RDataCnt - 2] == 0x0F00) //end flag right
			{
				USART_CMD_FLAG = SET;
				USART_ControlStatus = SET;
				USART_RDataCnt = 0;			
			}
			
			if (USART_RData[USART_RDataCnt - 1] == 0x0012 && rdataTemp == 0x48) //end flag error
			{
				USART_CMD_FLAG = SET;
				USART_ControlStatus = SET;
				USART_RDataCnt = 0;
				USART_EvenStatus = SET;
				LED_ON(RED);
				LED_ON(GREEN);
			}
		}
		
		USART_ClearITPendingBit(PC_COM, USART_IT_RXNE);   //clear interrupt flag  
	}
}

/*********************************************************************************
* Function: MSE_Data_Reciever
* Description: 
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void MSE_Data_Reciever(void)
{
	if(USART_GetFlagStatus(MSE_COM, USART_IT_RXNE) == SET) 
  {        
		Admesy_RData[Admesy_RDataCnt++] = USART_ReceiveData(MSE_COM); 	   //read data
		if(Admesy_RData[Admesy_RDataCnt - 1] == 0x0A && Admesy_RData[Admesy_RDataCnt - 2] == 0x0D)//end flag
		{
			Admesy_RDataCnt = 0;
			Admesy_RDataFlag = SET;
		}
		USART_ClearITPendingBit(MSE_COM, USART_IT_RXNE);   //clear interrupt flag    
	}
}

/*********************************************************************************
* Function: USART1_RXD
* Description: interrupt handler
* Input: none
* Output: none
* Return: none
* Call: call by USART1_IRQHandler
*/
void USART1_RXD(void) 
{
	if (PC_COM == USART1)
	{
		PC_Data_Reciever();
	}
	else
	{
		MSE_Data_Reciever();
	}
}

/*********************************************************************************
* Function: UART4_RXD
* Description: interrupt handler
* Input: none
* Output: none
* Return: none
* Call: call by UART4_IRQHandler
*/
void UART4_RXD(void) 
{ 
	if (MSE_COM == USART1)
	{
		PC_Data_Reciever();
	}
	else
	{
		MSE_Data_Reciever();
	}
}

/*********************************************************************************
* Function: USART_EventProcess
* Description: usart event processing
* Input: none
* Output: none
* Return: none
* Call: external
*/
void USART_EventProcess(void)
{
	uint16_t i;
	uint8_t rdBuf[64];

	while (USART_ControlStatus == SET)
	{
		Connect_Check();
//		Current_Check();
		if (KEY_GetState() == KEY_SLEEP)
		{
		   KEY_SLEEPIN();
		}
		else if (KEY_GetState() == KEY_TEST)
		{
				DriverIC_Reset();	
				IC_Init(ET2_InitCode);	
				LCD_SleepOut();
				LCD_VideoMode_ON();
				LCD_HSMode();	
		}

		if (USART_CMD_FLAG == SET)	
		{
			switch (USART_RData[0])
			{
				case (0xAA01):
					IC_Init(&USART_RData[1]);
					break;
				case (0xAA02):
					if (USART_RData[1] == 107)
					{
						switch (USART_RData[3])
						{
							case (0x0000):	i = vsum / 2 - SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], i >> 8, i);	break;
							case (0x0001):	i = vsum / 2 - SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], (i >> 8) + 0x80, i);	break;
							case (0x0002):	i = vsum / 2 + SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], i >> 8, i);	break;
							case (0x0003):	i = vsum / 2 + SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2],(i >> 8) + 0x80, i);	break;
							case (0x0004):	i = vsum / 6 + SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], i >> 8, i);	break;
							case (0x0005):	i = vsum / 6 + SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], (i >> 8) + 0x80, i);	break;
							case (0x0006):	i = vsum / 6 - SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], i >> 8, i);	break;
							case (0x0007):	i = vsum / 6 - SAM_CRST_OFFSET;	FPGA_DisPattern(107, USART_RData[2], (i >> 8) + 0x80, i);	break;
							default:	break;
						}
					}
					else if (USART_RData[1] == 108)
					{
						switch (USART_RData[3])
						{
							case (0x0000):	i = hsum / 2 - SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], i >> 8, i);	break;
							case (0x0001):	i = hsum / 2 - SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], (i >> 8) + 0x80, i);	break;							
							case (0x0002):	i = hsum / 2 + SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], i >> 8, i);	break;
							case (0x0003):	i = hsum / 2 + SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], (i >> 8) + 0x80, i);	break;
							case (0x0004):	i = hsum / 6 + SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], i >> 8, i);	break;
							case (0x0005):	i = hsum / 6 + SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], (i >> 8) + 0x80, i);	break;
							case (0x0006):	i = hsum / 6 - SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], i >> 8, i);	break;
							case (0x0007):	i = hsum / 6 - SAM_CRST_OFFSET;	FPGA_DisPattern(108, USART_RData[2], (i >> 8) + 0x80, i);	break;
							default:	break;
						}
					}
					else
					{
						FPGA_DisPattern(USART_RData[1], USART_RData[2], USART_RData[3], USART_RData[4]);
					}
					Delay_ms(20);			
					printf("*#*#ACK#*#*\r\n");
					///////////////
//					switch (USART_RData[2])
//          {
//						case (0x0000):			    FPGA_DisPicture(0);break;
//					  case (0x0001):			    FPGA_DisPicture(1);break;
//					  case (0x0002):			    FPGA_DisPicture(2);break;
//					  case (0x0003):			    FPGA_DisPicture(3);break;
//					  case (0x0004):			    FPGA_DisPicture(4);break;
//					  case (0x0005):			    FPGA_DisPicture(5);break;
//					  case (0x0006):			    FPGA_DisPicture(6);break;
//					  case (0x0007):			    FPGA_DisPicture(7);break;
//					  case (0x0008):			    FPGA_DisPicture(8);break;
//					  case (0x0009):			    FPGA_DisPicture(9);break;
//					  case (0x000A):			    FPGA_DisPicture(10);break;
//					  case (0x000B):			    FPGA_DisPicture(11);break; 

//					default:	break;					
//					}
		
					///////////////
					Current_Check();	//zxj@20160628 for G5.5 auto AOI
					break;
				case (0xAA10):
					FPGA_DisPicture(USART_RData[1]);
					Delay_ms(20);
				  printf("*#*#ACK#*#*\r\n");
					Current_Check();	//zxj@20160628 for G5.5 auto AOI
					break;
				//////////////////////////////////////start: for gammaexpert //////////////////////////////////////////////////
				case (0xAA03):
				  Delay_ms(20);
				  printf("*#*#%s$%s$%s$%s$\r\n", Info_IC_Vendor, PROJECT_NO, Info_IC_Type, VERSION_OTP);                            
				  printf("%.2f$%.2f$%.2f$", (SPEC_GAMMA - SPEC_GAMMA_RANGE), SPEC_GAMMA, (SPEC_GAMMA + SPEC_GAMMA_RANGE));
				  printf("%.3f$%.3f$%.3f$", (SPEC_x - SPEC_xy_RANGE), SPEC_x, (SPEC_x + SPEC_xy_RANGE));
					printf("%.3f$%.3f$%.3f$", (SPEC_y - SPEC_xy_RANGE), SPEC_y, (SPEC_y + SPEC_xy_RANGE));
					printf("%.3f$%.3f$%.3f$%.3f$", Info_Target_X2, Info_Target_Y2, Info_Target_X3, Info_Target_Y3);
					printf("%.2f$%.2f$%.2f$", Info_GVDDP, Info_GVDDN, SPEC_Lv);
					printf("%d$%d$%d$%d$\r\n", Info_XY_CCT, Info_Min_CCT, Info_Target_CCT, Info_Max_CCT);
				  printf("%s$%s$%s$%.2f$", Info_Min_VCOM, Info_Max_VCOM, Info_Typical_VCOM, SPEC_FLICKER_DB);
				  printf("%d$%d$", Info_Adjust_Gamma, Info_Adjust_Fliker);
					printf("%.3f$%.3f$%.3f$%.3f$", Info_Delta_XY_min, Info_Delta_XY_max, Info_Delta_X, Info_Delta_Y);
					printf("%.2f$%.3f$", SPEC_CONTRAST, Info_Crosstalk);
					printf("%.3f$%.3f$%.3f$", Info_TarRx, Info_TarRy, Info_RangeR);
					printf("%.3f$%.3f$%.3f$", Info_TarGx, Info_TarGy, Info_RangeG);
					printf("%.3f$%.3f$%.3f$", Info_TarBx, Info_TarBy, Info_RangeB);
					printf("%s$\r\n", Info_Check_gamma);
				  printf("%s$\r\n", Info_Fliker_Code);
				  printf("%s#*#*\r\n", Info_Gamma_Register);
				  Delay_ms(20);
				  break;
				case (0xAA04):
					LED_ON(BLUE);
					if (OTPSequence()== SUCCESS)
					{
						printf("*#*#OTP OK#*#*\r\n"); 
						LED_ON(GREEN);
						LED_OFF(RED); 
						LED_OFF(BLUE); 
					}
					else
					{
						printf("*#*#OTP NG#*#*\r\n");
						LED_ON(RED);
						LED_OFF(BLUE);
						LED_OFF(GREEN);
					} 
					KEY_SLEEPIN();
					break;
				case (0xAA05):              
					if (OTP_TIMES >= OTP_TIMES_MAX)
					{
						if (OTP_TIMES == 3)
						{
							FPGA_DisPattern(85, 0, 0, 0);
							LED_ON(RED);
							LED_OFF(BLUE);                                                
							LED_OFF(GREEN);					
						}
						else
						{
							LED_ON(GREEN);
							LED_OFF(BLUE);                                                
							LED_OFF(RED);														
						}						
					}
					else
					{
						printf("Check NG\r\n");
						FPGA_DisPattern(85, 0, 0, 0);
						LED_ON(RED);
						LED_OFF(BLUE);                                                
						LED_OFF(GREEN); 				
					}
					KEY_SLEEPIN();
					break;
				case (0xAA06):
					printf("\r\nDriver IC_Reset...\r\n");
					DriverIC_Reset();				
					Delay_ms(10);  //60
					printf("\r\nMIPI_Reset...\r\n");
					MIPI_Reset();
					Delay_ms(100);			
					IC_Init(SSDInitCode);	
					Delay_ms(20);	
  				printf("\r\nDriver IC_Reset...\r\n");				
					DriverIC_Reset();	
					Delay_ms(50);	
					IC_Init(ET1_InitCode);
					Delay_ms(50);
					LCD_PWM(0xFF);		
					break;	
				case (0xAA07):
					printf("\r\nDriver IC_Reset...\r\n");
					DriverIC_Reset();				
					Delay_ms(10);
					printf("\r\nMIPI_Reset...\r\n");
					MIPI_Reset();
					Delay_ms(100);			
					IC_Init(SSDInitCode);	
					Delay_ms(20);	
					printf("\r\nDriver IC_Reset...\r\n");				
					DriverIC_Reset();	
					Delay_ms(50);	
					IC_Init(ET2_InitCode);
					Delay_ms(50);
					LCD_PWM(0xFF);					
					break;
				case (0xAABB): 	// for AUTOGAMMA macth
					printf("ACK\r\n");
					break;
				//////////////////////////////////////end: for gammaexpert //////////////////////////////////////////////////					
				case (0xAA08):
					FPGA_porchPara.HACT = USART_RData[1];
					FPGA_porchPara.VACT = USART_RData[2];
					FPGA_porchPara.HSA = USART_RData[3];
					FPGA_porchPara.HBP = USART_RData[4];
					FPGA_porchPara.HFP = USART_RData[5];
					FPGA_porchPara.VSA = USART_RData[6];
					FPGA_porchPara.VBP = USART_RData[7];
					FPGA_porchPara.VFP = USART_RData[8];
					FPGA_Porch_Set(FPGA_porchPara);
					break;
				case (0xAA20):
					USART_ControlStatus = RESET;
					break;
				case (0xAA21):
					OTP_TIMES = OTPTimes_Read();
					FPGA_Info_Visible(USART_RData[1]);
					break;
				case (0xAA22):
					for (i = 0; i < 14; i++)
					{
						rdBuf[i] = USART_RData[i + 1];
					}
					FPGA_Info_Set(rdBuf);
					break;
				case (0xAA40):
					LED_ON(BLUE);
					OTP_TIMES = OTPTimes_Read();
					if (OTP_TIMES >= OTP_TIMES_MAX)
					{
						printf("OTP times is over %d!\r\n", OTP_TIMES_MAX);
						printf("OTP NG\r\n"); // for sony10.1 autogamma
						Delay_ms(5);
						printf("\r\n*#*#OTP:NG#*#*\r\n"); // for gammaExpert and autoLine OTP
						FPGA_Info_Visible(INFO_NONE);	
						FPGA_DisPattern(85, 0, 0, 0);
						LED_ON(RED);
						LED_OFF(BLUE);
					}
					else if (OTPSequence() == SUCCESS)
					{
						Delay_ms(5);
						printf("\r\n*#*#OTP:OK#*#*\r\n");
						FPGA_Info_Visible(INFO_OTPTIMES);
						FPGA_DisPattern(22, 128, 128, 128);	
						LED_ON(GREEN);
						LED_OFF(BLUE);
					}
					else
					{
						Delay_ms(5);
						printf("\r\n*#*#OTP:NG#*#*\r\n");
						FPGA_Info_Visible(INFO_NONE);	
						FPGA_DisPattern(85, 0, 0, 0);
						LED_ON(RED);
						LED_OFF(BLUE);
					}				
					break;
				case (0xAA41):
					if (AutoVcom() == ERROR)
					{
						printf("AutoVcom fail! Please check if both sensor and panel are ready.\r\n");
					}
					break;
				case (0xAA42):
					LCD_SleepIn();
					LCD_VideoMode_OFF();
					MIPI_SleepMode_ON();
					LCMPower_OFF();	
				  printf("\r\n*#*#POWER:OFF#*#*\r\n");
					while(1);
#ifdef CHROMA
				case (0xAA80):	
					IC_Init(&gamma[USART_RData[1]][0]);
					break;
				case (0xAA85): 	// write chroma to DRIVER IC
					Chroma_Set(USART_RData[1], USART_RData[2]);
					break;
				case (0xAA86): 	// write date to DRIVER IC
					Date_Set(USART_RData[1], USART_RData[2], USART_RData[3]);
					break;
#endif
				case (0xAA81):
					VCOM_Set(USART_RData[1]);
				  Delay_ms(5);
				  printf("\r\n*#*#VCOM:OK#*#*\r\n");
					break;
				case (0xAA82):	// READ DRIVER IC REGISTER
					SSD_B7 |= SSD_CFGR_REN;	 // READ MODE 
					printf("\r\nSSD_B7 = 0x%04x\r\n", SSD_B7);
					ReadDriverReg(MAIN_PORT, USART_RData[1], USART_RData[2], rdBuf);
					break;
				case (0xAA83): 	// WRITE SSD2828 REGISTER
					WriteCMD2SSD((PORT0 | PORT1), USART_RData[1]);
					WriteDATA2SSD((PORT0 | PORT1), USART_RData[2]);
					WriteDATA2SSD((PORT0 | PORT1), USART_RData[3]);
					SSD_B7 = ReadSSDReg(MAIN_PORT, SSD_CFGR);
					printf("SSD_B7 = 0x%04X\r\n", SSD_B7);
					break;
				case (0xAA84): 	// READ SSD2828 REGISTER
					printf("The parameter of SSD2828 register 0x%02X is 0x%04X\r\n", USART_RData[1], ReadSSDReg(MAIN_PORT, USART_RData[1]));
					break;
				case (0xAA90):
					if (USART_RData[1] == 0x0001)	POWER_IOVCC_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_IOVCC_Set();
					break;
				case (0xAA91):
					if (USART_RData[1] == 0x0001)	POWER_VCI_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_VCI_Set();
					break;
				case (0xAA92):
					if (USART_RData[1] == 0x0001)	POWER_VSP_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_VSP_Set();
					break;
				case (0xAA93):
					if (USART_RData[1] == 0x0001)	POWER_VSN_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_VSN_Set();
					break;
				case (0xAA94):
					if (USART_RData[1] == 0x0001)	POWER_LED_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_LED_Set();
					break;
				case (0xAA95):
					if (USART_RData[1] == 0x0001)	POWER_OTP_Reset();
					else if (USART_RData[1] == 0x0000)	POWER_OTP_Set();
					break;
				case (0xAA96):
					LCD_PWM(USART_RData[1] &0X00FF);
					break;
				//////////////////////////////////////start: for auto line check//////////////////////////////////////////////////
				case (0xAAA0):		//power on	
						LED_ON(GREEN);
						LED_OFF(RED);
						LED_OFF(BLUE);
						TEST_Config_ON();
						LCM_Init();
						OTP_TIMES = OTPTimes_Read();
						FPGA_Info_Visible(INFO_NONE);
						if (OTP_TIMES == 0)
						{
							FPGA_DisPattern(84, 0, 0, 0);
						}
						else
						{
							FPGA_DisPattern(0, 255, 255, 255); 
						}
 						Delay_ms(5);
						LED_OFF(GREEN);
						printf("\r\n*#*#POWER:ON#*#*\r\n");
					break;
				case (0xAAA1):	//power off
						LED_ON(RED);
						LCD_PWM(0x00);
						LCD_SleepIn();
						LCD_VideoMode_OFF();
						MIPI_SleepMode_ON();
						LCMPower_OFF();	
						TEST_Config_OFF();
						Delay_ms(5);
						LED_OFF(BLUE);
						LED_OFF(RED);
						LED_OFF(GREEN);
						printf("\r\n*#*#POWER:OFF#*#*\r\n");
					break;
				case (0xAAA2):	//program version		
						Project_Info_Upload();
					break;
				case (0xAAA3):	//otp
					LED_ON(BLUE);
					if (MSE_State == ERROR)
					{
						FPGA_DisPattern(114, 0, 0, 0);
						FPGA_Info_Visible(INFO_STR);
						FPGA_Info_Set((uint8_t *)"MSE ERROR");
						printf("MSE error! Please check the MSE connection!\r\n");
						break;
					}	
					
					FPGA_DisPattern(0, 255, 255, 255);
					Delay_ms(1000); //20151216:200-->1000	
					Meas_Yxy();
					if (Lv < 100)
					{
						printf("\r\n*#*#LIT:NG#*#*\r\n");
						break;
					}
					
					if (OTP() == ERROR)
					{
						FPGA_Info_Visible(INFO_NONE);	
						FPGA_DisPattern(85, 0, 0, 0);
						LED_OFF(BLUE);
					}
					else
					{
						FPGA_Info_Visible(INFO_OTPTIMES);
						FPGA_DisPattern(22, 128, 128, 128);	
						LED_OFF(BLUE);
					}
					printf("\r\n*#*#END#*#*\r\n");
					break;
				//////////////////////////////////////end: for auto line check//////////////////////////////////////////////////
				default:
					USART_ControlStatus = RESET;
					break;			
			}
			USART_CMD_FLAG = RESET;
#ifdef GAMMAEXPERT
			USART_ControlStatus = RESET;
#endif
		}
// 		else//mark for decrease time respond!
// 		{
// 			LED_OFF(BLUE);
// 			Delay_ms(100);
// 			LED_ON(BLUE);
// 			Delay_ms(100);		           
// 		}
	}
}

/*********************************************************************************
* Function: itoa
* Description: convert int data to string
* Input: -value, int data
* Input: -radix, = 10 indicate decimal, others return null
* Output: -string
* Return: pointer to the string
* Call: call by USART_printf
*/
static char *itoa(int value, char *string, int radix)
{
    int i, d;
    int flag = 0;
    char *ptr = string;

    if (radix != 10) // non-decimal, ruturn null string
    {
        *ptr = 0;
        return string;
    }

    if (!value) // 0
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    if (value < 0) 
    {
        *ptr++ = '-';  //symbol
        value *= -1;   //absolute value
    }

    for (i = 10000; i > 0; i /= 10) //posetive value
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;  //flag to indicate if 0 is effective
        }
    }

    *ptr = 0; //end of string

    return string;
}

/*********************************************************************************
* Function: USART_printf
* Description: like printf function
* Input: -USARTx
* Input: -Data, pointer to the string to be sent
* Input: -..., other parameters
* Output: none
* Return: none
* Call: external
*********************************************************************************
*	USART_printf( UART4, "\r\n this is a demo \r\n" );
* USART_printf( UART4, "\r\n %d \r\n", i );
* USART_printf( UART4, "\r\n %s \r\n", j );
*/
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data, ...)
{
	const char *s;
 	int d;   
	char buf[16];

  va_list ap;
 	va_start(ap, Data);

	while (*Data != 0)     //check if the end of the string
	{				                          
		if (*Data == 0x5c)  // '\' 
		{									  
			switch (*++Data)
			{
				case 'r':	// return
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':	// newline
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if (*Data == '%')	// format
		{									 
			switch (*++Data)
			{				
				case 's':	// string
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx, *s);
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
          }
					Data++;
          break;				
				case 'd': // decimal
					d = va_arg(ap, int);
		      itoa(d, buf, 10);
		      for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		      }
					Data++;
          break;
				default:
					Data++;
				  break;
			}		 
		} 
		else 
		{
			USART_SendData(USARTx, *Data++);
		}
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}

/*********************************************************************************
* Function: Debug_Printf
* Description: printf debug information
* Input: -Data
* Input: -args
* Output: none
* Return: none
* Call: external 
*/
void Debug_Printf(char *Data, ...)
{
	va_list ap;
	if (Debug == SET)
	{
		va_start(ap, Data);
		printf(Data, ap);
	}
}
///////////////
