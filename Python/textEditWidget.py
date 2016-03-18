import sys
from PyQt4 import QtGui, QtCore

class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example, self).__init__()
        self.resize(500, 500)
        self.setWindowTitle("Text Editor")
        
        self.btnQuit = QtGui.QPushButton("Quit")
        self.btnQuit.clicked.connect(self.close)
        
        self.btnOpenFile = QtGui.QPushButton("Open File...")
        self.btnOpenFile.clicked.connect(self.openFile)
        
        self.btnAddText = QtGui.QPushButton("Add text")
        self.btnAddText.clicked.connect(self.addText)
        
        self.btnClearText = QtGui.QPushButton("Clear text")
        self.btnClearText.clicked.connect(self.clearText)
        
        self.textBrowser = QtGui.QTextBrowser()
        self.lineEdit = QtGui.QLineEdit()
        self.label = QtGui.QLabel("Add something to the text browser:")
        self.lineEdit.returnPressed.connect(self.addText)
        
        self.vbox = QtGui.QVBoxLayout()
        self.vbox.addWidget(self.textBrowser)
        self.vbox.addWidget(self.label)
        self.vbox.addWidget(self.lineEdit)
        
        self.hbox = QtGui.QHBoxLayout()
        self.hbox.addWidget(self.btnAddText)
        self.hbox.addWidget(self.btnClearText)
        self.hbox.addWidget(self.btnQuit)
        self.hbox.addWidget(self.btnOpenFile)
        
        self.vbox.addLayout(self.hbox)
        
        self.setLayout(self.vbox)
    
    # Open a file and print the content to the text browser
    def openFile(self):
        self.filename = QtGui.QFileDialog.getOpenFileName(self, "Open a File", '/')
        f = open(self.filename, "r")
        text = f.read()
        self.textBrowser.append("Contents of " + self.filename + ":\n %s" % text)
    
    # Add some text to the browser
    def addText(self):
        text = self.lineEdit.text()
        self.textBrowser.append(text)
        # Clear text after adding and set cursor on the line edit
        self.lineEdit.clear()
        self.lineEdit.setFocus()
        
    # Clear the content of the text browser
    def clearText(self):
        self.textBrowser.clear()
        self.lineEdit.setFocus()
        
def main():
    app = QtGui.QApplication(sys.argv)
    form = Example()
    form.show()
    app.exec_()
    
if __name__ == "__main__":
    main()    