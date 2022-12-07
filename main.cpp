/***********************************/
/* CTC-34: FST auto complete of the english language */
/***********************************/

#include "FST.h"

int main() {
    // read the sorted list of english words
    // vector dictionary = *** function which reads an outside file***;

    // create a fst
    FST *current_fst = create_fst(dictionary);

    // print the FST and save in an outside file
    // some function that receives the current fst and print it

    // destroy fst;
    clear_transducer(current_fst);

}