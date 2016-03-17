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
standard FFI libraries only talk to functions with C linkage.

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

    - For each function in the DLL, define types for arguments and
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
    dllfile = 'hpImageProcessingDLL.dll'
elif sys.platform[0:5] == 'linux':
    libpath = '.'
    dllfile = 'hpImageProcessingDLL.so'
else:
    raise IOERROR( errno.ENOENT, 'Operating System not supported (' + sys.platform + ')')

# Load the library into the Python interpreter
m = ctypes.cdll.LoadLibrary(os.path.join(libpath, dllfile))

class hpImageProcessingOutputSignals(ctypes.Structure):
    _fields_ = [("algoStatusWord", ctypes.c_uint),
                ("borderLevel", ctypes.c_double),
                ("borderMax", ctypes.c_double),
                ("rawMaxIntensity", ctypes.c_double),
                ("rawMaxIndX", ctypes.c_double),
                ("rawMaxIndY", ctypes.c_double),
                ("rawMinIntensity", ctypes.c_double),
                ("rawSumIntensity", ctypes.c_double),
                ("rawMeanIntensity", ctypes.c_double),
                ("rawMedianIntensity", ctypes.c_double),
                ("simpleCentroidX", ctypes.c_double),
                ("simpleCentroidY", ctypes.c_double),
                ("simpleCentroidU", ctypes.c_double),
                ("simpleCentroidV", ctypes.c_double),
                ("simpleAngle", ctypes.c_double),
                ("simpleAmplitude", ctypes.c_double),
                ("simpleVarianceMajor", ctypes.c_double),
                ("simpleVarianceMinor", ctypes.c_double),
                ("simpleDiameterMajor", ctypes.c_double),
                ("simpleDiameterMinor", ctypes.c_double),
                ("simpleEnergyTotal", ctypes.c_double),
                ("simpleEllipseRatio", ctypes.c_double),
                ("simpleEnergyEllipse", ctypes.c_double),
                ("simpleEnergyRatio", ctypes.c_double),
                ("simplePerformance", ctypes.c_double),
                ("simpleCrossSectionalArea", ctypes.c_double),
                ("simpleDivergenceAngleMajor", ctypes.c_double),
                ("simpleDivergenceAngleMinor", ctypes.c_double),
                ("simpleAzimuthAngle", ctypes.c_double),
                ("symmetryMetric", ctypes.c_double),
                ("lsFitCentroidX", ctypes.c_double),
                ("lsFitCentroidY", ctypes.c_double),
                ("lsFitAngle", ctypes.c_double),
                ("lsFitAmplitude", ctypes.c_double),
                ("lsFitVarianceMajor", ctypes.c_double),
                ("lsFitVarianceMinor", ctypes.c_double),
                ("lsFitDiameterMajor", ctypes.c_double),
                ("lsFitDiameterMinor", ctypes.c_double),
                ("lsFitPerformance", ctypes.c_double),
                ("lsFitEllipseRatio", ctypes.c_double),
                # ("bgCWPower", ctypes.c_double),
                # ("bgPowerDensity", ctypes.c_double),
                # ("bgBeamPositionalStabilityCentX", ctypes.c_double),
                # ("bgBeamPositionalStabilityCentY", ctypes.c_double),
                # ("bgBeamPositionalStabilityDiamX", ctypes.c_double),
                # ("bgBeamPositionalStabilityDiamY", ctypes.c_double),
                # ("bgMaximumPower", ctypes.c_double),
                # ("bgEffectiveIrradiationArea", ctypes.c_double),
                # ("bgEffectiveAveragePower", ctypes.c_double),
                # ("bgFlatnessFactor", ctypes.c_double),
                # ("bgBeamUniformity", ctypes.c_double),
                # ("bgPlateauUniformity", ctypes.c_double),
                # ("bgEdgeSteepness", ctypes.c_double),
                # ("bgRoughnessOfFit", ctypes.c_double),
                # ("bgGoodnessOfFit", ctypes.c_double),
                ("pearsonCorrelation", ctypes.c_double),
                ("e2DiameterMajor", ctypes.c_double),
                ("e2DiameterMinor", ctypes.c_double),
                ("xSectionFFTMajorFreqErrorSum", ctypes.c_double),
                ("xSectionFFTMajorFreqErrorAverage", ctypes.c_double),
                ("xSectionFFTMajorFreq", ctypes.c_double),
                ("xSectionFFTMajorAmp", ctypes.c_double),
                ("xSectionFFTMinorFreqErrorSum", ctypes.c_double),
                ("xSectionFFTMinorFreqErrorAverage", ctypes.c_double),
                ("xSectionFFTMinorFreq", ctypes.c_double),
                ("xSectionFFTMinorAmp", ctypes.c_double),
                # ("xSectionFFTMajorFreq0", ctypes.c_double),
                # ("xSectionFFTMajorFreq1", ctypes.c_double),
                # ("xSectionFFTMajorFreq2", ctypes.c_double),
                # ("xSectionFFTMajorAmp0", ctypes.c_double),
                # ("xSectionFFTMajorAmp1", ctypes.c_double),
                # ("xSectionFFTMajorAmp2", ctypes.c_double),
                # ("xSectionFFTMinorFreq0", ctypes.c_double),
                # ("xSectionFFTMinorFreq1", ctypes.c_double),
                # ("xSectionFFTMinorFreq2", ctypes.c_double),
                # ("xSectionFFTMinorAmp0", ctypes.c_double),
                # ("xSectionFFTMinorAmp1", ctypes.c_double),
                # ("xSectionFFTMinorAmp2", ctypes.c_double),
                ("xSectionFFTMajorFreqVec", ctypes.POINTER(ctypes.c_float)),
                ("xSectionFFTMajorAmpVec", ctypes.POINTER(ctypes.c_float)),
                # ("xSectionFFTMajorDataVec", ctypes.POINTER(ctypes.c_float)),
                ("xSectionFFTMinorFreqVec", ctypes.POINTER(ctypes.c_float)),
                ("xSectionFFTMinorAmpVec", ctypes.POINTER(ctypes.c_float)),
                # ("xSectionFFTMinorDataVec", ctypes.POINTER(ctypes.c_float)),
                # ("rThetaHatE2Data", ctypes.POINTER(ctypes.c_float)),
                # ("rThetaHatEData", ctypes.POINTER(ctypes.c_float)),
                # ("rThetaHatPercentData", ctypes.POINTER(ctypes.c_float)),
                # ("rThetaHatESize", ctypes.c_int),
                # ("rThetaHatE2Size", ctypes.c_int),
                # ("rThetaHatPercSize", ctypes.c_int),
                ("xSectionFFTMajorSize", ctypes.c_int),
                ("xSectionFFTMinorSize", ctypes.c_int),
                # ("rThetaHatE2Contrast", ctypes.c_double),
                # ("rThetaHatEContrast", ctypes.c_double),
                # ("rThetaHatPercContrast", ctypes.c_double),
                # ("rThetaHatE2PkToPk", ctypes.c_double),
                # ("rThetaHatEPkToPk", ctypes.c_double),
                # ("rThetaHatPercPkToPk", ctypes.c_double),
                # ("rThetaHatE2Average", ctypes.c_double),
                # ("rThetaHatEAverage", ctypes.c_double),
                # ("rThetaHatPercAverage", ctypes.c_double),
                # ("rThetaHatRadiusE", ctypes.c_double),
                # ("rThetaHatRadiusE2", ctypes.c_double),
                # ("rThetaHatRadiusPerc", ctypes.c_double),
                ("offset_u", ctypes.c_uint),
                ("offset_v", ctypes.c_uint),
                ("roi_height", ctypes.c_uint),
                ("roi_width", ctypes.c_uint),
                ("roi_id", ctypes.c_uint),
                ("maxIntensityBeforeCalibration", ctypes.c_double),
                ("minIntensityBeforeCalibration", ctypes.c_double),
                ("fringeMetric", ctypes.c_double)]

    def getAllNumberFields(self):
        return len(self._fields_)

    def getAllFieldNames(self):
        return [seq[0] for seq in self._fields_]

    def getNumberFields(self):
        temp = len(self._fields_) - 7
        return temp

    def getFieldNames(self):
        result = [seq[0] for seq in self._fields_]
        result.remove("offset_u")
        result.remove("offset_v")
        result.remove("roi_height")
        result.remove("roi_width")
        result.remove("roi_id")    
        result.remove("xSectionFFTMajorFreqVec")
        result.remove("xSectionFFTMajorAmpVec")
        # result.remove("xSectionFFTMajorDataVec")
        result.remove("xSectionFFTMinorFreqVec")
        result.remove("xSectionFFTMinorAmpVec")
        # result.remove("xSectionFFTMinorDataVec")
        # result.remove("rThetaHatE2Data")
        # result.remove("rThetaHatEData")
        # result.remove("rThetaHatPercentData")
        return result

class passFailCriteriaStruct(ctypes.Structure):
    _fields_ = [('centroidXBotBound', ctypes.c_float),
                ('centroidXTopBound', ctypes.c_float),
                ('centroidYBotBound', ctypes.c_float),
                ('centroidYTopBound', ctypes.c_float),
                ('diameterMajorBotBound', ctypes.c_float),
                ('diameterMajorTopBound', ctypes.c_float),
                ('diameterMinorBotBound', ctypes.c_float),
                ('diameterMinorTopBound', ctypes.c_float),
                ('divergenceAngleMajorBotBound', ctypes.c_float),
                ('divergenceAngleMajorTopBound', ctypes.c_float),
                ('divergenceAngleMinorBotBound', ctypes.c_float),
                ('divergenceAngleMinorTopBound', ctypes.c_float),
                ('symmetryBotBound', ctypes.c_float),
                ('symmetryTopBound', ctypes.c_float),
                ('gaussianPerformanceBotBound', ctypes.c_float),
                ('gaussianPerformanceTopBound', ctypes.c_float),
                ('pearsonCorrelationBotBound', ctypes.c_float),
                ('pearsonCorrelationTopBound', ctypes.c_float),
                ('maxIntensityBotBound', ctypes.c_float),
                ('maxIntensityTopBound', ctypes.c_float)]

    def getNumberFields(self):
        return len(self._fields_)

    def getFieldNames(self):
        return [seq[0] for seq in self._fields_]


# The loaded DLL functions have attributes .argtypes and .restype.
# .argtypes is a sequence of ctypes types and .restype is a single
# ctypes type.

# Define Argument types
m._new.argtypes = (ctypes.c_int, ctypes.c_int)
m.InitializeObj.argtypes = (ctypes.c_void_p,)
m.SetParamFloat.argtypes = (ctypes.c_void_p, ctypes.c_long, ctypes.c_float)
m.SetParamUInt.argtypes = (ctypes.c_void_p, ctypes.c_long, ctypes.c_uint32)
# m.SetParamFloatArray.argtypes = (ctypes.c_void_p, ctypes.c_long, ctypes.POINTER(ctypes.c_float), ctypes.c_uint)
# m.SetParamStruct.argtypes = (ctypes.c_void_p, ctypes.POINTER(hpImageProcessingParams))
m.SetArrayByIndex.artypes = (ctypes.c_void_p, ctypes.c_long, ctypes.c_float, ctypes.c_int)
m.clearCalibrationImage.argtypes = (ctypes.c_void_p,)
m.addImageToCalibrationImage.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int16), ctypes.c_uint, ctypes.c_uint)
m.averageCalibrationImage.argtypes = (ctypes.c_void_p,)
m.calibrateImageIntArray.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int16), ctypes.c_uint, ctypes.c_uint)
m.calibrateImageUIntArray.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint16), ctypes.c_uint, ctypes.c_uint)
m.runImageProcessing.argtypes = (ctypes.c_void_p,)
m.getOutputSignals.argtypes = (ctypes.c_void_p, ctypes.c_int,)
m.getCalibrationImage.argtypes = (ctypes.c_void_p,)
m.getCalibratedImage.argtypes = (ctypes.c_void_p,)
m.processImageUIntArray.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint16), ctypes.c_uint, ctypes.c_uint)
m.processImageIntArray.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int16), ctypes.c_uint, ctypes.c_uint)
m.getPassFailCriteria.argtypes = (ctypes.c_void_p, ctypes.c_uint, ctypes.POINTER(passFailCriteriaStruct))
m._del.argtypes = (ctypes.c_void_p,)


# Define Return types
m._new.restype = ctypes.c_void_p
m.InitializeObj.restype = ctypes.c_void_p
m.SetParamFloat.restype = ctypes.c_void_p
m.SetParamUInt.restype = ctypes.c_void_p
# m.SetParamFloatArray.restype = ctypes.c_void_p
# m.SetParamStruct.restype = (ctypes.c_void_p, ctypes.POINTER(hpImageProcessingParams))
m.SetArrayByIndex.restype = ctypes.c_void_p
m.clearCalibrationImage.restype = ctypes.c_void_p
m.addImageToCalibrationImage.restype = ctypes.c_void_p
m.averageCalibrationImage.restype = ctypes.c_void_p
m.calibrateImageIntArray.restype = ctypes.POINTER(ctypes.c_uint16)
m.calibrateImageUIntArray.restype = ctypes.POINTER(ctypes.c_uint16)
m.runImageProcessing.restype = ctypes.c_void_p
m.getOutputSignals.restype = ctypes.POINTER(hpImageProcessingOutputSignals)
m.getCalibrationImage.restype = ctypes.POINTER(ctypes.c_int16)
m.getCalibratedImage.restype = ctypes.POINTER(ctypes.c_uint16)
m.processImageUIntArray.restype = ctypes.POINTER(ctypes.c_uint16)
m.processImageIntArray.restype = ctypes.POINTER(ctypes.c_uint16)
m.getPassFailCriteria.restype = ctypes.c_void_p
m._del.restype = ctypes.c_void_p
#----------------------------------------------------------

# Since we need an instance of MyMathFuncs to do anything, we wrap the
# functions in a class.  

# Define class function prototypes
class hpImageProcessingLib(object):
    def __init__(self, nrows, ncols):
        self._obj = m._new(nrows, ncols)
        return

    def Initialize(self):
        return m.InitializeObj(self._obj)

    def SetParamFloat(self, paramID, value):
        return m.SetParamFloat(self._obj, paramID, value)

    def SetParamUInt(self, paramID, value):
        return m.SetParamUInt(self._obj, paramID, value)

    # def SetParamFloatArray(self, paramID, pValue, numElements):
    #     return m.SetParamFloatArray(self._obj, paramID, pValue, numElements)

    def SetArrayByIndex(self, paramID, value, index):
        return m.SetArrayByIndex(self._obj, paramID, value, index)

    def clearCalibrationImage(self):
        return m.clearCalibrationImage(self._obj)

    def addImageToCalibrationImage(self, pImage, nrows, ncols):
        return m.addImageToCalibrationImage(self._obj, pImage, nrows, ncols)

    def averageCalibrationImage(self):
        return m.averageCalibrationImage(self._obj)

    def calibrateImageIntArray(self, pImage, nrows, ncols):
        return m.calibrateImageIntArray(self._obj, pImage, nrows, ncols)

    def calibrateImageUIntArray(self, pImage, nrows, ncols):
        return m.calibrateImageUIntArray(self._obj, pImage, nrows, ncols)

    def runImageProcessing(self):
        return m.runImageProcessing(self._obj)

    def getOutputSignals(self, roiNumber):
        return m.getOutputSignals(self._obj, roiNumber)

    def getCalibrationImage(self):
        return m.getCalibrationImage(self._obj)

    def getCalibratedImage(self):
        return m.getCalibratedImage(self._obj)

    def processImageUIntArray(self, imageBuffer, height, width):
        return m.processImageUIntArray(self._obj, imageBuffer, height, width) 

    def processImageIntArray(self, imageBuffer, height, width):
        return m.processImageIntArray(self._obj, imageBuffer, height, width) 

    def getPassFailCriteria(self, roiNumber, passFailCriteria):
        return m.getPassFailCriteria(self._obj, roiNumber, passFailCriteria)
    
    def __exit__(self):
        m._del(self._obj)