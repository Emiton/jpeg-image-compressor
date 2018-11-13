#include <dctQuantConversions.h>
#include <array2.h>
#include <bitpack.h>
#include <a2plain.h>
#include <a2methods.h>
#include <assert.h>
#include <inttypes.h>
#include "packing.h"

#define A2 A2Methods_Array2
extern A2 pack(A2 quantMap);
extern A2 unpack(A2 wordMap);

extern A2 pack(A2 quantMap)
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
             
            word = Bitpack_newu(word, 9, 23, a);
            word = Bitpack_news(word, 5, 18, b);
            word = Bitpack_news(word, 5, 13, c);
            word = Bitpack_news(word, 5, 8, d);
            word = Bitpack_newu(word, 4, 4, avgPb);
            word = Bitpack_newu(word, 4, 0, avgPr);
            //printf("%" PRIu64 "\n", word);
        }
    }
    return wordMap;    
            
}


extern A2 unpack(A2 wordMap)
{

    assert(wordMap);
    A2Methods_T methods = array2_methods_plain;
    
    int height = methods->height(wordMap);
    int width = methods->width(wordMap);
    A2 quantMap = methods->new(width, height, sizeof(quantizedValues));

    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width; col++)
        {
            uint64_t word = (uint64_t) methods->at(wordMap, col, row);
           
            quantizedValues quantTemp = (quantizedValues) methods->at(quantMap, col, row);

            quantTemp->a = Bitpack_getu(word, 9, 23);
            quantTemp->b = Bitpack_gets(word, 5, 18);
            quantTemp->c = Bitpack_gets(word, 5, 13);
            quantTemp->d = Bitpack_gets(word, 5, 8);
            quantTemp->avgPb = Bitpack_getu(word, 4, 4);
            quantTemp->avgPr = Bitpack_getu(word, 4, 0); 
        }
    }        

    return quantMap;
            
}

#undef A2


