//
// Created by rober on 13.03.2019.
//

#include "charvector.h"
#include <stdlib.h>

// Pushes {new_elem} to the end of vector {v}
int vectorPush(CharVector* v, char new_elem) {
    if (v->tab == NULL)
        return ALLOCATION_FAILURE;

    if (v->current_size + 1 >= v->max_size) {
        v->max_size *= 2;
        char* new_tab = realloc(v->tab, sizeof(char) * v->max_size);

        //allocation failure
        if (new_tab == NULL) {
            return ALLOCATION_FAILURE;
        }

        v->tab = new_tab;
    }

    v->tab[v->current_size] = new_elem;
    v->current_size++;
    return ALLOCATION_SUCCESS;
}

// Frees up memory allocated in place of {v}
void freeVector(CharVector *v) {
    free(v->tab);
    v->tab = NULL;
}

// Creates empty character vector in pointer {v}
int emptyVector(CharVector* v) {
    {
        v->current_size = 0;
        v->max_size = 4;
        v->tab = malloc(sizeof(char) * v->max_size);
        if (v->tab == NULL)
            return ALLOCATION_FAILURE;
        return ALLOCATION_SUCCESS;
    }
}
