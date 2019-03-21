//
// Created by rober on 16.03.2019.
//

#ifndef MALE_CONSTANTS_H
#define MALE_CONSTANTS_H

#ifndef true
#define true 1
#endif
#ifndef  false
#define false 0
#endif

#define ALLOCATION_FAILURE 1
#define ALLOCATION_SUCCESS 0

//Defines how many ascii characters after '0' are valid history numbers
#define HISTORY_NUMBERS 4

#define FILE_ENDED -1

#define INPUT_ERROR -1

//returned when malloc fail is detected
#define TASK_EXITCODE 1

#define COMMAND_SUCCESS 0

//constants for command VALID history
#define NO 2
#define YES 3

//this exitcode identifies programmers error
#define UNPLANNED_EXIT 42

#endif //MALE_CONSTANTS_H
