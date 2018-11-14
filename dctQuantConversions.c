/*
 * Filename: dctQuantConversions 
 *
 * Description: This function takes in a 2D array of component video float
 *              structs and reduces the information held by these float structs
 *              by applying the discrete cosine transform and quantizing the Pb
 *              and Pr values of 2 x 2 blocks. The inverse of these operations
 *              are also included within this file.
*/
#include "dctQuantConversions.h"
#include <math.h>
#define A2 A2Methods_Array2

// TODO: pass methods to all helper functions from compress
//
// TODO: rename this struct and place in header file
// TODO: what types are a, b, c, and d? a needs to be 9 bits, the rest: 5 bits

float discreteCosineTransform(char coefficient, float y1, float y2, float y3, float y4);
unsigned quantizeColorDifference(float chroma);
unsigned quantizeAcoefficient(float coeff);
signed quantizeBCDcoefficients(float coeff);
float dequantizeColorDifference(unsigned index);
float dequantizeAcoefficient(unsigned a);
float dequantizeBCDcoefficient(signed coeff);
float inverseDCT(int y, float a, float b, float c, float d);
extern A2 expand(A2 quantArray);

extern A2 reduce(A2 componentArray, int height, int width)
{
    assert(componentArray);
    assert(height > 0);
    assert(width > 0);
    
    int blockSize = 2;
    A2Methods_T methods = array2_methods_plain;
    A2 quantMap = methods->new(((methods->width(componentArray)) / blockSize),
                               ((methods->height(componentArray)) / blockSize),
                               sizeof(struct quantizedValues));
    int count = 0; 
    for(int row = 0; row < height; row+=2)
    {
        for(int col = 0; col < width; col+=2)
        {
            ybr_float ybrTemp1 = (ybr_float) methods->at(componentArray, col, row);
            ybr_float ybrTemp2 = (ybr_float) methods->at(componentArray, col + 1, row);
            ybr_float ybrTemp3 = (ybr_float) methods->at(componentArray, col, row + 1);
            ybr_float ybrTemp4 = (ybr_float) methods->at(componentArray, col + 1, row + 1 );
            
            reducedValues reducedTemp;
            reducedTemp.avgPb = (ybrTemp1->Pb + ybrTemp2->Pb + ybrTemp3->Pb + ybrTemp4->Pb) / 4.0;
            reducedTemp.avgPr = (ybrTemp1->Pr + ybrTemp2->Pr + ybrTemp3->Pr + ybrTemp4->Pr) / 4.0;
            float y1 = ybrTemp1->y;
            float y2 = ybrTemp2->y;
            float y3 = ybrTemp3->y;
            float y4 = ybrTemp4->y;
            
            reducedTemp.a = discreteCosineTransform('a', y1, y2, y3, y4);
            reducedTemp.b = discreteCosineTransform('b', y1, y2, y3, y4);
            reducedTemp.c = discreteCosineTransform('c', y1, y2, y3, y4);
            reducedTemp.d = discreteCosineTransform('d', y1, y2, y3, y4);
 
            quantizedValues quantTemp = (quantizedValues) methods->at(quantMap,(col / blockSize),(row / blockSize));
 
            quantTemp->avgPb = quantizeColorDifference(reducedTemp.avgPb);
            quantTemp->avgPr = quantizeColorDifference(reducedTemp.avgPr);
            quantTemp->a = quantizeAcoefficient(reducedTemp.a);
            quantTemp->b = quantizeBCDcoefficients(reducedTemp.b);
            quantTemp->c = quantizeBCDcoefficients(reducedTemp.c);
            quantTemp->d = quantizeBCDcoefficients(reducedTemp.d);
            //if(count++ < -1)
            //{
                //printf("NOTPASSEDIN COMPRESS QUANTTEMP VALUES  [%i, %i] -- A: %u, B: %i, C: %i, D: %i, avgPb: %u, avgPr: %u\n", col, row, quantTemp->a, quantTemp->b, quantTemp->c, quantTemp->d, quantTemp->avgPr, quantTemp->avgPr);
            //}
        }
    }
    
    return quantMap;
}

float discreteCosineTransform(char coefficient, float y1, float y2, float y3, float y4)
{
    float computedCoefficient;
    switch(coefficient) 
    {
        case 'a': 
            computedCoefficient = (y4 + y3 + y2 + y1) / 4.0; 
           // printf("computed Coefficient A:%f\n", computedCoefficient);
            break;
        case 'b': 
            computedCoefficient = (y4 + y3 - y2 - y1) / 4.0;
           // printf("computed Coefficient B:%f\n", computedCoefficient);
            break;
        case 'c': 
            computedCoefficient = (y4 - y3 + y2 - y1) / 4.0;
           // printf("computed Coefficient C:%f\n", computedCoefficient);
            break;
        case 'd': 
            computedCoefficient = (y4 - y3 - y2 + y1) / 4.0;
           // printf("computed Coefficient D:%f\n", computedCoefficient);
            break;
        default:
            fprintf(stderr, "Incorrect Coefficient\n");
            exit(EXIT_FAILURE);
            break;
    }

    return computedCoefficient;
}

unsigned quantizeColorDifference(float chroma)
{
    if(chroma > 0.5)
    {
        chroma = 0.5;
    }
    else if(chroma < -0.5)
    {
        chroma = -0.5;
    }

    return Arith_index_of_chroma(chroma); 
}

unsigned quantizeAcoefficient(float coeff)
{
    // 2^9 - 1 = 511, based on number of bits used to hold 'a'
    return roundf(coeff * 511.0);
}

signed quantizeBCDcoefficients(float coeff)
{
   if(coeff > 0.3)
   {
        coeff = 0.3;
   }
   else if (coeff < -0.3)
   {
       coeff = -0.3;
   }
   
   return roundf(coeff * 50.0);
}

extern A2 expand(A2 quantArray)
{
    assert(quantArray);
    int height = Array2_height(quantArray);
    int width = Array2_width(quantArray);
    A2Methods_T methods = array2_methods_plain;
    A2 YBRmap = methods->new((width * 2) , (height * 2), sizeof(struct ybr_float));
    
    for(int row = 0; row < height; row ++)
    {
        for(int col = 0; col < width; col ++)
        {
            quantizedValues incomingValues = methods->at(quantArray, col, row);
            reducedValues reducedTemp;

            reducedTemp.a = dequantizeAcoefficient(incomingValues->a);
            reducedTemp.b = dequantizeBCDcoefficient(incomingValues->b);
            reducedTemp.c = dequantizeBCDcoefficient(incomingValues->c);
            reducedTemp.d = dequantizeBCDcoefficient(incomingValues->d);
            reducedTemp.avgPb = dequantizeColorDifference(incomingValues->avgPb);
            reducedTemp.avgPr = dequantizeColorDifference(incomingValues->avgPr);
            
            ybr_float ybrTemp1 = (ybr_float) methods->at(YBRmap, (col * 2), (row * 2));
            ybr_float ybrTemp2 = (ybr_float) methods->at(YBRmap, (1 + col * 2), (row * 2));
            ybr_float ybrTemp3 = (ybr_float) methods->at(YBRmap, (col * 2), (1 + row * 2));
            ybr_float ybrTemp4 = (ybr_float) methods->at(YBRmap, (1 + col * 2), (1 + row * 2));

            ybrTemp1->y = inverseDCT(1, reducedTemp.a,
                                        reducedTemp.b,
                                        reducedTemp.c,
                                        reducedTemp.d);
            
            ybrTemp2->y = inverseDCT(2, reducedTemp.a,
                                        reducedTemp.b,
                                        reducedTemp.c,
                                        reducedTemp.d);

            ybrTemp3->y = inverseDCT(3, reducedTemp.a,
                                        reducedTemp.b,
                                        reducedTemp.c,
                                        reducedTemp.d);

            ybrTemp4->y = inverseDCT(4, reducedTemp.a,
                                        reducedTemp.b,
                                        reducedTemp.c,
                                        reducedTemp.d);

            ybrTemp1->Pb = reducedTemp.avgPb;
            ybrTemp1->Pr = reducedTemp.avgPr;

            ybrTemp2->Pb = reducedTemp.avgPb;
            ybrTemp2->Pr = reducedTemp.avgPr;

            ybrTemp3->Pb = reducedTemp.avgPb;
            ybrTemp3->Pr = reducedTemp.avgPr;

            ybrTemp4->Pb = reducedTemp.avgPb;
            ybrTemp4->Pr = reducedTemp.avgPr;
        }
    }

    return YBRmap;
}


float dequantizeColorDifference(unsigned index)
{
    return Arith_chroma_of_index(index);
}

float dequantizeAcoefficient(unsigned a)
{
    return (a / 511.0);
}

float dequantizeBCDcoefficient(signed coeff)
{
    return (coeff / 50.0);
}

float inverseDCT(int y, float a, float b, float c, float d)
{
    float luminanceValue;
    switch(y) 
    {
        case 1: 
            luminanceValue = a - b - c + d;
            break;
        case 2: 
            luminanceValue = a - b + c - d;
            break;
        case 3: 
            luminanceValue = a + b - c - d;
            break;
        case 4: 
            luminanceValue = a + b + c + d;
            break;
        default:
            fprintf(stderr, "Incorrect Y value chosen\n");
            exit(EXIT_FAILURE);
            break;
    }

    return luminanceValue;
}

#undef A2

