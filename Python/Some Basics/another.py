from PyQt4 import QtCore, QtGui

class directoryDialog(QtGui.QDialog):

    def __init__(self):
        super(self.__class__, self).__init__()
        directoryPathLabel = QtGui.QLabel('Path to Directory:')
        self.directoryPathLineEdit = QtGui.QLineEdit()
        imageTypeLabel = QtGui.QLabel('Image Type:')
        self.imageTypeLineEdit = QtGui.QLineEdit('png')
        okayButton = QtGui.QPushButton('Okay')
        okayButton.clicked.connect(self.okayClicked)
        cancelButton = QtGui.QPushButton('Cancel')
        cancelButton.clicked.connect(self.cancelClicked)

        inputDialogLineEditLayout = QtGui.QGridLayout()
        inputDialogLineEditLayout.addWidget(directoryPathLabel,0,0)
        inputDialogLineEditLayout.addWidget(self.directoryPathLineEdit,0,1)
        inputDialogLineEditLayout.addWidget(imageTypeLabel,1,0)
        inputDialogLineEditLayout.addWidget(self.imageTypeLineEdit,1,1)

        inputDialogButtonLayout = QtGui.QHBoxLayout()
        inputDialogButtonLayout.addWidget(okayButton)
        inputDialogButtonLayout.addWidget(cancelButton)

        inputDialogLayout = QtGui.QVBoxLayout()
        inputDialogLayout.addLayout(inputDialogLineEditLayout)
        inputDialogLayout.addLayout(inputDialogButtonLayout)

        self.setLayout(inputDialogLayout)
        return

    def okayClicked(self):
        self.imageDir = str(self.directoryPathLineEdit.text())
        self.imageType = str(self.imageTypeLineEdit.text())
        self.ok = True
        self.close()
        return

    def cancelClicked(self):
        self.imageDir = None
        self.imageType = None
        self.ok = False
        self.close()
        return

class testGUI(QtGui.QWidget):
    def __init__(self):
        super(self.__class__, self).__init__()
        self.setWindowTitle('Seed Stabilization GUI')
        self.show()

        self.inputDialog = directoryDialog()

        testButton = QtGui.QPushButton('Test Button')
        testButton.clicked.connect(self.triggerDialog)
        testLayout = QtGui.QHBoxLayout()
        testLayout.addWidget(testButton)
        self.setLayout(testLayout)

    def triggerDialog(self):
        self.inputDialog.show()

app = QtGui.QApplication([])
gui = testGUI()
app.exec_()
