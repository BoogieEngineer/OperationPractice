# demonstrates how to use a class with Tkinter

from Tkinter import * # import everything from Tkinter

class Application(Frame): #create a class
	""" A GUI application with three buttons. """
	
	def __init__(self, master):
		""" Initialize the Frame """
		Frame.__init__(self,master)
		self.grid()
		self.create_widgets()
		
	def create_widgets(self):
		""" Create 3 buttons that do nothing. """
		# first button
		self.button1 = Button(self, text = "This is the first button")
		self.button1.grid()
		
		# second button
		self.button2 = Button(self)
		self.button2.grid()
		self.button2.configure(text = "This will show up text")
		
		# third button
		self.button3 = Button(self)
		self.button3.grid()
		self.button3["text"] = "This will also show text"
root = Tk()
root.title("Useless buttons")
root.geometry("200x80")

app = Application(root)

root.mainloop()