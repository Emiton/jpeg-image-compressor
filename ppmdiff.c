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
int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    // either - filename
    //         filename filename
    //         filename -
    // BUT NOT - -
    //

    if(!strcmp(argv[1], "-"))
    {
        fp1 = fopen(argv[1], "r");
        
        if(!strcmp(argv[2], "-"))
        {
            fp2 = fopen(argv[2], "r");
        }

        else
        {
            fp2 = stdin;
        }
    }

    else
    {
        fp1 = stdin;

        fp2 = fopen(argv[2], "r");

    }

    if(fp1 == NULL)
    {
        printf("File 1 Error\n");

    }

    if(fp2 == NULL)
    {
        printf("File 2 Error\n");
    }
}

