from Tkinter import *

class Application(Frame):
	def __init__(self,master):
		Frame.__init__(self,master)
		self.grid()
		self.create_widgets()
		
	def create_widgets(self):
		self.instruction = Label(self, text = "Choose your movie types. Select all that apply.")
		self.instruction.grid(row=0,column=0,sticky=W)
		
		self.comedy = IntVar()
		self.comedy_check = Checkbutton(self,text = "Comedy",variable=self.comedy,command = self.text_display)
		self.comedy_check.grid(row=1,column=0,sticky=W)
		
		self.action = IntVar()
		self.action_check = Checkbutton(self,text = "Action",variable=self.action,command = self.text_display)
		self.action_check.grid(row=2,column=0,sticky=W)
		
		self.scifi = IntVar()
		self.scifi_check = Checkbutton(self,text = "Sci-Fi",variable=self.scifi,command = self.text_display)
		self.scifi_check.grid(row=3,column=0,sticky=W)
		
		self.result = Text(self,width=30,height=5,wrap=WORD)
		self.result.grid(row=4,column=0,columnspan=2)
		
	def text_display(self):
		check = ""
		
		if self.comedy.get() == 1:
			check += "You like comedy.\n"
		if self.action.get() ==1:
			check += "You like action.\n"
		if self.scifi.get() ==1:
			check += "You like sci-fi.\n"
			
		self.result.delete(0.0,END)
		self.result.insert(0.0,check)
		
root = Tk()
root.title("Movie type survey")
root.geometry("400x400")

app = Application(root)

root.mainloop()