//
// Created by rober on 14.03.2019.
//

#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "charvector.h"


/**
 * returns -1 if malloc fails
 * returns 1 if reaches end of file
 * returns 0 otherwise
 * @param vector
 * @return error_code
 */
int readLine(CharVector* vector);

#endif //MALE_INPUT_H
