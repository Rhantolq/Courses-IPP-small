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

//returns 0 if input is incorrect
uint64_t readNumber(const char *string) {
    uint64_t ret = 0;
    int i = 0;
    for (i = 0; string[i] >= '0' && string[i] <= '9'; i++) {
        if (ret > UINT64_MAX / 10)
            return 0; //value too big
        ret *= 10;
        ret += string[i] - '0';
    }
    if (string[i] != '\n')
        return 0; //input should end with newline
    return ret;
}

int commandLineCheck(const char* vector, int start, const char* command) {
    int vector_size = strlen(vector);
    if (vector_size <= start)
        return false;
    //command and vector have only one 0 character, so we only check if command hasnt ended
    for (int i = 0; command[i] != 0; i++) {
        if (vector[i] != command[i])
            return false;
    }
    return true;
}

TrieHolder declareHistory(const char* history, TrieHolder trie_holder) {
    TrieHolder ret;
    ret.trie = trie_holder.trie;
    ret.return_code = COMMAND_SUCCESS;

    for (int i = 0; history[i] != '\n'; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            ret.return_code = INPUT_ERROR;
            return ret;
        }
        trie_holder = goDown(trie_holder.trie, history[i] - '0');
        if (trie_holder.return_code == ALLOCATION_FAILURE) {
            ret.return_code = ALLOCATION_FAILURE;
            return ret;
        }
    }
    return ret;
}

int removeHistory(const char* history, TrieHolder trie_holder) {
    for (int i = 0; history[i] != '\n'; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            return INPUT_ERROR;
        }
        if (edgeExists(trie_holder.trie, history[i] - '0')) {
            if (history[i + 1] == '\n') {
                TrieHolder temp_holder = goDown(trie_holder.trie, history[i] - '0');
                freeTrie(temp_holder.trie);
                trie_holder.trie->sub_trees[history[i] - '0'] = NULL;

            } else {
                trie_holder = goDown(trie_holder.trie, history[i] - '0');
            }
        }
        else {
            //such tree does not exist
            return COMMAND_SUCCESS;
        }
    }
    return COMMAND_SUCCESS;
}

int validHistory(const char* history, TrieHolder trie_holder) {
    for (int i = 0; history[i] != '\n'; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            return INPUT_ERROR;
        }
        if (edgeExists(trie_holder.trie, history[i] - '0')) {
            trie_holder = goDown(trie_holder.trie, history[i] - '0');
        }
        else
            return NO;
        if (trie_holder.return_code == ALLOCATION_FAILURE) {
            //this behaviour should not happen
            return UNPLANNED_EXIT;
        }
    }
    return YES;
}

int energy(char* history, TrieHolder trie_holder) {
    int i = 0;
    for (i = 0; history[i] != ' ' && history[i] != '\n'; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            return INPUT_ERROR;
        }
        if (edgeExists(trie_holder.trie, history[i] - '0')) {
            trie_holder = goDown(trie_holder.trie, history[i] - '0');
        }
        else {
            return INPUT_ERROR;
        }
        if (trie_holder.return_code == ALLOCATION_FAILURE) {
            //this behaviour should not happen
            return UNPLANNED_EXIT;
        }
    }

    if (history[i] == ' ') {
        uint64_t new_energy = readNumber(history + i + 1);
        if (new_energy == 0)
            return INPUT_ERROR;
        Representative representative = find(trie_holder.trie->rep, FIRST_IN_FIND_FLAG);
        representative->val = new_energy;
        printf("OK\n");
        return COMMAND_SUCCESS;
    }
    else if (history[i] == '\n') {
        Representative representative = find(trie_holder.trie->rep, FIRST_IN_FIND_FLAG);
        if (representative->val == 0) {
            return INPUT_ERROR;
        }
        printf("%"PRIu64"\n", representative->val);
        return COMMAND_SUCCESS;
    }

    return INPUT_ERROR;
}

int equal(const char* history, TrieHolder trie_holder) {
    int i = 0;
    TrieHolder trie_holder2 = trie_holder;
    Representative rep1, rep2;

    for (i = 0; history[i] != ' '; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            return INPUT_ERROR;
        }
        if (edgeExists(trie_holder.trie, history[i] - '0')) {
            trie_holder = goDown(trie_holder.trie, history[i] - '0');
        }
        else {
            return INPUT_ERROR;
        }
        if (trie_holder.return_code == ALLOCATION_FAILURE) {
            //this behaviour should not happen
            return UNPLANNED_EXIT;
        }
    }
    for (i = i + 1; history[i] != '\n'; i++) {
        if (history[i] < '0' || history[i] >= '0' + HISTORY_NUMBERS) {
            return INPUT_ERROR;
        }
        if (edgeExists(trie_holder2.trie, history[i] - '0')) {
            trie_holder2 = goDown(trie_holder2.trie, history[i] - '0');
        }
        else {
            return INPUT_ERROR;
        }
        if (trie_holder2.return_code == ALLOCATION_FAILURE) {
            //this behaviour should not happen
            return UNPLANNED_EXIT;
        }
    }

    rep1 = find(trie_holder.trie->rep, FIRST_IN_FIND_FLAG);
    rep2 = find(trie_holder2.trie->rep, FIRST_IN_FIND_FLAG);

    if (rep1->val != 0 || rep2->val != 0) {
        unify(rep1, rep2);
        return COMMAND_SUCCESS;
    }
    //else
    return INPUT_ERROR;
}


int parseInput(CharVector vector, TrieHolder trie_holder) {

    if (commandLineCheck(vector.tab, 0, "DECLARE ")) {
        char* history = vector.tab + strlen("DECLARE ");
        trie_holder = declareHistory(history, trie_holder);
        if (trie_holder.return_code == COMMAND_SUCCESS) {
            printf("OK\n");
            return COMMAND_SUCCESS;
        }
        else if (trie_holder.return_code == ALLOCATION_FAILURE) {
            printf("NOPE4!");
            return TASK_EXITCODE;
        }
        else if (trie_holder.return_code == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
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
        else {
            //this behaviour cannot be reached
            printf("REMOVE FAIL worked == %d\n", worked);
            return worked;
        }
    }

    else if (commandLineCheck(vector.tab, 0, "VALID ")) {
        char* history = vector.tab + strlen("VALID ");
        int worked = validHistory(history, trie_holder);
        if (worked == ALLOCATION_FAILURE) {
            printf("NOPE5!");
            return TASK_EXITCODE;
        }
        else if (worked == INPUT_ERROR) {
            fprintf(stderr, "ERROR\n");
            return COMMAND_SUCCESS;
        }
        else {
            if (worked == YES) {
                printf("YES\n");
                return COMMAND_SUCCESS;
            }
            else if (worked == NO) {
                printf("NO\n");
                return COMMAND_SUCCESS;
            }
            else {
                //this behaviour cannot be reached
                printf("VALID FAIL worked == %d\n", worked);
                return worked;
            }
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
        else {
            //this behaviour cannot be reached
            printf("ENERGY FAIL worked == %d\n", worked);
            return worked;
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
        else {
            //this behaviour cannot be reached
            printf("ENERGY FAIL worked == %d\n", worked);
            return worked;
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
