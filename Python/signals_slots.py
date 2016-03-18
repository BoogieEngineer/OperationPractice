import sys
from PyQt4 import QtGui, QtCore

class Dialog(QtGui.QDialog):
    
    def __init__(self):
        super(Dialog, self).__init__()
        
        self.resize(200, 100)
        self.setWindowTitle("Signals and Slots")
        
        self.dial = QtGui.QDial()
        self.dial.setNotchesVisible(True)
        
        self.spinBox = QtGui.QSpinBox()
        
        self.hbox = QtGui.QHBoxLayout()
        self.hbox.addWidget(self.dial)
        self.hbox.addWidget(self.spinBox)
        
        self.setLayout(self.hbox)
        
        self.dial.valueChanged.connect(self.spinBox.setValue)
        self.spinBox.valueChanged.connect(self.dial.setValue)
        
def main():
    app = QtGui.QApplication(sys.argv)
    form = Dialog()
    form.show()
    app.exec_()
    
if __name__ == "__main__":
    main()