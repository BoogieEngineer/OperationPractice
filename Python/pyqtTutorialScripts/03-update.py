import sys
from PyQt4 import QtCore, QtGui

UPDATE_RATE_HZ = 100.0

class testGUI(QtGui.QWidget):
    def __init__(self):
        super(self.__class__, self).__init__()
        self.counter = 0
        self.started = False
        self.setWindowTitle('Stop Watch')
        self.show()

        testLabel = QtGui.QLabel('Time (s)')
        self.testCounterLabel = QtGui.QLabel(str(self.counter))
        self.testCounterLabel.setFixedSize(100,20)
        self.testCounterLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.testButton = QtGui.QPushButton('Start')
        self.testButton.clicked.connect(self.startStop)
        self.testClearButton = QtGui.QPushButton('Clear')
        self.testClearButton.clicked.connect(self.clearCounter)
        
        testLayout = QtGui.QHBoxLayout(self)
        testLayout.addStretch(1)
        testLayout.addWidget(testLabel)
        testLayout.addWidget(self.testCounterLabel)
        testLayout.addWidget(self.testButton)
        testLayout.addWidget(self.testClearButton)
        testLayout.addStretch(1)
        self.setLayout(testLayout)

        # creates a timer that on each timeout call self.update()
        self.updateTimer = QtCore.QTimer()
        self.updateTimer.timeout.connect(self.update)

    def startStop(self):
        if self.started:
            # stops the timer and sets the button to start
            self.testButton.setText('Start')
            self.updateTimer.stop()
            self.started = False
        else:
            # starts the timer and sets the button to stop
            self.testButton.setText('Stop')
            self.updateTimer.start(1000.0/UPDATE_RATE_HZ)
            self.started = True

    def clearCounter(self):
        # clears the time
        self.counter = 0
        self.testCounterLabel.setText('0')

    def update(self):
        # updates the time
        self.counter = self.counter + 1
        self.testCounterLabel.setText('%.2f' % (self.counter/UPDATE_RATE_HZ))


app = QtGui.QApplication([])
gui = testGUI()
sys.exit(app.exec_())