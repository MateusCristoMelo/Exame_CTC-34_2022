import keyboard
import string
import sys

def create_minimal_transducer(max_word_length, filename):
    minimal_transducer_states_dict = {}
    temp_states = [None] * (max_word_length + 1)
    temp_states[0] = State(0)
    initial_state = temp_states[0]
    previous_word = ""
    n_states = [1]
    with open(filename, "r") as input_list:
        for line in input_list:
            current_word = line.split('\n')[0]
            # Calculate the length of the longest common prefix of current_word and previous_word
            i = 0
            while i < len(current_word) and i < len(previous_word) and previous_word[i] == current_word[i]:
                i += 1
            prefix_length_plus_1 = i + 1
            # Minimize the states from the suffix of the previous word
            for i in range(len(previous_word), prefix_length_plus_1 - 1, -1):
                temp_states[i-1].transition[previous_word[i-1]] = find_minimized(temp_states[i], minimal_transducer_states_dict, n_states)
            # Initialize the tail states for the current word
            for i in range(prefix_length_plus_1, len(current_word) + 1):
                temp_states[i] = State(n_states[0])
                n_states[0] += 1
                temp_states[i-1].transition[current_word[i-1]] = temp_states[i]
            if current_word != previous_word:
                temp_states[len(current_word)].final = True
            previous_word = current_word
        # Minimize the states of the last word
        for i in range(len(current_word), 0, -1):
            temp_states[i-1].transition[previous_word[i-1]] = find_minimized(temp_states[i], minimal_transducer_states_dict, n_states)
        initial_state = find_minimized(temp_states[0], minimal_transducer_states_dict, n_states)
        return minimal_transducer_states_dict, initial_state

def find_minimized(state, minimal_transducer_states_dict, n_states):
    """Returns an equivalent state from the dictionary. If not present, inserts a copy of the parameter to the dictionary and returns it."""
    # to check if a state is in the minimal_transducer_states_dict, we have to check if their transitions are equivalent and if their output are equivalent
    # we can't use the == operator because it checks if the objects are the same, not if they are equivalent
    for state_id, state_in_dict in minimal_transducer_states_dict.items():
        if state.transition == state_in_dict.transition and state.final == state_in_dict.final:
            return state_in_dict
    new_state = copy_state(state, state.index)
    minimal_transducer_states_dict[new_state.index] = new_state
    return new_state

class State:
    def __init__(self, index = -1):
        self.index = index
        self.final = False
        self.transition = {}

def copy_state(state, index):
    """Copies a state to a new one."""
    new_state = State(index)
    new_state.final = state.final
    new_state.transition = dict(state.transition)
    return new_state

def print_fst(states_dict, initial_state):
    print("Initial state:", initial_state.index)
    print()
    for state_id, state in states_dict.items():
        print("State", state.index)
        print("Final:", state.final)
        print("Transitions:")
        for char, next_state in state.transition.items():
            print("  ", char, "->", next_state.index)
        print()

def complete_prefix(n, states, state, word, words):
    if len(words) >= n:
        return True
    if state.final:
        words.append(word)
    for letter in state.transition:
        finish = complete_prefix(n, states, state.transition[letter], word+letter, words)
        if finish:
            return True
    return False

def autocomplete(minimal_transducer_states_dict, initial_state):
    # Wait for the user to type a letter
    # When they type, prompt the matching words from the input dictionary (do not need enter press)
    # When they type another, do that again, and so on

    state = initial_state
    word = ""
    print(">>> ", end='', flush=True)
    while True:
        event = keyboard.read_event()
        if event.event_type == 'down':
            if event.name in ('space', 'enter'):
                word = ''
                print("\n>>> ", end='', flush=True)
                state = initial_state
            elif event.name == 'backspace':
                word = word[:-1]
                state = initial_state
                print('\b', end='', flush=True)
                for letter in word:
                    state = state.transition[letter]
            elif event.name in state.transition:
                word += event.name
                print(event.name, flush=True)
                state = state.transition[event.name]
                words = []
                complete_prefix(5, minimal_transducer_states_dict, state, word, words)
                for w in words:
                    print('    ' + w)
                print(">>> " + word, end='', flush=True)
            elif event.name == 'esc':
                break
            elif event.name in string.printable:
                print(event.name, flush=True)
                print("No matching words")
                print(">>> " + word, end='', flush=True)

def main():
    filename = 'american-english'
    with open(filename, 'r') as f:
        input_list = f.read().splitlines()
        max_word_length = max(len(word) for word in input_list)

    minimal_transducer_states_dict, initial_state = create_minimal_transducer(max_word_length, filename)
    
    autocomplete(minimal_transducer_states_dict, initial_state)

if __name__ == '__main__':
    main()
