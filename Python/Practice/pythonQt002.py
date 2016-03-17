import sys
from PyQt4 import QtGui,QtCore

class Example(QtGui.QMainWindow):
    
    def __init__(self):
        super(Example,self).__init__()
        
        self.initUI()
        
    def initUI(self):
        statusBar = self.statusBar()
        statusBar.showMessage("Ready")
        
        exitAction = QtGui.QAction("&Exit",self)
        exitAction.setShortcut("Ctrl+Q")
        exitAction.setStatusTip("Exit Application")
        exitAction.triggered.connect(QtCore.QCoreApplication.instance().quit)
        
        menuBar = self.menuBar()
        fileMenu = menuBar.addMenu("&File")
        fileMenu.addAction(exitAction)
        
        toolBar = self.addToolBar("Exit")
        toolBar.addAction(exitAction)
        
        textEdit = QtGui.QTextEdit()
        self.setCentralWidget(textEdit)
        
        self.setGeometry(50,50,300,200)
        self.setWindowTitle("This is the title")
        
def main():
    app = QtGui.QApplication(sys.argv)
    example = Example()
    example.show()
    app.exec_()
    
if __name__ == "__main__":
    main()