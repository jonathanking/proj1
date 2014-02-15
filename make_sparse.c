/*
 * PROJ1-1: YOUR TASK B CODE HERE
 * 
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "sparsify.h"
#include "utils.h"

/* Make a new Row struct. */
Row *row_new(int rownum) {
    Row *newrow;
    //Allocate memory for the struct.
    newrow = malloc(1 * sizeof(Row));
    //Check to see if we got memory!
    if (newrow == NULL) {
        allocation_failed();
    }
    //set y as the row number
    newrow->y = rownum;
    //set the elem as null
    newrow->elems = NULL;
    //set the next row to null
    newrow->next = NULL;
    return newrow;
}

/* Makes a new Elem struct. */
Elem *elem_new(int colnum, unsigned char val) {
    Elem *newelem;
    //Allocate memory for the struct.
    newelem = malloc(1 * sizeof(Elem));
    //check if we got memory
    if (newelem == NULL) {
        allocation_failed();
    }
    //set x as the column number
    newelem->x = colnum;
    //set the value as val
    newelem->value = val;
    //set the next Elem to null.
    newelem->next = NULL;
    return newelem;
}

/* Returns a NULL-terminated list of Row structs, each containing 
 * a NULL-terminated list of Elem structs.
 * See sparsify.h for descriptions of the Row/Elem structs.
 * Each Elem corresponds to an entry in dense_matrix whose
 * value is not 255 (white).
 * This function can return NULL if the dense_matrix is entirely white.
 */
Row *dense_to_sparse(unsigned char *dense_matrix, int width, int height) {
    /* YOUR CODE HERE */
    int hasElem, initR, initE;
    hasElem = 0;
    initR = initE = 1;
    Row *initRow = NULL;
    Row *lastRow;
    Elem *initElem, *lastElem;
    //start with the height
    for (int y = 0; y < height; y++) {
        //conservatively check whether the next row has elems
        for (int i = 0; i < width; i++) {
            if (dense_matrix[(y * height) + i] != 255) {
                hasElem = 1;
            }
        }
        //if this row has elements, make a new row
        if (hasElem) {
            if (initR) {
                initRow = row_new(y);
                lastRow = initRow;
                initR = 0;
            } else {
                Row *newRow = row_new(y);
                lastRow->next = newRow;
                lastRow = newRow;
            }
            //make the elem nodes for that row
            for (int x = 0; x < width; x++) {
                if (dense_matrix[(y * height) + x] != 255) {
                    if (initE) {
                        initElem = elem_new(x, dense_matrix[(y * height) + x]);
                        lastElem = initElem;
                        initE = 0;
                        lastRow->elems = initElem;
                    } else {
                        Elem *newElem = elem_new(x, dense_matrix[(y * height) + x]);
                        lastElem->next = newElem;
                        lastElem = newElem;
                    }
                }
            }
                hasElem = 0;
                initE = 1;
            }
    }
    return initRow;
}

/* Frees all memory associated with SPARSE. SPARSE may be NULL. */
void free_sparse(Row *sparse) {
    Row *currRow = sparse;
    Elem *currElem = currRow->elems;
    while (currRow != NULL) {
        while (currElem != NULL) {
            Elem *etemp = currElem;
            currElem = currElem->next;
            free(etemp);
        }
        Row *rtemp = currRow;
        currRow = currRow->next;
        free(rtemp);
        if (currRow) {
            currElem = currRow->elems;
        }
    }
}

