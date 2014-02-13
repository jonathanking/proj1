/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * You MUST implement the calc_min_dist() function in this file.
 *
 * You do not need to implement/use the swap(), flip_horizontal(), transpose(), or rotate_ccw_90()
 * functions, but you may find them useful. Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "digit_rec.h"
#include "utils.h"
#include "limits.h"

/* Swaps the values pointed to by the pointers X and Y. */
void swap(unsigned char *x, unsigned char *y)
{
    /* Optional function */
}

/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(unsigned char *arr, int width)
{
    /* Optional function */
}

/* Transposes the square array ARR. */
void transpose(unsigned char *arr, int width)
{
    /* Optional function */
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(unsigned char *arr, int width)
{
    for (int onion = 0; onion < (width / 2); onion++) {
        //rotate layer by layer
        int begin = onion;
        int end = width - onion - 1;
        for (int i = begin; i < end; i++) {
            //offset for things beyond corners
            int offset = i - begin;
            //better save this so it doesn't get lost
            char top = arr[(begin * width) + i];
            //right side to top side
            arr[(begin * width) + i] = arr[(i * width) + end];
            //bottom side to right side
            arr[(i * width) + end] = arr[(end * width) + end - offset];
            //left side moved to bottom
            arr[(end * width) + end - offset] =
                arr[((end - offset) * width) + begin];
            //top side to left side
            arr[((end - offset) * width) + begin] = top;
        }
    }
}

/** Returns an int array [x,y] where x is the row and y is the col of the pixel
  * at ARRAYINDEX. This refers to a Matrix that is of size R x C. */
int *get_XY(int arrayIndex, int r, int c) {
    int colPosition = arrayIndex / c;
    int rowPosition = arrayIndex / r;

    //     0  1
    // 0 { 1, 2 }
    // 1 { 3, 4 }

    // {1, 2, 3, 4}  what is get_XY(1, 2, 2)?
    //  0  1  2  3
}

/** Returns the array index of the point [x,y] in a Matrix of size R x C. */
int get_arrayIndex(int *XY, int r, int c){

}

/** Returns the sq Euclidean distance between TEMPLATE and IMAGE. First flips the
  * images horizontally. (x = -x) */
int flip_horizontal_dist(unsigned char *image, int i_width, int i_height,
                         unsigned char *template, int t_width ){
    unsigned char *imageFLIPPED;
    int image_size = i_width * i_height;
    for(int i = 0; i < image_size; i++){
        int *xy = get_XY(i, t_width, t_width);
        xy[0] = -1 * xy[0]; // x = -x
        int new_position = get_arrayIndex(xy, t_width, t_width);
        imageFLIPPED[new_position] = image[i];
    }


}

/** Computes the simplest cast of the Euclidean distance between TEMPLATE and IMAGE.
  * Will be used to compare other transformed images to templates. */
int simple_euclidean_dist(unsigned char *image, int i_width, int i_height,
                           unsigned char *template, int t_width){
        for (int p = 0; p < temp_size; p++) { // this loop assumes both images are square, though this will not be true for cases other than the extra credit
        char a_val = image[p];
        char b_val = template[p];
        e_distance += (a_val - b_val) * (a_val - b_val);
    }

}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 * 
 * Returns the smallest distance between IMAGE and all the possible
 * translations/rotations/flips of TEMPLATE.
 */
unsigned int calc_min_dist(unsigned char *image, int i_width, int i_height,
                           unsigned char *template, int t_width)
{
    unsigned int min_dist = UINT_MAX;
    //implement Euclidean distance formula, obtain repsective pixel values for each
    // ex (0,0) a vs (0,0)b ---> ((0,0)a - (0,0)b)^2
    // a and b represent test image, template image, respectively

    int MN = i_width * i_height;
    int temp_size = t_width * t_width;
    //inits an array of max values supposed to be the size of things tested.;
    unsigned int distanceArr[10];
    for (int i = 0; i < 10; i++) {
        distanceArr[i] = UINT_MAX;
    }
    int e_distance = 0;
    // this loop assumes both images are square and of the same size. checks the basic
    // case.
    for (int p = 0; p < temp_size; p++) {
        char a_val = image[p];
        char b_val = template[p];
        e_distance += (a_val - b_val) * (a_val - b_val);
    }

    distanceArr[0] = e_distance;
    e_distance = 0;
    //checks rotation each rotation ccw.
    for (int s = 1; s < 4; s++) {
        rotate_ccw_90(template, t_width);
        for (int q = 0; q < temp_size; q++) {
            char a_val = image[q];
            char b_val = template[q];
            e_distance += (a_val - b_val) * (a_val - b_val);
        }
        distanceArr[s] = e_distance;
        e_distance = 0;
    }
    //back to normal orientation
    rotate_ccw_90(template, t_width);
    //looks for the minimum
    for (int r = 0; r < 10; r++) {
        if (distanceArr[r] < min_dist) {
            min_dist = distanceArr[r];
        }
    }
    //murrrr
    //still must do flipz and translations.
    return min_dist;
}
