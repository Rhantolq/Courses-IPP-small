//
// Created by rober on 13.03.2019.
//

#ifndef MALE_CHARVECTOR_H
#define MALE_CHARVECTOR_H

#include "constants.h"

typedef struct CharacterVector CharVector;
typedef struct CharacterVectorHolder CharVectorHolder;

struct CharacterVector {
    char* tab;
    int max_size, current_size;
};

//adds new_elem on the end of the vector
int vectorPush(CharVector *vector, char new_elem);

//frees up space reserved by vector.tab
void freeVector(CharVector *vector);

//empty vector constructor
int emptyVector(CharVector *v);

#endif //MALE_CHARVECTOR_H
