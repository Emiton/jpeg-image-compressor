/*
 * Filename: ppmdiff
 * Authors: Emiton Alves and Cameron LaFreniere
 * Creation Date: 10-22-18
 *
 * Description: Compares two images of similar size and computes the difference
 *              using the root mean square difference
*/

#include <a2methods.h>
#include <a2plain.h>
#include <assert.h>
#include <math.h>
#include <pnm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods);
int get_min(int a, int b);
double square(double num);

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    (void) argc;

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
        fprintf(stderr, "Could not open files\n");
        exit (EXIT_FAILURE);
    }
    
    Pnm_ppm imgA, imgB;
    A2Methods_T methods = array2_methods_plain; // default to Array2 methods
    assert(methods); 
    imgA = Pnm_ppmread(fp1, methods);
    fclose(fp1);
    imgB = Pnm_ppmread(fp2, methods);
    fclose(fp2);
    double difference = compare(imgA, imgB, methods);
    printf("difference: %.4lf\n", difference); 
    return 0;
} 

// Compares the difference in RGB values between 2 photos of similar size
// Uses Root Mean Square to compute difference
double compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods)
{ 
    int w1 = image1->width;
    int w2 = image2->width;
    int h1 = image1->height;
    int h2 = image2->height;
    
    // If image width or height diff is greater than 1, terminate program
    if(abs(w1 - w2) > 1 || abs(h1 - h2) > 1)
    {
        fprintf(stderr, "File difference too great\n");
        exit (EXIT_FAILURE);
    }
   double r1, g1, b1, r2, b2, g2;
   // smaller width and height usedto compare images
   int width = get_min(w1, w2);
   int height = get_min(h1, h2);
   A2Methods_Array2 pixmap1 = image1->pixels;
   A2Methods_Array2 pixmap2 = image2->pixels;
   
   double sum = 0;
    // Get the difference in rgb values between corresponding pixels in both photos 
    for(int row = 0; row < height ; row++ )
    {
        for(int col = 0; col < width; col++)
        {
            Pnm_rgb temp1 = (Pnm_rgb)methods->at(pixmap1, col, row);
            Pnm_rgb temp2 = (Pnm_rgb)methods->at(pixmap2, col, row);

            r1 = (double)temp1->red;
            g1 = (double)temp1->green;
            b1 = (double)temp1->blue;

            r2 = (double)temp2->red;
            g2 = (double)temp2->green;
            b2 = (double)temp2->blue;

            double rdiff = square(r1-r2);
            double gdiff = square(g1-g2);
            double bdiff = square(b1-b2);
            
            sum = sum + rdiff + gdiff + bdiff; 
        }
    }

    double total = sum / (3 * width * height);
    total = sqrt(total);
    return total;
}

// Returns the minimum value between two integers
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

// Squares a floating point value
double square(double num)
{
    return (num*num);
}
