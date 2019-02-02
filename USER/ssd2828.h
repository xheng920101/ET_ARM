#ifndef __SSD2828_H
#define __SSD2828_H

#include "board_conf.h"

#define		PORT0	0x01
#define		PORT1	0x02

extern uint16_t SSD_B7;

/*********************************************************************************
 * SSD2828 address mapping
 */
#define			SSD_CFGR		0xB7    /*!<RGB Interface Control Register7		HS/LP DCS/GEN CLK VM...*/
#define     SSD_CFGR_HS     0x0001    
#define     SSD_CFGR_CKE    0x0002
#define     SSD_CFGR_SLP    0x0004
#define     SSD_CFGR_VEN    0x0008
#define     SSD_CFGR_HCLK   0x0010
#define     SSD_CFGR_CSS    0x0020
#define     SSD_CFGR_DCS    0x0040
#define     SSD_CFGR_REN    0x0080
#define     SSD_CFGR_ECD    0x0100
#define     SSD_CFGR_EOT    0x0200
#define     SSD_CFGR_LPE    0x0400

/*********************************************************************************
 * Function statement for external call
 */
void SSD2828_SPI_Config(void);
void MIPI_Reset(void);
void IC_Init(uint16_t * initCode);
uint16_t SPI3Read(uint8_t port);
void WriteCMD2SSD(uint8_t port, uint8_t CMD);
void WriteDATA2SSD(uint8_t port, uint8_t DATA);
void WriteSSDReg(uint8_t port, uint8_t reg, uint16_t para);
uint16_t ReadSSDReg(uint8_t port, uint8_t reg);
void WriteDriverReg(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void ReadDriverReg(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void MIPI_DCS_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void MIPI_GEN_Write(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void MIPI_DCS_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void MIPI_GEN_Read(uint8_t port, uint8_t reg, uint8_t nPara, uint8_t * para);
void SSD2828_Init(void);

#endif /* __SSD2828_H */
