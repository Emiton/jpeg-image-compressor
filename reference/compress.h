#include <stdio.h>

/* The two functions below are functions you should implement.
   They should take their input from the parameter and should
   write their output to stdout */
extern void compress(FILE *input); // reads PPM and writes a compressed image
extern void decompress(FILE *input); // reads a compressed image and writes PPM
