
#include "include.h" 

#define    SSD_HACT 720
#define    SSD_VACT 1280
#define    SSD_HSA  0x02  
#define    SSD_HBP  0x2E
#define    SSD_HFP  0x49 //0x40  
#define    SSD_VSA  0x02
#define    SSD_VBP  0x0C
#define    SSD_VFP  0x0E
 
 
FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
			720, //FPGA_HAC
			1280, //FPGA_VAC
			0x02, //FPGA_HSA
			0x2E, //FPGA_HBP
			0x49,//0x40, //FPGA_HFP
			0x02, //FPGA_VSA
			0x0A, //FPGA_VBP
			0x0E  //FPGA_VFP
};

unsigned short SSDInitCode[]= 
{

//	0X1BA,	0X206,	0X280,		// 24MHz * 102 / 3 = 896Mbps	 135*24/4=810
//	0X1BB,	0X204,	0X200,		//	896 / 8 / 11 = 10.18MHz

	0X1B7,	0X250,	0X202,
	0X1BA,	0X266,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
	0X1BB,	0X20A,	0X200,		//	800 / 2 / 8 / 8 = 6.25MHz
	0X1B9,	0X201,	0X200,		//PLL Control Register
	0X317,
	0X1B1,	(0X200 | SSD_HSA),	(0X200 | SSD_VSA),		//VSA,HSA
	0X1B2,	(0X200 | SSD_HBP),	(0X200 | SSD_VBP),		//VBP,HBP	
	0X1B3,	(0X200 | SSD_HFP),	(0X200 | SSD_VFP),		//VFP,HFP
	0X1B4,	(0X200 |(SSD_HACT & 0X00FF)),	(0X200 |((SSD_HACT >> 8) & 0X00FF)),		//HACT,	
	0X1B5,	(0X200 |(SSD_VACT & 0X00FF)),	(0X200 |((SSD_VACT >> 8) & 0X00FF)),		//VACT,	
	0X1B6,	0X207,	0X200,		//Non burst mode with sync events
//	0X1B6,	0X20b,	0X200,		//Burst mode
	0X1C9,	0X207,	0X22C,
	0X1CA,	0X20A,	0X228,
	0X1CB,	0X210,	0X205,
	0X1CC,	0X20F,	0X20F,
	0X1DE,	0X203,	0X200,		//Lane Configuration Register
	0X1D6,	0X205,	0X200,
	0X1B8,	0X200,	0X200,
	0X1BD,	0X200,	0X200,
	0XF00

};

unsigned short ET1_InitCode[]= 
{
//-------------  Display ON Setting  ---------------------						
0x311,
//================================================//
//===============CMD1=============================//
//================================================//
0x1BC,   	0x202,  	 
	 		0x200, 
0x1BD,   	0x200,  	 
	        0x200,
	 	
0x1BF,   	0x2FF, 
	        0x200,//CMD1        
//0x1BF,   	0x2FB,//01:Don�t reload MTP or register default value to register
//	        0x201,//00:Reload setting value from MTP or register default value to register
     
0x1BC,   	0x206,  	 
	        0x200,             
0x1BF,   	0x23B,
	        0x203,
	        0x206,//VBP=6
	        0x203,//VFP=3
	        0x202,//HBP=2
	        0x202,//HFP=2   
///================================================//
//=================CMD2 PAGE0 ====================//
//================================================//		
0x1BC,   	0x202,0x200,            
0x1BF,   	0x2FF,0x201,//CMD2 PAGE0        
0x1BF,   	0x2FB,0x201,                 
0x1BF,   	0x200,0x23A,//Panel CTRL  A:normal black /reslution 720*1280
0x1BF,   	0x201,0x233,//2PWR MODE,internal AVDD charge pump frequency setting in normal mode FOR
0x1BF,   	0x202,0x253,//2PWR MODE,internal AVDD charge pump frequency setting in idle mode
0x1BF,   	0x203,0x253,//2PWR MODE,internal AVEE charge pump frequency setting in normal mode
0x1BF,   	0x204,0x255,//2PWER MODE,internal AVDD charge pump frequency setting in idle mode
0x1BF,   	0x205,0x233,//2PWER MODE,AVDD+AVEE
0x1BF,   	0x206,0x222,//VCI SETTING FOR AVDD AND AVEE,2PWER MODE	   	         
0x1BF,   	0x208,0x216,//VGH AND VGL POWER SETTING,VCL-AVDD,2AVDD                
0x1BF,   	0x209,0x20A,//Disable VGL Clamp function 03/15               
0x1BF,   	0x20B,0x2A7,//GVDDP4.2V                
0x1BF,   	0x20C,0x2A7,//GVDDN-4.2V               
0x1BF,   	0x20D,0x22F,//SET VCI1 FOR 3.05V               
0x1BF,   	0x20E,0x233,//VGHO 10V		
0x1BF,   	0x20F,0x29E,//VGLO -7V								                
0x1BF,   	0x211,0x28B,//VCOMDC3  v
0x1BF,   	0x212,0x203,//VCOMDC3 HIGH 8 BIT                             
0x1BF,   	0x236,0x273,//EQ timing for CGOUT22-CGOUT27                   
0x1BF,   	0x237,0x202,//POWER OFF SEQUENCE VGLO PULL GND ADD 0315		
0x1BF,   	0x26F,0x200,//GATE Driving ability control,Default ultra high,ADD 0315		
0x1BF,   	0x271,0x22C,//set MIPI control TA-GO period
//=================CMD2 PAGE4 ====================//
0x1BF,   	0x2FF,0x205,//CMD2 PAGE4  
0x1BF,   	0x2FB,0x201,              
0x1BF,   	0x201,0x200,//RTN FOR HIGH 8 BITS
0x1BF,   	0x202,0x28D,//FRAME RATE CONTROL FOR NORMAL MODE   
0x1BF,   	0x203,0x28D,//FRAME RATE CONTROL FOR IDLE MODE 8D-141/11Mhz=12.8ms
0x1BF,   	0x204,0x28D,//FRAME RATE CONTROL FOR PARTIAL MODE
0x1BF,   	0x205,0x230,//SOURCE OUTPUT LEVEL FOR PORCH AREAS FOR NORMAL  MODE -GND
0x1BF,   	0x206,0x233,//SOURCE OUTPUT LEVEL FOR PORCH AREAS FOR IDLE AND PARTIAL MODE  	
0x1BF,   	0x207,0x277,//pre charge setting FOR normal mode/idle mode/partial mode.
//============LCD drive timing setting start============//
0x1BF,   	0x208,0x200, //0 
0x1BF,   	0x209,0x200,//FS=0  
0x1BF,   	0x20A,0x200,//FW=0,stv=16.67
0x1BF,   	0x20B,0x280,//CKV width 11.6352 
0x1BF,   	0x20C,0x2C8, 
0x1BF,   	0x20D,0x20A,//MUXSCKH2.7=08  
0x1BF,   	0x20E,0x21B,//muxwCKH2.7=1F
0x1BF,   	0x20F,0x206,//MUXG1CKH2.7=03
0x1BF,   	0x210,0x256,//MUXG2CKH2.7=53
0x1BF,   	0x211,0x200,//92
0x1BF,   	0x212,0x200,//03
0x1BF,   	0x213,0x21E,//?
0x1BF,   	0x214,0x200,//05
0x1BF,   	0x215,0x21A,  
0x1BF,   	0x216,0x205,//07
0x1BF,   	0x217,0x200,
////=,===========,LCD drive timing setting end==================
0x1BF,   	0x218,0x21E,
////=====Gout 1-2,7enable/disable H/L control start============
0x1BF,   	0x219,0x2FF,//03
0x1BF,   	0x21A,0x200,
0x1BF,   	0x21B,0x2FC,//FC
0x1BF,   	0x21C,0x280,//00
0x1BF,   	0x21D,0x200,	
0x1BF,   	0x21E,0x200,	  
0x1BF,   	0x21F,0x277,//00
////=====Gout 1-2,7enable/disable H/L control end============
0x1BF,  	0x220,0x200,//RGB Sequence
0x1BF,  	0x221,0x200,//RGB h scan and v Scan change
0x1BF,  	0x222,0x255,//column inversion
0x1BF,  	0x223,0x20D,//column inversion
0x1BF,  	0x231,0x2A0,
0x1BF,  	0x232,0x200,
0x1BF,  	0x233,0x2B8,
0x1BF,  	0x234,0x2BB,
0x1BF,   	0x235,0x211,
0x1BF,   	0x236,0x202,//GSP START POSITION
0x1BF,   	0x237,0x200,//GSP END POSITION
0x1BF,   	0x238,0x201,
0x1BF,   	0x239,0x20B,
0x1BF,   	0x244,0x208,
0x1BF,   	0x245,0x280,
0x1BF,   	0x246,0x2CC,
0x1BF,   	0x247,0x204,
0x1BF,   	0x248,0x200,
0x1BF,   	0x249,0x200,
0x1BF,   	0x24A,0x201,
0x1BF,   	0x26C,0x203,  
0x1BF,   	0x26D,0x203,
0x1BF,   	0x26E,0x22F,
0x1BF,   	0x243,0x200,
0x1BF,   	0x24B,0x223,
0x1BF,   	0x24C,0x201,
0x1BF,   	0x250,0x223,
0x1BF,   	0x251,0x201,
0x1BF,   	0x258,0x223,
0x1BF,   	0x259,0x201,
0x1BF,   	0x25D,0x223,
0x1BF,   	0x25E,0x201,
0x1BF,   	0x289,0x200,
0x1BF,   	0x28D,0x201,
0x1BF,   	0x28E,0x264,
0x1BF,   	0x28F,0x220,
0x1BF,   	0x297,0x28E,
0x1BF,   	0x282,0x28C,
0x1BF,   	0x283,0x202,
0x1BF,   	0x2BB,0x20A,//VFP
0x1BF,   	0x2BC,0x20A,//VBP
0x1BF,   	0x224,0x225,
0x1BF,   	0x225,0x255,
0x1BF,   	0x226,0x205,
0x1BF,   	0x227,0x223,
0x1BF,   	0x228,0x201,
0x1BF,   	0x229,0x231,
0x1BF,   	0x22A,0x25D,
0x1BF,   	0x22B,0x201,
0x1BF,   	0x22F,0x200,
0x1BF,   	0x230,0x210,
0x1BF,   	0x2A7,0x212,
0x1BF,   	0x22D,0x203,
///IT,L E0xND     


///////////////////4.67BBK USE DEMUX2
// 0x1BF,	0x2FF,	0x205,
// 0x1BF,	0x2FB,	0x201,
// 0x1BF,	0x223,	0x20D,
// 0x1BF,	0x26C,	0x203,
// 0x1BF,	0x26D,	0x203,
// 0x1BF,	0x26F,	0x225,
// 0x1BF,	0x289,	0x201,
//////////////////////////
//*************4.67BBK ID
// 0x1BF,    0x2FF,
	      // 0x201,	
// 0x1BF,    0x2FB,
	      // 0x201,		  
// 0x1BF,    0x223,
	      // 0x24E,

0x311,           
//----------------gamma setting---------------
0x1BF,	0x2FF,	0x200,//CMD1 PAGE0
0x1BF,	0x2FF,	0x201,//CMD2 PAGE0
0x1BF,	0x2FB,	0x201,
//R+
0x1BF, 0x275,0x200,
0x1BF, 0x276,0x200,
0x1BF, 0x277,0x200,
0x1BF, 0x278,0x22C,
0x1BF, 0x279,0x200,
0x1BF, 0x27A,0x25B,
0x1BF, 0x27B,0x200,
0x1BF, 0x27C,0x27B,
0x1BF, 0x27D,0x200,
0x1BF, 0x27E,0x294,
0x1BF, 0x27F,0x200,
0x1BF, 0x280,0x2A9,
0x1BF, 0x281,0x200,
0x1BF, 0x282,0x2BB,
0x1BF, 0x283,0x200,
0x1BF, 0x284,0x2CB,
0x1BF, 0x285,0x200,
0x1BF, 0x286,0x2D9,
0x1BF, 0x287,0x201,
0x1BF, 0x288,0x20A,
0x1BF, 0x289,0x201,
0x1BF, 0x28A,0x230,
0x1BF, 0x28B,0x201,
0x1BF, 0x28C,0x26C,
0x1BF, 0x28D,0x201,
0x1BF, 0x28E,0x29B,
0x1BF, 0x28F,0x201,
0x1BF, 0x290,0x2E5,
0x1BF, 0x291,0x202,
0x1BF, 0x292,0x220,
0x1BF, 0x293,0x202,
0x1BF, 0x294,0x222,
0x1BF, 0x295,0x202,
0x1BF, 0x296,0x25C,
0x1BF, 0x297,0x202,
0x1BF, 0x298,0x29A,
0x1BF, 0x299,0x202,
0x1BF, 0x29A,0x2BD,
0x1BF, 0x29B,0x202,
0x1BF, 0x29C,0x2E9,
0x1BF, 0x29D,0x203,
0x1BF, 0x29E,0x20A,
0x1BF, 0x29F,0x203,
0x1BF, 0x2A0,0x244,
0x1BF, 0x2A2,0x203,
0x1BF, 0x2A3,0x251,
0x1BF, 0x2A4,0x203,
0x1BF, 0x2A5,0x261,
0x1BF, 0x2A6,0x203,
0x1BF, 0x2A7,0x272,
0x1BF, 0x2A9,0x203,
0x1BF, 0x2AA,0x287,
0x1BF, 0x2AB,0x203,
0x1BF, 0x2AC,0x2A0,
0x1BF, 0x2AD,0x203,
0x1BF, 0x2AE,0x2BB,
0x1BF, 0x2AF,0x203,
0x1BF, 0x2B0,0x2CE,
0x1BF, 0x2B1,0x203,
0x1BF, 0x2B2,0x2D0,
//R(-) MCR cmd
0x1BF, 0x2B3,0x200,
0x1BF, 0x2B4,0x200,
0x1BF, 0x2B5,0x200,
0x1BF, 0x2B6,0x22C,
0x1BF, 0x2B7,0x200,
0x1BF, 0x2B8,0x25B,
0x1BF, 0x2B9,0x200,
0x1BF, 0x2BA,0x27B,
0x1BF, 0x2BB,0x200,
0x1BF, 0x2BC,0x294,
0x1BF, 0x2BD,0x200,
0x1BF, 0x2BE,0x2A9,
0x1BF, 0x2BF,0x200,
0x1BF, 0x2C0,0x2BB,
0x1BF, 0x2C1,0x200,
0x1BF, 0x2C2,0x2CB,
0x1BF, 0x2C3,0x200,
0x1BF, 0x2C4,0x2D9,
0x1BF, 0x2C5,0x201,
0x1BF, 0x2C6,0x20A,
0x1BF, 0x2C7,0x201,
0x1BF, 0x2C8,0x230,
0x1BF, 0x2C9,0x201,
0x1BF, 0x2CA,0x26C,
0x1BF, 0x2CB,0x201,
0x1BF, 0x2CC,0x29B,
0x1BF, 0x2CD,0x201,
0x1BF, 0x2CE,0x2E5,
0x1BF, 0x2CF,0x202,
0x1BF, 0x2D0,0x220,
0x1BF, 0x2D1,0x202,
0x1BF, 0x2D2,0x222,
0x1BF, 0x2D3,0x202,
0x1BF, 0x2D4,0x25C,
0x1BF, 0x2D5,0x202,
0x1BF, 0x2D6,0x29A,
0x1BF, 0x2D7,0x202,
0x1BF, 0x2D8,0x2BD,
0x1BF, 0x2D9,0x202,
0x1BF, 0x2DA,0x2E9,
0x1BF, 0x2DB,0x203,
0x1BF, 0x2DC,0x20A,
0x1BF, 0x2DD,0x203,
0x1BF, 0x2DE,0x244,
0x1BF, 0x2DF,0x203,
0x1BF, 0x2E0,0x251,
0x1BF, 0x2E1,0x203,
0x1BF, 0x2E2,0x261,
0x1BF, 0x2E3,0x203,
0x1BF, 0x2E4,0x272,
0x1BF, 0x2E5,0x203,
0x1BF, 0x2E6,0x287,
0x1BF, 0x2E7,0x203,
0x1BF, 0x2E8,0x2A0,
0x1BF, 0x2E9,0x203,
0x1BF, 0x2EA,0x2BB,
0x1BF, 0x2EB,0x203,
0x1BF, 0x2EC,0x2CE,
0x1BF, 0x2ED,0x203,
0x1BF, 0x2EE,0x2D0,
//G(+) MCR cmd
0x1BF, 0x2EF,0x200,
0x1BF, 0x2F0,0x200,
0x1BF, 0x2F1,0x200,
0x1BF, 0x2F2,0x22C,
0x1BF, 0x2F3,0x200,
0x1BF, 0x2F4,0x25B,
0x1BF, 0x2F5,0x200,
0x1BF, 0x2F6,0x27B,
0x1BF, 0x2F7,0x200,
0x1BF, 0x2F8,0x294,
0x1BF, 0x2F9,0x200,
0x1BF, 0x2FA,0x2A9,
//CMD2P1
0x1BF,	0x2FF,	0x200,//CMD1 PAGE0
0x1BF,	0x2FF,	0x202,//CMD2 PAGE1
0x1BF,	0x2FB,	0x201,
//
0x1BF, 0x200,0x200,
0x1BF, 0x201,0x2BB,
0x1BF, 0x202,0x200,
0x1BF, 0x203,0x2CB,
0x1BF, 0x204,0x200,
0x1BF, 0x205,0x2D9,
0x1BF, 0x206,0x201,
0x1BF, 0x207,0x20A,
0x1BF, 0x208,0x201,
0x1BF, 0x209,0x230,
0x1BF, 0x20A,0x201,
0x1BF, 0x20B,0x26C,
0x1BF, 0x20C,0x201,
0x1BF, 0x20D,0x29B,
0x1BF, 0x20E,0x201,
0x1BF, 0x20F,0x2E5,
0x1BF, 0x210,0x202,
0x1BF, 0x211,0x220,
0x1BF, 0x212,0x202,
0x1BF, 0x213,0x222,
0x1BF, 0x214,0x202,
0x1BF, 0x215,0x25C,
0x1BF, 0x216,0x202,
0x1BF, 0x217,0x29A,
0x1BF, 0x218,0x202,
0x1BF, 0x219,0x2BD,
0x1BF, 0x21A,0x202,
0x1BF, 0x21B,0x2E9,
0x1BF, 0x21C,0x203,
0x1BF, 0x21D,0x20A,
0x1BF, 0x21E,0x203,
0x1BF, 0x21F,0x244,
0x1BF, 0x220,0x203,
0x1BF, 0x221,0x251,
0x1BF, 0x222,0x203,
0x1BF, 0x223,0x261,
0x1BF, 0x224,0x203,
0x1BF, 0x225,0x272,
0x1BF, 0x226,0x203,
0x1BF, 0x227,0x287,
0x1BF, 0x228,0x203,
0x1BF, 0x229,0x2A0,
0x1BF, 0x22A,0x203,
0x1BF, 0x22B,0x2BB,
0x1BF, 0x22D,0x203,
0x1BF, 0x22F,0x2CE,
0x1BF, 0x230,0x203,
0x1BF, 0x231,0x2D0,
//G(-) MCR cmd
0x1BF, 0x232,0x200,
0x1BF, 0x233,0x200,
0x1BF, 0x234,0x200,
0x1BF, 0x235,0x22C,
0x1BF, 0x236,0x200,
0x1BF, 0x237,0x25B,
0x1BF, 0x238,0x200,
0x1BF, 0x239,0x27B,
0x1BF, 0x23A,0x200,
0x1BF, 0x23B,0x294,
0x1BF, 0x23D,0x200,
0x1BF, 0x23F,0x2A9,
0x1BF, 0x240,0x200,
0x1BF, 0x241,0x2BB,
0x1BF, 0x242,0x200,
0x1BF, 0x243,0x2CB,
0x1BF, 0x244,0x200,
0x1BF, 0x245,0x2D9,
0x1BF, 0x246,0x201,
0x1BF, 0x247,0x20A,
0x1BF, 0x248,0x201,
0x1BF, 0x249,0x230,
0x1BF, 0x24A,0x201,
0x1BF, 0x24B,0x26C,
0x1BF, 0x24C,0x201,
0x1BF, 0x24D,0x29B,
0x1BF, 0x24E,0x201,
0x1BF, 0x24F,0x2E5,
0x1BF, 0x250,0x202,
0x1BF, 0x251,0x220,
0x1BF, 0x252,0x202,
0x1BF, 0x253,0x222,
0x1BF, 0x254,0x202,
0x1BF, 0x255,0x25C,
0x1BF, 0x256,0x202,
0x1BF, 0x258,0x29A,
0x1BF, 0x259,0x202,
0x1BF, 0x25A,0x2BD,
0x1BF, 0x25B,0x202,
0x1BF, 0x25C,0x2E9,
0x1BF, 0x25D,0x203,
0x1BF, 0x25E,0x20A,
0x1BF, 0x25F,0x203,
0x1BF, 0x260,0x244,
0x1BF, 0x261,0x203,
0x1BF, 0x262,0x251,
0x1BF, 0x263,0x203,
0x1BF, 0x264,0x261,
0x1BF, 0x265,0x203,
0x1BF, 0x266,0x272,
0x1BF, 0x267,0x203,
0x1BF, 0x268,0x287,
0x1BF, 0x269,0x203,
0x1BF, 0x26A,0x2A0,
0x1BF, 0x26B,0x203,
0x1BF, 0x26C,0x2BB,
0x1BF, 0x26D,0x203,
0x1BF, 0x26E,0x2CE,
0x1BF, 0x26F,0x203,
0x1BF, 0x270,0x2D0,
//B(+) MCR cmd
0x1BF, 0x271,0x200,
0x1BF, 0x272,0x200,
0x1BF, 0x273,0x200,
0x1BF, 0x274,0x22C,
0x1BF, 0x275,0x200,
0x1BF, 0x276,0x25B,
0x1BF, 0x277,0x200,
0x1BF, 0x278,0x27B,
0x1BF, 0x279,0x200,
0x1BF, 0x27A,0x294,
0x1BF, 0x27B,0x200,
0x1BF, 0x27C,0x2A9,
0x1BF, 0x27D,0x200,
0x1BF, 0x27E,0x2BB,
0x1BF, 0x27F,0x200,
0x1BF, 0x280,0x2CB,
0x1BF, 0x281,0x200,
0x1BF, 0x282,0x2D9,
0x1BF, 0x283,0x201,
0x1BF, 0x284,0x20A,
0x1BF, 0x285,0x201,
0x1BF, 0x286,0x230,
0x1BF, 0x287,0x201,
0x1BF, 0x288,0x26C,
0x1BF, 0x289,0x201,
0x1BF, 0x28A,0x29B,
0x1BF, 0x28B,0x201,
0x1BF, 0x28C,0x2E5,
0x1BF, 0x28D,0x202,
0x1BF, 0x28E,0x220,
0x1BF, 0x28F,0x202,
0x1BF, 0x290,0x222,
0x1BF, 0x291,0x202,
0x1BF, 0x292,0x25C,
0x1BF, 0x293,0x202,
0x1BF, 0x294,0x29A,
0x1BF, 0x295,0x202,
0x1BF, 0x296,0x2BD,
0x1BF, 0x297,0x202,
0x1BF, 0x298,0x2E9,
0x1BF, 0x299,0x203,
0x1BF, 0x29A,0x20A,
0x1BF, 0x29B,0x203,
0x1BF, 0x29C,0x244,
0x1BF, 0x29D,0x203,
0x1BF, 0x29E,0x251,
0x1BF, 0x29F,0x203,
0x1BF, 0x2A0,0x261,
0x1BF, 0x2A2,0x203,
0x1BF, 0x2A3,0x272,
0x1BF, 0x2A4,0x203,
0x1BF, 0x2A5,0x287,
0x1BF, 0x2A6,0x203,
0x1BF, 0x2A7,0x2A0,
0x1BF, 0x2A9,0x203,
0x1BF, 0x2AA,0x2BB,
0x1BF, 0x2AB,0x203,
0x1BF, 0x2AC,0x2CE,
0x1BF, 0x2AD,0x203,
0x1BF, 0x2AE,0x2D0,
//B(-) MCR cmd
0x1BF, 0x2AF,0x200,
0x1BF, 0x2B0,0x200,
0x1BF, 0x2B1,0x200,
0x1BF, 0x2B2,0x22C,
0x1BF, 0x2B3,0x200,
0x1BF, 0x2B4,0x25B,
0x1BF, 0x2B5,0x200,
0x1BF, 0x2B6,0x27B,
0x1BF, 0x2B7,0x200,
0x1BF, 0x2B8,0x294,
0x1BF, 0x2B9,0x200,
0x1BF, 0x2BA,0x2A9,
0x1BF, 0x2BB,0x200,
0x1BF, 0x2BC,0x2BB,
0x1BF, 0x2BD,0x200,
0x1BF, 0x2BE,0x2CB,
0x1BF, 0x2BF,0x200,
0x1BF, 0x2C0,0x2D9,
0x1BF, 0x2C1,0x201,
0x1BF, 0x2C2,0x20A,
0x1BF, 0x2C3,0x201,
0x1BF, 0x2C4,0x230,
0x1BF, 0x2C5,0x201,
0x1BF, 0x2C6,0x26C,
0x1BF, 0x2C7,0x201,
0x1BF, 0x2C8,0x29B,
0x1BF, 0x2C9,0x201,
0x1BF, 0x2CA,0x2E5,
0x1BF, 0x2CB,0x202,
0x1BF, 0x2CC,0x220,
0x1BF, 0x2CD,0x202,
0x1BF, 0x2CE,0x222,
0x1BF, 0x2CF,0x202,
0x1BF, 0x2D0,0x25C,
0x1BF, 0x2D1,0x202,
0x1BF, 0x2D2,0x29A,
0x1BF, 0x2D3,0x202,
0x1BF, 0x2D4,0x2BD,
0x1BF, 0x2D5,0x202,
0x1BF, 0x2D6,0x2E9,
0x1BF, 0x2D7,0x203,
0x1BF, 0x2D8,0x20A,
0x1BF, 0x2D9,0x203,
0x1BF, 0x2DA,0x244,
0x1BF, 0x2DB,0x203,
0x1BF, 0x2DC,0x251,
0x1BF, 0x2DD,0x203,
0x1BF, 0x2DE,0x261,
0x1BF, 0x2DF,0x203,
0x1BF, 0x2E0,0x272,
0x1BF, 0x2E1,0x203,
0x1BF, 0x2E2,0x287,
0x1BF, 0x2E3,0x203,
0x1BF, 0x2E4,0x2A0,
0x1BF, 0x2E5,0x203,
0x1BF, 0x2E6,0x2BB,
0x1BF, 0x2E7,0x203,
0x1BF, 0x2E8,0x2CE,
0x1BF, 0x2E9,0x203,
0x1BF, 0x2EA,0x2D0,
//----------------gamma end----------------------
//0x3FF 


//=	========BC START  PWM===========================
0x1BC ,  0x202,  	
  		0x200,	  
0x1BF ,  0x2FF,
		0x200,
0x1BF ,  0x251,//brightness value of the display.
		0x2FF,
0x1BF ,  0x253,
		0x22C,
0x1BF ,  0x255,
		0x200,
//=========BC END===========================
//-------------------------------
0x1BF,   0x2FF,
		0x200,
0x1BF ,  0x2FB,
	    0x201, 
0x1BF ,  0x2BA,  
  	    0x203,//03:MIPI 4 lane
0XF00	
};

unsigned short ET2_InitCode[]= 
{	
//=	========BC START  PWM===========================
0x1BC ,  0x202,  	
  		0x200,	  
0x1BF ,  0x2FF,
		0x200,
0x1BF ,  0x251,//brightness value of the display.
		0x2FF,
0x1BF ,  0x253,
		0x22C,
0x1BF ,  0x255,
		0x200,

0XF00	
};

unsigned short RA_InitCode[]= 
{						 
	0x1BC,  0x202, 0x200,	  
	0x1BF,  0x2FF, 0x201,
	
//	0x1BF,  0x247, 0x20F,	
//	0x1BF,  0x248, 0x200,
	0x1BF,  0x249, 0x201,
	0x1BF,  0x24A, 0x201,  
	0XF00	
};


uint16_t gamma[10][64];

