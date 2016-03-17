from Tkinter import *

class Application(Frame):
	def __init__(self,master):
		Frame.__init__(self,master)
		self.grid()
		self.create_widgets()
		
	def create_widgets(self):
		self.instruction = Label(self, text = "Enter the password:")
		self.instruction.grid(row=0,column=0,sticky=W)
		
		self.password = Entry(self)
		self.password.grid(row=0,column=1,sticky=W)
		
		self.submit = Button(self, text = "Submit",command=self.reveal)
		self.submit.grid(row=1,column=0,sticky=W)
		
		self.password_outcome = Text(self, width=35,height=5,wrap = NONE)
		self.password_outcome.grid(row=2,column=0,sticky=W)
		
	def reveal(self):
		content = self.password.get()
		
		if content == "password":
			message = "Access granted!"
		else:
			message = "Access denied!"
			
		self.password_outcome.delete(0.0,END)
		self.password_outcome.insert(0.0,message)
		
root = Tk()
root.geometry("500x500")
root.title("Password")

app = Application(root)

root.mainloop()