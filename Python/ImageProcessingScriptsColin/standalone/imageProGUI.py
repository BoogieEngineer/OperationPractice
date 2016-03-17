'''
Image Processing GUI developed by Colin Togashi

Uses Aravis, an open source library, and is therefore intended for CYMER INTERNAL USE ONLY

IP Config Setup:
    - Get the IP camera of the GigE camera that is going to be connected to
    - Set the IP address of the network adapter to match the subnet of the cameraType
        - ie. If my camera has an IP of 192.168.5.100, set the network adapter to 192.168.5.xxx
    - Ping the GigE camera from terminal
        - If you get responses back, launch the GUI
        - If not, find out why the connection is not working

Currently, to view two cameras, open two instances of terminal and launch a GUI from each instance.

Uses the image processing algo block which is documented in the AlgoLibraryEDS. Ask Colin.

Known Bugs:
    - Camera sometimes does not initialize properly because a past instance is still communicating
        with it despite closing python.

Future Features:
    - Display processed image (need to make faster)
    - Add support for Pyrocam IIIHR and IV
    - Add support for loading in hdf5 files
    - Add logging for bugs


Bug Fixes Since Last Release:
    - Fixed changing numROIs to work


Version 1.0.0
'''

# TODO - add logging capabilities to monitor any bugs
# TODO - test interface with a pyrocam IIIHR and a IV

import sys
# if sys.platform[0:5] == 'win32':
#     sys.path.insert(0, r'C:\work\Cymer\Algorithm Blocks\pyDLLs\hpImageProcessingDLL')
#     sys.path.insert(0, r'C:\work\Cymer\Algorithm Blocks\pyDLLs\windowsGigECameraDLL')

import math
import numpy as np
import os
import csv
import glob
import errno
import sys
import socket
import time
import itertools
import PyQt4.Qwt5 as Qwt
import h5py
import gc
import ctypes
import traceback

import scipy.misc as misc
import scipy.special as special

from scipy import linalg as lin
from PyQt4 import Qt, QtCore, QtGui
from PyQt4.QtGui import qRgb
from collections import OrderedDict
if sys.platform[0:5] == 'linux':
    from gi.repository import Aravis
from Mapping import COLOR8_CROSSHAIR as COLOR8
from Mapping import COLOR16
from hpImageProcessingDLL_run import hpImageProcessingLib, hpImageProcessingOutputSignals
# if sys.platform[0:5] == 'win32':
#     from windowsGigECameraDLL_run import windowsGigECameraLib


DEBUG = True
GUI_UPDATE_HZ = 10
CUR_DIR = os.path.dirname(os.path.realpath(__file__))
NUM_BOUNDS = 4
MAX_ROIS = 4

ROI_BOX_PERC_MAX0 = .50
ROI_BOX_PERC_MAX1 = .80

# gui constants
GUI_WIDTH = 1000
GUI_HEIGHT = 800
IMAGE_PLOT_WIDTH = 350
IMAGE_PLOT_HEIGHT = 300
HISTOGRAM_HEIGHT = 80
METRICS_TABLE_HEIGHT = 700
PARAMETERS_TABLE_WIDTH = 200
PARAMETERS_TABLE_HEIGHT = 400
ROI_BOUNDS_TABLE_WIDTH = 400
LABEL_WIDTH = 100
LABEL_HEIGHT = 20
PARAMETER_LABEL_WIDTH = 220
PARAMETER_LABEL_HEIGHT = 20
PARAMETER_LINE_EDIT_WIDTH = 100
PARAMETER_LINE_EDIT_HEIGHT = 20
NUMBER_PARAMETER_COLUMNS = 2
IMAGE_OPTIONS_COLUMN_WIDTH = 60
CAMERA_CONTROLS_COLUMN_WIDTH = 60
BOUND_ROI_STRINGS = ['Left Bound', 'Right Bound', 'Bottom Bound', 'Top Bound']
ROI_STRINGS = ['roi0', 'roi1', 'roi2', 'roi3']
CONTROL_WORD_MAX_COLS = 1

FLIR_IMAGE_WIDTH = 640
FLIR_IMAGE_HEIGHT = 512
FLIR_BIT_DEPTH = 14
FLIR_FRAME_RATE = 30.0

PYRO_IMAGE_WIDTH = 320
PYRO_IMAGE_HEIGHT = 320
PYRO_BIT_DEPTH = 16
PYRO_FRAME_RATE = 50.0

MAX_IMAGE_WIDTH = max(FLIR_IMAGE_WIDTH, PYRO_IMAGE_WIDTH)
MAX_IMAGE_HEIGHT = max(FLIR_IMAGE_HEIGHT, PYRO_IMAGE_HEIGHT)

NUM_POINTS = 500
ROI_INTENS = [252, 253, 254, 255]
ROI_COLORS = ['white', 'green', 'cyan', 'pink']
CENT_IMAGE_PADDING = 4
SIGNAT = 'CT'

def checkWindowsOrLinux(winFuncCall, linuxFuncCall):
    if sys.platform[0:5] == 'win32':
        return winFuncCall
    elif sys.platform[0:5] == 'linux':
        return linuxFuncCall
    else:
        raise IOError( errno.ENOENT, "Operating System not supported")

def checkRGB(rgb_image):
    ''' 
    '''
    if np.shape(np.shape(rgb_image)) == 3:
        red = rgb_image[:,:,0]
        green = rgb_image[:,:,1]
        blue = rgb_image[:,:,2]
        full_image = .2125*red+.7154*green+.0721*blue
    else:
        full_image = rgb_image
    return full_image

def drawROI(image, roiInfo):
    nrows, ncols = np.shape(image)
    if roiInfo == None:
        return image

    try:
        for roi in range(roiInfo['numROIs']):
            const = 2.0*np.pi/NUM_POINTS
            for i in range(NUM_POINTS):
                ang = i*const
                r = int(roiInfo['crosshairRad'][roi]/2*math.sin(ang)) + roiInfo['crosshairY'][roi]
                c = int(roiInfo['crosshairRad'][roi]/2*math.cos(ang)) + roiInfo['crosshairX'][roi]
                if r >= roiInfo['yBoundBottom'][roi] and r < roiInfo['yBoundTop'][roi] and c >= roiInfo['xBoundLeft'][roi] and c < roiInfo['xBoundRight'][roi]:
                    image[r][c] = roiInfo['intensity'][roi]
            
            ang = 2.0*np.pi*roiInfo['crosshairAng'][roi]/360
            for i in range(int(NUM_POINTS/4)):
                radius = i*roiInfo['crosshairRad'][roi]/(NUM_POINTS/4)
                for j in range(4):
                    r = int(radius*math.sin(ang+2*np.pi/4*j)) + roiInfo['crosshairY'][roi]
                    c = int(radius*math.cos(ang+2*np.pi/4*j)) + roiInfo['crosshairX'][roi]
                    if r >= roiInfo['yBoundBottom'][roi] and r < roiInfo['yBoundTop'][roi] and c >= roiInfo['xBoundLeft'][roi] and c < roiInfo['xBoundRight'][roi]:
                        image[r][c] = roiInfo['intensity'][roi]

    except:
        print 'Crosshair Error'

    try:
        for roi in range(roiInfo['numROIs']):
            for r in range(roiInfo['yBoundBottom'][roi],roiInfo['yBoundTop'][roi]):
                if (r == roiInfo['yBoundBottom'][roi] or r == (roiInfo['yBoundTop'][roi] - 1)
                    or (r == roiInfo['yBoundBottom'][roi] + 1) or (r == roiInfo['yBoundTop'][roi]-2)):
                    for c in range(roiInfo['xBoundLeft'][roi],roiInfo['xBoundRight'][roi]):
                        image[r][c] = roiInfo['intensity'][roi]
                else:
                    image[r][roiInfo['xBoundLeft'][roi]] = roiInfo['intensity'][roi]
                    image[r][roiInfo['xBoundRight'][roi]-1] = roiInfo['intensity'][roi]
                    image[r][roiInfo['xBoundLeft'][roi]+1] = roiInfo['intensity'][roi]
                    image[r][roiInfo['xBoundRight'][roi]-2] = roiInfo['intensity'][roi]

    except:
        print 'ROI Error'

    try:
        for roi in range(roiInfo['numROIs']):
            for padX in range(-roiInfo['centroidPadding'],roiInfo['centroidPadding']):
                for padY in range(-roiInfo['centroidPadding'],roiInfo['centroidPadding']):
                    if ((roiInfo['centroidX'][roi] + padX) > roiInfo['xBoundLeft'][roi] and 
                            (roiInfo['centroidX'][roi] + padX) < roiInfo['xBoundRight'][roi] and
                            (roiInfo['centroidY'][roi] + padY) > roiInfo['yBoundBottom'][roi] and
                            (roiInfo['centroidY'][roi] + padY) < roiInfo['yBoundTop'][roi]):
                        image[roiInfo['centroidY'][roi] + padY][roiInfo['centroidX'][roi] + padX] = roiInfo['intensity'][roi]
    except:
        print 'Center Error'

    image.astype(np.uint8)
    return image
    
def qPixmap8bit(data, roiInfo=None):
    # Note - top three values are reserved for roi0, roi1, and crosshairs
    nrows, ncols = np.shape(data)
    # img_8bit = (data / 252.0).astype(np.uint8) # use the high 8bit
    img_8bit = ((data - data.min()) / (data.ptp() / 251.0)).astype(np.uint8) # map the data range to 0 - 255
    img_8bit = drawROI(img_8bit, roiInfo)
    img = QtGui.QImage(img_8bit, ncols, nrows, QtGui.QImage.Format_Indexed8)
    colortable = [qRgb(r,g,b) for [r,g,b,a] in COLOR8]
    img.setColorTable(colortable)
    pixmap = QtGui.QPixmap.fromImage(img)
    return pixmap
    
def qPixmap16Bit(data):
    nrows, ncols = np.shape(data)
    RED16 = COLOR16[:,0]
    GREEN16 = COLOR16[:,1]
    BLUE16 = COLOR16[:,2]
    
    total = np.zeros((nrows,ncols,4))
    total[:,:,2] = np.take(RED16, data)
    total[:,:,1] = np.take(GREEN16, data)
    total[:,:,0] = np.take(BLUE16, data)
    total = np.uint8(total)

    img = QtGui.QImage(total.data, ncols, nrows, QtGui.QImage.Format_RGB32)
    pixmap = QtGui.QPixmap.fromImage(img)
    return pixmap

class displayTableWidget(QtGui.QTableWidget):

    def __init__(self, rows, cols): 
        super(self.__class__, self).__init__(rows+1, cols)
        self.NAME_WIDTH = 140
        self.VALUE_WIDTH = 110
        
        self.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.horizontalHeader().hide()
        self.verticalHeader().hide()
        self.setColumnWidth(0,self.NAME_WIDTH)
        for c in range(cols):
            roiTitleItem = QtGui.QTableWidgetItem('roi' + str(c))
            roiTitleItem.setTextAlignment(QtCore.Qt.AlignCenter)
            self.setItem(0, c+1, roiTitleItem)
            self.setColumnWidth(c+1, self.VALUE_WIDTH)
        self.verticalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        # self.horizontalHeader().setStretchLastSection(True)
        return
        
    def setHeaderCol(self, dict):
        for row, key in enumerate(dict):
            wordItem = QtGui.QTableWidgetItem(key)
            wordItem.setTextAlignment(QtCore.Qt.AlignCenter)
            self.setItem(row+1, 0, wordItem)
        return

    def setDataCol(self, dict, col):
        for row, key in enumerate(dict):
            dataItem = QtGui.QTableWidgetItem('{0:9.4f}'.format(dict[key]))
            dataItem.setTextAlignment(QtCore.Qt.AlignCenter)
            self.setItem(row+1, col, dataItem)
        return
    
    def updateData(self, dict, col):
        for row, key in enumerate(dict):
            self.item(row,col).setText('{0:9.4f}'.format(dict[key]))
        return

class directoryDialog(QtGui.QDialog):

    def __init__(self):
        super(self.__class__, self).__init__()
        okayButton = QtGui.QPushButton('Okay')
        okayButton.clicked.connect(self.okayClicked)
        cancelButton = QtGui.QPushButton('Cancel')
        cancelButton.clicked.connect(self.cancelClicked)

        directoryPathLabel = QtGui.QLabel('Directory:')
        self.directoryPathLineEdit = QtGui.QLineEdit()
        imageTypeLabel = QtGui.QLabel('Image Type:')
        self.imageTypeLineEdit = QtGui.QLineEdit()
        delimiterLabel = QtGui.QLabel('Delimiter:')
        self.delimiterLineEdit = QtGui.QLineEdit()

        inputDialogLineEditLayout = QtGui.QGridLayout()
        inputDialogLineEditLayout.addWidget(directoryPathLabel,0,0)
        inputDialogLineEditLayout.addWidget(self.directoryPathLineEdit,0,1)
        inputDialogLineEditLayout.addWidget(imageTypeLabel,1,0)
        inputDialogLineEditLayout.addWidget(self.imageTypeLineEdit,1,1)
        inputDialogLineEditLayout.addWidget(delimiterLabel,2,0)
        inputDialogLineEditLayout.addWidget(self.delimiterLineEdit,2,1)

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
        self.delimiter = str(self.delimiterLineEdit.text()).decode('string-escape')
        self.ok = True
        self.close()
        return

    def cancelClicked(self):
        self.imageDir = None
        self.imageType = None
        self.delimiter = None
        self.ok = False
        self.close()
        return

class connectDialog(QtGui.QDialog):

    def __init__(self, numCameras=1,cameraLibs=None):
        super(self.__class__, self).__init__()
        self.ok = False
        self.camerasAvailable = True
        self.listOfDeviceLists = []

        self.numCameras = numCameras
        if sys.platform[0:5] == 'linux':
            Aravis.update_device_list()

        inputDialogLineEditLayout = QtGui.QGridLayout()

        self.detectedCamerasComboBoxes = []
        for cam in range(numCameras):
            if sys.platform[0:5] == 'win32':
                self.listOfDeviceLists.append(cameraLibs[cam].getDeviceList())
            detectedCamerasLabel = QtGui.QLabel('Detected Cameras:')

            self.detectedCamerasComboBoxes.append(QtGui.QComboBox())
            if sys.platform[0:5] == 'win32':
                if self.listOfDeviceLists[cam] == []:
                    warningMessageBox = QtGui.QMessageBox.question(self, '',
                        'No Available Camera', QtGui.QMessageBox.Ok)
                    self.camerasAvailable = False
                for num in range(len(self.listOfDeviceLists[cam])):
                    self.detectedCamerasComboBoxes[cam].addItem(self.listOfDeviceLists[cam][num])
            elif sys.platform[0:5] == 'linux':
                if Aravis.get_n_devices() == 0:
                    warningMessageBox = QtGui.QMessageBox.question(self, '',
                        'No Available Camera', QtGui.QMessageBox.Ok)
                    self.camerasAvailable = False
                for num in range(Aravis.get_n_devices()):
                    self.detectedCamerasComboBoxes[cam].addItem(Aravis.get_device_id(num))

            inputDialogLineEditLayout.addWidget(detectedCamerasLabel,cam,0)
            inputDialogLineEditLayout.addWidget(self.detectedCamerasComboBoxes[cam],cam,1)

        okayButton = QtGui.QPushButton('Okay')
        okayButton.clicked.connect(self.okayClicked)
        cancelButton = QtGui.QPushButton('Cancel')
        cancelButton.clicked.connect(self.cancelClicked)

        inputDialogButtonLayout = QtGui.QHBoxLayout()
        inputDialogButtonLayout.addWidget(okayButton)
        inputDialogButtonLayout.addWidget(cancelButton)

        inputDialogLayout = QtGui.QVBoxLayout()
        inputDialogLayout.addLayout(inputDialogLineEditLayout)
        inputDialogLayout.addLayout(inputDialogButtonLayout)

        self.setLayout(inputDialogLayout)
        return

    def okayClicked(self):
        self.ok = True
        if self.numCameras > 1:
            for [cam0,cam1] in [list(x) for x in itertools.combinations(range(0,self.numCameras),2)]:
                if str(self.detectedCamerasComboBoxes[cam0].currentText()) == str(self.detectedCamerasComboBoxes[cam1].currentText()):
                    warningMessageBox = QtGui.QMessageBox.question(self, '',
                        'Same camera chosen twice', QtGui.QMessageBox.Ok)
                    self.ok = False
                    return
        self.close()
        return

    def cancelClicked(self):
        self.ok = False
        self.close()
        return

class controlWordDialog(QtGui.QDialog):

    def __init__(self, listOfControlWordBits, defaultString):
        super(self.__class__, self).__init__()

        self.listOfCheckBoxes = []
        self.outputString = defaultString[::-1]

        okayButton = QtGui.QPushButton('Okay')
        okayButton.clicked.connect(self.okayClicked)
        cancelButton = QtGui.QPushButton('Cancel')
        cancelButton.clicked.connect(self.cancelClicked)

        controlWordDialogLineEditLayout = QtGui.QGridLayout()
        row = 0
        col = 0
        for num in range(len(defaultString)):
            self.listOfCheckBoxes.append(QtGui.QCheckBox(listOfControlWordBits[num]))
            if defaultString[num] == '1':
                self.listOfCheckBoxes[num].setCheckState(2)
            controlWordDialogLineEditLayout.addWidget(self.listOfCheckBoxes[num],row,col)
            if col >= CONTROL_WORD_MAX_COLS:
                col = 0
                row = row + 1
            else:
                col = col + 1

        controlWordDialogButtonLayout = QtGui.QHBoxLayout()
        controlWordDialogButtonLayout.addWidget(okayButton)
        controlWordDialogButtonLayout.addWidget(cancelButton)

        controlWordDialogLayout = QtGui.QVBoxLayout()
        controlWordDialogLayout.addLayout(controlWordDialogLineEditLayout)
        controlWordDialogLayout.addLayout(controlWordDialogButtonLayout)

        self.setLayout(controlWordDialogLayout)
        return

    def okayClicked(self):
        self.outputString = ''
        for checkbox in reversed(self.listOfCheckBoxes):
            if (checkbox.checkState() > 0):
                self.outputString += '1'
            else:
                self.outputString += '0'
        self.ok = True
        self.close()
        return

    def cancelClicked(self):
        self.ok = False
        self.close()
        return

class CartesianAxis(Qwt.QwtPlotItem):

    def __init__(self, masterAxis, slaveAxis):
        Qwt.QwtPlotItem.__init__(self)
        self.__axis = masterAxis
        if masterAxis in (Qwt.QwtPlot.yLeft, Qwt.QwtPlot.yRight):
            self.setAxis(slaveAxis, masterAxis)
        else:
            self.setAxis(masterAxis, slaveAxis)
        self.scaleDraw = Qwt.QwtScaleDraw()
        self.scaleDraw.setAlignment((Qwt.QwtScaleDraw.LeftScale,
                                     Qwt.QwtScaleDraw.RightScale,
                                     Qwt.QwtScaleDraw.BottomScale,
                                     Qwt.QwtScaleDraw.TopScale)[masterAxis])

    def draw(self, painter, xMap, yMap, rect):
        if self.__axis in (Qwt.QwtPlot.yLeft, Qwt.QwtPlot.yRight):
            self.scaleDraw.move(round(xMap.xTransform(0.0)), yMap.p2())
            self.scaleDraw.setLength(yMap.p1()-yMap.p2())
        elif self.__axis in (Qwt.QwtPlot.xBottom, Qwt.QwtPlot.xTop):
            self.scaleDraw.move(xMap.p1(), round(yMap.xTransform(0.0)))
            self.scaleDraw.setLength(xMap.p2()-xMap.p1())
        self.scaleDraw.setScaleDiv(self.plot().axisScaleDiv(self.__axis))
        self.scaleDraw.draw(painter, self.plot().palette())
        return


class HistogramPlot(Qwt.QwtPlot):

    def __init__(self, width, height, *args):
        Qwt.QwtPlot.__init__(self, *args)
        self.setFixedSize(width, height)
        # create a plot with a white canvas
        self.setCanvasBackground(Qt.Qt.white)
        # set plot layout
        self.plotLayout().setMargin(0)
        self.plotLayout().setCanvasMargin(0)
        self.plotLayout().setAlignCanvasToScales(True)
        # attach a grid
        grid = Qwt.QwtPlotGrid()
        grid.attach(self)
        grid.setPen(Qt.QPen(Qt.Qt.black, 0, Qt.Qt.DotLine))
        # attach a x-axis
        xaxis = CartesianAxis(Qwt.QwtPlot.xBottom, Qwt.QwtPlot.yLeft)
        xaxis.attach(self)
        self.enableAxis(Qwt.QwtPlot.xBottom, False)
        # attach a y-axis
        yaxis = CartesianAxis(Qwt.QwtPlot.yLeft, Qwt.QwtPlot.xBottom)
        yaxis.attach(self)
        self.enableAxis(Qwt.QwtPlot.yLeft, False)
        # attach a curve
        self.curve = Qwt.QwtPlotCurve()
        self.curve.attach(self)
        self.curve.setPen(Qt.QPen(Qt.Qt.blue, 2))
        return

class imageLabel(QtGui.QLabel):
    def __init__(self, width, height, numCrosshairs=0):
        super(self.__class__, self).__init__()
        self.width = width
        self.height = height
        self.numCrosshairs = numCrosshairs

        self.imageWidth = width
        self.imageHeight = height
        self.setFixedSize(self.width, self.height)
        self.setScaledContents(True)
        self.RAD_MIN = 10
        self.RAD_MAX = 300
        self.select = 0

        self.crosshairX = [0]*numCrosshairs
        self.crosshairY = [0]*numCrosshairs
        self.leftPressed = False
        self.rightPressed = False
        self.crosshairRad = [40]*numCrosshairs
        self.crosshairAng = [0]*numCrosshairs

        self.delX = [0]*numCrosshairs
        self.delY = [0]*numCrosshairs
        self.prevX = [0]*numCrosshairs
        self.prevY = [0]*numCrosshairs
        return

    def mousePressEvent(self, event):
        try:
            if event.button() == QtCore.Qt.LeftButton:
                self.leftPressed = True
                if event.x() > -1 and event.y() > -1:
                    self.crosshairX[self.select] = int((event.x()*(self.imageWidth-1))/self.width)
                    self.crosshairY[self.select] = int((event.y()*(self.imageHeight-1))/self.height)
            elif event.button() == QtCore.Qt.RightButton:
                self.rightPressed = True
                if event.x() > -1 and event.y() > -1:
                    self.prevX[self.select] = event.x()
                    self.prevY[self.select] = event.y()
            elif event.button() == QtCore.Qt.MiddleButton:
                print 'Middle'
            else:
                print 'What button did you press?'
            return
        except:
            print 'Mouse Press Failed'

    def mouseMoveEvent(self, event):
        try:
            self.delX[self.select] = 0
            self.delY[self.select] = 0

            if (event.x() > -1 and event.y() > -1 and event.x() < self.width 
                and event.y() < self.height and self.leftPressed):
                self.crosshairX[self.select] = int((event.x()*(self.imageWidth-1))/self.width)
                self.crosshairY[self.select] = int((event.y()*(self.imageHeight-1))/self.height)
            if event.x() > -1 and event.y() > -1 and self.rightPressed:
                if event.y() - self.prevX[self.select] > 0 or event.x() - self.prevY[self.select] > 0:
                    moveX = event.x() - self.prevX[self.select]
                    moveY = event.y() - self.prevY[self.select]
                    if (self.crosshairRad[self.select] + moveX > self.RAD_MIN 
                        and self.crosshairRad[self.select] + moveX < self.RAD_MAX): 
                        self.crosshairRad[self.select] += moveX
                    self.crosshairAng[self.select] = (self.crosshairAng[self.select] + moveY) % 360
                    self.prevX[self.select] = event.x()
                    self.prevY[self.select] = event.y()
        except:
            print 'Mouse Move Failed'

    def wheelEvent(self, event):
        try:
            if not self.leftPressed and not self.rightPressed:
                self.select = (self.select + 1) % self.numCrosshairs
        except:
            print 'Mouse Wheel Failed'

    def mouseReleaseEvent(self, event):
        try:
            if event.button() == QtCore.Qt.LeftButton:
                self.leftPressed = False
            elif event.button() == QtCore.Qt.RightButton:
                self.rightPressed = False
        except:
            print 'Mouse Release Failed'

    def setImage(self,pixmap):
        self.imageWidth = pixmap.width()
        self.imageHeight = pixmap.height()
        self.setPixmap(pixmap)

class numberedPushButton(QtGui.QPushButton):
    def __init__(self, label, num):
        super(self.__class__, self).__init__(label)
        self.num = num
        return

class imageProGUI(QtGui.QWidget):
    def __init__(self):
        super(self.__class__, self).__init__()
        self.lastTime = time.clock()
        self.isConnected = False
        self.numCameras = 1
        self.libs = []
        self.first = True
        self.isReady = True
        self.camerasInitiated = False
        self.algoInitiated = False
        self.changeCameras = False
        self.algoOn = True
        self.numROIsList = []
        self.cameras = []
        self.cameraTypes = []
        self.cameraPayloads =[]
        self.cameraStreams = []
        # parameter list contains all information for each parameter excluding number rois
        # [name, default_value, lower_limit, upper_limit, tool_tip]
        self.parametersList = [['CONTROL_WORD', '101010010000000', 0, 32767, 'Switches on and off different things in the algorithm. Little endian. (Right bit, first)'],
                               ['LOWPASS_SMOOTHING_FACTOR', 0.2, 0, 1.0, 'The lowpass smoothing factor to be used in the pixel low pass filtering.'],
                               ['MED_FILT_PADDING', 1, 0, 10, 'Number of pixels in each direction to take into consideration for median filtering.\nThus, the window will be (2*medianPadding+1)^2 pixels.'],
                               ['THRESHOLD_CUTOFF', 10, 0, 100, 'Below this percentage*maxIntensity, all values will be converted to 0 and excluded from\ncalculations.'],
                               ['LEAST_SQUARES_CUTOFF', 1, 0, 100, 'Below this percentage*maxIntensity, all values will be converted to 0 and excluded from\nleast squares calculations.'],
                               ['LEAST_SQUARES_FIT_DELTA', 2000, 0, 40000, 'The delta used in calculating the recursive inverse for least squares. If too low, least\nsquares will not converge fast enough. If too high, results will be undefined'],
                               ['SUB_ROI_RADIUS', 500, 0, 1000, 'Radius used for the circular sub-roi around the max intensitry indices'],
                               ['SYM_RADIAL_LINES', 4, 0, 360, 'Number of lines used for IDS symmetry calculations'],
                               ['SYM_RADIAL_DISTANCE', 20, 0, 100, 'Radius used for IDS symmetry calulations'],
                               ['MIN_GAUSS_PERF', .5, 0, 2, 'Minimum value for simplePerformance that will trigger non-Gaussian beam.'],
                               ['COM_VALID_DISTANCE', 5, 0, 400, 'Any beam inside this border will trigger clipping beam. Not implemented yet.'],
                               ['NUM_IMAGES_TO_SUM', 1, 0, 600, 'Number of images to sum in image summing or averaging.'],
                               ['WATT_PER_COUNT', 1, 0, 1000, 'Watt per intensity count conversion factor.'],
                               ['FOCAL_LENGTH', 1, 0, 1000, 'Focal length of the lens.'],
                               ['MM_PER_PIXEL', .1, 0, 1000, 'mm per pixel conversion factor.'],
                               ['POWER_DENSITY_X_OFFSET', 100, 0, 640, 'X offset for the power density calculation.'],
                               ['POWER_DENSITY_Y_OFFSET', 100, 0, 512, 'Y offset for the power density calculation.'],
                               ['POWER_DENSITY_WIDTH', 100, 0, 640, 'Width of the rectangular region for power density calculation.'],
                               ['POWER_DENSITY_HEIGHT', 100, 0, 512, 'Height of the rectangular region for power density calculation.'],
                               ['POWER_DENSITY_MAJOR_RAD', 100, 0, 320, 'Major radius of the elliptical region for power density calculation.'],
                               ['POWER_DENSITY_MINOR_RAD', 100, 0, 320, 'Minor radius of the elliptical region for power density calculation.'],
                               ['POWER_DENSITY_ANGLE', 0, 0, 360, 'Angle between the major axis and the positive horizontal axis of the elliptical region\nfor power density calculation.'],
                               ['EFFECTIVE_IRRADIATION_THRESHOLD', 10, 0, 100, 'Percent above which is counted toward the effective irradiation area.'],
                               ['BEAM_POSITIONAL_STABILITY_LOWPASS', .1, 0, 1, 'The lowpass smoothing factor to be used in the beam positional stability lowpass filtering.'],
                               ['R_THETA_TOP_HAT_NUM_LINES', 360, 0, 720, 'Number of lines used in the r*Theta Hat calculation.'],
                               ['R_THETA_TOP_HAT_PERCENT_LEVEL', .5, 0, 1, 'User defined percentage in the r*Theta Hat calculation.'],
                               ['ELLIPTICAL_SUB_ROI_PADDING', 110, 0, 120, 'Padding on the elliptical aperture major and minor radius from the calculated results.'],
                               ['CENTROID_BOT_BOUND', [0,0], -1000, 1000, 'User defined bot bound on the centroid.'],
                               ['CENTROID_TOP_BOUND', [1000,1000], -1000, 1000, 'User defined top bound on the centroid.'],
                               ['BEAM_DIAMETER_BOT_BOUND', [0,0], -1000, 1000, 'User defined bot bound on the diameter.'],
                               ['BEAM_DIAMETER_TOP_BOUND', [1000,1000], -1000, 1000, 'User defined top bound on the diameter.'],
                               ['DIVERGENCE_BOT_BOUND', [0,0], -1000, 1000, 'User defined bot bound on the divergence angle.'],
                               ['DIVERGENCE_TOP_BOUND', [1000,1000], -1000, 1000, 'User defined top bound on the divergence angle.'],
                               ['GAUSS_PERF_TOP_BOUND', [1000,1000], -1000, 1000, 'User defined top bound on the gaussian performance.'],
                               ['SYMMETRY_TOP_BOUND', 1000, -1000, 1000, 'User defined top bound on the symmetry.'],
                               ['PEARSON_BOT_BOUND', 0, -1000, 1000, 'User defined bot bound on the Pearson Correlation.'],
                               ['MAX_INTENSITY_BOT_BOUND', 0, -1000, 1000, 'User defined bot bound on the max intensity.'],
                               ['MAX_INTENSITY_TOP_BOUND', 1000, -1000, 1000, 'User defined top bound on the max intensity.'],
                               ['PROBE_ROI', 0, 0, 3, 'Roi that the probe will act on.'],
                               ['PROBE_LOCATION', 0, 0, 9, 'Location of the probe. Refer to EDS.'],
                               ['PROBE_MODE', 0, 0, 2, 'Probe Mode. \n0: No Data.\n1: Playback Data Buffer.\n2: Random noise.'],
                               ['PROBE_SIGNAL_PERIOD', 0, 0, 1000, 'Signal period in samples of the playback buffer.'],
                               ['PROBE_SIGNAL_AMPLITUDE', 0, -1000, 1000, 'Signal amplitude that the playback buffer is scaled by.'],
                               ['PROBE_SIGNAL_OFFSET', 0, -1000, 1000, 'Amount to offset the signal.'],
                               ['PROBE_SIGNAL_DATA', [0], -1, 4, 'Playback buffer.']]
        self.parameterStrings = ['CONTROL_WORD',
                                 'LOWPASS_SMOOTHING_FACTOR',
                                 'MED_FILT_PADDING',
                                 'THRESHOLD_CUTOFF',
                                 'LEAST_SQUARES_CUTOFF',
                                 'LEAST_SQUARES_FIT_DELTA',
                                 'SUB_ROI_RADIUS',
                                 'SYM_RADIAL_LINES',
                                 'SYM_RADIAL_DISTANCE',
                                 'COM_VALID_DISTANCE',
                                 'NUM_IMAGES_TO_SUM',
                                 'WATT_PER_COUNT',
                                 'FOCAL_LENGTH',
                                 'MM_PER_PIXEL',
                                 # 'POWER_DENSITY_X_OFFSET',
                                 # 'POWER_DENSITY_Y_OFFSET',
                                 # 'POWER_DENSITY_WIDTH',
                                 # 'POWER_DENSITY_HEIGHT',
                                 # 'POWER_DENSITY_MAJOR_RAD',
                                 # 'POWER_DENSITY_MINOR_RAD',
                                 # 'POWER_DENSITY_ANGLE',
                                 # 'EFFECTIVE_IRRADIATION_THRESHOLD',
                                 # 'BEAM_POSITIONAL_STABILITY_LOWPASS',
                                 # 'R_THETA_TOP_HAT_NUM_LINES',
                                 # 'R_THETA_TOP_HAT_PERCENT_LEVEL',
                                 'ELLIPTICAL_SUB_ROI_PADDING',
                                 'CENTROID_BOT_BOUND',
                                 'CENTROID_TOP_BOUND',
                                 'BEAM_DIAMETER_BOT_BOUND',
                                 'BEAM_DIAMETER_TOP_BOUND',
                                 'DIVERGENCE_BOT_BOUND',
                                 'DIVERGENCE_TOP_BOUND',
                                 'GAUSS_PERF_TOP_BOUND',
                                 'SYMMETRY_TOP_BOUND',
                                 'PEARSON_BOT_BOUND',
                                 'MAX_INTENSITY_BOT_BOUND',
                                 'MAX_INTENSITY_TOP_BOUND',
                                 'PROBE_ROI',
                                 'PROBE_LOCATION',
                                 'PROBE_MODE',
                                 'PROBE_SIGNAL_PERIOD',
                                 'PROBE_SIGNAL_AMPLITUDE',
                                 'PROBE_SIGNAL_OFFSET',
                                 'PROBE_SIGNAL_DATA']
        self.listOfControlWordBits = ['Image Subtraction',
                                      'Frame Summing',
                                      'Frame Averaging',
                                      'Median Filtering',
                                      'Background Subtraction',
                                      'Lowpass Filtering',
                                      'Circular Sub ROI',
                                      'Elliptical Sub ROI',
                                      'Simple Thresholding',
                                      'Correct for Thresholding',
                                      'Symmetry Calculations',
                                      'Least Squares Calculations',
                                      'BeamGage Calculations',
                                      'Rectangular Power Density',
                                      'Out of Range Warnings']
        self.parameterToolTips = ['Switches on and off different things in the algorithm. Little endian. (Right bit, first)',
                                 'The lowpass smoothing factor to be used in the pixel low pass filtering.',
                                 'Number of pixels in each direction to take into consideration for median filtering.\nThus, the window will be (2*medianPadding+1)^2 pixels.',
                                 'Below this percentage*maxIntensity, all values will be converted to 0 and excluded from\ncalculations.',
                                 'Below this percentage*maxIntensity, all values will be converted to 0 and excluded from\nleast squares calculations.',
                                 'The delta used in calculating the recursive inverse for least squares. If too low, least\nsquares will not converge fast enough. If too high, results will be undefined',
                                 'Radius used for the circular sub-roi around the max intensitry indices',
                                 'Number of lines used for IDS symmetry calculations',
                                 'Radius used for IDS symmetry calulations',
                                 'Any beam inside this border will trigger clipping beam. Not implemented yet.',
                                 'Number of images to sum in image summing or averaging.',
                                 'Watt per intensity count conversion factor.',
                                 'Focal length of the lens.',
                                 'mm per pixel conversion factor.',
                                 # 'X offset for the power density calculation.',
                                 # 'Y offset for the power density calculation.',
                                 # 'Width of the rectangular region for power density calculation.',
                                 # 'Height of the rectangular region for power density calculation.',
                                 # 'Major radius of the elliptical region for power density calculation.',
                                 # 'Minor radius of the elliptical region for power density calculation.',
                                 # 'Angle between the major axis and the positive horizontal axis of the elliptical region\nfor power density calculation.',
                                 # 'Percent above which is counted toward the effective irradiation area.',
                                 # 'The lowpass smoothing factor to be used in the beam positional stability lowpass filtering.',
                                 # 'Number of lines used in the r*Theta Hat calculation.',
                                 # 'User defined percentage in the r*Theta Hat calculation.',
                                 'Padding on the elliptical aperture major and minor radius from the calculated results.',
                                 'User defined bot bound on the centroid.',
                                 'User defined top bound on the centroid.',
                                 'User defined bot bound on the diameter.',
                                 'User defined top bound on the diameter.',
                                 'User defined bot bound on the divergence angle.',
                                 'User defined top bound on the divergence angle.',
                                 'User defined top bound on the simple Performance.',
                                 'User defined top bound on the symmetry.',
                                 'User defined bot bound on the Pearson Correlation.',
                                 'User defined bot bound on the max intensity.',
                                 'User defined top bound on the max intensity.',
                                 'Roi that the probe will act on.',
                                 'Location of the probe. Refer to EDS.', #Note: need to list locations
                                 'Probe Mode. \n0: No Data.\n1: Playback Data Buffer.\n2: Random noise.',
                                 'Signal period in samples of the playback buffer.',
                                 'Signal amplitude that the playback buffer is scaled by.',
                                 'Amount to offset the signal.',
                                 'Playback buffer.']
        # self.parameterDefaultValues = ['101010010000000',0.2,1,10,10,1,2000,500,4,20,.5,5,1,1,1,.1,100,100,100,100,100,100,0,10,.1,360,.5,110,[0,0],[1000,1000],[0,0],[1000,1000],[0,0],[1000,1000],0,1000,0,1000,0,0,0,0,0,0,[0]]
        # self.parameterLowerLimits= [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,0,0,0,0,-1000,-1000,-1]
        # self.parameterUpperLimits = [111111111111111,1.0,10,100,65535,100,40000,1000,360,100,2,400,600,1000,1000,1000,640,512,640,512,320,320,360,100,1,720,1,120,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,3,9,2,1000,1000,1000,4]
        # self.parameterCurrentValues = ['101010010000000',0.2,1,2,10,1,2000,500,4,20,.5,5,1,1,1,1,100,100,100,100,100,100,0,10,.1,360,.5,110,[0,0],[1000,1000],[0,0],[1000,1000],[0,0],[1000,1000],0,1000,0,1000,0,0,0,0,0,0,[0]]
        self.parameterDefaultValues = ['101010010000000',0.2,1,10,1,2000,500,4,20,5,1,1,1,.1,110,[0,0],[1000,1000],[0,0],[1000,1000],[0,0],[1000,1000],1000,1000,0,0,65536,0,0,0,0,0,0,[0]]
        self.parameterLowerLimits = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,0,0,0,0,-1000,-1000,-1]
        self.parameterUpperLimits = [32767,1.0,10,100,100,40000,1000,360,100,400,600,1000,1000,1000,120,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,100000,3,9,2,1000,1000,1000,4]
        self.parameterCurrentValues = ['101010010000000',0.2,1,2,1,2000,500,4,20,5,1,1,1,1,110,[0,0],[1000,1000],[0,0],[1000,1000],[0,0],[1000,1000],1000,1000,0,0,65536,0,0,0,0,0,0,[0]]
        self.intValidatorList = [2,8,11,25,26,27,28]
        self.noValidatorList = [16,17,18,19,20,21,31]
        # self.intValidatorList = []
        # self.noValidatorList = range(len(self.parameterStrings))
        self.imageDisplay = []
        self.metricsDisplay = []
        self.imageXHistograms = []
        self.imageYHistograms = []
        self.crosshairXDeltaDisplay = []
        self.crosshairYDeltaDisplay = []
        self.calibrateButtons = []
        self.shutterButtons = []

        self.parameterLineEdits = []
        self.boundLineEdits = []
        self.numberOfROIsComboBoxList = []
        self.parameterROIBoundsGroupBoxLayoutList = []
        self.parameterROIBoundsGroupBoxList = []
        self.imageSaveComboBoxList = []
        # if sys.platform[0:5] == 'win32':
        #     self.cameraLibs = []
        #     self.cameraLibs.append(windowsGigECameraLib())
        #     self.cameraLibs[0].Initialize()

        self.initGUI()      
        return                       
        
    def initGUI(self):
        self.setWindowTitle('Image Processing GUI')
        self.show()
        self.resize(GUI_WIDTH,GUI_HEIGHT)
        # self.setFixedSize(GUI_WIDTH,GUI_HEIGHT)
        
        self.intValidator = QtGui.QIntValidator()
        self.intValidator.setBottom(0)
        
        # create a double validator to accepts only doubles greater than 0
        self.doubleValidator = QtGui.QDoubleValidator()
        self.doubleValidator.setBottom(0)

        # create a setup tab
        self.setupTab = QtGui.QWidget()
        self.setupTabLayout = self.createSetupTabLayout(self.setupTab)
        
        self.tabWidget = QtGui.QTabWidget()     
        self.tabWidget.addTab(self.setupTab, 'Setup')

        # create a metrics tab
        self.metricsTabs = []
        self.metricsTabsLayouts = []
        for tab in range(self.numCameras):
            self.metricsTabs.append(QtGui.QWidget())
            self.metricsTabsLayouts.append(self.createMetricsTabLayout(self.metricsTabs[tab], self.numROIsList[tab]))
            self.tabWidget.addTab(self.metricsTabs[tab], 'Camera ' + str(tab))
            
        # create a status label at the bottom
        self.statusLabel = QtGui.QLabel('Status: Disconnected')
        self.frameRateLabel = QtGui.QLabel('N/A')

        statusLayout = QtGui.QHBoxLayout()
        statusLayout.addWidget(self.statusLabel)
        statusLayout.addStretch(1)
        statusLayout.addWidget(self.frameRateLabel)

        # create the main layout and add the widgets
        mainLayout = QtGui.QVBoxLayout(self)
        mainLayout.addWidget(self.tabWidget)
        mainLayout.addLayout(statusLayout)
        
        # if sys.platform[0:5] == 'win32':
        #     libpath = r"C:\work\Cymer\Algorithm Blocks\pyDLLs\imageProDLL\imageProDLL\Release"
        #     dllfile = "imageProDLL.dll"
        # elif sys.platform[0:5] == 'linux':
        #     libpath = r"./imageProDLL"
        #     dllfile = "imageProDLL.so"
        # else:
        #     raise IOError( errno.ENOENT, "Operating System not supported")

        # dllFilePath = os.path.join(libpath, dllfile)
        # warningMessageBox = QtGui.QMessageBox.question(self, '',
        #     'Warning: DLL last updated: %s' 
        #     % time.ctime(os.path.getmtime(dllFilePath)), QtGui.QMessageBox.Ok)

        self.updateTimer = QtCore.QTimer()
        self.updateTimer.timeout.connect(self.update)
        return
        
    def createSetupTabLayout(self, parent):                               
        # create a label for the image depth
        imageSourceLabel = QtGui.QLabel('Image Source')
        
        # create a combo box for the image options
        self.imageSourceComboBox = QtGui.QComboBox()
        self.imageSourceComboBox.addItem('Direct IP Connect')
        self.imageSourceComboBox.addItem('Image Directory')
        self.imageSourceComboBox.currentIndexChanged.connect(self.changeImageSource)
        
        numberOfCamerasLabel = QtGui.QLabel('Number of Cameras')
        
        # create a combo box for the image options
        self.numberOfCamerasComboBox = QtGui.QComboBox()
        self.numberOfCamerasComboBox.addItem('1')
        self.numberOfCamerasComboBox.addItem('2')
        self.numberOfCamerasComboBox.currentIndexChanged.connect(self.changeNumberOfCameras)

        imageDisplayLabel = QtGui.QLabel('Displayed Image')
        
        self.imageDisplayComboBox = QtGui.QComboBox()
        self.imageDisplayComboBox.addItem('Raw Image')
        self.imageDisplayComboBox.addItem('Processed Image')

        self.algoOnComboBox = QtGui.QComboBox()
        self.algoOnComboBox.addItem('IP Algo On')
        self.algoOnComboBox.addItem('IP Algo Off')
        
        # create a table for all the image options
        cameraControlsTableLayout = QtGui.QGridLayout()
        cameraControlsTableLayout.setAlignment(QtCore.Qt.AlignTop)

        cameraControlsTableLayout.addWidget(imageSourceLabel,0,0)
        cameraControlsTableLayout.addWidget(self.imageSourceComboBox,0,1)
        cameraControlsTableLayout.addWidget(numberOfCamerasLabel,0,2)
        cameraControlsTableLayout.addWidget(self.numberOfCamerasComboBox,0,3)    
        cameraControlsTableLayout.addWidget(imageDisplayLabel,0,4)
        cameraControlsTableLayout.addWidget(self.imageDisplayComboBox,0,5)
        cameraControlsTableLayout.addWidget(self.algoOnComboBox,0,6)

        
        for col in range(cameraControlsTableLayout.columnCount()):
            cameraControlsTableLayout.setColumnMinimumWidth(col,CAMERA_CONTROLS_COLUMN_WIDTH)

        # create a layout and add the image options and title
        cameraControlsWidget = QtGui.QGroupBox('Camera Controls')
        cameraControlsWidget.setLayout(cameraControlsTableLayout) 

        cameraControlsSpacingLayout = QtGui.QHBoxLayout()
        cameraControlsSpacingLayout.addWidget(cameraControlsWidget) 
        cameraControlsSpacingLayout.addStretch(1)

        self.cameraParameterGroupBoxList = []

        for cam in range(self.numCameras):
            self.cameraParameterGroupBoxList.append(self.createCameraParameterGroupBox(cam))

        self.parametersGroupBoxesSpacingLayout = QtGui.QHBoxLayout()
        for cameraParameterGroupBox in self.cameraParameterGroupBoxList:
            self.parametersGroupBoxesSpacingLayout.addWidget(cameraParameterGroupBox) 
        self.parametersGroupBoxesSpacingLayout.addStretch(1)
            
        # create a button to connect to the camera
        self.connectButton = QtGui.QPushButton('Connect')
        self.connectButton.clicked.connect(self.connectToImages)
        # self.acquisitionButton = QtGui.QPushButton('Start')
        # self.acquisitionButton.clicked.connect(self.startStopCameraAcquisition)
        connectButtonLayout = QtGui.QHBoxLayout()
        connectButtonLayout.addWidget(self.connectButton)
        # connectButtonLayout.addWidget(self.acquisitionButton)

        
        # create a layout to hold everything in a tab
        setupTabLayout = QtGui.QVBoxLayout(parent)
        setupTabLayout.addLayout(cameraControlsSpacingLayout)
        setupTabLayout.addLayout(self.parametersGroupBoxesSpacingLayout)
        setupTabLayout.addStretch(1)
        setupTabLayout.addLayout(connectButtonLayout)
        
        return setupTabLayout

    def createCameraParameterGroupBox(self, cam):
        parametersGroupBox = QtGui.QGroupBox('Camera '+str(cam)+' Parameters')
        parametersGroupBoxLayout = QtGui.QVBoxLayout(parametersGroupBox)
        parametersGroupBoxLayout.setAlignment(QtCore.Qt.AlignTop)

        numberOfROIsLabel = QtGui.QLabel('Number of ROIs')
        self.numberOfROIsComboBoxList.append(QtGui.QComboBox())
        self.numberOfROIsComboBoxList[cam].addItem('1')
        self.numberOfROIsComboBoxList[cam].addItem('2')
        self.numberOfROIsComboBoxList[cam].addItem('4')
        self.numberOfROIsComboBoxList[cam].currentIndexChanged.connect(self.changeNumberOfROIs)
        self.numROIsList.append(1)

        imageSaveLabel = QtGui.QLabel('Save Images?')

        self.imageSaveComboBoxList.append(QtGui.QComboBox())
        self.imageSaveComboBoxList[cam].addItem('Save Raw Images')
        self.imageSaveComboBoxList[cam].addItem('Do Not Save Images')

        parameterOptionsGroupBox = QtGui.QGroupBox('Image Options')
        parameterOptionsGroupBoxLayout = QtGui.QGridLayout(parameterOptionsGroupBox)
        parameterOptionsGroupBoxLayout.setAlignment(QtCore.Qt.AlignTop)

        parameterOptionsGroupBoxLayout.addWidget(numberOfROIsLabel,0,0)
        parameterOptionsGroupBoxLayout.addWidget(self.numberOfROIsComboBoxList[cam],0,1)
        parameterOptionsGroupBoxLayout.addWidget(imageSaveLabel,0,2)
        parameterOptionsGroupBoxLayout.addWidget(self.imageSaveComboBoxList[cam],0,3)  

        for col in range(parameterOptionsGroupBoxLayout.columnCount()):
            parameterOptionsGroupBoxLayout.setColumnMinimumWidth(col,IMAGE_OPTIONS_COLUMN_WIDTH)

        parametersIPGroupBox = QtGui.QGroupBox('IP Parameters')
        parametersIPGroupBoxLayout = self.createParametersIPGroupBoxLayout(parametersIPGroupBox, cam)

        parameterROIBoundsGroupBox = QtGui.QGroupBox('ROI Bounds')
        self.parameterROIBoundsGroupBoxLayoutList.append(self.createROIBoundsGroupBoxLayout(parameterROIBoundsGroupBox, self.numROIsList[cam]))
        self.parameterROIBoundsGroupBoxList.append(parameterROIBoundsGroupBox)

        parametersGroupBoxLayout.addWidget(parameterOptionsGroupBox)
        parametersGroupBoxLayout.addWidget(parametersIPGroupBox)
        parametersGroupBoxLayout.addWidget(self.parameterROIBoundsGroupBoxList[cam])

        return parametersGroupBox

    def createParametersIPGroupBoxLayout(self, parent, cam):
        parametersIPGroupBoxLayout = QtGui.QGridLayout(parent)
        parametersIPGroupBoxLayout.setAlignment(QtCore.Qt.AlignTop)

        offset = 0
        ind = 0

        # create a storage for the lineEdits
        parameterLineEditsTemp = []

        for i in range(len(self.parameterStrings)):
            doubleValidator = QtGui.QDoubleValidator()
            doubleValidator.setBottom(self.parameterLowerLimits[i])
            if i == 0:
                parameterLineEditsTemp.append(numberedPushButton(str(int(self.parameterDefaultValues[0],2)),cam))
                parameterLineEditsTemp[i].clicked.connect(self.editControlWord)
            else:
                parameterLineEditsTemp.append(QtGui.QLineEdit(str(self.parameterDefaultValues[i])))
                parameterLineEditsTemp[i].setFixedSize(PARAMETER_LINE_EDIT_WIDTH,PARAMETER_LINE_EDIT_HEIGHT)
                if i in self.intValidatorList:
                    parameterLineEditsTemp[i].setValidator(self.intValidator)
                elif i in self.noValidatorList:
                    pass
                else:
                    parameterLineEditsTemp[i].setValidator(doubleValidator)
            parameterLabelTemp = QtGui.QLabel(self.parameterStrings[i])
            parameterLabelTemp.setFixedSize(PARAMETER_LABEL_WIDTH,PARAMETER_LABEL_HEIGHT)
            parameterLabelTemp.setToolTip(self.parameterToolTips[i])
            if (i % ((len(self.parameterStrings)+1)/NUMBER_PARAMETER_COLUMNS+1)) == (len(self.parameterStrings)+1)/NUMBER_PARAMETER_COLUMNS:
                offset = offset + 2
                ind = 0
            parametersIPGroupBoxLayout.addWidget(parameterLabelTemp,ind,offset)
            parametersIPGroupBoxLayout.addWidget(parameterLineEditsTemp[i],ind,1+offset)
            ind = ind + 1

        self.parameterLineEdits.append(parameterLineEditsTemp)

        return parametersIPGroupBoxLayout

    def editControlWord(self):
        cwDialog = controlWordDialog(self.listOfControlWordBits, '{0:0{1}b}'.format(int(self.sender().text()),len(self.listOfControlWordBits))[::-1])
        cwDialog.exec_()
        self.sender().setText(str(int(cwDialog.outputString,2)))
        return

    def createROIBoundsGroupBoxLayout(self, parent, numROIs):
        # create a layout for the ROI Bounds
        parameterROIsTableLayout = QtGui.QGridLayout(parent)
        
        # initialize the lineEdits for storage 
        boundLineEditsTemp = [[]]*numROIs

        roiBoundsDefaultValues = [[0,320,0,320]]*numROIs

        # create the widgets in the ROI bounds table and add them to the layout
        for row in range(numROIs+1):
            rowBoundLineEdits = []
            for col in range(NUM_BOUNDS+1):
                if row == 0:
                    if col == 0:
                        pass
                    else:
                        parameterROIsTableLayout.addWidget(QtGui.QLabel(BOUND_ROI_STRINGS[col-1]),row,col)
                else:
                    if col == 0:
                        parameterROIsTableLayout.addWidget(QtGui.QLabel(ROI_STRINGS[row-1]),row,col)
                    else:
                        rowBoundLineEdits.append(QtGui.QLineEdit(str(roiBoundsDefaultValues[row-1][col-1])))
                        rowBoundLineEdits[col-1].setValidator(self.intValidator)
                        parameterROIsTableLayout.addWidget(rowBoundLineEdits[col-1],row,col)
            boundLineEditsTemp[row-1] = rowBoundLineEdits

        self.boundLineEdits.append(boundLineEditsTemp)
        return parameterROIsTableLayout

    def changeImageSource(self):
        if str(self.imageSourceComboBox.currentText()) == 'Direct IP Connect':
            self.numberOfCamerasComboBox.setEnabled(True)
        elif str(self.imageSourceComboBox.currentText()) == 'Image Directory':
            self.numberOfCamerasComboBox.setCurrentIndex(0)
            self.numberOfCamerasComboBox.setEnabled(False)

    def changeNumberOfCameras(self):
        if self.libs != []:
            for lib in self.libs:
                lib.__exit__()
                lib = None
            self.libs = []    
            gc.collect()

        self.numCameras = int(str(self.numberOfCamerasComboBox.currentText())[0])

        while len(self.cameraLibs) > self.numCameras:
            cameraLib = self.cameraLibs.pop(-1)
            cameraLib.__exit__()
            cameraLib = None
            gc.collect()
        while len(self.cameraLibs) < self.numCameras:
            self.cameraLibs.append(windowsGigECameraLib())
            self.cameraLibs[-1].Initialize()
        self.cameras = []
        self.cameraTypes = []
        self.cameraTypes = []
        self.cameraPayloads =[]
        self.cameraStreams = []
        self.camerasInitiated = False
        self.changeCameras = True
        self.changeNumberOfROIs()


    def changeNumberOfROIs(self):
        self.algoInitiated = False
        self.imageDisplay = []
        self.imageXHistograms = []
        self.imageYHistograms = []
        self.calibrateButtons = []
        self.shutterButtons = []
        self.metricsDisplay = []
        self.crosshairXDeltaDisplay = []
        self.crosshairYDeltaDisplay = []

        if self.changeCameras:
            while self.numCameras > len(self.cameraParameterGroupBoxList):
                self.cameraParameterGroupBoxList.append(self.createCameraParameterGroupBox(len(self.cameraParameterGroupBoxList)))
                self.parametersGroupBoxesSpacingLayout.insertWidget(len(self.cameraParameterGroupBoxList)-1,self.cameraParameterGroupBoxList[-1]) 

            while self.numCameras < len(self.cameraParameterGroupBoxList):
                temp = self.cameraParameterGroupBoxList.pop(-1)
                temp.setParent(None)
                self.numROIsList.pop(-1)
                self.parameterLineEdits.pop(-1)
                self.boundLineEdits.pop(-1)
                self.numberOfROIsComboBoxList.pop(-1)
                self.parameterROIBoundsGroupBoxList.pop(-1)
                self.parameterROIBoundsGroupBoxLayoutList.pop(-1)
                self.imageSaveComboBoxList.pop(-1)
        else:
            self.boundLineEdits = []
            for cam in range(self.numCameras):
                self.numROIsList[cam] = int(str(self.numberOfROIsComboBoxList[cam].currentText())[0])

                QtGui.QWidget().setLayout(self.parameterROIBoundsGroupBoxLayoutList[cam])
                layout = self.createROIBoundsGroupBoxLayout(self.parameterROIBoundsGroupBoxList[cam], self.numROIsList[cam])
                self.parameterROIBoundsGroupBoxLayoutList[cam] = layout

        self.changeCameras = False

        for cam in range(self.numCameras):
            self.numROIsList[cam] = int(str(self.numberOfROIsComboBoxList[cam].currentText())[0])

        if len(self.metricsTabs) < self.numCameras:
            while len(self.metricsTabs) < self.numCameras:
                self.metricsTabs.append(QtGui.QWidget())
                self.metricsTabsLayouts.append(QtGui.QVBoxLayout())
                self.tabWidget.addTab(self.metricsTabs[len(self.metricsTabs)-1], 'Camera ' + str((len(self.metricsTabs)-1)))
        elif len(self.metricsTabs) > self.numCameras:
            while len(self.metricsTabs) > self.numCameras:
                extraTab = self.tabWidget.widget(len(self.metricsTabs))
                self.tabWidget.removeTab(len(self.metricsTabs))
                extra = self.metricsTabs.pop(-1)
                del extraTab
                del extra

        for tab in range(len(self.metricsTabs)):
            QtGui.QWidget().setLayout(self.metricsTabsLayouts[tab])
            layout = self.createMetricsTabLayout(self.metricsTabs[tab], self.numROIsList[tab])
            self.metricsTabsLayouts[tab] = layout
            
        return

    def createMetricsTabLayout(self, parent, numROIsInTab):
        # create a graphics layout to hold the image plots
        graphicsLayout = QtGui.QVBoxLayout()
        # graphicsLayout.addStretch(1)

        tempCalibrateButton = numberedPushButton('Calibrate',len(self.calibrateButtons))
        tempShutterButton = numberedPushButton('Close',len(self.shutterButtons))
        tempCalibrateButton.clicked.connect(self.calibrateCamera)
        tempShutterButton.clicked.connect(self.shutterCamera)
        self.calibrateButtons.append(tempCalibrateButton)
        self.shutterButtons.append(tempShutterButton)

        imageButtonsLayout = QtGui.QHBoxLayout()
        imageButtonsLayout.addWidget(tempCalibrateButton)
        imageButtonsLayout.addWidget(tempShutterButton)
        graphicsLayout.addLayout(imageButtonsLayout)

        imageLayout = QtGui.QGridLayout()
        # create an imageItem 
        imagePlotTemp = imageLabel(IMAGE_PLOT_WIDTH, IMAGE_PLOT_HEIGHT, numROIsInTab)

        imageXHistogramTemp = HistogramPlot(IMAGE_PLOT_WIDTH, HISTOGRAM_HEIGHT)
        imageYHistogramTemp = HistogramPlot(HISTOGRAM_HEIGHT, IMAGE_PLOT_HEIGHT)

        imageLayout.addWidget(imageYHistogramTemp,0,0)
        imageLayout.addWidget(imagePlotTemp,0,1)
        imageLayout.addWidget(imageXHistogramTemp,1,1)
        imageGroupBox = QtGui.QGroupBox('Image')
        imageGroupBox.setLayout(imageLayout)
        graphicsLayout.addWidget(imageGroupBox)

        # graphicsLayout.addStretch(1)
        crosshairXDeltaDisplayTemp = []
        crosshairYDeltaDisplayTemp = []
        crosshairLayout = QtGui.QGridLayout()
        for cross in range(numROIsInTab):
            deltaXLabel = QtGui.QLabel('Delta ' + ROI_STRINGS[cross] + ' CoMx')
            deltaXDisplay = QtGui.QLineEdit()
            deltaXDisplay.setReadOnly(True)
            deltaXDisplay.setFixedSize(LABEL_WIDTH,LABEL_HEIGHT)
            deltaXDisplay.setAlignment(QtCore.Qt.AlignHCenter)
            deltaYLabel = QtGui.QLabel('Delta ' + ROI_STRINGS[cross] + ' CoMy')
            deltaYDisplay = QtGui.QLineEdit()
            deltaYDisplay.setReadOnly(True)
            deltaYDisplay.setFixedSize(LABEL_WIDTH,LABEL_HEIGHT)
            deltaYDisplay.setAlignment(QtCore.Qt.AlignHCenter)

            crosshairXDeltaDisplayTemp.append(deltaXDisplay)
            crosshairYDeltaDisplayTemp.append(deltaYDisplay)

            deltaCrosshairLayout = QtGui.QGridLayout()
            deltaCrosshairLayout.addWidget(deltaXLabel,0,0)
            deltaCrosshairLayout.addWidget(deltaXDisplay,0,1)
            deltaCrosshairLayout.addWidget(deltaYLabel,1,0)
            deltaCrosshairLayout.addWidget(deltaYDisplay,1,1)

            deltaCrosshairGroupBox = QtGui.QGroupBox('Crosshair' + str(cross) + ' (' + ROI_COLORS[cross] + ')')
            deltaCrosshairGroupBox.setLayout(deltaCrosshairLayout)

            crosshairLayout.addWidget(deltaCrosshairGroupBox,cross/2,cross%2)
        # graphicsLayout.addStretch(1)

        self.crosshairXDeltaDisplay.append(crosshairXDeltaDisplayTemp)
        self.crosshairYDeltaDisplay.append(crosshairYDeltaDisplayTemp)

        graphicsLayout.addLayout(crosshairLayout)
        # graphicsLayout.addStretch(1)

        metricsTextLayout = QtGui.QHBoxLayout()
        # metricsTextLayout.addStretch(1)
        # create a table to hold the metrics
        sample = hpImageProcessingOutputSignals()
        metricsDisplayGroupBox = QtGui.QGroupBox('Metrics')
        metricsDisplayTemp = displayTableWidget(sample.getNumberFields(),numROIsInTab+1)
        metricsDisplayLayout = QtGui.QVBoxLayout()
        metricsDisplayLayout.addWidget(metricsDisplayTemp)
        metricsDisplayGroupBox.setLayout(metricsDisplayLayout)
        metricsDisplayGroupBox.resize(900,METRICS_TABLE_HEIGHT)
        metricsTextLayout.addWidget(metricsDisplayGroupBox)
        self.metricsDisplay.append(metricsDisplayTemp)
            
        # create the tab layout to hold the images and metrics
        metricsTabLayout = QtGui.QGridLayout(parent)   
        metricsTabLayout.setSpacing(10)
        metricsTabLayout.setColumnStretch(1,5)
        metricsTabLayout.setColumnStretch(0,1)
        # add the graphics layout and metrics layout
        metricsTabLayout.addLayout(graphicsLayout,0,0)
        metricsTabLayout.addLayout(metricsTextLayout,0,1)
        
        self.imageDisplay.append(imagePlotTemp)
        self.imageXHistograms.append(imageXHistogramTemp)
        self.imageYHistograms.append(imageYHistogramTemp)
        
        return metricsTabLayout
       
    def calibrateCamera(self):
        print "Calibration called"
        if DEBUG:
            self.calibrateButtons[self.sender().num].setText("Ongoing")
            self.calibrateButtons[self.sender().num].setEnabled(False)
            self.libs[self.sender().num].clearCalibrationImage()
        else:
            if self.isConnected:
                if self.cameraTypes[self.sender().num] == 'FLIR':
                    self.cameraLibs[self.sender().num].commandNUC()
                elif self.cameraTypes[self.sender().num] == 'Spir' or self.cameraTypes[self.sender().num] == 'Pyro':
                    self.libs[self.sender().num].clearCalibrationImage()
            else:
                warningMessageBox = QtGui.QMessageBox.question(self, '', 'Camera not connected', QtGui.QMessageBox.Ok)
        return

    def shutterCamera(self):
        if self.isConnected:
            if self.cameraTypes[self.sender().num] == 'FLIR':
                if self.sender().text() == 'Open':
                    self.cameraLibs[self.sender().num].openShutter()
                    self.sender().setText('Close')
                elif self.sender().text() == 'Close':
                    self.cameraLibs[self.sender().num].closeShutter()
                    self.sender().setText('Open')
            elif self.cameraTypes[self.sender().num] == 'Spir' or self.cameraTypes[self.sender().num] == 'Pyro':
                warningMessageBox = QtGui.QMessageBox.question(self, '', 
                    'Spiricon does not support opening and closing shutter', QtGui.QMessageBox.Ok)
        else:
            warningMessageBox = QtGui.QMessageBox.question(self, '', 'Camera not connected', QtGui.QMessageBox.Ok)
        return

    def connectToImages(self):
        if self.imageSourceComboBox.currentText() == 'Direct IP Connect':
            self.connectionType = 'Direct'
            self.connectToCamera()
        elif self.imageSourceComboBox.currentText() == 'Image Directory':
            self.connectionType = 'Directory'
            self.connectToDirectory()
        else:
            print 'Failed getting connection type: ' + self.imageSourceComboBox.currentText()
            exit()

    def connectToDirectory(self):
        if self.isConnected:
            self.updateTimer.stop()
            self.isConnected = False
            self.isReady = True
            self.connectButton.setText('Connect')
            self.statusLabel.setText('Status: Disconnected')
        else:
            self.customDialog = directoryDialog()
            self.customDialog.exec_()

            ok = self.customDialog.ok

            if ok:
                image_dir_str = self.customDialog.imageDir
                self.image_type = self.customDialog.imageType
                self.delimiter = self.customDialog.delimiter
                if os.path.exists(image_dir_str):
                    search_string = os.path.join(image_dir_str, "*." + self.image_type)
                    self.image_files = sorted( glob.glob(search_string))
                else:
                    raise IOError( errno.ENOENT, "Directory: \"" + image_dir_str  + "\" Not Found")
            else:
                self.isReady = False
            self.k = 0
            self.setParameters()
            if self.isReady:
                self.isConnected = True
                self.connectButton.setText('Disconnect')
                self.updateTimer.start(1000/GUI_UPDATE_HZ)
                self.statusLabel.setText('Connected')
                self.saveImageClock = time.time()
            else:
                self.isReady = True

    def connectToCamera(self):
        if self.isConnected:
            self.updateTimer.stop()
            self.frameRateLabel.setText('N/A')
            if not DEBUG:
                if sys.platform[0:5] == 'win32':
                    for cam in range(self.numCameras):
                        # self.cameraLibs[cam].stopAcquisition()
                        self.cameraLibs[cam].disconnectGigEDevice()
                elif sys.platform[0:5] == 'linux':
                    for cam in range(self.numCameras):
                        self.cameras[cam].stop_acquisition()
                print 'Stop Acquisition'
            self.isConnected = False
            self.isReady = True
            self.connectButton.setText('Connect')
            self.statusLabel.setText('Status: Disconnected')
        else:
            if not DEBUG:
                if not self.camerasInitiated:
                    self.cameraDialog = connectDialog(self.numCameras, self.cameraLibs)
                    if self.cameraDialog.camerasAvailable:
                        self.cameraDialog.exec_()
                    if self.cameraDialog.ok:
                        if sys.platform[0:5] == 'win32':
                            for cam in range(self.numCameras):
                                self.cameraLibs[cam].connectGigEDevice(self.cameraDialog.listOfDeviceLists[cam].index(self.cameraDialog.detectedCamerasComboBoxes[cam].currentText()))
                                self.cameraTypes.append(str(self.cameraDialog.detectedCamerasComboBoxes[cam].currentText())[0:4])
                                if self.cameraTypes[cam] == 'FLIR':
                                    self.cameraLibs[cam].setPixelFormat(14)
                                elif self.cameraTypes[cam] == 'Spir' or self.cameraTypes[cam] == 'Pyro':
                                    print self.cameraTypes[cam]
                                    self.cameraLibs[cam].setPixelFormat(16)
                                else:
                                    'Camera Not Supported'
                                    exit()

                        elif sys.platform[0:5] == 'linux':
                            for cam in range(self.numCameras):
                                self.cameras.append(Aravis.Camera.new(str(self.cameraDialog.detectedCamerasComboBoxes[cam].currentText())))
                                self.cameraTypes.append(self.cameras[cam].get_vendor_name()[0:4])
                                if self.cameraTypes[cam] == 'FLIR':
                                    self.cameras[cam].set_pixel_format(Aravis.PIXEL_FORMAT_MONO_14)
                                    self.cameras[cam].set_region(0,0,FLIR_IMAGE_WIDTH,FLIR_IMAGE_HEIGHT)
                                    self.cameras[cam].set_frame_rate(FLIR_FRAME_RATE)
                                elif self.cameraTypes[cam] == 'Spir' or self.cameraTypes[cam] == 'Pyro':
                                    self.cameras[cam].set_pixel_format(Aravis.PIXEL_FORMAT_MONO_16)
                                    self.cameras[cam].set_region(0,0,PYRO_IMAGE_WIDTH,PYRO_IMAGE_HEIGHT)
                                    self.cameras[cam].set_frame_rate(PYRO_FRAME_RATE)
                                else:
                                    'Camera Not Supported'
                                    exit()

                                self.cameraStreams.append(self.cameras[cam].create_stream(None, None))

                                self.cameraPayloads.append(self.cameras[cam].get_payload())

                                # [x,y,width,height] = self.cameras[cam].get_region()

                                # print "Camera vendor : %s" %(self.cameras[cam].get_vendor_name())
                                # print "Camera model  : %s" %(self.cameras[cam].get_model_name())
                                # print "Camera id     : %s" %(self.cameras[cam].get_device_id())
                                # print "Camera FoV    : %dx%d at %d,%d" %(width, height, x, y)
                                # print "Payload       : %d bytes" %(self.cameraPayloads[cam])
                                # print "Pixel format  : %s" %(self.cameras[cam].get_pixel_format_as_string())
                            self.camerasInitiated = True
                        self.setParameters()
                    else:
                        self.isReady = False

            else:
                self.setParameters()

            self.k = 0       
            if self.isReady:
                if not DEBUG:
                    if sys.platform[0:5] == 'win32':
                        for cam in range(self.numCameras):
                            self.cameraLibs[cam].enableStreaming()
                            # self.cameraLibs[cam].startAcquisition()
                    elif sys.platform[0:5] == 'linux':
                        for cam in range(self.numCameras):
                            self.cameras[cam].start_acquisition()
                    print "Start Acquisition"
                self.isConnected = True
                self.connectButton.setText('Disconnect')
                self.updateTimer.start(1000/GUI_UPDATE_HZ)
                self.statusLabel.setText('Connected')
                self.lastTime = time.clock()
                self.saveImageClock = time.time()
            else:
                self.isReady = True

    def setParameters(self):
        self.saveDir = os.path.join(os.path.join(CUR_DIR,'GUI_Saves'),time.strftime("%Y%m%d_%H%M%S"))
        if not os.path.exists(self.saveDir):
            os.makedirs(self.saveDir)
        
        self.save_writers = []
        for cam in range(self.numCameras):
            for num in range(self.numROIsList[cam]):
                save_file = open(os.path.join(self.saveDir,'CAM'+str(cam)+'_ROI'+str(num)+'_metrics.csv'),'wb')
                self.save_writers.append(csv.writer(save_file, delimiter = ','))

        self.h5pyFiles = []
        for cam in range(self.numCameras):
            if self.imageSaveComboBoxList[cam].currentText() == 'Save Raw Images':
                if self.cameras != []:
                    self.h5pyFiles.append(h5py.File(os.path.join(self.saveDir,Aravis.get_device_id(cam)+'.hdf5'),'w'))
                else:
                    self.h5pyFiles.append(h5py.File(os.path.join(self.saveDir,'imageProGUI_CAM'+str(cam)+'.hdf5'), 'w'))

        if self.algoOnComboBox.currentText() == 'IP Algo On':
            self.algoOn = True
        else:
            self.algoOn = False


        roiLowerLimits = []
        roiUpperLimits = []
        if DEBUG:
            for cam in range(self.numCameras):
                roiLowerLimits.append([[0,0,0,0]]*self.numROIsList[cam])
                roiUpperLimits.append([[320,320,320,320]]*self.numROIsList[cam])
        else:
            if self.connectionType == 'Direct':
                if self.isReady:
                    for cam in range(self.numCameras):
                        roiLowerLimits.append([[0,0,0,0]]*self.numROIsList[cam])
                        if self.cameraTypes[cam] == 'FLIR':
                            roiUpperLimits.append([[FLIR_IMAGE_WIDTH,FLIR_IMAGE_WIDTH,FLIR_IMAGE_HEIGHT,FLIR_IMAGE_HEIGHT]]*self.numROIsList[cam])
                        elif self.cameraTypes[cam] == 'Spir' or self.cameraTypes[cam] == 'Pyro': 
                            # TODO - need to differentiate between Pyrocam IIIHR and IV
                            roiUpperLimits.append([[PYRO_IMAGE_WIDTH,PYRO_IMAGE_WIDTH,PYRO_IMAGE_HEIGHT,PYRO_IMAGE_HEIGHT]]*self.numROIsList[cam])
            elif self.connectionType == 'Directory':
                if self.isReady:
                    roiLowerLimits.append([[0,0,0,0]]*self.numROIsList[0])
                    if self.image_type == 'csv':
                        try:
                            f = open(self.image_files[0],'rb')
                            reader = csv.reader(f,delimiter=self.delimiter)
                            data = []
                            for i,row in enumerate(reader):
                                if row[-1] == '':
                                    data.append(row[:-1])
                                else:
                                    data.append(row)
                            sample = np.array(data)
                            nrows, ncols = np.shape(sample)
                        finally:
                            f.close()
                    else:
                        sample = misc.imread(self.image_files[0])
                        nrows, ncols = np.shape(sample)
                    roiUpperLimits.append([[ncols,ncols,nrows,nrows]]*self.numROIsList[0])

        if self.isReady:
            if self.imageDisplayComboBox.currentText() == 'Raw Image':
                self.returnRawImage = True
            elif self.imageDisplayComboBox.currentText() == 'Processed Image':
                self.returnRawImage = False
            else:
                self.isReady = False
        
        if self.isReady:
            self.parameters = []
            for cam in range(self.numCameras):
                parametersTemp = {}
                for i in range(len(self.parameterStrings)):
                    if (self.parameterStrings[i] == 'PROBE_SIGNAL_DATA' or self.parameterStrings[i] == 'CENTROID_BOT_BOUND' or self.parameterStrings[i] == 'CENTROID_TOP_BOUND'
                        or self.parameterStrings[i] == 'BEAM_DIAMETER_BOT_BOUND' or self.parameterStrings[i] == 'BEAM_DIAMETER_TOP_BOUND' or self.parameterStrings[i] == 'DIVERGENCE_BOT_BOUND'
                        or self.parameterStrings[i] == 'DIVERGENCE_TOP_BOUND'):
                        try:
                            floatarray = [float(x) for x in str(self.parameterLineEdits[cam][i].text()).replace('[','').replace(']','').split(',')]
                        except ValueError:
                            warningMessageBox = QtGui.QMessageBox.question(self, '', 'One of the entries was not a float.', QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        if len(floatarray) < 2*self.numROIsList[cam] and self.parameterStrings[i] != 'PROBE_SIGNAL_DATA':
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                self.parameterStrings[i] + ' requires more parameters. Number of ROIs: ' + str(self.numROIsList[cam]) + '.', QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        for dataNum in floatarray: 
                            if dataNum < self.parameterLowerLimits[i]:
                                warningMessageBox = QtGui.QMessageBox.question(self, '',
                                    self.parameterStrings[i] + ' is set too low. Lower Limit = ' 
                                    + str(self.parameterLowerLimits[i]), QtGui.QMessageBox.Ok)
                                self.isReady = False
                                break
                            elif dataNum > self.parameterUpperLimits[i]:
                                warningMessageBox = QtGui.QMessageBox.question(self, '',
                                    self.parameterStrings[i] + ' is set too high. Upper Limit = ' 
                                    + str(self.parameterUpperLimits[i]), QtGui.QMessageBox.Ok)
                                self.isReady = False
                                break
                        if self.isReady:
                            parametersTemp[self.parameterStrings[i]] = floatarray
                    else:
                        if str(self.parameterLineEdits[cam][i].text()).strip() == '':
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                self.parameterStrings[i] + ' is set to nothing.', QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        elif float(self.parameterLineEdits[cam][i].text()) < self.parameterLowerLimits[i]:
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                self.parameterStrings[i] + ' is set too low. Lower Limit = ' 
                                + str(self.parameterLowerLimits[i]), QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break

                        elif float(self.parameterLineEdits[cam][i].text()) > self.parameterUpperLimits[i]:
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                self.parameterStrings[i] + ' is set too high. Upper Limit = ' 
                                + str(self.parameterUpperLimits[i]), QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        else:
                            if i == 0:
                                parametersTemp[self.parameterStrings[i]] = int(self.parameterLineEdits[cam][i].text())
                            elif i in self.intValidatorList:
                                parametersTemp[self.parameterStrings[i]] = int(self.parameterLineEdits[cam][i].text())
                            else:
                                parametersTemp[self.parameterStrings[i]] = float(self.parameterLineEdits[cam][i].text())
                self.parameters.append(parametersTemp)
        
        if self.isReady:    
            self.ROIBounds = []
            self.roiInfo = []
            for cam in range(self.numCameras):
                roiBoundsTemp = []
                roiInfoTemp = {}
                roiInfoTemp['numROIs'] = self.numROIsList[cam]
                roiInfoTemp['xBoundLeft'] = []
                roiInfoTemp['xBoundRight'] = []
                roiInfoTemp['yBoundBottom'] = []
                roiInfoTemp['yBoundTop'] = []
                roiInfoTemp['intensity'] = ROI_INTENS
                roiInfoTemp['centroidPadding'] = CENT_IMAGE_PADDING
                for ROI in range(self.numROIsList[cam]):
                    for bound in range(NUM_BOUNDS):
                        if str(self.boundLineEdits[cam][ROI][bound].text()).strip() == '':
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                BOUND_ROI_STRINGS[bound] + ' of ' + ROI_STRINGS[ROI] 
                                + ' ROI is set to nothing.', QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        elif float(self.boundLineEdits[cam][ROI][bound].text()) < roiLowerLimits[cam][ROI][bound]:
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                BOUND_ROI_STRINGS[bound] + ' of ' + ROI_STRINGS[ROI] 
                                + ' ROI is set below the minimum (%d).' % roiLowerLimits[cam][ROI][bound], QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                        elif float(self.boundLineEdits[cam][ROI][bound].text()) > roiUpperLimits[cam][ROI][bound]:
                            warningMessageBox = QtGui.QMessageBox.question(self, '',
                                BOUND_ROI_STRINGS[bound] + ' of ' + ROI_STRINGS[ROI] 
                                + ' ROI is set above the maximum (%d).' % roiUpperLimits[cam][ROI][bound], QtGui.QMessageBox.Ok)
                            self.isReady = False
                            break
                    if float(self.boundLineEdits[cam][ROI][0].text()) == float(self.boundLineEdits[cam][ROI][1].text()):
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'Left and Right Bounds of ' + ROI_STRINGS[ROI] + ' ROI are equal.', QtGui.QMessageBox.Ok)
                        self.isReady = False
                        break
                    elif float(self.boundLineEdits[cam][ROI][0].text()) > float(self.boundLineEdits[cam][ROI][1].text()):
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'Left Bound of ' + ROI_STRINGS[ROI] + ' ROI is greater than the right bound.', QtGui.QMessageBox.Ok)
                        self.isReady = False
                        break
                    elif float(self.boundLineEdits[cam][ROI][2].text()) == float(self.boundLineEdits[cam][ROI][3].text()):
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'Bottom and Top Bounds of ROI ' +  ROI_STRINGS[ROI] + ' are equal.', QtGui.QMessageBox.Ok)
                        self.isReady = False            
                        break
                    elif float(self.boundLineEdits[cam][ROI][2].text()) > float(self.boundLineEdits[cam][ROI][3].text()):
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'Bottom Bound of ' + ROI_STRINGS[i] + ' ROI is greater than the top bound.', QtGui.QMessageBox.Ok)
                        self.isReady = False
                        break
                    else:
                        roiBoundsTemp.append([int(self.boundLineEdits[cam][ROI][0].text()),
                                                            int(self.boundLineEdits[cam][ROI][1].text()),
                                                            int(self.boundLineEdits[cam][ROI][2].text()),
                                                            int(self.boundLineEdits[cam][ROI][3].text())])
                        roiInfoTemp['xBoundLeft'].append(int(self.boundLineEdits[cam][ROI][0].text()))
                        roiInfoTemp['xBoundRight'].append(int(self.boundLineEdits[cam][ROI][1].text()))
                        roiInfoTemp['yBoundBottom'].append(int(self.boundLineEdits[cam][ROI][2].text()))
                        roiInfoTemp['yBoundTop'].append(int(self.boundLineEdits[cam][ROI][3].text()))

                self.ROIBounds.append(roiBoundsTemp)
                self.roiInfo.append(roiInfoTemp)
        
        if not self.algoInitiated:
            for cam in range(self.numCameras):
                self.libs.append(hpImageProcessingLib(MAX_IMAGE_HEIGHT, MAX_IMAGE_WIDTH))    
                self.libs[cam].Initialize()
                self.algoInitiated = True

        if self.isReady: 
            for cam in range(self.numCameras):
                for num in range(self.numROIsList[cam]):
                    self.libs[cam].SetParamUInt(0, np.uint32(self.numROIsList[cam]))
                    self.libs[cam].SetArrayByIndex(1, ctypes.c_float(self.parameters[cam]['CONTROL_WORD']), num)
                    self.libs[cam].SetParamFloat(2, ctypes.c_float(self.parameters[cam]['LOWPASS_SMOOTHING_FACTOR']))
                    self.libs[cam].SetParamUInt(3, np.uint32(self.parameters[cam]['MED_FILT_PADDING']))
                    self.libs[cam].SetArrayByIndex(4, ctypes.c_float(self.parameters[cam]['THRESHOLD_CUTOFF']), num)
                    self.libs[cam].SetArrayByIndex(5, ctypes.c_float(self.parameters[cam]['LEAST_SQUARES_CUTOFF']), num)
                    self.libs[cam].SetParamFloat(6, ctypes.c_float(self.parameters[cam]['LEAST_SQUARES_FIT_DELTA']))
                    self.libs[cam].SetArrayByIndex(7, ctypes.c_float(self.parameters[cam]['SUB_ROI_RADIUS']), num)
                    self.libs[cam].SetParamUInt(8, np.uint32(self.parameters[cam]['SYM_RADIAL_LINES']))
                    self.libs[cam].SetArrayByIndex(9, ctypes.c_float(self.parameters[cam]['SYM_RADIAL_DISTANCE']), num)
                    self.libs[cam].SetParamFloat(10, ctypes.c_float(self.parameters[cam]['COM_VALID_DISTANCE']))
                    self.libs[cam].SetParamUInt(11, np.uint32(self.parameters[cam]['NUM_IMAGES_TO_SUM']))
                    self.libs[cam].SetArrayByIndex(12, ctypes.c_float(self.ROIBounds[cam][num][0]), num)
                    self.libs[cam].SetArrayByIndex(13, ctypes.c_float(self.ROIBounds[cam][num][2]), num)
                    self.libs[cam].SetArrayByIndex(14, ctypes.c_float(self.ROIBounds[cam][num][1] - self.ROIBounds[cam][num][0]), num)
                    self.libs[cam].SetArrayByIndex(15, ctypes.c_float(self.ROIBounds[cam][num][3] - self.ROIBounds[cam][num][2]), num)
                    self.libs[cam].SetParamFloat(16, ctypes.c_float(self.parameters[cam]['WATT_PER_COUNT']))
                    self.libs[cam].SetParamFloat(17, ctypes.c_float(self.parameters[cam]['FOCAL_LENGTH']))
                    self.libs[cam].SetParamFloat(18, ctypes.c_float(self.parameters[cam]['MM_PER_PIXEL']))
                    # self.libs[cam].SetArrayByIndex(21, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_X_OFFSET']), num)
                    # self.libs[cam].SetArrayByIndex(22, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_Y_OFFSET']), num)
                    # self.libs[cam].SetArrayByIndex(23, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_WIDTH']), num)
                    # self.libs[cam].SetArrayByIndex(24, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_HEIGHT']), num)
                    # self.libs[cam].SetArrayByIndex(25, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_MAJOR_RAD']), num)
                    # self.libs[cam].SetArrayByIndex(26, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_MINOR_RAD']), num)
                    # self.libs[cam].SetArrayByIndex(27, ctypes.c_float(self.parameters[cam]['POWER_DENSITY_ANGLE']), num)
                    # self.libs[cam].SetArrayByIndex(28, ctypes.c_float(self.parameters[cam]['EFFECTIVE_IRRADIATION_THRESHOLD']), num)
                    # self.libs[cam].SetArrayByIndex(29, ctypes.c_float(self.parameters[cam]['BEAM_POSITIONAL_STABILITY_LOWPASS']), num)
                    # self.libs[cam].SetArrayByIndex(30, ctypes.c_float(self.parameters[cam]['R_THETA_TOP_HAT_NUM_LINES']), num)
                    # self.libs[cam].SetArrayByIndex(31, ctypes.c_float(self.parameters[cam]['R_THETA_TOP_HAT_PERCENT_LEVEL']), num)
                    self.libs[cam].SetArrayByIndex(19, ctypes.c_float(self.parameters[cam]['ELLIPTICAL_SUB_ROI_PADDING']), num)   
                    for subInd in range(len(self.parameters[cam]['CENTROID_BOT_BOUND'])):
                        self.libs[cam].SetArrayByIndex(20, ctypes.c_float(self.parameters[cam]['CENTROID_BOT_BOUND'][subInd]),subInd)
                    for subInd in range(len(self.parameters[cam]['CENTROID_TOP_BOUND'])):
                        self.libs[cam].SetArrayByIndex(21, ctypes.c_float(self.parameters[cam]['CENTROID_TOP_BOUND'][subInd]),subInd)
                    for subInd in range(len(self.parameters[cam]['BEAM_DIAMETER_BOT_BOUND'])):
                        self.libs[cam].SetArrayByIndex(22, ctypes.c_float(self.parameters[cam]['BEAM_DIAMETER_BOT_BOUND'][subInd]),subInd)
                    for subInd in range(len(self.parameters[cam]['BEAM_DIAMETER_TOP_BOUND'])):
                        self.libs[cam].SetArrayByIndex(23, ctypes.c_float(self.parameters[cam]['BEAM_DIAMETER_TOP_BOUND'][subInd]),subInd)
                    for subInd in range(len(self.parameters[cam]['DIVERGENCE_BOT_BOUND'])):
                        self.libs[cam].SetArrayByIndex(24, ctypes.c_float(self.parameters[cam]['DIVERGENCE_BOT_BOUND'][subInd]),subInd)
                    for subInd in range(len(self.parameters[cam]['DIVERGENCE_TOP_BOUND'])):
                        self.libs[cam].SetArrayByIndex(25, ctypes.c_float(self.parameters[cam]['DIVERGENCE_TOP_BOUND'][subInd]),subInd) 
                    self.libs[cam].SetArrayByIndex(26, ctypes.c_float(self.parameters[cam]['GAUSS_PERF_TOP_BOUND']),num) 
                    self.libs[cam].SetArrayByIndex(27, ctypes.c_float(self.parameters[cam]['SYMMETRY_TOP_BOUND']),num)
                    self.libs[cam].SetArrayByIndex(28, ctypes.c_float(self.parameters[cam]['PEARSON_BOT_BOUND']),num)
                    self.libs[cam].SetArrayByIndex(29, ctypes.c_float(self.parameters[cam]['MAX_INTENSITY_BOT_BOUND']),num)
                    self.libs[cam].SetArrayByIndex(30, ctypes.c_float(self.parameters[cam]['MAX_INTENSITY_TOP_BOUND']),num)
                    self.libs[cam].SetParamFloat(31, ctypes.c_float(self.parameters[cam]['PROBE_ROI']))
                    self.libs[cam].SetParamFloat(32, ctypes.c_float(self.parameters[cam]['PROBE_LOCATION']))
                    self.libs[cam].SetParamFloat(33, ctypes.c_float(self.parameters[cam]['PROBE_MODE']))
                    self.libs[cam].SetParamFloat(34, ctypes.c_float(self.parameters[cam]['PROBE_SIGNAL_PERIOD']))
                    self.libs[cam].SetParamFloat(35, ctypes.c_float(self.parameters[cam]['PROBE_SIGNAL_AMPLITUDE']))
                    self.libs[cam].SetParamFloat(36, ctypes.c_float(self.parameters[cam]['PROBE_SIGNAL_OFFSET']))
                    for subInd in range(len(self.parameters[cam]['PROBE_SIGNAL_DATA'])):
                        self.libs[cam].SetArrayByIndex(37, ctypes.c_float(self.parameters[cam]['PROBE_SIGNAL_DATA'][subInd]),subInd)

        self.first = True

    # def startStopCameraAcquisition(self):
    #     if str(self.acquisitionButton.text()) == 'Start':
    #         if not DEBUG:
    #             for cam in range(self.numCameras):
    #                 self.cameraLibs[cam].startAcquisition()
    #         self.updateTimer.start(1000/GUI_UPDATE_HZ)
    #         self.acquisitionButton.setText('Stop')
    #     else:
    #         if not DEBUG:
    #             for cam in range(self.numCameras):
    #                 self.cameraLibs[cam].stopAcquisition()
    #         self.updateTimer.stop()
    #         self.acquisitionButton.setText('Start')

    def update(self):
        start_time = time.clock()
        if DEBUG:
            # rgb_image = misc.imread(os.path.join(CUR_DIR,r'testImages/noCrosshair1.png'))

            sineAmp = 0.1

            nrows = 320
            ncols = 320
            y_2d, x_2d = np.mgrid[:nrows,:ncols]

            amp = 10000;
            theta = 0*math.pi/180
            Tmat = np.array([[math.cos(theta),math.sin(theta)],[-math.sin(theta),math.cos(theta)]])
            VOC0 = np.array([[(100.0/4)**2,0],[0,(80.0/4)**2]])
            # VOC0 = np.array([[1600,0],[0,900]])
            VOC = lin.inv(np.dot(np.dot(Tmat.T,VOC0),Tmat))
            x0 = 180
            y0 = 120
            alpha = 0.1

            y_1d = y_2d.ravel()
            vecX = np.array([x_2d.ravel()-x0,y_1d-y0])
            # z0 = np.sqrt(lin.det(VOC))/2/np.pi*np.exp(-0.5*(VOC[0][0]*vecX[0]**2+2*VOC[0][1]*vecX[0]*vecX[1]+VOC[1][1]*vecX[1]**2))
            # z0 = amp*np.exp(-0.5*(VOC[0][0]*vecX[0]**2+2*VOC[0][1]*vecX[0]*vecX[1]+VOC[1][1]*vecX[1]**2))*(1+sineAmp*np.sin(2*np.pi/10*vecX[0]))
            z0 = amp*np.exp(-0.5*(VOC[0][0]*vecX[0]**2+2*VOC[0][1]*vecX[0]*vecX[1]+VOC[1][1]*vecX[1]**2))*(1+sineAmp*np.sin(2*np.pi/10*vecX[0]))*(1+special.erf(alpha*vecX[0]/math.sqrt(2)))
            z0 = np.reshape(z0,(nrows,ncols))
            rgb_image = np.array(z0)

            # amp = 32000;
            # amp = 10000;
            # theta = 20*math.pi/180
            # Tmat = np.array([[math.cos(theta),math.sin(theta)],[-math.sin(theta),math.cos(theta)]])
            # VOC0 = np.array([[(80.0/4)**2,0],[0,(50.0/4)**2]])
            # # VOC0 = np.array([[1600,0],[0,900]])
            # VOC = lin.inv(np.dot(np.dot(Tmat.T,VOC0),Tmat))            # x0 = 200
            # y0 = 200

            # y_1d = y_2d.ravel()
            # vecX = np.array([x_2d.ravel()-x0,y_1d-y0])
            # # z0 = np.sqrt(lin.det(VOC))/2/np.pi*np.exp(-0.5*(VOC[0][0]*vecX[0]**2+2*VOC[0][1]*vecX[0]*vecX[1]+VOC[1][1]*vecX[1]**2))
            # z1 = amp*np.exp(-0.5*(VOC[0][0]*vecX[0]**2+2*VOC[0][1]*vecX[0]*vecX[1]+VOC[1][1]*vecX[1]**2))
            # z1 = np.reshape(z1,(nrows,ncols))
            # rgb_image = np.array(z1) + np.array(z0)

            # noise = np.random.randint(-250,250,(nrows,ncols))
            noise = np.random.randint(-250,250,(nrows,ncols))
            # rgb_image = noise
            rgb_image = rgb_image + noise

            for cam in range(self.numCameras):
                self.algoDisplaySave(rgb_image, cam)
        else:
            if self.connectionType == 'Direct':
                try:
                    if sys.platform[0:5] == 'win32':
                        for cam in range(self.numCameras):
                            rgb_image = DumpBuffer(self.cameraLibs[cam].receiveGigEBuffer(), self.cameraLibs[cam].getBufferSize(), self.cameraTypes[cam])
                            self.algoDisplaySave(rgb_image, cam)
                    elif sys.platform[0:5] == 'linux':
                        for cam in range(self.numCameras):
                            self.cameraStreams[cam].push_buffer(Aravis.Buffer.new_allocate(self.cameraPayloads[cam]))
                            buffer = self.cameraStreams[cam].pop_buffer()
                            if buffer:
                                cameraData = buffer.get_data()
                                rgb_image = DumpBuffer(cameraData, len(cameraData), self.cameraTypes[cam])
                                self.algoDisplaySave(rgb_image, cam)
                except AttributeError:
                    print "Error. Most likely two instances of Aravis connecting to same camera."
                    print 'Close other instance, and try relaunching.'
                    exit()
                except:
                    print "Unexpected error:", sys.exc_info()[0]
                    print traceback.print_exc(file=sys.stdout)
                    print 'Please copy error and report problem.'
                    exit()
            elif self.connectionType == 'Directory':
                print 'Image [%d/%d]' % (self.k, len(self.image_files))
                if self.image_type == 'csv':
                    if self.k < (len(self.image_files)-1):
                        try:
                            f = open(self.image_files[self.k],'rb')
                            reader = csv.reader(f,delimiter=self.delimiter)
                            data = []
                            for i,row in enumerate(reader):
                                if row[-1] == '':
                                    data.append(row[:-1])
                                else:
                                    data.append(row)
                            image = np.array(data)
                            self.algoDisplaySave(image)
                        finally:
                            f.close()
                    else:
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'All Images Displayed', QtGui.QMessageBox.Ok)
                        self.connectToDirectory()
                else:
                    if self.k < (len(self.image_files)-1):
                        self.algoDisplaySave(misc.imread(self.image_files[self.k]))
                    else:
                        warningMessageBox = QtGui.QMessageBox.question(self, '',
                            'All Images Displayed', QtGui.QMessageBox.Ok)
                        self.connectToDirectory()

        newTime = time.clock()
        self.frameRateLabel.setText(str(1/(newTime-self.lastTime)))
        self.lastTime = newTime

                
    def algoDisplaySave(self, rgb_image, cam=0):
        orig_image = checkRGB(rgb_image)
        # full_image = np.int64(np.flipud(orig_image))
        orig_image = np.float64(orig_image)

        histX = np.max(orig_image,axis=0)
        countX = range(len(histX))

        histY = np.max(orig_image,axis=1)
        countY = range(len(histY))

        nrows, ncols = np.shape(orig_image)
        c_float_p = ctypes.POINTER(ctypes.c_float)
        c_uint16_p = ctypes.POINTER(ctypes.c_uint16)
        c_int16_p = ctypes.POINTER(ctypes.c_int16)

        image = np.round(orig_image)
        image = image.astype(np.int16)
        fImage = image.flatten().astype(ctypes.c_int16)
        pImage = fImage.ctypes.data_as(c_int16_p)
        if self.algoOn:
            pReturnImage = self.libs[cam].processImageIntArray(pImage, nrows, ncols)
            # self.libs[cam].runImageProcessing()
            pReturnImage = self.libs[cam].getCalibratedImage()

        self.roiInfo[cam]['centroidX'] = []
        self.roiInfo[cam]['centroidY'] = []

        for num in range(self.numROIsList[cam]):
            phpImageProcessingOutputSignals = self.libs[cam].getOutputSignals(num)

            metricsData = {}

            for key in phpImageProcessingOutputSignals.contents.getFieldNames():
                metricsData[key] = eval('phpImageProcessingOutputSignals.contents.'+key)
            metricsData = OrderedDict(sorted(metricsData.items(), key=lambda t: t[0]))

            metricsData['simpleCentroidX'] += self.ROIBounds[cam][num][0]
            metricsData['simpleCentroidY'] += self.ROIBounds[cam][num][2]
            metricsData['simpleCentroidU'] += self.ROIBounds[cam][num][0]
            metricsData['simpleCentroidV'] += self.ROIBounds[cam][num][2]
            metricsData['rawMaxIndX'] += self.ROIBounds[cam][num][0]
            metricsData['rawMaxIndY'] += self.ROIBounds[cam][num][2]

            # print 'Act: ', self.act, ' , Calc: ', metricsData['simpleCentroidX']

            self.roiInfo[cam]['centroidX'].append(metricsData['simpleCentroidU'])
            self.roiInfo[cam]['centroidY'].append(metricsData['simpleCentroidV'])
        
            control = format(self.parameters[cam]['CONTROL_WORD'], '#016b')
            if control[4] == '1':
                metricsData['lsFitCentroidX'] += self.ROIBounds[cam][num][0]
                metricsData['lsFitCentroidY'] += self.ROIBounds[cam][num][2]  

            deltaX = self.imageDisplay[cam].crosshairX[num] - metricsData['simpleCentroidU']
            deltaY = self.imageDisplay[cam].crosshairY[num] - metricsData['simpleCentroidV']
            self.crosshairXDeltaDisplay[cam][num].setText('%.4f' % (deltaX))
            self.crosshairYDeltaDisplay[cam][num].setText('%.4f' % (deltaY))

            self.metricsDisplay[cam].setDataCol(metricsData,num+1)

            saveHeader = list(metricsData.keys())
            saveValues = list(metricsData.values())
            saveHeader.insert(0,'Frame Number')
            if self.connectionType == 'Direct':
                saveValues.insert(0,'Frame%09d' % (self.k))
            elif self.connectionType == 'Directory':
                saveValues.insert(0,self.image_files[self.k])
            
            if self.first == True:
                self.save_writers[num].writerow(saveHeader)
            self.save_writers[num].writerow(saveValues)

        save_image_h5py = np.copy(orig_image)
        if self.imageSaveComboBoxList[cam].currentText() == 'Save Raw Images':
            if DEBUG:
                save_image_h5py = np.int16(save_image_h5py)
                dset = self.h5pyFiles[0].create_dataset('Frame%09d' % (self.k), data=save_image_h5py, compression='gzip', compression_opts=6)
                dset.attrs["CLASS"] = np.string_("IMAGE")
                dset.attrs["IMAGE_SUBCLASS"] = np.string_("IMAGE_GRAYSCALE")
                dset.attrs["IMAGE_WHITE_IS_ZERO"] = np.array(0, dtype="int16")
                dset.attrs["IMAGE_MINMAXRANGE"] = [save_image_h5py.min(), save_image_h5py.max()]
                dset.attrs["DISPLAY_ORIGIN"] = np.string_("UL") # not rotated
                dset.attrs["IMAGE_VERSION"] = np.string_("1.2")
            else:
                if self.cameraTypes[cam] == 'FLIR':
                    save_image_h5py = np.uint16(save_image_h5py)
                    dset = self.h5pyFiles[0].create_dataset('Frame%09d' % (self.k), data=save_image_h5py, compression='gzip', compression_opts=6)
                    dset.attrs["CLASS"] = np.string_("IMAGE")
                    dset.attrs["IMAGE_SUBCLASS"] = np.string_("IMAGE_GRAYSCALE")
                    dset.attrs["IMAGE_WHITE_IS_ZERO"] = np.array(0, dtype="uint16")
                    dset.attrs["IMAGE_MINMAXRANGE"] = [save_image_h5py.min(), save_image_h5py.max()]
                    dset.attrs["DISPLAY_ORIGIN"] = np.string_("UL") # not rotated
                    dset.attrs["IMAGE_VERSION"] = np.string_("1.2")
                elif self.cameraTypes[cam] == 'Spir' or self.cameraTypes[cam] == 'Pyro':
                    save_image_h5py = np.int16(save_image_h5py)
                    dset = self.h5pyFiles[0].create_dataset('Frame%09d' % (self.k), data=save_image_h5py, compression='gzip', compression_opts=6)
                    dset.attrs["CLASS"] = np.string_("IMAGE")
                    dset.attrs["IMAGE_SUBCLASS"] = np.string_("IMAGE_GRAYSCALE")
                    dset.attrs["IMAGE_WHITE_IS_ZERO"] = np.array(0, dtype="int16")
                    dset.attrs["IMAGE_MINMAXRANGE"] = [save_image_h5py.min(), save_image.max()]
                    dset.attrs["DISPLAY_ORIGIN"] = np.string_("UL") # not rotated
                    dset.attrs["IMAGE_VERSION"] = np.string_("1.2")
        
        save_image_png = np.copy(orig_image)
        if self.connectionType == 'Direct':
            if time.time() - self.saveImageClock > 3600:
                save_image_png = np.uint16(save_image_png)
                misc.imsave(os.path.join(self.saveDir, 'FrameOnHour%05d.png' % (self.k)), save_image_png)
                self.saveImageClock = time.time()

        self.metricsDisplay[cam].setHeaderCol(metricsData)

        self.roiInfo[cam]['crosshairX'] = []
        self.roiInfo[cam]['crosshairY'] = []
        self.roiInfo[cam]['crosshairRad'] = []
        self.roiInfo[cam]['crosshairAng'] = []

        for cross in range(self.numROIsList[cam]):
            self.roiInfo[cam]['crosshairX'].append(self.imageDisplay[cam].crosshairX[cross])
            self.roiInfo[cam]['crosshairY'].append(self.imageDisplay[cam].crosshairY[cross])
            self.roiInfo[cam]['crosshairRad'].append(self.imageDisplay[cam].crosshairRad[cross])
            self.roiInfo[cam]['crosshairAng'].append(self.imageDisplay[cam].crosshairAng[cross])

        if self.returnRawImage or not self.algoOn:
            # 16 bit implementation is much slower while visually they look similar
            orig_image = np.uint16(orig_image)
            self.imageDisplay[cam].setImage(qPixmap8bit(orig_image, self.roiInfo[cam]))
        else:
            array_pointer = ctypes.cast(pReturnImage, ctypes.POINTER(ctypes.c_uint16*(nrows*ncols)))
            returnImage = np.reshape(np.frombuffer(array_pointer.contents, dtype='uint16'),(nrows,ncols))
            self.imageDisplay[cam].setImage(qPixmap8bit(returnImage, self.roiInfo[cam]))

        self.imageXHistograms[cam].curve.setData(countX, histX)
        self.imageXHistograms[cam].setAxisScale(Qwt.QwtPlot.xBottom, 0.0, len(histX))
        self.imageXHistograms[cam].replot()

        self.imageYHistograms[cam].curve.setData(histY[::-1], countY)
        self.imageYHistograms[cam].setAxisScale(Qwt.QwtPlot.yLeft, 0.0, len(histY))
        self.imageYHistograms[cam].replot()
        self.first = False
        self.k += 1

    def closeEvent(self, event):
        try:
            if self.isConnected:
                self.updateTimer.stop()
                # if sys.platform[0:5] == 'win32':
                #     for cam in range(self.numCameras):
                #         self.cameraLibs[cam].stopAcquisition()
                #         self.cameraLibs[cam].disconnectGigEDevice()
                # elif sys.platform[0:5] == 'linux':
                #     for cam in range(self.numCameras):
                #         self.cameras[cam].stop_acquisition()
                # print 'Stop Acquisition'
            print 'Exiting'
            del self.camera
            for lib in self.libs:
                lib.__exit__()
                lib = None
            del self.libs   
            gc.collect()
            # if sys.platform[0:5] == 'win32':
            #     for cameraLib in self.cameraLibs:
            #         cameraLib.__exit__()
            #         cameraLib = None
            #     del self.cameraLibs   
            gc.collect()
        except:
            exit()

def DumpBuffer (buf, length, cameraType='FLIR'):
    if cameraType == 'FLIR':
        array_pointer = ctypes.cast(buf, ctypes.POINTER(ctypes.c_uint16*(length/2)))
        image = np.reshape(np.frombuffer(array_pointer.contents, dtype='uint16'),(512,640))
    elif cameraType == 'Spir' or cameraType == 'Pyro':
        array_pointer = ctypes.cast(buf, ctypes.POINTER(ctypes.c_int16*(length/2)))
        image = np.reshape(np.frombuffer(array_pointer.contents, dtype='int16'),(320,320))
    else:
       raise IOError( errno.ENOENT, "Camera not supported (Dump Buffer)")
    return image

def main():
    app = QtGui.QApplication([])
    imagePro = imageProGUI()
    sys.exit(app.exec_())
    
if __name__ == '__main__':
    main()
