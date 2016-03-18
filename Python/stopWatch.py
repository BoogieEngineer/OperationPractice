import sys
from PyQt4 import QtCore, QtGui

UPDATE_RATE_HZ = 100.0

class mainWindow(QtGui.QWidget):
	# class variables are declared here, that are shared by all instances of 
	# this class.
	def __init__(self):
		super(mainWindow, self).__init__()
		self.setGeometry(400,300,300,100)				
		self.setWindowTitle("Stop Watch")

		self.counter = 0.0
		self.started = False

		# creating labels and buttons
		# In general, do not pass "self" as argument when instantiating Qt widgets
		self.timeLabel = QtGui.QLabel("Time (s):")
		self.timeCounterLabel = QtGui.QLabel("%.2f" % self.counter)
		self.startButton = QtGui.QPushButton("Start")
		self.clearButton = QtGui.QPushButton("Clear")

		# Arranging labels and buttons
		# Attributes without "self" are accessible only within the method where
		# they are declared, e.g these layouts below won't be accessible
		# by any other methods rather than this one (i.e __init__ method).
		# Can do this to layouts since they are only necessary at initialization.
		# Otherwise, be careful with scopes and namespaces.
		h1Layout = QtGui.QHBoxLayout()
		h2Layout = QtGui.QHBoxLayout()
		vLayout = QtGui.QVBoxLayout()
		h1Layout.addWidget(self.timeLabel)
		#h1Layout.addStretch(1)
		h1Layout.addWidget(self.timeCounterLabel)
		h2Layout.addWidget(self.startButton)
		#h2Layout.addStretch(1)
		h2Layout.addWidget(self.clearButton)
		vLayout.addLayout(h1Layout)
		#vLayout.addStretch(1)
		vLayout.addLayout(h2Layout)

		self.setLayout(vLayout)

		self.clearButton.clicked.connect(self.clearCounter)
		self.startButton.clicked.connect(self.startStop)

		# creates a QTimer object
		self.updateTimer = QtCore.QTimer()
		# also need a function that reads timer and update counter label
		self.updateTimer.timeout.connect(self.updateCounterLabel)

	def startStop(self):
		if self.started: # initially False
			# if not started, stops the timer, clears the counter, button reads "Start"
			self.startButton.setText("Start")
			self.updateTimer.stop()
			self.started = False # update the state for next button click
		else:
			# if started, run the timer, update the counter, button reads "Stop"
			self.startButton.setText("Stop")

			# update text every 1000/UPDATE_RATE_HZ msec
			self.updateTimer.start(1000.0/UPDATE_RATE_HZ)

			self.started = True # update the state for next button click

	def clearCounter(self):
		# clears the counter
		self.counter = 0
		self.timeCounterLabel.setText(str(self.counter))

	def updateCounterLabel(self):
		# update the counter label
		self.counter += 1
		self.timeCounterLabel.setText("%.2f" % (self.counter/UPDATE_RATE_HZ))

def main():
	app = QtGui.QApplication(sys.argv)
	GUI = mainWindow()
	GUI.show()
	app.exec_()

if __name__ == "__main__":
	main()