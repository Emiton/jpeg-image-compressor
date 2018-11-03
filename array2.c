#include "assert.h"
#include "mem.h"
#include "array.h"
#include "array2.h"

#define T Array2_T

struct T {
  int width, height;
  int size;
  Array_T rows; /* Array_T of 'height' UArray_Ts,
                   each of length 'width' and size 'size' */
   // Element (i, j) in the world of ideas maps to
   //   rows[j][i] where the square brackets stand for access
   //   to a Hanson UArray_T
};

static inline Array_T row(T a, int j) {
  Array_T *prow = Array_get(a->rows, j); 
  return *prow;
}

static int is_ok(T a) {
  return a && Array_length(a->rows) == a->height &&
              Array_size(a->rows) == sizeof(Array_T) &&
              (a->height == 0 || (   Array_length(row(a, 0)) == a->width
                                  && Array_size  (row(a, 0)) == a->size));
}

T Array2_new(int width, int height, int size) {
  int i;  /* interates over row number */
  T array;
  NEW(array);
  array->width  = width;
  array->height = height;
  array->size   = size;
  array->rows   = Array_new(height, sizeof(Array_T));
  for (i = 0; i < height; i++) {
    Array_T *rowp = Array_get(array->rows, i);
    *rowp = Array_new(width, size);
  }
  assert(is_ok(array));
  return array;
}

void Array2_free(T *array2) {
  int i;
  assert(array2 && *array2);
  for (i = 0; i < (*array2)->height; i++) {
    Array_T p = row(*array2, i);
    Array_free(&p);
  }
  Array_free(&(*array2)->rows);
  FREE(*array2);
}

void *Array2_at(T array2, int i, int j) {
  assert(array2);
  return Array_get(row(array2, j), i);
}

int Array2_height(T array2) {
        assert(array2);
        return array2->height;
}
int Array2_width(T array2) {
        assert(array2);
        return array2->width;
}
int Array2_size(T array2) {
        assert(array2);
        return array2->size;
}

void Array2_map_row_major(T array2, 
    void apply(int i, int j, T array2, void *elem, void *cl), void *cl) {
  assert(array2);
  int h = array2->height;  // keeping height and width in registers 
  int w = array2->width;   // avoids extra memory traffic
  for (int j = 0; j < h; j++) {
    Array_T thisrow = row(array2, j); // don't want row/Array_at in inner loop
    for (int i = 0; i < w; i++)
      apply(i, j, array2, Array_get(thisrow, i), cl);
  }
}

void Array2_map_col_major(T array2, 
    void apply(int i, int j, T array2, void *elem, void *cl), void *cl) {
  assert(array2);
  int h = array2->height;  // keeping height and width in registers 
  int w = array2->width;   // avoids extra memory traffic
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      apply(i, j, array2, Array_get(row(array2, j), i), cl);
}
