#include "FST.h"
#include <stdbool.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>



// transducer states ****************************************************************************************************************

STATE *new_state(FST *t, bool bounds) {

    STATE *p = (STATE *) malloc(sizeof(STATE));

    p->transitions = 0;

    //************************************** UNDERSTAND HOW TO IMPLEMENT THE LIST OF STATES *****************************************

    int number_of_ascii = 50; // it could change 

    p->transitions_list = (TRANST **) malloc(number_of_ascii * sizeof(TRANST));

    // make a copy of transition and use to make the reverse transitions list

    p->reverse_transitions = 0;

    p->reverse_transitions_list = (TRANST **) malloc(number_of_ascii * sizeof(TRANST));

    p->index = -1;
    
    if(!bounds)
    {

        // insert the new state in the fst

        int ind = t->length;

        p->index = ind;

        insert_transducer(t, p);

    }

    return p;

};

// dont need worry about the total transitions because the list of transitions of each state is frozen after the inputs, so no changes
void clear_state(STATE *x) {

    int size = x->transitions;
    
    for(int i = 0; i < size ; ++i)
    {

        if(x->transitions_list[i] != NULL)
        {
            erase_transition(x->transitions_list[i]);
            --(x->transitions);
        };

    };

    int reverse_size = x->reverse_transitions;
    
    for(int i = 0; i < reverse_size ; ++i)
    {

        if(x->reverse_transitions_list[i] != NULL)
        {
            erase_transition(x->reverse_transitions_list[i]);
            --(x->reverse_transitions);
        };

    };

    free(x);

};

// operations *********************************************************************************************************************

STATE *next_state(STATE *x, char letter) {
    
    STATE *next = nullptr;

    if(x->transitions != 0)
    {

        for(int i = 0; i < x->transitions ; ++i)
        {
            
            if(x->transitions_list[i]->letter == letter)
            {
                return x->transitions_list[i]->next;
            };

        };

    }
    
    return next;

};

STATE *previous_state(STATE *x, char letter) {
    
    STATE *previous = nullptr;

    if(x->reverse_transitions != 0)
    {

        for(int i = 0; i < x->reverse_transitions ; ++i)
        {
            
            if(x->reverse_transitions_list[i]->letter == letter)
            {
                // in this case, the next is actually the previous
                return x->reverse_transitions_list[i]->next;
            };

        };

    }
    
    return previous;

};

// transitions *********************************************************************************************************************

TRANST *new_transition(STATE *next, char letter, int weight) {

    TRANST *a = (TRANST *) malloc(sizeof(TRANST));

    a->letter = letter;
    a->weight = weight;
    a->next = next;

    return a;

};

void set_transition(STATE *from, STATE *next, char letter, int weight) {
    
    TRANST *a = new_transition(next, letter, weight);

    int size = from->transitions;

    from->transitions = size + 1;

    from->transitions_list[size] = a;

    // set the reverse transition in next (to x) too
    TRANST *reverse_a = new_transition(from, letter, weight);

    int reverse_size = next->reverse_transitions;

    next->reverse_transitions = reverse_size + 1;

    next->reverse_transitions_list[reverse_size] = reverse_a;

};

void erase_transition(TRANST *a) {
    
    free(a);

};

// weigths ************************************************************************************************************************

void set_weight(STATE *x, STATE *next, int new_weight) {
    
    for(int i = 0; i < x->transitions ; ++i)
    {
        
        if(x->transitions_list[i]->next == next)
        {
            x->transitions_list[i]->weight = new_weight;
        };

    };

    // set the weight on reverse transition too

    for(int i = 0; i < next->reverse_transitions ; ++i)
    {
        
        if(next->reverse_transitions_list[i]->next == x)
        {
            next->reverse_transitions_list[i]->weight = new_weight;
        };

    };

};

// transducer *********************************************************************************************************************

FST *new_transducer(int dictionary_length, int max_word_size) {
    
    FST *t = (FST *) malloc(sizeof(FST));

    STATE *begin_aux = new_state(t, true);
    t->begin = begin_aux;

    STATE *end_aux = new_state(t, true);
    t->end = end_aux;

    //************************************** UNDERSTAND HOW TO IMPLEMENT THE LIST OF STATES *****************************************

    int number_of_ascii = 50; // it could change 

    t->length = 0;

    t->states_list = (STATE **) malloc(number_of_ascii * (max_word_size + 1) * sizeof(STATE));

    return t;

};

bool is_final(FST *t, STATE *x) {
    
    if(x == t->end)
    {
        
        return true;
    
    };

    return false;

};

bool is_initial(FST *t, STATE *x) {
    
    if(x == t->begin)
    {
        
        return true;
    
    };

    return false;

};

void insert_transducer(FST *t, STATE *x) {

    int size = t->length;
    
    t->length = size + 1;

    t->states_list[size] = x;

};

void clear_transducer(FST *t) {

    int size = t->length;
    
    for(int i = 0; i < size ; ++i)
    {

        if(t->states_list[i] != NULL)
        {
            clear_state(t->states_list[i]);
            --(t->length);
        };

    };

    clear_state(t->begin);
    clear_state(t->end);

    free(t);

};

// list ***********************************************************************************************************************

LIST *create_list(int max_words, int max_word_size) {
    
    LIST *l = (LIST *) malloc(sizeof(LIST));

    l->length = 0;

    //************************************** NEED TO PASS THE MAXIMUM LENGHT **************************************************
    
    l->words_list = (char **) malloc(max_words * (max_word_size + 1) * sizeof(char));

    return l;

};

void add_list(LIST *list, char *word) {

    int size = list->length;
    
    list->length = size + 1;

    list->words_list[size] = word;

};

void clear_list(LIST *list) {

    int size = list->length;
    
    for(int i = 0; i < size ; ++i)
    {

        if(list->words_list[i] != NULL)
        {
            free(list->words_list[i]);
            --(list->length);
        };

    };

    free(list);

};

// create FST using the file dictionary ***************************************************************************************

FST *create_fst(void) {

    // get the length of words already in the fst
    int dictionary_length = 0;

    int max_word_size = 0;

    // read words from 'american-english' file
    std::ifstream infile("american-english");
    std::string current_word;
    while (infile >> current_word) {
        if(current_word.length() > max_word_size)
        {

            max_word_size = current_word.length();

        };

        ++dictionary_length;
    }
    infile.close();

    // create transducer

    FST *transducer = new_transducer(dictionary_length, max_word_size);

    // create input list;

    LIST *input = create_list(dictionary_length, max_word_size);

    // loop through the dictionary list and adding each input word to input list and fst

    int input_length = 0;

    std::ifstream infile2("american-english");
    while(infile2 >> current_word)
    {
        ++input_length;

        int prefix_length = 0;

        // idea to have a FST with better shape is to find in EACH state the word which has the same preffix
        // the same we could do to suffix search: not find only in the final state, but also in every state and make a transition after

        // loop through the input list and find the word which has the prefix with the greatest lenght in commom to the current word
        for(int index = 0; index < input_length ; ++index)
        {

            std::string aux = input->words_list[index];

            int i = 0;

            while(i < current_word.length())
            {
                
                if(current_word[i] != aux[i])
                {

                    break;

                };

                ++i;

            };

            if(prefix_length < i)
            {

                prefix_length = i;

            };

        };

        // find a prefix using the FST beggining in the final state
        STATE *nex = transducer->begin;

        int curr_weight = 0;

        int ind_prefix = 0;

        while(ind_prefix < prefix_length)
        {
            curr_weight = nex->transitions_list[current_word[ind_prefix]]->weight + curr_weight;

            STATE *nex_aux = next_state(nex, current_word[ind_prefix]);

            if(nex_aux == nullptr || is_final(transducer, nex_aux))
            {

                break;

            };

            nex = nex_aux;

            ++ind_prefix;

        };

        // find a suffix using the FST beggining in the final state
        STATE *prev = transducer->end;

        int ind_suffix = current_word.length();

        while(ind_suffix > prefix_length)
        {
            
            STATE *prev_aux = previous_state(prev, current_word[ind_suffix]);

            if(prev_aux == nullptr || is_initial(transducer, prev_aux))
            {

                break;

            };

            prev = prev_aux;

            --ind_suffix;

        };

        // construct the rest of the string from the prefix index state to the suffix index state

        int n = ind_prefix;

        while(n < (ind_suffix - 1))
        {
            if(n == ind_prefix)
            {
                curr_weight = input_length;
            }
            else
            {
                curr_weight = 0;
            }
            
            STATE *aux = new_state(transducer, false);
            
            set_transition(nex, aux, current_word[n], curr_weight);

            nex = aux;

            ++n;

        };

        set_transition(nex, prev, current_word[n], 0);

        char *word = new char[current_word.length() + 1];
        strcpy(word, current_word.c_str());

        add_list(input, word);

    };

    infile2.close();
    clear_list(input);

    return transducer;

};