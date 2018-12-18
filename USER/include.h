#ifndef __INCLUDE_H
#define __INCLUDE_H

/*********************************************************************************
 * optional test functions
 */
#define	NO_FLASH_MODE 
//#define	SDCARD_MODE
//#define CMD_MODE
#define SINGLE_PORT						  //single port enable, if project use dual port, please comment the define
//#define DIFFER2_DETECT				//only for FT8716, FOCAL CTP test
//#define PWM_DETECT						
//#define TE_DETECT
//#define CHROMA								// HTC SONY 10.1 chroma control, if you don't need it, please comment the define
//#define	HOT_PLUG							//FPC connection check, if you don't need it, please comment the define
//#define	CURRENT_CHECK					//current check, if you don't need it, please comment the define
//#define	CURRENT_METER					//current measure, if you don't need it, please comment the define
//#define	AG_TEST							  //FPC connection check, if you don't need it, please comment the define

/*********************************************************************************
 * adjustable parameters
 */																	    //OTP_FLAG_xxx: test item select: b7 = flicker, b6 = contrast, b5 = chroma, b4 = gamma, others = reserved
#define OTP_FLAG_AUTO						0xF0		//config for auto line otp instead of OTP_FLAG
#define OTP_FLAG_MAN						0xF0		//config for manual otp instead of OTP_FLAG
#define ET_DLY_LOCK							1		    //ET delay lock enable: 1 = enable, 0 = disable
#define CODE_MAX_LEN						512			//usart receive buffer length
#define ADMESY_FLICKER_SAMPLES	2048		//samples for calculate flicker in DB (measure by MSE)
#define ADMESY_RDATA_LEN				(2048 * 4 + 32)	//usart receive buffer length, depending on ADMESY_FLICKER_SAMPLES
#define KEEPNUM 								5				//best vcom keep number during flicker scan
#define PWM_T										100			//ARM output PWM singal duration, unit is 10us(100KHz)
#define AUTOSWITCH_T						20			//display pattern auto switch time: (AUTO_TIME*0.1)s
#define AUTORESET_T							1800		//for RA auto reset period set, unit is 1 second
#define OTP_TIMES_MAX						2				//maximum number of OTP times
#define PIC_NUM									0 	  //picture loading number, 0 indicate not to load picture
#define SD_MODE_PIC             2   
#define INFO_Y_AXIS							100   		//informatin display start Y point, uint is line

#define SAM_CRST_OFFSET		      143 		//samsung crosstalk offset, uint is pixel, 9mm = ? pixel
#define DOT_R1									100 //			//bright dot up-line
#define DOT_G1									70  //
#define DOT_B1									170 //
#define DOT_R2									0   //			//bright dot middle-line
#define DOT_G2									0   //
#define DOT_B2									0   //
#define DOT_R3									0		//      //bright dot down-line
#define DOT_G3									0
#define DOT_B3									0
#define HW_CRST_BG_R						127			//HW color crosstalk background color
#define HW_CRST_BG_G						127
#define HW_CRST_BG_B						127
#define HW_CROSS_BG_R						255			//HW color cross background color
#define HW_CROSS_BG_G						255
#define HW_CROSS_BG_B						0
#define RECT_START_X						0			//for hole align
#define RECT_START_Y						0
#define RECT_SIZE_X							0
#define RECT_SIZE_Y							0

/*********************************************************************************
 * product informations
 */
#define	TEST_MODE_DEBUG		0
#define	TEST_MODE_ET1			1
#define	TEST_MODE_ET2			2
#define	TEST_MODE_ET3			3
#define	TEST_MODE_OTP			4
#define	TEST_MODE_RA			5	//BIST AUTO RUN
#define	TEST_MODE_ESD			6 //EXTERN`AL AUTO RUN
#define	TEST_MODE_OD			7			   
#define	TEST_MODE_DEMO		8		
#define	TEST_MODE_CTP			9		
#define	TEST_MODE_OQC1		10	

#define	VERSION_DEBUG	"DEBUG_V1P0"	
#define	VERSION_ET1		"ET1_V1P0"
#define	VERSION_ET2		"ET2_V1P0"	  
//#define	VERSION_ET2		"OQC2_V01"	//OQC2 = ET2 with the same delay
//#define	VERSION_ET2		"FP_V01"		//FP = ET2 without delay
#define	VERSION_ET3		"ET3_V1P0"
#define	VERSION_OTP		"OTP_V1P0"
#define	VERSION_RA		"RA_V1P0"
#define	VERSION_ESD		"ESDDJ_V1P0"
#define	VERSION_OD		"OD_V2P0"			   
#define	VERSION_DEMO	"DEMO_V1P0"
#define	VERSION_CTP		"CTP_V1P0"	  
#define	VERSION_OQC1	"OQC1_V1P0"	
#define	VERSION_SDmode	"V1P0"

//Begin=====================SPEC should be edited by each projcet=======================
#define SPEC_MIN_IOVCC	2.0								//mA
#define SPEC_MIN_VSP 		2.0								//mA
#define SPEC_MIN_VSN		2.0								//mA

#define SPEC_MAX_RED_IOVCC		36.0			//mA
#define SPEC_MAX_RED_VSP 			9.0				//mA
#define SPEC_MAX_RED_VSN			9.0				//mA

#define SPEC_MAX_FLICKER_IOVCC		200.0				//mA if not check ,use lager data such as 200
#define SPEC_MAX_FLICKER_VSP 			200.0				//mA
#define SPEC_MAX_FLICKER_VSN			200.0				//mA

#define SPEC_MAX_CHECK_DOT_IOVCC		200.0				//mA if not check ,use lager data such as 200
#define SPEC_MAX_CHECK_DOT_VSP 			200.0				//mA
#define SPEC_MAX_CHECK_DOT_VSN			200.0				//mA

#define SPEC_MAX_CHECK_PIXEL_IOVCC		200.0			//mA if not check ,use lager data such as 200
#define SPEC_MAX_CHECK_PIXEL_VSP 			200.0			//mA
#define SPEC_MAX_CHECK_PIXEL_VSN			200.0			//mA

#define SPEC_SLEEP_IOVCC			2800.0						//uA
#define SPEC_SLEEP_VSP 				180.0					  //uA
#define SPEC_SLEEP_VSN 				50.0					    //uA

#define SPEC_MIN_LEDA_NORMAL	38 				        //mA 38
#define SPEC_MAX_LEDA_NORMAL	42								//mA

#define SPEC_MIN_LEDA_DIM			1.6				          //mA 1.6
#define SPEC_MAX_LEDA_DIM			2.4								//mA 2.4



//Project ID
//DP082---JHSN771100-----TL065FVXF01-00/TL065FVXF06-00/TL065FVXF07-00/TL065FVXF08-00
//DP102---JACK771100-----TL065FVXF02-00/TL065FVXF03-00/TL065FVXF04-00/TL065FVXF05-00
//#define ID_DP082
#define ID_DP102 

#ifdef ID_DP082 
	#define ID_PJ1	0x4A //J--ASCII=4A
	#define ID_PJ2	0x48 //H--ASCII=48
	#define ID_PJ3	0x53 //S--ASCII=53
	#define ID_PJ4	0x4E //N--ASCII=4E
	#define ID_PJ5	0x77 //77
	#define ID_PJ6	0x11 //11
	#define ID_PJ7	0x00 //00
#endif

#ifdef ID_DP102 
	#define ID_PJ1	0x4A //J--ASCII=4A
	#define ID_PJ2	0x41 //A--ASCII=41
	#define ID_PJ3	0x43 //C--ASCII=43
	#define ID_PJ4	0x4B //K--ASCII=4B
	#define ID_PJ5	0x77 //77
	#define ID_PJ6	0x11 //11
	#define ID_PJ7	0x00 //00
#endif



//color ID 
//White---0xE1;  Black--0xD2;  PINK--0xC3;  Red---0xB4; 
//YELLOW--0xA5;  Blue---0x96;  GOLD--0x87;  Gray--0x78;  

#define ID_BLACK  //TL065FVXF01-00 ;  TL065FVXF02-00 ;
//#define ID_WHITE  //TL065FVXF**-00  
//#define ID_RED    //
//#define ID_BLUE   //
//#define ID_GRAY   //

//color ID
#ifdef ID_BLACK
	#define ID_COLOR		0xD2
#endif
#ifdef ID_WHITE
	#define ID_COLOR		0xE1
#endif
#ifdef ID_RED
	#define ID_COLOR		0xB4 
#endif
#ifdef ID_BLUE
	#define ID_COLOR		0x96 
#endif
#ifdef ID_GRAY
	#define ID_COLOR		0x78
#endif


//CG Color & PROJECT_NO
#ifdef ID_BLACK
	#define	PROJECT_NO	"TL065FVXF01-00_G6" //TL065FVXF01-00_G6 ; TL065FVXF02-00_G6
#endif
#ifdef ID_WHITE
	#define	PROJECT_NO	"TL065FVXF01-00_G6" //
#endif
#ifdef ID_RED
	#define	PROJECT_NO	"TL065FVXF01-00_G6" //
#endif
#ifdef ID_BLUE
	#define	PROJECT_NO	"TL065FVXF01-00_G6" //
#endif
#ifdef ID_GRAY
	#define	PROJECT_NO	"TL065FVXF01-00_G6" 
#endif


#define	FW_NO						"TL065FVXF01-00" //FW--TL065FVXF01-00 ; 固定不需要修改

//**********************************OTP, GAMMAEXPERT SETTING BEGIN*******************************************************//
//=======================BASIC SETTING(NECESSARY)=================================//
//#define	PROJECT_NO						"TL065FVXF01-00_G6" //TL063FVMC02-01_G6
#define Info_IC_Vendor				"NOVATEK"				//IC vendor,replace in HIMAX/RSP/NOVATEK/ORISE/ILITEC
#define Info_IC_Type					"NT36672A"				//IC part number:Select IC type
                                              //NOVATEK:NT35596,NT36672
                                              //ORISE:OTM1901A,OTM1911
																							//RSP:R63319,TD4310,TD4300,TD4322
																							//HIMAX:HX8399-C
																							//ILITEC:7807D
#define Info_Adjust_Gamma			0								//gamma adjust type:1 = 1gamma;  2 = 1gamma+3gamma; 0 = don't adjust(just adjust Vcom)
#define Info_Vlimit						0.0						    //Gamma voltage limit
//=======================Vcom Scan SETTING(NECESSARY)=============================//
#define Info_Min_VCOM					"0108"							//VCOM down limitation
#define Info_Max_VCOM					"0114"							//VCOM up limitation
#define Info_Typical_VCOM			"010E"							//VCOM typical
//=========================SPEC SETTING(NECESSARY)================================//
#define SPEC_Lv 							380.00
#define SPEC_Lv_MAX							600.00
#define SPEC_x 								0.295
#define SPEC_y								0.315
#define SPEC_xy_RANGE 				0.05						//Default TM spec = 0.03
#define SPEC_GAMMA						2.2
#define SPEC_GAMMA_RANGE 			0.30
#define SPEC_FLICKER_DB 			-32.0
#define SPEC_FLICKER_PERCENT 	10
//=========================SPEC SETTING(OPTIONAL)================================//set 0.0 when no used
#define Info_SHX1							0.0
#define Info_SHY1							0.0
#define Info_SHX2							0.0
#define Info_SHY2							0.0
#define Info_SHX3							0.0
#define Info_SHY3							0.0
#define Info_SHX4							0.0
#define Info_SHY4							0.0
/* 
     (THX1,THY1)___________________(THX4,THY4)
               /                  /
							/                  /
						 /                  /
            /__________________/
     (THX2,THY2)               (THX3,THY3)

*/

#define Info_GVDDP						5.0							//Gamma positive
#define Info_GVDDN						5.0							//Gamma negitive
#define Info_Check_gamma			"64 90 127 128 160 180 200 220"	//Check one gamma node
#define SPEC_CONTRAST 				500.00
#define Info_Target_X2				0.300							//chroma x2 threshold
#define Info_Target_Y2				0.325							//chroma y2 threshold
#define Info_Target_X3				0.395							//chroma x3 threshold
#define Info_Target_Y3				0.320							//chroma y3 threshold
#define Info_XY_CCT						0									//ajust mode select: 0 = chroma; 1 = CCT
#define Info_Min_CCT					8126							//CCT min
#define Info_Target_CCT				8136							//CCT target
#define Info_Max_CCT					8146							//CCT max
#define Info_Adjust_Fliker		1									//Fliker adjust enable
#define Info_Delta_XY_min			0.00							//Wx,Wy differ min
#define Info_Delta_XY_max			0.5							//Wx,Wy differ max
#define Info_Delta_X					0.5							//Wx precision
#define Info_Delta_Y					0.5							//Wy precision
#define Info_Crosstalk				0.8						//crostalk
#define Info_TarRx            0.622
#define Info_TarRy            0.339
#define Info_RangeR           0.5
#define Info_TarGx            0.313
#define Info_TarGy            0.609
#define Info_RangeG           0.5
#define Info_TarBx            0.149
#define Info_TarBy            0.054
#define Info_RangeB           0.5
#define ID_OTP_FLAG           1
#define GammaPro_VERSION      "GammaPro_V1P0"
#define Info_Fliker_Code			"0x1B7 0x259 0x202; 0x1BC 0x202 0x200; 0x1BF 0x2FF 0x220;0x1BF 0x2FB 0x201;0x1BF 0x289 VCOM"//VCOM change code
#define Info_Gamma_Register		""												//one gamma initial setting 
//**********************************OTP, GAMMAEXPERT SETTING FINISH*******************************************************//

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
#include "ff9/ff.h"			  /* Declarations of FatFs API */
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
#include "test_SDcard.h"
#include "TaskPolling.h"


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
extern float chroma_x_before;
extern float chroma_y_before;
extern float chroma_Lv_before;
extern uint8_t MAIN_PORT;
extern uint8_t ID1;
extern uint8_t ID2;
extern uint8_t ID3;
extern uint8_t DATE_YY;
extern uint8_t DATE_MM;
extern uint8_t DATE_DD;
extern uint8_t DATE_HR;
extern uint8_t DATE_MIM;
extern uint8_t DATE_SEC;

void SendPage(unsigned char pageNum); //add wwp 20180428
	
void VCOM_Set(unsigned short vcom);
unsigned short VCOM_Read(void);
unsigned char OTPTimes_Read(void);
void ScanBackward(void);
void ScanForward(void);
ErrorStatus OTPSequence(void);
void IDCheck(void);
////
//void DeepStandby(void);
//void ExitStandby(void);
////
#ifdef CHROMA
extern uint16_t gamma[10][64];
void Chroma_Set(unsigned char u, unsigned char v);
void Date_Set(unsigned char year, unsigned char month, unsigned char day);
#endif

#ifdef NO_FLASH_MODE
extern char FWVersion[];
ErrorStatus Program_FW(void);
ErrorStatus RA_Program_FW(void);
void TCH_SPI_Config(void);
void TCH_SPI_UNConfig(void);

/*********************************************************************************
 * Interface definition for all IC, not to change!
 */
#ifndef	TCH_SPI_CSN1_PIN

	#define	TCH_SPI_CSN1_PIN					GPIO_Pin_9		//TEST22: TP_SDA --> SPI_CSN
	#define	TCH_SPI_CSN1_GPIO_PORT		GPIOB

	#define	TCH_SPI_SCK_PIN						GPIO_Pin_8		//TEST23: TP_SCL --> SPI_SCK
	#define	TCH_SPI_SCK_GPIO_PORT			GPIOB

	#define	TCH_SPI_MISO_PIN					GPIO_Pin_6		//TEST19: POWER_I2C_SCL --> SPI_MISO
	#define	TCH_SPI_MISO_GPIO_PORT		GPIOB

	#define	TCH_SPI_MOSI_PIN					GPIO_Pin_5		// TEST18: POWER_I2C_SDA --> SPI_MOSI
	#define	TCH_SPI_MOSI_GPIO_PORT		GPIOB

	#define	TCH_SPI_CSN2_PIN					GPIO_Pin_7		//TEST20: TP_INT --> SPI_CSN
	#define	TCH_SPI_CSN2_GPIO_PORT		GPIOB

	#define	TCH_SPI_CSN3_PIN					GPIO_Pin_2		//TEST21: TP_SCL --> SPI_CSN
	#define	TCH_SPI_CSN3_GPIO_PORT		GPIOC

	#define	TCH_RST_PIN								GPIO_Pin_3		//TEST24: TP_RST --> TP_RST
	#define	TCH_RST_GPIO_PORT					GPIOC
	
#endif
	
	//YWB
	#define	TCH_SPI_CSN1_2_PIN				GPIO_Pin_4	
	#define	TCH_SPI_CSN1_2_GPIO_PORT	GPIOE

	#define	TCH_SPI_CSN2_2_PIN				GPIO_Pin_5		
	#define	TCH_SPI_CSN2_2_GPIO_PORT	GPIOE

	#define	TCH_SPI_CSN3_2_PIN				GPIO_Pin_6		
	#define	TCH_SPI_CSN3_2_GPIO_PORT	GPIOE

	#define	TCH_SPI_CSN1_3_PIN				GPIO_Pin_6	
	#define	TCH_SPI_CSN1_3_GPIO_PORT	GPIOF

	#define	TCH_SPI_CSN2_3_PIN				GPIO_Pin_7		
	#define	TCH_SPI_CSN2_3_GPIO_PORT	GPIOF

	#define	TCH_SPI_CSN3_3_PIN				GPIO_Pin_8		
	#define	TCH_SPI_CSN3_3_GPIO_PORT	GPIOF
	

/* 
 * End of interface definition
*********************************************************************************/

#endif

#endif /* _INCLUDE_H */
