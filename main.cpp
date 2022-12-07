/***********************************/
/* CTC-34: FST auto complete of the english language */
/***********************************/

#include "FST.h"
#include <stdio.h>

int main() {
    // create a fst from the outside file
    FST *current_fst = create_fst();

    /*
    // prepare the code for read substrings
    char a = 's';

    STATE *s = current_fst->begin;

    // substring read from terminal char by char
    while(scanf("%c", a))
    {
        // get the output from a given state in the transducer

        s = next_state(s, a);

        if(s == nullptr || is_final(current_fst, s))
        {
            
            printf("there's no words in dictionary anymore.\n");

        }
        else
        {
            // take a limit to the search in the FST
            int search_limit = 50;

            STATE *x = s;

            for(int i = 0; i < search_limit ; ++i)
            {
                
                for(int j = 0; j < x->transitions ; ++j)
                {
                    
                    x->transitions_list[j]->next;
                    char c_aux = x->transitions_list[j]->letter;

                };
                
                char c_aux = x->transitions;

                while(x != nullptr || !is_final(current_fst, x))
                {

                };

            };
        };

    };

    */

    // print the FST and save in an outside file
    // some function that receives the current fst and print it

    // destroy fst;
    clear_transducer(current_fst);

}