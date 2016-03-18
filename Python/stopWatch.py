import sys
from PyQt4 import QtCore, QtGui

UPDATE_RATE_HZ = 100.0

class mainWindow(QtGui.QWidget):
	def __init__(self):
		super(mainWindow, self).__init__()
		self.setGeometry(400,300,300,100)				
		self.setWindowTitle("Stop Watch")

		self.counter = 0

		# creating labels and buttons 
		self.timeLabel = QtGui.QLabel("Time (s):")
		self.timeCounterLabel = QtGui.QLabel(str(self.counter))
		self.startButton = QtGui.QPushButton("Start")
		self.clearButton = QtGui.QPushButton("Clear")

		# arranging labels and buttons
		self.hLayout = QtGui.QHBoxLayout()
		self.vLayout = QtGui.QVBoxLayout()
		self.vLayout.addWidget(self.timeLabel)
		self.vLayout.addWidget(self.timeCounterLabel)
		self.vLayout.addWidget(self.startButton)
		self.vLayout.addWidget(self.clearButton)

		self.setLayout(self.vLayout)

def main():
	app = QtGui.QApplication(sys.argv)
	GUI = mainWindow()
	GUI.show()
	app.exec_()

if __name__ == "__main__":
	main()