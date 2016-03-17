/******************************************************************************
 *                                                                            *
 *                     C O P Y R I G H T   N O T I C E                        *
 *                                                                            *
 *                  COPYRIGHT (c) 2012 CYMER, Inc.                            *
 *  An unpublished work by CYMER, Inc.  All rights reserved.  This program is *
 *  an unpublished work protected by United States Copyright Laws (Title 17   *
 *  United States Code) and contains trade secrets of CYMER,Inc. which must   *
 *  be held in strict confidence.                                             *
 ******************************************************************************
 * FILENAME: hpImageProcessing.cpp
 *
 * DESCRIPTION: 
 *   Defines a class that is used for computing a beam profile on an image.
 *   This class contains the algorithms that are defined for the HPSS beam
 *   profiling.  We do not make this algorithm a CRASH object since we will
 *   want to use the same code for the algorithm for multiple cameras which all
 *   have different rois.
 *
 *****************************************************************************/
#include <math.h>
#include <float.h>
#include "hpImageProcessing.h"
#include <iostream>

#ifdef LC12
#include "LcaUtilities.h"
#endif

/* ========================================================================= */
/*                             hpImageProcessing                              */
/* ========================================================================== */
/**
 * Constructor for the hpImageProcessing class.
 */

hpImageProcessing::hpImageProcessing():
	m_initialized(false),
    m_NUM_ROIS(1),
    m_LOWPASS_SMOOTHING_FACTOR(0),
    m_MED_FILT_PADDING(0),
    m_LEAST_SQUARES_FIT_DELTA(0),
    m_SYM_RADIAL_LINES(0),
    m_COM_VALID_DISTANCE(1),
    m_NUM_IMAGES_TO_SUM(1),
    m_WATT_PER_COUNT(1),
    m_FOCAL_LENGTH(1),
    m_MM_PER_PIXEL(1),
    m_PROBE_ROI(0),
    m_PROBE_LOCATION(0),
    m_PROBE_MODE(0),
    m_PROBE_SIGNAL_PERIOD(0),
    m_PROBE_SIGNAL_AMPLITUDE(0),
    m_PROBE_SIGNAL_OFFSET(0),
    m_maxImageWidth(0),
    m_maxImageHeight(0),
    m_algoStatusWord(),
    m_numberImagesInCalibration(0),
    m_finishedSumming(false),
    m_inputImageHeight(0),
    m_inputImageWidth(0),
    m_numberImagesSummed(0),
    m_calibratedImage(NULL),
    m_calibrationImage(NULL),
    m_calibrationCompleted(true),
    m_inputImageMaxVal(0),
    m_inputImageMinVal(0)
{
    construct();
}

void hpImageProcessing::construct(void)
{
    // parameters
    m_CONTROL_WORD.SetSize(MAX_NUM_ROIS);
    m_THRESHOLD_CUTOFF.SetSize(MAX_NUM_ROIS);
    m_LEAST_SQUARES_CUTOFF.SetSize(MAX_NUM_ROIS);
    m_SUB_ROI_RADIUS.SetSize(MAX_NUM_ROIS);
    m_ELLIPTICAL_SUB_ROI_PADDING.SetSize(MAX_NUM_ROIS);
    m_SYM_RADIAL_DISTANCE.SetSize(MAX_NUM_ROIS);
    m_ROI_X_OFFSET.SetSize(MAX_NUM_ROIS);
    m_ROI_Y_OFFSET.SetSize(MAX_NUM_ROIS);
    m_ROI_WIDTH.SetSize(MAX_NUM_ROIS);
    m_ROI_HEIGHT.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_X_OFFSET.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_Y_OFFSET.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_WIDTH.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_HEIGHT.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_MAJOR_RAD.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_MINOR_RAD.SetSize(MAX_NUM_ROIS);
    // m_POWER_DENSITY_ANGLE.SetSize(MAX_NUM_ROIS);
    // m_EFFECTIVE_IRRADIATION_THRESHOLD.SetSize(MAX_NUM_ROIS);
    // m_BEAM_POSITIONAL_STABILITY_LOWPASS.SetSize(MAX_NUM_ROIS);
    // m_R_THETA_TOP_HAT_NUM_LINES.SetSize(MAX_NUM_ROIS);
    // m_R_THETA_TOP_HAT_PERCENT_LEVEL.SetSize(MAX_NUM_ROIS);
    m_CENTROID_BOT_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_CENTROID_TOP_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_BEAM_DIAMETER_BOT_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_BEAM_DIAMETER_TOP_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_DIVERGENCE_BOT_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_DIVERGENCE_TOP_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_GAUSS_PERF_TOP_BOUND.SetSize(2*MAX_NUM_ROIS);
    m_SYMMETRY_TOP_BOUND.SetSize(MAX_NUM_ROIS);
    m_PEARSON_BOT_BOUND.SetSize(MAX_NUM_ROIS);
    m_MAX_INTENSITY_BOT_BOUND.SetSize(MAX_NUM_ROIS);
    m_MAX_INTENSITY_TOP_BOUND.SetSize(MAX_NUM_ROIS);
    m_PROBE_SIGNAL_DATA.SetSize(M_MAX_PROBE_DATA_SIZE);
}

void hpImageProcessing::setSize(unsigned int maxImageHeight, unsigned int maxImageWidth)
{
    m_maxImageWidth = maxImageWidth;
    m_maxImageHeight = maxImageHeight;
    m_CALIBRATION_IMAGE.SetSize(maxImageHeight*maxImageWidth);

    m_inputImage.setSize(maxImageHeight,maxImageWidth);
    m_imageSum.setSize(maxImageHeight,maxImageWidth);

    if (m_calibrationImage != NULL)
    {
        delete [] m_calibrationImage;
        m_calibrationImage = NULL;
    }
    if (m_calibratedImage != NULL)
    {
        delete [] m_calibratedImage;
        m_calibrationImage = NULL;
    }

    m_calibratedImage = new uint16_t [maxImageWidth*maxImageHeight];
    m_calibrationImage = new int16_t [maxImageWidth*maxImageHeight];

    for(unsigned int ind=0; ind<m_maxImageWidth*m_maxImageHeight; ind++)
    {
        m_calibratedImage[ind] = 0;
        m_calibrationImage[ind] = 0;
    }
    for (int i=0; i<MAX_NUM_ROIS; i++)
    {
        m_imageProObj[i].setSize(maxImageHeight, maxImageWidth);
    }
}

hpImageProcessing::hpImageProcessing(unsigned int maxImageHeight, unsigned int maxImageWidth):
    m_initialized(false),
    m_NUM_ROIS(1),
    m_LOWPASS_SMOOTHING_FACTOR(0),
    m_MED_FILT_PADDING(0),
    m_LEAST_SQUARES_FIT_DELTA(0),
    m_SYM_RADIAL_LINES(0),
    m_COM_VALID_DISTANCE(1),
    m_NUM_IMAGES_TO_SUM(1),
    m_WATT_PER_COUNT(1),
    m_FOCAL_LENGTH(1),
    m_MM_PER_PIXEL(1),
    m_PROBE_ROI(0),
    m_PROBE_LOCATION(0),
    m_PROBE_MODE(0),
    m_PROBE_SIGNAL_PERIOD(0),
    m_PROBE_SIGNAL_AMPLITUDE(0),
    m_PROBE_SIGNAL_OFFSET(0),
    m_maxImageWidth(0),
    m_maxImageHeight(0),
    m_algoStatusWord(),
    m_numberImagesInCalibration(0),
    m_finishedSumming(false),
    m_inputImageHeight(0),
    m_inputImageWidth(0),
    m_numberImagesSummed(0),
    m_calibratedImage(NULL),
    m_calibrationImage(NULL),
    m_calibrationCompleted(true),
    m_inputImageMaxVal(0),
    m_inputImageMinVal(0)
{
    /* Note: Before C++11, you cannot call peer constructors inside other constructors (ie. cannot call hpImageProcessing()). 
    This is known as delegation. Without it, calling the any other constructor will create a temporary object that is deleted at the
    end if the ; (similar to creating a temporary variable such as int or float except the object is not saved to a local variable).
    Thus, none of the original object's members are initialized. */
    construct();
    setSize(maxImageHeight, maxImageWidth);
}

/** 
 * Destructor for the hpImageProcessing class.
 */
hpImageProcessing::~hpImageProcessing()
{
	if (m_calibrationImage != NULL)
	{
		delete [] m_calibrationImage;
		m_calibrationImage = NULL;
	}
	if (m_calibratedImage != NULL)
	{
		delete [] m_calibratedImage;
		m_calibratedImage = NULL;
	}
}

CrashResult hpImageProcessing::Initialize()
{
    CrashResult result(SUCCESS);
    if (m_initialized==false)
    {
        m_initialized = true;
        for (int i=0; i<MAX_NUM_ROIS; i++)
        {
            result = m_imageProObj[i].Initialize();
            if (result != static_cast<unsigned long>(SUCCESS))
            {
              return result;
            }
        }

        for (int ind=0; ind<m_CALIBRATION_IMAGE.GetSize(); ind++)
        {
            m_CALIBRATION_IMAGE[ind] = 0;
        }
        m_imageSum.zeros();
        m_inputImage.zeros();
        result = m_probe.Initialize();
        if (result != static_cast<unsigned long>(SUCCESS))
        {
          return result;
        }

        return CrashBaseObject::Initialize();
    }
    else
    {
        return CrashBaseObject::ALREADY_INITIALIZED;
    }
} // end of Initialize

CrashResult hpImageProcessing::SetParam(long paramID, float value)
{
    switch (paramID)
    {
        case LOWPASS_SMOOTHING_FACTOR: 
        {
            if (value < 0)
            {
                return LOWPASS_SMOOTHING_NEG;
            }
            else if (value > 1)
            {
                return LOWPASS_SMOOTHING_GREATER_THAN_ONE;
            }
            else
            {
                m_LOWPASS_SMOOTHING_FACTOR = value;
            }
            break;
        }
		case MED_FILT_PADDING: 
        {
            if (value >= 0)
            {
                m_MED_FILT_PADDING = static_cast<uint32_t>(value);
            }
            else
            {
                return MED_FILT_PADDING_NEG;
            }
            break;
        }
        case LEAST_SQUARES_FIT_DELTA: 
        {
            if (value > 0)
            {
                m_LEAST_SQUARES_FIT_DELTA = value;
            }
            else
            {
                return LEAST_SQUARES_FIT_DELTA_NEG_OR_ZERO;
            }
            break;
        }
		case SYM_RADIAL_LINES: 
        {
            if (value >= 0)
            {
                m_SYM_RADIAL_LINES = static_cast<uint32_t>(value);
            }
            else
            {
                return SYM_RADIAL_LINES_NEG;
            }
            break;
        }
        case COM_VALID_DISTANCE: 
        {
            if (value >= 0)
            {
                m_COM_VALID_DISTANCE = value;
            }
            else
            {
                return COM_VALID_DISTANCE_NEG;
            }
            break;
        }
		        
        case NUM_IMAGES_TO_SUM: 
        {
            if (value > 0)
            {
                m_NUM_IMAGES_TO_SUM = static_cast<uint32_t>(value);
            }
            else
            {
                return NUM_IMAGES_TO_SUM_NEG_OR_ZERO;
            }
            break;
        }
        case NUM_ROIS: 
        {
            if (value <= 0)
            {
                return NUM_ROIS_NEG_OR_ZERO;
            }
            else if (value > MAX_NUM_ROIS)
            {
				m_NUM_ROIS = MAX_NUM_ROIS;
                return NUM_ROIS_GREATER_THAN_MAX_NUM_ROIS;
            }
            else
            {
                m_NUM_ROIS = static_cast<uint32_t>(value);
            }
            break;
        }
        case WATT_PER_COUNT:
        {
            if (value > 0)
            {
                m_WATT_PER_COUNT = value;
            }
            else
            {
                return WATT_PER_COUNT_NEG_OR_ZERO;
            }
            break;
        }
        case FOCAL_LENGTH:
        {
            if (value > 0)
            {
                m_FOCAL_LENGTH = value;
            }
            else
            {
                return FOCAL_LENGTH_NEG_OR_ZERO;
            }
            break;
        }
        case MM_PER_PIXEL:
        {
            if (value > 0)
            {
                m_MM_PER_PIXEL = value;
            }
            else
            {
                return MM_PER_PIXEL_NEG_OR_ZERO;
            }
            break;
        }
        case PROBE_ROI:
        {
            m_PROBE_ROI = static_cast<int32_t>(value);
            break;
        }
        case PROBE_LOCATION:
        {
            m_PROBE_LOCATION = static_cast<int32_t>(value);
            break;
        }
        case PROBE_MODE: 
        {
            if (value >= 0)
            {
                m_probe.SetParam(FuncGen::MODE, static_cast<int32_t>(value));
            }
            else
            {
                return PROBE_MODE_NEG;
            }
            break;
        }
        case PROBE_SIGNAL_PERIOD: 
        {
            if (value >= 0)
            {
                m_probe.SetParam(FuncGen::PERIOD, static_cast<int32_t>(value));
            }
            else
            {
                return PROBE_SIGNAL_PERIOD_NEG;
            }
            break;
        }
        case PROBE_SIGNAL_AMPLITUDE:
        {
            m_probe.SetParam(FuncGen::AMPLITUDE, value);
            break;
        }
        case PROBE_SIGNAL_OFFSET:
        {
            m_probe.SetParam(FuncGen::OFFSET, value);
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;
    }

    return CrashBaseObject::SUCCESS;
} // end of SetParam

CrashResult hpImageProcessing::SetParam(long paramID, uint32_t value)
{
    switch (paramID)
    {
        case MED_FILT_PADDING: 
        {
            if (value >= 0)
            {
                m_MED_FILT_PADDING = value;
            }
            else
            {
                return MED_FILT_PADDING_NEG;
            }
            break;
        }
        case SYM_RADIAL_LINES: 
        {
            if (value >= 0)
            {
                m_SYM_RADIAL_LINES = value;
            }
            else
            {
                return SYM_RADIAL_LINES_NEG;
            }
            break;
        }
        case NUM_IMAGES_TO_SUM: 
        {
            if (value > 0)
            {
                m_NUM_IMAGES_TO_SUM = value;
            }
            else
            {
                return NUM_IMAGES_TO_SUM_NEG_OR_ZERO;
            }
            break;
        }
        case NUM_ROIS: 
        {
            if (value <= 0)
            {
                return NUM_ROIS_NEG_OR_ZERO;
            }
            else if (value > MAX_NUM_ROIS)
            {
				m_NUM_ROIS = MAX_NUM_ROIS;
                return NUM_ROIS_GREATER_THAN_MAX_NUM_ROIS;
            }
            else
            {
                m_NUM_ROIS = value;
            }
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;
    }

    return CrashBaseObject::SUCCESS;
} // end of SetParam

CrashResult hpImageProcessing::SetParam(long paramID, int value)
{
    switch (paramID)
    {
        case PROBE_MODE: 
        {
            if (value >= 0)
            {
                m_probe.SetParam(FuncGen::MODE, static_cast<int32_t>(value));
            }
            else
            {
                return PROBE_MODE_NEG;
            }
            break;
        }
        case PROBE_SIGNAL_PERIOD: 
        {
            if (value >= 0)
            {
                m_probe.SetParam(FuncGen::PERIOD, static_cast<int32_t>(value));
            }
            else
            {
                return PROBE_SIGNAL_PERIOD_NEG;
            }
            break;
        }
        case PROBE_ROI:
        {
            m_PROBE_ROI = value;
            break;
        }
        case PROBE_LOCATION:
        {
            m_PROBE_LOCATION = value;
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;
    }

    return CrashBaseObject::SUCCESS;
} // end of SetParam

CrashResult hpImageProcessing::SetParam(long paramID, float* rValue, int numElements)
{
    switch (paramID)
    {
        case CONTROL_WORD:
        {
            for (int i=0; i<m_CONTROL_WORD.GetSize(); i++)
            {
				if (i < numElements)
                {
                    if (rValue[i] < 0)
                    {
                        return CONTROL_WORD_NEG;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_CONTROL_WORD[i] = rValue[i];
                    }
                    else
                    {
                        return CONTROL_WORD_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_CONTROL_WORD[i] = 0;
                }
            }
            break;
        }
		case THRESHOLD_CUTOFF: 
        {
			for (int i=0; i<m_THRESHOLD_CUTOFF.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] > 100)
                    {
                        return THRESHOLD_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                    }
                    else if (rValue[i] >= 0)
                    {
                        m_THRESHOLD_CUTOFF[i] = rValue[i];
                    }
                    else
                    {
                        return THRESHOLD_CUTOFF_NEG;
                    }
                }
                else
                {
                    m_THRESHOLD_CUTOFF[i] = 0;
                }
            }
            break;
        }
		case LEAST_SQUARES_CUTOFF:
        {
			for (int i=0; i<m_LEAST_SQUARES_CUTOFF.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] > 100)
                    {
                        return LEAST_SQUARES_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                    }
                    else if (rValue[i] > 0)
                    {
                        m_LEAST_SQUARES_CUTOFF[i] = rValue[i];
                    }
                    else
                    {
                        return LEAST_SQUARES_CUTOFF_NEG_OR_ZERO;
                    }
                }
                else
                {
                    m_LEAST_SQUARES_CUTOFF[i] = 0;
                }
            }
            break;
        }
        case SUB_ROI_RADIUS:
        {
            for (int i=0; i<m_SUB_ROI_RADIUS.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] <= 0)
                    {
                        return SUB_ROI_RADIUS_NEG_OR_ZERO;
                    }
                    else
                    {
                        m_SUB_ROI_RADIUS[i] = rValue[i];
                    }
                }
                else
                {
                    m_SUB_ROI_RADIUS[i] = 0;
                }
            }
            break;
        }
        case SYM_RADIAL_DISTANCE:
        {
            for (int i=0; i<m_SYM_RADIAL_DISTANCE.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] <= 0)
                    {
                        return SYM_RADIAL_DISTANCE_NEG_OR_ZERO;
                    }
                    else
                    {
                        m_SYM_RADIAL_DISTANCE[i] = rValue[i];
                    }
                }
                else
                {
                    m_SYM_RADIAL_DISTANCE[i] = 0;
                }
            }
            break;
        }
        case ROI_X_OFFSET:
        {
            for (int i=0; i<m_ROI_X_OFFSET.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] < 0)
                    {
                        return ROI_X_OFFSET_NEG;
                    }
                    else if (rValue[i] > MAX_ROI_WIDTH)
                    {
                        return ROI_X_OFFSET_GREATER_THAN_MAX;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_ROI_X_OFFSET[i] = rValue[i];
                    }
                    else
                    {
                        return ROI_X_OFFSET_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_X_OFFSET[i] = 0;
                }
            }
            break;
        }
        case ROI_Y_OFFSET:
        {
            for (int i=0; i<m_ROI_Y_OFFSET.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] < 0)
                    {
                        return ROI_Y_OFFSET_NEG;
                    }
                    else if (rValue[i] > MAX_ROI_HEIGHT)
                    {
                        return ROI_Y_OFFSET_GREATER_THAN_MAX;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_ROI_Y_OFFSET[i] = rValue[i];
                    }
                    else
                    {
                        return ROI_Y_OFFSET_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_Y_OFFSET[i] = 0;
                }
            }
            break;
        }
        case ROI_WIDTH:
        {
            for (int i=0; i<m_ROI_WIDTH.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] <= 0)
                    {
                        return ROI_WIDTH_NEG_OR_ZERO;
                    }
                    else if (rValue[i] > MAX_ROI_WIDTH)
                    {
                        return ROI_WIDTH_GREATER_THAN_MAX;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_ROI_WIDTH[i] = rValue[i];
                    }
                    else
                    {
                        return ROI_WIDTH_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_WIDTH[i] = 0;
                }
            }
            break;
        }
        case ROI_HEIGHT:
        {
            for (int i=0; i<m_ROI_HEIGHT.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] <= 0)
                    {
                        return ROI_HEIGHT_NEG_OR_ZERO;
                    }
                    else if (rValue[i] > MAX_ROI_HEIGHT)
                    {
                        return ROI_HEIGHT_GREATER_THAN_MAX;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_ROI_HEIGHT[i] = rValue[i];
                    }
                    else
                    {
                        return ROI_HEIGHT_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_HEIGHT[i] = 0;
                }
            }
            break;
        }
        // case POWER_DENSITY_X_OFFSET:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_X_OFFSET.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return POWER_DENSITY_X_OFFSET_NEG;
        //             }
        //             else if (rValue[i] > MAX_ROI_WIDTH)
        //             {
        //                 return POWER_DENSITY_X_OFFSET_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_X_OFFSET[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_X_OFFSET_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_X_OFFSET[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_Y_OFFSET:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_Y_OFFSET.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_NEG;
        //             }
        //             else if (rValue[i] > MAX_ROI_HEIGHT)
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_Y_OFFSET[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_Y_OFFSET[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_WIDTH:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_WIDTH.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_WIDTH_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > MAX_ROI_WIDTH)
        //             {
        //                 return POWER_DENSITY_WIDTH_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_WIDTH[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_WIDTH_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_WIDTH[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_HEIGHT:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_HEIGHT.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_HEIGHT_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > MAX_ROI_HEIGHT)
        //             {
        //                 return POWER_DENSITY_HEIGHT_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_HEIGHT[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_HEIGHT_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_HEIGHT[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_MAJOR_RAD:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_MAJOR_RAD.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_MAJOR_RAD_NEG_OR_ZERO;
        //             }
        //             else
        //             {
        //                 m_POWER_DENSITY_MAJOR_RAD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_MAJOR_RAD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_MINOR_RAD:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_MINOR_RAD.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_MINOR_RAD_NEG_OR_ZERO;
        //             }
        //             else
        //             {
        //                 m_POWER_DENSITY_MINOR_RAD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_MINOR_RAD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_ANGLE:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_ANGLE.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             m_POWER_DENSITY_ANGLE[i] = rValue[i];
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_ANGLE[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case EFFECTIVE_IRRADIATION_THRESHOLD:
        // {
        //     for (int i=0; i<m_EFFECTIVE_IRRADIATION_THRESHOLD.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return EFFECTIVE_IRRADIATION_THRESHOLD_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > 100)
        //             {
        //                 return EFFECTIVE_IRRADIATION_THRESHOLD_GREATER_THAN_ONE_HUNDRED;
        //             }
        //             else
        //             {
        //                 m_EFFECTIVE_IRRADIATION_THRESHOLD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_EFFECTIVE_IRRADIATION_THRESHOLD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case BEAM_POSITIONAL_STABILITY_LOWPASS:
        // {
        //     for (int i=0; i<m_BEAM_POSITIONAL_STABILITY_LOWPASS.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_NEG;
        //             }
        //             else if (rValue[i] > 1)
        //             {
        //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_GREATER_THAN_ONE;
        //             }
        //             else
        //             {
        //                 m_BEAM_POSITIONAL_STABILITY_LOWPASS[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_BEAM_POSITIONAL_STABILITY_LOWPASS[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case R_THETA_TOP_HAT_NUM_LINES:
        // {
        //     for (int i=0; i<m_R_THETA_TOP_HAT_NUM_LINES.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return R_THETA_TOP_HAT_NUM_LINES_NEG_OR_ZERO;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_R_THETA_TOP_HAT_NUM_LINES[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return R_THETA_TOP_HAT_NUM_LINES_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_R_THETA_TOP_HAT_NUM_LINES[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case R_THETA_TOP_HAT_PERCENT_LEVEL:
        // {
        //     for (int i=0; i<m_R_THETA_TOP_HAT_PERCENT_LEVEL.GetSize(); i++)
        //     {
        //         if (i < numElements)
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > 100)
        //             {
        //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_GREATER_THAN_ONE_HUNDRED;
        //             }
        //             else
        //             {
        //                 m_R_THETA_TOP_HAT_PERCENT_LEVEL[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_R_THETA_TOP_HAT_PERCENT_LEVEL[i] = 0;
        //         }
        //     }
        //     break;
        // }
        case ELLIPTICAL_SUB_ROI_PADDING:
        {
            for (int i=0; i<m_ELLIPTICAL_SUB_ROI_PADDING.GetSize(); i++)
            {
                if (i < numElements)
                {
                    if (rValue[i] <= 0)
                    {
                        return ELLIPTICAL_SUB_ROI_PADDING_NEG_OR_ZERO;
                    }
                    else
                    {
                        m_ELLIPTICAL_SUB_ROI_PADDING[i] = rValue[i];
                    }
                }
                else
                {
                    m_ELLIPTICAL_SUB_ROI_PADDING[i] = 0;
                }
            }
            break;
        }
        case CENTROID_BOT_BOUND:
        {
            for (int i=0; i<m_CENTROID_BOT_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_CENTROID_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_CENTROID_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case CENTROID_TOP_BOUND:
        {
            for (int i=0; i<m_CENTROID_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_CENTROID_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_CENTROID_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case BEAM_DIAMETER_BOT_BOUND:
        {
            for (int i=0; i<m_BEAM_DIAMETER_BOT_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_BEAM_DIAMETER_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_BEAM_DIAMETER_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case BEAM_DIAMETER_TOP_BOUND:
        {
            for (int i=0; i<m_BEAM_DIAMETER_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_BEAM_DIAMETER_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_BEAM_DIAMETER_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case DIVERGENCE_BOT_BOUND:
        {
            for (int i=0; i<m_DIVERGENCE_BOT_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_DIVERGENCE_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_DIVERGENCE_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case DIVERGENCE_TOP_BOUND:
        {
            for (int i=0; i<m_DIVERGENCE_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_DIVERGENCE_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_DIVERGENCE_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case GAUSS_PERF_TOP_BOUND:
        {
            for (int i=0; i<m_GAUSS_PERF_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_GAUSS_PERF_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_GAUSS_PERF_TOP_BOUND[i] = 0;
                }
            }
            break;
        }   
        case SYMMETRY_TOP_BOUND:
        {
            for (int i=0; i<m_SYMMETRY_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_SYMMETRY_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_SYMMETRY_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case PEARSON_BOT_BOUND:
        {
            for (int i=0; i<m_PEARSON_BOT_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_PEARSON_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_PEARSON_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case MAX_INTENSITY_BOT_BOUND:
        {
            for (int i=0; i<m_MAX_INTENSITY_BOT_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_MAX_INTENSITY_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_MAX_INTENSITY_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case MAX_INTENSITY_TOP_BOUND:
        {
            for (int i=0; i<m_MAX_INTENSITY_TOP_BOUND.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_MAX_INTENSITY_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_MAX_INTENSITY_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case PROBE_SIGNAL_DATA:
        {
            for (int i=0; i<m_PROBE_SIGNAL_DATA.GetSize(); i++)
            {
                if (i < numElements)
                {
                    m_PROBE_SIGNAL_DATA[i] = rValue[i];
                }
                else
                {
                    m_PROBE_SIGNAL_DATA[i] = 0;
                }
            }
            m_probe.SetParam(FuncGen::DATA, m_PROBE_SIGNAL_DATA);
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;
    }
    return CrashBaseObject::SUCCESS;
} // end of SetParam

CrashResult hpImageProcessing::SetParam(long paramID, CrashFloatArray &rValue)
{
    switch (paramID)
    {
        case CONTROL_WORD:
        {
            for (int i=0; i<m_CONTROL_WORD.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] < 0)
                    {
                        return CONTROL_WORD_NEG;
                    }
                    else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
                    {
                        m_CONTROL_WORD[i] = rValue[i];
                    }
                    else
                    {
                        return CONTROL_WORD_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_CONTROL_WORD[i] = 0;
                }
            }
            break;
        }
        case THRESHOLD_CUTOFF: 
        {
            for (int i=0; i<m_THRESHOLD_CUTOFF.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] > 100)
                    {
                        return THRESHOLD_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                    }
                    else if (rValue[i] >= 0)
                    {
                        m_THRESHOLD_CUTOFF[i] = rValue[i];
                    }
                    else
                    {
                        return THRESHOLD_CUTOFF_NEG;
                    }
                }
                else
                {
                    m_THRESHOLD_CUTOFF[i] = 0;
                }
            }
            break;
        }
        case LEAST_SQUARES_CUTOFF:
        {
            for (int i=0; i<m_LEAST_SQUARES_CUTOFF.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] > 100)
                    {
                        return LEAST_SQUARES_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                    }
                    else if (rValue[i] > 0)
                    {
                        m_LEAST_SQUARES_CUTOFF[i] = rValue[i];
                    }
                    else
                    {
                        return LEAST_SQUARES_CUTOFF_NEG_OR_ZERO;
                    }
                }
                else
                {
                    m_LEAST_SQUARES_CUTOFF[i] = 0;
                }
            }
            break;
        }
        case SUB_ROI_RADIUS:
        {
            for (int i=0; i<m_SUB_ROI_RADIUS.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
					if (rValue[i] <= 0)
                    {
                        return SUB_ROI_RADIUS_NEG_OR_ZERO;
                    }
					else
					{
						m_SUB_ROI_RADIUS[i] = rValue[i];
					}
                }
                else
                {
                    m_SUB_ROI_RADIUS[i] = 0;
                }
            }
            break;
        }
        case SYM_RADIAL_DISTANCE:
        {
            for (int i=0; i<m_SYM_RADIAL_DISTANCE.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
					if (rValue[i] <= 0)
                    {
                        return SYM_RADIAL_DISTANCE_NEG_OR_ZERO;
                    }
					else
					{
						m_SYM_RADIAL_DISTANCE[i] = rValue[i];
					}
                }
                else
                {
                    m_SYM_RADIAL_DISTANCE[i] = 0;
                }
            }
            break;
        }
        case ROI_X_OFFSET:
        {
            for (int i=0; i<m_ROI_X_OFFSET.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
					if (rValue[i] < 0)
					{
						return ROI_X_OFFSET_NEG;
					}
					else if (rValue[i] > MAX_ROI_WIDTH)
					{
						return ROI_X_OFFSET_GREATER_THAN_MAX;
					}
					else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
					{
						m_ROI_X_OFFSET[i] = rValue[i];
					}
					else
					{
                        return ROI_X_OFFSET_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_X_OFFSET[i] = 0;
                }
            }
            break;
        }
        case ROI_Y_OFFSET:
        {
            for (int i=0; i<m_ROI_Y_OFFSET.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] < 0)
					{
						return ROI_Y_OFFSET_NEG;
					}
					else if (rValue[i] > MAX_ROI_HEIGHT)
					{
						return ROI_Y_OFFSET_GREATER_THAN_MAX;
					}
					else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
					{
						m_ROI_Y_OFFSET[i] = rValue[i];
					}
					else
					{
                        return ROI_Y_OFFSET_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_Y_OFFSET[i] = 0;
                }
            }
            break;
        }
        case ROI_WIDTH:
        {
            for (int i=0; i<m_ROI_WIDTH.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] <= 0)
					{
						return ROI_WIDTH_NEG_OR_ZERO;
					}
					else if (rValue[i] > MAX_ROI_WIDTH)
					{
						return ROI_WIDTH_GREATER_THAN_MAX;
					}
					else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
					{
						m_ROI_WIDTH[i] = rValue[i];
					}
					else
					{
                        return ROI_WIDTH_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_WIDTH[i] = 0;
                }
            }
            break;
        }
        case ROI_HEIGHT:
        {
            for (int i=0; i<m_ROI_HEIGHT.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] <= 0)
					{
						return ROI_HEIGHT_NEG_OR_ZERO;
					}
					else if (rValue[i] > MAX_ROI_HEIGHT)
					{
						return ROI_HEIGHT_GREATER_THAN_MAX;
					}
					else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
					{
						m_ROI_HEIGHT[i] = rValue[i];
					}
					else
					{
                        return ROI_HEIGHT_NOT_AN_INTEGER;
                    }
                }
                else
                {
                    m_ROI_HEIGHT[i] = 0;
                }
            }
            break;
        }
        // case POWER_DENSITY_X_OFFSET:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_X_OFFSET.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return POWER_DENSITY_X_OFFSET_NEG;
        //             }
        //             else if (rValue[i] > MAX_ROI_WIDTH)
        //             {
        //                 return POWER_DENSITY_X_OFFSET_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_X_OFFSET[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_X_OFFSET_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_X_OFFSET[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_Y_OFFSET:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_Y_OFFSET.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_NEG;
        //             }
        //             else if (rValue[i] > MAX_ROI_HEIGHT)
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_Y_OFFSET[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_Y_OFFSET_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_Y_OFFSET[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_WIDTH:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_WIDTH.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_WIDTH_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > MAX_ROI_WIDTH)
        //             {
        //                 return POWER_DENSITY_WIDTH_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_WIDTH[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_WIDTH_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_WIDTH[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_HEIGHT:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_HEIGHT.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_HEIGHT_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > MAX_ROI_HEIGHT)
        //             {
        //                 return POWER_DENSITY_HEIGHT_GREATER_THAN_MAX;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_POWER_DENSITY_HEIGHT[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return POWER_DENSITY_HEIGHT_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_HEIGHT[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_MAJOR_RAD:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_MAJOR_RAD.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_MAJOR_RAD_NEG_OR_ZERO;
        //             }
        //             else
        //             {
        //                 m_POWER_DENSITY_MAJOR_RAD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_MAJOR_RAD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_MINOR_RAD:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_MINOR_RAD.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return POWER_DENSITY_MINOR_RAD_NEG_OR_ZERO;
        //             }
        //             else
        //             {
        //                 m_POWER_DENSITY_MINOR_RAD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_MINOR_RAD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case POWER_DENSITY_ANGLE:
        // {
        //     for (int i=0; i<m_POWER_DENSITY_ANGLE.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             m_POWER_DENSITY_ANGLE[i] = rValue[i];
        //         }
        //         else
        //         {
        //             m_POWER_DENSITY_ANGLE[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case EFFECTIVE_IRRADIATION_THRESHOLD:
        // {
        //     for (int i=0; i<m_EFFECTIVE_IRRADIATION_THRESHOLD.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return EFFECTIVE_IRRADIATION_THRESHOLD_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > 100)
        //             {
        //                 return EFFECTIVE_IRRADIATION_THRESHOLD_GREATER_THAN_ONE_HUNDRED;
        //             }
        //             else
        //             {
        //                 m_EFFECTIVE_IRRADIATION_THRESHOLD[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_EFFECTIVE_IRRADIATION_THRESHOLD[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case BEAM_POSITIONAL_STABILITY_LOWPASS:
        // {
        //     for (int i=0; i<m_BEAM_POSITIONAL_STABILITY_LOWPASS.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] < 0)
        //             {
        //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_NEG;
        //             }
        //             else if (rValue[i] > 1)
        //             {
        //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_GREATER_THAN_ONE;
        //             }
        //             else
        //             {
        //                 m_BEAM_POSITIONAL_STABILITY_LOWPASS[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_BEAM_POSITIONAL_STABILITY_LOWPASS[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case R_THETA_TOP_HAT_NUM_LINES:
        // {
        //     for (int i=0; i<m_R_THETA_TOP_HAT_NUM_LINES.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return R_THETA_TOP_HAT_NUM_LINES_NEG_OR_ZERO;
        //             }
        //             else if (fabs(static_cast<double>(rValue[i] - static_cast<float>(floor(static_cast<double>(rValue[i]))))) <= FLT_MIN)
        //             {
        //                 m_R_THETA_TOP_HAT_NUM_LINES[i] = rValue[i];
        //             }
        //             else
        //             {
        //                 return R_THETA_TOP_HAT_NUM_LINES_NOT_AN_INTEGER;
        //             }
        //         }
        //         else
        //         {
        //             m_R_THETA_TOP_HAT_NUM_LINES[i] = 0;
        //         }
        //     }
        //     break;
        // }
        // case R_THETA_TOP_HAT_PERCENT_LEVEL:
        // {
        //     for (int i=0; i<m_R_THETA_TOP_HAT_PERCENT_LEVEL.GetSize(); i++)
        //     {
        //         if (i < rValue.GetSize())
        //         {
        //             if (rValue[i] <= 0)
        //             {
        //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_NEG_OR_ZERO;
        //             }
        //             else if (rValue[i] > 100)
        //             {
        //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_GREATER_THAN_ONE_HUNDRED;
        //             }
        //             else
        //             {
        //                 m_R_THETA_TOP_HAT_PERCENT_LEVEL[i] = rValue[i];
        //             }
        //         }
        //         else
        //         {
        //             m_R_THETA_TOP_HAT_PERCENT_LEVEL[i] = 0;
        //         }
        //     }
        //     break;
        // }
        case ELLIPTICAL_SUB_ROI_PADDING:
        {
            for (int i=0; i<m_ELLIPTICAL_SUB_ROI_PADDING.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    if (rValue[i] <= 0)
                    {
                        return ELLIPTICAL_SUB_ROI_PADDING_NEG_OR_ZERO;
                    }
                    else
                    {
                        m_ELLIPTICAL_SUB_ROI_PADDING[i] = rValue[i];
                    }
                }
                else
                {
                    m_ELLIPTICAL_SUB_ROI_PADDING[i] = 0;
                }
            }
            break;
        }
        case CENTROID_BOT_BOUND:
        {
            for (int i=0; i<m_CENTROID_BOT_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_CENTROID_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_CENTROID_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case CENTROID_TOP_BOUND:
        {
            for (int i=0; i<m_CENTROID_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_CENTROID_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_CENTROID_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case BEAM_DIAMETER_BOT_BOUND:
        {
            for (int i=0; i<m_BEAM_DIAMETER_BOT_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_BEAM_DIAMETER_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_BEAM_DIAMETER_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case BEAM_DIAMETER_TOP_BOUND:
        {
            for (int i=0; i<m_BEAM_DIAMETER_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_BEAM_DIAMETER_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_BEAM_DIAMETER_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case DIVERGENCE_BOT_BOUND:
        {
            for (int i=0; i<m_DIVERGENCE_BOT_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_DIVERGENCE_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_DIVERGENCE_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case DIVERGENCE_TOP_BOUND:
        {
            for (int i=0; i<m_DIVERGENCE_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_DIVERGENCE_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_DIVERGENCE_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case GAUSS_PERF_TOP_BOUND:
        {
            for (int i=0; i<m_GAUSS_PERF_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_GAUSS_PERF_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_GAUSS_PERF_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case SYMMETRY_TOP_BOUND:
        {
            for (int i=0; i<m_SYMMETRY_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_SYMMETRY_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_SYMMETRY_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case PEARSON_BOT_BOUND:
        {
            for (int i=0; i<m_PEARSON_BOT_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_PEARSON_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_PEARSON_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case MAX_INTENSITY_BOT_BOUND:
        {
            for (int i=0; i<m_MAX_INTENSITY_BOT_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_MAX_INTENSITY_BOT_BOUND[i] = rValue[i];
                }
                else
                {
                    m_MAX_INTENSITY_BOT_BOUND[i] = 0;
                }
            }
            break;
        }
        case MAX_INTENSITY_TOP_BOUND:
        {
            for (int i=0; i<m_MAX_INTENSITY_TOP_BOUND.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_MAX_INTENSITY_TOP_BOUND[i] = rValue[i];
                }
                else
                {
                    m_MAX_INTENSITY_TOP_BOUND[i] = 0;
                }
            }
            break;
        }
        case PROBE_SIGNAL_DATA:
        {
            for (int i=0; i<m_PROBE_SIGNAL_DATA.GetSize(); i++)
            {
                if (i < rValue.GetSize())
                {
                    m_PROBE_SIGNAL_DATA[i] = rValue[i];
                }
                else
                {
                    m_PROBE_SIGNAL_DATA[i] = 0;
                }
            }
            m_probe.SetParam(FuncGen::DATA, m_PROBE_SIGNAL_DATA);
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;

    }
    return CrashBaseObject::SUCCESS;
} // end of SetParam

CrashResult hpImageProcessing::SetArrayByIndex(long paramID, float value, int index)
{
    switch (paramID)
    {
        case CONTROL_WORD:
        {
            if (index < m_CONTROL_WORD.GetSize())
            {
                if (value < 0)
                {
                    return CONTROL_WORD_NEG;
                }
                else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
                {
                    m_CONTROL_WORD[index] = value;
                }
                else
                {
                    return CONTROL_WORD_NOT_AN_INTEGER;
                }
            }
            break;
        }
        case THRESHOLD_CUTOFF: 
        {
            if (index < m_THRESHOLD_CUTOFF.GetSize())
            {
                if (value > 100)
                {
                    return THRESHOLD_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                }
                else if (value >= 0)
                {
                    m_THRESHOLD_CUTOFF[index] = value;
                }
                else
                {
                    return THRESHOLD_CUTOFF_NEG;
                }
            }
            break;
        }
        case LEAST_SQUARES_CUTOFF:
        {
            if (index < m_THRESHOLD_CUTOFF.GetSize())
            {
                if (value > 100)
                {
                    return LEAST_SQUARES_CUTOFF_GREATER_THAN_ONE_HUNDRED;
                }
                else if (value > 0)
                {
                    m_LEAST_SQUARES_CUTOFF[index] = value;
                }
                else
                {
                    return LEAST_SQUARES_CUTOFF_NEG_OR_ZERO;
                }
            }
            break;
        }
        case SUB_ROI_RADIUS:
        {
            if (index < m_SUB_ROI_RADIUS.GetSize())
            {
				if (value <= 0)
                {
                    return SUB_ROI_RADIUS_NEG_OR_ZERO;
                }
				else
				{
					m_SUB_ROI_RADIUS[index] = value;
				}
            }
            break;
        }
        case SYM_RADIAL_DISTANCE:
        {
            if (index < m_SYM_RADIAL_DISTANCE.GetSize())
            {
				if (value <= 0)
                {
                    return SYM_RADIAL_DISTANCE_NEG_OR_ZERO;
                }
				else
				{
					m_SYM_RADIAL_DISTANCE[index] = value;
				}
            }
            break;
        }
        case ROI_X_OFFSET:
        {
            if (index < m_ROI_X_OFFSET.GetSize())
            {
				if (value < 0)
				{
					return ROI_X_OFFSET_NEG;
				}
				else if (value > MAX_ROI_WIDTH)
				{
					return ROI_X_OFFSET_GREATER_THAN_MAX;
				}
				else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
				{
					m_ROI_X_OFFSET[index] = value;
				}
				else
				{
                    return ROI_X_OFFSET_NOT_AN_INTEGER;
                }
            }
            break;
        }
        case ROI_Y_OFFSET:
        {
            if (index < m_ROI_Y_OFFSET.GetSize())
            {
                if (value < 0)
				{
					return ROI_Y_OFFSET_NEG;
				}
				else if (value > MAX_ROI_HEIGHT)
				{
					return ROI_Y_OFFSET_GREATER_THAN_MAX;
				}
				else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
				{
					m_ROI_Y_OFFSET[index] = value;
				}
				else
				{
                    return ROI_Y_OFFSET_NOT_AN_INTEGER;
                }
			}
            break;
        }
        case ROI_WIDTH:
        {
            if (index < m_ROI_WIDTH.GetSize())
            {
                if (value <= 0)
				{
					return ROI_WIDTH_NEG_OR_ZERO;
				}
				else if (value > MAX_ROI_WIDTH)
				{
					return ROI_WIDTH_GREATER_THAN_MAX;
				}
				else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
				{
					m_ROI_WIDTH[index] = value;
				}
				else
				{
                    return ROI_WIDTH_NOT_AN_INTEGER;
                }
            }
            break;
        }
        case ROI_HEIGHT:
        {
            if (index < m_ROI_HEIGHT.GetSize())
            {
                if (value <= 0)
				{
					return ROI_HEIGHT_NEG_OR_ZERO;
				}
				else if (value > MAX_ROI_HEIGHT)
				{
					return ROI_HEIGHT_GREATER_THAN_MAX;
				}
				else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
				{
					m_ROI_HEIGHT[index] = value;
				}
				else
				{
                    return ROI_HEIGHT_NOT_AN_INTEGER;
                }
            }
            break;
        }
    //     case POWER_DENSITY_X_OFFSET:
    //     {
    //         if (index < m_POWER_DENSITY_X_OFFSET.GetSize())
    //         {
    //             if (value < 0)
    //             {
    //                 return POWER_DENSITY_X_OFFSET_NEG;
    //             }
    //             else if (value > MAX_ROI_WIDTH)
    //             {
    //                 return POWER_DENSITY_X_OFFSET_GREATER_THAN_MAX;
    //             }
    //             else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
    //             {
    //                 m_POWER_DENSITY_X_OFFSET[index] = value;
    //             }
    //             else
    //             {
    //                 return POWER_DENSITY_X_OFFSET_NOT_AN_INTEGER;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_Y_OFFSET:
    //     {
    //         if (index < m_POWER_DENSITY_Y_OFFSET.GetSize())
    //         {
    //             if (value < 0)
    //             {
    //                 return POWER_DENSITY_Y_OFFSET_NEG;
    //             }
    //             else if (value > MAX_ROI_HEIGHT)
    //             {
    //                 return POWER_DENSITY_Y_OFFSET_GREATER_THAN_MAX;
    //             }
    //             else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
    //             {
    //                 m_POWER_DENSITY_Y_OFFSET[index] = value;
    //             }
    //             else
    //             {
    //                 return POWER_DENSITY_Y_OFFSET_NOT_AN_INTEGER;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_WIDTH:
    //     {
    //         if (index < m_POWER_DENSITY_WIDTH.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return POWER_DENSITY_WIDTH_NEG_OR_ZERO;
    //             }
    //             else if (value > MAX_ROI_WIDTH)
    //             {
    //                 return POWER_DENSITY_WIDTH_GREATER_THAN_MAX;
    //             }
    //             else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
    //             {
    //                 m_POWER_DENSITY_WIDTH[index] = value;
    //             }
    //             else
    //             {
    //                 return POWER_DENSITY_WIDTH_NOT_AN_INTEGER;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_HEIGHT:
    //     {
    //         if (index < m_POWER_DENSITY_HEIGHT.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return POWER_DENSITY_HEIGHT_NEG_OR_ZERO;
    //             }
    //             else if (value > MAX_ROI_HEIGHT)
    //             {
    //                 return POWER_DENSITY_HEIGHT_GREATER_THAN_MAX;
    //             }
    //             else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
    //             {
    //                 m_POWER_DENSITY_HEIGHT[index] = value;
    //             }
    //             else
    //             {
    //                 return POWER_DENSITY_HEIGHT_NOT_AN_INTEGER;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_MAJOR_RAD:
    //     {
    //         if (index < m_POWER_DENSITY_MAJOR_RAD.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return POWER_DENSITY_MAJOR_RAD_NEG_OR_ZERO;
    //             }
    //             else
    //             {
    //                 m_POWER_DENSITY_MAJOR_RAD[index] = value;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_MINOR_RAD:
    //     {
    //         if (index < m_POWER_DENSITY_MINOR_RAD.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return POWER_DENSITY_MINOR_RAD_NEG_OR_ZERO;
    //             }
    //             else
    //             {
    //                 m_POWER_DENSITY_MINOR_RAD[index] = value;
    //             }
    //         }
    //         break;
    //     }
    //     case POWER_DENSITY_ANGLE:
    //     {
    //         if (index < m_POWER_DENSITY_ANGLE.GetSize())
    //         {
    //             m_POWER_DENSITY_ANGLE[index] = value;
    //         }
    //         break;
    //     }
    //     case EFFECTIVE_IRRADIATION_THRESHOLD:
    //     {
    //         if (index < m_EFFECTIVE_IRRADIATION_THRESHOLD.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return EFFECTIVE_IRRADIATION_THRESHOLD_NEG_OR_ZERO;
    //             }
    //             else if (value > 100)
    //             {
    //                 return EFFECTIVE_IRRADIATION_THRESHOLD_GREATER_THAN_ONE_HUNDRED;
    //             }
				// else
    //             {
    //                 m_EFFECTIVE_IRRADIATION_THRESHOLD[index] = value;
    //             }
    //         }
    //         break;
    //     }
    //     case BEAM_POSITIONAL_STABILITY_LOWPASS:
    //     {
    //         if (index < m_BEAM_POSITIONAL_STABILITY_LOWPASS.GetSize())
    //         {
    //             if (value < 0)
    //             {
    //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_NEG;
    //             }
				// else if (value > 1)
    //             {
    //                 return BEAM_POSITIONAL_STABILITY_LOWPASS_GREATER_THAN_ONE;
    //             }
    //             else
    //             {
    //                 m_BEAM_POSITIONAL_STABILITY_LOWPASS[index] = value;
    //             }
    //         }
    //         break;
    //     }
    //     case R_THETA_TOP_HAT_NUM_LINES:
    //     {
    //         if (index < m_R_THETA_TOP_HAT_NUM_LINES.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return R_THETA_TOP_HAT_NUM_LINES_NEG_OR_ZERO;
    //             }
    //             else if (fabs(static_cast<double>(value - static_cast<float>(floor(static_cast<double>(value))))) < FLT_MIN)
    //             {
    //                 m_R_THETA_TOP_HAT_NUM_LINES[index] = value;
    //             }
    //             else
    //             {
    //                 return R_THETA_TOP_HAT_NUM_LINES_NOT_AN_INTEGER;
    //             }
    //         }
    //         break;
    //     }
    //     case R_THETA_TOP_HAT_PERCENT_LEVEL:
    //     {
    //         if (index < m_R_THETA_TOP_HAT_PERCENT_LEVEL.GetSize())
    //         {
    //             if (value <= 0)
    //             {
    //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_NEG_OR_ZERO;
    //             }
				// else if (value > 100)
    //             {
    //                 return R_THETA_TOP_HAT_PERCENT_LEVEL_GREATER_THAN_ONE_HUNDRED;
    //             }
    //             else
    //             {
    //                 m_R_THETA_TOP_HAT_PERCENT_LEVEL[index] = value;
    //             }
    //         }
    //         break;
    //     }
        case ELLIPTICAL_SUB_ROI_PADDING:
        {
            if (index < m_ELLIPTICAL_SUB_ROI_PADDING.GetSize())
            {
                if (value <= 0)
                {
                    return ELLIPTICAL_SUB_ROI_PADDING_NEG_OR_ZERO;
                }
                else
                {
                    m_ELLIPTICAL_SUB_ROI_PADDING[index] = value;
                }
            }
            break;
        }
        case CENTROID_BOT_BOUND:
        {
            if (index < m_CENTROID_BOT_BOUND.GetSize())
            {
                m_CENTROID_BOT_BOUND[index] = value;
            }
            break;
        }
        case CENTROID_TOP_BOUND:
        {
            if (index < m_CENTROID_TOP_BOUND.GetSize())
            {
                m_CENTROID_TOP_BOUND[index] = value;
            }
        }
        case BEAM_DIAMETER_BOT_BOUND:
        {
            if (index < m_BEAM_DIAMETER_BOT_BOUND.GetSize())
            {
                m_BEAM_DIAMETER_BOT_BOUND[index] = value;
            }
            break;
        }
        case BEAM_DIAMETER_TOP_BOUND:
        {
            if (index < m_BEAM_DIAMETER_TOP_BOUND.GetSize())
            {
                m_BEAM_DIAMETER_TOP_BOUND[index] = value;
            }
            break;
        }
        case DIVERGENCE_BOT_BOUND:
        {
            if (index < m_DIVERGENCE_BOT_BOUND.GetSize())
            {
                m_DIVERGENCE_BOT_BOUND[index] = value;
            }
            break;
        }
        case DIVERGENCE_TOP_BOUND:
        {
            if (index < m_DIVERGENCE_TOP_BOUND.GetSize())
            {
                m_DIVERGENCE_TOP_BOUND[index] = value;
            }
            break;
        }
        case GAUSS_PERF_TOP_BOUND:
        {
            if (index < m_GAUSS_PERF_TOP_BOUND.GetSize())
            {
                m_GAUSS_PERF_TOP_BOUND[index] = value;
            }
            break;
        }
        case SYMMETRY_TOP_BOUND:
        {
            if (index < m_SYMMETRY_TOP_BOUND.GetSize())
            {
                m_SYMMETRY_TOP_BOUND[index] = value;
            }
            break;
        }
        case PEARSON_BOT_BOUND:
        {
            if (index < m_PEARSON_BOT_BOUND.GetSize())
            {
                m_PEARSON_BOT_BOUND[index] = value;
            }
            break;
        }
        case MAX_INTENSITY_BOT_BOUND:
        {
            if (index < m_MAX_INTENSITY_BOT_BOUND.GetSize())
            {
                m_MAX_INTENSITY_BOT_BOUND[index] = value;
            }
            break;
        }
        case MAX_INTENSITY_TOP_BOUND:
        {
            if (index < m_MAX_INTENSITY_TOP_BOUND.GetSize())
            {
                m_MAX_INTENSITY_TOP_BOUND[index] = value;
            }
            break;
        }
        case PROBE_SIGNAL_DATA:
        {
            if (index < m_PROBE_SIGNAL_DATA.GetSize())
            {
                m_PROBE_SIGNAL_DATA[index] = value;
            }
            m_probe.SetParam(FuncGen::DATA, m_PROBE_SIGNAL_DATA);
            break;
        }
        default: return CrashBaseObject::INVALID_PARAM_ID;
            break;
    }
    return CrashBaseObject::SUCCESS;
} // end of SetArrayByIndex

CrashResult hpImageProcessing::SetParamStruct(hpImageProcessingParams& paramStruct)
{
    CrashResult result;
    result = hpImageProcessing::SetParam(static_cast<long>(NUM_ROIS),paramStruct.NUM_ROIS);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(CONTROL_WORD),paramStruct.CONTROL_WORD);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(LOWPASS_SMOOTHING_FACTOR),paramStruct.LOWPASS_SMOOTHING_FACTOR);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(MED_FILT_PADDING),paramStruct.MED_FILT_PADDING);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(THRESHOLD_CUTOFF),paramStruct.THRESHOLD_CUTOFF);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(LEAST_SQUARES_CUTOFF),paramStruct.LEAST_SQUARES_CUTOFF);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(LEAST_SQUARES_FIT_DELTA),paramStruct.LEAST_SQUARES_FIT_DELTA);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(SUB_ROI_RADIUS),paramStruct.SUB_ROI_RADIUS);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(SYM_RADIAL_LINES),paramStruct.SYM_RADIAL_LINES);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(SYM_RADIAL_DISTANCE),paramStruct.SYM_RADIAL_DISTANCE);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(COM_VALID_DISTANCE),paramStruct.COM_VALID_DISTANCE);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(NUM_IMAGES_TO_SUM),paramStruct.NUM_IMAGES_TO_SUM);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(ROI_X_OFFSET),paramStruct.ROI_X_OFFSET);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(ROI_Y_OFFSET),paramStruct.ROI_Y_OFFSET);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(ROI_WIDTH),paramStruct.ROI_WIDTH);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(ROI_HEIGHT),paramStruct.ROI_HEIGHT);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_X_OFFSET),paramStruct.POWER_DENSITY_X_OFFSET);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_Y_OFFSET),paramStruct.POWER_DENSITY_Y_OFFSET);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_WIDTH),paramStruct.POWER_DENSITY_WIDTH);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_HEIGHT),paramStruct.POWER_DENSITY_HEIGHT);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_MAJOR_RAD),paramStruct.POWER_DENSITY_MAJOR_RAD);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_MINOR_RAD),paramStruct.POWER_DENSITY_MINOR_RAD);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(POWER_DENSITY_ANGLE),paramStruct.POWER_DENSITY_ANGLE);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(EFFECTIVE_IRRADIATION_THRESHOLD),paramStruct.EFFECTIVE_IRRADIATION_THRESHOLD);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(BEAM_POSITIONAL_STABILITY_LOWPASS),paramStruct.BEAM_POSITIONAL_STABILITY_LOWPASS);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(R_THETA_TOP_HAT_NUM_LINES),paramStruct.R_THETA_TOP_HAT_NUM_LINES);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    // result = hpImageProcessing::SetParam(static_cast<long>(R_THETA_TOP_HAT_PERCENT_LEVEL),paramStruct.R_THETA_TOP_HAT_PERCENT_LEVEL);
    // if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
    //     return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(CENTROID_BOT_BOUND), paramStruct.CENTROID_BOT_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(CENTROID_TOP_BOUND), paramStruct.CENTROID_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(BEAM_DIAMETER_BOT_BOUND), paramStruct.BEAM_DIAMETER_BOT_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(BEAM_DIAMETER_TOP_BOUND), paramStruct.BEAM_DIAMETER_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(DIVERGENCE_BOT_BOUND), paramStruct.DIVERGENCE_BOT_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(DIVERGENCE_TOP_BOUND), paramStruct.DIVERGENCE_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(GAUSS_PERF_TOP_BOUND), paramStruct.GAUSS_PERF_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(SYMMETRY_TOP_BOUND), paramStruct.SYMMETRY_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PEARSON_BOT_BOUND), paramStruct.PEARSON_BOT_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(MAX_INTENSITY_BOT_BOUND), paramStruct.MAX_INTENSITY_BOT_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(MAX_INTENSITY_TOP_BOUND), paramStruct.MAX_INTENSITY_TOP_BOUND);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_ROI), paramStruct.PROBE_ROI);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_LOCATION), paramStruct.PROBE_LOCATION);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_MODE), paramStruct.PROBE_MODE);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_SIGNAL_PERIOD), paramStruct.PROBE_SIGNAL_PERIOD);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_SIGNAL_AMPLITUDE), paramStruct.PROBE_SIGNAL_AMPLITUDE);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_SIGNAL_OFFSET), paramStruct.PROBE_SIGNAL_OFFSET);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}
    result = hpImageProcessing::SetParam(static_cast<long>(PROBE_SIGNAL_DATA), paramStruct.PROBE_SIGNAL_DATA);
    if (result != static_cast<unsigned long>(CrashBaseObject::SUCCESS)){
        return result;}

    return CrashBaseObject::SUCCESS;
} // end of SetParamStruct

void hpImageProcessing::setInputImage2ImageAverage()
{
    for (unsigned int r=1; r<=m_inputImageHeight; r++)
    {
        for (unsigned int c=1; c<=m_inputImageWidth; c++)
        {
            m_inputImage(r,c) = m_imageSum(r,c)/static_cast<double>(m_numberImagesSummed);
        }
    }
    m_numberImagesSummed = 0;
    m_imageSum.zeros();
}

void hpImageProcessing::setInputImage2ImageSum()
{
    for (unsigned int r=1; r<=m_inputImageHeight; r++)
    {
        for (unsigned int c=1; c<=m_inputImageWidth; c++)
        {
            m_inputImage(r,c) = m_imageSum(r,c);
        }
    }
    m_numberImagesSummed = 0;
    m_imageSum.zeros();
}

void hpImageProcessing::clearCalibrationImage(void)
{
    for (int ind=0; ind<m_CALIBRATION_IMAGE.GetSize(); ind++)
    {
        m_CALIBRATION_IMAGE[ind] = 0;
    }
    m_numberImagesInCalibration = 0;
    m_calibrationCompleted = false;
}

void hpImageProcessing::averageCalibrationImage(void)
{
    for (unsigned int ind=0; ind<m_maxImageHeight*m_maxImageWidth; ind++)
    {
        m_CALIBRATION_IMAGE[ind] /= static_cast<float>(m_numberImagesInCalibration);
    }
}

uint16_t* hpImageProcessing::getCalibratedImage(void)
{
    unsigned long ind = 0;

    for (unsigned int r=1; r<=m_inputImageHeight; r++)
    {
        for (unsigned int c=1; c<=m_inputImageWidth; c++)
		{
            if (m_inputImage(r,c) > 0)
            {
                m_calibratedImage[ind] = static_cast<uint16_t>(m_inputImage(r,c));
            }
			else
			{
				m_calibratedImage[ind] = 0;
			}
            ind++;
        }
    }

    return m_calibratedImage;
}

int16_t* hpImageProcessing::getCalibrationImage(void)
{
    unsigned long ind = 0;

    for (unsigned int r=1; r<=m_maxImageHeight; r++)
    {
        for (unsigned int c=1; c<=m_maxImageWidth; c++)
        {
            m_calibrationImage[ind] = static_cast<int16_t>(m_CALIBRATION_IMAGE[ind]);
            ind++;
        }
    }
    return m_calibrationImage;
}

bool hpImageProcessing::detectRoiOverlap()
{
    // detect overlapping rois
    for (unsigned int i=0; i<m_NUM_ROIS; i++)
    {
		m_algoStatusWord[i] = 0x0;
        if (static_cast<unsigned int>(m_ROI_X_OFFSET[i] + m_ROI_WIDTH[i]) > m_inputImageWidth
            || static_cast<unsigned int>(m_ROI_Y_OFFSET[i] + m_ROI_HEIGHT[i]) > m_inputImageHeight
            || m_ROI_X_OFFSET[i] + m_ROI_WIDTH[i] < 0
            || m_ROI_Y_OFFSET[i] + m_ROI_HEIGHT[i] < 0)
        {
            m_algoStatusWord[i] |= (1 << ALGO_STATUS_BIT_ROI_OUT_OF_BOUNDS);
			return true;
        }
        for (unsigned int j=0; j<(m_NUM_ROIS-1); j++)
        {
            if (i != j && i > j)
            {
                if (((m_ROI_X_OFFSET[i] + m_ROI_WIDTH[i] > m_ROI_X_OFFSET[j] && m_ROI_X_OFFSET[i] < m_ROI_X_OFFSET[j] + m_ROI_WIDTH[j])
                    && (m_ROI_Y_OFFSET[i] + m_ROI_HEIGHT[i] > m_ROI_Y_OFFSET[j] && m_ROI_Y_OFFSET[i] < m_ROI_Y_OFFSET[j] + m_ROI_HEIGHT[j]))
                    || ((m_ROI_X_OFFSET[j] + m_ROI_WIDTH[j] > m_ROI_X_OFFSET[i]  && m_ROI_X_OFFSET[j]  < m_ROI_X_OFFSET[i] + m_ROI_WIDTH[i])
                    && (m_ROI_Y_OFFSET[j] + m_ROI_HEIGHT[j] > m_ROI_Y_OFFSET[i] && m_ROI_Y_OFFSET[j] < m_ROI_Y_OFFSET[i] + m_ROI_HEIGHT[i])))
                {
                    m_algoStatusWord[i] |= (1 << ALGO_STATUS_BIT_ROI_OVERLAP);
					return true;
                }
            }
        }
    }
	return false;
}

void hpImageProcessing::runIpAlgo()
{
    if (m_finishedSumming)
    {
        for (unsigned int i=0; i<m_NUM_ROIS; i++)
        {
            m_imageProObj[i].setImageColRoi(m_inputImage, static_cast<int>(m_ROI_X_OFFSET[i]), 
                   static_cast<int>(m_ROI_Y_OFFSET[i]), static_cast<int>(m_ROI_HEIGHT[i]), static_cast<int>(m_ROI_WIDTH[i]));

            /* runImageProDebug */ 
        	if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_MEDIAN_FILTER_BIT))
            {
                m_imageProObj[i].medianFilter(m_MED_FILT_PADDING);
        		m_imageProObj[i].setImage2ImageResult();
            }

            m_imageProObj[i].calcRawMetrics();

            m_imageProObj[i].sumBorderAndStoreInArray();
            if (m_imageProObj[i].getBorderMax() > ((static_cast<double>(m_THRESHOLD_CUTOFF[i])/100.0)*m_imageProObj[i].getMaxRawIntens()))
            {
                m_algoStatusWord[i] |= (1 << ALGO_STATUS_BIT_BORDER_ABOVE_THRESHOLD);
            }

            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_BACKGROUND_SUBTRACTION_BIT))
    		{
    			m_imageProObj[i].removeBorderMedian();
    		}

            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_LOWPASS_FILTERING_BIT))
            {
                if (! m_imageProObj[i].getLowpassInitialized())
                {
                    m_imageProObj[i].initializeLowpassStates();
                }
                else
                {
                    m_imageProObj[i].lowpassFilterOnImage(static_cast<double>(m_LOWPASS_SMOOTHING_FACTOR));
                    m_imageProObj[i].setImage2LowpassStates();
                }
            }

            /* If we are configured to do so, null out all the pixels that are beyond a
             * certain radius from the maximum pixel. */
            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_CIRC_SUB_ROI_OVER_MAX_INT_BIT))
            {
        		m_imageProObj[i].setCircROI(m_imageProObj[i].getMaxRawIndy(),m_imageProObj[i].getMaxRawIndx(),static_cast<double>(m_SUB_ROI_RADIUS[i]));
            }
            if ( (static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_ELLIPTICAL_SUB_ROI_OVER_MAX_INT_BIT)) > 0 
                && (static_cast<int>(m_algoStatusWord[i]) & (1 << ALGO_STATUS_BIT_MAX_INTENSITY_OUT_OF_RANGE)) <= 0)
            {
                if (m_imageProObj[i].getSimpCOM(0) > 0 && m_imageProObj[i].getSimpCOM(1) > 0 
                    && m_imageProObj[i].getSimpCOM(0) < m_ROI_X_OFFSET[i] + m_ROI_WIDTH[i]
                    && m_imageProObj[i].getSimpCOM(1) < m_ROI_Y_OFFSET[i] + m_ROI_HEIGHT[i])
                {
                    m_imageProObj[i].setEllipticalRoi(static_cast<double>(m_ELLIPTICAL_SUB_ROI_PADDING[i]));
                }
            }

            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_SIMPLE_THRESHOLDING_BIT))
            {
                m_imageProObj[i].cutImage((static_cast<double>(m_THRESHOLD_CUTOFF[i])/100.0)*m_imageProObj[i].getMaxRawIntens());
            }

        	m_imageProObj[i].calcSimpMetrics();
            m_imageProObj[i].calcDivergenceAngle(static_cast<double>(m_FOCAL_LENGTH));
            
            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_CORRECT_FOR_THRESHOLDING_BIT))
            {
        		m_imageProObj[i].correctForThresholding(static_cast<double>(m_THRESHOLD_CUTOFF[i]/100.0)); 
            }

            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_SYMMETRY_METRICS_BIT))
            {
        		m_imageProObj[i].calcSymmetryMetric(static_cast<int>(m_SYM_RADIAL_LINES), static_cast<double>(m_SYM_RADIAL_DISTANCE[i]));
            }
        	if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_LS_GAUSSIAN_FIT_METRICS_BIT))
            {
        		m_imageProObj[i].cutImage((static_cast<double>(m_LEAST_SQUARES_CUTOFF[i])/100.0)*m_imageProObj[i].getMaxRawIntens());
        		m_imageProObj[i].leastSquaresLogFit(static_cast<double>(m_LEAST_SQUARES_FIT_DELTA));
        		m_imageProObj[i].calcLSfitMetrics();
                if (!(m_imageProObj[i].getLsCOM(1) <= m_imageProObj[i].getLsCOM(1)) || !(m_imageProObj[i].getLsCOM(2) <= m_imageProObj[i].getLsCOM(2)))
                {
                    m_algoStatusWord[i] |= (1 << ALGO_STATUS_BIT_LS_NOT_CONVERGED);
                }
            }

            if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_MISCELLANEOUS_METRICS_BIT))
            {
                // m_imageProObj[i].calcCWPower(static_cast<double>(m_WATT_PER_COUNT));
                // m_imageProObj[i].calcMaximumPower(static_cast<double>(m_WATT_PER_COUNT));
                // m_imageProObj[i].calcGoodnessOfFitMetric();
                // if ( static_cast<int>(m_CONTROL_WORD[i]) & (1 << ENABLE_RECTANGULAR_POWER_DENSITY_BIT))
                // {
                //     m_imageProObj[i].calcPowerDensity(static_cast<int>(m_POWER_DENSITY_X_OFFSET[i]), static_cast<int>(m_POWER_DENSITY_Y_OFFSET[i]), static_cast<int>(m_POWER_DENSITY_WIDTH[i]), static_cast<int>(m_POWER_DENSITY_HEIGHT[i]), static_cast<double>(m_WATT_PER_COUNT), static_cast<double>(m_MM_PER_PIXEL));
                // }
                // else
                // {
                //     m_imageProObj[i].calcPowerDensityEllipse(static_cast<int>(m_POWER_DENSITY_X_OFFSET[i]), static_cast<int>(m_POWER_DENSITY_Y_OFFSET[i]),static_cast<int>(m_POWER_DENSITY_MAJOR_RAD[i]), static_cast<int>(m_POWER_DENSITY_MINOR_RAD[i]), static_cast<double>(m_POWER_DENSITY_ANGLE[i]), static_cast<double>(m_WATT_PER_COUNT), static_cast<double>(m_MM_PER_PIXEL));
                // }
                // m_imageProObj[i].calcEffectiveMetrics(static_cast<double>(m_EFFECTIVE_IRRADIATION_THRESHOLD[i]/100.0), static_cast<double>(m_WATT_PER_COUNT), static_cast<double>(m_MM_PER_PIXEL));
                // if (m_imageProObj[i].getBeamPositionalStabilityInitialized())
                // {
                //     m_imageProObj[i].calcBeamPositionalStability(static_cast<double>(m_BEAM_POSITIONAL_STABILITY_LOWPASS[i]));
                // }
                // else
                // {
                //     m_imageProObj[i].initBeamPositionalStability();
                // }
                m_imageProObj[i].calcE2Width();

                m_imageProObj[i].calcXSectionFFT();
                // m_imageProObj[i].calcRThetaTopHat(static_cast<int>(m_R_THETA_TOP_HAT_NUM_LINES[i]), static_cast<double>(m_R_THETA_TOP_HAT_PERCENT_LEVEL[i]));
                m_imageProObj[i].calcPearsonCorrelation();
                
            }
#ifndef LC12
            getImageColRoi(m_imageProObj[i].getImage(),static_cast<int>(m_ROI_X_OFFSET[i]), 
                   static_cast<int>(m_ROI_Y_OFFSET[i]), static_cast<int>(m_ROI_HEIGHT[i]), static_cast<int>(m_ROI_WIDTH[i]));
#endif
        }
    }
	/* End runImageProDebug */
}

hpImageProcessingOutputSignals* hpImageProcessing::getOutputSignals(int roiNumber)
{
    m_outputStruct.borderLevel = static_cast<double>(m_imageProObj[roiNumber].getBorderLevel());
    m_outputStruct.borderMax = static_cast<double>(m_imageProObj[roiNumber].getBorderMax());
    m_outputStruct.rawMaxIntensity = static_cast<double>(m_imageProObj[roiNumber].getMaxRawIntens());
    m_outputStruct.rawMaxIndX = static_cast<double>(m_imageProObj[roiNumber].getMaxRawIndx());
    m_outputStruct.rawMaxIndY = static_cast<double>(m_imageProObj[roiNumber].getMaxRawIndy());
    m_outputStruct.rawMinIntensity = static_cast<double>(m_imageProObj[roiNumber].getMinRawIntens());
    m_outputStruct.rawSumIntensity = static_cast<double>(m_imageProObj[roiNumber].getSumRawIntens());
    m_outputStruct.rawMeanIntensity = static_cast<double>(m_imageProObj[roiNumber].getMeanRawIntens());
    m_outputStruct.rawMedianIntensity = static_cast<double>(m_imageProObj[roiNumber].getMedianRawIntens());
    m_outputStruct.simpleCentroidX = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpCOM(1));
    m_outputStruct.simpleCentroidY = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpCOM(2));
    m_outputStruct.simpleCentroidU = static_cast<double>(m_imageProObj[roiNumber].getSimpCOMUV(1));
    m_outputStruct.simpleCentroidV = static_cast<double>(m_imageProObj[roiNumber].getSimpCOMUV(2));
    m_outputStruct.simpleAngle = static_cast<double>(180.0/M_PI*m_imageProObj[roiNumber].getSimpAngle());
    m_outputStruct.simpleAmplitude = static_cast<double>(m_imageProObj[roiNumber].getSimpMaxIntens());
    m_outputStruct.simpleVarianceMajor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpVxx());
    m_outputStruct.simpleVarianceMinor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpVyy());
    m_outputStruct.simpleDiameterMajor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpBeamDiamMajor());
    m_outputStruct.simpleDiameterMinor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpBeamDiamMinor());
    m_outputStruct.simpleEnergyTotal = static_cast<double>(m_imageProObj[roiNumber].getSimpEnergyTotal());
    m_outputStruct.simpleEllipseRatio = static_cast<double>(m_imageProObj[roiNumber].getSimpEllip());
    m_outputStruct.simpleEnergyEllipse = static_cast<double>(m_imageProObj[roiNumber].getSimpEnergyInEllipse());
    m_outputStruct.simpleEnergyRatio = static_cast<double>(m_imageProObj[roiNumber].getSimpEnergyRatio());
    m_outputStruct.simplePerformance = static_cast<double>(m_imageProObj[roiNumber].getSimpPerfMetric());
    m_outputStruct.simpleCrossSectionalArea = static_cast<double>(m_imageProObj[roiNumber].getSimpCrossSectionalArea());
    m_outputStruct.simpleDivergenceAngleMajor = static_cast<double>(m_MM_PER_PIXEL*180.0/M_PI*m_imageProObj[roiNumber].getSimpDivergenceAngle(0));
    m_outputStruct.simpleDivergenceAngleMinor = static_cast<double>(m_MM_PER_PIXEL*180.0/M_PI*m_imageProObj[roiNumber].getSimpDivergenceAngle(1));
    m_outputStruct.simpleAzimuthAngle = static_cast<double>(180.0/M_PI*m_imageProObj[roiNumber].getSimpAzimuthAngle());
    m_outputStruct.symmetryMetric = static_cast<double>(m_imageProObj[roiNumber].getSymmetryMetric());
    m_outputStruct.lsFitCentroidX = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsCOM(1));
    m_outputStruct.lsFitCentroidY = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsCOM(2));
    m_outputStruct.lsFitAngle = static_cast<double>(180.0/M_PI*m_imageProObj[roiNumber].getLsAngle());
    m_outputStruct.lsFitAmplitude = static_cast<double>(m_imageProObj[roiNumber].getLsMaxIntens());
    m_outputStruct.lsFitVarianceMajor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsVxx());
    m_outputStruct.lsFitVarianceMinor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsVyy());
    m_outputStruct.lsFitDiameterMajor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsBeamDiam_x());
    m_outputStruct.lsFitDiameterMinor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getLsBeamDiam_y());
    m_outputStruct.lsFitPerformance = static_cast<double>(m_imageProObj[roiNumber].getLsPerfMetric());
    m_outputStruct.lsFitEllipseRatio = static_cast<double>(m_imageProObj[roiNumber].getLsEllip());
    // m_outputStruct.bgCWPower = static_cast<double>(m_imageProObj[roiNumber].getSimpCWPower());
    // m_outputStruct.bgPowerDensity = static_cast<double>(m_imageProObj[roiNumber].getSimpPowerDensity());
    // m_outputStruct.bgBeamPositionalStabilityCentX = static_cast<double>(m_imageProObj[roiNumber].getSimpBeamPositionalStabilityCentX());
    // m_outputStruct.bgBeamPositionalStabilityCentY = static_cast<double>(m_imageProObj[roiNumber].getSimpBeamPositionalStabilityCentY());
    // m_outputStruct.bgBeamPositionalStabilityDiamX = static_cast<double>(m_imageProObj[roiNumber].getSimpBeamPositionalStabilityDiamX());
    // m_outputStruct.bgBeamPositionalStabilityDiamY = static_cast<double>(m_imageProObj[roiNumber].getSimpBeamPositionalStabilityDiamY());
    // m_outputStruct.bgMaximumPower = static_cast<double>(m_imageProObj[roiNumber].getSimpMaximumPower());
    // m_outputStruct.bgEffectiveIrradiationArea = static_cast<double>(m_imageProObj[roiNumber].getSimpEffectiveIrradiationArea());
    // m_outputStruct.bgEffectiveAveragePower = static_cast<double>(m_imageProObj[roiNumber].getSimpEffectiveAveragePower());
    // m_outputStruct.bgFlatnessFactor = static_cast<double>(m_imageProObj[roiNumber].getSimpFlatnessFactor());
    // m_outputStruct.bgBeamUniformity = static_cast<double>(m_imageProObj[roiNumber].getSimpBeamUniformity());
    // m_outputStruct.bgPlateauUniformity = static_cast<double>(m_imageProObj[roiNumber].getSimpPlateauUniformity());
    // m_outputStruct.bgEdgeSteepness = static_cast<double>(m_imageProObj[roiNumber].getSimpEdgeSteepness());
    // m_outputStruct.bgRoughnessOfFit = static_cast<double>(m_imageProObj[roiNumber].getSimpRoughnessOfFit());
    // m_outputStruct.bgGoodnessOfFit = static_cast<double>(m_imageProObj[roiNumber].getSimpGoodnessOfFit());
    m_outputStruct.pearsonCorrelation = static_cast<double>(m_imageProObj[roiNumber].getPearsonCorrelation());
    m_outputStruct.e2DiameterMajor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpleE2Diameter(0));
    m_outputStruct.e2DiameterMinor = static_cast<double>(m_MM_PER_PIXEL*m_imageProObj[roiNumber].getSimpleE2Diameter(1));
    m_outputStruct.xSectionFFTMajorFreqSum = m_imageProObj[roiNumber].getXSectionFFTMajorFreqErrorSum();
    m_outputStruct.xSectionFFTMajorFreqAverage = m_imageProObj[roiNumber].getXSectionFFTMajorFreqErrorAverage();
    m_outputStruct.xSectionFFTMajorFreq = static_cast<double>(m_imageProObj[roiNumber].getXSectionFFTMajorFreq0());
    m_outputStruct.xSectionFFTMajorAmp = static_cast<double>(m_imageProObj[roiNumber].getXSectionFFTMajorAmp0());
    m_outputStruct.xSectionFFTMinorFreqSum = m_imageProObj[roiNumber].getXSectionFFTMinorFreqErrorSum();
    m_outputStruct.xSectionFFTMinorFreqAverage = m_imageProObj[roiNumber].getXSectionFFTMinorFreqErrorAverage();
    m_outputStruct.xSectionFFTMinorFreq = static_cast<double>(m_imageProObj[roiNumber].getXSectionFFTMinorFreq0());
    m_outputStruct.xSectionFFTMinorAmp = static_cast<double>(m_imageProObj[roiNumber].getXSectionFFTMinorAmp0());
    m_outputStruct.xSectionFFTMajorFreqVec = m_imageProObj[roiNumber].getXSectionFFTMajorFreqVec();
    m_outputStruct.xSectionFFTMajorAmpVec = m_imageProObj[roiNumber].getXSectionFFTMajorAmpVec();
    // m_outputStruct.xSectionFFTMajorDataVec = m_imageProObj[roiNumber].getXSectionFFTMajorDataVec();
    m_outputStruct.xSectionFFTMinorFreqVec = m_imageProObj[roiNumber].getXSectionFFTMinorFreqVec();
    m_outputStruct.xSectionFFTMinorAmpVec = m_imageProObj[roiNumber].getXSectionFFTMinorAmpVec();
    // m_outputStruct.xSectionFFTMinorDataVec = m_imageProObj[roiNumber].getXSectionFFTMinorDataVec();
    // m_outputStruct.rThetaHatE2Data = m_imageProObj[roiNumber].getRThetaHatE2Data();
    // m_outputStruct.rThetaHatEData = m_imageProObj[roiNumber].getRThetaHatEData();
    // m_outputStruct.rThetaHatPercentData = m_imageProObj[roiNumber].getRThetaHatPercentData();
    // m_outputStruct.rThetaHatESize = static_cast<int>(m_imageProObj[roiNumber].getRThetaHatESize());
    // m_outputStruct.rThetaHatE2Size = static_cast<int>(m_imageProObj[roiNumber].getRThetaHatE2Size());
    // m_outputStruct.rThetaHatPercSize = static_cast<int>(m_imageProObj[roiNumber].getRThetaHatPercSize());
    m_outputStruct.xSectionFFTMajorSize = static_cast<int>(m_imageProObj[roiNumber].getXSectionFFTMajorSize());
    m_outputStruct.xSectionFFTMinorSize = static_cast<int>(m_imageProObj[roiNumber].getXSectionFFTMinorSize());
    // m_outputStruct.rThetaHatE2Contrast = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatE2Contrast());
    // m_outputStruct.rThetaHatEContrast = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatEContrast());
    // m_outputStruct.rThetaHatPercContrast = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatPercContrast());
    // m_outputStruct.rThetaHatE2PkToPk = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatE2PkToPk());
    // m_outputStruct.rThetaHatEPkToPk = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatEPkToPk());
    // m_outputStruct.rThetaHatPercPkToPk = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatPercPkToPk());
    // m_outputStruct.rThetaHatE2Average = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatE2Average());
    // m_outputStruct.rThetaHatEAverage = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatEAverage());
    // m_outputStruct.rThetaHatPercAverage = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatPercAverage());
    // m_outputStruct.rThetaHatRadiusE = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatRadiusE());
    // m_outputStruct.rThetaHatRadiusE2 = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatRadiusE2());
    // m_outputStruct.rThetaHatRadiusPerc = static_cast<double>(m_imageProObj[roiNumber].getRThetaHatRadiusPerc());
    m_outputStruct.maxIntensityBeforeCalibration = m_inputImageMaxVal;
    m_outputStruct.minIntensityBeforeCalibration = m_inputImageMinVal;
    m_outputStruct.fringeMetric = 0.0;


    if (m_PROBE_ROI == roiNumber)
    {
        switch (m_PROBE_LOCATION)
        {
            case PROBE_INACTIVE:
            {
                break;
            }
            case OUTPUT_CENTROID_X:
            {
                m_outputStruct.simpleCentroidX += static_cast<double>(m_probe.GetOutput());
                break;
            }
            case OUTPUT_CENTROID_Y:
            {
                m_outputStruct.simpleCentroidY += static_cast<double>(m_probe.GetOutput());
                break;
            }
            case OUTPUT_DIAMETER_MAJOR:
            {
                m_outputStruct.simpleDiameterMajor += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_DIAMETER_MINOR:
            {
                m_outputStruct.simpleDiameterMinor += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_DIVERGENCE_MAJOR:
            {
                m_outputStruct.simpleDivergenceAngleMajor += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_DIVERGENCE_MINOR:
            {
                m_outputStruct.simpleDivergenceAngleMinor += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_GAUSSIAN:
            {
                m_outputStruct.simplePerformance += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_SYMMETRY:
            {
                m_outputStruct.symmetryMetric += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            case OUTPUT_PEARSON:
            {
                m_outputStruct.pearsonCorrelation += static_cast<double>(m_probe.GetOutput());;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (m_outputStruct.simpleCentroidX < (m_outputStruct.simpleDiameterMajor)/2.0 ||
        m_outputStruct.simpleCentroidX > (static_cast<double>(m_ROI_WIDTH[roiNumber])*m_MM_PER_PIXEL - (m_outputStruct.simpleDiameterMajor)/2.0) ||
        m_outputStruct.simpleCentroidY < (m_outputStruct.simpleDiameterMinor)/2.0 ||
        m_outputStruct.simpleCentroidY > (static_cast<double>(m_ROI_HEIGHT[roiNumber])*m_MM_PER_PIXEL - (m_outputStruct.simpleDiameterMinor)/2.0))
    {
        m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_CLIP_BEAM);
    }

    if (fabs(static_cast<double>(m_outputStruct.simpleCentroidX-m_outputStruct.rawMaxIndX*m_MM_PER_PIXEL)) > m_COM_VALID_DISTANCE*m_MM_PER_PIXEL 
        || fabs(static_cast<double>(m_outputStruct.simpleCentroidY-m_outputStruct.rawMaxIndY*m_MM_PER_PIXEL)) > m_COM_VALID_DISTANCE*m_MM_PER_PIXEL)
    {
        m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_COM_NOT_VALID);
    }

    if (m_outputStruct.simplePerformance > static_cast<double>(m_GAUSS_PERF_TOP_BOUND[roiNumber]))
    {
        m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_GAUSSIAN_OUT_OF_RANGE);
    }

    if ( static_cast<int>(m_CONTROL_WORD[roiNumber]) & (1 << ENABLE_OUT_OF_RANGE_WARNINGS_BIT))
    {
        if (m_outputStruct.simpleDiameterMajor < m_BEAM_DIAMETER_BOT_BOUND[2*roiNumber] || m_outputStruct.simpleDiameterMajor > m_BEAM_DIAMETER_TOP_BOUND[2*roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_DIAMETER_MAJOR_OUT_OF_RANGE);
        }

        if (m_outputStruct.simpleDiameterMinor < m_BEAM_DIAMETER_BOT_BOUND[2*roiNumber+1] || m_outputStruct.simpleDiameterMinor > m_BEAM_DIAMETER_TOP_BOUND[2*roiNumber+1])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_DIAMETER_MINOR_OUT_OF_RANGE);
        }

        if (m_outputStruct.simpleCentroidX < m_CENTROID_BOT_BOUND[2*roiNumber] || m_outputStruct.simpleCentroidX > m_CENTROID_TOP_BOUND[2*roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_CENTROID_X_OUT_OF_RANGE);
        }

        if (m_outputStruct.simpleCentroidY < m_CENTROID_BOT_BOUND[2*roiNumber+1] || m_outputStruct.simpleCentroidY > m_CENTROID_TOP_BOUND[2*roiNumber+1])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_CENTROID_Y_OUT_OF_RANGE);
        }

        if (m_outputStruct.simpleDivergenceAngleMajor < m_DIVERGENCE_BOT_BOUND[2*roiNumber] || m_outputStruct.simpleDivergenceAngleMajor > m_DIVERGENCE_TOP_BOUND[2*roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_DIVERGENCE_MAJOR_OUT_OF_RANGE);
        }

        if (m_outputStruct.simpleDivergenceAngleMinor < m_DIVERGENCE_BOT_BOUND[2*roiNumber+1] || m_outputStruct.simpleDivergenceAngleMinor > m_DIVERGENCE_TOP_BOUND[2*roiNumber+1])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_DIVERGENCE_MINOR_OUT_OF_RANGE);
        }

        if (m_outputStruct.symmetryMetric > m_SYMMETRY_TOP_BOUND[roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_SYMMETRY_OUT_OF_RANGE);
        }

        if (m_outputStruct.pearsonCorrelation < m_PEARSON_BOT_BOUND[roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_PEARSON_OUT_OF_RANGE);
        }

        if (m_outputStruct.rawMaxIntensity < m_MAX_INTENSITY_BOT_BOUND[roiNumber] || m_outputStruct.rawMaxIntensity > m_MAX_INTENSITY_TOP_BOUND[roiNumber])
        {
            m_algoStatusWord[roiNumber] |= (1 << ALGO_STATUS_BIT_MAX_INTENSITY_OUT_OF_RANGE);
        }

    }

    m_outputStruct.algoStatusWord = static_cast<unsigned int>(m_algoStatusWord[roiNumber]);
    return &m_outputStruct;
}


#ifdef LC12
void hpImageProcessing::init(uint32_t cameraID)
{
	int cameraArraySize;
	uint32_t height, width;
    float f;
    int32_t l;
    uint32_t ul;

	ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(0),m_tempCrashFloatArray);
	cameraArraySize = m_tempCrashFloatArray.GetSize();
	for (int i=0; i<PROBE_ROI-1; ++i)
	{
		ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(i),m_tempCrashFloatArray);
		if (m_tempCrashFloatArray.GetSize() == cameraArraySize)
		{
            SetParam(i+1,m_tempCrashFloatArray[cameraID]);
        }
        else if (m_tempCrashFloatArray.GetSize() == 2*cameraArraySize)
        {
            for (int j=0; j<static_cast<int>(m_NUM_ROIS); j++)
            {
                SetArrayByIndex(i,m_tempCrashFloatArray[m_NUM_ROIS*cameraID+j],j);
            }
		}
        else if (m_tempCrashFloatArray.GetSize() == 4*cameraArraySize)
        {
            for (int j=0; j<static_cast<int>(2*m_NUM_ROIS); j++)
            {
                SetArrayByIndex(i,m_tempCrashFloatArray[m_NUM_ROIS*cameraID+j],j);
            }
        }
    }
    for (int i=PROBE_ROI-1; i<NUMBER_PARAMS-1; ++i)
    {
        if (i < PROBE_MODE-1)
        {
            ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(i),ul);
            SetParam(i,ul);
        }
        else if (i < PROBE_SIGNAL_AMPLITUDE-1)
        {
            ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(i),l);
            SetParam(i,static_cast<int>(l));
        }
        else if (i < PROBE_SIGNAL_DATA-1)
        {
            ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(i),f);
            SetParam(i,f);
        }
        else
        {
            ImageProcessingParamContainer::getInstance().GetParam(static_cast<ImageProcessingParamContainer::ParamIds>(i),m_tempCrashFloatArray);
            SetParam(i,m_tempCrashFloatArray);
        }
	}
    CameraParamContainer::getInstance().getHeight(cameraID, height);
	CameraParamContainer::getInstance().getWidth(cameraID, width);
    setSize(height, width);
    Initialize();
}
#endif

void hpImageProcessing::getPassFailCriteria(int roiNumber, passFailCriteriaStruct& passFailCriteria)
{
    passFailCriteria.centroidXBotBound = static_cast<float>(m_CENTROID_BOT_BOUND[2*roiNumber]);
    passFailCriteria.centroidXTopBound = static_cast<float>(m_CENTROID_TOP_BOUND[2*roiNumber]);
    passFailCriteria.centroidYBotBound = static_cast<float>(m_CENTROID_BOT_BOUND[2*roiNumber+1]);
    passFailCriteria.centroidYTopBound = static_cast<float>(m_CENTROID_TOP_BOUND[2*roiNumber+1]);
    passFailCriteria.diameterMajorBotBound = static_cast<float>(m_BEAM_DIAMETER_BOT_BOUND[2*roiNumber]);
    passFailCriteria.diameterMajorTopBound = static_cast<float>(m_BEAM_DIAMETER_TOP_BOUND[2*roiNumber]);
    passFailCriteria.diameterMinorBotBound = static_cast<float>(m_BEAM_DIAMETER_BOT_BOUND[2*roiNumber+1]);
    passFailCriteria.diameterMinorTopBound = static_cast<float>(m_BEAM_DIAMETER_TOP_BOUND[2*roiNumber+1]);
    passFailCriteria.divergenceAngleMajorBotBound = static_cast<float>(m_DIVERGENCE_BOT_BOUND[2*roiNumber]);
    passFailCriteria.divergenceAngleMajorTopBound = static_cast<float>(m_DIVERGENCE_TOP_BOUND[2*roiNumber]);
    passFailCriteria.divergenceAngleMinorBotBound = static_cast<float>(m_DIVERGENCE_BOT_BOUND[2*roiNumber+1]);
    passFailCriteria.divergenceAngleMinorTopBound = static_cast<float>(m_DIVERGENCE_TOP_BOUND[2*roiNumber+1]);
    passFailCriteria.gaussianPerformanceBotBound = static_cast<float>(0.0);
    passFailCriteria.gaussianPerformanceTopBound = static_cast<float>(m_GAUSS_PERF_TOP_BOUND[roiNumber]);
    passFailCriteria.symmetryBotBound = static_cast<float>(0.0);
    passFailCriteria.symmetryTopBound = static_cast<float>(m_SYMMETRY_TOP_BOUND[roiNumber]);
    passFailCriteria.pearsonCorrelationBotBound = static_cast<float>(m_PEARSON_BOT_BOUND[roiNumber]);
    passFailCriteria.pearsonCorrelationTopBound = static_cast<float>(1.0);
    passFailCriteria.maxIntensityBotBound = static_cast<float>(m_MAX_INTENSITY_BOT_BOUND[roiNumber]);
    passFailCriteria.maxIntensityTopBound = static_cast<float>(m_MAX_INTENSITY_TOP_BOUND[roiNumber]);
}

void hpImageProcessing::getImageColRoi(const matrixClass &A, int roiXOffset, int roiYOffset, int roiHeight,
            int roiWidth)
{
    for (int r=1; r<=roiHeight; r++)
    {
        for (int c=1; c<=roiWidth; c++)
        {
            m_inputImage(r+roiYOffset, c+roiXOffset) = A(r,c);
        }
    }
}

// Note: these are only used to keep the interface with LC12. use templates instead.
void hpImageProcessing::processImage(uint8_t* ptr, unsigned int height, unsigned int width)
{
    processImage<uint8_t*>(ptr, height, width);
}

void hpImageProcessing::processImage(uint16_t* ptr, unsigned int height, unsigned int width)
{
    processImage<uint16_t*>(ptr, height, width);
}

void hpImageProcessing::processImage(int16_t* ptr, unsigned int height, unsigned int width)
{
    processImage<int16_t*>(ptr, height, width);
}

void hpImageProcessing::calibrateImage(uint8_t* ptr, unsigned int height, unsigned int width)
{
    calibrateImage<uint8_t*>(ptr, height, width);
}

void hpImageProcessing::calibrateImage(uint16_t* ptr, unsigned int height, unsigned int width)
{
    calibrateImage<uint16_t*>(ptr, height, width);
}

void hpImageProcessing::calibrateImage(int16_t* ptr, unsigned int height, unsigned int width)
{
    calibrateImage<int16_t*>(ptr, height, width);
}

void hpImageProcessing::addCalibratedImageToSum(int16_t * ptr, unsigned int height, unsigned int width)
{
    addCalibratedImageToSum<int16_t*>(ptr, height, width);
}

void hpImageProcessing::addCalibratedImageToSum(uint16_t * ptr, unsigned int height, unsigned int width)
{
    addCalibratedImageToSum<uint16_t*>(ptr, height, width);
}

void hpImageProcessing::addImageToCalibrationImage(int16_t* ptr, unsigned int height, unsigned int width)
{
    addImageToCalibrationImage<int16_t*>(ptr, height, width);
}

void hpImageProcessing::addImageToCalibrationImage(uint16_t* ptr, unsigned int height, unsigned int width)
{
    addImageToCalibrationImage<uint16_t*>(ptr, height, width);
}