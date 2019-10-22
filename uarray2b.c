#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "uarray2b.h"
#include "uarray2.h"
#include "assert.h"
#include "mem.h"
#define T UArray2b_T

struct T{
    int arraySize;
    int arrayWidth;
    int arrayHeight;
    int blockSize;
    UArray2_T arr;
};


/* new blocked 2d array: blocksize = square root of # of cells in block */
T UArray2b_new (int width, int height, int size, int blockSize){
    //create the space for the blocked array
    T blockArray;
    NEW(blockArray);
    //Calculate the block width and block height
    int blockHeight = (int)ceil(height/blockSize);
    int blockWidth = (int)ceil(width/blockSize);

    //create new blocked array
    UArray2_T array= UArray2_new(blockWidth, blockHeight,size);

    blockArray->arrayHeight = height;
    blockArray->arrayWidth = width;
    blockArray->blockSize = blockSize;
    blockArray->arraySize = size;
    blockArray->arr = array;
    return blockArray;
}



/* new blocked 2d array: blocksize as large as possible provided
     block occupies at most 64KB (if possible) */
T UArray2b_new_64K_block(int width, int height, int size){
    int blockSize;
    if(size < 64000){
        blockSize = 64000/size;
    }else{
        blockSize  = 0;
    }
    return  UArray2b_new(width,height,size,blockSize);
}


void UArray2b_free (T *array2b){
    for(int i=0; i<(*array2b)->arrayWidth; i++){
        for(int j=0; j<(*array2b)->arrayHeight; j++){
            UArray2_free(UArray2_at((*array2b)->arr, i, j));
        }
    }
    UArray2_free(&((*array2b)->arr));
    FREE(*array2b);
}

int UArray2b_width (T array2b){return array2b->arrayWidth;}
int UArray2b_height(T array2b){return array2b->arrayHeight;}
int UArray2b_size (T array2b){return array2b->arraySize;}
int UArray2b_blocksize(T array2b){return array2b->blockSize;}

void *UArray2b_at(T array2b, int i, int j){
    UArray2_T *inner_block = UArray2_at(array2b->arr,(i/array2b->blockSize), (j/array2b->blockSize));
    return(UArray2_at((*inner_block),i%(array2b->blockSize), j %(array2b->blockSize)));
}
void UArray2b_map(T array2b, void apply(int i, int j, T array2b, void *elem, void *cl), void *cl){
    int width = ceil(array2b->arrayWidth / array2b->blockSize);
    int height = ceil(array2b->arrayHeight / array2b->blockSize);
    int blockSquare = (array2b->blockSize)*(array2b->blockSize);
    int total = width * height;
    for(int i = 0; i < total; i++){
        for(int j = 0; j < blockSquare; j++){
            int x = i / width + j/array2b->blockSize;
            int y = i % width + j% array2b->blockSize;
            apply(x,y,array2b,UArray2b_at(array2b,x,y),cl);
        }
    }
}
