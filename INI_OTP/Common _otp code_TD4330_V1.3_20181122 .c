#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP次数
uint16_t vcom_best = 0x0228;		// 最佳VCOM		
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1  // For TED use  PORT0
uint16_t vcom_default; 
unsigned char rdBuf[2];

unsigned short GAMMA_InitCode[]=
{
  //=======Gamma Code  Begin======//

 ////C7 		Gamma Setting (Common)		TANGYANFANG
0X1BC,0X24D,0X200,
0X1BF,0X2C7,0x200,  //1  VGMP0
            0x200,  //2		  	
            0x200,  //3  
            0x2DF,  //4  
            0x201,  //5  
            0x2CF,  //6  
            0x202,  //7  
            0x262,  //8  
            0x201,  //9  
            0x2E7,  //10  
            0x202,  //11  
            0x273,  //12  
            0x202,  //13  
            0x2DD,  //14  
            0x202,  //15  
            0x228,  //16  
            0x202,  //17  
            0x244,  //18  
            0x202,  //19  
            0x262,  //20  
            0x202,  //21  
            0x2B2,  //22  
            0x202,  //23  
            0x208,  //24  
            0x202,  //25  
            0x24B,  //26  
            0x202,  //27  
            0x2AF,  //28  
            0x202,  //29  
            0x21F,  //30  
            0x202,  //31  
            0x2BF,  //32  
            0x203,  //33  
            0x238,  //34  
            0x203,  //35  
            0x278,  //36  
            0x203,  //37  
            0x2A0,  //38  
            0x200,  //39  
            0x200,  //40  
            0x200,  //41  
            0x2DF,  //42  
            0x201,  //43  
            0x2CF,  //44  
            0x202,  //45  
            0x262,  //46  
            0x201,  //47  
            0x2E7,  //48  
            0x202,  //49  
            0x273,  //50  
            0x202,  //51  
            0x2DD,  //52  
            0x202,  //53  
            0x228,  //54  
            0x202,  //55  
            0x244,  //56  
            0x202,  //57  
            0x262,  //58  
            0x202,  //59  
            0x2B2,  //60  
            0x202,  //61  
            0x208,  //62  
            0x202,  //63  
            0x24B,  //64  
            0x202,  //65  
            0x2AF,  //66  
            0x202,  //67  
            0x21F,  //68  
            0x202,  //69  
            0x2BF,  //70  
            0x203,  //71  
            0x238,  //72  
            0x203,  //73  
            0x278,  //74  
            0x203,  //75  
            0x2A0,  //76  

 //=======Gamma Code   End======//

    0xF00   //end of initialization
};

/*********************************************************************************
 * 函数名：OSC_trim_Check
 * 描述  ：回读CEE0值，判断OSC机制是否开启 
 * 输入  : 无
 * 输出  : 无
 * 返回  ：CEE0值
 * 调用  ：外部调用
 */
void OSC_trim_Check(void)
{
	
}


/*********************************************************************************
 * 函数名：ScanForward
 * 描述  ：正扫
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */											
void ScanForward(void)
{
	unsigned char wtBuf[1];
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}

/*********************************************************************************
 * 函数名：ScanBackward
 * 描述  ：反扫
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void ScanBackward(void)
{
	unsigned char wtBuf[1];
	
	wtBuf[0] = 0x80;		//Bit D7 Page Address Order 1:Bottom to to
	MIPI_DCS_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}

/*********************************************************************************
 * 函数名：Vcom_Set
 * 描述  ：设置VCOM
 * 输入  ：-VCOM值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
//TD4300
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[9];

	wtBuf[0] = 0x0F; //0x03 For TD4300 vcom wirte protect OFF
	wtBuf[1] = 0x07;
	wtBuf[2] = 0x00;
	wtBuf[3] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE5, 4, wtBuf);	
	wtBuf[0] = vcom>>8;
	wtBuf[1] = vcom;
	wtBuf[2] = vcom>>8;
	wtBuf[3] = vcom;
	wtBuf[4] = 0x02;
	wtBuf[5] = 0xDC; 
	wtBuf[6] = 0x02;
	wtBuf[7] = 0xDC;
	MIPI_GEN_Write((PORT0 | PORT1), 0xD5, 8, wtBuf);
}

/*********************************************************************************
 * 函数名：Chroma_Set
 * 描述  ：设置Chroma
 * 输入  ：-u,v值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Chroma_Set(unsigned char u, unsigned char v)
{
	unsigned char wtBuf[6];

	wtBuf[0] = 0x01;
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = u;			  
	wtBuf[4] = v;
	wtBuf[5] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE2, 6, wtBuf);
}

/*********************************************************************************
 * 函数名：Date_Set
 * 描述  ：设置Date
 * 输入  ：-日期 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Date_Set(unsigned char year, unsigned char month, unsigned char day)
{
	unsigned char wtBuf[10];

	wtBuf[0] = 0x01;
	wtBuf[1] = 0x00;
	wtBuf[2] = 0x00;
	wtBuf[3] = 0x20 | (day & 0x1F); 
	wtBuf[4] = 0x59;
	wtBuf[5] = 0x00;
	wtBuf[6] = (year << 4) + (month & 0x0F);			  
	wtBuf[7] = 0x07;
	wtBuf[8] = 0x14;
	wtBuf[9] = 0xFF;
	MIPI_GEN_Write((PORT0 | PORT1), 0xE1, 9, wtBuf);
}

/*********************************************************************************
 * 函数名：VCOM_Read
 * 描述  ：回读VCOM
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：VCOM值
 * 调用  ：外部调用
 */
unsigned short VCOM_Read(void)
{
	unsigned char rdBuf[8];

	MIPI_GEN_Read(MAIN_PORT, 0xD5, 8, rdBuf);

	return ((rdBuf[0]<<8)+rdBuf[1]);
}

/*********************************************************************************
 * 函数名：OTPTimes_Read
 * 描述  ：回读OTP次数
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP次数
 * 调用  ：外部调用
 */
unsigned char OTPTimes_Read(void)
{
	unsigned char rdBuf[6];

	MIPI_DCS_Read(MAIN_PORT, 0xFB, 3, rdBuf);

	return (0x0F&rdBuf[1]);
}
/*********************************************************************************
 * ???:ID_Chroma_OTP
 * ??  :OTP ID, chroma...
 * ??  :? 
 * ??  :?
 * ??  :?
 * ??  :????
 */
void ID_Chroma_OTP(void)
{
		unsigned char buf[6];

	buf[0] = 0x0A;
	MIPI_DCS_Write((PORT0 | PORT1), 0xDA, 1, buf); 
	
		//If need to OTP ID, please uncomment the following code
//	unsigned char buf[6];

//	buf[0] = 88;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xDA, 1, buf); 
//	buf[0] = 99;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xDB, 1, buf); 
//	buf[0] = 77;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xDC, 1, buf); 
//	 
//	buf[0] = 0x11;
//  buf[1] = 0x22;
//	buf[2] = 0x33;
//	buf[3] = 0x44;
//	buf[4] = 0x55;
//	buf[5] = 0x66;
//	MIPI_DCS_Write((PORT0 | PORT1), 0xA1, 6, buf); //The command returns information from the display module 
//	 
}
/*********************************************************************************
 * 函数名：OTPSequence
 * 描述  ：OTP流程
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP执行结果
 * 调用  ：外部调用
 */
 ErrorStatus OTPSequence(void)
 {
		ErrorStatus otpResult;
		uint8_t buf[7],i;
		unsigned char gamma_flag = 0,vcom_flag = 0,timing_flag = 0;	 
		printf("\r\n==============OTP Sequence Start================\r\n");	
 
	//Enter sleep mode	 
		LCD_SleepIn();
		LCD_VideoMode_OFF();
		LCD_LPMode();
	 
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 
		buf[0] = 0x1D;
		MIPI_GEN_Write((PORT0 | PORT1), 0xF4, 1, buf); //GCS Deletes  tuned  gamma from  Flash	
		Delay_ms(300);
	 
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 
//		Delay_ms(300);
		MIPI_GEN_Read(MAIN_PORT, 0xFB, 3, buf); // Per Panel tuning
	i = 3;
	while(i--)
		printf("buf[%d]= %x\r\n",i,buf[i]);
	if((buf[2]&0xF0) == 0x30 ) 			  // programming succeeded
	{
		printf("\r\n========Gamma programming succeeded==========\r\n");
		gamma_flag = 1;
	}
	 else if((buf[2]&0xF0) == 0x00 ) //programming failed
	{
		printf("\r\n=Gamma programming failed due to flash related issues=\r\n");
	}
	 else if(buf[2]&0x80)						 //programming failed
	 {
		printf("programming executes over the maximum number of writable registers,OTP quite!\r\n");
	 }
          else
        { 
                printf("\r\n=Dispaly programming failed due to other issues=\r\n");

         }
	 
//write ID --Customer information write 
		buf[0] = 0x04;
	  MIPI_DCS_Write((PORT0 | PORT1), 0xB0, 1, buf); 	
	  buf[0] = 0x03;
	  buf[1] = 0x3F;
    MIPI_DCS_Write((PORT0 | PORT1), 0xE5, 2, buf); 	
		ID_Chroma_OTP();

//VCOM OTP start===========
//buf[0] = 0x1B;//  flash mode
//buf[1] = 0x00;//  flash mode
//MIPI_GEN_Write((PORT0 | PORT1), 0xF5, 2, buf); //  flash mode
buf[0] = 0x1B;// no flash mode
buf[1] = 0x4C;// no flash mode
MIPI_GEN_Write((PORT0 | PORT1), 0xF5, 2, buf); // no flash mode


Delay_ms(300);
		MIPI_GEN_Read(MAIN_PORT, 0xFB, 3, buf); // ANSIC configuration
	i = 3;
	while(i--)
		printf("buf[%d]= %x\r\n",i,buf[i]);
	if((buf[0]&0xF0) == 0x30 ) 			  // programming succeeded
	{
		printf("\r\n========ANSIC configuration programming succeeded==========\r\n");
		vcom_flag = 1;
	}
	 else if((buf[0]&0xF0) == 0x00 ) //programming failed
	{
		printf("\r\n=ANSIC configuration programming failed due to flash related issues=\r\n");
	}
	 else if(buf[0]&0x80)						 //programming failed
	 {
		printf("ANSIC configuration programming executes over the maximum number of writable registers,OTP quite!\r\n");
	 }
           else
        { 
                printf("\r\n=Dispaly programming failed due to other issues=\r\n");

         }
//VCOM OTP finish===========
	
/**********************************************************************/
//otp timing
		SSD_B7 = 0x0550 ;
		WriteSSDReg(MAIN_PORT, 0xB7, SSD_B7);
		
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 
		Delay_ms(10);

		buf[0] = 0x1C;
		MIPI_GEN_Write((PORT0 | PORT1), 0xF4, 1, buf);//OTP run!
//		Delay_ms(500);	
	  Delay_ms(300);																

		MIPI_GEN_Read(MAIN_PORT, 0xFB, 3, buf);
		i = 3;
	while(i--)
		printf("buf[%d]= %x\r\n",i,buf[i]);
	if((buf[1]&0xF0) == 0x30 ) 			 
	{
		printf("\r\n========Dispaly programming succeeded==========\r\n");
		timing_flag = 1;
	}
	 else if((buf[1]&0xF0) == 0x00 ) 
	{
		printf("\r\n=Dispaly programming failed due to flash related issues=\r\n");
	}
	 else if(buf[1]&0x80)						 
	 {
		printf("Dispaly profile programming executes over the maximum number of writable registers,OTP quite!\r\n");
	 }
	
        else
        { 
                printf("\r\n=Dispaly programming failed due to other issues=\r\n");

         }
	//--------OTP check-----------------//

	if(gamma_flag&&timing_flag&&vcom_flag)		//check whether OTP complete
	 {
		printf("OTP Success!\r\n");
		otpResult = SUCCESS;
	 }
	 else 
	 {
		printf("\r\nOTP_TIMES = %d\r\n",OTP_TIMES);
		printf("buf[0] = %d\r\nbuf[1] = %d\r\nbuf[2] = %d\r\n",buf[0],buf[1],buf[2]);
		printf("OTP NG!\r\n");
		otpResult = ERROR;
	 }
	//--------OTP finish-----------------//
	 


	DriverIC_Reset();
	MIPI_Reset();
	IC_Init(SSDInitCode);
	 if(otpResult)
	IC_Init(ET2_InitCode);
	 else
	IC_Init(ET1_InitCode);
	//LCD_PWM(0xFF);
	//LCD_SleepOut();
	//Delay_ms(50); 
	//LCD_VideoMode_ON();
	//LCD_HSMode();	
	 LCD_LitSquence();
	return 	otpResult;
	 
}

/*********************************************************************************
* Function: DriverIC_Reset
* Description: DDIC reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void DriverIC_Reset(void)
 {
	 printf("\r\nDriver IC reset...\r\n");
	 GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(5);
	 GPIO_ResetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(50);
	 GPIO_SetBits(LCD_nRST_GPIO_PORT, LCD_nRST_PIN);
	 Delay_ms(150);
 }
 /*********************************************************************************
* Function: DriverIC_Reset
* Description: DDIC reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
 void DriverIC_RST_TP(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	 GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //CONNECTOR2 / TP_RST
 	 GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);	
	 GPIO_ResetBits(TEST24_GPIO_PORT, TEST24_PIN);//TP reset pin
	 Delay_ms(10);//typical 10mS 
	 GPIO_SetBits(TEST24_GPIO_PORT, TEST24_PIN);  //TP reset pin
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	 GPIO_InitStructure.GPIO_Pin = TEST24_PIN; //CONNECTOR2 / TP_RST
 	 GPIO_Init(TEST24_GPIO_PORT, &GPIO_InitStructure);	
	 Delay_ms(20);
 } 
/*********************************************************************************
* Function: LCD_SoftReset
* Description: DDIC soft reset
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SoftReset(void)
{
	printf("\r\nDriver IC solfware reset...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0001);
	Delay_ms(150);	 //delay more than 120ms
}
 
/*********************************************************************************
* Function: LCD_SleepIn
* Description: DDIC sleep in and display off
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SleepIn(void)
{	
	printf("\r\nDriver IC display off then sleep in...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0028);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0010);
	Delay_ms(200);	  //delay more than 120ms
}

/*********************************************************************************
* Function: LCD_SleepOut
* Description: DDIC sleep out and display on
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_SleepOut(void)
{
	printf("\r\nDriver IC display on then sleep out...\r\n");
	SSD_B7 |= SSD_CFGR_DCS;
	SSD_B7 &= ~SSD_CFGR_REN;
	WriteSSDReg((PORT0 | PORT1), SSD_CFGR, SSD_B7);
	printf("SSD_B7 = 0x%04x\r\n", SSD_B7);

	WriteSSDReg((PORT0 | PORT1), 0xBC, 0x0001);
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0011);
	Delay_ms(200);	  //delay more than 120ms
	WriteSSDReg((PORT0 | PORT1), 0xBF, 0x0029);	
}

/*********************************************************************************
* Function: LCD_PWM
* Description: BLU PWM duty setting
* Input: -data, PWM duty(0X00~0XFF)
* Output: none
* Return: none
* Call: external
*/
void LCD_PWM(uint16_t data)
{
	uint8_t wtBuf[2];

	//8bit pwm
	wtBuf[0] = data & 0x00FF;
	PWM_DUTY = (wtBuf[0]*PWM_T/0xFF);
	printf("\r\nPWM_DUTY=%d\r\n", PWM_DUTY);	
	MIPI_DCS_Write((PORT0 | PORT1), 0x51, 1, wtBuf);

	wtBuf[0] = 0x24; //0x2C
	MIPI_DCS_Write((PORT0 | PORT1), 0x53, 1, wtBuf);
	wtBuf[0] = 0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0x55, 1, wtBuf);
	Delay_ms(10);
}

void LEDA_DIM(void) //1mA
{
	LCD_PWM(0x000D);
}

void LEDA_NORM(void) 
{
	LCD_PWM(0x0FFF);
}

/*********************************************************************************
* Function: LCD_LitSquence
* Description: 
* Input: none
* Output: none
* Return: none
* Call: external
*/
void LCD_LitSquence(void)
{
	//LCD_PWM(0x0FFF);
        LEDA_NORM();
	LCD_SleepOut();
	LCD_HSMode();	
#ifdef CMD_MODE
	LCD_VideoMode_OFF();
#else
	LCD_VideoMode_ON();
#endif
}

/*********************************************************************************
* Function: LCMPower_ON
* Description: power on timing, low active
* Input: none
* Output: none
* Return: none
* Call: external
*/
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

/*********************************************************************************
* Function: LCMPower_OFF
* Description: power off timing, low active
* Input: none
* Output: none
* Return: none
* Call: external
*/
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
 * ???:PreGamma_Set
 * ??  :download pre-gamma 
 * ??  :? 
 * ??  :?
 * ??  : ?
 * ??  :????
 */
void PreGamma_Set(void)
{
	//add your code according to your project

}
/*********************************************************************************
 * 函数名：OTP_HDL_MODE
 * 描述  ：OTP HDL流程  Interface mode 转换--I2C mode（F5H set 0x1B00）  转 SPI mode（F5H set 0x1B4C）
 * 输入  ：无 
 * 输出  ：none
 * 返回  ：OTP执行结果
 * 调用  ：外部调用
 *	------------------------------------------------------------------------------
*/
ErrorStatus OTP_HDL_MODE()
{
		ErrorStatus otpResult = ERROR;
		uint8_t buf[7];
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 	
		Delay_ms(10);
		MIPI_GEN_Read(MAIN_PORT, 0xFB, 1, buf);
		if((buf[0]&0x0F)!= 0x00){
			otpResult = SUCCESS;
			printf("NEED NOT OTP HDL!\r\n");
			return otpResult;
		}
	  
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 	
		Delay_ms(10);
		MIPI_GEN_Read(MAIN_PORT, 0xD5, 2, rdBuf);
		vcom_default=((rdBuf[0]<<8)+rdBuf[1]);
		if((vcom_default== 0x02DC)||(vcom_default== 0x0000)){
			otpResult = ERROR;
			printf("APO NEED NOT OTP HDL!\r\n");
			return otpResult;
		}
		
		printf("\r\n/*==============OTP HDL Sequence Begin================*/\r\n");	
 
	//Enter sleep mode	 
		LCD_SleepIn();
		LCD_VideoMode_OFF();
		LCD_LPMode();
	 
		buf[0] = 0x04;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf); 

		buf[0] = 0x1B;
		buf[1] = 0x4C;	
		MIPI_GEN_Write((PORT0 | PORT1), 0xF5, 2, buf); //write boot config = 0x4C into ASIC configuration
		Delay_ms(300);
	 
		MIPI_GEN_Read(MAIN_PORT, 0xFB, 1, buf);
		if((buf[0]&0x30)== 0x30){
			otpResult = SUCCESS;
			printf("OTP HDL mode success!\r\n");
		}
		else{
			printf("OTP HDL mode Fail!(ERROR CODE %2X)\r\n",buf[0]);			
		}
	
		buf[0] = 0x03;
		MIPI_GEN_Write((PORT0 | PORT1), 0xB0, 1, buf);// 
		printf("\r\n/*==============OTP HDL Sequence End================*/\r\n");		
		//reset &light up lcd
		DriverIC_Reset();
		if (TEST_MODE == TEST_MODE_RA)
		{
				IC_Init(RA_InitCode);
		}
		else
		{
				IC_Init(ET1_InitCode);		
				LCD_LitSquence();		
		}		
		//reset &light up lcd		
		return otpResult;
}

