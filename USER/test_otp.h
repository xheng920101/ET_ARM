#ifndef __TEST_OTP_H
#define __TEST_OTP_H

#include "board_conf.h"

extern char OTP_FLAG;

/*********************************************************************************
 * Function statement for external call
 */
void Test_OTP(void);
ErrorStatus OTP(void);
uint8_t AutoVcom(void);
uint8_t AutoVcom1(void);
ErrorStatus Gamma_Check(void);
ErrorStatus Gamma_Check1(void);
ErrorStatus Contrast_Check(void);
ErrorStatus Chroma_Check(void);

#endif /* __TEST_OTP_H */
