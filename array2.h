#ifndef ARRAY2_INCLUDED
#define ARRAY2_INCLUDED
#define T Array2_T
typedef struct T *T;

typedef void Array2_applyfun(int i, int j, T array2, void *elem, void *cl);
typedef void Array2_mapfun(T array2, Array2_applyfun apply, void *cl);

extern T     Array2_new   (int width, int height, int size);
extern void  Array2_free  (T *array2);
extern int   Array2_width (T array2);
extern int   Array2_height(T array2);
extern int   Array2_size  (T array2);
extern void *Array2_at    (T array2, int i, int j);
extern void  Array2_map_row_major(T array2, Array2_applyfun apply, void *cl);
extern void  Array2_map_col_major(T array2, Array2_applyfun apply, void *cl);
#undef T
#endif
