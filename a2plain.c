#include <stdlib.h>

#include <a2plain.h>
#include "array2.h"

// define a private version of each function in A2Methods_T that we implement

static A2Methods_Array2 new(int width, int height, int size) {
  return Array2_new(width, height, size);
}

static A2Methods_Array2 new_with_blocksize(int width, int height, int size,
                                        int blocksize)
{
  (void) blocksize;
  return Array2_new(width, height, size);
}

// ... many more private (static) definitions follow ...
static void a2free (A2Methods_Array2 *array2p) {
  Array2_free((Array2_T *)array2p);
}

static int width    (A2Methods_Array2 array2) { return Array2_width (array2); }
static int height   (A2Methods_Array2 array2) { return Array2_height(array2); }
static int size     (A2Methods_Array2 array2) { return Array2_size  (array2); }
static int blocksize(A2Methods_Array2 array2) { (void)array2; return 1; }
static A2Methods_Object *at(A2Methods_Array2 array2, int i, int j) {
  return Array2_at(array2, i, j);
}
   
static void map_row_major  (A2Methods_Array2 array2, A2Methods_applyfun apply, void *cl) {
  Array2_map_row_major(array2, (Array2_applyfun*)apply, cl);
}
static void map_col_major  (A2Methods_Array2 array2, A2Methods_applyfun apply, void *cl) {
  Array2_map_col_major(array2, (Array2_applyfun*)apply, cl);
}

// struct small_closure {
//   A2Methods_smallapplyfun *apply; 
//   void *cl;
// };

// static void apply_small(int i, int j, UArray2_T uarray2, void *elem, void *vcl) {
//   struct small_closure *cl = vcl;
//   (void)i;
//   (void)j;
//   (void)uarray2;
//   cl->apply(elem, cl->cl);
// }

// static void small_map_row_major(A2Methods_UArray2 a2, A2Methods_smallapplyfun apply, void *cl) {
//   struct small_closure mycl = { apply, cl };
//   UArray2_map_row_major(a2, apply_small, &mycl);
// }

// static void small_map_col_major(A2Methods_UArray2 a2, A2Methods_smallapplyfun apply, void *cl) {
//   struct small_closure mycl = { apply, cl };
//   UArray2_map_col_major(a2, apply_small, &mycl);
// }

// now create the private struct containing pointers to the functions

static struct A2Methods_T array2_methods_plain_struct = {
  new,
  new_with_blocksize,
//   ... other functions follow in order, with NULL for those not implemented ...
  a2free,
  width,
  height,
  size,
  blocksize,
  at,
  map_row_major,
  map_col_major,
  NULL,
  map_row_major
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T array2_methods_plain = &array2_methods_plain_struct;
