#ifndef __BSP_BMP_H
#define	__BSP_BMP_H

#include "stm32f2xx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
#pragma diag_suppress 870 	//avoid warning: invalid multibyte character sequence

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

/*********************************************************************************
 * 数据类型定义
 * 位图文件头信息结构定义
 * 其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）
 */
typedef struct tagBITMAPFILEHEADER 
{   
    DWORD bfSize; 															//文件大小																				4
    WORD bfReserved1; 													//保留字，不考虑		 															2
    WORD bfReserved2; 													//保留字，同上			 															2
    DWORD bfOffBits; 														//实际位图数据的偏移字节数，即前三个部分长度之和	4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; 														//指定此结构体的长度，为40		 												4
    LONG biWidth; 														//位图宽											 												4
    LONG biHeight; 														//位图高											 												4
    WORD biPlanes; 														//平面数，为1								 													2
    WORD biBitCount; 													//采用颜色位数，可以是1，2，4，8，16，24新的可以是32	2
    DWORD biCompression; 											//压缩方式，可以是0，1，2，其中0表示不压缩						4
    DWORD biSizeImage; 												//实际位图数据占用的字节数														4
    LONG biXPelsPerMeter; 										//X方向分辨率																					4
    LONG biYPelsPerMeter; 										//Y方向分辨率																					4
    DWORD biClrUsed; 													//使用的颜色数，如果为0，则表示默认值(2^颜色位数)			4
    DWORD biClrImportant; 										//重要颜色数，如果为0，则表示所有颜色都是重要的				4
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;

typedef struct tagRGBQUAD 
{
    BYTE rgbBlue; 													//R channel
    BYTE rgbGreen; 													//G channel
    BYTE rgbRed; 														//B channel
    BYTE rgbReserved;											 	//reserved
} RGBQUAD,tagRGBQUAD;

typedef struct RGB_PIXEL
{      
    unsigned char   rgbBlue;
    unsigned char   rgbGreen;
    unsigned char   rgbRed;
}RGB_PIXEL;

extern ErrorStatus Pic_Load;
extern FlagStatus Pic_Load_Finish;

/*********************************************************************************
 * Function statement for external call
 */
ErrorStatus Lcd_Load_bmp(LONG Bmp_Width, LONG Bmp_Hight, unsigned char *Pic_Name);
ErrorStatus PIC_Load_BMP(uint8_t picNum);
ErrorStatus PIC_Load_BMP_ET(uint8_t * picName);
ErrorStatus SD_Write_Str2File(char *File_Name, char *Write_String);

#endif /* __BSP_BMP_H */

