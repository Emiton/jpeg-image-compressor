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

#define A2 A2Methods_Array2

// TODO: pass methods to all helper functions from compress
//
// TODO: rename this struct and place in header file
// TODO: what types are a, b, c, and d? a needs to be 9 bits, the rest: 5 bits

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
    
    for(int row = 0; row < height; row+=2)
    {
        for(int col = 0; col < width; col+=2)
        {
            quantizedValues quantTemp = (quantizedValues) methods->at(quantMap, col / blockSize, row / blockSize);
            ybr_float ybrTemp1 = (ybr_float) methods->at(componentArray, col, row);
            ybr_float ybrTemp2 = (ybr_float) methods->at(componentArray, col + 1, row);
            ybr_float ybrTemp3 = (ybr_float) methods->at(componentArray, col, row + 1);
            ybr_float ybrTemp4 = (ybr_float) methods->at(componentArray, col + 1, row + 1 );
            
            reducedValues reducedTemp;//TODO = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
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

            quantTemp->avgPb = quantizeColorDifference(reducedTemp.avgPb);
            quantTemp->avgPr = quantizeColorDifference(reducedTemp.avgPr);
            
            // TODO: create int array with values [-15 to 15] 
            //      multiply b, c, and d each by 50
            //      see which value within the array that it comes closest to
            //      TODO: // store the index value closest to b, c, and d OR actual value
            //      convert this integer to a signed binary value
            //      store in quantTemp as a 5 bit signed binary value
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
            break;
        case 'b': 
            computedCoefficient = (y4 + y3 - y2 - y1) / 4.0;
            break;
        case 'c': 
            computedCoefficient = (y4 - y3 + y2 - y1) / 4.0;
            break;
        case 'd': 
            computedCoefficient = (y4 - y3 - y2 + y1) / 4.0;
            break;
        default:
            fprintf(stderr, "Incorrect Coefficient\n");
            computedCoefficient = -1.0;
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

#undef A2
