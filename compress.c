#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>
#include "dctQuantConversions.h"
#include <bitpack.h>
#define A2 A2Methods_Array2

extern void compress(FILE *input)
{
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    int h = img->height;
    int w = img->width;
    if(h % 2 == 1)
    {
        h = h - 1;
    } 
    if(w % 2 == 1)
    {
        w = w - 1;
    }

    A2 rgbFloatArray = scaledIntToFloat(img); // rgb scaled int to rgb float
    A2 ybrFloatArray = rgbFloatToYbrFloat(rgbFloatArray, h, w); // rgb float to ybr
    A2 quantizedArray = reduce(ybrFloatArray, h, w); // ybr to quantize
    //packing
    (void) quantizedArray;
}

extern void decompress(FILE *input)
{    
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    int h = img->height;
    int w = img->width;

    A2 ybrFloatArray = expand(quantizedArray); //quantize to ybr
    A2 rgbFloatArray = ybrFloatToRgbFloat(ybrFloatArray); // ybr to rgb float
    A2 scaledIntArray = rgbFloatToScaledInt(rgbFloatArray); // rbg float to rgb scaled int
/*    // stage 1 compress
    A2 rgbFloatArrayC = scaledIntToFloat(img); 
    // stage 2 compress
    A2 ybrFloatArrayC = rgbFloatToYbrFloat(rgbFloatArrayC, h, w);    
    // stage 3 compress
    A2 quantizedArrayC = reduce(ybrFloatArrayC, h, w);


    // stage 3 decompress
    A2 ybrFloatArrayD = expand(quantizedArrayC);
    // stage 2 decompress
    A2 rgbFloatArrayD = ybrFloatToRgbFloat(ybrFloatArrayD);
    // stage 1 decompress
    A2 scaledIntArrayD = rgbFloatToScaledInt(rgbFloatArrayD);
*/    
    struct Pnm_ppm returnImg;
    returnImg.width = w;
    returnImg.height = h;
    returnImg.denominator = 255;
    returnImg.pixels = scaledIntArrayD;
    returnImg.methods = methods;
//    (void)returnImg;    
    Pnm_ppmwrite(stdout, &returnImg);
}
#undef A2
