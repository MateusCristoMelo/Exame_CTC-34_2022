from tkinter import *

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

enter_button = Button(window, text = "Enter", command = levenshteinfunc)
enter_button.grid(column = 0, row = 5, padx = 10, pady = 10)

autocomplete_text = Label(window, text = "")
autocomplete_text.grid(column = 0, row = 6, padx = 10, pady = 10)


window.mainloop()