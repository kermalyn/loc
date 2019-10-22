#include <stdlib.h>
#include <stdio.h>
#include "array.h"
#include "mem.h"
#include "assert.h"
#include "uarray2.h"

#define T UArray2_T

struct T {
    int height;
    int width;
    int size;
    Array_T outerArray;
};
//returns size
int UArray2_size(T array2){
    return array2->size;
}

//returns width
int UArray2_Width(T array2){
    return array2->width;
}
//returns height
int UArray2_Height(T array2){
    return array2->height;
}

T UArray2_new(int width, int height, int size) {
    int i;  /* interates over row number */
    T array;
    NEW(array);
    array->width  = width;
    array->height = height;
    array->size   = size;
    array->outerArray   = Array_new(height, sizeof(Array_T));
    for (i = 0; i < height; i++) {
        Array_T *rowp = Array_get(array->outerArray, i);
        *rowp = Array_new(width, size);
    }
    return array;
}

static inline Array_T row(T a, int j) {
    Array_T *prow = Array_get(a->outerArray, j);
    return *prow;
}

/* Accesses the element in array at position x_i, y_i */
void *UArray2_at(T array2, int i, int j) {
    assert(array2);
    return Array_get(row(array2, j), i);
}


void UArray2_map_col_major(T array2, void apply(int width, int height, T array2, void* elems, void *cl), void *cl){
    for(int i = 0; i < array2->width; i++){
        for(int j = 0; j< array2->height; j++){
            int* elems = UArray2_at(array2,i,j);
            apply(i,j,array2,elems,cl);
        }
    }
}

void UArray2_map_row_major(T array2, void apply(int width, int height, T array2, void* elems, void *cl), void *cl){
    for(int i = 0; i < array2->height; i++){
        for(int j = 0; j< array2->width; j++){
            int* elems = UArray2_at(array2,j,i);
            apply(j,i,array2,elems,cl);
        }
    }
}

void UArray2_free(T *array2){
    assert(array2 != NULL);
    //Loop through the array. For every array at position i delete the array
    for(int i=0; i<(*array2)->width; i++){
        //delete the array pointed to at address i in array2
        Array_free(Array_get((*array2)->outerArray,i));
    }
    //delete array2
    Array_free(&((*array2)->outerArray));
    FREE(*array2);
}
