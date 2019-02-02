#include "include.h" 

//Syna. Recommend Porch Setting
#define		SSD_HACT	480 
#define		SSD_VACT	2560
#define		SSD_HSA		0x01    
#define		SSD_HBP		0x50
#define		SSD_HFP		0x70  
#define		SSD_VSA		0x01
#define		SSD_VBP		0x05
#define		SSD_VFP		0x09

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	SSD_HACT,	//FPGA_HAC
	SSD_VACT,	//FPGA_VAC
	SSD_HSA,	//FPGA_HSA
	(SSD_HBP-SSD_HSA),	//FPGA_HBP
	SSD_HFP,	//FPGA_HFP
	SSD_VSA,	//FPGA_VSA
	(SSD_VBP-SSD_VSA),	//FPGA_VBP
	SSD_VFP 	//FPGA_VFP
};

unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
//	0X1BA,	0X266,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
//	0X1BB,	0X20F,	0X200,		//	816 / 16 / 8 = 6.375MHz
	0X1BA,	0X266,	0X2C4,		// 24MHz * 150 / 4 = 900Mbps	 135*24/4=810
	0X1BB,	0X20B,	0X200,		//	900 / 16 / 8 = 7.03125MHz
//	0X1BB,	0X20B,	0X200,		//	900 / 12 / 8 = 9.375MHz
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
//DCS packet setting
0x1B7,	0x250,	0x205,
0x1BC,	0x202,	0x200,
0x1BF,	0x211, // 20160126 add for Initial setting cover FW setting.
				0x200,
0x3FF,

//Generic packet setting
0x1B7,	0x210,	0x205,

//Manufacture Command Access Protect
0x1BC,	0x202,	0x200,
0x1BF,	0x2B0,
//				0x200, // MCAP=0h : Unlock Manufacture Command Protect
				0x204, // Unlock Manufacture Command Protect (high-privilege)

//Deep Standby Mode: Hardware Reset pull Low.

//=========LCD Manufacture Command Start==========//
//Interface setting
0x1BC,	0x204,	0x200,
0x1BF,	0x2B3,
				0x201, // Command Mode
				0x200,
				0x206,

//Interface ID setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2B4,
				0x200,

//DSI Control
0x1BC,	0x206,	0x200,
0x1BF,	0x2B6,
				0x232,//FOR CMD 1/3 RAM
				0x2BB,
				0x201,
				0x2FF,
				0x2FF,

//Back Light Control 1 (C=1 GUI)
0x1BC,	0x208,	0x200,
0x1BF,	0x2B8,
				0x257,
				0x23D,
				0x219,
				0x21E,
				0x20A,
				0x250,
				0x250,

//Back Light Control 2 (C=2 Still)
0x1BC,	0x208,	0x200,
0x1BF,	0x2B9,
				0x26F,
				0x23D,
				0x228,
				0x23C,
				0x214,
				0x2C8,
				0x2C8,

//Back Light Control 3 (C=3 Movie)
0x1BC,	0x208,	0x200,
0x1BF,	0x2BA,
				0x2B5,
				0x233,
				0x241,
				0x264,
				0x223,
				0x2A0,
				0x2A0,

//SRE Control 1 (SRE=1 Weak setting)
0x1BC,	0x203,	0x200,
0x1BF,	0x2BB,
				0x214,
				0x214,

//SRE Control 2 (SRE=2 Middle setting)
0x1BC,	0x203,	0x200,
0x1BF,	0x2BC,
				0x237,
				0x232,

//SRE Control 3 (SRE=3 Strong setting)
0x1BC,	0x203,	0x200,
0x1BF,	0x2BD,
				0x264,
				0x232,

//External Clock Setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2BE,
				0x201,

//Slew Rate Adjustment (SOUT CKH/XCKH slew rate)
0x1BC,	0x202,	0x200,
0x1BF,	0x2C0,
				0x200, // SOUTTF2: panel left falling, SOUTTR2: panel left rising, SOUTTF1: panel right falling, SOUTTR1: panel right rising

//Display Setting1 common
0x1BC,	0x230,	0x200,
0x1BF,	0x2C1,
				0x204, // 1P
				0x240,
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x295,
				0x24E, // 5P
				0x219,
				0x2E1,
				0x27F,
				0x2FF,
				0x2FF, // 10P
				0x25F,
				0x206,
				0x26B,
				0x2C1,
				0x25A, // 15P
				0x2D0,
				0x298,
				0x220,
				0x2FF,
				0x2FF, // 20P
				0x2FF,
				0x23E,
				0x28C,
				0x272,
				0x2D2, // 25P
				0x20A,
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 20160219 For RGB/BGR timing from 00 to 10
				0x200, // 35P
				0x202,
				0x242,
				0x201,
				0x206,
				0x206, // 40P
				0x204,
				0x200,
				0x201,
				0x200,
				0x201, // 45P
				0x25F,
				0x200,

//Display Setting2
0x1BC,	0x214,	0x200,
0x1BF,	0x2C2,
				0x201, // 1P
				0x2FA,
				0x200,
				0x206, // BP
				0x268, // 20160216 From 69 to 68
				0x208, // 20160216 From 09 to 08
				0x200,
				0x200,
				0x200,
				0x288, // 20160127 From 08 to 88, 20160317 For RGB/BGR timing H-line issue from 88 to 8A (not in SPEC)
				0x230, // 20160112 For H-line from 70 to 30
				0x200,
				0x200,
				0x200,
				0x200, // 15P
				0x200,
				0x203, // 20160216 From 04 to 03
				0x274, // 20160216 From 89 to 74
				0x200,

//Display Setting3 
0x1BC,	0x240,	0x200,
0x1BF,	0x2C3,
				0x20A, // 1P
				0x20E,
				0x20A,
				0x20D, // 20160216 From 0E to 0D
				0x20A, // 5P
				0x20E,
				0x200,
				0x200,
				0x200,
				0x200, // 10P
				0x200,
				0x200,
				0x200,
				0x200,
				0x205, // 15P
				0x209,
				0x200,
				0x200,
				0x200,
				0x200, // 20P
				0x205,
				0x20A,
				0x200,
				0x200,
				0x201, // 25P
				0x201,
				0x203,
				0x228,
				0x209,
				0x201, // 30P
				0x202,
				0x201,
				0x211,
				0x201,
				0x22C, // 35P
				0x200,
				0x222, // 20160216 From 2C to 22
				0x200,
				0x22C,
				0x200, // 40P
				0x226,
				0x202,
				0x222, // 20160216 From 2C to 22
				0x200,
				0x226, // 45P
				0x202,
				0x226,
				0x202,
				0x222, // 20160216 From 2C to 22
				0x200, // 50P
				0x226,
				0x202,
				0x258,
				0x240, // 20160129 From 80 to 40
				0x204, // 20160129 From 05 to 04
				0x258,
				0x280,
				0x205,
				0x244, // 20160216 From 58 to 44
				0x280, // 60P
				0x205,
				0x228,
				0x214,

//Source Timing Setting & SOUT Equalize Setting
//Note: CKH/XCKH EQ rising max set 'h1(2clk), it will aggravate H-line in worse pattern;
//      CKH/XCKH EQ falling max set 'h1(2clk), it will take up all the CKH/XCKH gap;
0x1BC,	0x211,	0x200,
0x1BF,	0x2C4,
				0x214, // 1P
				0x200,
				0x200,
				0x200, // EQW5, EQW3
				0x200, // EQW7: XCKH falling to GND, EQW6: XCKH rising to GND
				0x200, // EQW9: CKH falling to GND, EQW8: CKH rising to GND
				0x200,
				0x200, // EQW13: XCKH falling to VSP, EQW12: XCKH rising to VSN
				0x200, // EQW15: CKH falling to VSN, EQW14: CKH rising to VSP
				0x201, // SNT1: Source pre-charge (Set inhibit???)
				0x202, // SNT2
				0x201, // SEQGND2, SEQGND1
				0x201,
				0x200,
				0x200, // 15P
				0x200,

//Cross hair and PC Idle
0x1BC,	0x208,	0x200,
0x1BF,	0x2C5,
				0x208,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,

//LTPS Timing Setting
0x1BC,	0x219,	0x200,
0x1BF,	0x2C6,
				0x248, // RTN0: 20160129 From 43 to 48
				0x200, // ST1:  20160127 From 00 to 01, 20160129 From 01 to 00
				0x248, // SW1:  20160127 From 43 to 46, 20160129 From 46 to 48
				0x201, // ST2:  20160129 From 04 to 01, 20160321 From 01 to 03, 20160405 From 03 to 01
				0x244, // SW2:  20160129 From 3B to 46, 20160307 From 46 to 43, 20160321 From 43 to 44
				0x200, // ST3
				0x200, // SW3
				0x200, // ST4
				0x200, // SW4
				0x200, // ST5
				0x200, // SW5
				0x200, // ST6
				0x200, // SW6
				0x200,
				0x200, // 15P
				0x200,
				0x200,
				0x203, // PSWT1: 20160127 From 06 to 02, 20160216 From 02 to 05, 20160405 From 05 to 03
				0x212, // PSWW1: 20160127 From 0E to 14, 20160129 From 14 to 15, 20160216 From 15 to 12, 20160315 From 12 to 11, 20160321 From 11 to 12
				0x205, // PSWG1: 20160127 From 06 to 02, 20160216 From 02 to 05
				0x243, // RTN2
				0x200, // RTN_TPC1[11:8]: 20160129 From 01 to 00
				0x248, // RTN_TPC1[7:0]:  20160129 From 0C to 48
				0x248, // RTN_EX: 20160129 From 43 to 48

//----------------------XM 5.2 inch-----------------------//
//Gamma Setting Common Set
0x1BC,	0x21F,	0x200,
0x1BF,	0x2C7,
				0x200,
				0x213,
				0x21D,
				0x228,
				0x237,
				0x244,
				0x24D,
				0x25B,
				0x23E,
				0x246,
				0x252,
				0x25F,
				0x268,
				0x270,
				0x278,
				0x200,
				0x213,
				0x21D,
				0x228,
				0x237,
				0x244,
				0x24D,
				0x25B,
				0x23E,
				0x246,
				0x252,
				0x25F,
				0x268,
				0x270,
				0x278,
//--------------------------------------------------------//

//Digtal Gamma Setting
0x1BC,	0x238,	0x200,
0x1BF,	0x2C8,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,

//Digtal Gamma Setting
0x1BC,	0x214,	0x200,
0x1BF,	0x2C9,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x2FC,
				0x200,

//Color enhancement
0x1BC,	0x22C,	0x200,
0x1BF,	0x2CA,
				0x21C,
				0x2FC,
				0x2FC,
				0x2FC,
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
				0x200,
				0x200,
				0x200,

//Panal PIN Control - SOUT FUNC control
0x1BC,	0x212,	0x200,
0x1BF,	0x2CB,
				0x2FF, // 1P
				0x2FF,
				0x2FF, // 20160129 From F9 to FF
				0x2FF,
				0x20F, // 5P
				0x280,
				0x200,
				0x206, // 20160129 From 00 to 06
				0x210,
				0x200, // 10P
				0x238,
				0x270,
				0x2E9, // 20160129 From E9 to EF, 20160317 For sleep in sequence VSR signal pull high from EF to E9 (Appendix error!)
				0x2C0,
				0x201, // 15P
				0x200,
				0x240,

//Panel Interface Control
0x1BC,	0x202,	0x200,
0x1BF,	0x2CC,
				0x237,

//Touch control
0x1BC,	0x214,	0x200,
0x1BF,	0x2CD,
				0x200, // 1P
				0x200,
				0x25C,
				0x2C0,
				0x248, // 20160216 From 5C to 48
				0x2C0,
				0x25C,
				0x200,
				0x23C,
				0x225, // 20160216 From 3C to 25
				0x236,
				0x21F, // 20160216 From 36 to 1F
				0x2D6,
				0x2D6,
				0x2D6, // 15P
				0x2D6,
				0x201,
				0x200,
				0x200,

//Back Light Control 4
0x1BC,	0x21A,	0x200,
0x1BF,	0x2CE,
				0x25D, // 1P
				0x240,
				0x249,
				0x253,
				0x259, // 5P
				0x25E,
				0x263,
				0x268,
				0x26E,
				0x274, // 10P
				0x27E,
				0x28A,
				0x298,
				0x2A8,
				0x2BB, // 15P
				0x2D0,
				0x2FF,
				0x204, // PWM_DIV   23.5kHz(default): 04h, 3kHz: 18h, 10kHz: 09h, 34kHz: 01h
				0x200, // PWM_CYCLE 23.5kHz(default): 00h, 3kHz: 91h, 10kHz: 2Dh, 34kHz: BAh
				0x204, // 20P
				0x204,
				0x242,
				0x200,
				0x269,
				0x25A, // 25P

//-------------clear B/L light on (20160312 care)-----------------
//Panel synchronous output
0x1BC,	0x204,	0x200,
0x1BF,	0x2CF,
				0x240,
				0x210,
				0x200,
//----------------------------------------------------------------

//Power Setting (for CHGP)
0x1BC,	0x212,	0x200,
0x1BF,	0x2D0,
				0x211, // 20160127 From 33 to 11
				0x254,
				0x2CF,
				0x231,
				0x201,
				0x210,
				0x210,
				0x210,
				0x219,
				0x219,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,
				0x200,

//Touch Period and Source Drive Mode Select
0x1BC,	0x202,	0x200,
0x1BF,	0x2D1,
				0x200, // Long-H: 00, Long-V: 04.

//Power Setting for Internal Power 
0x1BC,	0x215,	0x200,
0x1BF,	0x2D3,
				0x21B, // 1P
				0x23B,
				0x2BB,
				0x277, // Source bias
				0x277, // 5P
				0x277,
				0x2BB,
				0x2B3,
				0x233,
				0x200, // 10P
				0x200,
				0x26E,
				0x26E,
				0x2DB, // VPLVL D[6:0]: 5.2V(5B), 4.1V(2F)
				0x2DB, // VNLVL D[6:0]: -5.2V(5B), -4.1V(2F)
				0x233,
				0x2BB,
				0x2F2, // VSP detect voltage: F0h(4.7V), F1h(5.1V), F2h(4.0V), F3h(4.4V)
				0x2FD,
				0x2C6, // 20P
//Note: TD4302 APO detection func. via three ways:
//      1. Driver IC RESET;
//      2. IOVCC (detect voltage 1.5V, fixed, if VSP is useable, a internal IOVCC(1.45V) will be builded through VSP);
//      3. VSP (detect setting refer to D3 18th parameter);

//VCOM Setting (VCOM2 Setting(2))
0x1BC,	0x208,	0x200,
0x1BF,	0x2D5,
				0x203,
				0x200,
				0x200,
				0x200,
				0x228, // 05D = -0.3666V, 055 = -0.33488V
				0x200,
				0x228,

//Sequencer Test Control
0x1BC,	0x202,	0x200,
0x1BF,	0x2D6,
				0x2C1,

//Sequencer Timing Control for Pon
0x1BC,	0x222,	0x200,
0x1BF,	0x2D7,
				0x2F6, // 1P
				0x2FF,
				0x203,
				0x205,
				0x241, // 5P
				0x224,
				0x280,
				0x21F,
				0x2C7,
				0x201, // 20160317 For sleep in sequence Source to GND set from 1F to 01, 20160418 TD4302 RD approve this setting (For 01 as TD4322.)
				0x21B,
				0x200,
				0x20C,
				0x207,
				0x220, // 15P
				0x200,
				0x200,
				0x200,
				0x200,
				0x200, // 20P
				0x20C,
				0x2F0,
				0x207, // 20160317 For sleep in sequence Vcom to GND set from 1F to 07
				0x200,
				0x20C, // 25P
				0x200,
				0x200,
				0x2AA,
				0x267,
				0x27E, // 30P
				0x25D,
				0x206,
				0x200,

//Power Sequencer Control
0x1BC,	0x203,	0x200,
0x1BF,	0x2D9,
				0x200,
				0x200,

//Test Image Generator
0x1BC,	0x205,	0x200,
0x1BF,	0x2DE,
				0x200,
				0x23F,
				0x2FF,
				0x290,

////DSC setting
//0x1BC,	0x205,	0x200,
//0x1BF,	0x2E0,
//				0x201,
//				0x208,
//				0x200,
//				0x200,

//Compression Test Register
0x1BC,	0x202,	0x200,
0x1BF,	0x2E7,
				0x200,

//PPS Setting
0x1BC,	0x281,	0x200,
0x1BF,	0x2E9,
				0x211,
				0x200,
				0x200,
				0x289,
				0x210,
				0x280,
				0x20A,
				0x200,
				0x205,
				0x2A0,
				0x200,
				0x220,
				0x202,
				0x2D0,
				0x202,
				0x2D0,
				0x202,
				0x200,
				0x202,
				0x268,
				0x200,
				0x220,
				0x203,
				0x287,
				0x200,
				0x20A,
				0x200,
				0x20C,
				0x203,
				0x219,
				0x202,
				0x263,
				0x218,
				0x200,
				0x210,
				0x2F0,
				0x203,
				0x20C,
				0x220,
				0x200,
				0x206,
				0x20B,
				0x20B,
				0x233,
				0x20E,
				0x21C,
				0x22A,
				0x238,
				0x246,
				0x254,
				0x262,
				0x269,
				0x270,
				0x277,
				0x279,
				0x27B,
				0x27D,
				0x27E,
				0x201,
				0x202,
				0x201,
				0x200,
				0x209,
				0x240,
				0x209,
				0x2BE,
				0x219,
				0x2FC,
				0x219,
				0x2FA,
				0x219,
				0x2F8,
				0x21A,
				0x238,
				0x21A,
				0x278,
				0x21A,
				0x2B6,
				0x22A,
				0x2F6,
				0x22B,
				0x234,
				0x22B,
				0x274,
				0x23B,
				0x274,
				0x26B,
				0x2F4,
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
				0x200,
				0x200,
				0x200,
				0x200,

//Compression method 
0x1BC,	0x203,	0x200,
0x1BF,	0x2EB,
				0x283,
				0x283,//83

//Compression Mode Setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2EC,
				0x207,//07

//Test Mode
0x1BC,	0x204,	0x200,
0x1BF,	0x2F1,
				0x200,
				0x200,
				0x200,

//Test Mode
0x1BC,	0x209,	0x200,
0x1BF,	0x2F9,
				0x224,
				0x23F,
				0x2E0,
				0x2BE,
				0x200,
				0x28D,
				0x2BF,
				0x280,
//=========LCD Manufacture Command End==========//

//OTP/Flash Load setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2D6,
				0x201, // T_SLPOUT_1=0h, T_SLPOUT_2=0h : OTP/Flash load OFF after exit_sleep_mode

////Interface setting
//0x1BC,	0x204,	0x200,
//0x1BF,	0x2B3,
//				0x201, // RM=0->1(video to ram)
//				0x200,
//				0x206,


//DCS packet setting
0x1B7,	0x250,	0x205,

0x1BC,	0x202,	0x200,
0x1BF,	0x251,	0x2FF,
0x1BF,	0x253,	0x22C,
0x1BF,	0x255,	0x200,


0xF00

 };

unsigned short ET2_InitCode[]= 
{
//DCS packet setting
0x1B7,	0x250,	0x205,
0x1BC,	0x202,	0x200,
0x1BF,	0x211, // 20160126 add for Initial setting cover FW setting.
				0x200,
0x3FF,

//Generic packet setting
0x1B7,	0x210,	0x205,

//Manufacture Command Access Protect
0x1BC,	0x202,	0x200,
0x1BF,	0x2B0,
//				0x200, // MCAP=0h : Unlock Manufacture Command Protect
				0x204, // Unlock Manufacture Command Protect (high-privilege)

//OTP/Flash Load setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2D6,
				0x201, // T_SLPOUT_1=0h, T_SLPOUT_2=0h : OTP/Flash load OFF after exit_sleep_mode

//Notice: the display mode in FW is command mode, so that it should set video to ram mode specially.
//Interface setting
0x1BC,	0x204,	0x200,
0x1BF,	0x2B3,
				0x201, // RM=0->1(video to ram)
				0x200,
				0x206,

////Manufacture Command Access Protect
//0x1BC,	0x202,	0x200,
//0x1BF,	0x2B0,
//				0x203, // MCAP=3h : Lock Manufacture Command Protect

//DCS packet setting
0x1B7,	0x250,	0x205,

0x1BC,	0x202,	0x200,
0x1BF,	0x251,	0x2FF,
0x1BF,	0x253,	0x22C,
0x1BF,	0x255,	0x200,

////set_partial_area
////Forward : SR=0, ER=2A7h(679)
////Backward : SR=758h(1880), ER=9FFh(2559)
//0x1BC,	0x205,	0x200,
//0x1BF,	0x230,
//				0x207,
//				0x258,
//				0x209,
//				0x2FF,

0xF00

};

unsigned short RA_InitCode[]=
{
//DCS packet setting
0x1B7,	0x250,	0x205,
0x1BC,	0x202,	0x200,
0x1BF,	0x229, // 20160126 add for Initial setting cover FW setting. 20160217 why need 29 to light on free-run-mode?????
				0x200,
0x3FF,

//Generic packet setting
0x1B7,	0x210,	0x205,

//Manufacture Command Access Protect
0x1BC,	0x202,	0x200,
0x1BF,	0x2B0,
//				0x200, // MCAP=0h : Unlock Manufacture Command Protect
				0x204, // Unlock Manufacture Command Protect (high-privilege)

//Test Image Generator
0x1BC,	0x205,	0x200,
0x1BF,	0x2DE,
				0x201, //1 TIGON=0h : Test Image Generator OFF
				0x230,
				0x2F9, 
				0x290,

////Manufacture Command Access Protect
//0x1BC,	0x202,	0x200,
//0x1BF,	0x2B0,
//				0x203, // MCAP=3h : Lock Manufacture Command Protect

//DCS packet setting
0x1B7,	0x250,	0x205,

0x1BC,	0x202,	0x200,
0x1BF,	0x251,	0x2FF,
0x1BF,	0x253,	0x22C,
0x1BF,	0x255,	0x200,

////set_partial_area
////Forward : SR=0, ER=2A7h(679)
////Backward : SR=758h(1880), ER=9FFh(2559)
//0x1BC,	0x205,	0x200,
//0x1BF,	0x230,
//				0x207,
//				0x258,
//				0x209,
//				0x2FF,

0xF00

};

unsigned short OTP_InitCode[]= 
{
//DCS packet setting
0x1B7,	0x250,	0x205,
0x1BC,	0x202,	0x200,
0x1BF,	0x211, // 20160126 add for Initial setting cover FW setting.
				0x200,
0x3FF,

//Generic packet setting
0x1B7,	0x210,	0x205,

//Manufacture Command Access Protect
0x1BC,	0x202,	0x200,
0x1BF,	0x2B0,
//				0x200, // MCAP=0h : Unlock Manufacture Command Protect
				0x204, // Unlock Manufacture Command Protect (high-privilege)

//OTP/Flash Load setting
0x1BC,	0x202,	0x200,
0x1BF,	0x2D6,
				0x201, // T_SLPOUT_1=0h, T_SLPOUT_2=0h : OTP/Flash load OFF after exit_sleep_mode

////----------------------SH 5.5 inch-----------------------//

////--------------------------------------------------------//
//----------------------XM 5.2 inch-----------------------//
//Gamma Setting Common Set
0x1BC,	0x21F,	0x200,
0x1BF,	0x2C7,
				0x200,
				0x20B,
				0x213,
				0x21E,
				0x22E,
				0x23D,
				0x247,
				0x258,
				0x23E,
				0x247,
				0x254,
				0x261,
				0x26A,
				0x272,
				0x278,
				0x200,
				0x20B,
				0x213,
				0x21E,
				0x22E,
				0x23D,
				0x247,
				0x258,
				0x23E,
				0x247,
				0x254,
				0x261,
				0x26A,
				0x272,
				0x278,
//--------------------------------------------------------//

//VCOM Setting (VCOM2 Setting(2))
0x1BC,	0x208,	0x200,
0x1BF,	0x2D5,
				0x203,
				0x200,
				0x200,
				0x200,
				0x22A, // 05D = -0.3666V, 055 = -0.33488V
				0x200,
				0x22A,

//Notice: the display mode in FW is command mode, so that it should set video to ram mode specially.
//Interface setting
0x1BC,	0x204,	0x200,
0x1BF,	0x2B3,
				0x201, // RM=0->1(video to ram)
				0x200,
				0x206,

////Manufacture Command Access Protect
//0x1BC,	0x202,	0x200,
//0x1BF,	0x2B0,
//				0x203, // MCAP=3h : Lock Manufacture Command Protect

//DCS packet setting
0x1B7,	0x250,	0x205,

0x1BC,	0x202,	0x200,
0x1BF,	0x251,	0x2FF,
0x1BF,	0x253,	0x22C,
0x1BF,	0x255,	0x200,


0xF00

};

unsigned short GAMMA_InitCode[]= 
{
//Gamma Setting Common Set
0x1BC,	0x21F,	0x200,
0x1BF,	0x2C7,
				0x200,
				0x213,
				0x21D,
				0x228,
				0x237,
				0x244,
				0x24D,
				0x25B,
				0x23E,
				0x246,
				0x252,
				0x25F,
				0x268,
				0x270,
				0x278,
				0x200,
				0x213,
				0x21D,
				0x228,
				0x237,
				0x244,
				0x24D,
				0x25B,
				0x23E,
				0x246,
				0x252,
				0x25F,
				0x268,
				0x270,
				0x278,

0xF00

};

uint16_t gamma[10][64];
