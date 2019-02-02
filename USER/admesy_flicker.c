/*
Copyright (c) 2003-2015, ADMESY B.V.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/* functions for flicker measurement
 *
 * This includes FFT, Hanning window and JEITA/VESA filer measurement method
 */
 
#define _USE_MATH_DEFINES
#include <stdlib.h>
//#include <malloc.h>
#include <math.h>
#include <float.h>
#include "admesy_colour.h"
#include "_kiss_fft_guts.h"

DLLIMPORT void fft(double *signal, double dt, FFT *myfft, double *df, unsigned long npt)
{
    kiss_fft_cfg  kiss_fft_state;
    kiss_fft_cpx  *cin;
    kiss_fft_cpx  *cout;
    unsigned long i;
    float maxfreq, interval;

    cin  = (kiss_fft_cpx *) malloc((npt+1)*sizeof(kiss_fft_cpx));
    cout = (kiss_fft_cpx *) malloc((npt+1)*sizeof(kiss_fft_cpx));
    if(cin == NULL || cout==NULL)
	{
		printf("Out of memory , when define *cin ||*cout!\r\n");
		return ;
	}
    interval = ((1/dt)/npt);
    maxfreq = (0.5*(1/dt))/interval;
    
    for(i=0;i < npt;i++) {
        cin[i].r = signal[i];
        cin[i].i = 0;
        cout[i].r = 0;
        cout[i].i = 0;
    }
    *df = interval;
    kiss_fft_state = kiss_fft_alloc(npt,0,NULL,NULL);
	if(kiss_fft_state == NULL)
	{
		printf("Out of memory , when define * kiss_fft_state!\r\n");
		return ;
	}
    kiss_fft(kiss_fft_state,cin,cout);
    
    myfft[0].freq = 0;
    myfft[0].mag  = sqrt( (cout[0].i*cout[0].i) + (cout[0].r*cout[0].r) )/ npt;
    for (i=1;i < maxfreq; i++){
        myfft[i].freq = interval*i;
        myfft[i].mag = sqrt(2) * sqrt( (cout[i].i*cout[i].i) + (cout[i].r*cout[i].r) )/ npt;
    }
    free(kiss_fft_state);
	kiss_fft_state=NULL;
    free (cin);
    free (cout);
	cin = NULL;
	cout = NULL;
}

DLLIMPORT void fftold(double *signal, double dt, FFT *myfft, double *df, unsigned long npt)
{
	unsigned long k,n, half;
	double WN, wk, scale, R, I;
    
	WN=2*M_PI/npt;
    scale = sqrt(2)/npt;
    *df = 1/(dt*npt);
    half = (npt/2) + (npt%2);
    
	for (k=0;k<half;k++)
	{
		R=0.0;
		I=0.0;
		wk=k*WN;
		for (n=0; n<npt;n++)
		{
		  R=R+signal[n]*cos(n*wk);
		  I=I-signal[n]*sin(n*wk);
	    }
	    if(k==0)
	    {
           R = R/npt;
           I = I/npt;
           myfft[k].freq = 0.0;
		}
        else
        {
           R = R*scale;
           I = I*scale;
           myfft[k].freq = k/(dt*npt);
        }
        myfft[k].mag  = sqrt ( R*R + I*I );
	}
}

void JEITA_weighing(FFT * myfft, unsigned long points)
{
    double a = -0.0243675714;
    double b = 1.4687671887;
    unsigned long i;
    
    i = 0;
    while (i++ < points)
    {
        if ((myfft[i].freq > 20) && (myfft[i].freq <= 80))
        {
           myfft[i].mag = myfft[i].mag * (a* myfft[i].freq + b);
           if (myfft[i].mag < 0) myfft[i].mag = 0;
        }
        if (myfft[i].freq > 80) myfft[i].mag = 0;
    } 
}

DLLIMPORT double calc_JEITA_flicker (double *signal, double dt, unsigned long npt)
{
    double df, a0,a1, jeita;
    
    unsigned long i;
    FFT *myfft = (FFT*) malloc((npt+2) * sizeof(double));
	if(myfft == NULL)
	{
		printf("Out of memory , when define *myfft!\r\n");
		return 0;
	}
    /* note, our FFT is in RMS this requires a small change in the JEITA formulas */
    fft(signal, dt, myfft, &df, npt);
    JEITA_weighing(myfft, ((npt/2) + (npt%2)));
    
    /* zero hz is always the highest */
    a0 = myfft[0].mag;
    a1 = 0;
    
    i = 2;
    while ( myfft[i].freq <= 65  && (i < ((npt/2) + (npt%2)) - 1) )
    {
      if( (myfft[i].mag > a1) && (myfft[i].mag > myfft[i-1].mag) && (myfft[i].mag > myfft[i+1].mag) )  a1 = myfft[i].mag;
      i++;
    }
    if ( (a0 > 0) && (a1 > 0) ) jeita = 20*log10(a1/a0); 
    else jeita = 0;

    free(myfft);
    return jeita;
}

DLLIMPORT double calc_VESA_flicker (double *signal, double dt, unsigned long npt)
{
    double df, a0,a1, vesa;
    FFT *myfft = (FFT*) malloc((npt+2) * sizeof(double));
    unsigned long i;
    if(myfft == NULL)
	{
		free(myfft);
		printf("Out of memory , when define *myfft!\r\n");
		return 0;
	}
    /* note, our FFT is in RMS this requires a small change in the JEITA formulas */
    fft(signal, dt, myfft, &df, npt);
    JEITA_weighing(myfft, ((npt/2) + (npt%2)));
    
    /* zero hz is always the highest */
    a0 = myfft[0].mag;
    a1 = 0;
    
    i = 2;
    while ( myfft[i].freq <= 80  && (i < ((npt/2) + (npt%2)) - 1) )
    {
      if( (myfft[i].mag > a1) && (myfft[i].mag > myfft[i-1].mag) && (myfft[i].mag > myfft[i+1].mag) )  a1 = myfft[i].mag;
      i++;
    }
    if ( (a0 > 0) && (a1 > 0) ) vesa = 20*log10(a1/a0) + 3.01; 
    else vesa = 0;

    free(myfft);
    return vesa;
}

DLLIMPORT double calc_flicker_mm (double *signal, double dt, unsigned long npt, char db)
{
    double flicker, fmin, fmax, avg;
    unsigned long i;
    
    fmin = 100000000;
    fmax = 0;
    avg = 0;
    for (i=0;i<npt;i++)
	{
        avg += *signal;
		if( *signal  < fmin) fmin = *signal;
		if( *signal > fmax) fmax = *signal;
		signal++;
	}
	avg = avg/npt;
    flicker  = ((fmax-fmin)/avg);
    if(db > 0) flicker = 10* log10(flicker);
    else flicker  = 100 * flicker; /* percentage */
    
    return flicker;
}

DLLIMPORT double calc_flicker_rms (double *signal, double dt, unsigned long npt, char db)
{
    double flicker, avg, square, squares;
    double *start;
    unsigned long i;
    
    start = signal;
    /* first get the average of the signal */
    avg =0;
    for (i=0;i<npt;i++)
	{
        avg += *signal;
		signal++;
	}
	avg = avg/npt;
	/* reset pointer and calc RMS */
    signal = start;
    squares = 0;  
    for (i=0;i<npt;i++)
	{
        square = (*signal - avg);
		squares += (square * square);
		signal++;
	}
    flicker  =  (sqrt (squares/npt))/avg;
    if(db > 0) flicker = 10* log10(flicker);
    else flicker  = 100* flicker; /* percentage */
    
    return flicker;
}

DLLIMPORT void dft(cplx *x, cplx *fft, unsigned long npt)
{
	unsigned long k,n;
	double WN, wk, c, s;//, XR[100000], XI[100000];
    double R,I;
    
	WN=2*M_PI/npt;

	for (k=0;k<npt;++k)
	{
		R=0.0;
		I=0.0;
		wk=k*WN;
		for (n=0; n<npt;++n)
		{
		  c=cos(n*wk);
		  s=sin(n*wk);
		  R=R+x[n].real*c+x[n+1].imag*s;
		  I=I-x[n].real*s+x[n+1].imag*c;
	    }
		fft[k].real=R;
		fft[k].imag=I;
	}
}

DLLIMPORT void idft(cplx *x, cplx *fft, unsigned long npt)
{
	unsigned long k,n;
	double WN, wk, c, s;
    double R,I;
    
	WN=-2*M_PI/npt;
	for (k=0;k<npt;++k)
	{
		R=0.0;
		I=0.0;
		wk=k*WN;
		for (n=0; n<npt;++n)
		{
		  c=cos(n*wk);
		  s=sin(n*wk);
		  R=R+x[n+1].real*c+x[n].imag*s;
		  I=I-x[n+1].real*s+x[n].imag*c;
	    }
	     /* divide by N for IDFT */			
		R=R/npt;
		I=I/npt;
		fft[k].real=R;
		fft[k].imag=I;
	}
}

