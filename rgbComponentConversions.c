#include "rgbComponentConversions.h"
#include <string.h>

#define A2 A2Methods_Array2

extern A2 scaledIntToFloat(Pnm_ppm img)
{
    assert(img);
    int h = img->height;
    assert(h > 0);
    int w = img->width;
    assert(w > 0);
    float denom = (float) img->denominator;
    assert(denom > 0.0 && denom < 65536.0);
    A2 pixmap = img->pixels;
    assert(pixmap); // TODO: check this out later
    
    A2Methods_T methods = array2_methods_plain;

    A2 floatMap = methods->new(methods->width(pixmap), 
                               methods->height(pixmap), 
                               sizeof(struct rgb_float));

    /* Get Pnm_rgb from original A2
     * Get rgb_float struct from output A2
     * Create values and stick them in output struct 
     * */
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            Pnm_rgb intTemp = (Pnm_rgb) methods->at(pixmap, col, row);
            rgb_float floatTemp = (rgb_float) methods->at(floatMap, col, row);
            floatTemp->r = ((float) intTemp->red) / denom;
            floatTemp->g = ((float) intTemp->green) / denom;
            floatTemp->b = ((float) intTemp->blue) / denom;
        }
    }

     return floatMap;
}
#undef A2
