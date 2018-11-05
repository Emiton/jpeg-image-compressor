#include "rgbComponentConversions.h"

typedef struct rgb_float {
    float r, g, b;
} *rgb_float;

//TODO: make return type A2.
void scaledIntToFloat(Pnm_ppm img)
{
    float denom = (float) img->denominator;
    printf("DENOM: %f\n", denom);
    (void) img;
    printf("vegan soup\n"); 
}   
