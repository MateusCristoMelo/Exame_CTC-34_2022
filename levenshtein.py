class SparseLevenshteinAutomaton:
    def __init__(self, string, n):
        self.string = string
        self.max_edits = n

    def start(self):
        return (range(self.max_edits+1), range(self.max_edits+1))

    def step(self, indices, values, c):
        if indices and indices[0] == 0 and values[0] < self.max_edits:
            new_indices = [0]
            new_values = [values[0] + 1]
        else:
            new_indices = []
            new_values = []

        for j,i in enumerate(indices):
            if i == len(self.string): break
            cost = 0 if self.string[i] == c else 1
            val = values[j] + cost
            if new_indices and new_indices[-1] == i:
                val = min(val, new_values[-1] + 1)
            if j+1 < len(indices) and indices[j+1] == i+1:
                val = min(val, values[j+1] + 1)
            if val <= self.max_edits:
                new_indices.append(i+1)
                new_values.append(val)

        return (new_indices, new_values)

    def is_match(self, indices):
        return bool(indices) and indices[-1] == len(self.string)

    def can_match(self, indices):
        return bool(indices)

    def transitions(self, indices):
        return set(self.string[i] for i in indices if i < len(self.string))

counter = [0] # list is a hack for mutable lexical scoping
states = {}
transitions = []
matching = []
words = []
dic = open("american-english", 'r')
dicWords = dic.read().splitlines() 

#string = input("Enter a word: ")
string = input()
lev = SparseLevenshteinAutomaton(string, 1)

def explore(state):
    key = (tuple(state[0]),tuple(state[1])) # lists can't be hashed in Python because they are mutable, so convert to a tuple
    if key in states: return states[key]
    i = counter[0]
    counter[0] += 1
    states[key] = i
    transitions.append([])
    if lev.is_match(state[0]): matching.append(i)
    for c in lev.transitions(state[0]) | set(['*']):
        newstate = lev.step(state[0], state[1], c)
        j = explore(newstate)
        transitions[i].append((j, c))
    return i

explore(lev.start())

def sortChar(elem):
    return elem[1]

for i in range(len(transitions)):
    transitions[i].sort(key = sortChar, reverse = True)

def levauto(word):
    current_state = 0
    if(abs(len(word) - len(string)) > 1):
        return

    for c in word:
        for i in transitions[current_state]:
            if(i[1] == c or i[1] == '*'):
                current_state = i[0]
                break
                

    for m in matching:
        if current_state == m:
            words.append(word)
            break

def levcomp():
    for i in dicWords:
        levauto(i)

levcomp()
print(words)