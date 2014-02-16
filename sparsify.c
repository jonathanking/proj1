/* CS61C Sp14 Project 1
 *
 * Converts a bitmap array into a sparse representation of the matrix.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdlib.h>
#include <stdio.h>
#include "sparsify.h"
#include "utils.h"

/* Performs some basic tests. Feel free to define additional tests. */
void test_basic()
{
    Row *sparse_matrix;

    unsigned char small[] = {
        0x01, 0x02, 0x03,
        0x04, 0x05, 0x06,
        0x07, 0x08, 0x09
    };

    unsigned char empty[] = {
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF
    };

    unsigned char tiny[] = { 56 };

    unsigned char empty_center[] = {
        0x01, 0x02, 0x03,
        0x04, 0xFF, 0x06,
        0x07, 0x08, 0x09
    };

    unsigned char white_margins_empty_spots[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x01, 0x01, 0x01, 0x01, 0xFF,
        0xFF, 0x02, 0x02, 0x02, 0x02, 0xFF,
        0xFF, 0x03, 0xFF, 0xFF, 0x03, 0xFF,
        0xFF, 0xFF, 0x04, 0x04, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    unsigned char theres_a_seven_in_there[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    unsigned char skips_middle_line[] = {
        0x01, 0x02, 0x03,
        0xFF, 0xFF, 0xFF,
        0x07, 0x08, 0x09
    };



    sparse_matrix = dense_to_sparse(small, 3, 3);
    test_sparse("small", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(empty, 3, 3);
    test_sparse("empty", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(tiny, 1, 1);
    test_sparse("tiny", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(empty_center, 3, 3);
    test_sparse("empty_center", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(white_margins_empty_spots, 6, 6);
    test_sparse("white_margins_empty_spots", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(theres_a_seven_in_there, 6, 6);
    test_sparse("theres_a_seven_in_there", sparse_matrix);
    free_sparse(sparse_matrix);

    sparse_matrix = dense_to_sparse(skips_middle_line, 3, 3);
    test_sparse("skips_middle_line", sparse_matrix);
    free_sparse(sparse_matrix);
}

int main(int argc, char **argv)
{
    Image img;
    Row *sparse_matrix;

    if ( argc != 2 ) {
        test_basic();
    } else {
        img = load_bmp(argv[1]);

        sparse_matrix = dense_to_sparse(img.data, img.width, img.height);
        test_sparse(argv[1], sparse_matrix);
        free_sparse(sparse_matrix);
        free(img.data);
    }
    return 0;
}

/* Prints a sparse matrix. */
void print_sparse(Row *sparse_matrix)
{
    Row *cur_row;
    Elem *cur_elem;
    int y;

    cur_row = sparse_matrix;
    printf("Printing sparse matrix:\n");
    while (cur_row) {
        y = cur_row->y;
        cur_elem = cur_row->elems;
        printf("Row %d: ", y);
        while (cur_elem) {
            printf("[(%d,%d):%d] ", cur_elem->x, y, cur_elem->value);
            cur_elem = cur_elem->next;
        }
        printf("\n");
        cur_row = cur_row->next;
    }
    printf("end of matrix\n");
}



