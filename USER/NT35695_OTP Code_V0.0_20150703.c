#include "include.h"

uint8_t OTP_TIMES = 0;			// OTP次数
uint16_t vcom_best = 0x10;		// 最佳VCOM		
uint8_t MAIN_PORT = PORT1;	  	// 主MIPI port(作为回读的数据口)	 WQXGA:PORT0 	HF/FHD:PORT1

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
	wtBuf[0]=0x10;	
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	wtBuf[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
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
	wtBuf[0]=0x10;	
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	wtBuf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}
/*********************************************************************************
 * 函数名：Comd1_set
 * 描述  ：Page select
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Comd1_set(void)
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x10;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
}
/*********************************************************************************
 * 函数名：Com2page0_set
* 描述  ：Page select:commmand2 page0
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Com2page0_set(void)
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x20;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
}


void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[1];

	Com2page0_set();
	wtBuf[0] = vcom;	
	MIPI_DCS_Write((PORT0 | PORT1), 0x11, 1, wtBuf);
  Comd1_set();	
  Delay_ms(10);	
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
	unsigned char wtBuf[1];	
	unsigned char rdBuf[1];
  wtBuf[0]=0x10;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	wtBuf[0]=0x20;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	MIPI_GEN_Read(MAIN_PORT, 0x11, 1, rdBuf);

	wtBuf[0]=0x10;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	

	return rdBuf[0];
}
/*********************************************************************************
 * 函数名：NVM_Flag
 * 描述  ：回读NVM_Flag
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：NVM_Flag
 * 调用  ：外部调用
 */
unsigned NVM_Flag(void)
{
 	ErrorStatus NVM_Status;	
	unsigned char wtBuf[1];
	unsigned char rdBuf[1];
	wtBuf[0]=0x10;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	wtBuf[0]=0x20;
  MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
  Delay_ms(5);	
	MIPI_GEN_Read(MAIN_PORT, 0x47, 1, rdBuf);
	printf("\r\nThe data read from NVM_Flag47 is: 0x%02X\r\n",rdBuf[0]);
  if(rdBuf[0]&0x01)
	{
		NVM_Status=SUCCESS;			
	}
	
  else
	{
		NVM_Status=ERROR; 	
	}
	
	
	return NVM_Status;
}

/*********************************************************************************
 * 函数名：CRCCheck
 * 描述  ：回读NVM_Flag
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：NVM_Flag
 * 调用  ：外部调用
 */
unsigned CRCCheck(void)
{
 	ErrorStatus CRC_Status;	
	uint8_t rdBuf[6];
	uint8_t rdBuf1[1];
  Comd1_set();
	Com2page0_set();
  Delay_ms(5);	
	MIPI_GEN_Read(MAIN_PORT,0x4A,0x01,rdBuf1);//4A,
	rdBuf[0]=rdBuf1[0];
	MIPI_GEN_Read(MAIN_PORT,0x4B,0x01,rdBuf1);//4B,
	rdBuf[1]=rdBuf1[0];
	MIPI_GEN_Read(MAIN_PORT,0x4C,0x01,rdBuf1);//4C,
	rdBuf[2]=rdBuf1[0];
	MIPI_GEN_Read(MAIN_PORT,0x4D,0x01,rdBuf1);//4D
	rdBuf[3]=rdBuf1[0];
	printf("\r\nThe data read from Reg4A-4D is: 0x%02X",rdBuf[0]);
	printf(",0x%02X",rdBuf[1]);
	printf(",0x%02X",rdBuf[2]);
	printf(",0x%02X",rdBuf[3]);
	
	MIPI_GEN_Read(MAIN_PORT,0x11,0x01,rdBuf1);//vcom
	printf("\r\nThe data read from Regvcom is: 0x%02X\r\n",rdBuf1[0]);
	rdBuf[4]=rdBuf1[0];
  Comd1_set();
	MIPI_GEN_Read(MAIN_PORT,0xDA,0x01,rdBuf1);//ID
	printf("\r\nThe data read from RegID is: 0x%02X\r\n",rdBuf1[0]);
	rdBuf[5]=rdBuf1[0];
	
	if(rdBuf[0]==0xFB&&rdBuf[1]==0xD8&&rdBuf[2]==0x50&&rdBuf[3]==0xCB&&rdBuf[4]==(uint8_t)vcom_best&&rdBuf[5]==0x30)
		CRC_Status=SUCCESS;
  else
		CRC_Status=ERROR;
	
  return CRC_Status;
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
	unsigned char wtBuf[1];
	unsigned char rdBuf[1];
	unsigned char OTPTime[1];
	wtBuf[0]=0x10;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	wtBuf[0]=0x20;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	MIPI_DCS_Read(MAIN_PORT, 0x43, 1, rdBuf);
  switch(rdBuf[0] & 0x0F)
   {
       case 0x00:
                          OTPTime[0]=0x00;
						  break;
       case 0x01:
                          OTPTime[0]=0x01;
						  break;
       case 0x03:
                          OTPTime[0]=0x02;
						  break;
       case 0x07:
                          OTPTime[0]=0x03;
						  break;
			 case 0x0F:		  
													OTPTime[0]=0x04;
						  break;  
	   default: 

                          OTPTime[0]=0x00; 
   } 

	wtBuf[0]=0x10;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	

	return OTPTime[0];
}

/*********************************************************************************
 * 函数名：OTPSequence
 * 描述  ：OTP流程
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：OTP执行结果
 * 调用  ：外部调用
 *	------------------------------------------------------------------------------
 * 	0xE0寄存器
 *		-第四个参数bit6：NVMFTT指示 OTP操作 是否完成(0->1: start squence	1->0: squence finish)
 *		-第五个参数bit0：NVMVFFLGER指示 erase 是否成功
 *  	-第五个参数bit1：NVMVFFLGWR指示 write 是否成功
 *	------------------------------------------------------------------------------
 */
 ErrorStatus OTPSequence(void)
 {
 	ErrorStatus otpResult;
	uint16_t ReadVcom;
 	uint8_t buf[1];
  Comd1_set();   //CMD1
	LCD_DisplayOff();
	Delay_ms(20);	 
	LCD_VideoMode_OFF();
	Delay_ms(20);	 
	LCD_LPMode();
	Delay_ms(200);

 //enable CRC Check
	Com2page0_set();   //CMD2_P0
	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x37, 1, buf);
	Delay_ms(1);

	Com2page0_set();   //CMD2_P0
		if(OTP_TIMES==0x00)//00
	{			//判断OTP次数，若未OTP
		buf[0]=0x03;
		MIPI_GEN_Write((PORT0 | PORT1), 0x3F, 1, buf);// 3F 03/3F==选择ab区,03-AB ZONE
	}
	else
	{
		buf[0]=0x01;
		MIPI_GEN_Write((PORT0 | PORT1), 0x3F, 1,buf);  // 01-A ZONE
	}	
	buf[0]=0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x38, 1, buf);		//38 01
	Delay_us(100);
	buf[0]=0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0x39, 1,buf);		//39 00
	Delay_us(200);
	buf[0]=0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x3D, 1, buf);		//3D 01
	buf[0]=0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0x40, 1, buf);		//40 55
	buf[0]=0xAA;
	MIPI_GEN_Write((PORT0 | PORT1), 0x41, 1, buf);		//41 AA
	buf[0]=0x66;
	MIPI_GEN_Write((PORT0 | PORT1), 0x42, 1, buf);		//42 66
	Delay_ms(800);
	//enable CRC Check
  Com2page0_set();   //CMD2_P0
	buf[0]=0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x37, 1, buf);		

	if( NVM_Flag())  //if otp success
	{
		if(CRCCheck())			//the OTP value is right
		{

			buf[0]=0x00;
			MIPI_GEN_Write((PORT0 | PORT1), 0x3D, 1, buf);		//3D 00
			Delay_us(600);	

			buf[0]=0x10;
			MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);	//COMD1
      buf[0]=0x10;
			MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);		//FF 10
			Delay_us(150);

			buf[0]=0x20;
			MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);	//COMD2_p0

			buf[0]=0x20;
			MIPI_GEN_Write((PORT0 | PORT1), 0x43, 1, buf);		//43 20

			buf[0]=0x10;
			MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);		//COMD1

			buf[0]=0x01;
			MIPI_GEN_Write((PORT0 | PORT1), 0x35, 1, buf);		//35 01

			buf[0]=0x06;
			MIPI_GEN_Write((PORT0 | PORT1), 0xD3, 1, buf);	//D3 06

			buf[0]=0x0E;
			MIPI_GEN_Write((PORT0 | PORT1), 0xD4, 1, buf);	//D4 0E  
		  LCD_PWM(0XFF);

			OTP_TIMES=OTPTimes_Read();			//Read OTP times
		  ReadVcom = VCOM_Read();				  //Read Vcom after OTP
		  if(ReadVcom!=vcom_best )
		 {
			printf("read vcom(0x%02x) isn's the best_vcom(0x%02x), OTP fail!!\r\n",ReadVcom,vcom_best);
			FPGA_DisPattern(85, 0 , 0 ,0 );
			otpResult = ERROR;
		 }

		 else
		 {
			printf("Congratulations,OTP successed!\r\n");
			otpResult = SUCCESS;
		 }
		  printf("\r\nMIPI_Reset...\r\n");
	    MIPI_Reset();
	    printf("\r\nSSD_Init...\r\n");	
			/* SSD2828 initial */		
	    IC_Init(SSDInitCode);
		  DriverIC_Reset();	
      IC_Init(ET2_InitCode);			 
	    /* display on */
    	printf("\r\nLCD_SleepOut...\r\n");
    	LCD_SleepOut();
	    printf("\r\nLCD_VideoMode_ON...\r\n");
	    LCD_VideoMode_ON();
	    printf("\r\nLCD_HSMode...\r\n");
	    LCD_HSMode();
	 }
	 else 
	 {
		 printf("Warning!!\r\n");
		 printf("The OTP value is wrong!\r\n");
		 otpResult = ERROR;
	 }

 }
else 
 {
		 printf("The OTP Fail!\r\n");
		 otpResult = ERROR;
 }	
	return 	otpResult;

}





