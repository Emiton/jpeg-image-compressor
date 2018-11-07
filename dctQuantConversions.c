/*
 * Filename: dctQuantConversions 
 *
 * Description: This function takes in a 2D array of component video float
 *              structs and reduces the information held by these float structs
 *              by applying the discrete cosine transform and quantizing the Pb
 *              and Pr values of 2 x 2 blocks. The inverse of these operations
 *              are also included within this file.
*/

#include <>

#define A2 A2Methods_Array2

typedef struct temp {
    float a,b,c,d;
    float avgPb, avgPr;
}  *temp;

extern A2 reduce(A2 componentArray, int height, int width)
{
    assert(componentArray);
    assert(height > 0);
    assert(width > 0);
    discreteCosineTransform(componentArray, height, width);
}


A2 discreteCosineTransform(A2 componentArray, int height, int width)
{

}






















#undef A2
