import sys
from PyQt4 import QtCore, QtGui

class testGUI(QtGui.QWidget):
    def __init__(self):
        # base class constructor
        super(self.__class__, self).__init__()
        self.setWindowTitle('Test GUI')
        # show the gui
        self.show()
        
        # create a test label
        testLabel = QtGui.QLabel('Test')
        testLayout = QtGui.QHBoxLayout()
        testLayout.addWidget(testLabel)
        # set the gui layout
        self.setLayout(testLayout)

# create application
app = QtGui.QApplication([])
# make an instance of the gui class
gui = testGUI()
# start the event loop
sys.exit(app.exec_())
