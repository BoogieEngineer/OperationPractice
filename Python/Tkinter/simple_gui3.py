# demonstrates how to use a class with Tkinter

from Tkinter import * # import everything from Tkinter

class Application(Frame): #create a class
	""" A GUI application with three buttons. """
	
	def __init__(self, master):
		""" Initialize the Frame """
		Frame.__init__(self,master)
		self.grid()
		self.button_clicks = 0 # initialize number of clicks
		self.create_widgets()
		
	def create_widgets(self):
		""" Create button that counts number of clicks. """
		self.button = Button(self)
		self.button["text"] = "Total clicks: 0"
		self.button["command"] = self.update_count
		self.button.grid()
		
	def update_count(self):
		""" Increase the click count and display the new click count """
		self.button_clicks += 1
		self.button["text"] = "Total clicks: " + str(self.button_clicks)
		
root = Tk()
root.title("Useless buttons")
root.geometry("200x80")

app = Application(root)

root.mainloop()