#include "FST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

int main() {
    // Create the FST from the file "american-english"
    FST *fst = create_fst();

    std::cout << "Enter a prefix to search for matching words: ";
    std::string prefix;
    std::cin >> prefix;

    // Print the words that begin with the given prefix
    print_words_with_prefix(prefix, fst);

    return 0;
}