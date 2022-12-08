from tkinter import *
from subprocess import run

def choice_function():
    if(radio_button_var.get() == "Levenshtein"):
        autocomplete_text["text"] = run(["python", "levenshtein.py"], text = True, input = input_string.get(), capture_output= True).stdout
        #levenshtein.string = input_string.get()
        #autocomplete_text["text"] = levenshtein.words

    elif(radio_button_var.get() == "FST"):
        autocomplete_text["text"] = run(["python", "fst.py"],  text = True, cwd = "FST")

    elif(radio_button_var.get() == "Hash"):
        #autocomplete_text["text"] = run(["python", "/FST/hash_table.py"], text = True, input = input_string.get(), capture_output= True).stdout
        autocomplete_text["text"] = run(["python", "hash_table.py"], text = True, input = input_string.get(), capture_output= True, cwd = "FST").stdout
    else:
        autocomplete_text["text"] = "Selecione uma opção"

def levenshteinfunc():
    autocomplete_text["text"] = radio_button_var.get() + ": " + input_string.get() 

window = Tk()
window.title("Exame CTC-34")
method = Label(window, text = "Clique para escolher um método")
method.grid(column = 0, row = 0, padx = 10, pady = 10)

radio_button_var = StringVar()

FST_selection_button = Radiobutton(window, text = "FST", value = "FST", variable = radio_button_var)
FST_selection_button.grid(column = 0, row = 1, padx = 10)

Hash_selection_button = Radiobutton(window, text = "Hashtable", value = "Hash", variable = radio_button_var)
Hash_selection_button.grid(column = 0, row = 2, padx = 10)

Levenshtein_selection_button = Radiobutton(window, text = "Levenshtein", value = "Levenshtein", variable = radio_button_var)
Levenshtein_selection_button.grid(column = 0, row = 3, padx = 10)

input_string = Entry(window)
input_string.grid(column = 0, row = 4, padx = 10, pady = 10)

enter_button = Button(window, text = "Enter", command = choice_function)
enter_button.grid(column = 0, row = 5, padx = 10, pady = 10)

autocomplete_text = Label(window, text = "")
autocomplete_text.grid(column = 0, row = 6, padx = 10, pady = 10)

window.mainloop()