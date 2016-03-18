import sys
from PyQt4 import QtGui, QtCore

class Window(QtGui.QMainWindow):
	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(400,300,300,100)
		self.setWindowTitle("Button that counts mouse clicks")

		self.GUI()

	def GUI(self):
		self.update_1count = 0
		self.update_2count = 0
		self.btn = QtGui.QPushButton("Initial total clicks: 0", self)
		self.btn.resize(300,20)
		self.btn2 = QtGui.QPushButton("Initial total double clicks: 0", self)
		self.btn2.resize(300,20)
		self.btn2.move(0,20)
		self.btn3 = QtGui.QPushButton("Sum of clicks: 0", self)
		self.btn3.resize(300,20)
		self.btn3.move(0,40)
		self.btn.clicked.connect(self.clickCount)
		self.btn2.clicked.connect(self.doubleClickCount)

	def clickCount(self):
		self.update_1count += 1
		self.btn.setText("Total clicks now that you have clicked the mouse: " + str(self.update_1count))
		self.btn3.setText("Sum of all clicks: " + str(self.update_1count + self.update_2count))

	def doubleClickCount(self):
		self.update_2count += 2
		self.btn2.setText("Total double clicks now: " + str(self.update_2count))
		self.btn3.setText("Sum of all clicks: " + str(self.update_1count + self.update_2count))
		

def main():
	app = QtGui.QApplication(sys.argv)
	ButtonCount = Window()
	ButtonCount.show()
	app.exec_()

if __name__ == "__main__":
	main()