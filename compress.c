#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>
#include "dctQuantConversions.h"

#define A2 A2Methods_Array2

// TODO: Trim image
extern void compress(FILE *input)
{
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    int h = img->height;
    int w = img->width;
     
    A2 rgbFloatArray = scaledIntToFloat(img);
    A2 ybrFloatArray = rgbFloatToYbrFloat(rgbFloatArray, h, w);
    A2 quantizedArray = reduce(ybrFloatArray, h, w);
    (void) quantizedArray;
}

extern void decompress(FILE *input)
{
    (void) input;     
}
#undef A2
