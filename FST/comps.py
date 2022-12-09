import matplotlib.pyplot as plt
import time
import math
import csv
from fst import create_minimal_transducer, search_fst
from hash_table import create_hash_table, search_hash_table

def main():
    start_file_time = time.time()

    filename = 'american-english'
    with open(filename, 'r') as f:
        input_list = f.read().splitlines()
        max_word_length = max(len(word) for word in input_list)

    # compare time to create fst and hash table and count the total number of edges
    # FST **************************************************************************
    start_time = time.time()
    minimal_transducer_states_dict, initial_state = create_minimal_transducer(max_word_length, filename)
    fst_creation_time = (time.time() - start_time)
    fst_edges = len(minimal_transducer_states_dict)

    # Hash **************************************************************************
    start_time = time.time()
    hash_dict = create_hash_table()
    hash_creation_time = (time.time() - start_time)
    hash_edges = len(hash_dict)

    filename = 'test-list'
    with open(filename, 'r') as f:
        test_list = f.read().splitlines()

    autocomplete_time_list_avg = [[] for _ in range(5)]
    fst_time_list_avg = [[] for _ in range(5)]
    hash_time_list_avg = [[] for _ in range(5)]
    
    autocomplete_time_list = [0 for _ in range(5)]
    fst_time_list = [0 for _ in range(5)]
    hash_time_list = [0 for _ in range(5)]

    word_index = 0

    # compare time to execute exact searches, in fst measure the autocomplete time too
    for word in test_list:
        word_len = math.floor(len(word) / 2)

        if word_len > 5:
            word_len = 4
        elif word_len < 1:
            word_len = 0
        else:
            word_len -= 1

        # call FST exact match tests with a specific range of words to compare with hash table

        # FST time **************************************************************************
        # exact match time

        start_time = time.time()
        search_fst(minimal_transducer_states_dict, initial_state, word, True)
        fst_time = time.time() - start_time
        fst_time_list_avg[word_len].append(float(fst_time))
        print("FST exact time of %s: %s seconds" % (word, fst_time))

        # autocomplete time

        start_time = time.time()
        search_fst(minimal_transducer_states_dict, initial_state, word, False)
        autocomplete_time = time.time() - start_time
        autocomplete_time_list_avg[word_len].append(float(autocomplete_time))
        print("FST autocomplete time of %s: %s seconds" % (word, autocomplete_time))

        # Hash time **************************************************************************
        # only exact match

        start_time = time.time()
        search_hash_table(hash_dict, word)
        hash_time = time.time() - start_time
        hash_time_list_avg[word_len].append(float(hash_time))
        print("Hash exact time of %s: %s seconds" % (word, hash_time))

        word_index += 1

        print("Word %s processed\n" % word_index)

    size_list = [0 for _ in range(5)]

    for index in range(len(size_list)):
        size_list[index] = 2*index

    for length in range(5):
        size = len(autocomplete_time_list_avg[length])
        sum = 0
        for i in range(size):
            sum += autocomplete_time_list_avg[length][i]
        autocomplete_time_list[length] = sum / size
        
        size = len(fst_time_list_avg[length])
        sum = 0
        for i in range(size):
            sum += fst_time_list_avg[length][i]
        fst_time_list[length] = sum / size
        
        size = len(hash_time_list_avg[length])
        sum = 0
        for i in range(size):
            sum += hash_time_list_avg[length][i]
        hash_time_list[length] = sum / size

    csv_rowlist = []
    for index in range(len(size_list)):
        csv_rowlist.append([size_list[index], hash_time_list[index], fst_time_list[index], autocomplete_time_list[index]])

    with open('comps.csv', mode='w') as file:
        csv_writer = csv.writer(file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        csv_writer.writerow(csv_rowlist)

    # plot comparable charts
    plt.figure()
    plt.plot(size_list, fst_time_list, label="Exact time")
    plt.plot(size_list, autocomplete_time_list, label="Autocomplete")
    plt.title('FST: with vs. wit5hout autocomplete')
    plt.xlabel('Word size (n)')
    plt.ylabel('Time (s)')
    plt.legend(loc="upper left")
    plt.savefig('autocomplete_tests.png')

    print("Chart with time comparison between autocomplete and exact search (using only FST) ploted.\n")

    plt.figure()
    plt.plot(size_list, fst_time_list, label="FST")
    plt.plot(size_list, hash_time_list, label="Hash")
    plt.title('Exact search: FST vs. Hash')
    plt.xlabel('Word size (n)')
    plt.ylabel('Time (s)')
    plt.legend(loc="upper left")
    plt.savefig('comps_tests.png')

    print("Chart with time comparison (of exact search) between FST and Hash Table ploted.\n\n")

    # show creation time
    print("Creation times:")
    print("FST -> %s seconds" % fst_creation_time)
    print("Hash Table -> %s seconds\n" % hash_creation_time)

    # show the number of edges
    print("Number of states:")
    print("FST -> %s states" % fst_edges)
    print("Hash Table -> %s lines\n" % hash_edges)

    # show the memory cost
    print("Memory cost:")
    fst_memory = 855260
    print("FST -> %sK" % fst_memory)
    hash_memory = 11936
    print("Hash Table -> %sK\n" % hash_memory)

    print("--- Exec time of all tests: %s seconds ---\n" % (time.time() - start_file_time))

    plt.show()

if __name__ == "__main__":
    main()