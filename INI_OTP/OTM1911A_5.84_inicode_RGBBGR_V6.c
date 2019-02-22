/*********************************************************************************
*							Driver IC: OTM1911A  FOR CMOS 5.84 2018-2-5
 ********************************************************************************/

#include "include.h" 


#define		SSD_HACT	1080
#define		SSD_VACT	2280
#define		SSD_HSA		0x14     
#define		SSD_HBP		0x19   
#define		SSD_HFP		0x18     
#define		SSD_VSA		0x02
#define		SSD_VBP		0x08
#define		SSD_VFP		0x0E   //0x08ÎªICÄÚ²¿ÅäÖÃ 

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	SSD_HACT,					//
	SSD_VACT,					//
	SSD_HSA,					//
	SSD_HBP,					//  HBP(FPGA) = HBP(2828) - HSA; this config have been verified,ok!
	SSD_HFP,					// 
	SSD_VSA,					//
	SSD_VBP- SSD_VSA, //  VBP(FPGA) = VBP(2828) - VSA;
	SSD_VFP,			    // 	        
};

unsigned short SSDInitCode[]= 
{
	//-------------  Start  -------------------------------- 	 
	0x1B9,   	0x200,  //PLL control register.//PLL enable	
				    0x200,  //divide by 1
	0x1DE,   	0x203,  //Lane configuration register//4 Lane	
	0x200,
	0x1B8,   	0x200,  //VC control register 	
				    0x200,  //virtual channel 	
	0x1B7,   	0x250,  //configuration register0	
	0x202,
	0x1BA,    0x252,	//24B	[7:0]  NS=75  MAX=96  PLL=24M*82/2=984MHz 50 64  4B
				    0x2C2,  //2C2	[12:8] MS=2  
	//0x1BA,	  0x266,	
	//	        0x2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810	
	//0x1BA,	0x296,
	//         	0x2C4,		// 24MHz * 150 / 4 = 900Mbps	
	
	0x1BB,    0x20B,	//  LP clock = MIPI(PLL) Mbps / LPD / 8 = 984 / 12 / 8 = 10.25MHz
	0x200,
	0x1B9,    0x201,
	0x200,
	
	0x1C9,   	0x207, //delay adjustment register10//Ths-prepare 	
	          0x22C, //Ths-zero	
	0x1CA,   	0x20A, //delay adjustment register20 //Tclk-zero 	
	          0x228, //Tclk-prepare
	0x1CC,   	0x20F, //delay adjustment register40 //Ths-trail
	          0x20F, //Tclk-trail
	0x1D6,   	0x205, //test register0//color order BGR,B is the higher 
	          0x200,  
	/////////////////////////////
	0x1B1,	(0x200 | SSD_HSA),	(0x200 | SSD_VSA),		//VSA,HSA
	0x1B2,	(0x200 | SSD_HBP),	(0x200 | SSD_VBP),		//VBP,HBP	
	0x1B3,	(0x200 | SSD_HFP),	(0x200 | SSD_VFP),		//VFP,HFP
	0x1B4,	(0x200 |(SSD_HACT & 0x00FF)),	(0x200 |((SSD_HACT >> 8) & 0x00FF)),		//HACT,	
	0x1B5,	(0x200 |(SSD_VACT & 0x00FF)),	(0x200 |((SSD_VACT >> 8) & 0x00FF)),		//VACT,	
	
	0x1B6,	0x207,	0x200,		//Non burst mode with sync events
	//	0x1B6,	0x20b,	0x200,		//Burst mode
	//////////////////////////////
	0x1CB,   	0x210, //delay adjustment register30 //Tclk-post	 
	          0x205, //Tclk-per	 
	
	///////////////////////////////////////////////////FHD OK
	0x1B7,   	0x210, //10 configuration register0 	
	          0x205, //05 long packet enable,short packet?
	
	0x1BD,   	0x200, //packet size control register 20 	 
	          0x200, //0x2C,0x3C
	0xF00
};

unsigned short ET1_InitCode[]= 
{
	0x1B7,	 0x250,	 0x205,
//RFF00:CMD2_ENA1
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x204,  0x200,                                                                              
	0x1BF,   0x2FF,  0x219,                                                                              
					         0x211,                                                                              
					         0x201,	//Enable command2                                                             
//RFF00:CMD2_ENA2																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x203,  0x200,                                                                              
	0x1BF,   0x2FF,  0x219,                                                                              
					         0x211,     

//A581:	LTPS Timing Setting
	0x1BC,   0x202,  0x200,                                               
	0x1BF,   0x200,  0x281,                                               
	0x1BC,   0x202,  0x200,                                               
	0x1BF,   0x2A5,  0x2B0,	// Timing Mode RGBBGR = 0xB0, RGBRGB = 0x00
//C080:	TCON RTN SETTING
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x208,  0x200,                                                                              
	0x1BF,   0x2C0,  0x25D,                                                                              
									 0x200,                                                                              
									 0x208,                                                                              
									 0x208,                                                                              
									 0x25D,                                                                     
									 0x204,                                                                              
									 0x200,	//TCON RTN Setting  
//RB392	Panel MODE							 
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x292,                                                  
	0x1BC,   0x203,  0x200,                                                                              
	0x1BF,   0x2B3,  0x218,                                                                              
					         0x206,	//Panel Mode and Use B393 to enable S/W Setting     
//RB3B0													
	0x1BC,   0x202,  0x200,	                                                                              
	0x1BF,   0x200,  0x2B0,	//                                                   
	0x1BC,   0x205,  0x200,	                                                                              
	0x1BF,   0x2B3,  0x204,	                                                                              
									 0x238,	//
									 0x208,	//
									 0x2E8,	//1080*2280
//C08B Panle Driving Mode																								
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x28B,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2C0,  0x288, //Panel Driving Mode                                                          
//C094 TCON LTPS SETTING
  0X1BC,	0X202,	0X200,	
	0X1BF,	0X200,	0X294,	
	0X1BC,	0X208,	0X200,	
	0X1BF,	0X2C0,	0X200,	
									0X20C,	
									0X203,	//CKV and CKH GAP                                                                    
									0X200,	
									0X202,	//CKH SWT Gap                                                                                     
									0X211,	//CKH width           
									0X205,	//TCON LTPS Setting  CKH NOP Gap
											
//C180 Oscillator Ajustment				
	0x1BC,   0x202,  0x200,   //19:9                                                                             
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2C1,  0x20F,//OSC Adjust    
//C181 OSC_SSC																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x281,
	0x1BC,   0x204,  0x200,                                                                              
	0x1BF,   0x2C1,  0x210, //0x290(+/-0.9%),0x2A0(+/-1.8%),0x2B0(+/-2.7%) 
					         0x2C0, 
					         0x2F0,
	
//C191 VIDEO_SYNC																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x291,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2C1,  0x20F,//Video Mode Sync Signal Control 	
//C280 LTPS_CKV1	
	0x1BC,   0x202,  0x200,                       //  19:9                                                       
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x207,  0x200,                                                                              
	0x1BF,   0x2C2,  0x283,     //STV1 shift                                                                       
									 0x202,    	//STV1 width                                                                         
									 0x255,     //STV1  chop¡ª¡ªhead                                                                           
									 0x21C,    	//STV1  shift-tial                                                                            
									 0x283,     //STV2 shift                                                                          
									 0x202,		  //LTPS STV2 Setting    
//C290 LTPS CIC Setting																																																																	
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x290,                                                                              
	0x1BC,   0x20C,  0x200,                                                                              
	0x1BF,   0x2C2,  0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,//LTPS CIC Setting          

	//C2B0 LTPS_CKV1	
	0X1BC,	 0X202,	 0X200,	
	0X1BF,	 0X200,	 0X2B0,	
	0X1BC,	 0X20F,	 0X200,	
	0X1BF,	 0X2C2,	 0X282,	//CKV1_active_str                                             
									 0X202,	//CKV1_active_end                                    
									 0X200,	
									 0X213,	//CKV width 05 8.2us   08 7.8us  (rising)      0A  // 0C TO12      20171204                               
									 0X281,	//CKV width falling    00~81 cover last line display issue                                       
									 0X282,	//CKV2_active_str                                             
									 0X202,	//CKV2_active_end                                        
									 0X211,	
									 0X282,	//CKV3_active_str                                              
									 0X202,	//CKV3_active_end                                             
									 0X222,	
									 0X282,	//CKV4_active_str                                             
									 0X202,	//CKV4_active_end                                          
									 0X233,	//LTPS CKV Setting1   									 
									 
									 
//C2C0 LTPS_CKV2																											
	0x1BC,   0x202,  0x200,                                                                             
	0x1BF,   0x200,  0x2C0,                                                                              
	0x1BC,   0x20F,  0x200,                                                                              
	0x1BF,   0x2C2,  0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,//LTPS CKV5Setting2      
//C2D0 LTPS_CKV3																																							
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2D0,                                                                              
	0x1BC,   0x206,  0x200,                                                                              
	0x1BF,   0x2C2,  0x233,                                                                              
					         0x233,                                                                              
					         0x200,                                                                              
					         0x200,//LTPS CKV Setting3     
									 0x200,							 
//C2E0	LTPS_VEND																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2E0,                                                                              
	0x1BC,   0x207,  0x200,                                                                              
	0x1BF,   0x2C2,  0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,	//LTPS VEND Setting    
//C2F0	LTPS_RST																								
	0x1BC,   0x202,  0x200, //                                                                           
	0x1BF,   0x200,  0x2F0,                                                                              
	0x1BC,   0x206,  0x200,                                                                              
	0x1BF,   0x2C2,  0x280, //RST_Shift_MSB[3:0] 1   tcon_ltps_rst rising edge locates before reference point                                                                            
					         0x205, //RST_Shift_LSB[7:0]    tcon_ltps_rst rising edge locates before reference point                                                                             
					         0x200,                                                                              
					         0x214,                                                                              
					         0x2B0, //LTPS RST Setting                                                            
//C2F5 LTPS_CKHSEQ																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2F5,                                                                              
	0x1BC,   0x206,  0x200,                                                                              
	0x1BF,   0x2C2,  0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200, //LTPS CKHSEQ                                                                 
//A580 LTPS SET																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2A5,  0x200, //PCH_toggle[0] rotate[7:4]                                                   
//C380 LTPS_GND_PCG																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x209,  0x200,                                                                              
	0x1BF,   0x2C3,  0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x222,  //EQ width VGL-GND  GND-VGH                                                                 
					         0x222,  //LTPS GND Precharge                                                          
//C390 LTPS_VSP_VSN_PCG																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x290,                                                                              
	0x1BC,   0x205,  0x200,                                                                              
	0x1BF,   0x2C3,  0x200,  //EQ  GND-VSP rising                                                                             
					         0x200,                                                                              
					         0x200,  //EQ GND-VSN falling                                                                         
					         0x200,	 //LTPS VSP/VSN Precharge                                                      
//C480 SD_DISSET																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x203,  0x200,                                                                              
	0x1BF,   0x2C4,  0x241,  //B7 = 1 ,while blanking Source ouput Hiz      0x41--0xC1                                                                     
					         0x202,                                                                              
//C580	PWR_CTRL1
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x208,  0x200,                                                                              
	0x1BF,   0x2C5,  0x255, //C580 D[6:4] = VGH_PUMP_CLK_RATIO[2:0] ;D[2:0] = VGL_PUMP_CLK_RATIO[2:0] 5 = 2Line Rate                                                                
	                 0x205, //C581 D[7:6]=VCL[1:0] =0: - 3.50V   D[3:2]=VGH_SEL[1:0]=01:?; D[1:0]=DVGH_SEL_PROG[1:0]=01:2XAVDD                                                                         
					         0x211, //C582 D[6:4]=VGL_SEL[2:0]= 001:2xAVEE ;D[2:0]= VGL_SEL_PROG[2:0]                                                                    
					         0x2B1, //C583 VGH_CLP[6:0]=0xB1 VGH Clamp on (Clamp of is for noise reduce) VGH=11V                                                                
					         0x2C6, //C584 VGL_CLP[6:0]=0xC6 VGH Clamp on  VGL=-11V                                                                 
					         0x296, //C585 VGH_CLP_PROG[6:0]= 0x16 :8.3V   This Voltage is For OTP USE Only									 
					         0x2AA, //C586 VGL_CLP_PROG[6:0]= 0x2A :-8.2V                                                     
//C590	PWR_CTRL2
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x290,                                                                              
	0x1BC,   0x206,  0x200,                                                                              
	0x1BF,   0x2C5,  0x245,  //C590 VREF_CAP[7:4] ;VREF_AP[3:0];45 GAP 8->4 , AP 8->5, 20160420?? For Power Saving Setting Modify 88                                                                               
	                 0x2A0,  //C591 VREF_SAP[7:4:;]source bias A0 4.0uA  F0 6.5uA                                                                             
					         0x275,  //C592 VREF_VDD[7:4];VREF_LVDSVDD[3:0];                                                                         
					         0x245,  //C593 LVD_VCI[1:0]:;LVD_VDD[1:0];LVD_AVDD[1:0];LVD_AVEE[1:0];APO   VCI~2.1V IOVCC~1.4V VSP~4V VSN~-4V                            
					         0x200,  //C594 Enable/Disable VCI = Disable(0) ,VDD = Eanble(0) AVDD|AVEE = Enable(0)                                                                                   
//C5A0	PWR_CTRL3																								
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2A0,                                                                              
	0x1BC,   0x205,  0x200,                                                                              
	0x1BF,   0x2C5,  0x294,  //94 VGHO=8.0V,  For Power Saving Setting Modify                                                                               
					         0x29E,  //9E VGLO=-7.0V,  For Power Saving Setting Modify                                                                            
					         0x204,                                                                           
					         0x200,	 //Power Control Setting3         
//C5B0	PWR_CTRL4								 
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2B0,                                                                              
	0x1BC,   0x204,  0x200,                                                                              
	0x1BF,   0x2C5,  0x2A6,                                                                              
					         0x208,  //Power Control Setting4   Gamma calibration   08 enable 00 disable                                                     
					         0x222,  //Power Control Setting4   Gamma_chop                                         
//CAB0 PWM_CTRL								
	0x1BC,   0x202,  0x200,   //pwm                                             
	0x1BF,   0x200,  0x2B0,                                                                                                               
	0x1BC,   0x207,  0x200,                                               
	0x1BF,   0x2CA,  0x20F,   //adjust pwm pulse frequency in normal mode                                               
					         0x20F,   //adjust pwm pulse frequency in idle mode                                               
					         0x209,   //D[2:0] pwm frequency adjuestment
					         0x20F,   //pwm low value bits                                            
					         0x2FF,   //pwm high value bits                                           
					         0x200,   //pwm dimming frame number                                
//CB90	PANELIF																								
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x290,                                                                              
	0x1BC,   0x204,  0x200,                                                                              
	0x1BF,   0x2CB,  0x203,                                                                              
					         0x200,                                                                              
					         0x20C,//Single enmode     
//CBC0	PANELIF					
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2C0,                                                                              
	0x1BC,   0x20D,  0x200,                                                                              
	0x1BF,   0x2CB,  0x2FE,                                                                              
					         0x2F4,                                                                              
					         0x2F4,                                                                              
					         0x2F4,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x200,                                                                              
					         0x2F4,                                                                              
					         0x207,                                                                              
					         0x200,                                                                              
					         0x200,//Single enmode    H                                                           
//CBF0 PANELIF		
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x2F0,                                                                              
	0x1BC,   0x204,  0x200,                                                                              
	0x1BF,   0x2CB,  0x2FF,                                                                              
					         0x200,                                                                              
					         0x203,//Single enmode     LVD                                       
//CD80	PANELIF																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x280,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2CD,  0x20A,//CGOUT set                                                
//D800	GVDDSET																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x203,  0x200,                                                                              
	0x1BF,   0x2D8,  0x22B,//GVDDP 1B~4.2V 1F~4.4V 2B~5V 2F~5.2V                                                                          
					         0x22B,//GVDDN                                                       
//D900	VCOMDC																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x203,  0x200,                                                                              
	0x1BF,   0x2D9,  0x2B4,
					         0x200,//VCOM                                                                  
//E000	GAMCHG																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x2E0, 0x201,//Gamma Separate Change                                                       
	
//E100 GAMCTRP
	0x1BC,   0x202,  0x200, 
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E1,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                              
//E200 GAMCTRN																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E2,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                                    
//E300 GAMCTGP																										
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E3,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                                      
//E400 GAMCTGN																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E4,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                                         
//E500	GAMCTBP																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E5,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                                          
//E600	GAMCTBN																									
	0x1BC,   0x202,  0x200,                                                                              
	0x1BF,   0x200,  0x200,                                                                              
	0x1BC,   0x226,  0x200,                                                                              
	0x1BF,   0x2E6,  0x231,
					         0x262,
					         0x2be,
					         0x212,
					         0x240,
					         0x23f,
					         0x270,
					         0x2b6,
					         0x2ed,
					         0x255,
					         0x2dd,
					         0x20d,
					         0x237,
					         0x25a,
					         0x2a9,
					         0x283,
					         0x2a8,
					         0x21a,
					         0x23d,
					         0x2aa,
					         0x25e,
					         0x281,
					         0x2a0,
					         0x2be,
					         0x2aa,
					         0x2e3,
					         0x2e5,
					         0x20c,
					         0x23d,
					         0x2fa,
					         0x25d,
					         0x27b,
					         0x2ab,
					         0x2e9,
					         0x2ff,
					         0x2fd,
					         0x203,                                                                                                                                                         
//C08A	SCAN_MODE																									
	0x1BC,   0x202,  0x200,                                              
	0x1BF,   0x200,  0x28A,                                               
	0x1BC,   0x202,  0x200,                                               
	0x1BF,   0x2C0,  0x204,   //Panel Scan Mode 	
	
	//0x1BC,	0x202,	0x200,	 //TE 
	//0x1BF,	0x235,  0x200,	
	0xF00
 };

unsigned short ET2_InitCode[]= 
{
//-------------  Display Setting  -------------------------
   0x1B7,   0x250,  0x205,	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x200,
   0x1BC,   0x204,  0x200,
   0x1BF,   0x2FF,  0x219,
                    0x211, 
                    0x201,

   0x310,
	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x280,     //set parameter shift value
   0x1BC,   0x203,  0x200,
   0x1BF,   0x2FF,  0x219,
                    0x211,     //enbale orise command
                    0x310,
	
   0x1BC,   0x202,  0x200,                                                                              
   0x1BF,   0x200,  0x2B0,                                                   
   0x1BC,   0x205,  0x200,                                                                              
   0x1BF,   0x2B3,  0x204,                                                                              
   				          0x238,
   				          0x208,
					          0x2E8,    //1080*2280
	0xF00
};

unsigned short RA_InitCode[]=
{
//-------------  Display Setting  -------------------------
  0x1B7,   0x250,  0x205,	
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x200,
  0x1BC,   0x204,  0x200,
  0x1BF,   0x2FF,  0x219,
                   0x211, 
                   0x201,
	
  0x310,
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x280,         //set parameter shift value
  0x1BC,   0x203,  0x200,
  0x1BF,   0x2FF,  0x219,
                   0x211,         //enbale orise command
	
  0x1BC,   0x202,  0x200,                                                                              
  0x1BF,   0x200,  0x2B0,                                                   
  0x1BC,   0x205,  0x200,                                                                            
  0x1BF,   0x2B3,  0x204,                                                                            
				           0x238,
				           0x208,
				           0x2E8,         //1080*2280

  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x291,         //Free run mode video
  0x1BC,   0x202,  0x200,
  0x1BF,   0x2c1,  0x208,	
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x2A0,         //Free run mode BWRGBG127
  0x1BC,   0x20B,  0x200,
  0x1BF,   0x2F6,  0x286,
                   0x210, 
                   0x223, 
				   0x245,
                   0x267,
                   0x289,    
                   0x2AB, 
				   0x2CD,
                   0x2EF,
                   0x25A,    

  0x1BC,  0x202,   0x200,
  0x1BF,  0x200,   0x200,
  0x1BC,  0x202,   0x200,
  0x1BF,  0x21C,   0x233,

  0x1BC,   0x202,  0x200,                                               
  0x1BF,   0x200,  0x200,         //Disable command2                                               
  0x1BC,   0x204,  0x200,                                               
  0x1BF,   0x2FF,  0x200,                                               
				           0x200,                                               
				           0x200,  
  0xF00	
};

uint16_t gamma[10][64];

