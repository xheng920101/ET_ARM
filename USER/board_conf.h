/*
 * POWER	------------------
 *			| PD3  - EN_VCIM3   |
 *      | PD4  - EN_IOVCCM3 |
 *			| PD6  - EN_LEDM3   |
 *      | PD7  - EN_VSNM3   |
 *			| PG9  - EN_VSPM3   |
 *      | PB14 - EN_OTPM3   |
 *        ------------------
 * LED		------------------------
 *			| PE3  - LED_Flag4(RED)   |
 *      | PE2  - LED_Flag5(GREEN) |
 *      | PF5  - LED_Flag6(BLUE)  |
 *        ------------------------
 * KEY		---------------
 *			| PC4  - M3_SW1  |
 *      | PC5  - M3_SW2  |
 *      | PB0  - M3_SW3  |
 *      | PB1  - M3_SW4  |
 *        ---------------
 * MIPI		------------------------
 *      | PA4  - ARM_SPI0_SCSM3   |
 *			| PA5  - ARM_SPI0_SCKM3   |
 *      | PA6  - ARM_SPI0_SDOM3   |
 *			| PA7  - ARM_SPI0_SDIM3   |
 *      | PB12 - ARM_SPI1_SCSM3   |
 *			| PG2  - ARM_MIPI_RESET   |
 *      | PG8  - ARM_SPI0_SDCM3   |
 *        ------------------------
 * FPGA		------------------------
 *      | PF11 - FPGA_ARM_SCSM3   |
 *			| PF12 - FPGA_ARM_SCKM3   |
 *      | PF13 - FPGA_ARM_SDOM3   |	// USE AS FPGA_RESET
 *			| PF14 - FPGA_ARM_SDIM3   |
 *        ------------------------
 * LCD		--------------------
 *     	| PG10 - LCD_RSTM3  	 |
 *        --------------------
 * AD7994	 ---------------------------
 *			| PE4(TEST29)  - ADC_I2C_SCL  |
 *			| PE5(TEST30)  - ADC_I2C_SDA  |
 *			| PE6(TEST32)  - ADC_I2C      |
 *        ----------------------------
 * TEST		--------------------------
  *			| PG13(TEST15) - CTP2				|
 *			| PG15(TEST17) - PWM_DETECT	/ PWM_OUTPUT|
 *			| PB5(TEST18)  - CTP1			  |
 *			| PB6(TEST19)  - CTP0				|
 *			| PB7(TEST20)  - TP_INT		  |
 *			| PB8(TEST21)  - TP_SCL		  |
 *			| PB9(TEST22)  - TP_SDA			|
 *			| PC2(TEST23)  - I_IOVCC		|
 *			| PC3(TEST24)  - CONNECTOR2  / TP_RST |
 *			| PG3(TEST25)  - CONNECTOR1 |
 *			| PF10(TEST4)  - RES_TEST |
 *			| PG11(TE11)  - TE_DETECT |
 *			| PC13(TEST1)  - CTP START |
 *			| PF6(TEST2)  -  CTP ACK|
 *			| PA8(TEST31)  - solenoid valve  |
 *			| PB10(TEST26)  - AUTO SET  |
 *      	--------------------------
 * TPS65312	 ---------------------------
 *			| PB5(TEST18)  - POWER_I2C_SDA  |
 *			| PB6(TEST19)  - POWER_I2C_SCL  |
 *        ----------------------------
 * EEPROM(AT24C512)	 ------------
 *			| PF1  - EEPROM_I2C_SDA  |
 *			| PF0  - EEPROM_I2C_SCL  |
 *			| PF4  - EEPROM_I2C_WP   |
 *        -----------------------
 */
										  
#ifndef __BOARD_CONF_H
#define __BOARD_CONF_H

#include "stm32f2xx.h"

extern char *prj_no;

/*********************************************************************************
 * POWER configure
 */
#define    POWER_VCI_PIN			GPIO_Pin_3
#define    POWER_VCI_GPIO_PORT		GPIOD

#define    POWER_IOVCC_PIN			GPIO_Pin_4
#define    POWER_IOVCC_GPIO_PORT	GPIOD

#define    POWER_OTP_PIN			GPIO_Pin_14
#define    POWER_OTP_GPIO_PORT		GPIOB

#define    POWER_LED_PIN			GPIO_Pin_6
#define    POWER_LED_GPIO_PORT		GPIOD

#define    POWER_VSN_PIN			GPIO_Pin_7
#define    POWER_VSN_GPIO_PORT		GPIOD

#define    POWER_VSP_PIN			GPIO_Pin_9
#define    POWER_VSP_GPIO_PORT		GPIOG

/*********************************************************************************
 * TEST configure
 */
#define    TEST15_PIN                 GPIO_Pin_13
#define    TEST15_GPIO_PORT           GPIOG 
 
#define    TEST17_PIN                 GPIO_Pin_15
#define    TEST17_GPIO_PORT           GPIOG

#define    TEST18_PIN                 GPIO_Pin_5
#define    TEST18_GPIO_PORT           GPIOB

#define    TEST19_PIN                 GPIO_Pin_6
#define    TEST19_GPIO_PORT           GPIOB

#define    TEST20_PIN                 GPIO_Pin_7
#define    TEST20_GPIO_PORT           GPIOB

#define    TEST21_PIN                 GPIO_Pin_8
#define    TEST21_GPIO_PORT           GPIOB

#define    TEST22_PIN                 GPIO_Pin_9
#define    TEST22_GPIO_PORT           GPIOB

#define    TEST23_PIN                 GPIO_Pin_2
#define    TEST23_GPIO_PORT           GPIOC

#define    TEST24_PIN                 GPIO_Pin_3
#define    TEST24_GPIO_PORT           GPIOC

#define    TEST25_PIN                 GPIO_Pin_3
#define    TEST25_GPIO_PORT           GPIOG

#define    TEST4_PIN                  GPIO_Pin_10
#define    TEST4_GPIO_PORT            GPIOF

#define    TE_PIN                 		GPIO_Pin_11
#define    TE_GPIO_PORT            		GPIOG

#define    CTP_START_PIN              GPIO_Pin_13
#define    CTP_START_GPIO_PORT        GPIOC

#define    CTP_ACK_PIN                GPIO_Pin_6
#define    CTP_ACK_GPIO_PORT          GPIOF

#define    TEST31_PIN                  GPIO_Pin_8
#define    TEST31_GPIO_PORT            GPIOA

#define    TEST26_PIN                  GPIO_Pin_10
#define    TEST26_GPIO_PORT            GPIOB

/*********************************************************************************
 * LED configure
 */
typedef enum
{
    RED = 0,
    GREEN,
    BLUE
} LED_TypeDef;

#define    LED1_PIN                 GPIO_Pin_3
#define    LED1_GPIO_PORT           GPIOE

#define    LED2_PIN                 GPIO_Pin_2
#define    LED2_GPIO_PORT           GPIOE

#define    LED3_PIN                 GPIO_Pin_5
#define    LED3_GPIO_PORT           GPIOF

/*********************************************************************************
 * KEY configure
 */
#define    KEY1_PIN                 GPIO_Pin_4
#define    KEY1_GPIO_PORT           GPIOC            

#define    KEY2_PIN                 GPIO_Pin_5
#define    KEY2_GPIO_PORT           GPIOC

#define    KEY3_PIN                 GPIO_Pin_0
#define    KEY3_GPIO_PORT           GPIOB

#define    KEY4_PIN                 GPIO_Pin_1
#define    KEY4_GPIO_PORT           GPIOB

/*********************************************************************************
 * interface to SSD2828 configure
 */
#define    SSD2828_SPI_nCS0_PIN          GPIO_Pin_4
#define    SSD2828_SPI_nCS0_GPIO_PORT    GPIOA           

#define    SSD2828_SPI_nCS1_PIN          GPIO_Pin_12
#define    SSD2828_SPI_nCS1_GPIO_PORT    GPIOB

#define    SSD2828_SPI_SCK_PIN           GPIO_Pin_5
#define    SSD2828_SPI_SCK_GPIO_PORT     GPIOA

#define    SSD2828_SPI_SDO_PIN           GPIO_Pin_6
#define    SSD2828_SPI_SDO_GPIO_PORT     GPIOA

#define    SSD2828_SPI_SDI_PIN           GPIO_Pin_7
#define    SSD2828_SPI_SDI_GPIO_PORT     GPIOA

#define    SSD2828_SPI_SDC_PIN           GPIO_Pin_8
#define    SSD2828_SPI_SDC_GPIO_PORT     GPIOG

#define    SSD2828_nRST_PIN              GPIO_Pin_2
#define    SSD2828_nRST_GPIO_PORT        GPIOG

/*********************************************************************************
 * interface to FPGA configure
 */
#define    FPGA_SPI_nCS_PIN          GPIO_Pin_11
#define    FPGA_SPI_nCS_GPIO_PORT    GPIOF       

#define    FPGA_SPI_SCK_PIN          GPIO_Pin_12
#define    FPGA_SPI_SCK_GPIO_PORT    GPIOF

#define    FPGA_SPI_SDO_PIN          GPIO_Pin_13
#define    FPGA_SPI_SDO_GPIO_PORT    GPIOB

#define    FPGA_SPI_SDI_PIN          GPIO_Pin_14 
#define    FPGA_SPI_SDI_GPIO_PORT    GPIOF

#define    FPGA_nRst_PIN         		 GPIO_Pin_13
#define    FPGA_nRst_GPIO_PORT       GPIOF 

#define    FPGA_MCU_CSX_PIN					 GPIO_Pin_11
#define  	 FPGA_MCU_CSX_GPIO_PORT		 GPIOD

/*********************************************************************************
 * interface to LCD configure
 */
#define    LCD_nRST_PIN              GPIO_Pin_10
#define    LCD_nRST_GPIO_PORT        GPIOG    

/*********************************************************************************
 * interface to AD7994 configure
 */
#define    DSENSOR_I2C_SCL_PIN          GPIO_Pin_4
#define    DSENSOR_I2C_SCL_GPIO_PORT    GPIOE

#define    DSENSOR_I2C_SDA_PIN          GPIO_Pin_5
#define    DSENSOR_I2C_SDA_GPIO_PORT    GPIOE      

/*********************************************************************************
 * interface to TPS65312 VSP¡¢VSN configure
 */
#define    POWER_I2C_SCL_PIN          GPIO_Pin_6
#define    POWER_I2C_SCL_GPIO_PORT    GPIOB

#define    POWER_I2C_SDA_PIN          GPIO_Pin_5
#define    POWER_I2C_SDA_GPIO_PORT    GPIOB

/*********************************************************************************
 * interface to EEPROM configure
 */
#define    EEPROM_I2C_SCL_PIN   			GPIO_Pin_0
#define    EEPROM_I2C_SCL_GPIO_PORT   GPIOF 

#define    EEPROM_I2C_SDA_PIN   			GPIO_Pin_1
#define    EEPROM_I2C_SDA_GPIO_PORT   GPIOF 

#define    EEPROM_I2C_WP_PIN   				GPIO_Pin_4
#define    EEPROM_I2C_WP_GPIO_PORT  	GPIOF 

/*********************************************************************************
 * Function statement for external call
 */
void Board_Init(void);
void NVIC_Config(void);
	
#endif /* __BOARD_CONF_H */
