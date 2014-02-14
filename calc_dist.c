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
    int tmp = *x;
    *x = *y;
    *y = tmp;
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

/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(unsigned char *arr, int width)
{
    int x, y;
    for (y = 0; y < width; y += 1) {
        for (x = 0; x < width / 2; x += 1) {
            swap(&arr[(width * y) + x], &arr[(width * y) + (width - 1 - x)]);
        }
    }
}

/* Flips the elements of a square array ARR across the x-axis. */
void flip_vertical(unsigned char *arr, int width)
{
    flip_horizontal(arr, width);
    rotate_ccw_90(arr, width);
    rotate_ccw_90(arr, width);
}

/* Transposes the square array ARR. */
void transpose(unsigned char *arr, int width)
{
    /* Optional function */
}

// /** Returns an int array [x,y] where x is the row and y is the col of the pixel
//   * at ARRAYINDEX. This refers to a Matrix that is of size R x C. */
// int *get_XY(int arrayIndex, int r, int c)
// {
//     int colPosition = arrayIndex / c;
//     int rowPosition = arrayIndex / r;
//     return 0;
//     //not implemented
// }

// /** Returns the array index of the point [x,y] in a Matrix of size R x C. */
// int get_arrayIndex(int *XY, int r, int c)
// {
//     return 0;
//     //not implemented
// }

/** Returns the minimum value in the array ARR of size SIZE. */
int min(int *arr, int size) {
    int min_val = UINT_MAX;
    for(int i = 0; i < size; i ++){
        if(arr[i] < min_val){
            min_val = arr[i];
        }
    }
    return min_val;
}

/** Computes the simplest cast of the Euclidean distance between TEMPLATE and IMAGE.
  * Will be used to compare other transformed images to templates.
  * Assumes both images are square. */
int simple_euclidean_dist(unsigned char *image, unsigned char *template, int width)
{
    int e_distance = 0;
    int temp_size = width * width;
    for (int p = 0; p < temp_size; p++) { // this loop assumes both images are square
        char a_val = image[p];
        char b_val = template[p];
        e_distance += (a_val - b_val) * (a_val - b_val);
    }
    return e_distance;
}

/** Returns the sq Euclidean distance between TEMPLATE and IMAGE. First flips the
  * images horizontally. (x = -x) */
int flip_horizontal_dist(unsigned char *image, unsigned char *template, int width)
{
    flip_horizontal(template, width);
    int ans = simple_euclidean_dist(image, template, width);
    flip_horizontal(template, width);
    return ans;
}

/** Returns the sq Euclidean distance between TEMPLATE and IMAGE. First flips the
  * images vertically. (y = -y) */
int flip_vertical_dist(unsigned char *image, unsigned char *template, int width)
{
    flip_vertical(template, width);
    int ans = simple_euclidean_dist(image, template, width);
    flip_vertical(template, width);
    return ans;
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
    int temp_size = t_width * t_width;
    static int ARRAYSIZE = 10;
    //inits an array of max values supposed to be the size of things tested.;
    unsigned int distanceArr[ARRAYSIZE];
    for (int i = 0; i < 10; i++) {
        distanceArr[i] = UINT_MAX;
    }
    int index = 0;
    int e_distance = 0;
    // this function and assignment assumes both images are square and of the same size. checks the basic
    // case. non-translated
    distanceArr[index] = simple_euclidean_dist(image, template, t_width);
    index++;
    //checks rotation each rotation ccw.
    for (; index < 4; index++) {
        rotate_ccw_90(template, t_width);
        for (int q = 0; q < temp_size; q++) {
            char a_val = image[q];
            char b_val = template[q];
            e_distance += (a_val - b_val) * (a_val - b_val);
        }
        distanceArr[index] = e_distance;
        e_distance = 0;
    }
    //back to normal orientation
    rotate_ccw_90(template, t_width);
    // add the euclidean distance acheived from flipping template and applying to image
    distanceArr[index] = flip_horizontal_dist(template, image, t_width);
    index++;
    distanceArr[index] = flip_vertical_dist(template, image, t_width);
    index++;

    //looks for the minimum, it doesn't seem like the function i made works though
    int minimum = min(distanceArr, ARRAYSIZE);
    for (int r = 0; r < 10; r++) {
        if (distanceArr[r] < min_dist) {
            min_dist = distanceArr[r];
        }
    }
    //murrrr
    //still must do translations.
    return min_dist;
}
