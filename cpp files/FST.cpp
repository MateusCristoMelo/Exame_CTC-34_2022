#include "FST.h"
#include <stdbool.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>


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

std::vector<STATE *> *next_states(STATE *x, char letter) {

    std::vector<STATE *> *next_states = new std::vector<STATE *>();

    if(x->transitions != 0)
    {

        for(int i = 0; i < x->transitions ; ++i)
        {
            
            if(x->transitions_list[i]->letter == letter)
            {
                next_states->push_back(x->transitions_list[i]->next);
            };

        };

    }

    return next_states;

};

std::vector<STATE *> *previous_states(STATE *x, char letter) {

    std::vector<STATE *> *previous_states = new std::vector<STATE *>();

    if(x->reverse_transitions != 0)
    {
        for(int i = 0; i < x->reverse_transitions ; ++i)
        {
            if(x->reverse_transitions_list[i]->letter == letter)
            {
                previous_states->push_back(x->reverse_transitions_list[i]->next);
            };
        };
    }
    return previous_states;
}    

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
    t->end->index = -2;

    //************************************** UNDERSTAND HOW TO IMPLEMENT THE LIST OF STATES *****************************************

    int number_of_ascii = 50; // it could change 

    t->length = 0;

    t->states_list = (STATE **) malloc(number_of_ascii * (max_word_size + 1) * sizeof(STATE));

    return t;

};

bool is_final(FST *t, STATE *x) {
    return (x == t->end);
};

bool is_initial(FST *t, STATE *x) {
    return (x == t->begin);
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

std::vector<std::string> *create_list(int size) {
    
    std::vector<std::string> *l = new std::vector<std::string>(size);

    return l;

};

void add_list(std::vector<std::string> *list, char *word, int index) {
    std::string word_string = word;
    list->at(index) = word_string;
};

void clear_list(std::vector<std::string> *list) {
    list->clear();
};

// create FST using the file dictionary ***************************************************************************************

FST *create_fst(void) {

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

    std::vector<std::string> *input = create_list(dictionary_length);

    // loop through the dictionary list and adding each input word to input list and fst

    int input_length = 0;

    std::ifstream infile2("american-english");
    while(infile2 >> current_word)
    {
        int prefix_length = 0;
        int suffix_length = 0;

        // loop through the input list and find the word which has the prefix with the greatest length in commom to the current word (same for the suffix)
        for(int index = 0; index < input_length ; ++index)
        {
            std::string aux = input->at(index);
            int i = 0;
            int j = 0;

            while(i < current_word.length() && i < aux.length() - 1)
            {
                if(current_word[i] != aux[i]) {
                    break;
                }
                ++i;
            };
            while(j < current_word.length() && j < aux.length() - 1)
            {
                if(current_word[current_word.length() - 1 - j] != aux[aux.length() - 1 - j]) {
                    break;
                }
                ++j;
            };

            if(prefix_length < i)
                prefix_length = i;

            if(suffix_length < j)
                suffix_length = j;
        };

        // find the state which has the prefix with the greatest length in commom to the current word.
        // 

        int curr_weight = 0;
        STATE *nex;
        STATE *begin = transducer->begin;
        find_prefix_state(begin, current_word, prefix_length, 0, &curr_weight, &nex);
        int ind_prefix = prefix_length - 1;

        // find the state which has the suffix with the greatest length in commom to the current word.

        STATE *prev;
        STATE *end = transducer->end;
        find_suffix_state(end, current_word, suffix_length, current_word.length() - 1, &prev);
        int ind_suffix = current_word.length() - suffix_length;


        // construct the rest of the string from the prefix index state to the suffix index state

        for(int i = ind_prefix + 1; i < ind_suffix - 1; ++i)
        {
            STATE *aux = new_state(transducer, false);

            set_transition(nex, aux, current_word[i], 0);

            nex = aux;

        };
        
        if(nex != prev)
        {
            if(ind_suffix == ind_prefix + 1)
            {
                // if the word can be created by the concatenation of prefix and suffix but nex and prev are different, we cannot create a transition from nex to prev (there is no letter). So we go back to the previous state before nex and create a transition from it to prev or we go forward to the next state after prev and create a transition from nex to it.
                if(ind_prefix >= 0)
                {
                    std::vector<STATE *> *prev_states = previous_states(nex, current_word[ind_prefix]);
                    STATE *prev_state = prev_states->at(0);
                    set_transition(prev_state, prev, current_word[ind_prefix], 0);
                }
                else
                {
                    std::vector<STATE *> *next_states_ = next_states(prev, current_word[ind_suffix]);
                    STATE *next_state = next_states_->at(0);
                    set_transition(nex, next_state, current_word[ind_suffix], 0);
                }
            }
            else
            {
                set_transition(nex, prev, current_word[ind_suffix - 1], 0);
            }
        }

        // add the word to the input list

        char *word = new char[current_word.length() + 1];
        strcpy(word, current_word.c_str());

        add_list(input, word, input_length);
        ++input_length;

    };

    infile2.close();
    clear_list(input);

    return transducer;

};

bool find_prefix_state(STATE *state, std::string& word, int prefix_length, int ind_prefix, int *curr_weight, STATE **nex) {
    if(prefix_length == 0)
    {
        *nex = state;
        return true;
    }
    
    std::vector<STATE *> *states = next_states(state, word[ind_prefix]);

    for(int i = 0; i < states->size(); ++i)
    {
        STATE *aux = states->at(i);
        bool found = find_prefix_state(aux, word, prefix_length - 1, ind_prefix + 1, curr_weight, nex);
        if(found)
            return true;
    }
    return false;
}

bool find_suffix_state(STATE *state, std::string& word, int suffix_length, int ind_suffix, STATE **prev) {
    if(suffix_length == 0)
    {
        *prev = state;
        return true;
    }
    
    std::vector<STATE *> *states = previous_states(state, word[ind_suffix]);

    for(int i = 0; i < states->size(); ++i)
    {
        STATE *aux = states->at(i);
        bool found = find_suffix_state(aux, word, suffix_length - 1, ind_suffix - 1, prev);
        if(found)
            return true;
    }
    return false;
}

void print_words_with_prefix(std::string *prefix, FST *fst) {
    // Start at the beginning state of the FST
    std::vector<STATE *> *prefix_states = new std::vector<STATE *>();

    bool found = find_prefix_states(fst->begin, *prefix, prefix->length(), 0, prefix_states);

    if(!found)
    {
        std::cout << "No words with this prefix" << std::endl;
        return;
    }

    for(int i = 0; i < prefix_states->size(); ++i)
    {
        STATE *state = prefix_states->at(i);
        search_words(fst, state, "", prefix);
    }
}

bool find_prefix_states(STATE *state, std::string& word, int prefix_length, int ind_prefix, std::vector<STATE *> *prefix_states) {
    bool found = false;
    if(prefix_length == 0)
    {
        if(std::find(prefix_states->begin(), prefix_states->end(), state) == prefix_states->end())
            prefix_states->push_back(state);
        return true;
    }
    
    std::vector<STATE *> *states = next_states(state, word[ind_prefix]);

    for(int i = 0; i < states->size(); ++i)
    {
        STATE *aux = states->at(i);
        bool found_aux = find_prefix_states(aux, word, prefix_length - 1, ind_prefix + 1, prefix_states);
        if(found_aux)
            found = true;
    }
    return found;
}

void search_words(FST *fst, STATE *state, std::string prefix, std::string *search) {
    if (is_final(fst, state)) {
        std::cout << *search << prefix << std::endl;
    }

    for (int i = 0; i < state->transitions; ++i) {
        TRANST *transition = state->transitions_list[i];
        search_words(fst, transition->next, prefix + transition->letter, search);
    }
}