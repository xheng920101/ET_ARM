/*********************************************************************************
*							Driver IC: OTM1911A  20170615 For 18：9
 ********************************************************************************/

#include "include.h" 


#define		SSD_HACT	1080
#define		SSD_VACT	2160
#define		SSD_HSA		0x04    
#define		SSD_HBP		0x20   //0x32
#define		SSD_HFP		0x20   //0x32  
#define		SSD_VSA		0x02
#define		SSD_VBP		0x0B
#define		SSD_VFP		0x0E  //0x08为IC内部配置 20170224

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	1080,	//FPGA_HAC
	2160,	//FPGA_VAC
	0x04,	//FPGA_HSA
	0x20,	//FPGA_HBP 0x17 
	0x20,	//FPGA_HFP 0x18
	0x02,	//FPGA_VSA
	0x09,	//FPGA_VBP
	0x0E  //FPGA_VFP 0x08为IC内部配置20170224                                                            
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
//0x1BA,    0x24B,	//24B	[7:0]  NS=75  MAX=96  PLL=24M*96/2=1.152GHz 50 64  4B
//          0x2C2,  //2C2	[12:8] MS=2  so PLL=24M*75/2=900M		        C2 C3  C2
//0x1BA,    0x247,	//24B	[7:0]  NS=71  MAX=96  PLL=24M*96/2=1.152GHz 50 64  4B
//          0x2C2,  //2C2	[12:8] MS=2  so PLL=24M*71/2=850M		        C2 C3  C2
	
0x1BA,	  0x250,	   //[7:0]  NS=80
	        0x2C2,		// [12:8] MS=2 24MHz * 80 / 2 = 960Mbps	

//0x1BA,	  0x253,
//         	0x2C2,		// 24MHz * 83 / 2 = 1000Mbps	
	
0x1BB,    0x20A,	// RA 0F  normal 0A PL=900/10/8=11.25MHZ
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
//-------------  Display Setting  -------------------------	
0x1B7,	 0x250,	 0x205,	

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,
0x1BC,   0x204,  0x200,                                                                              
0x1BF,   0x2FF,  0x219,
                 0x211,
                 0x201,
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x203,  0x200,                                                                              
0x1BF,   0x2FF,  0x219,
                 0x211,	

//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x200,  0x281,
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x2A5,  0x200,//LTPS Timing Setting  
	
0x1BC,   0x202,  0x200,                                               
0x1BF,   0x200,  0x281,                                               
0x1BC,   0x202,  0x200,                                               
0x1BF,   0x2A5,  0x2B0,     // Timing Mode RGBBGR B0  

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x208,  0x200,                                                                              
0x1BF,   0x2C0,  0x260,
                 0x200,
                 0x205,
                 0x203,
                 0x25D,
                 0x204,
                 0x200,//TCON RTN Setting 
 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x292,
0x1BC,   0x203,  0x200,                                                                              
0x1BF,   0x2B3,  0x218,
                 0x206,//Panel Mode and Use B393 to enable S/W Setting
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x28B,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2C0,  0x288,//Panel Driving Mode 

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x294,//ckh
0x1BC,   0x208,  0x200,                                                                              
0x1BF,   0x2C0,  0x200,
                 0x200,
                 0x200, //CKV and CKH GAP     14(18：9)    
                 0x200,
                 0x201, //CKH SWT Gap    1
                 0x218, //CKH width   
                 0x205, //TCON LTPS Setting  CKH NOP Gap 5
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2C1,  0x20B, //OSC Adjust  

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x281,
0x1BC,   0x204,  0x200,                                                                              
0x1BF,   0x2C1,  0x230,
                 0x202,
                 0x2B0, //SSC Setting 
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x291,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2C1,  0x20F, //Video Mode Sync Signal Control

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,//stv
0x1BC,   0x207,  0x200,                                                                              
0x1BF,   0x2C2,  0x283, //STV1 shift 
                 0x202, //STV1 width   
                 0x25A, //STV1  chop——head
                 0x204, //STV1  shift-tial
                 0x200, //STV2 shift
                 0x200, //LTPS STV2 Setting 
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x290,//CIC
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
                 0x200,
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2B0,//ckv1~4
0x1BC,   0x20F,  0x200,                                                                              
0x1BF,   0x2C2,  0x282, //1 CKV1_active_str
                 0x202, //2 CKV1_active_end 
                 0x211, //3 CKV width
                 0x205, //4 CKV width rising
                 0x282, //5 CKV width falling 88
                 0x282, //6 CKV2_active_str
                 0x202, //7 CKV2_active_end
                 0x200, //8
                 0x282, //9 CKV3_active_str 
                 0x202, //10 CKV3_active_end
                 0x233, //11
                 0x282, //12 CKV4_active_str 
                 0x202, //13 CKV4_active_end 
                 0x222, //14 LTPS CKV Setting1
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2C0,//ckv5~8
0x1BC,   0x20F,  0x200,                                                                              
0x1BF,   0x2C2,  0x282,
                 0x202,
                 0x222,
                 0x205,
                 0x282,//88
                 0x282,
                 0x202,
                 0x233,
                 0x282,
                 0x202,
                 0x200,
                 0x282,
                 0x202,
                 0x211,
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2D0,//
0x1BC,   0x205,  0x200,                                                                              
0x1BF,   0x2C2,  0x233,
                 0x233,
                 0x233,
                 0x233,//LTPS CKV Setting3  
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2E0,//
0x1BC,   0x207,  0x200,                                                                              
0x1BF,   0x2C2,  0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x200,//LTPS VEND Setting 
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2F0,//GRST
0x1BC,   0x206,  0x200,                                                                              
0x1BF,   0x2C2,  0x200, //RST_Shift_MSB[3:0] 1   tcon_ltps_rst rising edge locates before reference point    
                 0x200, //RST_Shift_LSB[7:0]    tcon_ltps_rst rising edge locates before reference point  
                 0x200,
                 0x200,
                 0x2b0,//LTPS RST Setting  
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2F5,
0x1BC,   0x206,  0x200,                                                                              
0x1BF,   0x2C2,  0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x200,//LTPS CKHSEQ  
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2A5,  0x200,//PCH_toggle[0] rotate[7:4] 

0x1BC,   0x202,  0x200,     //?                                                                         
0x1BF,   0x200,  0x280,
0x1BC,   0x209,  0x200,                                                                              
0x1BF,   0x2C3,  0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x200,
                 0x212, //EQ width VGL-GND  GND-VGH 
                 0x222, //LTPS GND Precharge
                 
0x1BC,   0x202,  0x200,                                                                         
0x1BF,   0x200,  0x290,
0x1BC,   0x205,  0x200,                                                                              
0x1BF,   0x2C3,  0x220, //EQ GND-VSP     rising 
                 0x220,
                 0x201, //EQ GND-VSN      falling 
                 0x202, //LTPS VSP/VSN Precharge 
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x203,  0x200,                                                                              
0x1BF,   0x2C4,  0x241,
                 0x202,
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,//VGH VGL CLAMP VOLTAGE
0x1BC,   0x208,  0x200,                                                                              
0x1BF,   0x2C5,  0x255,
                 0x205,
                 0x211,
                 0x2AC,//10.5V  0x2AC
                 0x2A3,//-7.5V   0x2A3
                 0x296,
                 0x2AA,//Power Control Setting1
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x290,//LVD GP AP SAP
0x1BC,   0x206,  0x200,                                                                              
0x1BF,   0x2C5,  0x245,// 45 GAP 8->4 , AP 8->5, For Power Saving Setting Modify 88  
                 0x2F0,//source bias A0 4.0uA  F0 6.5uA  
                 0x275,
                 0x265,// APO   50 IOVCC 1.35V  VSP 3.6V VSN ﹣3.6V    65 IOVCC 1.4V  VSP 4V VSN ﹣4V 
                 0x280,//Power Control Setting2 
  
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2A0,
0x1BC,   0x205,  0x200,                                                                              
0x1BF,   0x2C5,  0x2A8,  //VGHO 10V For Power Saving Setting Modify            
                 0x29E,  //VGLO -7V For Power Saving Setting Modify            
                 0x204,  //power saving setting 00
                 0x288, //Power Control Setting3   
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2B0,
0x1BC,   0x204,  0x200,                                                                              
0x1BF,   0x2C5,  0x2A6,
                 0x208,//Power Control Setting4   Gamma calibration   08 enable 00 disable 
                 0x222,//Power Control Setting4   
                 
//0x1BC,   0x203,  0x200,                                                                              
//0x1BF,   0x251,  0x2FF,
//                 0x200,
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x253,  0x224,
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x255,  0x203, //CABC

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2B0,
0x1BC,   0x207,  0x200,                                                                              
0x1BF,   0x2CA,  0x20F, //adjust pwm pulse frequency in normal mode
                 0x20F, //adjust pwm pulse frequency in idle mode
                 0x209, //D[2:0] pwm frequency adjuestment
                 0x20F, // pwm low value bits
                 0x2FF, //pwm high value bits 
                 0x200, 
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x290,
0x1BC,   0x204,  0x200,                                                                              
0x1BF,   0x2CB,  0x200,//4 power on sequence
                 0x200,
                 0x203,
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2C0,
0x1BC,   0x20D,  0x200,                                                                              
0x1BF,   0x2CB,  0x200,// normal display and 3 power off sequence
                 0x214,
                 0x214,
                 0x214,
                 0x214,
                 0x214,
                 0x200,
                 0x200,
                 0x2d7,
                 0x200,
                 0x200,
                 0x200,//Single enmode 
                              
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x2F0,
0x1BC,   0x204,  0x200,                                                                              
0x1BF,   0x2CB,  0x203,// set LTPS single when LVD detected
                 0x200,
                 0x203,//Single enmode     LVD  
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x280,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2CD,  0x202,//CGOUT set 

0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,
0x1BC,   0x203,  0x200,                                                                              
0x1BF,   0x2D8,  0x22B, //GVDDP=5.0V
                 0x22B, //GVDDN=-5.0V
                 
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,
0x1BC,   0x203,  0x200,                                                                              
0x1BF,   0x2D9,  0x2B4, //VCOM
                 0x200,

///////////////////////GAMMA/////////////////////////////////////////
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,                                                                              
0x1BC,   0x202,  0x200,                                                                              
0x1BF,    0x2E0, 0x200,//Gamma Separate Change                                                       
                                                                                                     
                                                                             
0x1BC,   0x202,  0x200, 
0x1BF,   0x200,  0x200,                                                                              
0x1BC,   0x226,  0x200,                                                                              
0x1BF,   0x2E1,  0x200,                                                                              
                 0x25C,                                                                              
                 0x2CC,                                                                              
                 0x227,                                                                              
                 0x240,                                                                              
                 0x253,                                                                              
                 0x282,                                                                              
                 0x2D2,                                                                              
                 0x208,                                                                              
                 0x295,                                                                              
                 0x2F6,                                                                              
                 0x227,                                                                              
                 0x24E,                                                                              
                 0x270,                                                                              
                 0x2A9,                                                                              
                 0x299,                                                                              
                 0x2BE,                                                                              
                 0x22F,                                                                              
                 0x253,                                                                              
                 0x2AA,                                                                              
                 0x273,                                                                              
                 0x296,                                                                              
                 0x2B1,                                                                              
                 0x2D3,                                                                              
                 0x2AA,                                                                              
                 0x2F7,                                                                              
                 0x2F6, 
                 0x21A,								 
                 0x24E,                                                                              
                 0x2FA,                                                                              
                 0x26D,                                                                              
                 0x28B,                                                                              
                 0x2B6,                                                                              
                 0x2F2,                                                                                                                                                                      
                 0x2FF,                                                                              
                 0x2FD,                                                                              
                 0x203,//Gamma Separate Change                                                       
                                                                                                     
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x200,  0x200,                                                                              
//0x1BC,   0x226,  0x200,                                                                              
//0x1BF,   0x2E2,  0x200,                                                                              
//                 0x25F,                                                                              
//                 0x2B4,                                                                              
//                 0x2FD,                                                                              
//                 0x200,                                                                              
//                 0x225,                                                                              
//                 0x255,                                                                              
//                 0x29A,                                                                              
//                 0x2C8,                                                                              
//                 0x255,                                                                              
//                 0x2B3,                                                                              
//                 0x2E3,                                                                              
//                 0x205,                                                                              
//                 0x228,                                                                              
//                 0x2A5,                                                                              
//                 0x24D,                                                                              
//                 0x272,                                                                              
//                 0x2E4,                                                                              
//                 0x204,                                                                              
//                 0x29A,                                                                              
//                 0x229,                                                                              
//                 0x24E,                                                                              
//                 0x26F,                                                                              
//                 0x291,                                                                              
//                 0x2AA,                                                                              
//                 0x2BB,                                                                              
//                 0x2C3,                                                                              
//                 0x2F7,                                                                              
//                 0x234,                                                                              
//                 0x2EA,                                                                              
//                 0x263,                                                                              
//                 0x286,                                                                              
//                 0x2C0,                                                                              
//                 0x2F9,                                                                              
//                 0x2FF,                                                                              
//                 0x2FD,                                                                              
//                 0x203,//Gamma Separate Change                                                       
                                                                                                     
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,                                                                              
0x1BC,   0x226,  0x200,                                                                              
0x1BF,   0x2E3,  0x200,                                                                              
                 0x25C,                                                                              
                 0x2CC,                                                                              
                 0x227,                                                                              
                 0x240,                                                                              
                 0x253,                                                                              
                 0x282,                                                                              
                 0x2D2,                                                                              
                 0x208,                                                                              
                 0x295,                                                                              
                 0x2F6,                                                                              
                 0x227,                                                                              
                 0x24E,                                                                              
                 0x270,                                                                              
                 0x2A9,                                                                              
                 0x299,                                                                              
                 0x2BE,                                                                              
                 0x22F,                                                                              
                 0x253,                                                                              
                 0x2AA,                                                                              
                 0x273,                                                                              
                 0x296,                                                                              
                 0x2B1,                                                                              
                 0x2D3,                                                                              
                 0x2AA,                                                                              
                 0x2F7,                                                                              
                 0x2F6, 
                 0x21A,								 
                 0x24E,                                                                              
                 0x2FA,                                                                              
                 0x26D,                                                                              
                 0x28B,                                                                              
                 0x2B6,                                                                              
                 0x2F2,                                                                                                                                                                      
                 0x2FF,                                                                              
                 0x2FD,                                                                              
                 0x203,//Gamma Separate Change                                                       
                                                                                                     
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x200,  0x200,                                                                              
//0x1BC,   0x226,  0x200,                                                                              
//0x1BF,   0x2E4,  0x200,                                                                              
//                 0x25F,                                                                              
//                 0x2B4,                                                                              
//                 0x2FD,                                                                              
//                 0x200,                                                                              
//                 0x225,                                                                              
//                 0x255,                                                                              
//                 0x29A,                                                                              
//                 0x2C8,                                                                              
//                 0x255,                                                                              
//                 0x2B3,                                                                              
//                 0x2E3,                                                                              
//                 0x205,                                                                              
//                 0x228,                                                                              
//                 0x2A5,                                                                              
//                 0x24D,                                                                              
//                 0x272,                                                                              
//                 0x2E4,                                                                              
//                 0x204,                                                                              
//                 0x29A,                                                                              
//                 0x229,                                                                              
//                 0x24E,                                                                              
//                 0x26F,                                                                              
//                 0x291,                                                                              
//                 0x2AA,                                                                              
//                 0x2BB,                                                                              
//                 0x2C3,                                                                              
//                 0x2F7,                                                                              
//                 0x234,                                                                              
//                 0x2EA,                                                                              
//                 0x263,                                                                              
//                 0x286,                                                                              
//                 0x2C0,                                                                              
//                 0x2F9,                                                                              
//                 0x2FF,                                                                              
//                 0x2FD,                                                                              
//                 0x203,//Gamma Separate Change                                                       
                                                                                                     
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x200,                                                                              
0x1BC,   0x226,  0x200,                                                                              
0x1BF,   0x2E5,  0x200,                                                                              
                 0x25C,                                                                              
                 0x2CC,                                                                              
                 0x227,                                                                              
                 0x240,                                                                              
                 0x253,                                                                              
                 0x282,                                                                              
                 0x2D2,                                                                              
                 0x208,                                                                              
                 0x295,                                                                              
                 0x2F6,                                                                              
                 0x227,                                                                              
                 0x24E,                                                                              
                 0x270,                                                                              
                 0x2A9,                                                                              
                 0x299,                                                                              
                 0x2BE,                                                                              
                 0x22F,                                                                              
                 0x253,                                                                              
                 0x2AA,                                                                              
                 0x273,                                                                              
                 0x296,                                                                              
                 0x2B1,                                                                              
                 0x2D3,                                                                              
                 0x2AA,                                                                              
                 0x2F7,                                                                              
                 0x2F6, 
                 0x21A,								 
                 0x24E,                                                                              
                 0x2FA,                                                                              
                 0x26D,                                                                              
                 0x28B,                                                                              
                 0x2B6,                                                                              
                 0x2F2,                                                                                                                                                                      
                 0x2FF,                                                                              
                 0x2FD,                                                                              
                 0x203,//Gamma Separate Change                                                       
                                                                                                     
//0x1BC,   0x202,  0x200,                                                                              
//0x1BF,   0x200,  0x200,                                                                              
//0x1BC,   0x226,  0x200,                                                                              
//0x1BF,   0x2E6,  0x200,                                                                              
//                 0x25F,                                                                              
//                 0x2B4,                                                                              
//                 0x2FD,                                                                              
//                 0x200,                                                                              
//                 0x225,                                                                              
//                 0x255,                                                                              
//                 0x29A,                                                                              
//                 0x2C8,                                                                              
//                 0x255,                                                                              
//                 0x2B3,                                                                              
//                 0x2E3,                                                                              
//                 0x205,                                                                              
//                 0x228,                                                                              
//                 0x2A5,                                                                              
//                 0x24D,                                                                              
//                 0x272,                                                                              
//                 0x2E4,                                                                              
//                 0x204,                                                                              
//                 0x29A,                                                                              
//                 0x229,                                                                              
//                 0x24E,                                                                              
//                 0x26F,                                                                              
//                 0x291,                                                                              
//                 0x2AA,                                                                              
//                 0x2BB,                                                                              
//                 0x2C3,                                                                              
//                 0x2F7,                                                                              
//                 0x234,                                                                              
//                 0x2EA,                                                                              
//                 0x263,                                                                              
//                 0x286,                                                                              
//                 0x2C0,                                                                              
//                 0x2F9,                                                                              
//                 0x2FF,                                                                              
//                 0x2FD,                                                                              
//                 0x203,//Gamma Separate Change           



0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x200,  0x28A,
0x1BC,   0x202,  0x200,                                                                              
0x1BF,   0x2C0,  0x204,	

/////////////Resolution////////////////////////    

0x1BC,   0x202,  0x200, 
0x1BF,   0x200,  0x2B0,
0x1BC,   0x205,  0x200, 
0x1BF,   0x2B3,  0x204,
         0x238,
				 0x208,
				 0x270,    

	
0x3FF,	
	
	0xF00	
	
};

unsigned short ET2_InitCode[]= 
{
//-------------  Display Setting  -------------------------
   0x1B7,   0x250,  0x205,	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x200,
   0x1BC,   0x204 , 0x200,
   0x1BF,   0x2FF,  0x219,
                    0x211, 
                    0x201,
	                //0x200,      //Enable command2&enable shift function

   0x310,
	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x280,     //set parameter shift value
   0x1BC,   0x203,  0x200,
   0x1BF,   0x2FF,  0x219,
                    0x211,     //enbale orise command

/////////////Resolution////////////////////////    
0x1BC,   0x202,  0x200, 
0x1BF,   0x200,  0x2B0,
0x1BC,   0x205,  0x200, 
0x1BF,   0x2B3,  0x204,
                 0x238,
				         0x208,
				         0x270,  
				 

 

0x310,


	 
//==========PWM setting==================//
//  0x30A,
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x251, 0x2FF,	      // Adjust the  brightness	of BL
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x253, 0x22C,	  
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x255, 0x200 ,	      //  00= off 01=low 10=medium(still picture) 11=High(moving image)	


//==========sleep out and display on=====//

//	0x1BC,	0x201, 0x200,
//	0x1BF,	0x211,
//	0x3E1,

//	0x1BC,	0x201, 0x200,
//	0x1BF,	0x229,
//	0x3FF,

//===========set 2828 to High speed mode===//
//  0x3FF,
//	0x1B7,	0x219, 0x205,	
//	0x3FF,

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
	                 //0x200,       //Enable command2&enable shift function

  0x310,
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x280,         //set parameter shift value
  0x1BC,   0x203,  0x200,
  0x1BF,   0x2FF,  0x219,
                   0x211,         //enbale orise command

	0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x291,         //Free run mode videoìyía2?20161122
	0x1BC,   0x202,  0x200,
  0x1BF,   0x2C1,  0x208,	
	0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x2A0,         //Free run mode BWRGBG127
	0x1BC,   0x20B,  0x200,
  0x1BF,   0x2F6,  0x285,
                   0x201, 
                   0x223, 
									 0x245,
                   0x267,
                   0x289,    
                   0x2AB, 
									 0x2CD,
                   0x2EF,
                   0x25A,    
	
	0x3FF,
/////////////Resolution////////////////////////    
0x1BC,   0x202,  0x200, 
0x1BF,   0x200,  0x2B0,
0x1BC,   0x205,  0x200, 
0x1BF,   0x2B3,  0x204,
                 0x238,
				         0x208,
				         0x270,  

	0x1BC,	0x202,	0x200,
	0x1BF,	0x200,	0x200,
	0x1BC,	0x202,	0x200,
	0x1BF,	0x21C,	0x233,

       0x1BC,   0x202,  0x200,                                               
       0x1BF,   0x200,  0x200,  //Disable command2                                               
       0x1BC,   0x204,  0x200,                                               
       0x1BF,   0x2FF,  0x2FF,                                               
                        0x2FF,                                               
                        0x2FF,  

	0x3FF,
//	
//==========PWM setting==================//
//	0x30A,
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x251, 0x2FF,	  // Adjust the  brightness	of BL
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x253, 0x22C,	  
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x255, 0x200 ,	  //  00= off 01=low 10=medium(still picture) 11=High(moving image)


////==========sleep out and display on=====//

//	0x1BC,	0x201, 0x200,
//	0x1BF,	0x211,
//	0x3E1,

//	0x1BC,	0x201, 0x200,
//	0x1BF,	0x229,
//	0x3FF,

////========set 2828 to High speed mode===//
//  0x3FF,
//	0x1B7,	0x219, 0x205,	
//	0x3FF,

								 
	0xF00	
};

uint16_t gamma[10][64];

