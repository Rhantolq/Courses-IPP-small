//
// Created by rober on 17.03.2019.
//

#ifndef MALE_LOGIC_H
#define MALE_LOGIC_H

#include <stdlib.h>
#include "trie.h"
#include "charvector.h"


TrieHolder declareHistory(const char* history, TrieHolder trie_holder);

int removeHistory(const char* history, TrieHolder trie_holder);

int validHistory(const char* history, TrieHolder trie_holder);

int energy(char* history, TrieHolder trie_holder);

int commandLineCheck(const char* vector, int start, const char* command);

int parseInput(CharVector vector, TrieHolder trie_holder);

#endif //MALE_LOGIC_H
