#ifndef FST_
#define FST_

#include <vector>
#include <string>

typedef struct state STATE;
typedef struct transition TRANST;
typedef struct fst FST;

typedef struct state
{
    int index;
    //transition list have an upper bound as a maximum amount of letters using in english language following the ASCII table
    int transitions;
    TRANST **transitions_list;
    //just make another array to store all the transitions this state is receiving, the next in these transitions is actually the previous
    // the reverse transitions ARENT THE SAME of the transitions, so they are a different object, just a copy, but not the same pointer
    int reverse_transitions;
    TRANST **reverse_transitions_list;
} STATE;

typedef struct transition
{
    char letter;
    int weight;
    STATE *next;
} TRANST;

typedef struct fst
{
    STATE *begin;
    STATE *end;
    int length;
    STATE **states_list;
} FST;

STATE *new_state(FST *t, bool bounds);

void clear_state(STATE *x);

STATE *next_state(STATE *x, char letter);

STATE *previous_state(STATE *x, char letter);

TRANST *new_transition(STATE *next, char letter, int weight);

void set_transition(STATE *from, STATE *next, char letter, int weight);

void erase_transition(TRANST *a);

void set_weight(STATE *x, STATE *next, int new_weight);

FST *new_transducer(int dictionary_length, int max_word_size);

bool is_final(FST *t, STATE *x);

bool is_initial(FST *t, STATE *x);

void insert_transducer(FST *t, STATE *x);

void clear_transducer(FST *t);

std::vector<std::string> *create_list(int max_words, int max_word_size);

void add_list(std::vector<std::string> *list, char *word);

void clear_list(std::vector<std::string> *list);

FST *create_fst(void);

void print_words_with_prefix(FST *fst, std::string prefix);

std::string get_word(FST *fst, STATE *state);

#endif