// *****************************************************************************
//
//  R63353 Initial Code
//
//  FileName : LCD16-03_002_R63353_InitailCode_r000_20160301_for_Tianma.xlsm
//  Date  : 2016/3/10
//
//  Reference Address : I
//
//  ** Information **
//     No1
//     Tianma 204 pin
//     TE052VDXP04-00
//     Panel3 : NMOS Demux + NMOS VSR 1V1
//     (CMOS Dual)
//     Rev000
//
// *****************************************************************************"

#include "include.h" 

#define		SSD_HACT	1080
#define		SSD_VACT	1920
#define		SSD_HSA		0x02 
#define		SSD_HBP		0x2D 
#define		SSD_HFP		0x78 
#define		SSD_VSA		0x02 
#define		SSD_VBP		0x06 
#define		SSD_VFP		0x0E 

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	SSD_HACT,	        //FPGA_HAC
	SSD_VACT,	        //FPGA_VAC
	SSD_HSA,	        //FPGA_HSA
	SSD_HBP-SSD_HSA,	//FPGA_HBP
	SSD_HFP,	        //FPGA_HFP
	SSD_VSA,	        //FPGA_VSA
	SSD_VBP-SSD_VSA,	//FPGA_VBP
	SSD_VFP 	        //FPGA_VFP
};

unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
//	0X1BA,	0X266,	0X2C3,		// 24MHz * 102 / 3 = 816Mbps	 135*24/4=810
	0X1BA,	0X296,	0X2C4,		// 24MHz * 150 / 4 = 900Mbps
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
	0X1BC, 0X202,
	   	   0X200,
	0X1BF, 0X2B0,
	   	   0X204,//OR 0X200(protect NVM,open Manufacturer CMD),0x04(both NVM Manufacturer CMD enable)	

	0x1BC, 0x206,
				 0x200,
	0x1BF, 0x2B3, 
				 0x214, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2B4, 
				 0x20C, 
				 0x200, 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2B6, 
				 0x24B, 
				 0x2DB, 
				 0x216, 

	0x1BC, 0x208,
				 0x200,
	0x1BF, 0x2B8, 
				 0x257, 
				 0x23D, 
				 0x219, 
				 0x21E, 
				 0x20A, 
				 0x250,
				 0x250, 

	0x1BC, 0x208,
				 0x200,
	0x1BF, 0x2B9, 
				 0x26F, 
				 0x23D,
				 0x228, 
				 0x23C, 
				 0x214, 
				 0x2C8,
				 0x2C8, 

	0x1BC, 0x208,
				 0x200,
	0x1BF, 0x2BA, 
				 0x2B5, 
				 0x233, 
				 0x241, 
				 0x264, 
				 0x223, 
				 0x2A0, 
				 0x2A0,

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2BB, 
				 0x214, 
				 0x214, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2BC, 
				 0x237,
				 0x232, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2BD, 
				 0x264, 
				 0x232, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2BE, 
				 0x200, 
				 0x204, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2C0, 
				 0x200,
				 0x200, 

//Display Setting 1		 
	0x1BC, 0x228,
				 0x200,
	0x1BF, 0x2C1, 
				 0x20C,//P1  BLS, BLREV, PTLREV, REV,BGR1, SS1 
				 0x2A0,//P2  HRE1
				 0x200,//P3  NWZIG,RGBSWLP,UDS[0]£º00:Forward 01:Backward
				 0x2B2,//P4  GSWAP
				 0x20C,//P5  GSWAP
				 0x241,//P6  GSWAP 
				 0x21A,//P7  GSWAP 
				 0x20B,//P8  GSWAP 
				 0x239,//P9  GSWAP 
				 0x2E7,//P10 GSWAP 
				 0x29C,//P11 GSWAP 
				 0x273,//P12 GSWAP 
				 0x2CE,//P13 GSWAP 
				 0x2F8,//P14 GSWAP 
				 0x2DA,//P15 GSWAP 
				 0x24A,//P16 GSWAP 
				 0x2E7,//P17 GSWAP 
				 0x2A4,//P18 GSWAP 
				 0x2D5,//P19 GSWAP
				 0x25E,//P20 GSWAP
				 0x29C,//P21 GSWAP
				 0x273,//P22 GSWAP
				 0x2CE,//P23 GSWAP 
				 0x239,//P24 GSWAP 
				 0x2E7,//P25 GSWAP 
				 0x20C,//P26 GSWAP 
				 0x258,//P27 GSWAP 
				 0x21B,//P28 GSWAP 
				 0x245,//P29 GSWAP 
				 0x210,//P30 GSWAP 
				 0x209,//P31 GSWAP 
				 0x200,//P32 SPCON,SFTCKODRB,SFTCKODRF 
				 0x268,//P33 BLGCK, BLPSW, SCANMODE,SPCMODE 
				 0x201,//P34 CICMODE,FDBS_SW, BLPSH,FDBS 
				 0x202,//P35 FDBS_CK,BLSOUT1 
				 0x280,//P36 HIZ_CANCEL, BLSOUT2 
				 0x200,//P37 TOGL_EN,INITLIINE 
				 0x201,//P38 INITLIINE 
				 0x211,//P39 ABSPOL6 

	0x1BC, 0x22D,
				 0x200,
	0x1BF, 0x2C2, 
				 0x235,//P1  PSEUDO_EN,PSEUDO_LINE,PNLSEL
				 0x2F7,//P2  LINEINVA NL
				 0x280,//P3  NL
				 0x206,//P4  VDCSEL,PNSET,BP
				 0x20E,//P5  FP 
				 0x200,//P6   
				 0x200,//P7   
				 0x208,//P8   
				 0x2E5,//P9  DSPODR* 
				 0x214,//P10 DSPODR* 
				 0x294,//P11 DSPODR* 
				 0x253,//P12 DSPODR* 
				 0x250,//P13 DSPODR*
				 0x24E,//P14 DSPODR* 
				 0x241,//P15 DSPODR* 
				 0x239,//P16 DSPODR* 
				 0x205,//P17 DSPODR*
				 0x2E5,//P18 DSPODR* 
				 0x214,//P19 DSPODR* 
				 0x294,//P20 DSPODR* 
				 0x253,//P21 DSPODR* 
				 0x250,//P22 DSPODR* 
				 0x24E,//P23 DSPODR* 
				 0x241,//P24 DSPODR* 
				 0x239,//P25 DSPODR* 
				 0x205,//P26 DSPODR* 
				 0x2E5,//P27 DSPODR* 
				 0x214,//P28 DSPODR* 
				 0x294,//P29 DSPODR* 
				 0x253,//P30 DSPODR* 
				 0x250,//P31 DSPODR* 
				 0x24E,//P32 DSPODR* 
				 0x241,//P33 DSPODR* 
				 0x239,//P34 DSPODR* 
				 0x205,//P35 DSPODR* 
				 0x2E5,//P36 DSPODR* 
				 0x214,//P37 DSPODR* 
				 0x294,//P38 DSPODR* 
				 0x253,//P39 DSPODR* 
				 0x250,//P40 DSPODR* 
				 0x24E,//P41 DSPODR* 
				 0x241,//P42 DSPODR* 
				 0x239,//P43 DSPODR* 
				 0x205,//P44 DSPODR* 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2C3, 
				 0x200,
				 0x200, 
				 0x200, 

	0x1BC, 0x210,
				 0x200,
	0x1BF, 0x2C4,
				 0x230,
				 0x200, 
				 0x200, 
				 0x200, //P4  EQW5, EQW3  sets a SOUTx pre-charge(GND) period. SOUT2,3,4,5,40,41,42,43     CKV1 CKV2 CKV3 CKV4
				 0x200, //P5  EQW7, EQW6  sets a SOUTx pre-charge(GND) period. SOUT14,15,16,29,30,31       XCKH1 XCKH2 XCKH3
				 0x200, //P6  EQW9, EQW8  sets a SOUTx pre-charge(GND) period. SOUT11,12,13,32,33,34       XCKH4 XCKH5 XCKH6
				 0x200, //P7  EQW11,EQW10 sets a SOUTx pre-charge(VSP:VSN) period. SOUT2,3,4,5,40,41,42,43 CKV1 CKV2 CKV3 CKV4
				 0x200, //P8  EQW13,EQW12 sets a SOUTx pre-charge(VSP:VSN) period. SOUT14,15,16,29,30,31   XCKH1 XCKH2 XCKH3
				 0x200, //P9  EQW15,EQW14 sets a SOUTx pre-charge(VSP:VSN) period. SOUT11,12,13,32,33,34   XCKH4 XCKH5 XCKH6
				 0x200, //P10 EQW17,EQW16 sets a SOUTx pre-charge(GND) period. SOUT20,21,22,23,24,25       CKH4 CHK5 CKH6
				 0x200, //P11 EQW19,EQW18 sets a SOUTx pre-charge(GND) period. SOUT17,18,19,26,27,28       CKH1 CKH2 CKH3
				 0x200, //P12 EQW21,EQW20 sets a SOUTx pre-charge(VSP:VSN) period. SOUT20,21,22,23,24,25   CKH4 CHK5 CKH6
				 0x200, //P13 EQW23,EQW22 sets a SOUTx pre-charge(VSP:VSN) period. SOUT17,18,19,26,27,28   CKH1 CKH2 CKH3
				 0x201, 
				 0x202, 

	0x1BC, 0x209,
				 0x200,
	0x1BF, 0x2C5, 
				 0x200, 
				 0x208,
				 0x200,
				 0x200,
				 0x200,
				 0x200,
				 0x270,
				 0x200, 

	0x1BC, 0x219,
				 0x200,
	0x1BF, 0x2C6,
				 0x201, //P1  RTN
				 0x290, //P2  RTN
				 0x20E, //P3  ST1  STV rising edge
				 0x227, //P4  SW1  STV width
				 0x20E, //P5  ST2  CKV rising edge
				 0x2D2, //P6  SW2  CKV width
				 0x200, //P7  ST3
				 0x200, //P8  SW3
				 0x200, //P9  ST4  For line inversion case, you can use ST4/SW4/ST5/SW5 for pre-charge indirectly applied to source as below, the kind of pre-charge function with ST4/SW4/ST5/SW5 is like what we did for the "spike on 1st line" issue.
				 0x200, //P10 SW4
				 0x200, //P11 ST5
				 0x200, //P12 SW5
				 0x200, //P13 ST6
				 0x200, //P14 SW6
				 0x200, //P15 ST7
				 0x200, //P16 SW7
				 0x200, //P17 ST8
				 0x200, //P18 SW9
				 0x218, //P19 PSWT1 CKH rising edge .  Sets the Rising or Falling position of the 
				 0x219, //P20 PSWW1 CKH width .        Sets the High or Low period of the SOUT sig xRCLK(1/28M)  19h = 0.893uS  17h= 0.786uS  14h = 0.714uS
				 0x208, //P21 PSWG1 CKH Gap .          Sets the Non-overlap period of the SOUT sig
				 0x201, //P22 
				 0x290, //P23 
				 0x200, //P24 TRWC3/TRWC2/TRWC1/TRWC0 ¸ü¸ÄÆµÂÊ£º60-->30Hz need change this setting(00-55)

//Gamma Setting Common Set	
	0x1BC, 0x227,
	       0x200, 
	0x1BF, 0x2C7,
	       0x200, //VGMP0
				 0x21B, //VGMP1
				 0x22C, //VGMP2
				 0x240, //VGMP3
				 0x250, //VGMP4
				 0x25C, //VGMP5
				 0x274, //VGMP6
				 0x284, //VGMP7
				 0x291, //VGMP8
				 0x29C, //VGMP9
				 0x24E, //VGMP10
				 0x25A, //VGMP11
				 0x268, //VGMP12
				 0x27B, //VGMP13
				 0x284, //VGMP14
				 0x290, //VGMP15
				 0x29E, //VGMP16
				 0x2A9, //VGMP17
				 0x2B2, //VGMP18 
				 0x200, //VGMN0 
				 0x21B, //VGMN1 
				 0x22C, //VGMN2 
				 0x240, //VGMN3 
				 0x250, //VGMN4 
				 0x25C, //VGMN5 
				 0x274, //VGMN6 
				 0x284, //VGMN7 
				 0x291, //VGMN8 
				 0x29C, //VGMN9 
				 0x24E, //VGMN10 
				 0x25A, //VGMN11 
				 0x268, //VGMN12 
				 0x27B, //VGMN13 
				 0x284, //VGMN14 
				 0x290, //VGMN15 
				 0x29E, //VGMN16 
				 0x2A9, //VGMN17 
				 0x2B2, //VGMN18 	
				 
	0x1BC, 0x238,
				 0x200,
	0x1BF, 0x2C8,
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

	0x1BC, 0x214,
				 0x200,
	0x1BF, 0x2C9, 
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

	0x1BC, 0x22C,
				 0x200,
	0x1BF, 0x2CA, 
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

	0x1BC, 0x216,
				 0x200,
	0x1BF, 0x2CB,
				 0x2FF,
				 0x200, 
				 0x2FF, 
				 0x20F,
				 0x2F0, 
				 0x20F, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x241, 
				 0x200,
				 0x2FF,
				 0x20F,
				 0x220, 
				 0x208, 
				 0x2E8, 
				 0x200, 
				 0x200, 

	0x1BC, 0x202,
				 0x200,
	0x1BF, 0x2CC, 
				 0x249, 

	0x1BC, 0x21A,
				 0x200,
	0x1BF, 0x2CE,
				 0x255, 
				 0x240, 
				 0x249, 
				 0x253, 
				 0x259, 
				 0x25E, 
				 0x263, 
				 0x268, 
				 0x26E, 
				 0x274, 
				 0x27E, 
				 0x28A, 
				 0x298,
				 0x2A8,
				 0x2BB, 
				 0x2D0, 
				 0x2FF, 
				 0x204, 
				 0x200, 
				 0x204, 
				 0x204, 
				 0x242, 
				 0x200, 
				 0x269,
				 0x25A, 

	0x1BC, 0x20B,
				 0x200,
	0x1BF, 0x2D0, 
				 0x211, 	//P1   [7]BT3, [6:4]DC3, [3]BT2, [2:0]DC2 (setting VGH >(VCI2*2)10V (1EH) BT2 must be set;setting VGL<(VSN-VCI3) [-7-3.6V]=-10.6V BT3 should be set
				 0x200,   //P2
				 0x200,   //P3
				 0x214,   //P4   VLM2[6:0]:This register controls output level of GVDD and VGH Restriction: (VGH - 0.5V)>GVDD £¬ GVDD= 5.0+(VLM2 -5)*0.2V 5V¡ÜGVDD¡Ü17.4V
				 0x28F,   //P5   VLM3E:SEL VGL/GVSS, 	VLM3[5:0]:This register controls output voltage level of GVSS(VGL)     £¬ GVSS= -5.0-(VLM2 -5)*0.2V -5V¡ÜGVSS¡Ü-12V
				 0x240,   //P6   [0]:VLM2E This register controls the output specification of VGH/GVDD
				 0x219,   //P7 
				 0x219,   //P8 
				 0x209,   //P9 
				 0x200,   //P10 

	0x1BC, 0x205,
				 0x200,
	0x1BF, 0x2D1, 
				 0x204,
				 0x240,
				 0x206,
				 0x20F, 

	0x1BC, 0x205,
				 0x200,
	0x1BF, 0x2D2, 
				 0x200, 
				 0x200, 
				 0x2FF, 
				 0x2FF, 

	0x1BC, 0x21B,
				 0x200,
	0x1BF, 0x2D3, 
				 0x21B, //P1
				 0x233, //P2
				 0x2BB, //P3
				 0x277, //P4 
				 0x277, //P5 
				 0x277, //P6 
				 0x233, //P7
				 0x233, //P8
				 0x233, //P9
				 0x200, //P10 
				 0x201, //P11 
				 0x200, //P12 
				 0x200, //P13 
				 0x2DF, //P14 
				 0x2A0, //P15 
				 0x206, //P16 
				 0x25B, //P17 VPL 5B--5.2V
				 0x25B, //P18 VNL
				 0x233, //P19 
				 0x233, //P20 
				 0x272, //P21 VBTS £¨VSP LVD 4.02V£©
				 0x212, //P22 VBTS_IOVCC£¨IOVCC LVD 1.50
				 0x28A, //P23 VBTS_VCI(VCI LVD 2.14V)
				 0x207, //P24 
				 0x23D, //P25 
				 0x2BC, //P26 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2D4, 
				 0x241,
				 0x204, 
				 0x200, 

	0x1BC, 0x208,
				 0x200,
	0x1BF, 0x2D5, 
				 0x206, 
				 0x200, 
				 0x200, 
				 0x201,
				 0x221,
				 0x201, 
				 0x221, 

// 	0x1BC, 0x202,
// 				 0x200,
// 	0x1BF, 0x2D6,
// 				 0x281, //81 Default

	0x1BC, 0x202,//  T_SLPOUT=0h : NVM load OFF after exit_sleep_mode	
	       0x200, 
	0x1BF, 0x2D6,
	       0x201,
				 
	0x1BC, 0x21A,
				 0x200,
	0x1BF, 0x2D7, 
				 0x2BF, 
				 0x2F8, 
				 0x27F, 
				 0x2A8, 
				 0x2CE, 
				 0x23E, 
				 0x2FC, 
				 0x2C1, 
				 0x2F1, 
				 0x2EF, 
				 0x283, 
				 0x207, 
				 0x23F, 
				 0x210, 
				 0x27F, 
				 0x2C0, 
				 0x201, 
				 0x2E7, 
				 0x240, 
				 0x21C, 
				 0x200, 
				 0x2C0, 
				 0x280,//90: power on timing pull STV CKV VGH
				 0x201, 
				 0x200, 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2D8, 
				 0x200, 
				 0x200, 
				 0x200, 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2D9, 
				 0x200, 
				 0x200, 
				 0x20F, 

	0x1BC, 0x205,
				 0x200,
	0x1BF, 0x2DD, 
				 0x230, 
				 0x206, 
				 0x223, 
				 0x265, 

	0x1BC, 0x205,
				 0x200,
	0x1BF, 0x2DE, 
				 0x200, 
				 0x23F, 
				 0x2FF,
				 0x210, 

	0x1BC, 0x20B,
				 0x200,
	0x1BF, 0x2E1, 
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

	0x1BC, 0x207,
				 0x200,
	0x1BF, 0x2E2, 
				 0x200, 
				 0x200,
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2E3,
				 0x200,
				 0x200, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2E7, 
				 0x200, 
				 0x200, 

	0x1BC, 0x202,
				 0x200,
	0x1BF, 0x2E9, 
				 0x200, 

	0x1BC, 0x202,
				 0x200,
	0x1BF, 0x2EA, 
				 0x200, 

	0x1BC, 0x207,
				 0x200,
	0x1BF, 0x2EB, 
				 0x200, 
				 0x200, 
				 0x200,
				 0x200, 
				 0x200, 
				 0x200, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2EC, 
				 0x240, 
				 0x210, 

	0x1BC, 0x204,
				 0x200,
	0x1BF, 0x2ED, 
				 0x200,
				 0x200,
				 0x200, 

	0x1BC, 0x203,
				 0x200,
	0x1BF, 0x2EE, 
				 0x200, 
				 0x232, 
//=======================================================================================			 
// 	0x1BC, 0x202,//  T_SLPOUT=0h : NVM load OFF after exit_sleep_mode	
// 	       0x200, 
// 	0x1BF, 0x2D6,
// 	       0x201,

// 	0x1BC, 0x202,//  MCAP=3h : Lock Manufacture Command Protect				 
// 	       0x200, 
// 	0x1BF, 0x2B0,
// 	       0x203,			
//=======================================================================================
//Change to DCS
	0X1B7,	0X250,
	   	   	0X205,

//Column Address Order Right to Left
	0X1BC,	0X201,
	   	   	0X200,
	0X136,	0X200,


//display on   
	0x3FF,
		
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
	0X1BF,	0X2B0,	0X200,		//Manufacturer Command Access Protect
	0X355,

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
	0X1BF,	0X2B0,	0X200,		//Manufacturer Command Access Protect
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
// 	   	   	0X200,
// 	   	   	0X202,
	   	   	0X230,
	   	   	0X2F9,
	   	   	0X210,
					
  0X1BC,	0X202,	0X200,
	0X1BF,	0X2B0,	0X203,		//03:Manufacturer Command Access Protect ON 04:Protect off
	0X355,

	0XF00	
};

uint16_t gamma[10][64];
