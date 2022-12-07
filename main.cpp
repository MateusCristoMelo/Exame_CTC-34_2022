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

    // Print the words that begin with the given prefix
    print_words_with_prefix(fst, prefix);

    return 0;
}