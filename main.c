#include <stdio.h>
#include <stdlib.h>
#include "charvector.h"
#include "input.h"
#include "trie.h"
#include "logic.h"

int main(void) {
    int read_code = 0;
    CharVector vector;
    TrieHolder trie_holder;

    trie_holder = newTrie();
    if (trie_holder.return_code == ALLOCATION_FAILURE) {
        printf("NOPE1!");
        return TASK_EXITCODE;
    }

    if (emptyVector(&vector) == ALLOCATION_FAILURE) {
        printf("NOPE2!");
        return TASK_EXITCODE;
    }

    do {
        read_code = readLine(&vector);
        if (read_code == ALLOCATION_FAILURE) {
            printf("NOPE3!");
            freeTrie(trie_holder.trie);
            freeVector(&vector);
            return TASK_EXITCODE;
        }

        int return_code = parseInput(vector, trie_holder);

        if (return_code == TASK_EXITCODE) {
            freeVector(&vector);
            freeTrie(trie_holder.trie);
            return TASK_EXITCODE;
        }
    } while (read_code == 0);

    //printf("no more reading!");
    freeVector(&vector);
    freeTrie(trie_holder.trie);
    //printf("Happy end!");
    return 0;
}