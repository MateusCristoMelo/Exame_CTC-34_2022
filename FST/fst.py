import keyboard
import string

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

def autocomplete_test(minimal_transducer_states_dict, initial_state, word_input):

    state = initial_state
    word = ""
    for char in word_input:
        if char in state.transition:
            word += char
            state = state.transition[char]
            words = []
            if state.final and char == word_input[-1:]:
                print("The word %s is in the dictionary." % word)
        elif char in string.printable:
            print(char, flush=True)
            print("No matching words")

def search_fst(minimal_transducer_states_dict, initial_state, word, is_exact):

    if is_exact:
        current_state = initial_state
        for c in word:
            flag_pass = False
            for char, next_state in current_state.transition.items():
                if c == char:
                    flag_pass = True
                    current_state = next_state
            if not flag_pass:
                break

        if current_state.final is True:
            print("The word %s is in the dictionary." % word)
        else:
            print("The word %s is not in the dictionary." % word)

    else:
        autocomplete_test(minimal_transducer_states_dict, initial_state, word)

def save_fst(minimal_transducer_states_dict, initial_state):
    # save the fst to json
    with open('fst.json', 'w') as outfile:
        json_dict = {}
        json_dict['initial_state'] = initial_state.index
        json_dict['states'] = {}
        for state_id, state in minimal_transducer_states_dict.items():
            json_dict['states'][state.index] = {}
            json_dict['states'][state.index]['final'] = state.final
            json_dict['states'][state.index]['transitions'] = {}
            for t in state.transition:
                json_dict['states'][state.index]['transitions'][t] = state.transition[t].index
        json.dump(json_dict, outfile)

def load_fst():
    # load the fst from json
    with open('fst.json', 'r') as infile:
        json_dict = json.load(infile)
        minimal_transducer_states_dict = {}
        for state_id, state in json_dict['states'].items():
            minimal_transducer_states_dict[int(state_id)] = State(int(state_id))
            minimal_transducer_states_dict[int(state_id)].final = state['final']
            minimal_transducer_states_dict[int(state_id)].transition = {}
            for t in state['transitions']:
                minimal_transducer_states_dict[int(state_id)].transition[t] = State(int(state['transitions'][t]))
        initial_state_index = json_dict['initial_state']
        initial_state = minimal_transducer_states_dict[initial_state_index]
        return minimal_transducer_states_dict, initial_state

def measure_memory_usage(minimal_transducer_states_dict):
    transitions_count = 0
    for state in minimal_transducer_states_dict:
        transitions_count += len(minimal_transducer_states_dict[state].transition)
    print("Number of states: %s" % len(minimal_transducer_states_dict))
    print("Number of transitions: %s" % transitions_count)
    print("Dict size: %s bytes" % sys.getsizeof(minimal_transducer_states_dict))

def main():
    filename = 'american-english'
    with open(filename, 'r') as f:
        input_list = f.read().splitlines()
        max_word_length = max(len(word) for word in input_list)

    minimal_transducer_states_dict, initial_state = create_minimal_transducer(max_word_length, filename)
    
    autocomplete(minimal_transducer_states_dict, initial_state)

if __name__ == '__main__':
    main()
