#ifndef ARRAY2B_INCLUDED
#define ARRAY2B_INCLUDED

#define T Array2b_T
typedef struct T *T;

extern T    Array2b_new (int width, int height, int size, int blocksize);
  /* new blocked 2d array: blocksize = square root of # of cells in block */
extern T    Array2b_new_64K_block(int width, int height, int size);
  /* new blocked 2d array: blocksize as large as possible provided
     block occupies at most 64KB (if possible) */

extern void  Array2b_free  (T *array2b);

extern int   Array2b_width (T array2b);
extern int   Array2b_height(T array2b);
extern int   Array2b_size  (T array2b);
extern int   Array2b_blocksize(T array2b);

extern void *Array2b_at(T array2b, int i, int j);
  /* return a pointer to the cell in column i, row j;
     index out of range is a checked run-time error
   */

extern void  Array2b_map(T array2b, 
    void apply(int i, int j, T array2b, void *elem, void *cl), void *cl);
      /* visits every cell in one block before moving to another block */

/* it is a checked run-time error to pass a NULL T
   to any function in this interface */

#undef T
#endif

