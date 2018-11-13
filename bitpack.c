#include "bitpack.h"
#include <assert.h> // TODO: quotes or angle brackets??
#include <stdio.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);

    if(width == 0)
    {
        return false;
    }
    
    if ((uint64_t)(1 << width) - 1 < n)
    {
        return false;
    }

    return true;
}

bool Bitpack_fitss( int64_t n, unsigned width)
{
    if (width <= 1)
    {
        return false;
    }

    uint64_t n_signed = (uint64_t)n;
        if (n < 0)
                n_signed = ~n;
    width--;
    return Bitpack_fitsu(n_signed, width);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
    unsigned shift = 64 - (width + lsb);
    assert(shift <= 64);
    if(shift == 64)
    {
        word = 0;
    }
    else
    {
        word = word << shift;
    }
    shift = 64 - width;
    assert(shift <= 64);
    if(shift == 64)
    {
        word = 0;
    }
    else
    {
    word = word >> shift;
    }
    return word;

}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    unsigned shift = 64 - (width + lsb);
    assert(shift <= 64);
    if(shift == 64)
    {
        word = 0;
    }
    else
    {
        word = word << shift; // TODO: uint64_t vs int64_t
    }
    shift = 64 - width;
    assert(shift <= 64);
    if(shift == 64)
    {
        word = 0;
    }
    else
    {
        word = word >> shift; // TODO: uint64_t vs int64_t
    }
    return word;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
    if(!Bitpack_fitsu(value, width))
    {
        RAISE(Bitpack_Overflow);
    }

    // Set all bits to 1
    uint64_t maxValue = ~0;
    uint64_t leftShift;

    unsigned shift = 64 - width;
    assert(shift <= 64);
    if (shift == 64)
    {
        leftShift = 0;
    }
    else
    {
        leftShift = maxValue << shift; 
    }

    uint64_t rightShift;
    shift = 64 - (width + lsb);
    assert(shift <= 64);
    if (shift == 64)
    {
        rightShift = 0;
    }
    else
    {
        rightShift = leftShift >> shift;
    }
    uint64_t mask = ~rightShift;
    unsigned vShift = lsb;
    uint64_t newVal;
    assert(vShift <= 64);
    if (vShift == 64)
    {
        newVal = 0;
    }
    else
    {
        newVal = value << vShift;
    }
    return (word & mask) | newVal;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
    if(!Bitpack_fitss(value, width))
    {
        RAISE(Bitpack_Overflow);
    }

     // Set all bits to 1
    uint64_t maxValue = ~0;
    uint64_t leftShift;

    unsigned shift = 64 - width;
    assert(shift <= 64);
    if (shift == 64)
    {
        leftShift = 0;
    }
    else
    {
        leftShift = maxValue << shift; 
    }

    uint64_t rightShift;
    shift = 64 - (width + lsb);
    assert(shift <= 64);
    if (shift == 64)
    {
        rightShift = 0;
    }
    else
    {
        rightShift = leftShift >> shift;
    }
    uint64_t mask = ~rightShift;

    unsigned vShift = 64 - width;
    uint64_t newVal;
    assert(vShift <= 64);
    if (vShift == 64)
    {
        newVal = 0;
    }
    else
    {
        newVal = value << vShift;
    }

    vShift = 64 - (width + lsb);
    assert(vShift <= 64);
    if (vShift == 64)
    {
        newVal = 0;
    }
    else
    {
        newVal = newVal >> vShift;
    }

    return (word && mask) | newVal;
}
