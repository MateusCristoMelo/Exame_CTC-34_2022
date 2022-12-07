#include "FST.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Create the FST from the file "american-english"
    FST *fst = create_fst();

    // Keep watching the input for new letters
    std::cout << "Enter a prefix to search for matching words: ";
    std::string prefix;
    std::cin >> prefix;

    // Initialize the current state as the initial state of the FST
    STATE *current_state = fst->begin;

    // Traverse the FST using the letters of the prefix
    for (char c : prefix) {
        current_state = next_state(current_state, c);
        if (current_state == nullptr) {
            // If the prefix is not found in the FST, there are no matching words
            std::cout << "No matching words found for prefix '" << prefix << "'" << std::endl;
            return 0;
        }
    }

    // Print the matching words for the prefix
    std::cout << "Matching words for prefix '" << prefix << "':" << std::endl;
    for (int i = 0; i < current_state->transitions; i++) {
        // Traverse the transitions of the current state to find the matching words
        TRANST *transition = current_state->transitions_list[i];
        // If the transition leads to a final state, it represents a matching word
        if (is_final(fst, transition->next)) {
            std::cout << transition->letter << std::endl;
        }
    }

    return 0;
}