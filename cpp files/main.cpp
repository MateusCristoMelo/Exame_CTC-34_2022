#include "FST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

void print_fst(FST *fst);

int main() {
    // Create the FST from the file "american-english"
    FST *fst = create_fst();

    print_fst(fst);

    std::cout << "Enter a prefix to search for matching words: ";
    std::string prefix;
    std::cin >> prefix;

    // Print the words that begin with the given prefix
    print_words_with_prefix(&prefix, fst);

    return 0;
}

void print_fst(FST *fst) {
    // Print the starting and ending states
    std::cout << "Starting state: " << fst->begin->index << std::endl;
    std::cout << "Ending state: " << fst->end->index << std::endl;

    // Print the states and their transitions
    STATE *state = fst->begin;
    std::cout << "State " << state->index << ": ";
    for (int j = 0; j < state->transitions; ++j) {
        TRANST *transition = state->transitions_list[j];
        std::cout << transition->letter << "(" << transition->weight << ") -> " << transition->next->index << "  ";
    }
    std::cout << std::endl;

    state = fst->end;
    std::cout << "State " << state->index << ": ";
    for (int j = 0; j < state->transitions; ++j) {
        TRANST *transition = state->transitions_list[j];
        std::cout << transition->letter << "(" << transition->weight << ") -> " << transition->next->index << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < fst->length; ++i) {
        state = fst->states_list[i];
        std::cout << "State " << state->index << ": ";
        for (int j = 0; j < state->transitions; ++j) {
            TRANST *transition = state->transitions_list[j];
            std::cout << transition->letter << "(" << transition->weight << ") -> " << transition->next->index << "  ";
        }
        std::cout << std::endl;
    }
}