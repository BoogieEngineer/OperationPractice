import sys
sys.path.insert(0, r'C:\work\Cymer\Algorithm Blocks\pyDLLs\windowsGigECameraDLL')
from windowsGigECameraDLL_run import windowsGigECameraLib
import numpy as np
from scipy import misc
import ctypes
from numpy import linalg as lin
import time
import math
from matplotlib import pyplot as plt
from matplotlib import gridspec
import gc
    
def displayImage(image, image_b=None, metrics=None):
    if image_b == None:
      fig = plt.figure( figsize =(16,8), facecolor = 'white')
      gs = gridspec.GridSpec(1, 1, width_ratios=[1])
      ax = plt.subplot(gs[0])
      plt.ylim([0,nrows-1])
      # if 'SimpleCentroidX' in metrics:
      #     ax.plot( metrics['SimpleCentroidX'], 
      #                metrics['SimpleCentroidY'],
      #                'wo',
      #                markersize = 8,
      #                label = 'Simple')
      # ax1 = plt.subplot(gs[1])
      # plt.ylim([0,nrows-1])
      # if 'SimpleCentroidX' in metrics:
      #     ax1.plot( metrics['SimpleCentroidX'], 
      #                metrics['SimpleCentroidY'],
      #                'wo',
      #                markersize = 8,
      #                label = 'Simple')
      # img = ax.imshow(image)
      # img1 = ax1.imshow(image_b)
    plt.show()
    plt.ion
    fig.canvas.update()
    fig.canvas.flush_events()
    
    answer = ''
    while answer == '':
        answer = input('Enter')

if __name__ == "__main__":
    nrows = 512
    ncols = 640
    lib = windowsGigECameraLib()
    lib.Initialize()
    deviceList = lib.getDeviceList()
    lib.connectGigEDevice(0)
    lib.setPixelFormat(14)
    lib.startAcquisition()
    start = time.clock()
    pReturnImage = lib.receiveGigEBuffer()
    array_pointer = ctypes.cast(pReturnImage, ctypes.POINTER(ctypes.c_int16*(lib.getBufferSize()/2)))
    # print lib.getBufferSize()
    returnImage = np.reshape(np.frombuffer(array_pointer.contents, dtype='int16'),(nrows,ncols))
    print time.clock() - start
    # time.sleep(1)
    # print "NUC"
    # lib.commandNUC()
    # time.sleep(1)
    # print "closeShutter"
    # lib.closeShutter()
    # time.sleep(1)
    # print "openShutter"
    # lib.openShutter()
    # time.sleep(1)
    
    lib.disconnectGigEDevice()
    # print set(returnImage.flatten())
    # print len(set(returnImage.flatten()))
    plt.imshow(returnImage)
    plt.show()

    # plt.ion
    lib.__exit__()
    # answer = ''
    # while answer == '':
    #     answer = input('Enter')
