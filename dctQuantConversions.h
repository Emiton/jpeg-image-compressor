#include <assert.h>
#include <a2plain.h>
#include <a2methods.h>
#include <array2.h>
#include "arith.h"
#include "rgbComponentConversions.h"
#include <stdio.h>
#include <stdlib.h>

#define A2 A2Methods_Array2

typedef struct quantizedValues {
    unsigned a;
    signed b,c,d;
    unsigned avgPb, avgPr;
}  *quantizedValues;
  
typedef struct reducedValues {
    float a,b,c,d;
    float avgPb, avgPr;
}  reducedValues;

extern A2 reduce(A2 componentArray, int height, int width);
float discreteCosineTransform(char coefficient, float y1, float y2, float y3, float y4);
unsigned quantizeColorDifference(float chroma);
extern A2 expand(A2 quantArray);
#undef A2
