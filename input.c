//
// Created by rober on 14.03.2019.
//

#include <stdio.h>
#include "input.h"
#include "charvector.h"

// Reads from standard input
// returns FILE_ENDED when it meats end of input
// returns ALLOCATION_FAILURE if memory allocation fails in {vector}
// returns ALLOCATION_SUCCESS otherwise.
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

    //we will convert EOF to char 0
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