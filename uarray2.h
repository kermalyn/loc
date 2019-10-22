#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "mem.h"
#include "assert.h"


#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T

typedef struct T *T;
/* Used return the number oof Columns */
extern int UArray2_Width(T array2);

/* Used to get the number of rows*/
extern int UArray2_Height(T array2);


//takes in the array and calls the UArray
extern int UArray2_size(T array2);

/* Used to initialize a new array given size and width supplied from the main.*/
extern T UArray2_new(int width, int height, int size);


/* Accesses the element in array at position x_i, x_y */
extern void *UArray2_at(T array, int x_i,int x_y);

/* Frees up or deallocates the array that the pointer points to */
extern void UArray2_free(T *array2);

/*uses the apply function to transform or print out each element at each (x,y) position based upon the row */
void UArray2_map_row_major(T array2, void apply(int width, int height, T array2, void* elems, void *cl), void *cl);

/*uses the apply function to transform or print out each element  at each (x,y) position based upon the column */
extern void UArray2_map_col_major(T array2, void apply(int width, int height, T array2, void* elems, void *cl), void *cl);

#undef T
#endif
