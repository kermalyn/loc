#include <stdlib.h>
#include <stdio.h>


#include "math.h"
#include "uarray2b.h"
#include "uarray2.h"



#define T Array2b_T

typedef struct UArray2b_T{
    int arraySize;
    int arrayWidth;
    int arrayHeight;
    int blocksize;
    Uarray2_T arr;
};

/* new blocked 2d array: blocksize = square root of # of cells in block */
extern T UArray2b_new (int width, int height, int size, int blocksize){
    //create the space for the blocked array
    Uarray2b_T blockArray= malloc(sizeof(*blockArray));
    // initialize its variables
    blockArray->arrayHeight = height;
    blockArray->arrayWidth = width;
    blockArray->arrayBlocksize = blocksize;
    blockArray->arraySize = size;

    //Calculate the block width and block height
    int blockHeight = ceiling(height/blocksize);
    int blockWidth = ceiling( width/blocksize);

    //create new blocked array
    blockArray= Uarray2_new(blockWidth, blockHeight, arraySize,blocksize);

}

/* new blocked 2d array: blocksize as large as possible provided
     block occupies at most 64KB (if possible) */
extern T UArray2b_new_64K_block(int width, int height, int size)

extern void UArray2b_free (T *array2b){


}

extern int UArray2b_width (T array2b){
    return array2b->width;
}


extern int UArray2b_height(T array2b){
    return array2b->height;
}


extern int UArray2b_size (T array2b){
    return array2b->size;
}


extern int UArray2b_blocksize(T array2b){
    return array2b->blocksize;
}

/* return a pointer to the cell in column i, row j;
     index out of range is a checked run-time error
   */
extern void *UArray2b_at(T array2b, int i, int j){
    if(array2b->blocksize ==1){
        return(UArray2_at(array2b->arr, i, j));
    }

    //get the block_height and block_width i.e the number of rows in blocks and columns n blocks
    blkarray_W = i/(array2b->blockside);
    blkarray_H = j/(array2b->blocksize);

    // get pointer to array of pointers that accesses index pointing to array[blkW][blkH]
    Array_T *block_number= Uarray2_at(array2b, (i/array2b->blocksize), (j/array2b->blocksize));

    //within the block get the cell and return it
    return(Array_at((*block_number), ((array2b->blocksize * i%(array2b->blocksize)) +j %(array2b->blocksize))));
}


/* visits every cell in one block before moving to another block */
/* it is a checked run-time error to pass a NULL T to any function in this interface */
extern void UArray2b_map(T array2b, void apply(int i, int j, T array2b, void *elem, void *cl), void *cl){

}

