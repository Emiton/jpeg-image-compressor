/*
 * Filename: ppmdiff
 * Authors: Emiton Alves and Cameron LaFreniere
 * Creation Date: 10-22-18
*/


// get image input
    // one image can come from directory
    // another image can come from stdin
    // read image 1 into I 
    // read image 2 into I'
// check difference in height
    // if diff > 1, return error -> print 1.0 to stdout
// check difference in width
    // if difference > 1 return error -> print 1.0 to stdout

// Compute root mean square difference (E)
// Check if images use same scale for RGB values
    // loop through each pixel 
    // get difference between RGB values
    // square differences
    // add the squares
    // add this new sum to the total value
// Divide total by 3 x w x h
// Square the current value
// Print E to stdout with 4 digits after decimal
//
//
//
#include <string.h>
#include <pnm.h>
#include <stdio.h>
#include <a2plain.h>
#include <a2methods.h>
#include <assert.h>
#include <stdlib.h>

double compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods);
int get_min(int a, int b);

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;

    if(!strcmp(argv[1], "-"))
    {
        fp1 = stdin;
        fp2 = fopen(argv[2], "r");
    }
    else
    {
        fp1 = fopen(argv[1], "r");

        if(!strcmp(argv[2], "-"))
        {
            fp2 = stdin;
        }
        else 
        {
            fp2 = fopen(argv[2], "r");
        }
    }

    if(fp1 == NULL || fp2 == NULL)
    {
        // TODO: remove argc 
        fprintf(stderr, "Could not open files %i\n", argc);
        return 0;
    }
    // TODO: replace if with these asserts
    assert(fp1 != NULL);
    assert(fp2 != NULL);

    Pnm_ppm imgA, imgB;
    A2Methods_T methods = array2_methods_plain; // default to Array2 methods
    assert(methods);
    printf("ABOUT TO READ\n"); 
    imgA = Pnm_ppmread(fp1, methods);
    imgB = Pnm_ppmread(fp2, methods);
    printf("YOU READ SOMETHING!\n");
    double difference = compare(imgA, imgB, methods);
    printf("difference: %f\n", difference);
    fclose(fp1);
    fclose(fp2);
    printf("We did it!\n");
    return 0;
} 


double compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods)
{ 
    int w1 = image1->width;
    int w2 = image2->width;
    int h1 = image1->height;
    int h2 = image2->height;
    int r, g, b;
    if(abs(w1 - w2) > 1 || abs(h1 - h2) > 1)
    {
        fprintf(stderr, "File difference too great\n");
        exit (EXIT_FAILURE);
    }

   int width = get_min(w1, w2);
   int height = get_min(h1, h2);
   A2Methods_Array2 pixels = image1->pixels;
    // Accessing pixels
    for(int row = 0; row < height ; row++ )
    {
        for(int col = 0; col < width; col++)
        {
            Pnm_rgb temp = (Pnm_rgb)methods->at(pixels, col, row);
            r = temp->red;
            g = temp->green;
            b = temp->blue;
            printf("RED: %i BLUE:%i GREEN:%i\n",r,g,b);
        }
    }

    return w1;
}

int get_min(int a, int b)
{
    int min;
    if(a - b <= 0)
    {
        min = a;
    }
    else if(a - b > 0)
    {
        min = b;
    }

    return min;
}
