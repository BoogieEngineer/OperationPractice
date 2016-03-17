import sys
from PyQt4 import QtCore, QtGui

class testGUI(QtGui.QWidget):
    def __init__(self):
        super(self.__class__, self).__init__()
        self.setWindowTitle('Seed Stabilization GUI')
        self.show()


        testButton = QtGui.QPushButton('Test Button')
        # creates a callback function for when the button is clicked
        testButton.clicked.connect(self.printHello)
        testLayout = QtGui.QHBoxLayout()
        testLayout.addWidget(testButton)
        self.setLayout(testLayout)

    def printHello(self):
        # prints hello each time the button is clicked
        print 'Hello'

app = QtGui.QApplication([])
gui = testGUI()
sys.exit(app.exec_())
