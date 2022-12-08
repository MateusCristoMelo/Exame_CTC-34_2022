def create_hash_table():
    word_dict = {}
    filename = 'american-english'
    with open(filename, "r") as input_list:
        i = 0
        for line in input_list:
            current_word = line.split('\n')[0]
            word_dict[current_word] = i
            i += 1
    return word_dict

def search_hash_table(word_dict, word):
    return word in word_dict

def main():
    word_dict = create_hash_table()
    search = input("Enter a word to search: ")
    if search_hash_table(word_dict, search):
        print("The word is in the dictionary.")
    else:
        print("The word is not in the dictionary.")

if __name__ == "__main__":
    main()