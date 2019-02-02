/*********************************************************************************
 *							Driver IC: OTM1901A
 ********************************************************************************/

#include "include.h" 

#define		SSD_HACT	1080
#define		SSD_VACT	1920
#define		SSD_HSA		0x14   //0x14  
#define		SSD_HBP		0x19   //0x19
#define		SSD_HFP		0x18   //0x18  
#define		SSD_VSA		0x02
#define		SSD_VBP		0x08
#define		SSD_VFP		0x0E

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	1080,	//FPGA_HAC
	1920,	//FPGA_VAC
	0x14,	//FPGA_HSA
	0x1F,	//FPGA_HBP 0x17 
	0x1F,	//FPGA_HFP 0x18
	0x02,	//FPGA_VSA
	0x06,	//FPGA_VBP
	0x0E 	//FPGA_VFP                                                         
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
0x1BA,    0x24B,	//24B	[7:0]  NS=75  MAX=96  PLL=24M*96/2=1.152GHz 50 64  4B
          0x2C2,  //2C2	[12:8] MS=2  so PLL=24M*75/2=900M		        C2 C3  C2
	
0x1BB,    0x20A,	//0F
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
	0X1B1,	(0X200 | SSD_HSA),	(0X200 | SSD_VSA),		//VSA,HSA
	0X1B2,	(0X200 | SSD_HBP),	(0X200 | SSD_VBP),		//VBP,HBP	
	0X1B3,	(0X200 | SSD_HFP),	(0X200 | SSD_VFP),		//VFP,HFP
	0X1B4,	(0X200 |(SSD_HACT & 0X00FF)),	(0X200 |((SSD_HACT >> 8) & 0X00FF)),		//HACT,	
	0X1B5,	(0X200 |(SSD_VACT & 0X00FF)),	(0X200 |((SSD_VACT >> 8) & 0X00FF)),		//VACT,	

0X1B6,	0X207,	0X200,		//Non burst mode with sync events
//	0X1B6,	0X20b,	0X200,		//Burst mode
//////////////////////////////
0x1CB,   	0x210, //delay adjustment register30 //Tclk-post	 
	        0x205, //Tclk-per	 

///////////////////////////////////////////////////FHD OK
0x1B7,   	0x210, //10 configuration register0 	
	        0x205, //05 long packet enable,short packet?

0x1BD,   	0x200, //packet size control register 20 	 
	        0x200, //0x2C,0X3C
0XF00
};

unsigned short ET1_InitCode[]= 
{
//-------------  Display Setting  -------------------------	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x200,
   0x1BC,   0x204 , 0x200,
   0x1BF,   0x2FF,  0x219,
                    0x201, 
                    0x201,
	                //0x200,         //Enable command2&enable shift function
//	////////////////ID Setting/////////////////
   0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X200,	0X200,
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X2D0,	0X242,  //41更改为42  
	
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X200,	0X200,
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X2D1,	0X203,     //02更改为03
	
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X200,	0X201,
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X2D1,	0X204,     //03更改为04 
	
0x310,
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x280,         //set parameter shift value
   0x1BC,   0x203,  0x200,
   0x1BF,   0x2FF,  0x219,
                    0x201,         //enbale orise command

	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X200,	0X202,
	 0X1BC,	  0X202,	0X200,
	 0X1BF,	  0X2FF,	0X203,           

0x310,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X200,

0x310,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X202,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2FF,	0X201,

0x310,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X21C,	0X233,      //Cannot OTP this register

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X294,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X284,	    // C194=07--> Disable spread spectrum function
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X298,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X274,	

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,		  //00 //modsel_tcon_clk_video

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A1,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X2ED,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A2,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X210,	   // Video mode Sync signal control

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A3,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A4,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A5,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A6,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A7,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,   //Orise suggest to change the value form 02->00;--20150201

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X207,	0X200,
	0X1BF,	0X2C0,	0X200,	0X22F,	0X200,	0X200,	0X200,	0X201,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2C0,
	0X1BC,	0X207,	0X200,
	0X1BF,	0X2C0,	0X200,	0X22F,	0X200,	0X200,	0X200,	0X201,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X29A,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X21E,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2AC,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X206,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2DC,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X206,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X281,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2A5,	0X206,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2D0,
	0X1BC,	0X205,	0X200,
	0X1BF,	0X2F5,	0X200,	0X200,	0X200,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B3,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X2CC,	  // CC=Pixel based colunm inversion   00=dot-based column

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2BC,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X200,	  // For inversion type setting

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X284,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C4,	0X222,	  //	20=2:6 demux   00=1:3demux	
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X282,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C4,	0X2F0,	  //	ORISE GAI

		//[Setting by Panel]
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X293,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X214,		//28--VGH=10V	 14--VGH=8V

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X297,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X214,		//28--VGHO=10V	 14--VGHO=8V

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A3,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X214,		//28--VGH=10V    14--VGH=8V  when idle mode

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X295,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X21E,		//VGL=-7V

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X299,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X21E,		//VGLO=-7V	1E=-7V

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A5,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X21E,		//VGL=-7V  when idle mode

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X203,	0X200,
	0X1BF,	0X2D8,	0X21E,	    //GVDDP 1B=4.2V  1A=4.15V   2B=5V   1E=4.35V
	                0X21E,		  //GVDDN   

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X20F,	0X200,
	0X1BF,	0X2C0,	0X200,	    // RTN[15:0]
	                0X27A,	    // RTN[7:0] default=0x277
					        0X200,	    // VFP[15:8]
					        0X208,	    // VFP[7:0]
					        0X208,	    // VBP[7:0]
					        0X200,	
					        0X27A,	
					        0X208,	
					        0X208,	
					        0X200,	
					        0X27A,	
					        0X200,	
					        0X208,	
					        0X208,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2F0,
	0X1BC,	0X207,	0X200,
	0X1BF,	0X2C3,	0X222,	0X200,	0X200,	0X200,	0X200,	0X20C,

 ///////////////CKH EQ Setting////////////////////
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2F7,
	0X1BC,	0X205,	0X200,
	0X1BF,	0X2C3,	0X204,  // GND Rising	
	                0X216,	// GND falling
					        0X200,  // AVDD width
					        0X200,  // AVEE width

 ///////////////CKH and CKV Slew rate Setting///////
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2F2,
	0X1BC,	0X204,	0X200,
	0X1BF,	0X2C1,	0X200,  // 0x00=disable slew rate control 0x80=enable slew rate control	
	                0X200,	// 0x00(slow)~0x3F(Fast) to adjust falling edge slew rate level
					        0X200,	// 0x00(slow)~0x3F(Fast) to adjust  rising edge slew rate level
															
///////////////////////////////////////////////////	

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X208,	0X200,
	0X1BF,	0X2C0,	0X200,	 //CKH
	                0X200,	 //
					        0X210,	 //0x210
					        0X200,	 //set non overlap after PCG
					        0X202,	 //set non overlap time befor CKH rising edge
					        0X21A,	 //set CKH width
					        0X205,	 //set non overlap time aftre CKH falling time


	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2D0,
	0X1BC,	0X208,	0X200,
	0X1BF,	0X2C0,	0X200,   //CKH for video mode
	                0X200,	
					        0X210,	 //0x210
					        0X200,	
					        0X202,	
					        0X21A,	
					        0X205,	


	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2C2,	0X282,     // specify the VST1 shift position	   case1
	                0X201,	   // D[3:0]specify the VST1 width/length of pulse (unit:1 line)
					        0X200,	   // set the delay of VST1 rising edge of the pulse
					        0X20E,	   // set the shift of early and delay on the VST1 falling edge of the pluse
					        0X283,	   // specify the VST2 shift position	
					        0X200,	   // D[3:0]specify the VST2 width/length of pulse (unit:1 line)
					        0X21D,	   // set the delay of VST2 rising edge of the pulse
					        0X2BA,	   // set the shift of early and delay on the VST2 falling edge of the pluse

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X20D,	0X200,
	0X1BF,	0X2C3,	0X282,	   //CKV1	 set start position case1
	                0X200,	   //set end position
					        0X203,	   //Period
					        0X200,	   //[7:4]start pull high positon	[3:0] finish pull high positon
					        0X212,	   // set delay	of rising edge (unit:mclk)
					        0X280,	   // set delay or early of falling edge (unit:mclk)
					        0X281,	   //CKV2  start position  
					        0X200,	   //set end position
					        0X203,	   //Period
					        0X200,	   //[7:4]start pull high positon	[3:0] finish pull high positon
					        0X212,	   // set delay	of rising edge	(unit:mclk)
					        0X280,	   // set delay or early of falling edge (unit:mclk)

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X20D,	0X200,
	0X1BF,	0X2C3,	0X280,	   //CKV3
	                0X200,	
					        0X203,	
					        0X200,	
					        0X212,	
					        0X280,	
					        0X201,	   //CKV4
					        0X201,	
					        0X203,	
					        0X200,	
					        0X212,	
					        0X280,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CC,	0X209,	0X20A,	0X211,	0X212,	0X213,	0X214,	0X215,	0X216,	0X217,	0X218,	0X228,	0X228,	0X228,	0X228,	0X228,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CC,	0X209,	0X20A,	0X214,	0X213,	0X212,	0X211,	0X215,	0X216,	0X217,	0X218,	0X228,	0X228,	0X228,	0X228,	0X228,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CC,	0X21D,	0X21E,	0X21F,	0X219,	0X21A,	0X21B,	0X21C,	0X220,	0X221,	0X222,	0X223,	0X224,	0X225,	0X226,	0X227,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B0,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2CC,	0X201,	0X202,	0X203,	0X205,	0X206,	0X207,	0X204,	0X208,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2C0,
	0X1BC,	0X20D,	0X200,
	0X1BF,	0X2CC,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2D0,
	0X1BC,	0X20D,	0X200,
	0X1BF,	0X2CC,	0X2FF,	0X20F,	0X200,	0X200,	0X205,	0X200,	0X200,	0X200,	0X233,	0X203,	0X200,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CB,	0X200,	0X200,	0X2FF,	0X2FF,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CB,	0X200,	0X200,	0X2FF,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CB,	0X2D5,	0X22A,	0X2D5,	0X2D5,	0X2D5,	0X2D5,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B0,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CB,	0X200,	0X205,	0X205,	0X205,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2C0,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2CB,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X277,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2D0,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2CB,	0X200,	0X200,	0X200,	0X200,	0X200,	0X200,	0X277,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2E0,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2CB,	0X231,	0X231,	0X231,	0X201,	0X201,	0X201,	0X277,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2F0,
	0X1BC,	0X209,	0X200,
	0X1BF,	0X2CB,	0X211,	0X211,	0X211,	0X211,	0X211,	0X211,	0X277,	0X277,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CD,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X204,	0X206,	0X23F,	0X202,	0X211,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X20C,	0X200,
	0X1BF,	0X2CD,	0X212,	0X201,	0X23F,	0X224,	0X223,	0X222,	0X221,	0X220,	0X21F,	0X225,	0X226,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X210,	0X200,
	0X1BF,	0X2CD,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X23F,	0X203,	0X205,	0X23F,	0X202,	0X211,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B0,
	0X1BC,	0X20C,	0X200,
	0X1BF,	0X2CD,	0X212,	0X201,	0X23F,	0X224,	0X223,	0X222,	0X221,	0X220,	0X21F,	0X225,	0X226,


	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X203,	0X200,
	0X1BF,	0X2D9,	0X200,	
	                0X2B2,		// B2=VCOMDC=-0.22V  
									
									
									

									
//==============Gamma setting   0820=================//									
0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E1,
//0x202,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,
//0x202,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,//orise调整
0x202,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2

0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E2,
//0x200,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,
//0x200,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,
0x200,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2

0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E3,
//0x202,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,
//0x202,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,
0x202,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2

0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E4,
//0x200,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,
//0x200,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,
0x200,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2

0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E5,
//0x202,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,
//0x202,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,
0x202,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2

0x1BC,0x202,0x200,
0x1BF,0x200,0x200,
0x1BC,0x219,0x200,
0x1BF,0x2E6,
//0x200,0x20e,0x217,0x223,0x22c,0x233,0x23f,0x251,0x25a,0x26e,0x27b,0x286,0x272,0x26a,0x264,0x253,0x241,0x231,0x226,0x220,0x218,0x210,0x20e,0x20b,								
//0x200,0x213,0x219,0x226,0x22f,0x236,0x242,0x253,0x25d,0x270,0x27d,0x287,0x271,0x269,0x263,0x252,0x241,0x231,0x226,0x220,0x219,0x20d,0x20c,0x20b,
0x200,0x20D,0x215,0x222,0x22B,0x232,0x23E,0x250,0x25A,0x26D,0x27B,0x284,0x273,0x26C,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20E,0x20B,//orise调2
						
// //==============Gamma setting   0807=================//
// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E1,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,

// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E2,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,

// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E3,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,

// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E4,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,

// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E5,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,

// 0x1BC,0x202,0x200,
// 0x1BF,0x200,0x200,
// 0x1BC,0x219,0x200,
// 0x1BF,0x2E6,
// 0x200,0x20d,0x215,0x222,0x22b,0x232,0x23e,0x250,0x25a,0x26d,0x27b,0x284,0x273,0x26c,0x265,0x254,0x242,0x232,0x227,0x220,0x219,0x210,0x20e,0x20b,


//==============Gamma setting   original=================//
 /* 0x1BC,  0x202,  0x200,
  0x1BF,  0x200,  0x200,
  0x1BC,  0x219,  0x200,
  0x1BF,  0x2E1,
  0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,

	0x1BC,  0x202,  0x200,
	0x1BF,  0x200,  0x200,
	0x1BC,  0x219,  0x200,
	0x1BF,  0x2E2,
	0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,

	0x1BC,  0x202,  0x200,
	0x1BF,  0x200,  0x200,
	0x1BC,  0x219,  0x200,
	0x1BF,  0x2E3,
	0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,

	0x1BC,  0x202,  0x200,
	0x1BF,  0x200,  0x200,
	0x1BC,  0x219,  0x200,
	0x1BF,  0x2E4,																																  
	0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,

	0x1BC,  0x202,  0x200,
	0x1BF,  0x200,  0x200,
	0x1BC,  0x219,  0x200,
	0x1BF,  0x2E5,
	0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,

	0x1BC,  0x202,  0x200,
	0x1BF,  0x200,  0x200,
	0x1BC,  0x219,  0x200,
	0x1BF,  0x2E6,
	0x200,0x210,0x218,0x224,0x22d,0x233,0x23f,0x24e,0x259,0x26b,0x278,0x281,0x276,0x26f,0x268,0x257,0x244,0x234,0x229,0x222,0x21b,0x213,0x211,0x203,
*/
/*
/////////////////Digital gamma setting/////////
	0x1BC,   	0x202,			0x200, //Digital gamma R
	0x1BF,	  0x200,	    0x200,									
	0x1BC,   	0x222,			0x200, 
	0x1BF,    0x2EC,
	0x240,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x234,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x204,


	0x1BC,   	0x202,			0x200, //Digital gamma G
	0x1BF,	  0x200,	    0x200,									
	0x1BC,   	0x222,			0x200, 
	0x1BF,    0x2ED,
	0x240,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x243,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x234,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x204,

			
	0x1BC,   	0x202,			0x200, //Digital gamma B
	0x1BF,	  0x200,	    0x200,									
	0x1BC,   	0x222,			0x200, 
	0x1BF,    0x2EE,
	0x230,0x232,0x233,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x244,0x20c,
	*/

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X292,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2E9,	0x200,    // Orise suggest add this register --20150201

/////////////////////WA Setting//////////////////////////////

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X280,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x283,0x281,0x281,0x281,0x281,0x281,0x282,0x283,0x285,0x287,0x28e,0x295,  

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X290,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x202,0x202,0x202,0x204,0x205,0x206,0x208,0x20a,0x20a,0x20c,0x21c,0x22c,  

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2A0,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x285,0x284,0x285,0x287,0x28a,0x28d,0x291,0x295,0x299,0x2a2,0x2ad,0x2b7,  

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2B0,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x203,0x204,0x206,0x208,0x20c,0x210,0x213,0x217,0x21a,0x22c,0x247,0x262,  

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2C0,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x281,0x201,0x202,0x204,0x206,0x208,0x20a,0x20c,0x20c,0x20c,0x22a,0x249,   

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2D0,
//	0X1BC,	0X20D,	0X200,
//	0X1BF,	0X2D8,	0x200,0x200,0x200,0x200,0x200,0x280,0x281,0x281,0x281,0x281,0x286,0x28b, 

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2C0,
//	0X1BC,	0X205,	0X200,
//	0X1BF,	0X2D7,	0x242,0x280,0x2C0,0X2FF,
////0X1BC,	0X202,	0X200,
////0X1BF,	0X200,	0X2C0,
////0X1BC,	0X202,	0X200,
////0X1BF,	0X2D7,	0x200,

//	0x30A,
//	0x1BC, 0x202, 0x200 ,
//	0x1BF, 0x284, 0x280,	 
	 ////////////////////////////////////
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2AD,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2F5,	0X215, 

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2BD,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2F5,	0X215,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2D7,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2F5,	0X215,

////////////////Abnormal power off setting///////////////

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X282,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2A5,	0X21F,   // for abnormal power off detect mode  1F for IOVCC or VSP or VSN

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2C2,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X21C,   // Detect IOVCC voltage setting  10=1.0v 14=1.1v 18=1.2v 1C=1.3v

///////////////////Reset setting///////////////////////////

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X289,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2F3,	0X25A, 	  //5A--> reset-->power off sequence. Default=00 reset --> sleep in sequence

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X290,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2F3,	0X201,    // For Reset to LVD sequence.  For Cut V1.2
	
///////////////Free Run mode///////////////////

//	0X1BC,	0X202,	0X200,	 // Must set in orise mode.
//	0X1BF,	0X200,	0X2A0,
//	0X1BC,	0X20B,	0X200,
//	0X1BF,	0X2F6,	0x28F, 0x201, 0x223,0x245, 0x267, 0x289, 0x2AB, 0x2CD,0x2EF, 0x25A,

  
///////////////BIST Mode/////////////////////

///////////enable BIST mode///////////

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2A0,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2C1,	0X280,
//
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X290,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2F6,	0x214,  // select different pattern
//
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X200,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2FB,	0X201,
//
//    0x3FF,	
		                
///////////Disable BIST mode///////////

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X290,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2F6,	0X200,
//
// 	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2A0,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2C1,	0X2E0,
//
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X200,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2FB,	0X201,
//	
//	0x3FF,

///////////////reduce power solution ///////////////
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X280,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C4,	0X216, // source driver bias  default =18，有原来的12更改为16，解决灰阶过度不均

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X29D,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X288, // Gamma bias  and Analog bias   default =88

 	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X29B,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X244, // VGH pump clk rate

 	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X29C,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X240, // VGL pump clk rate

  0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X292,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X2A0, // VGH pump ratio

  0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X294,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C5,	0X290, // VGL pump ratio
 
/////////////////Scan direction control/////// 

  0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B4,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C0,	0X280,    // 80 for normal scan, 90 for reverse scan

/////////////////Sleep in sequence to LVD sequence 0821//////////////////
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2CD,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2F5, 0X219,     
	 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2C1,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2F5, 0X215,
	 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2C3,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2F5, 0X215,
	 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2DB,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2F5, 0X219, 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X28D,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2F5, 0X220,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2F5,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2C1, 0X240,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2B9,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2C0, 0X211,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2A2,
	0X1BC, 0X205, 0X200,
	0X1BF, 0X2CB, 0X2FD, 0X2FD, 0X2FD, 0X2FD, //CKV1234=0xD5->0xFD,VGHO IN SLEEP
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2A0,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2CB, 0X2FD,// VST=0xD5->0xFD,STV VGHO IN SLEEP
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2A1,
	0X1BC, 0X202, 0X200,
	0X1BF, 0X2CB, 0X2FE,// 2A->FE,GRESET=VGHO IN SLEEP
	 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2E0,
	0X1BC, 0X204, 0X200,
	0X1BF, 0X2CB, 0X233, 0X233, 0X233, //0x31->0x33,CKH=VGHO IN SLEEP
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2F0,
	0X1BC, 0X204, 0X200,
	0X1BF, 0X2CB, 0X231, 0X231, 0X231, //0x11->0x31,CKH=VGHO IN SLEEP
	 
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2E3,
	0X1BC, 0X204, 0X200,
	0X1BF, 0X2CB, 0X200, 0X200, 0X200, //0x01->0x00,XCKH=VGLO IN SLEEP
	0X1BC, 0X202, 0X200,
	0X1BF, 0X200, 0X2F3,
	0X1BC, 0X204, 0X200,
	0X1BF, 0X2CB, 0X201, 0X201, 0X201, //0x11->0x01,XCKH=VGLO IN SLEEP

//////////////////TE Setting///////////////////
//0x30A,
//0x1BC, 0x202, 0x200,
//0x1BF, 0x235, 0x201,        //35=00 frame mode  35=01 line mode

////////////////Deep standby mode///////////////////

//0x30A,
//	0X1BC,	0X202,	0X200,	
//	0X1BF,	0X200,	0X200,
//	0X1BC,	0X205,	0X200,
//	0X1BF,	0X2F7,	0X25A,
//	                0x2A5,
//					0x219,
//					0x201,        // must be sent in sleepin mode, and IC only can be recovered by hardware reset.

////////////////CABC Setting/////////////////
	0x30A,
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x251, 0x2FF,	  // Adjust the  brightness	of BL
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x253, 0x22C,	  
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x255, 0x200 ,	  //  00= off 01=low 10=medium(still picture) 11=High(moving image)

  0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2B0,
	0X1BC,	0X205,	0X200,
	0X1BF,	0X2CA,	0X210,   //PWM_DEF_NORM[7:0] adjust PWM pulse frequency in normal mode	 default=10
	                0X220,	 //PWM_DEF_IDLE[7:0] adjust PWM pulse frequency in idle mode
					        0X25F,	 //BC_DIM_FRM[4:0]	 set PWM BC dimming frame number. please follow default setting.
				        	0X240, 	 //PWM_FREQ_SEL  select PWM adjustment frequency
									

////////////sleep out and display on///////////
 
//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X211,
//	0X3E1,

//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X229,
//	0X3FF,

////======================== End: LCD setting =======================================================//
//////////////////set 2828 to High speed mode///////////

//  0X3FF,
//	0X1B7,	0X279, 0X205,	
//	0X3FF,
			
	0XF00		
 };

unsigned short ET2_InitCode[]= 
{
		//-------------  Display Setting  -------------------------	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x200,
   0x1BC,   0x204 , 0x200,
   0x1BF,   0x2FF,  0x219,
                    0x201, 
                    0x201,
	                //0x200,      //Enable command2&enable shift function

   0x310,
	
   0x1BC,   0x202,  0x200,
   0x1BF,   0x200,  0x280,     //set parameter shift value
   0x1BC,   0x203,  0x200,
   0x1BF,   0x2FF,  0x219,
                    0x201,     //enbale orise command

  0x310,

////============================================
//// For reload OTP
////===========================================

//  0x340,

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X2DA,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2F5,	0X221,

//  0x310,

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X200,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2FB,	0X200,

//  0x310,

//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X200,	0X200,
//	0X1BC,	0X202,	0X200,
//	0X1BF,	0X2EB,	0X210,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X21C,	0X233,      //Cannot OTP this register

  0x310, 
	 
//==========PWM setting==================//
  0x30A,
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x251, 0x2FF,	      // Adjust the  brightness	of BL
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x253, 0x22C,	  
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x255, 0x200 ,	      //  00= off 01=low 10=medium(still picture) 11=High(moving image)	


//==========sleep out and display on=====//

//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X211,
//	0X3E1,

//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X229,
//	0X3FF,

//===========set 2828 to High speed mode===//
//  0X3FF,
//	0X1B7,	0X219, 0X205,	
//	0X3FF,

	0XF00
	
};

unsigned short RA_InitCode[]=
{
//-------------  Display Setting  -------------------------	
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x200,
  0x1BC,   0x204 , 0x200,
  0x1BF,   0x2FF,  0x219,
                    0x201, 
                    0x201,
	                //0x200,       //Enable command2&enable shift function

  0x310,
  0x1BC,   0x202,  0x200,
  0x1BF,   0x200,  0x280,         //set parameter shift value
  0x1BC,   0x203,  0x200,
  0x1BF,   0x2FF,  0x219,
                    0x201,         //enbale orise command

////============================================
//// For reload OTP
////===========================================

//  0x340,

////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X200,	0X2DA,
////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X2F5,	0X221,

////  0x310,

////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X200,	0X200,
////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X2FB,	0X200,

////  0x310,

////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X200,	0X200,
////	0X1BC,	0X202,	0X200,
////	0X1BF,	0X2EB,	0X210,

  0x310,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X21C,	0X233,      //Cannot OTP this register
	
	
	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X2A2,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X2C1,	0X200,      // must set this register in RA intial code.
	
	0X3FF,
	
//==========Free Run Mode================//
	0X1BC,	0X202,	0X200,	 // Must set in orise mode.
	0X1BF,	0X200,	0X2A0,
	0X1BC,	0X20B,	0X200,
	0X1BF,	0X2F6,	0x28F, 0x201, 0x223,0x245, 0x267, 0x289, 0x2AB, 0x2CD,0x2EF, 0x25A,

	0X1BC,	0X202,	0X200,
	0X1BF,	0X200,	0X200,
	0X1BC,	0X202,	0X200,
	0X1BF,	0X21C,	0X233,
	0X3FF,
	
//==========PWM setting==================//
	0x30A,
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x251, 0x2FF,	  // Adjust the  brightness	of BL
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x253, 0x22C,	  
	0x1BC, 0x202, 0x200 ,
	0x1BF, 0x255, 0x200 ,	  //  00= off 01=low 10=medium(still picture) 11=High(moving image)


////==========sleep out and display on=====//

//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X211,
//	0X3E1,

//	0X1BC,	0X201, 0X200,
//	0X1BF,	0X229,
//	0X3FF,

////========set 2828 to High speed mode===//
//  0X3FF,
//	0X1B7,	0X219, 0X205,	
//	0X3FF,

	0XF00	
};

uint16_t gamma[10][64];
