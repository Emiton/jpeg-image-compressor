#include "rgbComponentConversions.h"

typedef struct rgb_float {
    float r, g, b;
} *rgb_float;

A2 scaledIntToFloat(A2 pixmap, float denom)
{
    printf("DENOM: %f\n", denom);
    //rgb_float temp;
    //temp->r = ((float) img->red) / denom;
    //temp->g = ((float) img->green) / denom;
    //temp->b = ((float) img->blue) / denom;
    return pixmap;
}   
