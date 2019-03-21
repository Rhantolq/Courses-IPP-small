//
// Created by rober on 14.03.2019.
//

#include "trie.h"
#include <stdlib.h>
#include "find_and_union.h"

// creates new trie
TrieHolder newTrie(void) {
    TrieHolder ret;
    ret.trie = malloc(sizeof(struct TrieTree));
    if (ret.trie == NULL) {
        ret.return_code = ALLOCATION_FAILURE;
        return ret;
    }
    UnionHolder new_rep = newRepresentative(0);
    ret.trie->rep = new_rep.representative;
    if (new_rep.return_code == ALLOCATION_FAILURE) {
        free(ret.trie);
        ret.return_code = ALLOCATION_FAILURE;
        return ret;
    }

    for (int i = 0; i < HISTORY_NUMBERS; i++)
        ret.trie->sub_trees[i] = NULL;
    ret.return_code = ALLOCATION_SUCCESS;
    return ret;
}

// returns true if subtree specified by {edge} exists
int edgeExists(Trie trie, int edge) {
    if (trie != NULL)
        return (trie->sub_trees[edge] != NULL);
    else
        return false;
}

// goes down the trie through {edge}
// if subtree didn't exist previously it creates it
TrieHolder goDown(Trie trie, int edge) {
    TrieHolder ret;
    if (edgeExists(trie, edge)) {
        ret.trie = trie->sub_trees[edge];
        ret.return_code = ALLOCATION_SUCCESS;
        return ret;
    }
    else {
        ret = newTrie();
        if (ret.return_code == ALLOCATION_FAILURE)
            return ret;
        trie->sub_trees[edge] = ret.trie;
        return ret;
    }
}

// Frees the memory allocated by trie
void freeTrie(Trie trie) {
    if (trie != NULL) {
        for (int i = 0; i < HISTORY_NUMBERS; i++) {
            if (edgeExists(trie, i))
                freeTrie(trie->sub_trees[i]);
        }
        markAsVacant(trie->rep);
        free(trie);
    }
}
