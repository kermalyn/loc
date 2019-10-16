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

//x & width are synonymous
T UArray2_new(int width, int height, int size) {
    UArray2_T newArray;
    NEW(newArray);
    //creates the outer array with the size of x
    newArray->outerArray = Array_new(width, sizeof(Array_T * ));

    for (int i=0; i< width; i++){
        //returns a pointer to whats in the index
        Array_T *innerArray = Array_get(newArray->outerArray,i);
        //creates the inner array in that index
        *innerArray = Array_new(height,sizeof(Array_T*));
    }
    //adds other attributes of a UArray2_T
    newArray->height = height;
    newArray->width = width;
    newArray->size = size;
    return newArray;
}

/* Accesses the element in array at position x_i, y_i */
void *UArray2_at(T array, int x_i, int y_i) {
    Array_T *innerArray = Array_get((array)->outerArray, x_i);
    return (Array_get(*innerArray, y_i));
}

void UArray2_map_row_major(T array2, void apply(int width, int height, T array2, int* elems, void *cl), void *cl){
    for(int i = 0; i < array2->width; i++){
        for(int j = 0; j< array2->height; j++){
            Array_T *outer= Array_get(array2->outerArray, i);
            int* elems = Array_get(*outer,j);
            apply(i,j,array2,elems,cl);
        }
    }
}

void UArray2_map_col_major(T array2, void apply(int width, int height, T array2, int* elems, void *cl), void *cl){
    for(int i = 0; i < array2->height; i++){
        for(int j = 0; j< array2->width; j++){
            Array_T outer= Array_get(array2->outerArray, i);
            void* elems = Array_get(outer,j);
            apply(i,j,array2,elems,cl);
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
