#simple GUI

#import the Tkinter tool box library
from Tkinter import *

#create a background window
root = Tk()

#modify root window using attributes define in Tkinter
root.title("Labeler") #title of the window
root.geometry("200x100") #width by height

app = Frame(root) #create a frame to hold widgets
app.grid() #put the frame on the grid

button1 = Button(app, text = "This is a button") #add a button to 'app'
button1.pack() #put button on the grid

#another way to put text on a button
button2 = Button(app)
button2.grid()
button2.configure(text = "This will show text")

#another way to put text on a button
button3 = Button(app)
button3.grid()
button3["text"] = "This is a third button"

#kick off the event loop
root.mainloop()