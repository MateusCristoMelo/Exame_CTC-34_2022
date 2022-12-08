import matplotlib.pyplot as plt
import time

def main():
    filename = 'american-english'
    with open(filename, 'r') as f:
        input_list = f.read().splitlines()

    # compare time to create fst and hash table and count the total number of edges
    # FST **************************************************************************
    start_time = time.time()
    # FST executable: create_fst()
    fst_creation_time = (time.time() - start_time)
    fst_edges = 10

    # Hash **************************************************************************
    start_time = time.time()
    # Hash executable: create_hash_table()
    hash_creation_time = (time.time() - start_time)
    hash_edges = 10


    filename = 'test-list'
    with open(filename, 'r') as f:
        test_list = f.read().splitlines()
    
    autocomplete_time_list = []
    fst_time_list = []
    hash_time_list = []
    word_index = 0

    # compare time to execute exact searches, in fst measure the autocomplete time too
    for word in test_list:
        # call FST exact match tests with a specific range of words to compare with hash table

        # FST time **************************************************************************
        # exact match time

        start_time = time.time()
        # FST executable
        fst_time = time.time() - start_time
        fst_time_list.append(fst_time)
        print("--- %s seconds ---\n" % fst_time)

        # autocomplete time

        start_time = time.time()
        # FST executable
        autocomplete_time = time.time() - start_time
        autocomplete_time_list.append(autocomplete_time)
        print("--- %s seconds ---\n" % autocomplete_time)

        # Hash time **************************************************************************
        # only exact match

        start_time = time.time()
        # Hash executable
        hash_time = time.time() - start_time
        hash_time_list.append(hash_time)
        print("--- %s seconds ---\n" % hash_time)



        word_index = word_index + 1

    # plot comparable charts
    plt.plot(autocomplete_time_list, fst_time_list)
    plt.title('FST: with vs. without autocomplete')
    plt.xlabel('Autocomplete')
    plt.ylabel('Exact search')
    plt.show()
    plt.savefig('autocomplete_tests.png')

    print("Chart with time comparison between autocomplete and exact search (using only FST) ploted.\n")

    plt.plot(fst_time_list, hash_time_list)
    plt.title('Exact search: FST vs. Hash')
    plt.xlabel('FST')
    plt.ylabel('Hash')
    plt.show()
    plt.savefig('autocomplete_tests.png')

    print("Chart with time comparison (of exact search) between FST and Hash Table ploted.\n\n")

    # show creation time
    print("Creation times:\n")
    print("FST -> %s seconds" % fst_creation_time)
    print("Hash Table -> %s seconds" % hash_creation_time)

    # show the number of edges
    print("Number of edges:\n")
    print("FST -> %s edges" % fst_edges)
    print("Hash Table -> %s edges" % hash_edges)

    # show the memory cost
    print("Memory cost:\n")
    fst_memory = 855260
    print("FST -> %sK" % fst_memory)
    hash_memory = 10
    print("Hash Table -> %s" % hash_memory)