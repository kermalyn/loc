#include <stdlib.h>
#include <a2plain.h>
#include "uarray2.h"
#include "mem.h"
#include "assert.h"

// define a private version of each function in A2Methods_T that we implement
typedef A2Methods_Array2 A2A;

static A2Methods_Array2 new(int width, int height, int size) {
    return UArray2_new(width,height,size);
}

static A2Methods_Array2 new_with_blocksize(int width, int height, int size,
                                           int blocksize)
{
    (void) blocksize;
    return UArray2_new(width,height,size);
}

//... many more private (static) definitions follow ...
static void a2afree (A2A *array) {
    UArray2_free((UArray2_T *)array);
}

static int width    (A2A array2) { return UArray2_Width(array2); }
static int height   (A2A array2) { return UArray2_Height(array2); }
static int size     (A2A array2) { return UArray2_size(array2); }
static int blocksize(A2A array2) { assert(array2); return 0; }

static A2Methods_Object *at(A2A array, int i, int j) {
    return UArray2_at(array, i, j);
}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

static void map_row_major(A2A array2, A2Methods_applyfun apply, void *cl)
{
    (void)array2;
    UArray2_map_row_major(array2, (applyfun *) apply, cl);
}

static void map_col_major(A2A array2, A2Methods_applyfun apply, void *cl)
{
    (void)array2;
    UArray2_map_col_major(array2, (applyfun*) apply, cl);
}

static struct A2Methods_T array2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2afree,
        width,
        height,
        size,
        blocksize,             // blocksize
        at,
        map_row_major,
        map_col_major,
        NULL,             // map_block_major
        map_row_major,      // map_default
};


A2Methods_T array2_methods_plain = &array2_methods_plain_struct;
