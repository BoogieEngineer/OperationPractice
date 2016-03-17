from __future__ import division
import sys
from math import *
from PyQt4 import QtGui, QtCore

## Caculator GUI
class Form(QtGui.QWidget):
    
    def __init__(self):
        
        super(Form,self).__init__()
        
        self.browser = QtGui.QTextBrowser()
        self.lineedit = QtGui.QLineEdit("Type an expression and press Enter") # Initial text
        self.lineedit.selectAll() #Select all text in the lineedit widget
        #This ensure the initial text above will be overwritten as the user types text in
        
        self.calBtn = QtGui.QPushButton("Calculate (or press Enter)")
        self.calBtn.clicked.connect(self.updateUi)
         
        self.quitButton = QtGui.QPushButton("Quit")
        self.quitButton.clicked.connect(self.close)
        
        self.hbox = QtGui.QHBoxLayout()
        self.hbox.addWidget(self.calBtn)
        self.hbox.addWidget(self.quitButton)
        
        self.vbox = QtGui.QVBoxLayout()
        self.vbox.addWidget(self.browser)
        self.vbox.addWidget(self.lineedit)
        self.vbox.addLayout(self.hbox)
        self.setLayout(self.vbox)
        
        self.lineedit.setFocus()
        self.lineedit.returnPressed.connect(self.updateUi)
        self.setWindowTitle("Calculator")
        
    def updateUi(self):
        try:
            text = unicode(self.lineedit.text())
            self.browser.append("%s = <b>%s</b>" % (text,eval(text)))
        except:
            self.browser.append("<font color=red>%s is invalid!</font>" % text)
        
        self.lineedit.selectAll()
        self.lineedit.setFocus()

def main():
    app = QtGui.QApplication(sys.argv)
    form = Form()
    form.show()
    app.exec_()
    
if __name__ == "__main__":
    main()