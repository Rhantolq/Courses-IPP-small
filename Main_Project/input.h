//
// Created by rober on 14.03.2019.
//

#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "charvector.h"

// Reads from standard input
// returns FILE_ENDED when it meats end of input
// returns ALLOCATION_FAILURE if memory allocation fails in {vector}
// returns ALLOCATION_SUCCESS otherwise.
int readLine(CharVector *vector);

#endif //MALE_INPUT_H
