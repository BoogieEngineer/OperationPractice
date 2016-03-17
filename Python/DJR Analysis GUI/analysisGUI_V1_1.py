## Release Notes
# Initial release, V1.
# April, 2013.
# DJR.

# Dependency: StreamingDataParser_DJR_V2

# Not implemented:
# Input file check box logic
# Output file check box logic
# Demux check box logic
# Anything on the data analysis tab

# Todo:
# Make 0 size streaming data file after demux
# Allow user to parse more than one file set at a time
# Use workers for parsing several files in a single set at the same time

## Data Parsing GUI
import StreamingDataParser_DJR_V3 as sdp_djr
import random, sys, time, csv, os, glob
import PyQt4.QtCore as QtCore
import PyQt4.QtGui as QtGui
#import PyQt4.Qwt5 as Qwt # Need a fast plot package...

class Communicate(QtCore.QObject):    
    threadEmit = QtCore.pyqtSignal(str) 

class myDialog(QtGui.QDialog):
    def __init__(self, title):
        QtGui.QDialog.__init__(self)
        self.setWindowTitle(title)
        self.resize(300,50)
        # Ok button.
        self.ok = QtGui.QPushButton("Ok")
        self.ok.clicked.connect(self.close)
        # Layout.
        hbox = QtGui.QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(self.ok)
        hbox.addStretch(1)
        self.setLayout(hbox)

class analysisGUI(QtGui.QWidget):
    def __init__(self):
        super(analysisGUI,self).__init__()
        
        # Set up messaging to main GUI from data processing thread.
        self.c = Communicate()
        self.c.threadEmit.connect(self.threadTerminated)
        
        # Create and connect data processing push button.
        self.parseDataPush = QtGui.QPushButton("Parse Data")
        self.parseDataPush.clicked.connect(self.parseDataCallback)
        
        # Create check boxes for input file options.
        self.RT = []
        RTStrings = ["RT01","RT02","RT05","RT06","RT09/16","RT11"]
        for string in RTStrings:
            check = QtGui.QCheckBox(string)
            self.RT.append(check)
        
        # Create check boxes for output file options.
        self.matlab = QtGui.QCheckBox("Mat File")
        self.binary = QtGui.QCheckBox("Binary")
        self.csv = QtGui.QCheckBox("CSV File")
        self.workspace = QtGui.QCheckBox("Work Space")
        self.demux = QtGui.QCheckBox("Signal Demux")
        
        # Create label for messaging about status of processing.
        self.parseStatus = QtGui.QLabel("Select a Configuration, Select a Data Folder, and Click Parse Data Button")
        
        # Plot.
        #self.plot, self.curve = self.createPlot()
        
        # Pan and zoom spinboxes.
        self.xPanSpinBox = QtGui.QSpinBox()
        self.xPanSpinBox.setValue(0)
        self.xPanSpinBox.setRange(0,1e6)
        self.xPanSpinBox.valueChanged.connect(self.xPanCallback)
        self.xPanSpinBox.setKeyboardTracking(False)
        
        self.xZoomSpinBox = QtGui.QSpinBox()
        self.xZoomSpinBox.setValue(1000) # Units: ms
        self.xZoomSpinBox.setRange(0,1e6)
        self.xZoomSpinBox.valueChanged.connect(self.xZoomCallback)
        self.xZoomSpinBox.setKeyboardTracking(False)
        
        # Layout.
        in_vbox1 = QtGui.QVBoxLayout()
        in_vbox2 = QtGui.QVBoxLayout()
        for kk in range(3):
            in_vbox1.addWidget(self.RT[kk])
            in_vbox2.addWidget(self.RT[kk+3])
        in_hbox = QtGui.QHBoxLayout()
        in_hbox.addLayout(in_vbox1)
        in_hbox.addStretch(1)
        in_hbox.addLayout(in_vbox2)
        in_hbox.addStretch(1)
        
        out_vbox1 = QtGui.QVBoxLayout()
        out_vbox1.addWidget(self.binary)
        out_vbox1.addWidget(self.matlab)

        out_vbox2 = QtGui.QVBoxLayout()
        out_vbox2.addWidget(self.csv)
        out_vbox2.addWidget(self.workspace)
        
        out_hbox = QtGui.QHBoxLayout()
        out_hbox.addLayout(out_vbox1)
        out_hbox.addStretch(1)
        out_hbox.addLayout(out_vbox2)
        out_hbox.addStretch(1)
        
        self.treeView1,self.fileSystemModel1 = self.createTreeView()
        self.treeView2,self.fileSystemModel2 = self.createTreeView()
        
        inputFrameTitle = QtGui.QLabel("Files to be Parsed")
        inputFrame = QtGui.QFrame(self)
        inputFrame.setFrameShape(QtGui.QFrame.StyledPanel)
        inputFrameLayout = QtGui.QVBoxLayout(inputFrame)  
        inputFrameLayout.addWidget(inputFrameTitle)
        inputFrameLayout.addLayout(in_hbox)
        inputFrameLayout.addStretch(1)
        
        outputFrameTitle = QtGui.QLabel("Output File Options")
        outputFrame = QtGui.QFrame(self)
        outputFrame.setFrameShape(QtGui.QFrame.StyledPanel)
        outputFrameLayout = QtGui.QVBoxLayout(outputFrame)
        outputFrameLayout.addWidget(outputFrameTitle)
        outputFrameLayout.addLayout(out_hbox)
        outputFrameLayout.addStretch(1)
        
        self.vbox = QtGui.QVBoxLayout()
        self.vbox.addWidget(inputFrame)
        self.vbox.addWidget(outputFrame)
        self.vbox.addStretch(1)
        self.vbox.addWidget(self.demux)
        self.vbox.addWidget(self.parseDataPush)
        self.vbox.addWidget(self.parseStatus)
        
        # Tab 2.
        xPanZoomLayout = QtGui.QHBoxLayout()
        xPanZoomLayout.addWidget(self.xPanSpinBox)
        xPanZoomLayout.addWidget(self.xZoomSpinBox)
        xPanZoomLayout.addStretch(1)
        tab2LHS = QtGui.QVBoxLayout()
        #tab2LHS.addWidget(self.plot)
        tab2LHS.addLayout(xPanZoomLayout)
        
        tab2Whole = QtGui.QHBoxLayout()
        tab2Whole.addLayout(tab2LHS)
        tab2Whole.addWidget(self.treeView2)
        
        # Tab widget.
        self.tabWidget = QtGui.QTabWidget()
        self.tabStrings = ["Parse Data","Plots and Analysis"]
        self.tab = []
        self.tabLayout = []
        for idx,tabString in enumerate(self.tabStrings):
            self.tab.append(QtGui.QWidget())
            self.tabWidget.addTab(self.tab[idx],tabString)
            self.tabLayout.append(QtGui.QVBoxLayout(self.tab[idx])) 
        
        self.hbox = QtGui.QHBoxLayout()
        self.hbox.addLayout(self.vbox)
        self.hbox.addWidget(self.treeView1)   
        
        idx = 0
        self.tabLayout[idx].addLayout(self.hbox)
        #self.tabLayout[1].addLayout(tab2Whole)
        #self.tabLayout[1].addLayout(xPanZoomLayout)
        
        self.mainLayout = QtGui.QHBoxLayout()
        self.mainLayout.addWidget(self.tabWidget)
        self.setLayout(self.mainLayout) 
        
        # Show the GUI.
        self.setWindowTitle("DJR's Analysis GUI")
        self.show()
    
    def createTreeView(self):
        # Create file system model.
        treeView = QtGui.QTreeView()
        fileSystemModel = QtGui.QFileSystemModel(treeView)
        fileSystemModel.setReadOnly(False)
        fileSystemModel.setFilter(QtCore.QDir.Dirs|QtCore.QDir.Drives|QtCore.QDir.NoDotAndDotDot|QtCore.QDir.AllDirs)
        # Attach file system model to tree.
        root = fileSystemModel.setRootPath(r"C:\Users\mha\Desktop\Work")
        treeView.setModel(fileSystemModel)
        treeView.setRootIndex(root)
        treeView.header().setStretchLastSection(False);
        treeView.header().setResizeMode(0, QtGui.QHeaderView.Stretch);     
        treeView.setSortingEnabled(True)
        return treeView, fileSystemModel
    
    def parseDataCallback(self):
        self.parseDataPush.setEnabled(False)
        index = self.treeView1.currentIndex() 
        self.lastDirectory = str(self.fileSystemModel1.filePath(index))
        #print self.lastDirectory
        #print len(self.lastDirectory)
        if len(self.lastDirectory) == 0:
            self.parseDataPush.setEnabled(True)
            p = myDialog("No Folder Selected!")
            p.exec_()            
            return
        self.lastDirectory = sdp_djr.checkPathBackslashEnding(self.lastDirectory)
        
        status = self.grabParsingOptions()
        #print status
        if status:
            self.parseDataPush.setText("Abort")
            self.parseDataPush.clicked.disconnect(self.parseDataCallback)
            self.parseDataPush.clicked.connect(self.stopParseCallback)
            self.pdt = parseDataThread(self,self.lastDirectory)
            self.pdt.finished.connect(self.parsingCompleteCallback)
            self.pdt.start()            
            self.parseDataPush.setEnabled(True) 
        else:
            self.parseDataPush.setEnabled(True)
 
    def stopParseCallback(self):
        self.parseDataPush.setEnabled(False)
        self.pdt.terminate()        
        self.parseDataPush.setText("Parse Data")
        self.parseDataPush.clicked.connect(self.parseDataCallback)
        self.parseDataPush.clicked.disconnect(self.stopParseCallback)
        self.parseStatus.setText("Parsing Aborted")
        self.parseDataPush.setEnabled(True)
    
    def parsingCompleteCallback(self):
        self.parseDataPush.setEnabled(False)
        self.parseDataPush.setText("Parse Data")
        self.parseStatus.setText("Parsing Complete... Select Another Path")        
        self.parseDataPush.clicked.connect(self.parseDataCallback)
        self.parseDataPush.clicked.disconnect(self.stopParseCallback)
        self.parseDataPush.setEnabled(True)
        
    def grabParsingOptions(self):        
        self.input = {}
        self.input["RT01"] = self.RT[0].isChecked()
        self.input["RT02"] = self.RT[1].isChecked()
        self.input["RT05"] = self.RT[2].isChecked()
        self.input["RT06"] = self.RT[3].isChecked()
        self.input["RT09/16"] = self.RT[4].isChecked()
        self.input["RT11"] = self.RT[5].isChecked()
        rtSelected = False
        for rt in self.input:
            if self.input[rt]:
                rtSelected = True
                break

        if not rtSelected:
            p = myDialog("No RT Type Selected!")
            p.exec_()
            status = False
            return status
  
        self.output = {}
        self.output["matlab"] = self.matlab.isChecked()
        self.output["csv"] = self.csv.isChecked()
        self.output["binary"] = self.binary.isChecked()
        self.output["workspace"] = self.workspace.isChecked()
        outputSelected = False
        for output in self.output:
            if self.output[output]:
                outputSelected = True
                break
        self.output["demux"] = self.demux.isChecked()
        
        if not outputSelected:
            p = myDialog("No Output Type Selected!")
            p.exec_()
            status = False
            return status
        status = True
        return status

    def threadTerminated(self,msg):
        self.pdt.finished.disconnect(self.parsingCompleteCallback)
        self.pdt.terminate()
        self.pdt.finished.connect(self.parsingCompleteCallback)
        p = myDialog(msg)
        p.exec_()
        self.parseStatus.setText("Select Configuration")
        self.parseDataPush.clicked.connect(self.parseDataCallback)
        self.parseDataPush.clicked.disconnect(self.stopParseCallback)
        self.parseDataPush.setText("Parse Data")
    
#     def createPlot(self):
#         plot = Qwt.QwtPlot(self)
#         plot.setCanvasBackground(QtCore.Qt.black)
#         plot.setAxisTitle(Qwt.QwtPlot.xBottom, 'Time Since Gate [us]')
#         plot.setAxisScale(Qwt.QwtPlot.xBottom, 0, 20000, 1000)
#         plot.setAxisTitle(Qwt.QwtPlot.yLeft, 'Arrival Time [us]')
#         plot.setAxisScale(Qwt.QwtPlot.yLeft, -3, 3, 1)
#         
#         #labelStrings=["Miss","Hit","",""]
#         #plot.setAxisScaleDraw(Qwt.QwtPlot.yLeft, TextScaleDraw(labelStrings))
#         
#         plot.replot()
#         
#         curve = Qwt.QwtPlotCurve('1')
#         curve.attach(plot)
#         curve.setRenderHint(Qwt.QwtPlotItem.RenderAntialiased)
#         #pen = QPen(QColor('limegreen'))
#         pen = QtGui.QPen(QtGui.QColor('red'))
#         pen.setWidth(2)
#         curve.setPen(pen)
#                 
#         avg = Qwt.QwtPlotCurve('2')
#         avg.attach(plot)
#         avg.setRenderHint(Qwt.QwtPlotItem.RenderAntialiased)
#         pen = QtGui.QPen(QtGui.QColor('white'))
#         pen.setWidth(1)
#         avg.setPen(pen)       
#         return plot, curve
    
    def plotDataCallback(self):
        return
    
    def xPanCallback(self):
        return
        
    def xZoomCallback(self):
        return
        
class parseDataThread(QtCore.QThread):
    # Class for running tests in a separate thread, to keep the GUI alive.
    def __init__(self,topLevelSelf,lastDirectory):
        super(parseDataThread,self).__init__()

        self.lastDirectory = lastDirectory
        self.topLevelSelf = topLevelSelf
    
    def run(self):
        mydata = None
        myrecinfo = None  
        if self.topLevelSelf.input["RT02"]:      
            allDatFiles = glob.glob(self.lastDirectory+"*_RT02_*.dat")
        if self.topLevelSelf.input["RT06"]:
            allDatFiles = allDatFiles + glob.glob(self.lastDirectory+"*_RT06_*.dat")
        if self.topLevelSelf.input["RT09/16"]:
            allDatFiles = allDatFiles + glob.glob(self.lastDirectory+"*_RT16_*.dat")
        if len(allDatFiles) == 0:
            msg = "No RT Files Found, Searching First Level Subdirectories"
            dataFolders = glob.glob(self.lastDirectory+"*" + "\\")
            for dataFolder in dataFolders:
                if self.topLevelSelf.input["RT02"]:
                    allDatFiles = glob.glob(dataFolder+"*_RT02_*.dat")
                if self.topLevelSelf.input["RT06"]:
                    allDatFiles = allDatFiles + glob.glob(dataFolder+"*_RT06_*.dat")
                if self.topLevelSelf.input["RT09/16"]:
                    allDatFiles = allDatFiles + glob.glob(dataFolder+"*_RT16_*.dat")
                if len(allDatFiles)==0:
                    msg = "No RT Files Found, Please Select Another Folder"
                    self.topLevelSelf.c.threadEmit.emit(msg)
                    time.sleep(1e5)
                tmp = glob.glob(dataFolder+"*MetaData*")
                if len(tmp) == 0:
                    msg = "No Meta Data Found"
                    self.topLevelSelf.c.threadEmit.emit(msg)
                    time.sleep(1e5)
                tmp,meta = os.path.split(tmp[0])
                for fullPath in allDatFiles:
                    tmp,file = os.path.split(fullPath)
                    fileStatusString = "Parsing " + file
                    self.topLevelSelf.parseStatus.setText(fileStatusString)
                    # If this is the first file in a sequence...
                    if file[21:24] == "000":
                        # If this is not the first sequence, close the files and clear mydata.
                        if mydata is not None:
                            for field_name in mydata:
                                mydata[field_name][0].close()
                            mydata = None
                            myrecinfo = None
                    # Parse.
                    mydata,myrecinfo = sdp_djr.parse(dataFolder,file,meta,delete = True, data = mydata, recinfo = myrecinfo)
                # Close the last set of files.
                for field_name in mydata:
                    mydata[field_name][0].close()
            
                         
            return
                    
            #self.topLevelSelf.c.threadEmit.emit(msg)
            #time.sleep(1e5)
        
        tmp = glob.glob(self.lastDirectory+"*MetaData*")
        if len(tmp) == 0:
            msg = "No Meta Data Found"
            self.topLevelSelf.c.threadEmit.emit(msg)
            time.sleep(1e5)
            
        tmp,meta = os.path.split(tmp[0])
        #print meta
        #print "Parsing Data"
        for fullPath in allDatFiles:
            tmp,file = os.path.split(fullPath)
            fileStatusString = "Parsing " + file
            self.topLevelSelf.parseStatus.setText(fileStatusString)
            # If this is the first file in a sequence...
            if file[21:24] == "000":
                # If this is not the first sequence, close the files and clear mydata.
                if mydata is not None:
                    for field_name in mydata:
                        mydata[field_name][0].close()
                    mydata = None
                    myrecinfo = None
            
            # Parse.
            mydata,myrecinfo = sdp_djr.parse(self.lastDirectory,file,meta,delete = True,data = mydata, recinfo = myrecinfo)          
        # Close the last set of files.
        for field_name in mydata:
            mydata[field_name][0].close()
        
def main():
    app = QtGui.QApplication(sys.argv)
    form = analysisGUI()
    form.show()
    app.exec_()

if __name__ == "__main__":
    main()

#self.lastDirectory = str(QtGui.QFileDialog.getExistingDirectory(self,self.tr("Select Directory"),r"C:\Python26\DJRScripts"))