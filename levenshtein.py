import json

class LevenshteinAutomaton:
    def __init__(self, string, n):
        self.string = string
        self.max_edits = n
        
    def start(self):
    	# returns the start state
        return range(len(self.string)+1)

    def step(self, state, c):
    	# returns the next state given a state and a character
        new_state = [state[0]+1]
        for i in range(len(state)-1):
            cost = 0 if self.string[i] == c else 1
            new_state.append(min(new_state[i]+1, state[i]+cost, state[i+1]+1))
        return [min(x,self.max_edits+1) for x in new_state]

    def is_match(self, state):
        return state[-1] <= self.max_edits

    def can_match(self, state):
        return min(state) <= self.max_edits

    def transitions(self, state):
        return set(c for (i,c) in enumerate(self.string) if state[i] <= self.max_edits) 


counter = [0] # list is a hack for mutable lexical scoping
states = {}
transitions = []
matching = []

lev = LevenshteinAutomaton("wooo", 3)

def explore(state):
    key = tuple(state) # lists can't be hashed in Python so convert to a tuple
    if key in states: return states[key]
    i = counter[0]
    counter[0] += 1
    states[key] = i
    if lev.is_match(state): matching.append(i)
    for c in lev.transitions(state) | set(['*']):
        newstate = lev.step(state, c)
        j = explore(newstate)
        transitions.append((i, j, c))
    return i

explore(lev.start())


#word = input()
#data = json.load(open('data.json'))

#automaton = LevenshteinAutomaton(word, 1)
