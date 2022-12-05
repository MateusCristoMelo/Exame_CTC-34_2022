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
words = []
dic = open("american-english", 'r')
dicWords = dic.read().splitlines() 
interWords = []

string = "snake"
#string = input("Enter a word: ")
lev = LevenshteinAutomaton(string, 1)

def explore(state):
    key = tuple(state) # lists can't be hashed in Python so convert to a tuple
    if key in states: return states[key]
    i = counter[0]
    counter[0] += 1
    states[key] = i
    if lev.is_match(state): 
        matching.append(i)
    for c in lev.transitions(state) | set(['*']):
        newstate = lev.step(state, c)
        j = explore(newstate)
        transitions.append((i, j, c))
    return i

def findAllWordsUtil(source, destination, visited, word):

    visited[source] = True

    if source == destination:
        words.append(word)

    else:
        for i in transitions:
            if(i[0] == source):
                if visited[i[1]] == False:
                    word = word + i[2]
                    findAllWordsUtil(i[1], destination, visited, word)
                    word = word[: len(word) - 1]
    visited[source] = False

def findAllWords(source = 0):
    visited = [False] * (len(transitions))
    word = ""
    for s in matching:
        findAllWordsUtil(source, s, visited, word)

explore(lev.start())

def leveinsthein_comparation(word1, word2):
    if(len(word1) != len(word2)):
        return False
    for i in range(len(word1)):
        if word1[i] == word2[i] or word2[i] == "*":
            continue
        else:
            return False
    return True

def comp():
    for i in dicWords:
        for j in words:
                if leveinsthein_comparation(i, j) == True:
                    print(i)


#print(leveinstein_comparation("snake", "snak*"))
findAllWords()
comp()


#a = dicWords[87433]



#[]
#lev(abacax)
#(abacaxi) -> [abacaxi]
#lev(abacax*)
#...
#(abacaxeiro) -> [abacaxi, abacaxeiro]

# criacao de lista vazia

# V digitar palavra
# V montar grafo de todos que tem distancia 1
# V percorrer o grafo e escrever todas as palavras achadas que tem distancia 1
# fazer interseção com o dicionario pra ver se tem alguma palavra que vai
# se tiver, colocar esse elemento numa lista
# printar a lista no final

#word = input()
#data = json.load(open('data.json'))

#automaton = LevenshteinAutomaton(word, 1)
