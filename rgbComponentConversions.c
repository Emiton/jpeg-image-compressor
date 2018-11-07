#include "rgbComponentConversions.h"
#include <string.h>

#define A2 A2Methods_Array2
/*
A2 scaledIntToFloat(Pnm_ppm img)
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
    A2 floatMap = methods->new(methods->width(pixmap), methods->height(pixmap), sizeof(struct rgb_float));
    //TODO: use map to loop through all elements
    //TODO: use at to access a given element
    //TODO: make sure col, row in right order
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            Pnm_rgb intTemp = (Pnm_rgb) methods->at(pixmap, col, row);
            printf("INTS R: %i G: %i B:%i\n", intTemp->red, intTemp->green, intTemp->blue);
            rgb_float floatTemp;
            floatTemp.r = ((float) intTemp->red) / denom;
            floatTemp.g = ((float) intTemp->green) / denom;
            floatTemp.b = ((float) intTemp->blue) / denom;
            printf("FLOATS R: %f G: %f B:%f\n", floatTemp.r, floatTemp.g, floatTemp.b);

            //methods->at(floatMap, col, row)->floatTemp;
            //int mySize = sizeof(floatTemp);
            //memcpy(methods->at(floatMap, col, row), floatTemp, mySize);
            // TODO: use Array2_at??
            *(rgb_float*)Array2_at(floatMap, col, row) = floatTemp;
        }
    }

     return floatMap;
}  
*/
// for a given pixel
//void rgbIntToFloat(int i, int j, A2 pixmap, int denominator, void *data, void *cl)
//{
    //rgb_float temp;
    //temp->r = ((float) img->red) / denom;
    //temp->g = ((float) img->green) / denom;
    //temp->b = ((float) img->blue) / denom;
     
//}
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
    A2 floatMap = methods->new(methods->width(pixmap), methods->height(pixmap), sizeof(struct rgb_float));
    //TODO: use map to loop through all elements
    //TODO: use at to access a given element
    //TODO: make sure col, row in right order
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            Pnm_rgb intTemp = (Pnm_rgb) methods->at(pixmap, col, row);
            printf("INTS R: %i G: %i B:%i\n", intTemp->red, intTemp->green, intTemp->blue);
            rgb_float floatTemp = (rgb_float) methods->at(pixmap, col, row);
            floatTemp->r = ((float) intTemp->red) / denom;
            floatTemp->g = ((float) intTemp->green) / denom;
            floatTemp->b = ((float) intTemp->blue) / denom;
            printf("FLOATS R: %f G: %f B:%f\n", floatTemp->r, floatTemp->g, floatTemp->b);

            //methods->at(floatMap, col, row)->floatTemp;
            //int mySize = sizeof(floatTemp);
            //memcpy(methods->at(floatMap, col, row), floatTemp, mySize);
            // TODO: use Array2_at??
            *(rgb_float*)Array2_at(floatMap, col, row) = floatTemp;
            rgb_float floatChecker = (rgb_float) methods->at(floatMap, col, row);
            printf("CHECK A2--> RED: %f GREEN: %f BLUE: %f\n", floatChecker->r, floatChecker->g, floatChecker->b);

        }
    }

     return floatMap;
}
#undef A2
