//
// Created by rober on 14.03.2019.
//

#ifndef MALE_TRIE_H
#define MALE_TRIE_H

#include "constants.h"
#include "find_and_union.h"

typedef struct TrieTree* Trie;
typedef struct TrieTreeHolder TrieHolder;

struct TrieTree {
    Representative rep;
    Trie sub_trees[HISTORY_NUMBERS];
};

struct TrieTreeHolder {
    Trie trie;
    int return_code;
};

TrieHolder newTrie(void);

int edgeExists(Trie trie, int edge);

TrieHolder goDown(Trie trie, int edge);

void freeTrie(Trie trie);


#endif //MALE_TRIE_H
