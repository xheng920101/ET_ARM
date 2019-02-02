// admesy_colour.h : main header file for the admesy_colour DLL

#ifndef _ADMESY_COLOUR_H_
#define _ADMESY_COLOUR_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
#define DLLIMPORT
#endif /* Not BUILDING_DLL */

//  #pragma once
//  #pragma managed(push)
//  #pragma unmanaged

#define TRUE 1
#define FALSE 0
#define M_PI  3.1415926535897932384626433832795
// Typedef declarations

typedef struct {
        double cx;
        double cy;
} cxcy_t;

typedef struct {
    double u;
    double v;
} uv_colour_t;

typedef struct {
        double R;
        double G;
        double B;
}RGB_data_t;

typedef struct {
        unsigned char R;
        unsigned char G;
        unsigned char B;
}RGB_U8data_t;

typedef struct {
        double X;
        double Y;
        double Z;
} XYZ_space_t;

typedef struct {
        double Y;
        double x;
        double y;
} Yxy_space_t;

typedef struct {
        double L;
        double a;
        double b;
} CIELab_space_t;

typedef struct {
        double L;
        double u;
        double v;
} Luv_space_t;

typedef struct {
        double L;
        double C;
        double H;
}LCH_space_t;

typedef struct {
        double U;
        double V;
        double W;
} UVW_space_t;

typedef struct {
        long dimsize;
        XYZ_space_t data[1];
} XYZ_array_t;
typedef XYZ_array_t **XYZ_array;

typedef struct {
        long dimsize;
        Yxy_space_t data[1];
} Yxy_array_t;
typedef Yxy_array_t **Yxy_array;

typedef struct {
        long dimsize;
        CIELab_space_t data[1];
} CIELab_array_t;
typedef CIELab_array_t **CIELab_array;

typedef struct {
        long dimsize;
        cxcy_t data[1];
} cxcy_array_t;
typedef cxcy_array_t **cxcy_array;

typedef struct {
        long dimsize;
        RGB_data_t data[1];
} RGB_array_t;
typedef RGB_array_t **chromRGB_array;

typedef struct {
	double real;
	double imag;
} cplx;

typedef struct {
	double freq;
	double mag;
} FFT;

typedef struct {
    double x1;
    double x2;
    double x3;
    double y1;
    double y2;
    double y3;
    double z1;
    double z2;
    double z3;    
} CALMATRIX;

//structs for colour temperature calculation 
// LERP(a,b,c) = linear interpolation macro, is 'a' when c == 0.0 and 'b' when c == 1.0 
#define LERP(a,b,c)     (((b) - (a)) * (c) + (a))

typedef struct{
        double  u;
        double  v;
        double  t;
} UVT ;


typedef struct {
        int  wl;
        double  x;
        double  y;
} WL;

typedef struct {
        double dwl;
        double x;
        double y;
        double pe;
} DOMWL;

typedef struct {
    unsigned int wl;
    double xl;
    double yl;
    double zl;
    double s1;
    double s2;
    double s3;
} DSPEC;

typedef struct {
    double wavelength;
    double sample[16];
}CRISAMP;

typedef struct {
        double lambda;
        double x;
        double y;
        double z;
	    double x10;
	    double y10;
	    double z10;
} colormatch;

typedef struct {
        char name[16];
        double rx;
        double ry;
        double rY;
        double gx;
        double gy;
        double gY;
        double bx;
        double by;
        double bY;
        double wx;
        double wy;
        double gamma;
} RGBsystem;

typedef struct {
        double rx;
        double ry;
        double rY;
        double gx;
        double gy;
        double gY;
        double bx;
        double by;
        double bY;
        double wx;
        double wy;
        double gamma;
} RGBspace;

/**
 * Overall data structure for a CIECAM02 color
 */
struct CIECAM02color {
  double x, y, z;
  double J, C, h, H;
  double Q, M, s;
  double ac, bc;
  double as, bs;
  double am, bm;
};

/**
 * Overall data structure for CIECAM02 viewing conditions
 */
struct CIECAM02vc {
  double xw, yw, zw, aw;
  double la, yb;
  int surround;
  double n, z, f, c, nbb, nc, ncb, fl, d;
};

// function prototypes


DLLIMPORT void bwlp_filter(double* samples, int count, int rate, int freq);
DLLIMPORT void fft(double *signal, double dt, FFT *fft, double *df, unsigned long npt);
DLLIMPORT void dft(cplx *x, cplx *fft, unsigned long npt);
DLLIMPORT void idft(cplx *x, cplx *fft, unsigned long npt);
DLLIMPORT double calc_JEITA_flicker (double *signal, double dt, unsigned long npt);
DLLIMPORT double calc_VESA_flicker (double *signal, double dt, unsigned long npt);
DLLIMPORT double calc_flicker_mm (double *signal, double dt, unsigned long npt, char db);
DLLIMPORT double calc_flicker_rms (double *signal, double dt, unsigned long npt, char db);

#undef GLOBAL_VARS

//#pragma managed(pop)

#endif
