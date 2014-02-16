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

/** Returns the position of the corresponding index in TEST with
 * offsets X_OFFSET and Y_OFFSET. */
int test_position_after_translation(int template_pos, int x_offset, 
        int y_offset, int temp_width, int test_width){
    int numRows = template_pos/temp_width;
    int resulting_position = template_pos + (test_width - temp_width)*numRows
        + x_offset + (y_offset * test_width);
    return resulting_position;
}

/** Computes the Euclidean distance between TEMPLATE and IMAGE with
 * an X_OFFSET and Y_OFFSET in pixels. Will be used to compare other 
 * transformed images to templates.
 * Assumes both images are square. */
unsigned int complex_euclidean_dist(unsigned char *image, unsigned char *template,
        int temp_width, int x_offset, int y_offset, int test_width) {
    unsigned int e_distance = 0;
    int temp_size = temp_width * temp_width;
    for (int p = 0; p < temp_size; p++) {
        int translated_position = test_position_after_translation(p, x_offset, 
                y_offset, temp_width, test_width);
        char a_val = image[translated_position];
        char b_val = template[p];
        e_distance += (a_val - b_val) * (a_val - b_val);
    }
    return e_distance;
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
    int index = 0;
    int e_distance = 0;
    int excess_x = i_width - t_width;
    int excess_y = i_height - t_width;
    //inits an array of max values supposed to be the size of things tested.;
    int ARRAYSIZE = ((excess_y + 1) * (excess_x + 1) * 8);
    unsigned int distanceArr[ARRAYSIZE];
    for (int i = 0; i < ARRAYSIZE; i++) {
        distanceArr[i] = UINT_MAX;
    }
    //checks the edistance of 8 distinct pics and then translates left.
    for (int i = 0; i <= excess_y; i++) {
        int y_offset = i;
        for (int j = 0; j <= excess_x; j++) {
            int x_offset = j;
            //normal
            distanceArr[index] = complex_euclidean_dist(image, template, t_width,
                    x_offset, y_offset, i_width);
            index++;
            //check rotations
            for (; index % 4 != 0; index++) {
                rotate_ccw_90(template, t_width);
                distanceArr[index] = complex_euclidean_dist(image, template, t_width,
                    x_offset, y_offset, i_width);
            }
            //rotate back to normal
            rotate_ccw_90(template, t_width);
            //check flipped rotation
            flip_horizontal(template, t_width);
            distanceArr[index] = complex_euclidean_dist(image, template, t_width,
                    x_offset, y_offset, i_width);
            index++;
            for (; index % 4 != 0; index++) {
                rotate_ccw_90(template, t_width);
                distanceArr[index] = complex_euclidean_dist(image, template, t_width,
                    x_offset, y_offset, i_width);
            }
            rotate_ccw_90(template, t_width);
            // flip back to normal
            flip_horizontal(template, t_width);
        }
    }
    //looks for the minimum
    for (int r = 0; r < ARRAYSIZE; r++) {
        if (distanceArr[r] < min_dist) {
            min_dist = distanceArr[r];
        }
    }
    return min_dist;
}

