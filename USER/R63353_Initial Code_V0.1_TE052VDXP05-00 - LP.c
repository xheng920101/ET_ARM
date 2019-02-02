// *****************************************************************************
//
//  R63353 Initial Code
//
//  FileName : LCD16-03_002_R63353_InitailCode_r000_20160301_for_Tianma.xlsm
//  Date  : 2016/3/10
//
//  Reference Address : O
//
//  ** Information **
//     No3
//     Tianma 104 pin
//     TE052VDXP05
//     
//     (CMOS 2CLK Interlace)
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
	   	   0X204,//OR 0X200
//Interface setting	
	0x1BC, 0x206,
	       0x200,
  0x1BF, 0x2B3, 
	       0x214, //DM
				 0x200, //IDM_AMP_EN,DIVOSC
				 0x200, //ATRMDM,TEI
				 0x200, //EPFV
				 0x200, 
//Interface ID setting		   
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2B4, 
				 0x20C, //SELDL, DSIVC
				 0x200, 
//DSI Control	
	0x1BC, 0x204, 
	       0x200, 
	0x1BF, 0x2B6, 
	       0x24B, //DSITXDIV
				 0x2DB, //DSI_THSSET
				 0x216, //RECOVER_TME, MIPI_RESTART_SYNC, MIPI_RECOVER_ON
//Back Light Control 1 (C=1 GUI)		   
	0x1BC, 0x208, 
				 0x200, 
	0x1BF, 0x2B8, 
	       0x257, //BL_PERCENT1, AUTO_BL1
				 0x23D, //BR_INI1, AUTO_IMAGE1
				 0x219, //BR_SET1
				 0x21E, //CR_SET1
				 0x20A, //APLCR_SET1
				 0x250, //SATU_INI_C1
				 0x250, //SATU_SET_C1
//Back Light Control 2 (C=2 Still)		   
	0x1BC, 0x208, 
				 0x200, 
	0x1BF, 0x2B9,
	       0x26F, //BL_PERCENT2, AUTO_BL2
				 0x23D, //BR_INI2, AUTO_IMAGE2
				 0x228, //BR_SET2
				 0x23C, //CR_SET2
				 0x214, //APLCR_SET2
				 0x2C8, //SATU_INI_C2
				 0x2C8, //SATU_SET_C2
//Back Light Control 3 (C=3 Movie)	
	0x1BC, 0x208, 
	       0x200, 
  0x1BF, 0x2BA, 
	       0x2B5, //BL_PERCENT3, AUTO_BL3
				 0x233, //BR_INI3, AUTO_IMAGE3
				 0x241, //BR_SET3
				 0x264, //CR_SET3
				 0x223, //APLCR_SET3
				 0x2A0, //SATU_INI_C3
				 0x2A0, //SATU_SET_C3
//SRE Control 1 (SRE=1 Weak setting)	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2BB, 
	       0x214, //LIGHT_BR1
				 0x214, //LIGHT_CR1
//SRE Control 2 (SRE=2 Middle setting)	
	0x1BC, 0x203, 
	       0x200, 
	0x1BF, 0x2BC,
	       0x237, //LIGHT_BR2
		     0x232, //LIGHT_CR2
//SRE Control 3 (SRE=3 Strong setting)	
	0x1BC, 0x203,
	       0x200,
	0x1BF, 0x2BD,
	       0x264,  //LIGHT_BR3
		     0x232,  //LIGHT_CR3
//Test Mode	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2BE, 
	       0x200,
		     0x204, 
//Skew Rate Adjustment	
	0x1BC, 0x203,
	       0x200,
	0x1BF, 0x2C0, 
	       0x200, //SOUTTF2, SOUTTR2, SOUTTF1, SOUTTR1 0 5 A F
		     0x200, //SOUTTF3, SOUTTR3, SOUTTF4, SOUTTR4
//Display Setting 1	
	0x1BC, 0x228,
	       0x200, 
	0x1BF, 0x2C1,
	       0x20C, //P1  BLS, BLREV, PTLREV, REV,BGR1, SS1
				 0x2A0, //P2  HRE1
				 0x210, //P3  NWZIG,RGBSWLP,UDS[0]£º00:Forward 01:Backward
				 0x272, //P4  GSWAP
				 0x294, //P5  GSWAP
				 0x29C, //P6  GSWAP
				 0x21B, //P7  GSWAP
				 0x20B, //P8  GSWAP
				 0x239, //P9  GSWAP
				 0x24F, //P10 GSWAP
				 0x25A, //P11 GSWAP
				 0x2ED, //P12 GSWAP
				 0x2C5, //P13 GSWAP
				 0x2F8, //P14 GSWAP
				 0x2DA, //P15 GSWAP
				 0x24A, //P16 GSWAP
				 0x2E7, //P17 GSWAP
				 0x2A4, //P18 GSWAP
				 0x2D5, //P19 GSWAP
				 0x25E, //P20 GSWAP
				 0x28C, //P21 GSWAP
				 0x2AF, //P22 GSWAP
				 0x2AD, //P23 GSWAP
				 0x274, //P24 GSWAP
				 0x2E6, //P25 GSWAP
				 0x20C, //P26 GSWAP
				 0x258, //P27 GSWAP
				 0x2CB, //P28 GSWAP
				 0x299, //P29 GSWAP
				 0x208, //P30 GSWAP
				 0x209, //P31 GSWAP
				 0x200, //P32 SPCON,SFTCKODRB,SFTCKODRF
				 0x260, //P33 BLGCK, BLPSW, SCANMODE,SPCMODE
				 0x202, //P34 CICMODE,FDBS_SW, BLPSH,FDBS
				 0x202, //P35 FDBS_CK,BLSOUT1
				 0x280, //P36 HIZ_CANCEL, BLSOUT2
				 0x200, //P37 TOGL_EN,INITLIINE
				 0x201, //P38 INITLIINE
				 0x211, //P39 ABSPOL6
//Display Setting 2	
	0x1BC, 0x22D,
	       0x200, 
	0x1BF, 0x2C2,
	       0x235, //P1  PSEUDO_EN,PSEUDO_LINE,PNLSEL
				 0x2F7, //P2  LINEINVA NL
				 0x280, //P3  NL
				 0x206, //P4  VDCSEL,PNSET,BP
				 0x20E, //P5  FP
				 0x200, //P6  
				 0x200, //P7  
				 0x208, //P8  
				 0x2E5, //P9  DSPODR*
				 0x214, //P10 DSPODR*
				 0x220, //P11 DSPODR*
				 0x21A, //P12 DSPODR*
				 0x25B, //P13 DSPODR*
				 0x24E, //P14 DSPODR*
				 0x201, //P15 DSPODR*
				 0x2A2, //P16 DSPODR*
				 0x2B1, //P17 DSPODR*
				 0x2E5, //P18 DSPODR*
				 0x214, //P19 DSPODR*
				 0x220, //P20 DSPODR*
				 0x21A, //P21 DSPODR*
				 0x25B, //P22 DSPODR*
				 0x24E, //P23 DSPODR*
				 0x201, //P24 DSPODR*
				 0x2A2, //P25 DSPODR*
				 0x2B1, //P26 DSPODR*
				 0x2E5, //P27 DSPODR*
				 0x214, //P28 DSPODR*
				 0x220, //P29 DSPODR*
				 0x21A, //P30 DSPODR*
				 0x25B, //P31 DSPODR*
				 0x24E, //P32 DSPODR*
				 0x201, //P33 DSPODR*
				 0x2A2, //P34 DSPODR*
				 0x2B1, //P35 DSPODR*
				 0x2E5, //P36 DSPODR*
				 0x214, //P37 DSPODR*
				 0x220, //P38 DSPODR*
				 0x21A, //P39 DSPODR*
				 0x25B, //P40 DSPODR*
				 0x24E, //P41 DSPODR*
				 0x201, //P42 DSPODR*
				 0x2A2, //P43 DSPODR*
				 0x2B1, //P44 DSPODR*
//TPC Sync Control	
	0x1BC, 0x204,
	       0x200, 
	0x1BF, 0x2C3,
	       0x200, //VSOD, HSOM, TPSYNEN
				 0x200, //HSOD
				 0x200, //HSOHW
//Source Timing Setting	
	0x1BC, 0x210,
	       0x200, 
	0x1BF, 0x2C4,
	       0x230, //P1  DIV
				 0x200, //P2
				 0x200, //P3
				 0x200, //P4  EQW5, EQW3  sets a SOUTx pre-charge(GND) period. SOUT2,3,4,5,40,41,42,43     CKV1 CKV2 CKV3 CKV4
				 0x233, //P5  EQW7, EQW6  sets a SOUTx pre-charge(GND) period. SOUT14,15,16,29,30,31       XCKH1 XCKH2 XCKH3
				 0x233, //P6  EQW9, EQW8  sets a SOUTx pre-charge(GND) period. SOUT11,12,13,32,33,34       XCKH4 XCKH5 XCKH6
				 0x200, //P7  EQW11,EQW10 sets a SOUTx pre-charge(VSP:VSN) period. SOUT2,3,4,5,40,41,42,43 CKV1 CKV2 CKV3 CKV4
				 0x200, //P8  EQW13,EQW12 sets a SOUTx pre-charge(VSP:VSN) period. SOUT14,15,16,29,30,31   XCKH1 XCKH2 XCKH3
				 0x200, //P9  EQW15,EQW14 sets a SOUTx pre-charge(VSP:VSN) period. SOUT11,12,13,32,33,34   XCKH4 XCKH5 XCKH6
				 0x233, //P10 EQW17,EQW16 sets a SOUTx pre-charge(GND) period. SOUT20,21,22,23,24,25       CKH4 CHK5 CKH6
				 0x233, //P11 EQW19,EQW18 sets a SOUTx pre-charge(GND) period. SOUT17,18,19,26,27,28       CKH1 CKH2 CKH3
				 0x200, //P12 EQW21,EQW20 sets a SOUTx pre-charge(VSP:VSN) period. SOUT20,21,22,23,24,25   CKH4 CHK5 CKH6
				 0x200, //P13 EQW23,EQW22 sets a SOUTx pre-charge(VSP:VSN) period. SOUT17,18,19,26,27,28   CKH1 CKH2 CKH3
				 0x201, //P14 SNT1
				 0x205, //P15 SNT2
//Realtime Scaling & cross hair	
	0x1BC, 0x209,
	       0x200, 
	0x1BF, 0x2C5, 
	       0x200, //P1  RTSON
				 0x208, //P2  CROSS_CTL,CROSS_WIDTH
				 0x200, //P3  X_CROSS
				 0x200, //P4  X_CROSS
				 0x200, //P5  Y_CROSS
				 0x200, //P6  Y_CROSS
				 0x270, //P7  SELECT_COLOR,UNSELECT_COLOR
				 0x200, //P8  BASE_COLOR
//LTPS Timing Setting	
//STx:  Sets the Rising or Falling position of the SOUT signal
//SWx:  Sets the High or Low period of the SOUT signal.
	0x1BC, 0x219, 
	       0x200, 
	0x1BF, 0x2C6, 
	       0x201, //P1  RTN
				 0x290, //P2  RTN
				 0x2C4, //P3  ST1  STV rising edge
				 0x227, //P4  SW1  STV width
				 0x20E, //P5  ST2  CKV rising edge
				 0x2D2, //P6  SW2  CKV width
				 0x200, //P7  ST3
				 0x200, //P8  SW3
				 0x200, //P9  ST4
				 0x200, //P10 SW4
				 0x200, //P11 ST5
				 0x200, //P12 SW5
				 0x200, //P13 ST6
				 0x200, //P14 SW6
				 0x200, //P15 ST7
				 0x200, //P16 SW7
				 0x200, //P17 ST8
				 0x200, //P18 SW9
				 0x218, //P19 PSWT1 CKH rising edge .  Sets the Rising or Falling position of the SOUT 
				 0x219, //P20 PSWW1 CKH width .        Sets the High or Low period of the SOUT signal  xRCLK(1/28M)  19h = 0.893uS  17h= 0.786uS  14h = 0.714uS
				 0x208, //P21 PSWG1 CKH Gap .          Sets the Non-overlap period of the SOUT signal
				 0x201, //P22 
				 0x290, //P23 
				 0x200, //P24 TRWC3/TRWC2/TRWC1/TRWC0 ¸ü¸ÄÆµÂÊ£º60-->30Hz (00-55)need change this setting to magnification timing
			 
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
//Digital Gamma Setting	
	0x1BC, 0x238,
	       0x200, 
	0x1BF, 0x2C8,
	       0x200, //P1  GAMADJ    //0x01:enable digital Gamma 0x00:disable digital Gamma
				 0x200, //P2  BR0_R_TYP,
				 0x200, //P3  GAM0_R_TYP
				 0x200, //P4  GAM1_R_TYP 
				 0x200, //P5  GAM2_R_TYP 
				 0x2FC, //P6  BR1_R_TYP 
				 0x200, //P7  TILT1_R_TYP, TILT0_R_TYP 
				 0x200, //P8  BR0_G_TYP 
				 0x200, //P9  GAM0_G_TYP 
				 0x200, //P10 GAM1_G_TYP  
				 0x200, //P11 GAM2_G_TYP  
				 0x2FC, //P12 BR1_G_TYP  
				 0x200, //P13 TILT1_G_TYP, TILT0_G_TYP  
				 0x200, //P14 BR0_B_TYP  
				 0x200, //P15 GAM0_B_TYP  
				 0x200, //P16 GAM1_B_TYP
				 0x200, //P17 GAM2_B_TYP  
				 0x2FC, //P18 BR1_B_TYP  
				 0x200, //P19 TILT1_B_TYP, TILT0_B_TYP 
				 0x200, //P20 BR0_R_HI  
				 0x200, //P21 GAM0_R_HI  
				 0x200, //P22 GAM1_R_HI 
				 0x200, //P23 GAM2_R_HI  
				 0x2FC, //P24 BR1_R_HI  
				 0x200, //P25   
				 0x200, //P26 BR0_G_HI  
				 0x200, //P27 GAM0_G_HI  
				 0x200, //P28 GAM1_G_HI  
				 0x200, //P29 GAM2_G_HI  
				 0x2FC, //P30 BR1_G_HI  
				 0x200, //P31   
				 0x200, //P32 BR0_B_HI  
				 0x200, //P33 GAM0_B_HI  
				 0x200, //P34 GAM1_B_HI  
				 0x200, //P35 GAM2_B_HI  
				 0x2FC, //P36 BR1_B_HI 
				 0x200, //P37   
				 0x200, //P38 BR0_R_LO  
				 0x200, //P39 GAM0_R_LO  
				 0x200, //P40 GAM1_R_LO  
				 0x200, //P41 GAM2_R_LO  
				 0x2FC, //P42 BR1_R_LO  
				 0x200, //P43   
				 0x200, //P44 BR0_G_LO  
				 0x200, //P45 GAM0_G_LO  
				 0x200, //P46 GAM1_G_LO  
				 0x200, //P47 GAM2_G_LO  
				 0x2FC, //P48 BR1_G_LO  
				 0x200, //P49   
				 0x200, //P50 BR0_B_LO  
				 0x200, //P51 GAM0_B_LO  
				 0x200, //P52 GAM1_B_LO  
				 0x200, //P53 GAM2_B_LO  
				 0x2FC, //P54 BR1_B_LO  
				 0x200, //P55   
//Digital Gamma Setting2	
	0x1BC, 0x214,
	       0x200, 
	0x1BF, 0x2C9,
	       0x200, //P1 
				 0x200, //P2  BR0_R2_TYP
				 0x200, //P3  GAM0_R2_TYP
				 0x200, //P4  GAM1_R2_TYP
				 0x200, //P5  GAM2_R2_TYP
				 0x2FC, //P6  BR1_R2_TYP
				 0x200, //P7  TILT1_R2_TYP, TILT0_R2_TYP
				 0x200, //P8  BR0_G2_TYP
				 0x200, //P9  GAM0_G2_TYP
				 0x200, //P10 GAM1_G2_TYP
				 0x200, //P11 GAM2_G2_TYP
				 0x2FC, //P12 BR1_G2_TYP
				 0x200, //P13 TILT1_G2_TYP, TILT0_G2_TYP
				 0x200, //P14 BR0_B2_TYP
				 0x200, //P15 GAM0_B2_TYP 
				 0x200, //P16 GAM1_B2_TYP
				 0x200, //P17 GAM2_B2_TYP
				 0x2FC, //P18 BR1_B2_TYP
				 0x200, //P19 TILT1_B2_TYP, TILT0_B2_TYP
//Color Enhancementt	
	0x1BC, 0x22C,
	       0x200, 
	0x1BF, 0x2CA,
	       0x21C, //P1  DITHER_KIND, CSM_HI_LIM, CSM_LOW_LIM, SEL_COLOR_MIX, CSM_FRC_ON, CSM_ON
				 0x2FC, //P2  W_POINT_R
				 0x2FC, //P3  W_POINT_G
				 0x2FC, //P4  W_POINT_B
				 0x200, //P5  R_POINT_R
				 0x200, //P6  R_POINT_G
				 0x200, //P7  R_POINT_B
				 0x200, //P8  G_POINT_R
				 0x200, //P9  G_POINT_G
				 0x200, //P10 G_POINT_B
				 0x200, //P11 B_POINT_R
				 0x200, //P12 B_POINT_G
				 0x200, //P13 B_POINT_B
				 0x200, //P14 C_POINT_R
				 0x200, //P15 C_POINT_G
				 0x200, //P16 C_POINT_B
				 0x200, //P17 M_POINT_R
				 0x200, //P18 M_POINT_G
				 0x200, //P19 M_POINT_B
				 0x200, //P20 Y_POINT_R
				 0x200, //P21 Y_POINT_G
				 0x200, //P22 Y_POINT_B
				 0x200, //P23 SEL_COLOR0_BEF_R
				 0x200, //P24 SEL_COLOR0_BEF_G
				 0x200, //P25 SEL_COLOR0_BEF_B
				 0x200, //P26 SEL_COLOR0_AFT_R
				 0x200, //P27 SEL_COLOR0_AFT_G
				 0x200, //P28 SEL_COLOR0_AFT_B
				 0x200, //P29 SEL_COLOR0_AREA
				 0x200, //P30 SEL_COLOR1_BEF_R
				 0x200, //P31 SEL_COLOR1_BEF_G
				 0x200, //P32 SEL_COLOR1_BEF_B
				 0x200, //P33 SEL_COLOR1_AFT_R
				 0x200, //P34 SEL_COLOR1_AFT_G
				 0x200, //P35 SEL_COLOR1_AFT_B
				 0x200, //P36 SEL_COLOR1_AREA
				 0x200, //P37 SEL_COLOR2_BEF_R
				 0x200, //P38 SEL_COLOR2_BEF_G
				 0x200, //P39 SEL_COLOR2_BEF_B
				 0x200, //P40 SEL_COLOR2_AFT_R
				 0x200, //P41 SEL_COLOR2_AFT_G
				 0x200, //P42 SEL_COLOR2_AFT_B
				 0x200, //P43 SEL_COLOR2_AREA
//Panel PIN Control	
	0x1BC, 0x216,
	       0x200, 
	0x1BF, 0x2CB,
	       0x2E7, //P1  SOUTEN
				 0x2FC, //P2  SOUTEN
				 0x2FF, //P3  SOUTEN
				 0x2FF, //P4  SOUTEN
				 0x273, //P5  SOUTEN
				 0x20E, //P6  SOUTEN 
				 0x200, //P7  SOUTPL 
				 0x2FC, //P8  SOUTPL 
				 0x200, //P9  SOUTPL 
				 0x2F0, //P10 SOUTPL
				 0x203, //P11 SOUTPL
				 0x200, //P12 SOUTPL
				 0x2C7, //P13 SOUTABSPL
				 0x2FC, //P14 SOUTABSPL
				 0x2FF, //P15 SOUTABSPL
				 0x2FF, //P16 SOUTABSPL
				 0x233, //P17 SOUTABSPL
				 0x20E, //P18 SOUTABSPL
				 0x2E8, //P19 SOUTABSPLEN, SOUTHLV39, SOUTHLV6
				 0x200, //P20 SOUTLLVL(40,29,23,17,11,2)
				 0x200, //P21 SOUTHLVL(40,29,23,17,11,2)
//Panel Interface Control	
	0x1BC, 0x202,
	       0x200, 
	0x1BF, 0x2CC,
	       0x242, //LIM
//Back Light Control 4	
	0x1BC, 0x21A,
	       0x200, 
	0x1BF, 0x2CE,
	       0x255, //P1  PWM_DITH_ON,MODE_DIM, CABC_PTLON, IMG_DIM
				 0x240, //P2  BL_DATA0  
				 0x249, //P3  BL_DATA1
				 0x253, //P4  BL_DATA2
				 0x259, //P5  BL_DATA3
				 0x25E, //P6  BL_DATA4
				 0x263, //P7  BL_DATA5
				 0x268, //P8  BL_DATA6 
				 0x26E, //P9  BL_DATA7 
				 0x274, //P10 BL_DATA8
				 0x27E, //P11 BL_DATA9
				 0x28A, //P12 BL_DATA10
				 0x298, //P13 BL_DATA11
				 0x2A8, //P14 BL_DATA12
				 0x2BB, //P15 BL_DATA13
				 0x2D0, //P16 BL_DATA14
				 0x2FF, //P17 BL_DATA15
				 0x204, //P18 PWM_DIV
				 0x200, //P19 PWM_CYCLE
				 0x204, //P20 PWM_PERIOD_UP
				 0x204, //P21 PWM_PERIOD_DOWN
				 0x242, //P22 PWM_METHOD_DOWN, PWM_METHOD_UP
				 0x200, //P23 PWMWM, FRC_ON, PWM_BIT_LENGTH
				 0x269, //P24 BR_AREA_RATIO
				 0x25A, //P25 CR_AREA_RATIO
//Power Setting for Charge Pump	
	0x1BC, 0x20B, 
	       0x200, 
	0x1BF, 0x2D0,
	       0x211, //P1  BT3, DC3, BT2, DC2
				 0x200, //P2
				 0x200, //P3
				 0x214, //P4  VLM2[6:0]:This register controls output level of GVDD and VGH Restriction: (VGH - 0.5V)>GVDD
				 0x28F, //P5  VLM3E, VLM3[6:0]
				 0x240, //P6  VLM2E
				 0x219, //P7 
				 0x219, //P8 
				 0x209, //P9 
				 0x200, //P10 
//External Charge Pump Setting	
	0x1BC, 0x205,
	       0x200, 
	0x1BF, 0x2D1, 
	       0x204, //P1  DCDSCEN1, BT1 
				 0x240, //P2  EXCPSEL, CPSEL
				 0x206, //P3  DC1CLK
				 0x20F, //P4  VLMT1
//Test Register	
	0x1BC, 0x205, 
	       0x200,
	0x1BF, 0x2D2,
         0x200, //P1
				 0x200, //P2 
				 0x2FF, //P3 
				 0x2FF, //P4 
//Power Setting for Internal Power		   
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
				 0x200, //P10 SEQGND1, SEQGND2
				 0x201, //P11 
				 0x200, //P12 
				 0x200, //P13 
				 0x2DF, //P14 VC2
				 0x2A0, //P15 
				 0x206, //P16 VC3
				 0x25B, //P17 VPL 5B--5.2V
				 0x25B, //P18 VNL
				 0x233, //P19 set source driving  33->77
				 0x233, //P20 
				 0x272, //P21 VBTS
				 0x212, //P22 VBTS_IOVCC
				 0x28A, //P23 VBTS_VCI
				 0x207, //P24 GVDDDRCT, GVSSDRCT
				 0x23D, //P25 
				 0x2BC, //P26 
//Glass Break Detection Control	
	0x1BC, 0x204,
	       0x200, 
	0x1BF, 0x2D4,
	       0x241, //P1  GBD_EN
				 0x204, //P2  GBD_DET_RESI
				 0x200, //P3 
//VCOM Setting	
	0x1BC, 0x208,
	       0x200, 
	0x1BF, 0x2D5, 
	       0x206, //P1 WC_VDC, WC_VDDB 
				 0x200, //P2 
				 0x200, //P3 
				 0x201, //P4 VDC
				 0x221, //P5 VDC
				 0x201, //P6 VDCB
				 0x221, //P7 VDCB
//Sequencer Test Control	
// 	0x1BC, 0x202,
// 	       0x200, 
// 	0x1BF, 0x2D6,
// 	       0x281, //T_SLPOUT

	0x1BC, 0x202,   //  T_SLPOUT=0h : NVM load OFF after exit_sleep_mode
	       0x200, 
	0x1BF, 0x2D6,
	       0x201, 
//Sequencer Timing Control	
	0x1BC, 0x21A, 
	       0x200, 
	0x1BF, 0x2D7, 
	       0x2BF, //P1  TM
	       0x2F8, //P2  TM 
				 0x27F, //P3  TM 
				 0x2A8, //P4  TM 
				 0x2CE, //P5  TM 
				 0x23E, //P6  TM 
				 0x2FC, //P7  TM
				 0x2C1, //P8  TM 
				 0x2F1, //P9  TM 
				 0x2EF, //P10 TM
				 0x283, //P11 TM
				 0x207, //P12 TM
				 0x23F, //P13 TM
				 0x210, //P14 TM
				 0x27F, //P15 TM
				 0x2C0, //P16 TM
				 0x201, //P17 TM
				 0x2E7, //P18 TM
				 0x240, //P19 TM
				 0x21C, //P20 TM
				 0x200, //P21 TM
				 0x2C0, //P22 TM
				 0x290, //P23 TM
				 0x201, //P24 TM
				 0x200, //P25 TM 
//Test Register	
	0x1BC, 0x204,
	       0x200,
	0x1BF, 0x2D8, 
	       0x200,
				 0x200,
				 0x200, 
//Sequencer Control	
	0x1BC, 0x204,
	       0x200, 		
	0x1BF, 0x2D9,
	       0x200,//P1  INITST
				 0x200,//P2  OFFINIT,OFFSOURCE,INITCNT
				 0x2FF,//P3  OFFGATE1,OFFGATE0,OFFSRCSW1,OFFSRCSW0 
//Outline Sharpening Control	
	0x1BC, 0x205,
	       0x200, 
	0x1BF, 0x2DD,
	       0x230,//P1  NR, EGE_ON
				 0x206,//P2  OFFSET
				 0x223,//P3  EDGE
				 0x265,//P4  OS_LIM
//Test Image Generator	
	0x1BC, 0x205,
	       0x200, 
	0x1BF, 0x2DE, 
	       0x200,//P1  TIGCYC, TIGON
				 0x23F,//P2  TIP
				 0x2FF,//P3  TIP
				 0x210,//P4 
//set_DDB write Control	
	0x1BC, 0x20B, 
	       0x200, 
	0x1BF, 0x2E1,
	       0x200,//P1  WCIFAD,WCDDB
				 0x200,//P2  
				 0x200,//P3   
				 0x200,//P4  DDB0 
				 0x200,//P5  DDB1 
				 0x200,//P6  DDB2
				 0x200,//P7  DDB3 
				 0x200,//P8  DDB4 
				 0x200,//P9  DDB5 
				 0x200,//P10 IFID0 
//Read ID Code	
	0x1BC, 0x207,
	       0x200, 
	0x1BF, 0x2E2,
	       0x200,//P1 WCRDID
				 0x200,//P2 
				 0x200,//P3 
				 0x200,//P4 RDID1
				 0x200,//P5 RDID2
				 0x200,//P6 RDID2
//NVM Load Control	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2E3,
	       0x200,//NVMLD
				 0x200, 
//Test Register	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2E7,
	       0x200, 
		     0x200, 
//Read Chip Information Enable	
	0x1BC, 0x202,
	       0x200, 
	0x1BF, 0x2E9,
	       0x200,//WCDDBCOL 
//ESD detect mode	
	0x1BC, 0x202, 
	       0x200, 
	0x1BF, 0x2EA,
	       0x200,//ESD_ABS,ESD_TE_EN,REG_CHKSUM_E,MIPI_ERR_FLAG_EN,AB_STATE_EN,AB_HSYNC_EN 
//Panel information	
	0x1BC, 0x207,
	       0x200, 
	0x1BF, 0x2EB,
	       0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
				 0x200, 
//Panel synchronous output	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2EC,
	       0x240,
				 0x210, 
//Panel synchronous output	
	0x1BC, 0x204, 
	       0x200, 
	0x1BF, 0x2ED,
	       0x200, 
				 0x200, 
				 0x200, 
//Panel synchronous output	
	0x1BC, 0x203,
	       0x200, 
	0x1BF, 0x2EE,
	       0x200, 
				 0x232, 
//=======================================================================================			 	 
// 	0x1BC, 0x202,		//  MCAP=3h : Lock Manufacture Command Protect
// 	       0x200, 
// 	0x1BF, 0x2B0,
// 	       0x203,		 
//==================================CABC Setting=========================================
// 	0x1BC,	0x202,	0x200,
// 	0x1BF,	0x251,	0x2ff, // Duty 50%
// 	0x1BF,	0x253,	0x20C, //
// 	0x1BF,	0x255,	0x200,
// 	0x3FF,
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
