#include <a2methods.h>
#include <a2plain.h>
#include "pnm.h"
#include <stdio.h>
#include <assert.h>
#include <array2.h>

typedef A2Methods_Array2 A2;

// TODO: Should this be a pointer type or not?
/*
typedef struct rgb_float {
    float r, g, b;
} *rgb_float;
*/
typedef struct rgb_float {
    float r, g, b;
} *rgb_float;

extern A2 scaledIntToFloat(Pnm_ppm img);
