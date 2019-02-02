/*********************************************************************************
 *							Driver IC: R63423
 ********************************************************************************/

#include "include.h" 

#define		SSD_HACT	720
#define		SSD_VACT	2560
#define		SSD_HSA		0x14  
#define		SSD_HBP		0x34
#define		SSD_HFP		0x64  
#define		SSD_VSA		0x02
#define		SSD_VBP		0x08
#define		SSD_VFP		0x08

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	720,	//FPGA_HAC
	2560,	//FPGA_VAC
	0x14,	//FPGA_HSA
	0x20,	//FPGA_HBP
	0x64,	//FPGA_HFP
	0x02,	//FPGA_VSA
	0x06,	//FPGA_VBP
	0x08 	//FPGA_VFP
};


unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
	0X1BA,	0X270,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
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
		//-------------  Display Setting  -------------------------	
	0X1B7,	0X210,	0X205,		//for generate package
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,		//Manufacturer Command Access Protect
	0X355,
        
        0X1BC,	0X202,	0X200,
	0X1BF,	0X2D6,	0X201,	  
	
        0X1BC,	0X204,	0X200,
	0X1BF,	0X2B3,	0X214,		//Interface setting,command or vidoe mode, //Vedio Mode=14, Command Mode=04
	   	   	0X200,
	   	   	0X200,
	   	   	

	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B4,	0X200,		//Interface ID setting,SELDL=3h : DSI Data 4lane,DSIVC=0h : Virtual channel 00
	   	   	
	0X1BC,	0X203,	0X200,
	0X1BF,	0X2B6,	0X23A,		//DSI Control
	   	   	0X2D3,		//bit 6-4:DSI Speed

	0X1BC,	0X204,	0X200,
	0X1BF,	0X2C3,	0X200,		//TPC Sync Control
	   	   	0X200,
	   	   	0X200,

	0X1BC,	0X205,	0X200,
	0X1BF,	0X2C0,	0X200,		//SOUT Slew rate adjustment
	   	   	0X2FF,
	   	   	0X2FF,
	   	   	0X200,
	   	   

	0X1BC,	0X224,	0X200,
	0X1BF,	0X2C1,	0X244,  //1st Display Setting 1,SS1=0h : Horizontal Scan Direction,BLS,BLREV=0h : V255P/N Output
                        0X261,	//2  bit2-0:1600*2560=0, bit2-0:1440*2560=1
	   	   	0X200,	//3  BIT0=0  U2D,   BIT0=1  D2U 	
	   	   	0X2E1,		 
                        0X298,
	   	   	0X251,
	   	   	0X23E,
	   	   	0X2FA,
	   	   	0X2FF,
	   	   	0X247,
	   	   	0X2F8,
	   	   	0X25C,
	   	   	0X263,
	   	   	0X2AC,
	   	   	0X2B9,
	   	   	0X207,
	   	   	0X2E3,
	   	   	0X2FF,
	   	   	0X21F,
	   	   	0X27D,
	   	   	0X222,
	   	   	0X2CC,
	   	   	0X201,
	   	   	0X200,
	   	   	0X200,
            0X200,
	   	   	0X200,					
	   	   	0X200,					
	   	   	0X20A,					
	   	   	0X203,					
	   	   	0X202,					
	   	   	0X202,					
	   	   	0X200,					
	   	   	0X201,	
            0X211,				
	   	   				



	0X1BC,	0X209,	0X200,
	0X1BF,	0X2C2,	0X20D,		//Display Setting 2,PNLSEL=1h : Alternation
	   	   	0X20A,
	   	   	0X200,		//NL=A00h : 2560line
	   	   	0X208,
	   	   	0X208,		//BP=8h : 8line
	   	   	0X2F0,		//FP
	   	   	0X200,
	   	   	0X208,




		//Sorce timing setting	 EQ
	0X1BC,	0X20F,	0X200,
	0X1BF,	0X2C4,	0X270,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X22D,  //6	 GIP (FALLING GND RISING GND)
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,   //9   GIP (FALLING VSP RISING VSP)
	   	   	0X22D,  //10   GIP (FALLING GND RISING GND)
	   	   	0X200,  //11   GIP (FALLING VSN RISING VSN)
	   	   	0X201,
	   	   	0X203,
	   	   	0X201,
	   	   	



	//LTPS Timing Setting
	0X1BC,	0X216,	0X200,		
	0X1BF,	0X2C6,	0X271,		//RTN	  71
	   	   	0X200,		//ST1   00
	   	   	0X208,		//SW1   00
	   	   	0X208,		//ST2   08
	   	   	0X264,		//SW2   66
	   	   	0X200,		//ST3   00
	   	   	0X200,		//SW3   00
	   	   	0X200,		//ST4   00
	   	   	0X200,		//SW4   00
	   	   	0X200,		//ST5   00
	   	   	0X200,		//SW5   00
	   	   	0X200,		//ST6   00
	   	   	0X200,		//SW6   00
	   	   	0X200,		//ST7   00
	   	   	0X200,		//SW7   00
	   	   	0X200,		//ST8      00
	   	   	0X200,		//SW8      00
	   	   	0X209,		//PSWT1 14
	   	   	0X215,		//PSWW1 16
	   	   	0X207,		//PSWG1 07
	   	   	0X271,		//RTN	  71


	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CB,	0X25F,		//1 Panel PIN Control/SOUT Enable setting
	   	   	0X2FC,		//2
	   	        0X23F,		//3
	   	   	0X2FA,		//4
	   	   	        
			0X240,		//5
	   	        0X200,		//6
	   	   	0X200,		//7
	   	        0X202,		//8
	   	   	       
			0X21D,		//9	 Abnormal power off EN
	   	        0X2E0,		//10
                        0X207,     
	                0X2B8,
                  0X2E8,
                  0X200,
                  0X200,

         0X1BC,	0X202,	0X200,
	 0X1BF,	0X2CC,	0X20E,		//Panel Interface Control,LIM=0Eh : Tianma
	//0X1BF,	0X2D2,	0X200,		//Power setting for common

        
        0X1BC,0X20E,0X200,
        0X1BF,0X2D7,0X282,
                    0X2FF,
                    0X221,
                    0X28E,
                    0X28C,
                    0X2F1,
                    0X287,
                    0X23F,
                    0X27E,
                    0X210,
                    0X200,
		                0X200,
                    0X28F,

         0X1BC,0X202,0X200,
         0X1BF,0X2D9,0X200,
                     0X200,


//===========(3) Power Setting (Except Gamma Setting)=============			
	0X1BC,	0X205,	0X200,
	0X1BF,	0X2D0,	0X211,
                        0X214,			 //VGH 8V
                        0X20F,			 //VGL-7V
                        0X233,

        0X1BC,	0X211,	0X200,
     	  0X1BF,	0X2D2,	0X2CD,
                        0X25B,		  //VPL
                        0X25B,		  //VNL	  5B: 5.2	   63: 5.4
                        0X233,
                        0X212,		  //VSP 4.02V (Abnormal power off) 
                        0X233,
                        0X233,
                        0X233,
                        0X277,
                        0X277,
                        0X233,
                        0X233,
                        0X233,
                        0X200,
                        0X200,
                        0X200,



	0X1BC,	0X208,	0X200,
	0X1BF,	0X2D5,	0X206,		//VCOM Setting
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X219,
	   	   	0X201,
	   	   	0X219,

			//////////Gamma setting////////////
	0X1BC,	0X21F,	0X200,
	0X1BF,	0X2C7,
	   	   	0X200,
	   	   	0X210,
	   	   	0X219,
	   	   	0X225,
	   	   	0X234,
	   	   	0X242,
	   	   	0X24C,
	   	   	0X25A,
	   	   	0X23E,
	   	   	0X247,
	   	   	0X253,
	   	   	0X261,
	   	   	0X26A,
	   	   	0X270,
	   	   	0X276,
	   	   	0X200,
	   	   	0X210,
	   	   	0X219,
	   	   	0X225,
	   	   	0X234,
	   	   	0X242,
	   	   	0X24C,
	   	   	0X25A,
	   	   	0X23E,
	   	   	0X247,
	   	   	0X253,
	   	   	0X261,
	   	   	0X26A,
	   	   	0X270,
	   	   	0X276,

//  0X1BC,	0X202,	0X200,		//GVDD TEST
//	0X1BF,	0X2B0,	0X200,		
	0X355,
//  0X1BC,	0X204,	0X200,  //VPL\VNL TEST
//	0X1BF,	0X2F0,	0X2FD,		
//	   	   	        0X202,
//	   	   	        0X230,
   										
	//Test image generator
//	0X1BC,	0X205,	0X200,	   //FREE RUN
//	0X1BF,	0X2E5,	
//			0X201,
//			0x23F,
//			0x2FF,
//			0x210,    	   	       	   	   	 
		
	0XF00				
 };

unsigned short IDReg[]= 
{
	0XF00
};

unsigned short GAMMAReg[]= 
{
	0XF00
};

unsigned short ET2_InitCode[]= 
{


	0X1B7,	0X210,	0X205,		//for generate package

	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,		//Manufacturer Command Access Protect
	0X355,

	0XF00	
};

unsigned short RA_InitCode[]= 
{


	0X1B7,	0X210,	0X205,		//for generate package

	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,		//Manufacturer Command Access Protect
	0X355,

	//Test image generator
	0X1BC,	0X205,	0X200,	   //FREE RUN
	0X1BF,	0X2E5,	
			0X201,
            0x200,
			0x2F9,
			0x210,   

	0XF00	
};

unsigned short gamma[10][64]=  {
//-------------------------------- gamma0 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma1 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma2 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma3 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma4 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma5 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma6 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma7 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma8 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},

//-------------------------------- gamma9 --------------------------------
{
	0x1B7,	0x219,	0x205,
	0X1BC,	0X21E,	0X200,
	0X1BF,	0X2C7,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,
	   	   	0X201,
	   	   	0X211,
	   	   	0X218,
	   	   	0X220,
	   	   	0X22B,
	   	   	0X237,
	   	   	0X241,
	   	   	0X24F,
	   	   	0X236,
	   	   	0X240,
	   	   	0X24D,
	   	   	0X25B,
	   	   	0X26A,
	   	   	0X273,
	   	   	0X27C,


	0X1BC,	0X213,	0X200,
	0X1BF,	0X2C8,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X2FC,
	   	   	0X208,
	   	   	0X2E4,
	   	   	0X200,
	   	   	0X200,
	   	   	0X203,
	   	   	0X2FF,
	   	   	0X204,
	   	   	0X2FC,
	   	   	0X200,
	   	   	0X200,
	   	   	0X209,
	   	   	0X206,
	   	   	0X20B,
	   	   	0X2CA,
	   	   	0X200,
	
	0XF00,
},
};

