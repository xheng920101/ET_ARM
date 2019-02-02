#include "include.h" 

//Fram Rate£z
#define		SSD_HACT	720
#define		SSD_VACT	1440
#define		SSD_HSA		20  
#define		SSD_HBP		40
#define		SSD_HFP		71
#define		SSD_VSA		1
#define		SSD_VBP		8
#define		SSD_VFP		20

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	SSD_HACT, 						//FPGA_HAC
	SSD_VACT, 						//FPGA_VAC
	SSD_HSA, 							//FPGA_HSA
	SSD_HBP, 							//FPGA_HBP
	SSD_HFP,  						//FPGA_HFP
	SSD_VSA, 							//FPGA_VSA
	(SSD_VBP - SSD_VSA), 	//FPGA_VBP
	SSD_VFP	  						//FPGA_VFP
};

unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
	0X1BA,	0X266,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
//	0X1BA,	0X26E,	0X2C3,		// 24MHz * 113 / 3 = 904Mbps	 135*24/4=810
	0X1BB,	0X20F,	0X200,		//	800 / 2 / 8 / 8 = 6.25MHz
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
	0X1B7,	0X210,	0X205,	//for generate package
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,	//Manufacturer Command Access Protect
	
	0X1BC,	0X202,	0X200,	//NVM reload setting
	0X1BF,	0X2D6,	0X201,
	
//	0X1BC,	0X204,	0X200,
//	0X1BF,	0X2B3,	0X21C,	//Vedio Mode=1*, Command Mode=0*
//									0X200,
//									0X200,

//	0X1BC,	0X224,	0X200,
//	0X1BF,	0X2C1,	0X200,
//									0X261,	
//									0X201,	//bit2-0: 1600*2560=0, 1440*2560=1
//									0X25F,	//bit0: 0=U2D, 1=D2U  
//									0X290,
//									0X2FF,
//									0X251,
//									0X202,
//									0X2FF,
//									0X2FF,
//									0X2FF,
//									0X25D,
//									0X263,
//									0X2AC,
//									0X2B9,
//									0X2FF,
//									0X23F,
//									0X27C,
//									0X241,
//									0X2FD,
//									0X2FF,
//									0X2CB,
//									0X2F8,
//									0X200,
//									0X200,
//									0X200,
//									0X200,					
//									0X200,					
//									0X202,					
//									0X200,					
//									0X222,					
//									0X200,					
//									0X204,					
//									0X2C7,	
//									0X251,	
//						
//	0X1BC,	0X209,	0X200,
//	0X1BF,	0X2C2,	0X201,	//Display Setting 2, PNLSEL=0h:
//									0X20A,
//									0X200,	//NL=A00h: 2560line
//									0X208,	//BP=8h: 8line
//									0X208,	//FP
//									0X200,	//LINEINV£º1-line inversion drive
//									0X200,
//									0X208,	//FP2
//	
//	0X1BC,	0X205,	0X200,
//	0X1BF,	0X2D0,	0X211,
//                  0X21C,	//VGH 9.6V
//                  0X20F,	//VGL -7V
//                  0X2FD,

//	0X1BC,	0X211,	0X200,
//	0X1BF,	0X2D2,	0X25E,
//									0X253,	//VPL 5V
//									0X253,	//VNL	-5V
//									0X233,
//									0X212,	//VSP 4.02V (Abnormal power off) 
//									0X233,
//									0X233,
//									0X233,
//									0X277,
//									0X277,
//									0X233,
//									0X233,
//									0X233,
//									0X200,
//									0X200,
//									0X200,
//	
//	0X1BC,	0X208,	0X200,
//	0X1BF,	0X2D5,	0X206,	//VCOM Setting
//									0X200,
//									0X200,
//									0X201,
//									0X246,	//VPL - (VPL-VNL)*setting value = 5 - (5+5)*0.5470 = -0.47V
//									0X201,
//									0X246,

	0X1B7,	0X250,	0X205,	//for DCS package
	0X1BC,	0X202,	0X200,
	0X1BF,	0X251,	0X2BF, 	//BF = 15mA
	0X1BF,	0X253,	0X22C,
	0X1BF,	0X255,	0X200,
									
	0XF00		
 };

unsigned short ET2_InitCode[]= 
{
	0X1B7,	0X210,	0X205,	//for generate package
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,	//Manufacturer Command Access Protect

	0XF00		
};

unsigned short RA_InitCode[]=
{
	0X1B7,	0X210,	0X205,	//for generate package
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,	//Manufacturer Command Access Protect
	
	0X1BC,	0X205,	0X200,	//FREE RUN
	0X1BF,	0X2E5,	0X201,
									0x23F,
									0x2FF,
									0x210, 
	
	0XF00		
};
