//
// Created by rober on 17.03.2019.
//

#include "logic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "trie.h"
#include "constants.h"
#include "charvector.h"

// reads value of type unsingned long long from {string}
// returns 0 if input is incorrect or value is 0
static inline uint64_t readNumber(const char *string) {
    uint64_t ret = 0;
    int i = 0;

    for (i = 0; string[i] >= '0' && string[i] <= '9'; i++) {

        if (ret > UINT64_MAX / 10 || ret > UINT64_MAX - string[i] + '0')
            return 0; //value too big

        ret *= 10;
        ret += string[i] - '0';
    }

    if (string[i] != '\n')
        return 0; //input should end with newline
    return ret;
}

// checks if string {string} from position {start} is equal to {command}
static int commandLineCheck(const char *string, int start, const char *command) {
    int vector_size = strlen(string);
    if (vector_size <= start)
        return false;
    //command and vector have only one 0 character, so we only check if command hasn't ended
    for (int i = 0; command[i] != 0; i++) {
        if (string[i] != command[i])
            return false;
    }
    return true;
}

// returns true (1) if it's valid number for particle history.
// return false (0) otherwise.
static inline int validHistoryNumber(const char number) {
    return (number >= '0' && number < '0' + HISTORY_NUMBERS);
}

// checks if string {history} consists only of legal characters
// and ends with a specified {end_symbol}
// returns 0 if {history} is illegal
// returns length until end_symbol otherwise
static int validHistoryString(const char *history, const char end_symbol) {
    if (!validHistoryNumber(history[0]))
        return 0;

    int i = 0;

    for (i = 0; history[i] != end_symbol; i++) {
        if (!validHistoryNumber(history[i]))
            return 0;
    }

    return i;
}

// returns TrieHolder value that points to the Trie node specified by {history} string.
// the {history} path is only followed until it stumbles upon {end_symbol}
// returns YES if path exists
// returns NO otherwise
static TrieHolder goToValidHistory(const char *history, const char end_symbol, TrieHolder trie_holder) {
    for (int i = 0; history[i] != end_symbol; i++) {
        if (edgeExists(trie_holder.trie, history[i] - '0')) {
            trie_holder = goDown(trie_holder.trie, history[i] - '0');
        }
        else {
            trie_holder.return_code = NO;
            return trie_holder;
        }
    }

    trie_holder.return_code = YES;
    return trie_holder;
}

// Declares that {history} is now valid
static int declareHistory(const char *history, TrieHolder trie_holder) {
    if (!validHistoryString(history, '\n'))
        return INPUT_ERROR;

    for (int i = 0; history[i] != '\n'; i++) {
        trie_holder = goDown(trie_holder.trie, history[i] - '0');
        if (trie_holder.return_code == ALLOCATION_FAILURE) {
            return ALLOCATION_FAILURE;
        }
    }

    return COMMAND_SUCCESS;
}

// Removes any histories from trie that have {history} as its prefix
static int removeHistory(char *history, TrieHolder trie_holder) {
    int length = validHistoryString(history, '\n');
    if (length) {
        char old_history = history[length - 1];
        history[length - 1] = 0;
        TrieHolder node = goToValidHistory(history, 0, trie_holder);

        if (node.return_code == INPUT_ERROR)
            return INPUT_ERROR;

        if (node.return_code == YES){
            freeTrie(node.trie->sub_trees[old_history - '0']);
            node.trie->sub_trees[old_history - '0'] = NULL;
        }

        return COMMAND_SUCCESS;
    }
    else
        return INPUT_ERROR;
}

// Checks if {history} is valid at the moment
static int validHistory(const char *history, TrieHolder trie_holder) {
    if (!validHistoryString(history, '\n'))
        return INPUT_ERROR;
    TrieHolder result = goToValidHistory(history, '\n', trie_holder);
    if (result.return_code == YES)
        return YES;
    if (result.return_code == NO)
        return NO;
    return result.return_code;
}

// If {history} is valid this function
// a) if string {history} contains another number it assigns its value to
// the specifies {history}
// b) if string {history} contains history with value previously assigned
// it prints its value
static int energy(char *history, TrieHolder trie_holder) {
    int length = validHistoryString(history, ' ');
    if (length) {
        TrieHolder node = goToValidHistory(history, ' ', trie_holder);

        if (node.return_code == NO)
            return INPUT_ERROR;

        uint64_t new_energy = readNumber(history + length + 1);

        if (new_energy == 0)
            return INPUT_ERROR;

        Representative representative = find(node.trie->rep, FIRST_IN_FIND_FLAG);
        representative->val = new_energy;
        printf("OK\n");
        return COMMAND_SUCCESS;
    }
    else if (validHistoryString(history, '\n')){
        TrieHolder node = goToValidHistory(history, '\n', trie_holder);

        if (node.return_code == NO)
            return INPUT_ERROR;

        Representative representative = find(node.trie->rep, FIRST_IN_FIND_FLAG);

        if (representative->val == 0) {
            return INPUT_ERROR;
        }
        //PRIu64 is alias for llu that doesn't send a warning
        printf("%"PRIu64"\n", representative->val);
        return COMMAND_SUCCESS;
    }
    else
        return INPUT_ERROR;
}

// if {history} contains two valid histories, this function unifies their values
static int equal(const char *history, TrieHolder trie_holder) {
    int length = validHistoryString(history, ' ');
    if (length) {
        if (!validHistoryString(history + length + 1, '\n'))
            return INPUT_ERROR;

        TrieHolder node1 = goToValidHistory(history, ' ', trie_holder);
        TrieHolder node2 = goToValidHistory(history + length + 1, '\n', trie_holder);

        if (node1.return_code == INPUT_ERROR || node2.return_code == INPUT_ERROR)
            return INPUT_ERROR;

        if (node1.return_code == NO || node2.return_code == NO)
            return INPUT_ERROR;

        Representative rep1, rep2;
        rep1 = find(node1.trie->rep, FIRST_IN_FIND_FLAG);
        rep2 = find(node2.trie->rep, FIRST_IN_FIND_FLAG);

        if (rep1->val != 0 || rep2->val != 0) {
            unify(rep1, rep2);
            return COMMAND_SUCCESS;
        }
        if (rep1 == rep2)
            return COMMAND_SUCCESS;
        //else
        return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
}

// This function matches the string in {vector} with respective function
// and calls the function with the commands arguments, then handles
// the value that the function has returned.
int parseInput(CharVector vector, TrieHolder trie_holder) {

    if (commandLineCheck(vector.tab, 0, "DECLARE ")) {
        char *history = vector.tab + strlen("DECLARE ");
        int worked = declareHistory(history, trie_holder);

        if (worked == COMMAND_SUCCESS) {
            printf("OK\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == ALLOCATION_FAILURE) {
            return TASK_EXITCODE;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "REMOVE ")) {
        char *history = vector.tab + strlen ("REMOVE ");
        int worked = removeHistory(history, trie_holder);

        if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        if (worked == COMMAND_SUCCESS) {
            printf("OK\n");
            return COMMAND_SUCCESS;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "VALID ")) {
        char *history = vector.tab + strlen("VALID ");
        int worked = validHistory(history, trie_holder);

        if (worked == ALLOCATION_FAILURE) {
            return TASK_EXITCODE;
        }
        else if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == YES) {
            printf("YES\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == NO) {
            printf("NO\n");
            return COMMAND_SUCCESS;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "ENERGY ")) {
        char *history = vector.tab + strlen("ENERGY ");
        int worked = energy(history, trie_holder);

        if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == COMMAND_SUCCESS){
            return COMMAND_SUCCESS;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "EQUAL ")) {
        char *history = vector.tab + strlen("EQUAL ");
        int worked = equal(history, trie_holder);

        if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        else if (worked == COMMAND_SUCCESS) {
            printf("OK\n");
            return COMMAND_SUCCESS;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "\n")) {
        //do nothing
        return COMMAND_SUCCESS;
    }

    else if (vector.tab[0] == '#') {
        //do nothing
        return COMMAND_SUCCESS;
    }

    else if (vector.tab[0] == 0) {
        //when input ends
        return COMMAND_SUCCESS;
    }

    else {
        fprintf(stderr, "ERROR\n");
        return COMMAND_SUCCESS;
    }

    return UNPLANNED_EXIT;
}
