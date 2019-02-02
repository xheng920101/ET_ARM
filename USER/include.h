#ifndef __INCLUDE_H
#define __INCLUDE_H

/*********************************************************************************
 * optional test functions
 */ 
//#define CMD_MODE
//#define AUTO_LINE							//auto line enable, if not use on auto line, please comment the define				
#define SINGLE_PORT						//single port enable, if project use dual port, please comment the define
//#define GAMMAEXPERT						//use GammaExpert to do auto 3gamma, if you don't need it, please comment the define
//#define DIFFER2_DETECT				//CTP SENSOR detect, if you don't need it, please comment the define
//#define PWM_DETECT						
//#define TE_DETECT
//#define CHROMA								// HTC SONY 10.1 chroma control, if you don't need it, please comment the define
//#define	HOT_PLUG							//FPC connection check, if you don't need it, please comment the define
//#define	RES_TEST							//Silver resistance test, if you don't need it, please comment the define
//#define	CURRENT_CHECK					//current check, if you don't need it, please comment the define
#define	CURRENT_METER					//current measure, if you don't need it, please comment the define

/*********************************************************************************
 * adjustable parameters
 */
#define OTP_FLAG 								0x80		//OTP test item select: b7 = flicker, b6 = contrast, b5 = chroma, b4 = gamma, others = reserved
#define ET_DLY_LOCK							1				//ET delay lock enable: 1 = enable, 0 = disable
#define PC_COM 									USART1	//specify com for the communication between ARM and PC
#define MSE_COM 								UART4		//specify com for the communication between ARM and MSE
#define CODE_MAX_LEN						512			//usart receive buffer length
#define ADMESY_FLICKER_SAMPLES	2048		//samples for calculate flicker in DB (measure by MSE)
#define ADMESY_RDATA_LEN				(2048 * 4 + 32)	//usart receive buffer length, depending on ADMESY_FLICKER_SAMPLES
#define KEEPNUM 								5				//best vcom keep number during flicker scan
#define PWM_T										100			//ARM output PWM singal duration, unit is 10us(100KHz)
#define AUTO_TIME								20			//display pattern auto switch time: (AUTO_TIME*0.1)s
#define TAUTORESET							3600
//for RA outo reset period set ,second
#define OTP_TIMES_MAX						3				//maximum number of OTP times
#define PIC_NUM									10		//picture loading number, 0 indicate not to load picture
#define SAM_CRST_OFFSET		      143 		//samsung crosstalk offset, uint is pixel, 9mm = ? pixel
#define DOT_R1									127			//MEIZU bright dot or HW color crosstalk background color
#define DOT_G1									127
#define DOT_B1									127
#define DOT_R2									255			//MEIZU bright dot or HW cross-crosstalk corner color
#define DOT_G2									255
#define DOT_B2									0

/*********************************************************************************
 * product informations
 */
#define	TEST_MODE_DEBUG		0
#define	TEST_MODE_ET1			1
#define	TEST_MODE_ET2			2
#define	TEST_MODE_ET3			3
#define	TEST_MODE_OTP			4
#define	TEST_MODE_RA			5	//BIST AUTO RUN
#define	TEST_MODE_ESD			6 //EXTERNAL AUTO RUN
#define	TEST_MODE_OD			7			   
#define	TEST_MODE_DEMO		8		
#define	TEST_MODE_CTP			9		
#define	TEST_MODE_OQC1		10		

#define	VERSION_DEBUG	"DEBUG"	

//#define	VERSION_ET1		"ET1_V2P0"
#define	VERSION_ET1		"OQC_V3P0"

#define	VERSION_ET2		"ET2_V1P0"		
//#define	VERSION_ET2		"OQC2_V01"	//OQC2 = ET2 with the same delay
//#define	VERSION_ET2		"FP_V01"		//FP = ET2 without delay
#define	VERSION_ET3		"ET3_V1P0"
#define	VERSION_OTP		"OTP_V1P1"
#define	VERSION_RA		"RA_V1P0"
#define	VERSION_ESD		"ESD_V1P0"
#define	VERSION_OD		"OD_V1P0"			   
#define	VERSION_DEMO	"DEMO_V1P0"
#define	VERSION_CTP		"CTP_V1P0"	
#define	VERSION_OQC1	"OQC1_V1P0"	

//CURRENT_METER
// #define SPEC_NORMAL_IOVCC_MAX	40.0					//mA
// #define SPEC_NORMAL_VSP_MAX 	20.0						//mA
// #define SPEC_NORMAL_VSN_MAX		20.0					//mA
#define SPEC_NORMAL_IOVCC_MIN	1.0								//mA
#define SPEC_NORMAL_VSP_MIN 	1.0								//mA
#define SPEC_NORMAL_VSN_MIN		1.0								//mA
#define SPEC_SLEEP_IOVCC			3.6//5							    //uA
#define SPEC_SLEEP_VSP 				0.05//0.2							    //uA
#define SPEC_SLEEP_VSN 				0.03//0.1							    //uA

//OTP, GAMMAEXPERT
#define	PROJECT_NO						"TL057BFXS05-00"	
#define SPEC_CONTRAST 				1000.00
#define SPEC_Lv 							400.00
#define SPEC_x 								0.290
#define SPEC_y								0.310
#define SPEC_xy_RANGE 				0.03							//Default TM spec = 0.03
#define SPEC_GAMMA						2.2
#define SPEC_GAMMA_RANGE 			0.20
#define SPEC_FLICKER_DB 			-32.0
#define SPEC_FLICKER_PERCENT 	10
#define Info_IC_Vendor				"Synaptics"						//IC vendor
#define Info_IC_Type					"TD4302"					//IC part number Novatek:NT35596/Orise:OTM1901A/RSP:R63319/Himax:HX8399-C
#define Info_Target_X2				0.300							//chroma x2 threshold
#define Info_Target_Y2				0.325							//chroma y2 threshold
#define Info_Target_X3				0.395							//chroma x3 threshold
#define Info_Target_Y3				0.320							//chroma y3 threshold
#define Info_GVDDP						4.1 							//Gamma positive
#define Info_GVDDN						4.1 							//Gamma negitive
#define Info_XY_CCT						0									//ajust mode select: 0 = chroma; 1 = CCT
#define Info_Min_CCT					8126							//CCT min
#define Info_Target_CCT				8136							//CCT target
#define Info_Max_CCT					8146							//CCT max
#define Info_Min_VCOM					"05"							//VCOM down limitation
#define Info_Max_VCOM					"1E"							//VCOM up limitation
#define Info_Typical_VCOM			"0E"							//VCOM typical
#define Info_Adjust_Gamma			0									//gamma adjust type£º1 = 1gamma;  2 = 1gamma+3gamma; 0 = don't adjust
#define Info_Adjust_Fliker		1									//Fliker adjust enable
#define Info_Delta_XY_min			0.01							//Wx,Wy differ min
#define Info_Delta_XY_max			0.03							//Wx,Wy differ max
#define Info_Delta_X					0.005							//Wx precision
#define Info_Delta_Y					0.005							//Wy precision
#define Info_Crosstalk				0.003							//crostalk
#define Info_TarRx            0.622
#define Info_TarRy            0.339
#define Info_RangeR           0.05
#define Info_TarGx            0.313
#define Info_TarGy            0.609
#define Info_RangeG           0.05
#define Info_TarBx            0.149
#define Info_TarBy            0.054
#define Info_RangeB           0.05
#define Info_Check_gamma			"80 90 100 110 120 130 140 150 160 170 180 190 200 210"	//Check one gamma node
#define Info_Fliker_Code			"0x1B7 0x259 0x202; 0x1BC 0x202 0x200; 0x1BF 0x211 VCOM"										//VCOM change code
#define Info_Gamma_Register		"0X200 0X2C7"												//one gamma initial setting          

/*********************************************************************************
 * include file
 */
#include "misc.h"
#include "board_conf.h"
#include "sysTick.h"
#include "timer.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "admesy.h"
#include "adc.h"
#include "i2c.h"
#include "power.h"
#include "digital_sensor.h"
#include "eeprom.h"
#include "sdcard.h"		
#include "ff9/ff.h"			/* Declarations of FatFs API */
#include "ff9/diskio.h"		/* Declarations of disk I/O functions */
#include "bsp_bmp.h"
#include "ssd2828.h"
#include "fpga.h"
#include "lcd.h"
#include "lcd_test.h"
#include "test_debug.h"
#include "test_et1.h"
#include "test_et2.h"
#include "test_et3.h"
#include "test_otp.h"
#include "test_ra.h"
#include "test_esd.h"
#include "test_od.h"
#include "test_demo.h"
#include "test_ctp.h"
#include "test_oqc1.h"

/*********************************************************************************
 * global defines
 */
extern uint8_t TEST_MODE;
extern FPGAPORCH_TypeDef FPGA_porchPara;
extern uint16_t SSDInitCode[];
extern uint16_t ET1_InitCode[];
extern uint16_t ET2_InitCode[];
extern uint16_t RA_InitCode[];
extern uint16_t GAMMA_InitCode[];

extern uint8_t OTP_TIMES;
extern uint16_t vcom_best;
extern float flicker_best;
extern float white;
extern float black;
extern float chroma_x;
extern float chroma_y;
extern float chroma_Lv;
extern uint8_t MAIN_PORT;

void VCOM_Set(unsigned short vcom);
unsigned short VCOM_Read(void);
unsigned char OTPTimes_Read(void);
void ScanBackward(void);
void ScanForward(void);
ErrorStatus OTPSequence(void);
void MES(void);
void LCM_Init(void);
uint16_t ReadSensor(void);
double Voltage(void);
void Sys_DET_Resistor(void);
void Con_Check(void);



#ifdef CHROMA
extern uint16_t gamma[10][64];
void Chroma_Set(unsigned char u, unsigned char v);
void Date_Set(unsigned char year, unsigned char month, unsigned char day);
#endif

#endif /* _INCLUDE_H */
