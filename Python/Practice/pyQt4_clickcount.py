import sys
from PyQt4 import QtGui, QtCore

class Window(QtGui.QMainWindow):
    
    def __init__(self):
        super(Window,self).__init__()
        self.setGeometry(700,200,500,300)
        self.setWindowTitle("Button that counts mouse clicks")
        self.home()
        
    def home(self):
        self.btn2 = QtGui.QPushButton("Total clicks: 0",self)
        self.btn2.clicked.connect(self.clickCount)
        self.btn2.resize(100,20)
        self.btn2.move(0,0)
        
        self.update_count = 0
             
    def clickCount(self):
        self.update_count += 1
        self.btn2.setText("Total clicks: " + str(self.update_count))
        
def main():
    app = QtGui.QApplication(sys.argv)
    GUI = Window()
    GUI.show()
    app.exec_()
    
if __name__ == "__main__":
    main()