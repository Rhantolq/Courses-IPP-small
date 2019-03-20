//
// Created by rober on 14.03.2019.
//

#include <stdio.h>
#include "input.h"
#include "charvector.h"

/**
 * returns -1 if malloc fails
 * returns 1 if reaches end of file
 * returns 0 otherwise
 * @param vector
 * @return error_code
 */
int readLine(CharVector* vector) {
    int i_char;

    //making sure we don't leak memory
    freeVector(vector);
    if (emptyVector(vector) == ALLOCATION_FAILURE)
        return ALLOCATION_FAILURE;

    do {
        i_char = getchar();
        if (vectorPush(vector, (char)i_char) == ALLOCATION_FAILURE) {
            freeVector(vector);
            return ALLOCATION_FAILURE;
        }
    } while (i_char != EOF && (char)i_char != '\n');

    if (i_char == EOF)
        vector->current_size--;

    if (vectorPush(vector, 0) == ALLOCATION_FAILURE) {
        freeVector(vector);
        return ALLOCATION_FAILURE;
    }

    if (i_char == EOF)
        return FILE_ENDED;
    return ALLOCATION_SUCCESS;
}