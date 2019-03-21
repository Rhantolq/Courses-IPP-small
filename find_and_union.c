//
// Created by rober on 16.03.2019.
//

#include "find_and_union.h"
#include <stdlib.h>
#include <stdint.h>

// Allocates memory for new find and union structure representative
UnionHolder newRepresentative(uint64_t value) {
    UnionHolder ret;
    ret.representative = malloc(sizeof(struct UnionRepresentative));
    if (ret.representative == NULL) {
        ret.return_code = ALLOCATION_FAILURE;
        return ret;
    }
    ret.representative->val = value;
    ret.representative->rep = ret.representative;
    ret.representative->is_vacant = false;
    ret.representative->number_of_predecessors = 0;
    ret.return_code = ALLOCATION_SUCCESS;
    return ret;
}

// frees up space allocated by {representative}
void freeRepresentative(Representative representative) {
    free(representative);
}

// returns the representative if a given union
// frees up allocated memory if possible
Representative find(Representative representative, int nodes_deleted) {
    if (representative->rep == representative) {
        if (nodes_deleted == representative->number_of_predecessors && representative->is_vacant) {
            freeRepresentative(representative);
            return NULL;
        }

        else if (nodes_deleted != FIRST_IN_FIND_FLAG)
            representative->number_of_predecessors -= nodes_deleted;
        return representative;
    }

    //the node that calls find function cannot be vacant if it is
    //not called from the markAsVacant function
    if (representative->is_vacant) {
        Representative new_rep = find(representative->rep, nodes_deleted + 1);
        freeRepresentative(representative);
        return new_rep;
    }

    else {
        //FIRST_IN_FIND_FLAG is a guard against
        //unwanted change of number_of_predecessors
        //it is a default find argument for the
        //first find call
        Representative new_rep;
        if (nodes_deleted == FIRST_IN_FIND_FLAG) {
            new_rep = find(representative->rep, 0);
        }
        else {
            new_rep = find(representative->rep, nodes_deleted);
            representative->number_of_predecessors = 0;
        }
        representative->rep = new_rep;
        return new_rep;
    }
}

// Creates a union between {rep1} and {rep2}
// handles value as specified by command EQUAL
void unify(Representative rep1, Representative rep2) {
    rep1 = find(rep1, FIRST_IN_FIND_FLAG);
    rep2 = find(rep2, FIRST_IN_FIND_FLAG);

    if(rep1 == rep2)
        return;

    if (rep1->number_of_predecessors < rep2->number_of_predecessors) {
        rep1->rep = rep2;
        rep2->number_of_predecessors += rep1->number_of_predecessors + 1;

        uint64_t new_val = 0;
        if (rep2->val % 2 == 1 && rep1->val %2 == 1)
            new_val = 1;
        if (rep1->val == 0 || rep2->val == 0)
            rep2->val = ((rep1->val > rep2->val) ? rep1->val : rep2->val);
        else
            rep2->val = new_val + (rep2->val / 2) + (rep1->val / 2);
    }

    else {
        rep2->rep = rep1;
        rep1->number_of_predecessors += rep2->number_of_predecessors + 1;

        uint64_t new_val = 0;
        if (rep2->val % 2 == 1 && rep1->val %2 == 1)
            new_val = 1;
        if (rep1->val == 0 || rep2->val == 0)
            rep1->val = ((rep1->val > rep2->val) ? rep1->val : rep2->val);
        else
            rep1->val = new_val + (rep2->val / 2) + (rep1->val / 2);
    }
}

// Marks {rep} to be deleted
void markAsVacant(Representative rep) {
    if (rep->number_of_predecessors == 0) {
        rep->is_vacant = true;
        find(rep, 0); //removing any other vacant objects
    }
    else {
        rep->is_vacant = true;
    }
}