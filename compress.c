#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>
#include "dctQuantConversions.h"
#include "packing.h"

#define A2 A2Methods_Array2
//void roundTripTesting(Pnm_ppm img, FILE *input, int h, int w);
// TODO: When printing, print curren
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
    
/*    
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
*/    
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
    //A2Methods_T methods = array2_methods_plain;
    //Pnm_ppm img = Pnm_ppmread(input, methods);
    //int h = img->height;
    //int w = img->width;
    
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
/* 
    // stage 4 decompress 
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
    returnImg.pixels = scaledIntArrayD;
    returnImg.methods = methods;
//    (void)returnImg;    
    Pnm_ppmwrite(stdout, &returnImg);
*/
    // height and width of original uncompressed image
    //roundTripTesting(img, input, height, width);
}

/*
void roundTripTesting(Pnm_ppm img, FILE *input, int h, int w)
{
    // ROUND TRIP TESTING
    A2Methods_T methods = array2_methods_plain;
    // stage 1 compress
    A2 rgbFloatArrayC = scaledIntToFloat(img);
    // stage 2 compress
    A2 ybrFloatArrayC = rgbFloatToYbrFloat(rgbFloatArrayC, h, w);
    // stage 3 compress
    A2 quantizedArrayC = reduce(ybrFloatArrayC, h, w);
    // stage 4 compress
    putWord(quantizedArrayC, h, w);
    // stage 5 decompress - extra step to get file
    A2 wordMapC = getProcessedWord(input, h, w); 
    // stage 4 decompress 
    A2 quantizedArrayD = unpack(wordMapC);
    // stage 3 decompress
    A2 ybrFloatArrayD = expand(quantizedArrayD);
    // stage 2 decompress
    A2 rgbFloatArrayD = ybrFloatToRgbFloat(ybrFloatArrayD);
    // stage 1 decompress
    A2 scaledIntArrayD = rgbFloatToScaledInt(rgbFloatArrayD);
    struct Pnm_ppm returnImg;
    returnImg.width = w;
    returnImg.height = h;
    returnImg.denominator = 255;
    returnImg.pixels = scaledIntArrayD;
    returnImg.methods = methods;
   
    Pnm_ppmwrite(stdout, &returnImg);
    */
#undef A2
