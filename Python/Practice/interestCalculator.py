import sys
from PyQt4 import QtGui, QtCore

class Dialog(QtGui.QDialog):
    
    def __init__(self):
        super(Dialog, self).__init__()
        
        self.setWindowTitle("Compound Interest Calculator")
        
        self.principalLabel = QtGui.QLabel("Principal:")
        self.rateLabel = QtGui.QLabel("Rate:")
        self.yearsLabel = QtGui.QLabel("Years:")
        self.amountLabel = QtGui.QLabel("Amount")
        
        self.resultLabel = QtGui.QLabel()
        
        self.principalSpinBox = QtGui.QDoubleSpinBox()
        self.principalSpinBox.setPrefix("$ ")
        self.principalSpinBox.setRange(0.0, 1e12)
        self.principalSpinBox.setValue(2000.0)
        
        self.rateSpinBox = QtGui.QDoubleSpinBox()
        self.rateSpinBox.setSuffix(" %")
        self.rateSpinBox.setRange(0.0, 100)
        self.rateSpinBox.setValue(5.0)
        
        self.yearsComboBox = QtGui.QComboBox()
        self.yearsComboBox.addItem("1 year")
        self.yearsComboBox.addItems(["%d years" % x for x in range(2, 26)])
        
        self.grid = QtGui.QGridLayout()
        self.grid.addWidget(self.principalLabel, 0, 0)
        self.grid.addWidget(self.principalSpinBox, 0, 1)
        self.grid.addWidget(self.rateLabel, 1, 0)
        self.grid.addWidget(self.rateSpinBox, 1, 1)
        self.grid.addWidget(self.yearsLabel, 2, 0)
        self.grid.addWidget(self.yearsComboBox, 2, 1)
        self.grid.addWidget(self.amountLabel, 3, 0)
        self.grid.addWidget(self.resultLabel, 3, 1)

        self.setLayout(self.grid)
        
        self.principalSpinBox.valueChanged.connect(self.updateUI)
        self.rateSpinBox.valueChanged.connect(self.updateUI)
        self.yearsComboBox.currentIndexChanged.connect(self.updateUI)
        
        # This is only to initialize the result according to initial values of parameters
        self.updateUI()
        
    def updateUI(self):
        principal = self.principalSpinBox.value()
        rate = self.rateSpinBox.value()
        years = self.yearsComboBox.currentIndex() + 1
        amount = principal * ((1 + (rate / 100.0)) ** years)
        
        self.resultLabel.setText("$ %.2f" % amount)
        
def main():
    app = QtGui.QApplication(sys.argv)
    form = Dialog()
    form.show()
    app.exec_()
    
if __name__ == "__main__":
    main()