// TODO: pass in methods to files
#include "rgbComponentConversions.h"
#include <string.h> // TODO: remove

#define A2 A2Methods_Array2
// TODO: Change argument(s) to A2, denominator, w, h
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
   // int testWidth = Array2_width(floatMap);
   // printf("TESTWIDTH: %i\n", testWidth); 
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

// TODO: Restructure/Clean up function
extern A2 rgbFloatToYbrFloat(A2 rgbFloatArray, int h, int w)
{
    assert(rgbFloatArray);
    assert(h > 0);
    assert(w > 0);
    
    A2Methods_T methods = array2_methods_plain;

    A2 ybrMap = methods->new(methods->width(rgbFloatArray),
                             methods->height(rgbFloatArray),
                             sizeof(struct ybr_float));
   
     for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            rgb_float rgbFloatTemp = (rgb_float) methods->at(rgbFloatArray, col, row);
            ybr_float ybrFloatTemp = (ybr_float) methods->at(ybrMap, col, row);

            float red = rgbFloatTemp->r;
            float green = rgbFloatTemp->g;
            float blue = rgbFloatTemp->b;
            
            ybrFloatTemp->y = 0.299 * red + 0.587 * green + 0.114 * blue;
            ybrFloatTemp->Pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
            ybrFloatTemp->Pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;
            //printf("YBR VALUES: Y: %f, B: %f, R: %f\n", ybrFloatTemp->y, ybrFloatTemp->Pb, ybrFloatTemp->Pr);
        }
    }
    
    return ybrMap;
}

extern A2 ybrFloatToRgbFloat(A2 ybrFloatMap)
{
    assert(ybrFloatMap);
    int w = Array2_width(ybrFloatMap);
    assert(w > 0);
    int h = Array2_height(ybrFloatMap);
    assert(h > 0);
    
    A2Methods_T methods = array2_methods_plain;

    A2 rgbFloatMap = methods->new(methods->width(ybrFloatMap),
                             methods->height(ybrFloatMap),
                             sizeof(struct rgb_float));
   
     for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            ybr_float ybrFloatTemp = (ybr_float) methods->at(ybrFloatMap, col, row);
            rgb_float rgbFloatTemp = (rgb_float) methods->at(rgbFloatMap, col, row);
            
            float y = ybrFloatTemp->y;
            float Pb = ybrFloatTemp->Pb;
            float Pr = ybrFloatTemp->Pr;
            
            
            rgbFloatTemp->r = 1.0 * y + 0.0 * Pb + 1.402 * Pr;
            rgbFloatTemp->g = 1.0 * y - 0.344136 * Pb - 0.714136 * Pr;
            rgbFloatTemp->b = 1.0 * y + 1.772 * Pb + 0.0 * Pr;
            printf("RGB FLOAT VALUES: R: %f, G: %f, B: %f\n", rgbFloatTemp->r, rgbFloatTemp->g, rgbFloatTemp->b);
        }
    } 
    return rgbFloatMap;
}

extern A2 rgbFloatToScaledInt(A2 rgbFloatMap)
{
    assert(rgbFloatMap);
    int h = Array2_height(rgbFloatMap);
    assert(h > 0);
    int w = Array2_width(rgbFloatMap);
    assert(w > 0);
    //TODO: denom will be 255
    float denom = 255.0;
    
    //assert(pixmap); // TODO: check this out later
    
    A2Methods_T methods = array2_methods_plain;

    A2 rgbIntMap = methods->new(methods->width(rgbFloatMap), 
                               methods->height(rgbFloatMap), 
                               sizeof(struct rgb_int));
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            rgb_float floatTemp = (rgb_float) methods->at(rgbFloatMap, col, row);
            rgb_int intTemp = (rgb_int) methods->at(rgbIntMap, col, row);
            intTemp->r = (int) roundf(((floatTemp->r) * denom));
            intTemp->g = (int) roundf(((floatTemp->g) * denom));
            intTemp->b = (int) roundf(((floatTemp->b) * denom));
            printf("RGB SCALED INT  VALUES: R: %i, G: %i, B: %i\n", intTemp->r, intTemp->g, intTemp->b);
        }
    }

     return rgbIntMap;
}

#undef A2
