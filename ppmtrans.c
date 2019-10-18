#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"


typedef  A2Methods_Array2 A2; //like the a2blocked kinda
void translation(int rotation, Pnm_ppm image, A2Methods_mapfun *map, A2Methods_T method);
void rotate90(int i, int j ,A2 image, void *elem, void *transformed);

int main(int argc, char *argv[]) {
    FILE* input;
    int rotation = 0;
    A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
    assert(methods);
    A2Methods_mapfun *map = methods->map_default; // default to best map
    assert(map);
#define SET_METHODS(METHODS, MAP, WHAT) do { \
      methods = (METHODS); \
      assert(methods); \
      map = methods->MAP; \
      if (!map) { \
        fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
        exit(1); \
      } \
    } while(0)

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-row-major")) {
            SET_METHODS(array2_methods_plain, map_row_major, "row-major");
        } else if (!strcmp(argv[i], "-col-major")) {
            SET_METHODS(array2_methods_plain, map_col_major, "column-major");
        } else if (!strcmp(argv[i], "-block-major")) {
            SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
        } else if (!strcmp(argv[i], "-rotate")) {
            assert(i + 1 < argc);
            char *endptr;
            rotation = strtol(argv[++i], &endptr, 10);
            assert(*endptr == '\0'); // parsed all correctly
            assert(rotation == 0   || rotation == 90
                   || rotation == 180 || rotation == 270);
        } else if (*argv[i] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
            exit(1);
        } else if (argc - i > 2) {
            fprintf(stderr, "Usage: %s [-rotate <angle>] "
                            "[-{row,col,block}-major] [filename]\n", argv[0]);
            exit(1);
        } else {
            break;
        }
    }
    //assert(0); // the rest of this function is not yet implemented
    if(i<argc){
        input= fopen(argv[i],"r");
    }else{
        input = stdin;
    }
    assert(input!=NULL);
    Pnm_ppm image= Pnm_ppmread(input, methods);
    translation(rotation, image, map, methods);

    fclose(input);
}



void translation(int rotation, Pnm_ppm image, A2Methods_mapfun *map, A2Methods_T methods)
{

    if(rotation==0){
        Pnm_ppmwrite(stdout, image);

    }else{

        Pnm_ppm new_image = methods->new(image->height, image->width, sizeof(Pnm_rgb));
        map(image, rotate90, new_image);
        Pnm_ppmwrite(stdout, new_image);

    }
}


void rotate90(int i, int j, A2 oldImage, void *elem, void *transformed){
    Pnm_ppm newImage =  transformed; //sets the 2D array to a2methods_2darray
    const struct A2Methods_T *methods = newImage->methods;
    Pnm_rgb final_pos = elem; //makes the positon of current element which contains a rgb value
    Pnm_rgb location=  methods->at(newImage, methods->width(oldImage)-i-1, j);
    *location = *final_pos;
}


/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mem.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

//void temp(){return;}

typedef  A2Methods_Array2 A2; //like the a2blocked kinda
Pnm_ppm translation(int rotation, Pnm_ppm image, A2Methods_mapfun *map, A2Methods_T method);
void rotate90(int i, int j ,A2 image, void *elem, void *transformed);

int main(int argc, char *argv[]) {
    FILE* input;
    int rotation = 0;
    A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
    assert(methods);
    A2Methods_mapfun *map = methods->map_default; // default to best map
    assert(map);
#define SET_METHODS(METHODS, MAP, WHAT) do { \
      methods = (METHODS); \
      assert(methods); \
      map = methods->MAP; \
      if (!map) { \
        fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
        exit(1); \
      } \
    } while(0)

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-row-major")) {
            SET_METHODS(array2_methods_plain, map_row_major, "row-major");
        } else if (!strcmp(argv[i], "-col-major")) {
            SET_METHODS(array2_methods_plain, map_col_major, "column-major");
        } else if (!strcmp(argv[i], "-block-major")) {
            SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
        } else if (!strcmp(argv[i], "-rotate")) {
            assert(i + 1 < argc);
            char *endptr;
            rotation = strtol(argv[++i], &endptr, 10);
            assert(*endptr == '\0'); // parsed all correctly
            assert(rotation == 0   || rotation == 90
                   || rotation == 180 || rotation == 270);
        } else if (*argv[i] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
            exit(1);
        } else if (argc - i > 2) {
            fprintf(stderr, "Usage: %s [-rotate <angle>] "
                            "[-{row,col,block}-major] [filename]\n", argv[0]);
            exit(1);
        } else {
            break;
        }
    }
    //assert(0); // the rest of this function is not yet implemented
    if(i<=argc-1){
        input= fopen(argv[i],"r");
    }else{
        input = stdin;
    }
    assert(input!=NULL);
    Pnm_ppm image= Pnm_ppmread(input, methods);
    translation(rotation, image, map, methods);

    fclose(input);
}


Pnm_ppm translation(int rotation, Pnm_ppm image, A2Methods_mapfun *map, A2Methods_T methods){
    Pnm_ppm new_image;
    NEW(new_image);
    if(rotation==0){
        Pnm_ppmwrite(stdout, image);
    }else if(rotation == 90){
        new_image->width= image->height;
        new_image->height= image->width;
        new_image->pixels = methods->new(new_image->height,new_image->width, sizeof(Pnm_rgb));
        methods->at(image->pixels, rotate90, new_image);
        Pnm_ppmwrite(stdout, new_image);

    }
}

void rotate90(int i, int j, A2 oldImage, void *elem, void *transformed){
    A2 newImage =  transformed; //sets the 2D array to a2methods_2darray
    Pnm_rgb final_pos = elem; //makes the positon of current element which contains a rgb value
    Pnm_rgb location=  methods->at(new_image, methods->width(image)-i-1, i);
    *location = *final_pos;
}


*/