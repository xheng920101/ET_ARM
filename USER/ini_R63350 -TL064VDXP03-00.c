/*********************************************************************************
 *							Driver IC: R63350
 ********************************************************************************/

#include "include.h" 

#define		SSD_HACT	1080
#define		SSD_VACT	1920
#define		SSD_HSA		0x14  
#define		SSD_HBP		0x19
#define		SSD_HFP		0x28//18  
#define		SSD_VSA		0x02
#define		SSD_VBP		0x06
#define		SSD_VFP		0x0E

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	1080,	//FPGA_HAC
	1920,	//FPGA_VAC
	0x14,	//FPGA_HSA
	0x19,	//FPGA_HBP
	0x28,	//FPGA_HFP //18
	0x02,	//FPGA_VSA
	0x04,	//FPGA_VBP
	0x0E 	//FPGA_VFP
};

unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
	0X1BA,	0X266,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
//	0X1BA,	0X296,	0X2C4,		// 24MHz * 150 / 4 = 900Mbps
	0X1BB,	0X20A,	0X200,		//	900 / 11 / 8 = 10.23MHz, RA use 0F.
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
	0X1B7,	0X210,	0X205,
//Manufacturer Command Access Protect
	0X1BC,	0X202,
	   	   	0X200,
	0X1BF,	0X2B0,
	   	   	0X204,
//NVM reload setting
	0X1BC,	0X202,
	   	   	0X200,
	0X1BF,	0X2D6,
	   	   	0X201,
////Deep Standby mode setting (Must be already in sleep in mode.)
//	0X1BC,	0X202,
//	   	   	0X200,
//	0X1BF,	0X2B1,
//	   	   	0X201,
////set DDB write control
//	0x1BC,	0x20B,
//	        0x200,
//	0x1BF,	0x2E1,
//	   	   	0x203,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x200,	// OTP times
//	   	   	0x200,
////read ID code
//	0x1BC,	0x207,
//	   	   	0x200,
//	0x1BF,	0x2E2,
//	   	   	0x201,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x230,
//	   	   	0x200,
//	   	   	0x200,
//interface setting video mode
	0X1BC,	0X207,
	   	   	0X200,
	0X1BF,	0X2B3,
	   	   	0X214,	// Assignment of Display Mode and Based signals, DSI-video through mode.
	   	   	0X200,
	   	   	0X200,	// Interval of TE signal output for every frame.
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
//interface ID setting DSI_4Lane
	0X1BC,	0X203,
	   	   	0X200,
	0X1BF,	0X2B4,
	   	   	0X20C,	// MIPI 4lane
	   	   	0X200,
//DSI_control
	0X1BC,	0X204,
	   	   	0X200,
	0X1BF,	0X2B6,
	   	   	0X24A,	// LP mode data transmission = fDSICLK/24 = 450/24 = 18.75MHz (data sheet error)
	   	   	0X2DB,	// 740Mbps ¡Ü DSI DPHY operating frequency ¡Ü 1000Mbps 
	   	   	0X216,	// MIPI time out function! Function on as a default value, MIPI stop detecting time set as 125us.
//Checksum and ECC error count
	0X1BC,	0X202,
	   	   	0X200,
	0X1BF,	0X2B7,
	   	   	0X200,	// DSI Error count not clear, for what????

//SOUT Slew rate adjustment
	0X1BC,	0X202,
	   	   	0X200,
	0X1BF,	0X2C0,
	   	   	0X200,	// SOUTTF2 D[7:6], SOUTTF1 D[3:2]: CKH falling edge; SOUTTR2 D[5:4], SOUTTR1 D[1:0]: CKH rising edge; 1: right CKH, 2: left CKH; orig: 11h
//Display setting 1
	0X1BC,	0X224,
	   	   	0X200,
	0X1BF,	0X2C1,
	   	   	0X284,	// Source output direction is Sn-->S1. channing modify: 04h-->84h
	   	   	0X260,
	   	   	0X200,
	   	   	0X2FF,
	   	   	0X28F,
	   	   	0X2F2,
	   	   	0X2D1,
	   	   	0X231,
	   	   	0X2E1,
	   	   	0X247,
	   	   	0X2F8,
	   	   	0X25C,
	   	   	0X263,
	   	   	0X2AC,
	   	   	0X2B9,
	   	   	0X207,
	   	   	0X2E3,
	   	   	0X207,
	   	   	0X2E6,
	   	   	0X2A0,	// ??? R63315: FC
	   	   	0X24F,
	   	   	0X2C4,
	   	   	0X2FF,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X26A,	// ??? R63315: 68
	   	   	0X201,
	   	   	0X202,	// modified by channing, 00h-->02h ???try!
	   	   	0X220,	// ??? R63315: 22
	   	   	0X200,
	   	   	0X201,
	   	   	0X211,
//Sub pix inversion 0x30
	0X1BC,	0X209,
	   	   	0X200,
	0X1BF,	0X2C2,
	   	   	0X231,	// PNLSEL D[1:0]=1h, Sub-pixel inversion (Alternation).(Refer to Appendix data sheet.)
	   	   	0X2F7,	// 07 dot-inversion, F7 column-inversion
	   	   	0X280,	// NL[10:0]=782h, 780h=1920.
	   	   	0X206,		//VBP
	   	   	0X20E,		//VFP
	   	   	0X200,
	   	   	0X200,
	   	   	0X208,	// VFP for idle mode???
//tpc_sync_control vsout hsout off
	0X1BC,	0X204,
	   	   	0X200,
	0X1BF,	0X2C3,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
//SOUT EQ & Source timing setting
	0X1BC,	0X20C,
	   	   	0X200,
	0X1BF,	0X2C4,
	   	   	0X270,
	   	   	0X200,
	   	   	0X200,
	   	   	0X222,	// EQW5 D[7:4], EQW3 D[3:0]
	   	   	0X222,	// EQW7 D[7:4]: CKH falling edge VGH->GND, EQW6 D[3:0]: CKH rising edge VGL->GND
	   	   	0X222,	// EQW9 D[7:4]: XCKH falling edge VGH->GND, EQW8 D[3:0]: XCKH rising edge VGL->GND
	   	   	0X233,	// EQW11 D[7:4], EQW10 D[3:0]
	   	   	0X233,	// EQW13 D[7:4]: CKH falling edge GND->VSN, EQW12 D[3:0]: CKH rising edge GND->VSP
	   	   	0X233,	// EQW15 D[7:4]: XCKH falling edge GND->VSN, EQW14 D[3:0]: XCKH rising edge GND->VSP
	   	   	0X201,	// SNT1 D[7:0], 0h setting is inhibited.
	   	   	0X201,	// SNT2 D[6:0], 0h setting is inhibited.

//ST_SW
	0X1BC,	0X216,
	   	   	0X200,
	0X1BF,	0X2C6,
	   	   	0X2C8,	// RTN0: one line period ???
	   	   	0X201,	// (STV delay) ST1 D[6:0], 0h setting is inhibited.
	   	   	0X275,	// (STV width) SW1 D[7:0], 0h setting is inhibited.
	   	   	0X205,	// (CKV delay) ST2 D[6:0], 0h setting is inhibited.
	   	   	0X26A,	// (CKV width) SW2 D[7:0], 0h setting is inhibited.
	   	   	0X200,	// ST3
	   	   	0X200,	// SW3
	   	   	0X200,	// ST4
	   	   	0X200,	// SW4
	   	   	0X200,	// ST5
	   	   	0X200,	// SW5
	   	   	0X200,	// ST6
	   	   	0X200,	// SW6
	   	   	0X200,	// ST7
	   	   	0X200,	// SW7
	   	   	0X200,
	   	   	0X200,
	   	   	0X20A,	// (CKH delay) PSWT1 D[6:0], 0h setting is inhibited.
	   	   	0X21B,	// (CKH width) PSWW1 D[7:0], 0h setting is inhibited.
	   	   	0X207,	// (CKH gap) PSWG1 D[5:0], 0h setting is inhibited.
	   	   	0X2C8,	// RTN2 ???

////Gamma setting common set
//	0X1BC,	0X21F,
//	   	   	0X200,
//	0X1BF,	0X2C7,
////----------------- GAMMA--------------// Gamma 2.4 
//                // 20160223
//	   	   	0X200,//    
//					0X212,//    
//					0X21B,//    
//					0X225,//    
//					0X233,//    
//					0X242,//    
//					0X24C,//    
//					0X25D,//    
//					0X242,//    
//					0X24B,//    
//					0X257,//    
//					0X264,//    
//					0X26C,//    
//					0X271,//    
//					0X275,//    
//								     

//	   	   	0X200,//    
//					0X212,//    
//					0X21B,//    
//					0X225,//    
//					0X233,//    
//					0X242,//    
//					0X24C,//    
//					0X25D,//    
//					0X242,//    
//					0X24B,//    
//					0X257,//    
//					0X264,//    
//					0X26C,//    
//					0X271,//    
//					0X275,// 


//Gamma setting common set
	0X1BC,	0X21F,
	   	   	0X200,
	0X1BF,	0X2C7,
//----------------- GAMMA--------------// Gamma 2.4 
                // 20160301
	   	   	0X200,//    
					0X212,//    
					0X21A,//    
					0X225,//    
					0X233,//    
					0X242,//    
					0X24C,//    
					0X25C,//    
					0X242,//    
					0X24A,//    
					0X255,//    
					0X25F,//    
					0X269,//    
					0X26F,//    
					0X275,//    
								     

	   	   	0X200,//    
					0X212,//    
					0X21A,//    
					0X225,//    
					0X233,//    
					0X242,//    
					0X24C,//    
					0X25C,//    
					0X242,//    
					0X24A,//    
					0X255,//    
					0X25F,//    
					0X269,//    
					0X26F,//    
					0X275,// 





//Digtal Gamma set 20160222
//	0X1BC,	0X214,
//	   	   	0X200,
//	0X1BF,	0X2C8,
//	   	   	0x201,
//	   	   	0x200,
//	   	   	0x205,
//					0x205,
//	   	   	0x2F8,
//	   	   	0x2BA,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x20A,
//	   	   	0x204,
//	   	   	0x2FA,
//	   	   	0x2FC,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x202,
//	   	   	0x20A,
//	   	   	0x208,
//	   	   	0x2EC,
//	   	   	0x200,
	   	   	

////Digtal Gamma set 20160301 13£º20
//	0X1BC,	0X214,
//	   	   	0X200,
//	0X1BF,	0X2C8,
//	   	   	0x201,
//	   	   	0x200,
//	   	   	0x206,
//					0x205,
//	   	   	0x2FA,
//	   	   	0x2FC,
//	   	   	0x200,
//	   	   	0x200,
//	   	   	0x20A,
//	   	   	0x205,
//	   	   	0x2FC,
//	   	   	0x2FC,
//	   	   	0x200,
//	   	   	0x205,
//	   	   	0x200,
//	   	   	0x20B,
//	   	   	0x20C,
//	   	   	0x2FC,
//	   	   	0x200,

//Digtal Gamma set 20160301 14£º54
	0X1BC,	0X214,
	   	   	0X200,
	0X1BF,	0X2C8,
	   	   	0x201,
	   	   	0x200,
	   	   	0x2FE,
					0x200,
	   	   	0x2FE,
	   	   	0x2C8,
	   	   	0x200,
	   	   	0x200,
	   	   	0x202,
	   	   	0x200,
	   	   	0x200,
	   	   	0x2FC,
	   	   	0x200,
	   	   	0x204,
	   	   	0x2FE,
	   	   	0x204,
	   	   	0x20D,
	   	   	0x2ED,
	   	   	0x200,














//Digtal Gamma setting
//	0X1BC,	0X214,
//	   	   	0X200,
//	0X1BF,	0X2C8,//    ES3_gamma2.4(0.30,0.32)  ES3_gamma2.2(0.31,0.33)   ES3_gamma2.4(0.29,0.31)      20150921       20150928    20151008-ES3_gamma2.4(0.30,0.32)
//	   	   	0x200,//00            01                      01                         01                    01            01          01
//	   	   	0x200,//00            04                      03                         04                    00            00          00
//	   	   	0x200,//00            00                      FE                         FF                    02            00          00
//	   	   	0x201,//00            FD                      00                         FF                    FF            FE          01
//	   	   	0x2FF,//00            FD                      F7                         FF                    FC            FE          FF
//	   	   	0x2FC,//FC            FC                      FC                         C5                    FC            FC          FC
//	   	   	0x200,//00            00                      00                         00                    00            00          00
//	   	   	0x200,//00            00                      00                         00                    00            00          00
//	   	   	0x200,//00            01                      00                         01                    01            00          00
//	   	   	0x200,//00            00                      02                         00                    01            FD          00
//	   	   	0x200,//00            01                      FA                         01                    FF            00          00
//	   	   	0x2FC,//FC            ED                      D0                         D8                    DC            FC          FC
//	   	   	0x200,//00            00                      00                         00                    00            00          00
//	   	   	0x200,//00            05                      03                         05                    00            04          00
//	   	   	0x203,//00            00                      00                         00                    01            FE          03
//	   	   	0x207,//00            08                      09                         08                    08            03          07
//	   	   	0x20E,//00            0E                      03                         11                    09            0D          0E
//	   	   	0x2FC,//FC            F0                      B4                         FC                    DC            FC          FC
//	   	   	0x200,//00            00                      00                         00                    00            00          00
                          
													 
////Color enhancement      
//	0X1BC,	0X22C,
//	   	   	0X200,
//	0X1BF,	0X2CA,
//	   	   	0X21C,
//	   	   	0X2FC,
//	   	   	0X2FC,
//	   	   	0X2FC,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,	// 10P
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,	// 20
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,	// 30P
//        	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,	// 40
//	   	   	0X200,
//	   	   	0X200,
//	   	   	0X200,

//panel pin control
	0X1BC,	0X210,
	   	   	0X200,
	0X1BF,	0X2CB,
	   	   	0X2EC,
	   	   	0X2FD,
	   	   	0X2BF,
	   	   	0X237,
	   	   	0X220,
	   	   	0X200,
	   	   	0X200,
	   	   	0X204,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
	   	   	0X2E0,
	   	   	0X200,
	   	   	0X200,
//Panel interface control typeB
	0X1BC,	0X202,
	   	   	0X200,
	0X1BF,	0X2CC,
	   	   	0X20E,

////Back Light Control 4
//0X1BC,	0X21A,
//        	0X200,
//0X1BF,	0X2CE,
//				0X255,
//				0X240,
//				0X249,
//				0X253,
//				0X259,
//				0X25E,
//				0X263,
//				0X268,
//				0X26E,
//				0X274,//10
//				0X27E,
//				0X28A,
//				0X298,
//				0X2A8,
//				0X2BB,
//				0X2D0,
//				0X2FF,
//				0X204,//PWM_DIV   21.8kHz(default): 04h, 3kHz: 23h, 10kHz: 09h, 34kHz: 02h
//				0X200,//PWM_CYCLE 21.8kHz(default): 00h, 3kHz: 04h, 10kHz: 19h, 34kHz: 13h
//				0X204,//20
//				0X204,
//				0X242,
//				0X200,
//				0X269,
//				0X25A,//25

//Power setting 1 for charge pump 
	0X1BC,	0X20B,
	   	   	0X200,
	0X1BF,	0X2D0,
	   	   	0X211,	// BT2(When VLM2E and GVDDDRCT set 1h, VGH=BT2) D[3]=1h(VCI2x2), BT3(When VLM3E and GVSSDRCT set 1h, VGL=BT3) D[7]=1h(VSN-VCI3), DC2=DC3=210 division.
	   	   	0X200,
	   	   	0X200,
	   	   	0X254,	// GVDD(VGH) D[5:0]=14h(8.0V), VLM2E D[6]=1h(VGH=GVDD, when GVDDDRCT=1h)
	   	   	0X2CF,	// GVSS(VGL) D[5:0]=0Fh(-7.0V), VLM3E D[6]=1h(VGL=GVSS, when GVDDDRCT=1h)
	   	   	0X240,
	   	   	0X219,
	   	   	0X219,
	   	   	0X209,
	   	   	0X200,
//Power setting 2
	0X1BC,	0X205,
	   	   	0X200,
	0X1BF,	0X2D1,	// VSP/VSN used power IC.(2power type A)
	   	   	0X200,
	   	   	0X248,
	   	   	0X216,
	   	   	0X20F,

//Power setting for internal power
	0X1BC,	0X21B,
	   	   	0X200,
	0X1BF,	0X2D3,
	   	   	0X21B,	// VDCON
	   	   	0X233,
	   	   	0X299,	// Gamma Bias pos
	   	   	0X2BB,	// SEGON
	   	   	0X2B3,
	   	   	0X233,
	   	   	0X233,
	   	   	0X233,
	   	   	0X211,	// Gamma Bias neg
	   	   	0X200,	// SEQGND2 D[7:4]=0h, SEQGND1 D[3:0]=0h, GND time setting between two source output.
	   	   	0X201,
	   	   	0X200,
	   	   	0X200,
	   	   	0X2D8,	// VC2 D[7:4]=Dh(5V), VCI2 is internal voltage for generating VGH.
	   	   	0X2A0,
	   	   	0X205,	// VC3 D[3:0]=5h(3.4v), VCI3 is internal voltage for generating VGL.
	   	   	0X23F,	// VNLVL=2F=4.1v 35=4.25v ; 37=4.3v ; 47=4.7v  4B=4.8V
	   	   	0X23F,	// VPLVL=2F=4.1v
	   	   	0X233,
	   	   	0X233,
	   	   	0X272,	// VBTS D[1:0]=2h(4.02V), detect VSP to enter APO sequence.(VBTS function is available, when VCIONB=GND(select internal(IOVCC)/external(GND) VCI).)
	   	   	0X212,	// VBTS_IOVCC D[1:0]=2h(1.50V), detect IOVCC to enter APO sequence.
	   	   	0X28A,	// VBTS_VCI D[1:0]=2h(2.14V), detect VCI to enter APO sequence.(VBTS_VCI function is available, when VCIONB=IOVCC(select internal(IOVCC)/external(GND) VCI).)
	   	   	0X257,	// GVDDDRCT D[7:6]=1h(VGH short), GVSSDRCT[5:4]=1h(VGL short)
	   	   	0X23D,
	   	   	0X2BC,
//VCOM setting
	0X1BC,	0X208,
	   	   	0X200,
	0X1BF,	0X2D5,
	   	   	0X206,
	   	   	0X200,
	   	   	0X200,
	   	   	0X201,
	   	   	0X21A,
	   	   	0X201,
	   	   	0X21A,
//sequence timing control for power on
	0X1BC,	0X219,
	   	   	0X200,
	0X1BF,	0X2D7,
	   	   	0X2BF,
	   	   	0X2F8,
	   	   	0X27F,
	   	   	0X2A8,
	   	   	0X2CE,
	   	   	0X23E,
	   	   	0X2FC,
	   	   	0X2C1,
	   	   	0X2E1,
	   	   	0X2EF,
	   	   	0X283,
	   	   	0X207,
	   	   	0X23F,
	   	   	0X210,
	   	   	0X27F,
	   	   	0X2C0,
	   	   	0X201,
	   	   	0X2E7,
	   	   	0X240,
	   	   	0X21C,
	   	   	0X200,
	   	   	0X2C0,
	   	   	0X200,
	   	   	0X200,

//Test image generator
	0X1BC,	0X205,
	   	   	0X200,
	0X1BF,	0X2DE,
	   	   	0X200,	// enable
	   	   	0X233,
	   	   	0X2F8,
	   	   	0X210,















//	0X1BC,	0X205,
//	   	   	0X200,
//	0X1BF,	0X2FD,//VPLVL/VNLVL monitor
//	   	   	0X200,
//					0X200,
//					0X200,
//					0X230,
	


//Change to DCS
	0X1B7,	0X250,
	   	   	0X203,

//Column Address Order Right to Left
	0X1BC,	0X201,
	   	   	0X200,
	0X136,	0X200,

//CABC
	0X1BC,	0X201,
	   	   	0X200,
	0X151,	0X2FF,

	0X1BC,	0X201,
	   	   	0X200,
	0X153,	0X22C,

	0X1BC,	0X201,
	   	   	0X200,
	0X155,	0X200,
//display on   
//	0x3FF,
		
	0XF00		
 };

unsigned short ET2_InitCode[]= 
{
	0X1B7,	0X250,	0X205,
	0x1BC,	0x201,  0x200,
  0x151,	0x2FF,

  0x1BC,	0x201, 0x200,
  0x153,	0x22C,

  0x1BC,	0x201, 0x200,
  0x155,	0x200,
	
	0X1BC,	0X202,	0X200,
//	0X1BF,	0X236,	0X240,		//set address mode
//	0X1BF,	0X23A,	0X270,		//set pixel format	
	0X325,

	0X1B7,	0X210,	0X205,		//for generate package

	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,		//Manufacturer Command Access Protect
//	0X355,

	0XF00	
};

unsigned short RA_InitCode[]=
{
	0X1B7,	0X250,	0X205,
	
	0X1BC,	0X202,	0X200,
//	0X1BF,	0X236,	0X240,		//set address mode
//	0X1BF,	0X23A,	0X270,		//set pixel format	
	0X325,

	0X1B7,	0X210,	0X205,		//for generate package

	0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X204,		//Manufacturer Command Access Protect
//interface setting video mode
	0X1BC,	0X207,
	   	   	0X200,
	0X1BF,	0X2B3,
	   	   	0X204,	// Assignment of Display Mode and Based signals, DSI-internal OSC.
	   	   	0X200,
	   	   	0X200,	// Interval of TE signal output for every frame.
	   	   	0X200,
	   	   	0X200,
	   	   	0X200,
//Test image generator
	0X1BC,	0X205,
	   	   	0X200,
	0X1BF,	0X2DE,
	   	   	0X201,	// enable
	   	   	0X230,
	   	   	0X2F9,
//	   	   	0X23F,
//	   	   	0X2FF,
	   	   	0X210,
	0X355,

	0XF00	
};

uint16_t gamma[10][64];
