/***********************************/
/* CTC-34: FST auto complete of the english language */
/***********************************/

#include "FST.h"
#include <stdio.h>

int main() {
    // create a fst from the outside file
    FST *current_fst = create_fst();

    STATE *s = current_fst->begin;

    // Prompt the user to enter letters
    std::cout << "Enter some letters: ";

    // Keep prompting the user to enter letters until they enter the '#' character
    std::string prefix;
    while (std::cin >> prefix) {
        if (prefix == "#") {
            break;
        }

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

        // Print the matching words
        std::cout << "Matching words: " << std::endl;
        for (const std::string& word : words) {
            std::cout << word << std::endl;
        }

        // Prompt the user to enter another set of letters
        std::cout << "Enter some letters: ";
    }

    // substring read from terminal char by char
    while(scanf("%c", a))
    {
        // get the output from a given state in the transducer

        

        

    };

    // print the FST and save in an outside file
    // some function that receives the current fst and print it

    // destroy fst;
    clear_transducer(current_fst);

}