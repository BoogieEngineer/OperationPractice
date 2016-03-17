# Example from Qwt documentation

import random
import sys

from PyQt4 import QtCore, QtGui
from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt
import numpy as np


class DataPlot(Qwt.QwtPlot):

    def __init__(self, *args):
        # call base class constructor
        Qwt.QwtPlot.__init__(self, *args)

        self.setCanvasBackground(Qt.Qt.white)

        # initialize data
        self.x = np.arange(0.0, 100.1, 0.5)
        self.y = np.zeros(len(self.x), np.float)
        self.z = np.zeros(len(self.x), np.float)

        # title
        self.setTitle("A Moving QwtPlot Demonstration")
        # legend
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.BottomLegend);

        # plot going right
        self.curveR = Qwt.QwtPlotCurve("Data Moving Right")
        self.curveR.attach(self)
        # plot going left
        self.curveL = Qwt.QwtPlotCurve("Data Moving Left")
        self.curveL.attach(self)

        # accent on each data point of plot going left
        self.curveL.setSymbol(Qwt.QwtSymbol(Qwt.QwtSymbol.Ellipse,
                                        Qt.QBrush(),
                                        Qt.QPen(Qt.Qt.yellow),
                                        Qt.QSize(7, 7)))

        # line colors
        self.curveR.setPen(Qt.QPen(Qt.Qt.red))
        self.curveL.setPen(Qt.QPen(Qt.Qt.blue))

        # set a line marker at zero
        mY = Qwt.QwtPlotMarker()
        mY.setLabelAlignment(Qt.Qt.AlignRight | Qt.Qt.AlignTop)
        mY.setLineStyle(Qwt.QwtPlotMarker.HLine)
        mY.setYValue(0.0)
        mY.attach(self)

        # axis titles
        self.setAxisTitle(Qwt.QwtPlot.xBottom, "Time (seconds)")
        self.setAxisTitle(Qwt.QwtPlot.yLeft, "Values")
    
        self.phase = 0.0

    # __init__()
    
    def update(self):
        if self.phase > np.pi - 0.0001:
            self.phase = 0.0

        # y moves from left to right:
        # shift y array right and assign new value y[0]
        self.y = np.concatenate((self.y[:1], self.y[:-1]), 1)
        self.y[0] = np.sin(self.phase) * (-1.0 + 2.0*random.random())
        
        # z moves from right to left:
        # Shift z array left and assign new value to z[n-1].
        self.z = np.concatenate((self.z[1:], self.z[:1]), 1)
        self.z[-1] = 0.8 - (2.0 * self.phase/np.pi) + 0.4*random.random()

        self.curveR.setData(self.x, self.y)
        self.curveL.setData(self.x, self.z)

        self.replot()
        self.phase += np.pi*0.02

    # update()

# class DataPlot

class testGUI(QtGui.QWidget):
    def __init__(self):
        # call base class constructor
        super(self.__class__, self).__init__()
        self.setWindowTitle('Qwt Plot')
        self.show()
        
        # plot widget
        testPlot = DataPlot()
        testLayout = QtGui.QHBoxLayout()
        testLayout.addWidget(testPlot)
        self.setLayout(testLayout)

        self.updateTimer = QtCore.QTimer()
        self.updateTimer.timeout.connect(testPlot.update)
        self.updateTimer.start(50)


def main(args): 
    app = QtGui.QApplication([])
    gui = testGUI()
    gui.resize(500, 300)
    sys.exit(app.exec_())

# main()

if __name__ == '__main__':
    main(sys.argv)
