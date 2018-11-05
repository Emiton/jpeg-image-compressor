#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>

extern void compress(FILE *input)
{
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    A2 pixmap = img->pixels;
    float denom = img->denominator;
    scaledIntToFloat(pixmap, denom);
}

extern void decompress(FILE *input)
{
    (void) input;     
}


