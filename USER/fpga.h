#ifndef __FPGA_H
#define __FPGA_H

#include "board_conf.h"

/*********************************************************************************
 * command list
 */                                    
#define		INFO_NONE			0x00
#define		INFO_STR			0x01	//first line diaplay comment: string set by FPGA_Info_Set(string)
#define		INFO_PTNNUM			0x02	//third line display pattern number: PXXX(XXX = 000~255)
#define		INFO_RGBVAL			0x04	//third line display gray value:(R;G;B)
#define		INFO_OTPTIMES		0x08	//third line display OTP TIMES: XX
#define		INFO_VERSION		0x11	//first line display program version: ET1_VXX FPGA_VXX
#define		INFO_ARM_VERSION	0x20 //first line display ARM template program version: ARM_VXX
#define		INFO_PROJECT_NO		0x40 //second line display project number: TL055VDXP40-00

//added by luosijian 20140725
#define		ADDR_HACT_1   		0x10
#define		ADDR_HACT_2  		0x11
#define		ADDR_VACT_1  		0x12
#define		ADDR_VACT_2  		0x13
#define		ADDR_VPW     		0x14
#define		ADDR_VBP_1   		0x15
#define		ADDR_VBP_2   		0x16
#define		ADDR_VFP_1   		0x17
#define		ADDR_VFP_2   		0x18
#define		ADDR_HPW     		0x19
#define		ADDR_HBP_1   		0x1A
#define		ADDR_HBP_2   		0x1B
#define		ADDR_HFP_1   		0x1C
#define		ADDR_HFP_2   		0x1D

#define 	ADDR_PIC_WR_EN      0x20
#define 	ADDR_PIC_WR_NUM     0x21
#define 	ADDR_PIC_BST_NUM_1  0x22
#define 	ADDR_PIC_BST_NUM_2  0x23
#define 	ADDR_PIC_SIZE_RSV   0x24
#define 	ADDR_PIC_LAST_BST_NUM_1   0x25
#define 	ADDR_PIC_LAST_BST_NUM_2   0x26

#define 	ADDR_DIS_MODE       0x30
#define 	ADDR_DIS_SN         0x31
#define 	ADDR_PORT_MAP       0x32
#define 	ADDR_PORT_MAIN      0x33
#define 	ADDR_OTP_TIMES_1   	0x34
#define 	ADDR_OTP_TIMES_2   	0x35
#define 	ADDR_PAT_GRAY_1    	0x36
#define 	ADDR_PAT_GRAY_2    	0x37
#define 	ADDR_PAT_GRAY_3    	0x38

#define 	ADDR_INIT_END       0x40
#define 	ADDR_TE_DETECT      0x41
#define 	ADDR_PIC_MASK       0x42

#define 	ADDR_PAT_R       	0x50
#define 	ADDR_PAT_G       	0x51
#define 	ADDR_PAT_B       	0x52
#define 	ADDR_PAT_GRAY      	0x53
#define 	ADDR_PAT_R1       	0x54
#define 	ADDR_PAT_G1       	0x55
#define 	ADDR_PAT_B1       	0x56
#define 	ADDR_PAT_R2       	0x57
#define 	ADDR_PAT_G2       	0x58
#define 	ADDR_PAT_B2       	0x59
#define 	ADDR_PAT_R3       	0x5A
#define 	ADDR_PAT_G3       	0x5B
#define 	ADDR_PAT_B3       	0x5C
#define 	ADDR_PAT_BG_R       0x5D
#define 	ADDR_PAT_BG_G       0x5E
#define 	ADDR_PAT_BG_B       0x5F

#define 	ADDR_OTP_TIMES     	0x60
#define 	ADDR_INFO_SHOW_EN  	0x61	
#define 	ADDR_INFO_CHAR_0 	0x62
#define 	ADDR_INFO_CHAR_1 	0x63
#define 	ADDR_INFO_CHAR_2 	0x64
#define 	ADDR_INFO_CHAR_3 	0x65
#define 	ADDR_INFO_CHAR_4 	0x66
#define 	ADDR_INFO_CHAR_5 	0x67
#define 	ADDR_INFO_CHAR_6 	0x68
#define 	ADDR_INFO_CHAR_7 	0x69
#define 	ADDR_INFO_CHAR_8 	0x6A
#define 	ADDR_INFO_CHAR_9 	0x6B
#define 	ADDR_INFO_CHAR_10 	0x6C
#define 	ADDR_INFO_CHAR_11 	0x6D
#define 	ADDR_INFO_CHAR_12 	0x6E
#define 	ADDR_INFO_CHAR_13 	0x6F

//PROJECT_NO	"TL055VDXP40-00"
#define 	ADDR_PROJECT_CHAR_0 	0x70
#define 	ADDR_PROJECT_CHAR_1 	0x71
#define 	ADDR_PROJECT_CHAR_2 	0x72
#define 	ADDR_PROJECT_CHAR_3 	0x73
#define 	ADDR_PROJECT_CHAR_4 	0x74
#define 	ADDR_PROJECT_CHAR_5 	0x75
#define 	ADDR_PROJECT_CHAR_6 	0x76
#define 	ADDR_PROJECT_CHAR_7 	0x77
#define 	ADDR_PROJECT_CHAR_8 	0x78
#define 	ADDR_PROJECT_CHAR_9 	0x79
#define 	ADDR_PROJECT_CHAR_10 	0x7A
#define 	ADDR_PROJECT_CHAR_11 	0x7B
#define 	ADDR_PROJECT_CHAR_12 	0x7C
#define 	ADDR_PROJECT_CHAR_13 	0x7D
#define 	ADDR_PROJECT_CHAR_14 	0x7E
#define 	ADDR_PROJECT_CHAR_15 	0x7F
#define 	ADDR_PROJECT_CHAR_16 	0x80
#define 	ADDR_PROJECT_CHAR_17 	0x81
#define 	ADDR_PROJECT_CHAR_18 	0x82
#define 	ADDR_PROJECT_CHAR_19 	0x83
#define 	ADDR_PROJECT_CHAR_20 	0x84
#define 	ADDR_PROJECT_CHAR_21 	0x85

//version
#define		ADDR_VERSION_CHAR_0		0x86
#define		ADDR_VERSION_CHAR_1		0x87
#define		ADDR_VERSION_CHAR_2		0x88
#define		ADDR_VERSION_CHAR_3		0x89
#define		ADDR_VERSION_CHAR_4		0x8A
#define		ADDR_VERSION_CHAR_5		0x8B
#define		ADDR_VERSION_CHAR_6		0x8C
#define		ADDR_VERSION_CHAR_7		0x8D
#define		ADDR_VERSION_CHAR_8		0x8E
#define		ADDR_VERSION_CHAR_9		0x8F
#define		ADDR_VERSION_CHAR_10	0x90
#define		ADDR_VERSION_CHAR_11	0x91
#define		ADDR_VERSION_CHAR_12	0x92

#define 	ADDR_OP_TYPE 	0xBA
#define 	ADDR_INI_DCX 	0xBB
#define 	ADDR_INI_DATA 0xBC
#define 	ADDR_READ_FINISH	0xBE

typedef struct
{
	uint16_t HACT;
	uint16_t VACT;
	uint8_t HSA; 
	uint8_t HBP;
	uint8_t HFP; 
	uint8_t VSA;
	uint8_t VBP;
	uint8_t VFP;   		
}FPGAPORCH_TypeDef;

extern uint16_t hsum;
extern uint16_t vsum;

/*********************************************************************************
 * Function statement for external call
 */
void FPGA_FSMC_Config(void);
void FPGA_SPI_Config(void);
void FPGA_SPI3Write(uint8_t dataWrt);
uint8_t FPGA_SPI3Read(void);
void FPGA_Reset(void);
void FPGA_INIT_END_INFO(uint8_t init_end);
void FPGA_PIC_WR_CFG(uint8_t wr_en, uint8_t wr_num, uint16_t bst_num, uint8_t size_rsv, uint16_t sdramlastBurstUse);
void FPGA_Porch_Set(FPGAPORCH_TypeDef FPGA_porchPara);
void FPGA_Display_Set(void);
void FPGA_Info_Set(uint8_t * info);
void FPGA_Project_Set(uint8_t * info);
void FPGA_Version_Set(uint8_t * info);
void FPGA_Info_Visible(uint8_t visible);
void FPGA_DisPattern(uint8_t ptnNum, uint8_t rdata, uint8_t gdata, uint8_t bdata);
void FPGA_DisPicture(uint8_t picNum);
void Version_Set(void);
void Project_Info_Upload(void);
void FPGA_PORT_MAP(uint8_t port0_map);
void FPGA_Initial(void);

#endif /* __FPGA_H */
