#ifndef _RGBCOMPONENTCONVERSIONSH_
#define _RGBCOMPONENTCONVERSIONSH_

#include <a2methods.h>
#include <a2plain.h>
#include "pnm.h"
#include <stdio.h>
#include <assert.h>
#include <array2.h>

#define A2 A2Methods_Array2

typedef struct rgb_float {
    float r, g, b;
} *rgb_float;

typedef struct ybr_float {
    float y, Pb, Pr;
} *ybr_float;

extern A2 scaledIntToFloat(Pnm_ppm img);
extern A2 rgbFloatToYbrFloat(A2 rgbFloatArray, int h, int w);
#undef A2
#endif
