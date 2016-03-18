# Learning layout management

import sys
from PyQt4 import QtGui, QtCore

class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example,self).__init__()
        
        self.initUI()
        
    def initUI(self):
        # Create a LCD number and a slider and connect them
        self.lcd = QtGui.QLCDNumber()
        self.sld = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.sld.valueChanged.connect(self.lcd.display)
        
        # Create a button
        self.btn = QtGui.QPushButton("Button 1")
        #self.btn.clicked.connect(self.lcd.reset) 
        
        
        # Create a horizontal box layout and put the widgets in it
        self.vbox = QtGui.QVBoxLayout()
        self.vbox.addWidget(self.lcd)
        self.vbox.addWidget(self.sld)
        self.vbox.addWidget(self.btn)
                
        self.setLayout(self.vbox)
        
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Signal & slot')
        
def main():
    app = QtGui.QApplication(sys.argv)
    example = Example()
    example.show()
    app.exec_()
    
if __name__ == "__main__":
    main()