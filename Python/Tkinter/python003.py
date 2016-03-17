from Tkinter import *

class Application(Frame):
	def __init__(self,master):
		Frame.__init__(self,master)
		self.grid()
		self.click_count = 0
		self.create_widgets()
		
	def create_widgets(self):
		self.button = Button(self, text = "Total clicks: 0", command = self.update_count)
		self.button.grid()
			
	def update_count(self):
		self.click_count += 1
		self.button["text"] = "Total clicks: " + str(self.click_count)
		
root = Tk()
root.geometry("200x200")
root.title("Click Count")

app = Application(root)

root.mainloop()
		