#ifndef __TEST_OTP_H
#define __TEST_OTP_H

#include "board_conf.h"

/*********************************************************************************
 * Function statement for external call
 */
void Test_OTP(void);
ErrorStatus OTP(void);
ErrorStatus AutoVcom(void);
ErrorStatus Gamma_Check(void);
ErrorStatus Contrast_Check(void);
ErrorStatus Chroma_Check(void);

#endif /* __TEST_OTP_H */
