#ifndef _PACKINGH_
#define _PACKINGH_

#include <dctQuantConversions.h>
#include <array2.h>
#include <bitpack.h>
#include <a2plain.h>
#include <a2methods.h>
#include <assert.h>
#define A2 A2Methods_Array2

extern A2 pack(A2 quantMap);
extern A2 unpack(A2 wordMap);
#undef A2
#endif
