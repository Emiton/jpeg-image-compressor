#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>
#include "dctQuantConversions.h"
#include "packing.h"

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

    A2 rgbFloatArray = scaledIntToFloat(img);
    A2 ybrFloatArray = rgbFloatToYbrFloat(rgbFloatArray, h, w);
    A2 quantizedArray = reduce(ybrFloatArray, h, w);
    putWord(quantizedArray, h, w);
       
}

extern void decompress(FILE *input)
{    
    
    // DECOMPRESS
    A2Methods_T methods = array2_methods_plain;
    unsigned height, width;
    int read = fscanf(input, "Compressed image format 2\n%u %u", &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
    // stage 5 decompress - extra step to get file
    A2 wordMapC = getProcessedWord(input, height, width); 
    // stage 4 decompress 
    A2 quantizedArrayD = unpack(wordMapC);
    // stage 3 decompress
    A2 ybrFloatArrayD = expand(quantizedArrayD);
    // stage 2 decompress
    A2 rgbFloatArrayD = ybrFloatToRgbFloat(ybrFloatArrayD);
    // stage 1 decompress
    A2 scaledIntArrayD = rgbFloatToScaledInt(rgbFloatArrayD);
    
    struct Pnm_ppm returnImg;
    returnImg.width = (int) width;
    returnImg.height = (int) height;
    returnImg.denominator = 255;
    returnImg.pixels = scaledIntArrayD;
    returnImg.methods = methods;
   
    Pnm_ppmwrite(stdout, &returnImg); 
}

#undef A2
