#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>

// Create a struct to represent a word and its frequency in the dictionary
struct Word {
  std::string word;
  int frequency;
};

// Create a hash function to map a string to an integer
struct Hash {
  // Use the sum of the ASCII values of each character in the string as the hash value
  std::size_t operator()(const std::string& s) const {
    std::size_t hash = 0;
    for (const char& c : s) {
      hash += c;
    }
    return hash;
  }
};

// Create a function to find words that begin with a given prefix
std::vector<std::string> autocomplete(const std::unordered_map<std::string, Word, Hash>& dictionary, const std::string& prefix) {
  // Create a vector of strings to store the matching words
  std::vector<std::string> matching_words;

  // Traverse the dictionary and find words that begin with the given prefix
  for (const auto& word : dictionary) {
    if (word.first.find(prefix) == 0) {
      // If the word begins with the given prefix, add it to the matching_words vector
      matching_words.push_back(word.first);
    }
  }

  // Return the vector of matching words
  return matching_words;
}


int main() {
  // Create a hash table to store the words and their frequencies
  std::unordered_map<std::string, Word, Hash> dictionary;

  // Read the list of words from the 'american-english' file
  std::ifstream infile("american-english");
  std::string word;
  while (infile >> word) {
    // Insert each word into the dictionary, or increment its frequency if it already exists
    if (dictionary.find(word) == dictionary.end()) {
      dictionary[word] = { word, 1 };
    } else {
      dictionary[word].frequency++;
    }
  }
  infile.close();

  // Prompt the user to enter letters
  std::cout << "Enter some letters: ";

  // Keep prompting the user to enter letters until they enter the '#' character
  std::string prefix;
  while (std::cin >> prefix) {
    if (prefix == "#") {
      break;
    }

    // Use the 'autocomplete' function to find words that begin with the entered letters
    std::vector<std::string> words = autocomplete(dictionary, prefix);

    // Print the matching words
    std::cout << "Matching words: " << std::endl;
    for (const std::string& word : words) {
      std::cout << word << std::endl;
    }

    // Prompt the user to enter another set of letters
    std::cout << "Enter some letters: ";
  }

  // Clear the dictionary and exit the program
  dictionary.clear();
  return 0;
}