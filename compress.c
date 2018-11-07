#include "compress.h"
#include "pnm.h"
#include "rgbComponentConversions.h"
#include <a2plain.h>
#include <a2methods.h>

#define A2 A2Methods_Array2

extern void compress(FILE *input)
{
    A2Methods_T methods = array2_methods_plain;
    Pnm_ppm img = Pnm_ppmread(input, methods);
    
    A2 a2Temp = scaledIntToFloat(img);
    int h = img->height;
    int w = img->width;
    int i = 0; // REMOVE
    //TODO: do the values of the returned A2 print correctly?
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
           //printf("ABOUT TO PRINT: \n"); 
           //Pnm_rgb rgbTemp = (Pnm_rgb) methods->at(img->pixels, col, row);
           //printf("R: %i, G: %i, B: %i\n", rgbTemp->red, rgbTemp->green, rgbTemp->blue); 
           rgb_float floatTemp = (rgb_float) methods->at(a2Temp, col, row);
           printf("CHECK COMPRESS: %f, G: %f, B: %f\n", floatTemp->r, floatTemp->g, floatTemp->b);
           i++;
        }
    }
    printf("COUNT: %i\n", i);
 
    
}

extern void decompress(FILE *input)
{
    (void) input;     
}
#undef A2
