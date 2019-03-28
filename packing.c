#include <dctQuantConversions.h>
#include <array2.h>
#include <bitpack.h>
#include <a2plain.h>
#include <a2methods.h>
#include <assert.h>
#include <inttypes.h>
#include "packing.h"

#define A2 A2Methods_Array2
A2 pack(A2 quantMap);
extern A2 unpack(A2 wordMap);
extern A2 getProcessedWord(FILE *input, unsigned height, unsigned width);
extern void putWord(A2 quantMap, int height, int width);
void getCompressedWords(int i, int j, A2 pixels, A2Methods_Object *ptr, void *cl);

A2 pack(A2 quantMap)
{

    assert(quantMap);
    A2Methods_T methods = array2_methods_plain;
    
    int height = methods->height(quantMap);
    int width = methods->width(quantMap);
    A2 wordMap = methods->new(width, height, sizeof(uint64_t));
    
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width; col++)
        {
            quantizedValues quantTemp = (quantizedValues) methods->at(quantMap, col, row);
            uint64_t word = (uint64_t) methods->at(wordMap, col, row);

            unsigned a = quantTemp->a;
            signed b = quantTemp->b;
            signed c = quantTemp->c;
            signed d = quantTemp->d;
            unsigned avgPb = quantTemp->avgPb;
            unsigned avgPr = quantTemp->avgPr;
/*            count++;
            if(count < 6)
            {
                printf("COMPRESS QUANTTEMP VALUES [%i, %i] -- A: %u, B: %i, C: %i, D: %i, avgPb: %u, avgPr: %u\n", col, row, a, b, c, d, avgPr, avgPr);
            }
            */
            word = Bitpack_newu(word, 9, 23, a);
            word = Bitpack_news(word, 5, 18, b);
            word = Bitpack_news(word, 5, 13, c);
            word = Bitpack_news(word, 5, 8, d);
            word = Bitpack_newu(word, 4, 4, avgPb);
            word = Bitpack_newu(word, 4, 0, avgPr);

        }
    }
    return wordMap;    
            
}

extern void putWord(A2 quantMap, int height, int width)
{
    A2Methods_T methods = array2_methods_plain;
    A2 processedWords = pack(quantMap);
    int h = methods->height(processedWords);
    int w = methods->width(processedWords);
    printf("Compressed image format 2\n%u %u\n", width, height);
    for(int row = 0; row < h; row++)
    {
        for(int col = 0; col < w; col++)
        {
            uint64_t *word = Array2_at(processedWords, col, row);
            putchar(Bitpack_getu(*word, 8, 24));
            putchar(Bitpack_getu(*word, 8, 16));
            putchar(Bitpack_getu(*word, 8, 8));
            putchar(Bitpack_getu(*word, 8, 0));
        }
    }
}

/*
extern A2 unpack2(A2 wordMap)
{

    assert(wordMap);
    A2Methods_T methods = array2_methods_plain;
    
    int height = methods->height(wordMap);
    int width = methods->width(wordMap);
    printf("HEIGHT: %i WIDTH: %i\n", height, width);
    // TODO: Width and Height multiplied by 2, but this is just a workaround
    // TODO: Remove it later?
    A2 quantMap = methods->new(width, height, sizeof(dequantizedValues));
    //printf("QuantMap width: %i\n", Array2_width(quantMap));
    //int count = 0;
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width; col++)
        {
            uint64_t word = (uint64_t) methods->at(wordMap, col, row);
            // TODO: Print word
            // TESTING
            dequantizedValues quantTemp = (dequantizedValues) methods->at(quantMap, col, row);
            
            //unsigned arraySize = sizeof(quantMap);
            //unsigned structSize = sizeof(quantTemp);
            //unsigned aSize = sizeof(quantTemp->a);
            //unsigned bSize = sizeof(quantTemp->b);
            //printf("Array Size: %u Struct Size: %u A Size: %u B: Size: %u\n", arraySize, structSize, aSize, bSize);
            //BIG ENDIAN
            quantTemp->a = (unsigned) Bitpack_getu(word, 9, 23); //returns a uint64
            quantTemp->b = (signed) Bitpack_gets(word, 5, 18); //returns a signed int64
            quantTemp->c = (signed) Bitpack_gets(word, 5, 13); //returns a signed int64
            quantTemp->d = (signed) Bitpack_gets(word, 5, 8); // returns a signed int64
            quantTemp->avgPb = (unsigned) Bitpack_getu(word, 4, 4);// returns a uint64
            quantTemp->avgPr = (unsigned) Bitpack_getu(word, 4, 0); // returns a uint64
            //count++;
            //printf("Current Count: %i\n", count);

            //count++;
            //if(count < 6){
              //  printf("DECOMPRESS QUANTTEMP VALUES [%i, %i] -- A: %u, B: %i, C: %i, D: %i, AvgPb: %u, AvgPr: %u\n", col, row, quantTemp->a, quantTemp->b, quantTemp->c, quantTemp->d, quantTemp->avgPb, quantTemp->avgPr);
           // } 
            
       }    
    }        
       printf("DONE UNPACKING\n");
    return quantMap;
            
}
*/

// First stage in decompress, takes file input. Creates A2 wordMap
extern A2 getProcessedWord(FILE *input, unsigned height, unsigned width)
{
    assert(input);
    assert(height > 0);
    assert(width > 0);
    A2Methods_T methods = array2_methods_plain;
    unsigned reducedHeight = height / 2;
    unsigned reducedWidth = width / 2; 
    A2 fileMap = methods->new(reducedWidth, reducedHeight, sizeof(uint64_t)); 
    for(unsigned row = 0; row < reducedHeight; row++)
    {
        for(unsigned col = 0; col < reducedWidth; col++)
        {
            uint64_t cell = (uint64_t) methods->at(fileMap, col, row);
            cell = Bitpack_newu(cell, 8, 24, getc(input));
            cell = Bitpack_newu(cell, 8,16, getc(input));
            cell = Bitpack_newu(cell, 8, 8, getc(input));
            cell = Bitpack_newu(cell, 8, 0, getc(input));
        }
    }
   return fileMap; 
}


extern A2 unpack(A2 wordMap)
{
    int height = Array2_height(wordMap);
    int width = Array2_width(wordMap);
    A2Methods_T methods = array2_methods_plain;
    A2 fileMap = methods->new(height, width, sizeof(struct dequantizedValues));
    methods->map_row_major(fileMap, &getCompressedWords, wordMap);
    return fileMap;
}

void getCompressedWords(int i, int j, A2 pixels, A2Methods_Object *ptr, void *cl)
{
    (void)pixels;
    A2Methods_T methods = array2_methods_plain;
    A2 input = (A2*)cl; // struct A2*
    uint64_t word = (uint64_t) ptr;
    dequantizedValues dv = (dequantizedValues)methods->at(input, i, j);
    dv->a = Bitpack_getu(word, 9, 23);
    dv->b = Bitpack_gets(word, 5, 18);
    dv->c = Bitpack_gets(word, 5, 13);
    dv->d = Bitpack_gets(word, 5, 8);
    dv->avgPb = Bitpack_getu(word, 4, 4);
    dv->avgPr = Bitpack_getu(word, 4, 0); 
}

#undef A2
