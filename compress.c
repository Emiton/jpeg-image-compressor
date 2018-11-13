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
    A2 wordMap = packing(quantizedArray);
    
    for(int row = 0; row  < height / 2; row++){
        for(int col = 0; col < width / 2; col++)
        {
            uint64_t *packed_word = methods->at(wordMap, col, row);
            putchar(Bitpack_getu(*packed_word, 9, 23));
            putchar(Bitpack_getu(*packed_word, 5, 18));
            putchar(Bitpack_getu(*packed_word, 5, 13));
            putchar(Bitpack_getu(*packed_word, 5, 8));
            putchar(Bitpack_getu(*packed_word, 4, 4));
            putchar(Bitpack_getu(*packed_word, 4, 0));
        }
    } 
    
}
/*
extern void decompress(FILE *input)
{
   // A2Methods_T methods = array2_methods_plain;
   // A2 ybrFloatArray = expand((void) input;     
   (void) input;
}
*/
extern void decompress(FILE *input)
{    
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    int h = img->height;
    int w = img->width;
     
    A2 quantizedArray = unpack(wordMap);
    // stage 3 decompress
    A2 ybrFloatArray = expand(quantizedArray);
    // stage 2 decompress
    A2 rgbFloatArray = ybrFloatToRgbFloat(ybrFloatArray);
    // stage 1 decompress
    A2 scaledIntArray = rgbFloatToScaledInt(rgbFloatArray);
    
    struct Pnm_ppm returnImg;
    returnImg.width = w;
    returnImg.height = h;
    returnImg.denominator = 255;
    returnImg.pixels = scaledIntArray;
    returnImg.methods = methods;
//    (void)returnImg;    
    Pnm_ppmwrite(stdout, &returnImg);
}
#undef A2
