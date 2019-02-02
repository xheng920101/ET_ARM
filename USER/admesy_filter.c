/*
Copyright (c) 2003-2010, ADMESY B.V.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



/* filter functions */

#define _USE_MATH_DEFINES	
#define M_PI  3.1415926535897932384626433832795
#include "include.h"
#include "admesy_colour.h"
#include <math.h>
#include <stdlib.h>
//#include <malloc.h>
                    
DLLIMPORT void bwlp_filter(double* samples, int count, int rate, int freq)
{
    unsigned long i;
    double c,a1,a2,a3,b1,b2,r;
    double in, in1, in2, out1, out2;
    double *data;
    
    data = (double*) malloc (sizeof(double)*count);
    if(data ==NULL)
	{
		printf("Out of memory, when define *data!\r\n");
		return;
	}
    r =sqrt(2);
    c = 1.0 / tan(M_PI * freq / rate);
    a1 = 1.0 / ( 1.0 + r * c + c * c);
    a2 =2* a1;
    a3 = a1;
    b1 = 2.0 * ( 1.0 - c*c) * a1;
    b2 = ( 1.0 - r * c + c * c) * a1;

    in = samples[0];
    in1 = 0;
    in2 = 0;
    out1 = 0;
    out2 = 0;
    data[0] = a1 * in + a2 * in1 + a3 * in2 - b1*out1 - b2*out2;
    in = samples[1];
    data[1] = a1 * in + a2 * in1 + a3 * in2 - b1*out1 - b2*out2;
    
    for (i=2;i<count;i++)
    {
        data[i] = a1 * samples[i] + a2 * samples[i-1] + a3 * samples[i-2] - b1*data[i-1] - b2*data[i-2];
       
    }
    for (i=0;i<count;i++)
    {
        samples[i] = data[i];
    }
	free(data);
}    


        
         
           
            
            
           
    
    

