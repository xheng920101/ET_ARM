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
 * �������Ͷ���
 * λͼ�ļ�ͷ��Ϣ�ṹ����
 * ���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
 */
typedef struct tagBITMAPFILEHEADER 
{   
    DWORD bfSize; 															//�ļ���С																				4
    WORD bfReserved1; 													//�����֣�������		 															2
    WORD bfReserved2; 													//�����֣�ͬ��			 															2
    DWORD bfOffBits; 														//ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��	4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; 														//ָ���˽ṹ��ĳ��ȣ�Ϊ40		 												4
    LONG biWidth; 														//λͼ��											 												4
    LONG biHeight; 														//λͼ��											 												4
    WORD biPlanes; 														//ƽ������Ϊ1								 													2
    WORD biBitCount; 													//������ɫλ����������1��2��4��8��16��24�µĿ�����32	2
    DWORD biCompression; 											//ѹ����ʽ��������0��1��2������0��ʾ��ѹ��						4
    DWORD biSizeImage; 												//ʵ��λͼ����ռ�õ��ֽ���														4
    LONG biXPelsPerMeter; 										//X����ֱ���																					4
    LONG biYPelsPerMeter; 										//Y����ֱ���																					4
    DWORD biClrUsed; 													//ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)			4
    DWORD biClrImportant; 										//��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��				4
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

