#ifndef FST_
#define FST_

typedef struct state STATE;
typedef struct transition TRANST;
typedef struct fst FST;
typedef struct list LIST;

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

LIST *create_list(int max_words, int max_word_size);

void add_list(LIST *list, char *word);

void clear_list(LIST *list);

FST *create_fst(void);

#endif