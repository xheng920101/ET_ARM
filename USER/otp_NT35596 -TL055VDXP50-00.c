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
	wtBuf[0]=0x00;	
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
	wtBuf[0]=0x00;	
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	wtBuf[0] = 0x80;
	MIPI_GEN_Write((PORT0 | PORT1), 0x36, 1, wtBuf);
}
/*********************************************************************************
 * 函数名：Pagesel0
 * 描述  ：Page select
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Pagesel0(void)
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
}
/*********************************************************************************
 * 函数名：Pagesel1
 * 描述  ：Page select
 * 输入  ：无 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void Pagesel1(void)
{
  unsigned char wtBuf[1];
	wtBuf[0]=0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
}

/*********************************************************************************
 * 函数名：Vcom_Set
 * 描述  ：设置VCOM
 * 输入  ：-VCOM值 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void VCOM_Set(unsigned short vcom)
{
	unsigned char wtBuf[1];
	wtBuf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	wtBuf[0] = vcom;	
	MIPI_GEN_Write((PORT0 | PORT1), 0x11, 1, wtBuf);
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

///*********************************************************************************
// * 函数名：Connect_Check
// * 描述  ：上电防呆
// * 输入  ：无 
// * 输出  ：无
// * 返回  ：IC连接状态
// * 调用  ：外部调用
// */
// void Connect_Check(void)
//{
//	unsigned char wtBuf[1];	
//	unsigned char rdBuf[1];
//	wtBuf[0]=0x00;	
//	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
//	Delay_ms(5);
//	MIPI_GEN_Read(MAIN_PORT, 0x0A, 1, rdBuf);
//	if (rdBuf[0] != 0x08)
//	{
//	   LED_ON(RED);
//	   LCMPower_OFF();
//	}
//}

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
	wtBuf[0]=0x01;	
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);
	MIPI_GEN_Read(MAIN_PORT, 0x11, 1, rdBuf);
	
	wtBuf[0]=0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);

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
	wtBuf[0]=0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	MIPI_GEN_Read(MAIN_PORT, 0x3F, 1, rdBuf);

  if(rdBuf[0]&0x01)
	{
		NVM_Status=SUCCESS;			
	}
	
  else
	{
		NVM_Status=ERROR;// 	
	}
	
	
	return NVM_Status;
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
	wtBuf[0]=0x01;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
	Delay_ms(5);	
	MIPI_DCS_Read(MAIN_PORT, 0x3F, 1, rdBuf);
  switch(rdBuf[0])
   {
       case 0x00:
                          OTPTime[0]=0x00;
						  break;
       case 0x10:
                          OTPTime[0]=0x01;
						  break;
       case 0x30:
                          OTPTime[0]=0x02;
						  break;
       case 0x70:
                          OTPTime[0]=0x03;
						  break;
	   case 0xF0:		  
													OTPTime[0]=0x04;
						  break;  
	   default: 

                          OTPTime[0]=0x00; 
   } 
	
  wtBuf[0]=0x00;
	MIPI_DCS_Write((PORT0 | PORT1), 0xFF, 1, wtBuf);
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
 	uint8_t buf[1];
	uint8_t NVM_buf[2]; 
	 
	NVM_buf[0] =  OTPTimes_Read(); 


	LCD_DisplayOff();
	Delay_ms(20);	 
	LCD_VideoMode_OFF();
	Delay_ms(20);	 
	LCD_LPMode();
	Delay_ms(20);
	 
	buf[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);	 
	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x00, 1, buf);
	 

	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);	 
	buf[0] = 0x01;
	MIPI_GEN_Write((PORT0 | PORT1), 0x38, 1, buf);
	Delay_ms(130);
	 
	buf[0] = 0x00;
	MIPI_GEN_Write((PORT0 | PORT1), 0x39, 1, buf);
	Delay_ms(210);

	buf[0] = 0x01; 
	MIPI_GEN_Write((PORT0 | PORT1), 0x3D, 1, buf);
	buf[0] = 0x55;
	MIPI_GEN_Write((PORT0 | PORT1), 0x40, 1, buf);	 
	buf[0] = 0xAA;
	MIPI_GEN_Write((PORT0 | PORT1), 0x41, 1, buf);
	buf[0] = 0x66;
	MIPI_GEN_Write((PORT0 | PORT1), 0x42, 1, buf);


	printf("Start OTP\r\n");
	Delay_ms(900);
	
  if(NVM_Flag())
	{
	buf[0] = 0x01; 
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
	Delay_ms(10);	
	buf[0] = 0x00; 
	MIPI_GEN_Write((PORT0 | PORT1), 0x3D, 1, buf);
	Delay_ms(200);			
  MIPI_Reset();
	Delay_ms(40);		
	DriverIC_Reset();	
	Delay_ms(20);
	IC_Init(SSDInitCode);	
	Delay_ms(20);				
	buf[0] = 0x01; 
	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
	Delay_ms(10);	
	buf[0] = 0x20; 
	MIPI_GEN_Write((PORT0 | PORT1), 0x43, 1, buf);		
	Delay_ms(200);		

  IC_Init(ET2_InitCode);	
	LCD_SleepOut();
	LCD_VideoMode_ON();
	LCD_HSMode();
	POWER_LED_Reset();
	LCD_PWM(0xFF);
	
	Delay_ms(100);		
	printf("OTP OK!\r\n");
	NVM_buf[1] =  OTPTimes_Read();
	Delay_ms(100);		


	printf("\r\nF1_buf[0] = 0x%04x\r\n", NVM_buf[0]);
	printf("\r\nF1_buf[1] = 0x%04x\r\n", NVM_buf[1]);
	OTP_TIMES = NVM_buf[1];	

    if(NVM_buf[1]>NVM_buf[0])
		{
			otpResult=SUCCESS;			
		}
		
    else
		{
			otpResult=ERROR;// 	
		}		
				
	}
	
  else
	{
		otpResult=ERROR;;// 	
	} 
	
	return 	otpResult;
}
/*********************************************************************************
 * 函数名：VcomCheck
 * 描述  ：check vcom
 * 输入  ：-
 *         -
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
 void VcomCheck(void)
 {
     if(vcom_best != VCOM_Read())
   {
 	    LCMPower_OFF();
		  LED_ON(RED);
		  LED_OFF(BLUE);
   }
 }

/*********************************************************************************
 * 函数名：ICRegCheck
 * 描述  ：check IC reg
 * 输入  ：-reg   MIPI回读缓冲区地址
 *         -nPara IC寄存器参数个数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
 void ICRegCheck(uint8_t nPara,uint8_t *ICReg,uint8_t *para)
 {
  ErrorStatus checkflag;
	uint8_t rdCnt ;
	unsigned char rdBuf[1];

    for(rdCnt = 0;rdCnt < nPara;rdCnt++)
	{
		MIPI_GEN_Read(MAIN_PORT, *(ICReg + rdCnt), 0x01, rdBuf);	
	  if(rdBuf[0] != *(para + rdCnt))
	  {
	   	checkflag=ERROR;
	    printf("\r\n IC reg check wrong 0x%02X : ", *(ICReg + rdCnt));	
	    printf("\r\n");			
		break;
	  }
	  else
	  {
	  	checkflag=SUCCESS; 
			
	  }
	}

   if(checkflag==ERROR)
   {
				LCMPower_OFF();
		 		LED_ON(RED);
		    LED_OFF(BLUE);
   }

 }
/*********************************************************************************
 * 函数名：ICCheckBO
 * 描述  ：check IC before OTP
 * 输入  ：-
 *         -
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
  */
  void ICCheckBO(void)
{
	 	uint8_t buf[1];
	  
    unsigned char RegBuf[70] =
    {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09, //1 
    0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,	0x10,	0x11,	0x12,	0x13, //2 
    0x14,	0x15,	0x16,	0x17,	0x18,	0x19,	0x1A,	0x1B,	0x1C,	0x1D, //3
    0x1E,	0x1F,	0x20,	0x21,	0x22,	0x23,	0x24,	0x25,	0x29,	0x2A, //4
    0x2B,	0x2F,	0x30,	0x31,	0x32,	0x33,	0x34,	0x35,	0x36,	0x37, //5
    0x38,	0x5D,	0x61,	0x65,	0x69,	0x6C,	0x7A,	0x7B,	0x7C,	0x7D, //6
    0x7E,	0x7F,	0x81,	0x82,	0x8A,	0x93,	0x94,	0x9B,	0xA4,	0xE7,	//7
		};			                                                    	
    unsigned char DataBuf[70]=
		{
		0x00,	0x00,	0x03,	0x04,	0x00,	0x11,	0x0C,	0x0B,	0x01,	0x00,	//1
    0x18,	0x16,	0x14,	0x17,	0x15,	0x13,	0x00,	0x00,	0x03,	0x04, //2
    0x00,	0x11,	0x0C,	0x0B,	0x01,	0x00,	0x18,	0x16,	0x14,	0x17, //3
    0x15,	0x13,	0x00,	0x02,	0x09,	0x67,	0x06,	0x1D,	0x58,	0x11, //4
    0x04,	0x02,	0x01,	0x49,	0x23,	0x01,	0x03,	0x6B,	0x00,	0x1D, //5
    0x00,	0x23,	0x15,	0x00,	0x04,	0x51,	0x00,	0x80,	0xD8,	0x10, //6
    0x06,	0x1B,	0x06,	0x02,	0x33,	0x06,	0x06,	0x0F,	0x0F,	0x80,	//7
		};
			

   	buf[0] = 0x05; 
   	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
   	Delay_ms(20);	
		
    ICRegCheck(0x46,RegBuf,DataBuf);
		
   	Delay_ms(20);	
   	buf[0] = 0x00; 
   	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
   	Delay_ms(20);			
}

/*********************************************************************************
 * 函数名：ICCheckAO
 * 描述  ：check IC after OTP 
 * 输入  ：-
 *         -
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
  */
  void ICCheckAO(void)
{
	 	uint8_t buf[1];
	  
    unsigned char RegBuf[70] =
    {
		0x09, //1 
    0x13, //2 
    0x1D, //3
    0x2A, //4
    0x37, //5
    0x7D, //6
    0xE7,	//7
		};			                                                    	
    unsigned char DataBuf[70]=
		{
		0x00,	//1
    0x04, //2
    0x17, //3
    0x11, //4
    0x1D, //5
    0x10, //6
    0x80,	//7
		};
			

   	buf[0] = 0x05; 
   	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
   	Delay_ms(20);	
		
    ICRegCheck(0x07,RegBuf,DataBuf);
		
   	Delay_ms(20);	
   	buf[0] = 0x00; 
   	MIPI_GEN_Write((PORT0 | PORT1), 0xFF, 1, buf);
   	Delay_ms(20);
}
