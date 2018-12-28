#include "include.h" 

//PCLK=150MHz, MIPI=960Mbps, frame rate = 60.03Hz
#define		SSD_HACT	1080
#define		SSD_VACT	2160
#define		SSD_HSA		20  
#define		SSD_HBP		100    //20181207ÐÞ¸Ä
#define		SSD_HFP		80     //20181207
#define		SSD_VSA		2
#define		SSD_VBP		8
#define		SSD_VFP		14

FPGAPORCH_TypeDef FPGA_porchPara = 
{ 
	SSD_HACT,	//FPGA_HAC
	SSD_VACT,	//FPGA_VAC
	SSD_HSA,	//FPGA_HSA
	SSD_HBP - SSD_HSA,	//FPGA_HBP
	SSD_HFP,	//FPGA_HFP
	SSD_VSA,	//FPGA_VSA
	SSD_VBP - SSD_VSA,	//FPGA_VBP
	SSD_VFP 	//FPGA_VFP
};

unsigned short SSDInitCode[]= 
{
	0X1B7,	0X250,	0X205,
	0X1BA,	0X2A0,	0X2C4,		//24MHz * 160 / 4 = 960Mbps
	0X1BB,	0X20F,	0X200,		//960 / 16 / 8 = 7.5MHz, 20151026 for OTP fail changed from 0A to 0F;
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
	0X1B7,	0X210,	0X203,

	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x207, // Change to Page 7
	
	0x1BC,	0x202,	0x200, 
	0x1BF,	0x242,	0x26C, // no re-load OTP after R11h, mustn't be OTP and can't be OTP.
	
	//=======================================================================================//
	//************************************************************************************//
	// Display and Power 
	//************************************************************************************//
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x201, // Change to Page 1
		
	0x1BC,	0x202,	0x200,
	//0x1BF,	0x258,	0x201, // Deep Standby (After this code need delay at lest 120ms.)

	0x1BF,	0x222,	0x206, // b3=BGR(0:RGB,1:BGR); b2=NBWSEL(0:NW,1:NB); b1=SS(0:S0-S1080,1:S1080-S0); b0=GS(0:U2D,1:D2U)
	0x1BF,	0x22E,	0x237, // NL[7:0]
	0x1BF,	0x22F,	0x204, // NL[10:8]
	0x1BF,	0x235,	0x222, // b5:4]=frame_chopper, b[1:0]=line_chopper; add@V3: improve dotcheck vertical jitter 
	0x1BF,	0x236,	0x200, // b1=DEMUX1_3(0:2to6,1:1to3); b0=PXL_INV(0:sub-pixel,1:pixel)
	0x1BF,	0x242,	0x211, // VGH=x4 VGL=x4
	0x1BF,	0x243,	0x2B2, // D[7]=0(Disable), 1(Enable) VGH_CLP = 11.0V(32)
	0x1BF,	0x244,	0x294, // D[7]=0(Disable), 1(Enable) VGL_CLP = -11.0V(3C)  9E(-8V) ->94(-7v)
	0x1BF,	0x245,	0x228, // VGHO = 10.0V(28), VGHO<=VGH-0.5V 
	0x1BF,	0x246,	0x21E, // VGLO = -7.0V(1E), VGLO>=VGL+0.5V 
	0x1BF,	0x250,	0x264, // GVDDP = 5.0V(64)
	0x1BF,	0x251,	0x264, // GVDDN = -5.0V(64)

	0x1BF,	0x2A1,	0x233, // Select Vcom setting before and after OTP, mustn't and can't be OTP. If the panel had already been OTP, 00: use OTP value, 33: use initial value.
	0x1BF,	0x2A2,	0x201, // VCOM1_MSB 
	0x1BF,	0x2A3,	0x21C, // VCOM1_LSB,

	0x1BF,	0x2B3,	0x270, // VGHO OP enable
	0x1BF,	0x2B4,	0x260, // VGLO OP enable  70->60 disable op

	0x1BF,	0x257,	0x288, // B7=LVD_EN, B3=LVD_MODE: 1->IOVCC/VSP/VSN enable
	0x1BF,	0x259,	0x230, // LVD VCI:2.1V  IOVCC:1.25V
	0x1BF,	0x25A,	0x255, // LVD VSP:4.0V  VSN:-4.0V
	
	//Source pre-charge
	0x1BF,	0x260,	0x210, // EQTP
	0x1BF,	0x263,	0x20B, // SDT  04->0B change@V3 adjust source hold time
	0x1BF,	0x264,	0x200, // EQT
	0x1BF,	0x265,	0x200, // EQT2
	0x1BF,	0x266,	0x200, // PCT
	0x1BF,	0x26C,	0x245, // b[7:6]=PRCB(0:hi-z,1:keep,2:GND); b[5:3]=PRC_IDLE; b[2:0]=PRC_NORMAL
	0x1BF,	0x26D,	0x200, // PCT2[6:0]

	//Source behavior in V-blanking period.
//	0x1BF,	0x26A,	0x200, // VP_NON_CKH_DUM area set VCI/VCL(04h), GND(00h), Hi-Z(03h).
//	0x1BF,	0x26B,	0x244, // VFP_CKH_DUM area and VBP_CKH_DUM area set VCI/VCL(4h), GND(0h), Hi-Z(3h).

	//=======================================================================================//
	//************************************************************************************//
	// Timing
	//************************************************************************************//
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x206, // Change to Page 6

	0x1BC,	0x202,	0x200, 
	0x1BF,	0x200,	0x241, // STV_A_Rise[6:5]: D[7:6]=01, STV_A_Rise[4:0]: D[4:0]=00001
	0x1BF,	0x201,	0x215, // FTI_A_Fall_mode: D[4]=1 
	0x1BF,	0x202,	0x206, // FTI_A_Rise[7:0]
	0x1BF,	0x203,	0x21B, // FTI_A_Fall[7:0]: change@V3: 0B-->1B, follow new timing(18-9 NMOS V2)
	0x1BF,	0x204,	0x200, // STV_B_Rise[6:5]: D[7:6], STV_B_Rise[4:0]: D[4:0]
	0x1BF,	0x205,	0x200, // FTI_B_Fall_mode: D[4], Phase_STV_B[1:0]: D[3:2], Overlap_STV_B[1:0]: D[1:0]
	0x1BF,	0x206,	0x21E, // FTI_B_Rise[7:0]
	0x1BF,	0x207,	0x200, // FTI_B_Fall[7:0]
	0x1BF,	0x208,	0x281, // CLK_A_Rise[5]: D[7]=1, CLK_A_Rise[4:0]: D[4:0]=00001
	0x1BF,	0x209,	0x202, // CLK_A_Fall[5]: D[7]=0, CLK_A_Fall[4:0]: D[4:0]=00010
	0x1BF,	0x20A,	0x230, // Phase_CLK_A[2:0]: D[6:4]=011, Overlap_CLK_A[2:0]: D[2:0]=000
	0x1BF,	0x20B,	0x201, // CLK_A_ct_mode: D[5], CLK_A_Keep: D[4], CLW_A_Rise_mode: D[1], CLW_A_Fall_mode: D[0]
	0x1BF,	0x20C,	0x20A, // CLW_A1_Rise[7:0]
	0x1BF,	0x20D,	0x20A, // CLW_A2_Rise[7:0]
	0x1BF,	0x20E,	0x262, // CLW_A1_Fall[7:0]
	0x1BF,	0x20F,	0x262, // CLW_A2_Fall[7:0]

	//CKV EQ setting
	0x1BF,	0x210,	0x200, // CLK_A_Rise_eqt1[7:0]
	0x1BF,	0x211,	0x200, // CLK_A_Rise_eqt2[7:0]
	0x1BF,	0x212,	0x200, // CLK_A_Fall_eqt1[7:0]
	0x1BF,	0x213,	0x200, // CLK_A_Fall_eqt2[7:0]
	
	0x1BF,	0x214,	0x200, // 
	0x1BF,	0x215,	0x200, // 

	//Forward Scanning Right VSR.
	0x1BF,	0x231,	0x222, // GAS
	0x1BF,	0x232,	0x212, // CKV4
	0x1BF,	0x233,	0x210, // CKV2
	0x1BF,	0x234,	0x213, // CKV3
	0x1BF,	0x235,	0x211, // CKV1
	0x1BF,	0x236,	0x208, // STVR
	0x1BF,	0x237,	0x201, // U2D
	0x1BF,	0x238,	0x200, // D2U
	0x1BF,	0x239,	0x207, //
	0x1BF,	0x23A,	0x202, // VTSW

	0x1BF,	0x23B,	0x228, // CKHR
	0x1BF,	0x23C,	0x229, // CKHG
	0x1BF,	0x23D,	0x22A, // CKHB
	0x1BF,	0x23E,	0x207, // 
	0x1BF,	0x23F,	0x207, // 
	0x1BF,	0x240,	0x207, //

	//Forward Scanning Left VSR.
	0x1BF,	0x241,	0x222, // GAS
	0x1BF,	0x242,	0x213, // CKV3
	0x1BF,	0x243,	0x211, // CKV1
	0x1BF,	0x244,	0x212, // CKV4
	0x1BF,	0x245,	0x210, // CKV2
	0x1BF,	0x246,	0x209, // STVL
	0x1BF,	0x247,	0x201, // U2D
	0x1BF,	0x248,	0x200, // D2U
	0x1BF,	0x249,	0x207, // VTSW
	0x1BF,	0x24A,	0x202, // 

	0x1BF,	0x24B,	0x228, // CKHR
	0x1BF,	0x24C,	0x229, // CKHG
	0x1BF,	0x24D,	0x22A, // CKHB
	0x1BF,	0x24E,	0x207,
	0x1BF,	0x24F,	0x207,
	0x1BF,	0x250,	0x207, 

	//Backward Scanning Right VSR.
	0x1BF,	0x261,	0x222, // GAS
	0x1BF,	0x262,	0x211, // CKV4
	0x1BF,	0x263,	0x213, // CKV2
	0x1BF,	0x264,	0x210, // CKV3
	0x1BF,	0x265,	0x212, // CKV1
	0x1BF,	0x266,	0x209, // STVR
	0x1BF,	0x267,	0x201, // U2D
	0x1BF,	0x268,	0x200, // D2U
	0x1BF,	0x269,	0x207, //
	0x1BF,	0x26A,	0x202, // VTSW

	0x1BF,	0x26B,	0x228, // CKHR
	0x1BF,	0x26C,	0x229, // CKHG
	0x1BF,	0x26D,	0x22A, // CKHB
	0x1BF,	0x26E,	0x207, // 
	0x1BF,	0x26F,	0x207, // 
	0x1BF,	0x270,	0x207, // 

	//Backward Scanning Left VSR.
	0x1BF,	0x271,	0x222, // GAS
	0x1BF,	0x272,	0x210, // CKV3
	0x1BF,	0x273,	0x212, // CKV1
	0x1BF,	0x274,	0x211, // CKV4
	0x1BF,	0x275,	0x213, // CKV2
	0x1BF,	0x276,	0x208, // STVL
	0x1BF,	0x277,	0x201, // U2D
	0x1BF,	0x278,	0x200, // D2U
	0x1BF,	0x279,	0x207, //
	0x1BF,	0x27A,	0x202, // VTSW

	0x1BF,	0x27B,	0x228, // CKHB
	0x1BF,	0x27C,	0x229, // CKHG
	0x1BF,	0x27D,	0x22A, // CKHR
	0x1BF,	0x27E,	0x207, // 
	0x1BF,	0x27F,	0x207, // 
	0x1BF,	0x280,	0x207, // 
	
	//CKH setting
	0x1BF,	0x2A0,	0x206, // T8[5:0]: gap between HSYNC and CKH 
	0x1BF,	0x2A1,	0x200, // T7P[5:0]: CKH precharge
	0x1BF,	0x2A2,	0x20B, // T9P[7:4]: CKHR continue setting, T9[3:0]: CKH GAP, 
	0x1BF,	0x2A3,	0x219, // T7[5:0]: CKH width  
	0x1BF,  0x2A6,  0x211, // CKH_VBP[3:0], CKH_VFP[3:0] dummy line number; change@V3: 00->11 for RGBBGR first line issue
	0x1BF,  0x2A7,  0x203, // b7=Vporch CKH toggle(0:disable; 1:toggle) change@V3: 00->03 for RGBBGR
	0x1BF,  0x2AE,  0x214, // add@V3 for RGBBGR

	//CKH EQ
	0x1BF,	0x2B1,	0x200, // CKH to VSP/VSN
	0x1BF,	0x2B2,	0x233, // CKH to GND, 08:before modulation  88:before & after modulation    power saving
	0x1BF,	0x2B3,	0x200, // CKH to VSP/VSN, 08:before modulation  88:before & after modulation    power saving
	0x1BF,	0x2B4,	0x200, // 
	0x1BF,	0x2B5,	0x200, //
	0x1BF,	0x2B6,	0x200, //

	//CKH/CKV slew rate
	0x1BF,	0x2E3,	0x2FF, // GIP_MUX2_SR[7:6]: CKH, GIP_MUX1_SR[5:4]: XCKH, GIP_GCK2_SR[3:2] GIP_GCK1_SR[1:0]: CKV.
	//default: 0xFF(SR OFF), 00: 1 switch turn on, 01: 2 switch turn on, 10: 3 switch turn on, 11: 4 switch turn on
	
	0x1BF,	0x2E5,	0x203, //03
		
	//Power On / Abnormal Power Off / Sleep in sequence setting
	0x1BF,	0x297,	0x222, // set number of blank frame
	0x1BF,	0x298,	0x213, // Set source output voltage level at blank frame
	0x1BF,	0x2D0,	0x201, // b0 = tcon_Lvd_opt, 1: enable, 0: disable
	0x1BF,	0x2D1,	0x222, // b[5:4] = onseq_ext,  b[2:0] = offseq_ext
	0x1BF,	0x2D2,	0x208, //
	0x1BF,	0x2DA,	0x200, // GAS to low
	0x1BF,	0x2DB,	0x240, // CKH, CKV, STV abnormal power off sequence  40
	0x1BF,	0x2DC,	0x201, // add@V3: APO timing change U2D from HIGH to LOW
	0x1BF,	0x2DD,	0x251, // Vcom to GND after 28h   
	
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x207, // Change to Page 7
	0x1BC,	0x202,	0x200, 
	
	//MIPI time out setting
	0x1BF,	0x232,	0x259, // Default=79: MIPI time out off, 59: MIPI time out on.

	//Power Saving(Bias setting)
	0x1BF,	0x220,	0x243, // default value: 43,  (SOURCE BIAS  46      power saving)
	0x1BF,	0x253,	0x200, // default value: 00,  (GAMMA OP  22         power saving)

	//1H setting for BIST mode frame rate
	0x1BF,	0x23A,	0x274, //RTNA[7:0]
	
//	0x1BC,	0x204,	0x200, 
//	0x1BF,	0x2FF,	0x278,	0x207,	0x208, // Change to Page 8
// 	
//	0x1BC,	0x202,	0x200,
//	0x1BF,	0x228,	0x2C0, // GVDDP/N output enable
//	0x1BF,	0x265,	0x2D0, //reset sequence follow sleep in sequence

	//=======================================================================================//
	//************************************************************************************//
	// Gamma
	//************************************************************************************//
 	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x202, //RED
	0x1BC,	0x202,	0x200, 
	
0X1BF, 0X200, 0X200,	
0X1BF, 0X201, 0X200,	
0X1BF, 0X202, 0X200,	
0X1BF, 0X203, 0X22F,	
0X1BF, 0X204, 0X200,	
0X1BF, 0X205, 0X26C,	
0X1BF, 0X206, 0X200,	
0X1BF, 0X207, 0X297,	
0X1BF, 0X208, 0X200,	
0X1BF, 0X209, 0X2B9,	
0X1BF, 0X20A, 0X200,	
0X1BF, 0X20B, 0X2D4,	
0X1BF, 0X20C, 0X200,	
0X1BF, 0X20D, 0X2EC,	
0X1BF, 0X20E, 0X201,	
0X1BF, 0X20F, 0X200,	
0X1BF, 0X210, 0X201,	
0X1BF, 0X211, 0X212,	
0X1BF, 0X212, 0X201,	
0X1BF, 0X213, 0X24B,	
0X1BF, 0X214, 0X201,	
0X1BF, 0X215, 0X274,	
0X1BF, 0X216, 0X201,	
0X1BF, 0X217, 0X2B2,	
0X1BF, 0X218, 0X201,	
0X1BF, 0X219, 0X2DF,	
0X1BF, 0X21A, 0X202,	
0X1BF, 0X21B, 0X222,	
0X1BF, 0X21C, 0X202,	
0X1BF, 0X21D, 0X258,	
0X1BF, 0X21E, 0X202,	
0X1BF, 0X21F, 0X259,	
0X1BF, 0X220, 0X202,	
0X1BF, 0X221, 0X28B,	
0X1BF, 0X222, 0X202,	
0X1BF, 0X223, 0X2C4,	
0X1BF, 0X224, 0X202,	
0X1BF, 0X225, 0X2E7,	
0X1BF, 0X226, 0X203,	
0X1BF, 0X227, 0X216,	
0X1BF, 0X228, 0X203,	
0X1BF, 0X229, 0X234,	
0X1BF, 0X22A, 0X203,	
0X1BF, 0X22B, 0X25B,	
0X1BF, 0X22C, 0X203,	
0X1BF, 0X22D, 0X266,	
0X1BF, 0X22E, 0X203,	
0X1BF, 0X22F, 0X273,	
0X1BF, 0X230, 0X203,	
0X1BF, 0X231, 0X281,	
0X1BF, 0X232, 0X203,	
0X1BF, 0X233, 0X291,	
0X1BF, 0X234, 0X203,	
0X1BF, 0X235, 0X2A7,	
0X1BF, 0X236, 0X203,	
0X1BF, 0X237, 0X2C0,	
0X1BF, 0X238, 0X203,	
0X1BF, 0X239, 0X2D9,	
0X1BF, 0X23A, 0X203,	
0X1BF, 0X23B, 0X2E6,	
0X1BF, 0X23C, 0X200,	
0X1BF, 0X23D, 0X200,	
0X1BF, 0X23E, 0X200,	
0X1BF, 0X23F, 0X22F,	
0X1BF, 0X240, 0X200,	
0X1BF, 0X241, 0X26C,	
0X1BF, 0X242, 0X200,	
0X1BF, 0X243, 0X297,	
0X1BF, 0X244, 0X200,	
0X1BF, 0X245, 0X2B9,	
0X1BF, 0X246, 0X200,	
0X1BF, 0X247, 0X2D4,	
0X1BF, 0X248, 0X200,	
0X1BF, 0X249, 0X2EC,	
0X1BF, 0X24A, 0X201,	
0X1BF, 0X24B, 0X200,	
0X1BF, 0X24C, 0X201,	
0X1BF, 0X24D, 0X212,	
0X1BF, 0X24E, 0X201,	
0X1BF, 0X24F, 0X24B,	
0X1BF, 0X250, 0X201,	
0X1BF, 0X251, 0X274,	
0X1BF, 0X252, 0X201,	
0X1BF, 0X253, 0X2B2,	
0X1BF, 0X254, 0X201,	
0X1BF, 0X255, 0X2DF,	
0X1BF, 0X256, 0X202,	
0X1BF, 0X257, 0X222,	
0X1BF, 0X258, 0X202,	
0X1BF, 0X259, 0X258,	
0X1BF, 0X25A, 0X202,	
0X1BF, 0X25B, 0X259,	
0X1BF, 0X25C, 0X202,	
0X1BF, 0X25D, 0X28B,	
0X1BF, 0X25E, 0X202,	
0X1BF, 0X25F, 0X2C4,	
0X1BF, 0X260, 0X202,	
0X1BF, 0X261, 0X2E7,	
0X1BF, 0X262, 0X203,	
0X1BF, 0X263, 0X216,	
0X1BF, 0X264, 0X203,	
0X1BF, 0X265, 0X234,	
0X1BF, 0X266, 0X203,	
0X1BF, 0X267, 0X25B,	
0X1BF, 0X268, 0X203,	
0X1BF, 0X269, 0X266,	
0X1BF, 0X26A, 0X203,	
0X1BF, 0X26B, 0X273,	
0X1BF, 0X26C, 0X203,	
0X1BF, 0X26D, 0X281,	
0X1BF, 0X26E, 0X203,	
0X1BF, 0X26F, 0X291,	
0X1BF, 0X270, 0X203,	
0X1BF, 0X271, 0X2A7,	
0X1BF, 0X272, 0X203,	
0X1BF, 0X273, 0X2C0,	
0X1BF, 0X274, 0X203,	
0X1BF, 0X275, 0X2D9,	
0X1BF, 0X276, 0X203,	
0X1BF, 0X277, 0X2E6,	
0X1BF, 0X278, 0X201,
0X1BF, 0X279, 0X201,


	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x203, //GREEN
	0x1BC,	0x202,	0x200, 

0X1BF, 0X200, 0X200,	
0X1BF, 0X201, 0X200,	
0X1BF, 0X202, 0X200,	
0X1BF, 0X203, 0X22F,	
0X1BF, 0X204, 0X200,	
0X1BF, 0X205, 0X26C,	
0X1BF, 0X206, 0X200,	
0X1BF, 0X207, 0X297,	
0X1BF, 0X208, 0X200,	
0X1BF, 0X209, 0X2B9,	
0X1BF, 0X20A, 0X200,	
0X1BF, 0X20B, 0X2D4,	
0X1BF, 0X20C, 0X200,	
0X1BF, 0X20D, 0X2EC,	
0X1BF, 0X20E, 0X201,	
0X1BF, 0X20F, 0X200,	
0X1BF, 0X210, 0X201,	
0X1BF, 0X211, 0X212,	
0X1BF, 0X212, 0X201,	
0X1BF, 0X213, 0X24B,	
0X1BF, 0X214, 0X201,	
0X1BF, 0X215, 0X274,	
0X1BF, 0X216, 0X201,	
0X1BF, 0X217, 0X2B2,	
0X1BF, 0X218, 0X201,	
0X1BF, 0X219, 0X2DF,	
0X1BF, 0X21A, 0X202,	
0X1BF, 0X21B, 0X222,	
0X1BF, 0X21C, 0X202,	
0X1BF, 0X21D, 0X258,	
0X1BF, 0X21E, 0X202,	
0X1BF, 0X21F, 0X259,	
0X1BF, 0X220, 0X202,	
0X1BF, 0X221, 0X28B,	
0X1BF, 0X222, 0X202,	
0X1BF, 0X223, 0X2C4,	
0X1BF, 0X224, 0X202,	
0X1BF, 0X225, 0X2E7,	
0X1BF, 0X226, 0X203,	
0X1BF, 0X227, 0X216,	
0X1BF, 0X228, 0X203,	
0X1BF, 0X229, 0X234,	
0X1BF, 0X22A, 0X203,	
0X1BF, 0X22B, 0X25B,	
0X1BF, 0X22C, 0X203,	
0X1BF, 0X22D, 0X266,	
0X1BF, 0X22E, 0X203,	
0X1BF, 0X22F, 0X273,	
0X1BF, 0X230, 0X203,	
0X1BF, 0X231, 0X281,	
0X1BF, 0X232, 0X203,	
0X1BF, 0X233, 0X291,	
0X1BF, 0X234, 0X203,	
0X1BF, 0X235, 0X2A7,	
0X1BF, 0X236, 0X203,	
0X1BF, 0X237, 0X2C0,	
0X1BF, 0X238, 0X203,	
0X1BF, 0X239, 0X2D9,	
0X1BF, 0X23A, 0X203,	
0X1BF, 0X23B, 0X2E6,	
0X1BF, 0X23C, 0X200,	
0X1BF, 0X23D, 0X200,	
0X1BF, 0X23E, 0X200,	
0X1BF, 0X23F, 0X22F,	
0X1BF, 0X240, 0X200,	
0X1BF, 0X241, 0X26C,	
0X1BF, 0X242, 0X200,	
0X1BF, 0X243, 0X297,	
0X1BF, 0X244, 0X200,	
0X1BF, 0X245, 0X2B9,	
0X1BF, 0X246, 0X200,	
0X1BF, 0X247, 0X2D4,	
0X1BF, 0X248, 0X200,	
0X1BF, 0X249, 0X2EC,	
0X1BF, 0X24A, 0X201,	
0X1BF, 0X24B, 0X200,	
0X1BF, 0X24C, 0X201,	
0X1BF, 0X24D, 0X212,	
0X1BF, 0X24E, 0X201,	
0X1BF, 0X24F, 0X24B,	
0X1BF, 0X250, 0X201,	
0X1BF, 0X251, 0X274,	
0X1BF, 0X252, 0X201,	
0X1BF, 0X253, 0X2B2,	
0X1BF, 0X254, 0X201,	
0X1BF, 0X255, 0X2DF,	
0X1BF, 0X256, 0X202,	
0X1BF, 0X257, 0X222,	
0X1BF, 0X258, 0X202,	
0X1BF, 0X259, 0X258,	
0X1BF, 0X25A, 0X202,	
0X1BF, 0X25B, 0X259,	
0X1BF, 0X25C, 0X202,	
0X1BF, 0X25D, 0X28B,	
0X1BF, 0X25E, 0X202,	
0X1BF, 0X25F, 0X2C4,	
0X1BF, 0X260, 0X202,	
0X1BF, 0X261, 0X2E7,	
0X1BF, 0X262, 0X203,	
0X1BF, 0X263, 0X216,	
0X1BF, 0X264, 0X203,	
0X1BF, 0X265, 0X234,	
0X1BF, 0X266, 0X203,	
0X1BF, 0X267, 0X25B,	
0X1BF, 0X268, 0X203,	
0X1BF, 0X269, 0X266,	
0X1BF, 0X26A, 0X203,	
0X1BF, 0X26B, 0X273,	
0X1BF, 0X26C, 0X203,	
0X1BF, 0X26D, 0X281,	
0X1BF, 0X26E, 0X203,	
0X1BF, 0X26F, 0X291,	
0X1BF, 0X270, 0X203,	
0X1BF, 0X271, 0X2A7,	
0X1BF, 0X272, 0X203,	
0X1BF, 0X273, 0X2C0,	
0X1BF, 0X274, 0X203,	
0X1BF, 0X275, 0X2D9,	
0X1BF, 0X276, 0X203,	
0X1BF, 0X277, 0X2E6,	
0X1BF, 0X278, 0X201,
0X1BF, 0X279, 0X201,

	0x1BC, 0x204, 0x200, 
	0x1BF, 0x2FF, 0x278, 0x207, 0x204, //BLUE
	0x1BC, 0x202, 0x200, 
	
0X1BF, 0X200, 0X200,	
0X1BF, 0X201, 0X200,	
0X1BF, 0X202, 0X200,	
0X1BF, 0X203, 0X22F,	
0X1BF, 0X204, 0X200,	
0X1BF, 0X205, 0X26C,	
0X1BF, 0X206, 0X200,	
0X1BF, 0X207, 0X297,	
0X1BF, 0X208, 0X200,	
0X1BF, 0X209, 0X2B9,	
0X1BF, 0X20A, 0X200,	
0X1BF, 0X20B, 0X2D4,	
0X1BF, 0X20C, 0X200,	
0X1BF, 0X20D, 0X2EC,	
0X1BF, 0X20E, 0X201,	
0X1BF, 0X20F, 0X200,	
0X1BF, 0X210, 0X201,	
0X1BF, 0X211, 0X212,	
0X1BF, 0X212, 0X201,	
0X1BF, 0X213, 0X24B,	
0X1BF, 0X214, 0X201,	
0X1BF, 0X215, 0X274,	
0X1BF, 0X216, 0X201,	
0X1BF, 0X217, 0X2B2,	
0X1BF, 0X218, 0X201,	
0X1BF, 0X219, 0X2DF,	
0X1BF, 0X21A, 0X202,	
0X1BF, 0X21B, 0X222,	
0X1BF, 0X21C, 0X202,	
0X1BF, 0X21D, 0X258,	
0X1BF, 0X21E, 0X202,	
0X1BF, 0X21F, 0X259,	
0X1BF, 0X220, 0X202,	
0X1BF, 0X221, 0X28B,	
0X1BF, 0X222, 0X202,	
0X1BF, 0X223, 0X2C4,	
0X1BF, 0X224, 0X202,	
0X1BF, 0X225, 0X2E7,	
0X1BF, 0X226, 0X203,	
0X1BF, 0X227, 0X216,	
0X1BF, 0X228, 0X203,	
0X1BF, 0X229, 0X234,	
0X1BF, 0X22A, 0X203,	
0X1BF, 0X22B, 0X25B,	
0X1BF, 0X22C, 0X203,	
0X1BF, 0X22D, 0X266,	
0X1BF, 0X22E, 0X203,	
0X1BF, 0X22F, 0X273,	
0X1BF, 0X230, 0X203,	
0X1BF, 0X231, 0X281,	
0X1BF, 0X232, 0X203,	
0X1BF, 0X233, 0X291,	
0X1BF, 0X234, 0X203,	
0X1BF, 0X235, 0X2A7,	
0X1BF, 0X236, 0X203,	
0X1BF, 0X237, 0X2C0,	
0X1BF, 0X238, 0X203,	
0X1BF, 0X239, 0X2D9,	
0X1BF, 0X23A, 0X203,	
0X1BF, 0X23B, 0X2E6,	
0X1BF, 0X23C, 0X200,	
0X1BF, 0X23D, 0X200,	
0X1BF, 0X23E, 0X200,	
0X1BF, 0X23F, 0X22F,	
0X1BF, 0X240, 0X200,	
0X1BF, 0X241, 0X26C,	
0X1BF, 0X242, 0X200,	
0X1BF, 0X243, 0X297,	
0X1BF, 0X244, 0X200,	
0X1BF, 0X245, 0X2B9,	
0X1BF, 0X246, 0X200,	
0X1BF, 0X247, 0X2D4,	
0X1BF, 0X248, 0X200,	
0X1BF, 0X249, 0X2EC,	
0X1BF, 0X24A, 0X201,	
0X1BF, 0X24B, 0X200,	
0X1BF, 0X24C, 0X201,	
0X1BF, 0X24D, 0X212,	
0X1BF, 0X24E, 0X201,	
0X1BF, 0X24F, 0X24B,	
0X1BF, 0X250, 0X201,	
0X1BF, 0X251, 0X274,	
0X1BF, 0X252, 0X201,	
0X1BF, 0X253, 0X2B2,	
0X1BF, 0X254, 0X201,	
0X1BF, 0X255, 0X2DF,	
0X1BF, 0X256, 0X202,	
0X1BF, 0X257, 0X222,	
0X1BF, 0X258, 0X202,	
0X1BF, 0X259, 0X258,	
0X1BF, 0X25A, 0X202,	
0X1BF, 0X25B, 0X259,	
0X1BF, 0X25C, 0X202,	
0X1BF, 0X25D, 0X28B,	
0X1BF, 0X25E, 0X202,	
0X1BF, 0X25F, 0X2C4,	
0X1BF, 0X260, 0X202,	
0X1BF, 0X261, 0X2E7,	
0X1BF, 0X262, 0X203,	
0X1BF, 0X263, 0X216,	
0X1BF, 0X264, 0X203,	
0X1BF, 0X265, 0X234,	
0X1BF, 0X266, 0X203,	
0X1BF, 0X267, 0X25B,	
0X1BF, 0X268, 0X203,	
0X1BF, 0X269, 0X266,	
0X1BF, 0X26A, 0X203,	
0X1BF, 0X26B, 0X273,	
0X1BF, 0X26C, 0X203,	
0X1BF, 0X26D, 0X281,	
0X1BF, 0X26E, 0X203,	
0X1BF, 0X26F, 0X291,	
0X1BF, 0X270, 0X203,	
0X1BF, 0X271, 0X2A7,	
0X1BF, 0X272, 0X203,	
0X1BF, 0X273, 0X2C0,	
0X1BF, 0X274, 0X203,	
0X1BF, 0X275, 0X2D9,	
0X1BF, 0X276, 0X203,	
0X1BF, 0X277, 0X2E6,	
0X1BF, 0X278, 0X201,
0X1BF, 0X279, 0X201,
	
	//============Gamma END=============

	//=======================================================================================//
	//************************************************************************************//
	// CABC duty test (test mode, normal don't use)
	//************************************************************************************//
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,  0x205,     // Change to Page 5
	0x1BC,	0x202,	0x200, 

	//PWM frequence default value is 300Hz, Default PWM precision is 12 bits!
//	0x1BF,	0x203,	0x220, // PWM precision is 12 bits
//	0x1BF,	0x204,	0x219, // PWM frequency=32MHz/((25+1)*4096)=300.48Hz

	//PWM frequence set 2.98k
//	0x1BF,	0x203,	0x260, // PWM precision is 8 bits
//	0x1BF,	0x204,	0x229, // PWM frequency=32MHz/((41+1)*256)=2.98k
	
	//PWM frequence set 10.416667k
//	0x1BF,	0x203,	0x260, // PWM precision is 8 bits
//	0x1BF,	0x204,	0x20B, // PWM frequency=32MHz/((11+1)*256)=10.416667k
	
	//PWM frequence set 31.25k
//	0x1BF,	0x203,	0x260, // PWM precision is 8 bits
//	0x1BF,	0x204,	0x203, // PWM frequency=32MHz/((3+1)*256)=31.25k

	//PWM frequence set 31.25k
	0x1BF,	0x203,	0x240, // PWM precision is 10 bits
	0x1BF,	0x204,	0x200, // PWM frequency=32MHz/((0+1)*1024)=31.25k

	//=======================================================================================//
	//************************************************************************************//
	// Others
	//************************************************************************************//	
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x200, // Change to Page 0

	0x1BC,	0x202,	0x200,
	0x1BF,	0x235,	0x200,
	
	0XF00	
};

unsigned short ET2_InitCode[]= 
{
	0X1B7,	0X210,  0X203,
	
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x200,     // Change to Page 0
	
	0x1BC,	0x202,	0x200,
	0x1BF,	0x235,	0x200,
	
	0XF00	
};

unsigned short RA_InitCode[]=
{
	0X1B7,	0X210,  0X203,
	0x1BC,	0x204,  0x200, 
	0x1BF,	0x2FF,  0x278,	0x207,	0x201,	// Change to Page 1
	
	0x1BC,	0x202,  0x200, 
	0x1BF,	0x28D,	0x200, //DOT_BASED FLICKER_GRAY
	0x1BF,	0x28F,	0x201, //BIST_EN, 03=8x8 chessboard
	0x1BF,	0x290,	0x2B6, //D7=FREE RUN enable   D6:4=INTERVAL TIME:120FRAMES=2S   D3:0=BISTPNUM
	0x1BF,	0x291,	0x202, //R
	0x1BF,	0x292,	0x203, //G
	0x1BF,	0x293,	0x204, //B
	0x1BF,	0x294,	0x201, //BLACK
	0x1BF,	0x295,	0x200, //W
	0x1BF,	0x296,	0x205, //G127 or 8x8 chessboard
	0x1BF,	0x297,	0x206, //G128
	0x1BF,	0x298,	0x207, //V-COLORBAR
	0x1BF,	0x299,	0x208, //V-GRAYBAR
	0x1BF,	0x29A,	0x209, //H-GRAYBAR
	0x1BF,	0x29B,	0x20A, //CROSSTALK
	0x1BF,	0x29C,	0x20B, //PICECK CHECK
	0x1BF,	0x29D,	0x20C, //DOT CHECK
	0x1BF,	0x29E,	0x20A, //SINGLE MODE PATTERN
	
	//If the RA jig is RA-1to9, please uncomment the following code
//	0x1BC,	0x204,	0x200, 
//	0x1BF,	0x2FF,	0x278,	0x207,  0x205,     // Change to Page 5
//	0x1BC,	0x202,	0x200, 
//	0x1BF,	0x203,	0x244, //PWM_POL always high
	
	0x1BC,	0x204,	0x200, 
	0x1BF,	0x2FF,	0x278,	0x207,	0x200,     // Change to Page 0
	
	0x1BC,	0x202,	0x200,
	0x1BF,	0x235,	0x200,

	0XF00	
};
