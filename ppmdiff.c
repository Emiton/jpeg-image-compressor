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

double compare(Pnm_ppm image1, Pnm_ppm image2);

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

    Pnm_ppm imgA, imgB;
    // TODO: solve methods issue
    A2Methods_T methods = array2_methods_plain->map_row_major; 
    imgA = Pnm_ppmread(fp1, methods);
    imgB = Pnm_ppmread(fp2, methods);
    double difference = compare(imgA, imgB);
    printf("difference: %f", difference);
    fclose(fp1);
    fclose(fp2);
    printf("We did it!\n");
    return 0;
} 


double compare(Pnm_ppm image1, Pnm_ppm image2)
{
    (void) image1;
    (void) image2;
    return 0.0;
}
