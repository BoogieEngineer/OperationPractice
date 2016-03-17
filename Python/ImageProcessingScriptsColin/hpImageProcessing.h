#ifndef __ALGOLIB_HP_IMAGE_PROCESSING
#define __ALGOLIB_HP_IMAGE_PROCESSING

#include <iostream>
#include <cfloat>
#include "imageProClass.h"
#include "CrashBaseObject.h"
#include "matrixClass.h"
#include "FuncGen.h"
#ifdef LC12
#include "ImageProcessingParamContainer.h"
#include "CameraParamContainer.h"
#endif

#define MAX_ROI_WIDTH			            640
#define MAX_ROI_HEIGHT                      512
#define MAX_NUM_ROIS                        4
#define MAX_MED_FILT_PADDING                40
#define MAX_SYM_RADIAL_LINES                256
#define M_PI                                3.14159265358979323846
#define M_MAX_PROBE_DATA_SIZE               1000
#define M_IMAGES_REQUIRED_FOR_CALIBRATION   64

typedef struct{
    uint32_t        NUM_ROIS;
    CrashFloatArray CONTROL_WORD;
    float           LOWPASS_SMOOTHING_FACTOR;
    uint32_t        MED_FILT_PADDING;
    CrashFloatArray THRESHOLD_CUTOFF;
    CrashFloatArray LEAST_SQUARES_CUTOFF;
    float           LEAST_SQUARES_FIT_DELTA;
    CrashFloatArray SUB_ROI_RADIUS;
    uint32_t        SYM_RADIAL_LINES;
    CrashFloatArray SYM_RADIAL_DISTANCE;
    float           COM_VALID_DISTANCE;
    uint32_t        NUM_IMAGES_TO_SUM;
    CrashFloatArray ROI_X_OFFSET;
    CrashFloatArray ROI_Y_OFFSET;
    CrashFloatArray ROI_WIDTH;
    CrashFloatArray ROI_HEIGHT;
    float           WATT_PER_COUNT;
    float           FOCAL_LENGTH;
    float           MM_PER_PIXEL;
    // CrashFloatArray POWER_DENSITY_X_OFFSET;
    // CrashFloatArray POWER_DENSITY_Y_OFFSET;
    // CrashFloatArray POWER_DENSITY_WIDTH;
    // CrashFloatArray POWER_DENSITY_HEIGHT;
    // CrashFloatArray POWER_DENSITY_MAJOR_RAD;
    // CrashFloatArray POWER_DENSITY_MINOR_RAD;
    // CrashFloatArray POWER_DENSITY_ANGLE;
    // CrashFloatArray EFFECTIVE_IRRADIATION_THRESHOLD;
    // CrashFloatArray BEAM_POSITIONAL_STABILITY_LOWPASS;
    // CrashFloatArray R_THETA_TOP_HAT_NUM_LINES;
    // CrashFloatArray R_THETA_TOP_HAT_PERCENT_LEVEL;
    CrashFloatArray ELLIPTICAL_SUB_ROI_PADDING;
    CrashFloatArray CENTROID_BOT_BOUND;
    CrashFloatArray CENTROID_TOP_BOUND;
    CrashFloatArray BEAM_DIAMETER_BOT_BOUND;
    CrashFloatArray BEAM_DIAMETER_TOP_BOUND;
    CrashFloatArray DIVERGENCE_BOT_BOUND;
    CrashFloatArray DIVERGENCE_TOP_BOUND;
    CrashFloatArray GAUSS_PERF_TOP_BOUND;
    CrashFloatArray SYMMETRY_TOP_BOUND;
    CrashFloatArray PEARSON_BOT_BOUND;
    CrashFloatArray MAX_INTENSITY_BOT_BOUND;
    CrashFloatArray MAX_INTENSITY_TOP_BOUND;
    int             PROBE_ROI;
    int             PROBE_LOCATION;
    int             PROBE_MODE;
    int             PROBE_SIGNAL_PERIOD;
    float           PROBE_SIGNAL_AMPLITUDE;
    float           PROBE_SIGNAL_OFFSET;
    CrashFloatArray PROBE_SIGNAL_DATA;

} hpImageProcessingParams;


typedef struct{
    unsigned int    algoStatusWord;
    double          borderLevel;
    double          borderMax;
    double          rawMaxIntensity;
    double          rawMaxIndX;
    double          rawMaxIndY;
    double          rawMinIntensity;
    double          rawSumIntensity;
    double          rawMeanIntensity;
    double          rawMedianIntensity;
    double          simpleCentroidX;
    double          simpleCentroidY;
    double          simpleCentroidU;
    double          simpleCentroidV;
    double          simpleAngle;
    double          simpleAmplitude;
    double          simpleVarianceMajor;
    double          simpleVarianceMinor;
    double          simpleDiameterMajor;
    double          simpleDiameterMinor;
    double          simpleEnergyTotal;
    double          simpleEllipseRatio;
    double          simpleEnergyEllipse;
    double          simpleEnergyRatio;
    double          simplePerformance;
    double          simpleCrossSectionalArea;
    double          simpleDivergenceAngleMajor;
    double          simpleDivergenceAngleMinor;
    double          simpleAzimuthAngle;
    double          symmetryMetric;
    double          lsFitCentroidX;
    double          lsFitCentroidY;
    double          lsFitAngle;
    double          lsFitAmplitude;
    double          lsFitVarianceMajor;
    double          lsFitVarianceMinor;
    double          lsFitDiameterMajor;
    double          lsFitDiameterMinor;
    double          lsFitPerformance;
    double          lsFitEllipseRatio;
    // double          bgCWPower;
    // double          bgPowerDensity;
    // double          bgBeamPositionalStabilityCentX;
    // double          bgBeamPositionalStabilityCentY;
    // double          bgBeamPositionalStabilityDiamX;
    // double          bgBeamPositionalStabilityDiamY;
    // double          bgMaximumPower;
    // double          bgEffectiveIrradiationArea;
    // double          bgEffectiveAveragePower;
    // double          bgFlatnessFactor;
    // double          bgBeamUniformity;
    // double          bgPlateauUniformity;
    // double          bgEdgeSteepness;
    // double          bgRoughnessOfFit;
    // double          bgGoodnessOfFit;
    double          pearsonCorrelation;
    double          e2DiameterMajor;
    double          e2DiameterMinor;
    double          xSectionFFTMajorFreqSum;
    double          xSectionFFTMajorFreqAverage;
    double          xSectionFFTMajorFreq;
    double          xSectionFFTMajorAmp;
    double          xSectionFFTMinorFreqSum;
    double          xSectionFFTMinorFreqAverage;
    double          xSectionFFTMinorFreq;
    double          xSectionFFTMinorAmp;
    const float*    xSectionFFTMajorFreqVec;
    const float*    xSectionFFTMajorAmpVec;
    // const float*    xSectionFFTMajorDataVec;
    const float*    xSectionFFTMinorFreqVec;
    const float*    xSectionFFTMinorAmpVec;
    // const float*    xSectionFFTMinorDataVec;
    // const float*    rThetaHatE2Data;
    // const float*    rThetaHatEData;
    // const float*    rThetaHatPercentData;
    // int             rThetaHatESize;
    // int             rThetaHatE2Size;
    // int             rThetaHatPercSize;
    int             xSectionFFTMajorSize;
    int             xSectionFFTMinorSize;
    // double          rThetaHatE2Contrast;
    // double          rThetaHatEContrast;
    // double          rThetaHatPercContrast;
    // double          rThetaHatE2PkToPk;
    // double          rThetaHatEPkToPk;
    // double          rThetaHatPercPkToPk;
    // double          rThetaHatE2Average;
    // double          rThetaHatEAverage;
    // double          rThetaHatPercAverage;
    // double          rThetaHatRadiusE;
    // double          rThetaHatRadiusE2;
    // double          rThetaHatRadiusPerc;
    uint32_t        offset_u;
    uint32_t        offset_v;
    uint32_t        roi_height;
    uint32_t        roi_width;
    uint32_t        roi_id;
    double          maxIntensityBeforeCalibration;
    double          minIntensityBeforeCalibration;
    double          fringeMetric;

} hpImageProcessingOutputSignals;

typedef struct{
    float centroidXBotBound;
    float centroidXTopBound;
    float centroidYBotBound;
    float centroidYTopBound;
    float diameterMajorBotBound;
    float diameterMajorTopBound;
    float diameterMinorBotBound;
    float diameterMinorTopBound;
    float divergenceAngleMajorBotBound;
    float divergenceAngleMajorTopBound;
    float divergenceAngleMinorBotBound;
    float divergenceAngleMinorTopBound;
    float gaussianPerformanceBotBound;
    float gaussianPerformanceTopBound;
    float symmetryBotBound;
    float symmetryTopBound;
    float pearsonCorrelationBotBound;
    float pearsonCorrelationTopBound;
    float maxIntensityBotBound;
    float maxIntensityTopBound;
} passFailCriteriaStruct;


class hpImageProcessing: public CrashBaseObject
{
    public:
        enum ParamIds
        {
            NUM_ROIS,                           /* Number of rois. */
            CONTROL_WORD,                       /* Control word in which each bit turn on/off a feature. */
            LOWPASS_SMOOTHING_FACTOR,           /* Factor which adjusts the amount of smoothing per sample in the lowpwass filter. */
            MED_FILT_PADDING,                   /* Padding on the median filter. */
            THRESHOLD_CUTOFF,                   /* Kill off the bottom X percent of intensity for simple metrics. */
            LEAST_SQUARES_CUTOFF,               /* Kill off the bottom X percent of intensity for for least squares fit. */
            LEAST_SQUARES_FIT_DELTA,            /* Initialization fo the RLS algorithm to fit Gaussian curve to data. */
            SUB_ROI_RADIUS,                     /* Radius of the circular ROI that is centered at the maximum intensity. */
            SYM_RADIAL_LINES,                   /* Number of radial lines to use for the symmetry metric. */
            SYM_RADIAL_DISTANCE,                /* Distance from centroid to consider for symmetry metric. */
            COM_VALID_DISTANCE,                 /* Minimum distance between RawMaxIntensity and SimpCOM that wll trigger COM not valid. */
            NUM_IMAGES_TO_SUM,                  /* Number of images to sum/average. */
            ROI_X_OFFSET,                       /* Array of x offsets for each roi. */
            ROI_Y_OFFSET,                       /* Array of y offsets for each roi. */
            ROI_WIDTH,                          /* Array of widths for each roi. */
            ROI_HEIGHT,                         /* Array of heights for each roi. */
            WATT_PER_COUNT,						/* */
            FOCAL_LENGTH,
            MM_PER_PIXEL,
            // POWER_DENSITY_X_OFFSET,
            // POWER_DENSITY_Y_OFFSET,
            // POWER_DENSITY_WIDTH,
            // POWER_DENSITY_HEIGHT,
            // POWER_DENSITY_MAJOR_RAD,
            // POWER_DENSITY_MINOR_RAD,
            // POWER_DENSITY_ANGLE,
            // EFFECTIVE_IRRADIATION_THRESHOLD,
            // BEAM_POSITIONAL_STABILITY_LOWPASS,
            // R_THETA_TOP_HAT_NUM_LINES,
            // R_THETA_TOP_HAT_PERCENT_LEVEL,
            ELLIPTICAL_SUB_ROI_PADDING,
            CENTROID_BOT_BOUND,
            CENTROID_TOP_BOUND,
            BEAM_DIAMETER_BOT_BOUND,
            BEAM_DIAMETER_TOP_BOUND,
            DIVERGENCE_BOT_BOUND,
            DIVERGENCE_TOP_BOUND,
            GAUSS_PERF_TOP_BOUND,
            SYMMETRY_TOP_BOUND,
            PEARSON_BOT_BOUND,
            MAX_INTENSITY_BOT_BOUND,
            MAX_INTENSITY_TOP_BOUND,
			PROBE_ROI,
            PROBE_LOCATION,
            PROBE_MODE,
            PROBE_SIGNAL_PERIOD,
            PROBE_SIGNAL_AMPLITUDE,
            PROBE_SIGNAL_OFFSET,
            PROBE_SIGNAL_DATA,
            NUMBER_PARAMS
        };

        enum ControlWordEnum 
        {
            ENABLE_IMAGE_SUBTRACTION_BIT, /* 0th */
            ENABLE_FRAME_SUMMING_BIT,
            ENABLE_FRAME_AVERAGING_BIT,
            ENABLE_MEDIAN_FILTER_BIT, 
            ENABLE_BACKGROUND_SUBTRACTION_BIT, 
            ENABLE_LOWPASS_FILTERING_BIT,
            ENABLE_CIRC_SUB_ROI_OVER_MAX_INT_BIT, 
            ENABLE_ELLIPTICAL_SUB_ROI_OVER_MAX_INT_BIT,
            ENABLE_SIMPLE_THRESHOLDING_BIT,
            ENABLE_CORRECT_FOR_THRESHOLDING_BIT, 
            ENABLE_SYMMETRY_METRICS_BIT,
            ENABLE_LS_GAUSSIAN_FIT_METRICS_BIT,
            ENABLE_MISCELLANEOUS_METRICS_BIT,
            ENABLE_RECTANGULAR_POWER_DENSITY_BIT,
            ENABLE_OUT_OF_RANGE_WARNINGS_BIT /* 13th */
        };

        enum ErrorFlagBitEnum
        {
            ALGO_STATUS_BIT_ROI_OUT_OF_BOUNDS, /* 0th */
            ALGO_STATUS_BIT_ROI_OVERLAP,
            ALGO_STATUS_BIT_CLIP_BEAM,
            ALGO_STATUS_BIT_BORDER_ABOVE_THRESHOLD,
            ALGO_STATUS_BIT_COM_NOT_VALID,
            ALGO_STATUS_BIT_GAUSSIAN_OUT_OF_RANGE,
            ALGO_STATUS_BIT_LS_NOT_CONVERGED,
            ALGO_STATUS_BIT_CENTROID_X_OUT_OF_RANGE,
            ALGO_STATUS_BIT_CENTROID_Y_OUT_OF_RANGE,
            ALGO_STATUS_BIT_DIAMETER_MAJOR_OUT_OF_RANGE,
            ALGO_STATUS_BIT_DIAMETER_MINOR_OUT_OF_RANGE,
            ALGO_STATUS_BIT_DIVERGENCE_MAJOR_OUT_OF_RANGE,
            ALGO_STATUS_BIT_DIVERGENCE_MINOR_OUT_OF_RANGE,
            ALGO_STATUS_BIT_SYMMETRY_OUT_OF_RANGE,
            ALGO_STATUS_BIT_PEARSON_OUT_OF_RANGE,
            ALGO_STATUS_BIT_MAX_INTENSITY_OUT_OF_RANGE, 
            ALGO_STATUS_BIT_CALIBRATION_IN_PROCESS, /* 16th */
        };

        enum ReturnCodes
        {
            LOWPASS_SMOOTHING_NEG = CrashBaseObject::BASE_NUMBER_RETURN_CODES,
            LOWPASS_SMOOTHING_GREATER_THAN_ONE,
            LEAST_SQUARES_FIT_DELTA_NEG_OR_ZERO,
            COM_VALID_DISTANCE_NEG,
            WATT_PER_COUNT_NEG_OR_ZERO,
            FOCAL_LENGTH_NEG_OR_ZERO,
            MM_PER_PIXEL_NEG_OR_ZERO,
            MED_FILT_PADDING_NEG,
            THRESHOLD_CUTOFF_NEG,
            THRESHOLD_CUTOFF_GREATER_THAN_ONE_HUNDRED,
            LEAST_SQUARES_CUTOFF_NEG_OR_ZERO,
            LEAST_SQUARES_CUTOFF_GREATER_THAN_ONE_HUNDRED,
            SYM_RADIAL_LINES_NEG,
            NUM_IMAGES_TO_SUM_NEG_OR_ZERO,
            NUM_ROIS_NEG_OR_ZERO,
            NUM_ROIS_GREATER_THAN_MAX_NUM_ROIS,
            CONTROL_WORD_NEG,
            CONTROL_WORD_NOT_AN_INTEGER,
			SUB_ROI_RADIUS_NEG_OR_ZERO,
			SYM_RADIAL_DISTANCE_NEG_OR_ZERO,
			ROI_X_OFFSET_NEG,
			ROI_X_OFFSET_GREATER_THAN_MAX,
			ROI_X_OFFSET_NOT_AN_INTEGER,
			ROI_Y_OFFSET_NEG,
			ROI_Y_OFFSET_GREATER_THAN_MAX,
			ROI_Y_OFFSET_NOT_AN_INTEGER,
			ROI_WIDTH_NEG_OR_ZERO,
			ROI_WIDTH_GREATER_THAN_MAX,
			ROI_WIDTH_NOT_AN_INTEGER,
			ROI_HEIGHT_NEG_OR_ZERO,
			ROI_HEIGHT_GREATER_THAN_MAX,
			ROI_HEIGHT_NOT_AN_INTEGER,
            // POWER_DENSITY_X_OFFSET_NEG,
            // POWER_DENSITY_X_OFFSET_GREATER_THAN_MAX,
            // POWER_DENSITY_X_OFFSET_NOT_AN_INTEGER,
            // POWER_DENSITY_Y_OFFSET_NEG,
            // POWER_DENSITY_Y_OFFSET_GREATER_THAN_MAX,
            // POWER_DENSITY_Y_OFFSET_NOT_AN_INTEGER,
            // POWER_DENSITY_WIDTH_NEG_OR_ZERO,
            // POWER_DENSITY_WIDTH_GREATER_THAN_MAX,
            // POWER_DENSITY_WIDTH_NOT_AN_INTEGER,
            // POWER_DENSITY_HEIGHT_NEG_OR_ZERO,
            // POWER_DENSITY_HEIGHT_GREATER_THAN_MAX,
            // POWER_DENSITY_HEIGHT_NOT_AN_INTEGER,
            // POWER_DENSITY_MAJOR_RAD_NEG_OR_ZERO,
            // POWER_DENSITY_MINOR_RAD_NEG_OR_ZERO,
            // EFFECTIVE_IRRADIATION_THRESHOLD_NEG_OR_ZERO,
            // EFFECTIVE_IRRADIATION_THRESHOLD_GREATER_THAN_ONE_HUNDRED,
            // BEAM_POSITIONAL_STABILITY_LOWPASS_NEG,
            // BEAM_POSITIONAL_STABILITY_LOWPASS_GREATER_THAN_ONE,
            // R_THETA_TOP_HAT_NUM_LINES_NEG_OR_ZERO,
            // R_THETA_TOP_HAT_NUM_LINES_NOT_AN_INTEGER,
            // R_THETA_TOP_HAT_PERCENT_LEVEL_NEG_OR_ZERO,
            // R_THETA_TOP_HAT_PERCENT_LEVEL_GREATER_THAN_ONE_HUNDRED,
            ELLIPTICAL_SUB_ROI_PADDING_NEG_OR_ZERO,
            PROBE_MODE_NEG,
            PROBE_SIGNAL_PERIOD_NEG,
            NUMBER_RETURN_CODES
        };

        enum ProbeLocationsEnum
        {
            PROBE_INACTIVE,
            OUTPUT_CENTROID_X,
            OUTPUT_CENTROID_Y,
            OUTPUT_DIAMETER_MAJOR,
            OUTPUT_DIAMETER_MINOR,
            OUTPUT_DIVERGENCE_MAJOR,
            OUTPUT_DIVERGENCE_MINOR,
            OUTPUT_GAUSSIAN,
            OUTPUT_SYMMETRY,
            OUTPUT_PEARSON,
            OUTPUT_MAX_INTENSITY,
        };

        hpImageProcessing();
        hpImageProcessing(unsigned int maxImageHeight, unsigned int maxImageWidth);
        void    setSize(unsigned int maxImageHeight, unsigned int maxImageWidth);
        virtual ~hpImageProcessing();
        void    construct();

        virtual CrashResult Initialize(void);
        virtual CrashResult SetParam(long ParamID, float value);
        virtual CrashResult SetParam(long paramID, uint32_t value);
        virtual CrashResult SetParam(long paramID, int value);
        virtual CrashResult SetParam(long paramID, float * rValue, int numElements);
        virtual CrashResult SetParam(long paramID, CrashFloatArray &rValue);
        virtual CrashResult SetArrayByIndex(long paramID, float value, int index);
        CrashResult SetParamStruct(hpImageProcessingParams& paramStruct);

#ifdef LC12
		void init(uint32_t cameraID);
#endif

        void        runIpAlgo();
        bool        detectRoiOverlap();
        
        // APIs for software
        hpImageProcessingOutputSignals* getOutputSignals(int roiNumber);
        void                            setInputImage2ImageAverage(void);
        void                            setInputImage2ImageSum(void);
        uint16_t*                       getCalibratedImage(void);
        void                            clearCalibrationImage(void);
        void                            averageCalibrationImage(void);
		int16_t*						getCalibrationImage(void);
        void                            startCalibration(void);

        // Note: these are only used to keep the interface with LC12. use templates instead.
        void                            processImage(uint8_t* imageBuffer, unsigned int height, unsigned int width);
        void                            processImage(uint16_t* imageBuffer, unsigned int height, unsigned int width);
        void                            processImage(int16_t* imageBuffer, unsigned int height, unsigned int width);
        void                            calibrateImage(uint8_t* ptr, unsigned int height, unsigned int width);
        void                            calibrateImage(uint16_t* ptr, unsigned int height, unsigned int width);
        void                            calibrateImage(int16_t* ptr, unsigned int height, unsigned int width);
        void                            addCalibratedImageToSum(int16_t * ptr, unsigned int height, unsigned int width);
        void                            addCalibratedImageToSum(uint16_t * ptr, unsigned int height, unsigned int width);
        void                            addImageToCalibrationImage(int16_t* imageBuffer, unsigned int height, unsigned int width);
        void                            addImageToCalibrationImage(uint16_t* imageBuffer, unsigned int height, unsigned int width);
        void                            getPassFailCriteria(int roiNumber, passFailCriteriaStruct& passFailCriteria);

        // Templates
        template <class type>
        void processImage(type ptr, unsigned int height, unsigned width)
        {
            m_inputImageHeight = height;
            m_inputImageWidth = width;
            if (m_calibrationCompleted) 
            {
                if (detectRoiOverlap())
                {

                }
                else
                {
                    calibrateImage<type>(ptr, height, width);
                    runIpAlgo();
                }
            }
            else if (m_numberImagesInCalibration < M_IMAGES_REQUIRED_FOR_CALIBRATION)
            {
                addImageToCalibrationImage<type>(ptr, height, width);
				for (unsigned int i=0; i<m_NUM_ROIS; i++)
				{
                    m_algoStatusWord[i] = 0x0;
					m_algoStatusWord[i] |= (1 << ALGO_STATUS_BIT_CALIBRATION_IN_PROCESS);
				}
            }
            else
            {
                averageCalibrationImage();
                m_calibrationCompleted = true;
            }

        };

        template <class type>
        void addImageToCalibrationImage(type ptr, unsigned int height, unsigned int width)
        {
            unsigned long ind = 0;
            for (unsigned int r=1; r<=height; r++)
            {
                for (unsigned int c=1; c<=width; c++)
                {
                    if (ind<m_maxImageHeight*m_maxImageWidth)
                    {
                        m_CALIBRATION_IMAGE[ind] += static_cast<float>(ptr[ind]);
                    }
                    ind++;
                }
            }
            m_numberImagesInCalibration += 1;
        };

        template <class type>
        void addCalibratedImageToSum(type ptr, unsigned int height, unsigned int width)
        {
            long ind = 0;
            m_inputImageHeight = height;
            m_inputImageWidth = width;

            m_inputImageMinVal = DBL_MAX;
            m_inputImageMaxVal = -DBL_MAX;

            if ( static_cast<int>(m_CONTROL_WORD[0]) & (1 << ENABLE_IMAGE_SUBTRACTION_BIT))
            {
                for (unsigned int r=1; r<=height; r++)
                {
                    for (unsigned int c=1; c<=width; c++)
                    {
                        if (static_cast<double>(ptr[ind]) > m_inputImageMaxVal)
                        {
                            m_inputImageMaxVal = static_cast<double>(ptr[ind]);
                        }
                        else if (static_cast<double>(ptr[ind]) < m_inputImageMinVal)
                        {
                            m_inputImageMinVal = static_cast<double>(ptr[ind]);
                        }

                        m_imageSum(r,c) += static_cast<double>(ptr[ind]) - static_cast<double>(m_CALIBRATION_IMAGE[ind]);
                        ind++;
                    }
                }
            }
            else
            {
                for (unsigned int r=1; r<=height; r++)
                {
                    for (unsigned int c=1; c<=width; c++)
                    {
                        if (static_cast<double>(ptr[ind]) > m_inputImageMaxVal)
                        {
                            m_inputImageMaxVal = static_cast<double>(ptr[ind]);
                        }
                        else if (static_cast<double>(ptr[ind]) < m_inputImageMinVal)
                        {
                            m_inputImageMinVal = static_cast<double>(ptr[ind]);
                        }

                        m_imageSum(r,c) += static_cast<double>(ptr[ind]);
                        ind++;
                    }
                }
            }
            m_numberImagesSummed++;
        };

        template <class type>
        void calibrateImage(type ptr, unsigned int height, unsigned int width)
        {
            long ind = 0;
            m_inputImageHeight = height;
            m_inputImageWidth = width;

            m_inputImageMinVal = DBL_MAX;
            m_inputImageMaxVal = -DBL_MAX;

            if (( static_cast<int>(m_CONTROL_WORD[0]) & (1 << ENABLE_FRAME_SUMMING_BIT))
                | ( static_cast<int>(m_CONTROL_WORD[0]) & (1 << ENABLE_FRAME_AVERAGING_BIT)))
            {
                addCalibratedImageToSum<type>(ptr,height,width);
                if ( m_numberImagesSummed < m_NUM_IMAGES_TO_SUM)
                {
                    m_finishedSumming = false;
                }
                else
                {
                    if ( static_cast<int>(m_CONTROL_WORD[0]) & (1 << ENABLE_FRAME_AVERAGING_BIT))
                    {
                        setInputImage2ImageAverage();
                    }
                    else
                    {
                        setInputImage2ImageSum();
                    }
                    m_finishedSumming = true;
                }
            }
            else
            {
                m_finishedSumming = true;
                if ( static_cast<int>(m_CONTROL_WORD[0]) & (1 << ENABLE_IMAGE_SUBTRACTION_BIT))
                {
                    for (unsigned int r=1; r<=height; r++)
                    {
                        for (unsigned int c=1; c<=width; c++)
                        {
                            if (static_cast<double>(ptr[ind]) > m_inputImageMaxVal)
                            {
                                m_inputImageMaxVal = static_cast<double>(ptr[ind]);
                            }
                            else if (static_cast<double>(ptr[ind]) < m_inputImageMinVal)
                            {
                                m_inputImageMinVal = static_cast<double>(ptr[ind]);
                            }

                            m_inputImage(r,c) = static_cast<double>(ptr[ind]) - static_cast<double>(m_CALIBRATION_IMAGE[ind]);
                            ind++;
                        }
                    }
                }
                else
                {
                    for (unsigned int r=1; r<=height; r++)
                    {
                        for (unsigned int c=1; c<=width; c++)
                        {
                            if (static_cast<double>(ptr[ind]) > m_inputImageMaxVal)
                            {
                                m_inputImageMaxVal = static_cast<double>(ptr[ind]);
                            }
                            else if (static_cast<double>(ptr[ind]) < m_inputImageMinVal)
                            {
                                m_inputImageMinVal = static_cast<double>(ptr[ind]);
                            }

                            m_inputImage(r,c) = static_cast<double>(ptr[ind]);
                            ind++;
                        }
                    }
                }
            }
        };

    protected:
        bool            m_initialized;

        // parameters
        uint32_t        m_NUM_ROIS;
        CrashFloatArray m_CONTROL_WORD;
        float           m_LOWPASS_SMOOTHING_FACTOR;
        uint32_t        m_MED_FILT_PADDING;
        CrashFloatArray m_THRESHOLD_CUTOFF;
        CrashFloatArray m_LEAST_SQUARES_CUTOFF;
        float           m_LEAST_SQUARES_FIT_DELTA;
        CrashFloatArray m_SUB_ROI_RADIUS;
        uint32_t        m_SYM_RADIAL_LINES;
        CrashFloatArray m_SYM_RADIAL_DISTANCE;
        float           m_COM_VALID_DISTANCE;
        uint32_t        m_NUM_IMAGES_TO_SUM;
        CrashFloatArray m_ROI_X_OFFSET;
        CrashFloatArray m_ROI_Y_OFFSET;
        CrashFloatArray m_ROI_WIDTH;
        CrashFloatArray m_ROI_HEIGHT;
        float           m_WATT_PER_COUNT;
        float           m_FOCAL_LENGTH;
        float           m_MM_PER_PIXEL;
        // CrashFloatArray m_POWER_DENSITY_X_OFFSET;
        // CrashFloatArray m_POWER_DENSITY_Y_OFFSET;
        // CrashFloatArray m_POWER_DENSITY_WIDTH;
        // CrashFloatArray m_POWER_DENSITY_HEIGHT;
        // CrashFloatArray m_POWER_DENSITY_MAJOR_RAD;
        // CrashFloatArray m_POWER_DENSITY_MINOR_RAD;
        // CrashFloatArray m_POWER_DENSITY_ANGLE;
        // CrashFloatArray m_EFFECTIVE_IRRADIATION_THRESHOLD;
        // CrashFloatArray m_BEAM_POSITIONAL_STABILITY_LOWPASS;
        // CrashFloatArray m_R_THETA_TOP_HAT_NUM_LINES;
        // CrashFloatArray m_R_THETA_TOP_HAT_PERCENT_LEVEL;
        CrashFloatArray m_ELLIPTICAL_SUB_ROI_PADDING;
        CrashFloatArray m_BEAM_DIAMETER_TOP_BOUND;
        CrashFloatArray m_BEAM_DIAMETER_BOT_BOUND;
        CrashFloatArray m_CENTROID_TOP_BOUND;
        CrashFloatArray m_CENTROID_BOT_BOUND;
        CrashFloatArray m_GAUSS_PERF_TOP_BOUND;
        CrashFloatArray m_SYMMETRY_TOP_BOUND;
        CrashFloatArray m_PEARSON_BOT_BOUND;
        CrashFloatArray m_DIVERGENCE_BOT_BOUND;
        CrashFloatArray m_DIVERGENCE_TOP_BOUND;
        CrashFloatArray m_MAX_INTENSITY_BOT_BOUND;
        CrashFloatArray m_MAX_INTENSITY_TOP_BOUND;
        int32_t         m_PROBE_ROI;
        int32_t         m_PROBE_LOCATION;
        int32_t         m_PROBE_MODE;
        int32_t         m_PROBE_SIGNAL_PERIOD;
        float           m_PROBE_SIGNAL_AMPLITUDE;
        float           m_PROBE_SIGNAL_OFFSET;
        CrashFloatArray m_PROBE_SIGNAL_DATA;

        FuncGen m_probe;

        CrashFloatArray m_CALIBRATION_IMAGE;



    private:
        unsigned int                    m_maxImageWidth;
        unsigned int                    m_maxImageHeight;
        unsigned int                    m_algoStatusWord[MAX_NUM_ROIS];
        unsigned int                    m_numberImagesInCalibration;
        imageProClass                   m_imageProObj[MAX_NUM_ROIS];
        bool                            m_finishedSumming;
        unsigned int                    m_inputImageHeight;
        unsigned int                    m_inputImageWidth;
        matrixClass                     m_inputImage;
        matrixClass                     m_imageSum;
        unsigned int                    m_numberImagesSummed;
        uint16_t*                       m_calibratedImage;
		int16_t*                        m_calibrationImage;
		CrashFloatArray		            m_tempCrashFloatArray;
        hpImageProcessingOutputSignals  m_outputStruct;
        bool                            m_calibrationCompleted;
        double                          m_inputImageMaxVal;
        double                          m_inputImageMinVal;

		hpImageProcessing(const hpImageProcessing&);
		hpImageProcessing& operator=(const hpImageProcessing&);

        void getImageColRoi(const matrixClass &A, int roiXOffset, int roiYOffset, int roiHeight, int roiWidth);
};

#endif