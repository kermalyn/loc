#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mem.h"
#include "assert.h"
#include "except.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

typedef  A2Methods_Array2 A2;

void transform(int rotation, Pnm_ppm image,A2Methods_mapfun *map,A2Methods_T methods);

int main(int argc, char *argv[]) {
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
            //if(rotation == 90){ translation(rotation, image, map, methods);}
            assert(*endptr == '\0'); // parsed all correctly #aborts here
            assert(rotation == 0   || rotation == 90
                   || rotation == 180 || rotation == 270);
            //if(rotation == 90){ translation(rotation, image, map, methods);}
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
    FILE *file;
    file  = fopen(argv[argc-1],"r");

    Pnm_ppm image;

    if(file != NULL){
        image = Pnm_ppmread(file,methods);
    }else{
        image = Pnm_ppmread(stdin,methods);
    }
    transform(rotation,image,map,methods);
    fclose(file);
}

void rotate0(int i, int j ,A2 image, void *elem, void *transformed){
    (void)image;
    Pnm_ppm newImage = transformed;
    const struct A2Methods_T *methods = newImage->methods;
    Pnm_rgb final_pos = (Pnm_rgb)elem;
    Pnm_rgb dk = methods->at(newImage->pixels,i,j);
    *dk = *final_pos;
}
void rotate90(int i, int j ,A2 image, void *elem, void *transformed){
    (void)image;
    Pnm_ppm newImage = transformed;
    const struct A2Methods_T *methods = newImage->methods;
    Pnm_rgb final_pos = (Pnm_rgb)elem;
    Pnm_rgb dk = methods->at(newImage->pixels,methods->height(image)-j-1,i);
    *dk = *final_pos;
}
void rotate180(int i, int j ,A2 image, void *elem, void *transformed){
    (void)image;
    Pnm_ppm newImage = transformed;
    const struct A2Methods_T *methods = newImage->methods;
    Pnm_rgb final_pos = (Pnm_rgb)elem;
    Pnm_rgb dk = methods->at(newImage->pixels,methods->width(image)-i-1,methods->height(image)-j-1);
    *dk = *final_pos;
}
void rotate270(int i, int j ,A2 image, void *elem, void *transformed){
    (void)image;
    Pnm_ppm newImage = transformed;
    const struct A2Methods_T *methods = newImage->methods;
    Pnm_rgb final_pos = (Pnm_rgb)elem;
    Pnm_rgb dk = methods->at(newImage->pixels,j,methods->width(image)-i-1);
    *dk = *final_pos;
}


void transform(int rotation, Pnm_ppm image,A2Methods_mapfun *map,A2Methods_T methods){
    Pnm_ppm newImage;
    NEW(newImage);
    newImage->methods=methods;
    if(rotation == 270 || rotation == 90){
        newImage->width = image->height;
        newImage->height = image->width;
    }else{
        newImage->width = image->width;
        newImage->height = image->height;
    }
    newImage->denominator = image->denominator;
    newImage->pixels = methods->new(newImage->width,newImage->height, methods->size(image->pixels));
    if(rotation == 0){
        map(image->pixels, rotate0, newImage);
        Pnm_ppmwrite(stdout, newImage);
    }else if(rotation == 90){
        map(image->pixels, rotate90, newImage);
        Pnm_ppmwrite(stdout, newImage);
    }else if(rotation == 180){
        map(image->pixels, rotate180, newImage);
        Pnm_ppmwrite(stdout, newImage);
    }else if(rotation == 270){
        map(image->pixels, rotate270, newImage);
        Pnm_ppmwrite(stdout, newImage);
    }
    Pnm_ppmfree(&image);
    Pnm_ppmfree(&newImage);
}
