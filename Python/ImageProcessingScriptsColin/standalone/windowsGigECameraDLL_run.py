# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# Notes: - For using this python module, see imageProDLLTest.py example
#        - The coordinates specified in the DLL are (u,v) (origin at top 
#               left hand corner of the image

"""Example file for using ctypes to access a C++ DLL in Python

This file contains a simple example of loading and calling
functions from a C++ DLL.  

When we say C++ DLL, we really mean that the C++ API has been
wrapped in C-compatible functions with an 'extern "C"'
declaration.  This extra bit of work is necessary as Python's
standard FFI libraries only talk to funcitons with C linkage.

There are libraries out there that do this for you (i.e. SWIG and
Boost.Python).  They work by essentially generating a C wrapper around
all functions and then building a Python loadable module (i.e. a
".pyd" file).  This example, however, shows how you would do it
without those libraries.

The C++ DLL that we use for this example contains very simple
functions that take 2 doubles, perform a mathematical operation on the
doubles, and return the result.  We use the ctypes module to load the
DLL and call those functions.

To load a DLL in Python the following work must be performed: 

(1) On the DLL side, you need to wrap any C++ API you want
exposed in functions with C linkage.  This involves the
following: 

    - Declare all wrapper functions with 'extern "C"' (i.e. in a
      header file)

    - Implement the wrapper functions.  If, in C++, you need to
      instantiate an object to access functionality, you will need to
      create functions to instantiate (new) and deallocate (delete)
      the object.

(2) On the Python side: 

    - Load the library using ctypes.cdll.LoadLibrary(filename)

    - For each funciton in the DLL, define types for arguments and
      return types.  

      ctypes has the ability to manage types for integers, strings,
      and unicode strings.  For others, you need to either wrap the
      values in the corresponding ctypes type (i.e. c_double) or
      define the type of the function.  The examples below shows this
      in more detail.

    - Write Python wrapper functions around the C ones.  

      Strictly speaking, you don't really need to do this to use the C
      functions, but it would make your life much easier if you wrap
      the C functions in something more Pythonic.  For example, the
      Python wrapper can instantiate and deallocate any object
      instances for you and eliminate the need for you to pass object
      instances around.
"""

import sys
import os
import ctypes
import math
import errno
import gc
if sys.version_info > (2,7):
    from collections import OrderedDict
else:
    from orderedDictionary import OrderedDict

# Path and filename for the DLL file
if sys.platform[0:5] == 'win32':
    libpath = r'.'
    dllfile = 'windowsGigECameraDLL.dll'
elif sys.platform[0:5] == 'linux':
    libpath = '.'
    dllfile = 'windowsGigECameraDLL.so'
else:
    raise IOERROR( errno.ENOENT, 'Operating System not supported (' + sys.platform + ')')

# Load the libarary into the Python interpreter
m = ctypes.cdll.LoadLibrary(os.path.join(libpath, dllfile))

# The loaded DLL functions have attributes .argtypes and .restype.
# .argtypes is a sequence of ctypes types and .restype is a single
# ctypes type.

# Define Argument types
m._new.argtypes = ()
m.Initialize.argtypes = (ctypes.c_void_p,)
m.getDeviceList.argtypes = (ctypes.c_void_p,)
m.getNumberDevices.argtypes = (ctypes.c_void_p,)
m.connectGigEDevice.argtypes = (ctypes.c_void_p, ctypes.c_int)
m.setPixelFormat.argtypes = (ctypes.c_void_p, ctypes.c_int)
m.enableStreaming.argtypes = (ctypes.c_void_p,)
m.startAcquisition.argtypes = (ctypes.c_void_p,)
m.stopAcquisition.argtypes = (ctypes.c_void_p,)
m.getBufferSize.argtypes = (ctypes.c_void_p,)
m.receiveGigEBuffer.argtypes = (ctypes.c_void_p,)
m.commandNUC.argtypes = (ctypes.c_void_p,)
m.openShutter.argtypes = (ctypes.c_void_p,)
m.closeShutter.argtypes = (ctypes.c_void_p,)
m.disconnectGigEDevice.argtypes = (ctypes.c_void_p,)
m._del.argtypes = (ctypes.c_void_p,)


# Define Return types
m._new.restype = ctypes.c_void_p
m.Initialize.restype = ctypes.c_void_p
m.getDeviceList.restype = ctypes.POINTER(ctypes.c_char_p)
m.getNumberDevices.restype = ctypes.c_int
m.connectGigEDevice.restype = ctypes.c_void_p
m.setPixelFormat.restype = ctypes.c_void_p
m.enableStreaming.restype = ctypes.c_void_p
m.startAcquisition.restype = ctypes.c_void_p
m.stopAcquisition.restype = ctypes.c_void_p
m.getBufferSize.restype = ctypes.c_uint32
m.receiveGigEBuffer.restype = ctypes.POINTER(ctypes.c_ubyte)
m.commandNUC.restype = ctypes.c_void_p
m.openShutter.restype = ctypes.c_void_p
m.closeShutter.restype = ctypes.c_void_p
m.disconnectGigEDevice.restype = ctypes.c_void_p
m._del.restype = ctypes.c_void_p
#----------------------------------------------------------

# Since we need an instance of MyMathFuncs to do anything, we wrap the
# functions in a class.  

# Define class function prototypes
class windowsGigECameraLib(object):
    
    def __init__(self):
        self._obj = m._new()
        return

    def Initialize(self):
        return m.Initialize(self._obj)

    def getDeviceList(self):
        pDeviceList = m.getDeviceList(self._obj)
        array_pointer = ctypes.cast(pDeviceList, ctypes.POINTER(ctypes.c_char_p*(self.getNumberDevices())))
        deviceList = []
        for i in range(self.getNumberDevices()):
            if array_pointer.contents[i] != 'None':
                deviceList.append(array_pointer.contents[i])
        return deviceList

    def getNumberDevices(self):
        return m.getNumberDevices(self._obj)
        
    def connectGigEDevice(self, device):
        return m.connectGigEDevice(self._obj, device)

    def setPixelFormat(self, formatNum):
        return m.setPixelFormat(self._obj, formatNum)

    def enableStreaming(self):
        return m.enableStreaming(self._obj)
    
    def startAcquisition(self):
        return m.startAcquisition(self._obj)    

    def stopAcquisition(self):
        return m.stopAcquisition(self._obj)

    def getBufferSize(self):
        return m.getBufferSize(self._obj)

    def receiveGigEBuffer(self):
        return m.receiveGigEBuffer(self._obj)

    def commandNUC(self):
        return m.commandNUC(self._obj)

    def openShutter(self):
        return m.openShutter(self._obj)

    def closeShutter(self):
        return m.closeShutter(self._obj)
        
    def disconnectGigEDevice(self):
        return m.disconnectGigEDevice(self._obj)

    def __exit__(self):
        m._del(self._obj)