//
// Created by rober on 16.03.2019.
//

#ifndef MALE_FIND_AND_UNION_H
#define MALE_FIND_AND_UNION_H

#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include "constants.h"
#define FIRST_IN_FIND_FLAG -1

typedef struct UnionRepresentative* Representative;
typedef struct UnionRepresentativeHolder UnionHolder;

struct UnionRepresentative {
    uint64_t val;
    Representative rep;
    int is_vacant;
    int number_of_predecessors;
};

struct UnionRepresentativeHolder {
    Representative representative;
    int return_code;
};

UnionHolder newRepresentative(uint64_t value);

//void freeRepresentative(Representative rep);

Representative find(Representative rep, int nodes_deleted);

void unify(Representative rep1, Representative rep2);

void markAsVacant(Representative rep);

#endif //MALE_FIND_AND_UNION_H
