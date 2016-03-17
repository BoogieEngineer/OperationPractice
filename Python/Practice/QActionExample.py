import sys
from PyQt4 import QtGui, QtCore

class Window(QtGui.QMainWindow):
    
    def __init__(self):
        super(Window,self).__init__()
        
        self.setGeometry(700,200,500,300)
        self.setWindowTitle("Window that quits")
        
        self.extractAction = QtGui.QAction("&Quit", self)
        self.extractAction.setShortcut("Alt+O")
        self.extractAction.setStatusTip("Leave the app")
        self.extractAction.triggered.connect(self.close)
        
        # another way to quit!
        self.extractAction2 = QtGui.QAction("&This quits too", self)
        self.extractAction2.triggered.connect(QtCore.QCoreApplication.instance().quit)
        
        self.statusBar()
        
        self.mainMenu = self.menuBar()
        self.fileMenu = self.mainMenu.addMenu("&File")
        self.quitMenu = self.fileMenu.addMenu("&This quits")
        self.quitMenu.addAction(self.extractAction)
        self.quitMenu.addAction(self.extractAction2)
        
        self.home()
        
    def home(self):
        self.btn = QtGui.QPushButton("Quit!", self)
        self.btn.clicked.connect(self.close)
        self.btn.resize(100,100)
        self.btn.move(0,20)
        
def main():
    app = QtGui.QApplication(sys.argv)
    GUI = Window()
    GUI.show()
    app.exec_()
    
if __name__ == "__main__":
    main()