
/******************************************************************************
*
*   File Name:  
*
*   Description:  
*                
*
******************************************************************************/
#ifndef __ALGOLIB_IMAGE_PRO_CLASS
#define __ALGOLIB_IMAGE_PRO_CLASS

#include "CrashBaseObject.h"
#include "matrixClass.h"
#include <math.h>
#include <iostream>
#define M_PI							3.14159265358979323846
#define M_VARIANCE_CUT					0.005
#define M_KURTOSIS_CUT					0.005
#define M_VARIANCE_PARAM1				-43676.530452
#define M_VARIANCE_PARAM2				56.971350
#define M_VARIANCE_PARAM3				12734.782869
#define M_VARIANCE_PARAM4				-15.418146
#define M_VARIANCE_PARAM5				-1120.015988
#define M_VARIANCE_PARAM6				4.340196
#define M_VARIANCE_PARAM7				27.406184
#define M_VARIANCE_PARAM8				1.002506
#define M_VARIANCE_PARAM9				-0.267149
#define M_KURTOSIS_PARAM1				-220.063048
#define M_KURTOSIS_PARAM2				135.793530
#define M_KURTOSIS_PARAM3				-31.573821
#define M_KURTOSIS_PARAM4				3.997878
#define M_KURTOSIS_PARAM5				1.034832
#define M_IMAGE_DEPTH					65536
#define M_MAX_ITERATIONS				5000
#define M_CONVOLUTION_MATRIX_MAX_WIDTH	5
#define M_CONVOLUTION_MATRIX_MAX_HEIGHT 5
#define M_E2_MAX_RADIUS					600
#define M_ONE_OVER_E2					0.135335283236613
#define M_ONE_OVER_E					0.367879441171442
#define M_MAX_FFT_LENGTH				2048
#define M_NUM_SAMPLES_FFT_LS_FIT 		3
#define M_NUM_MAX_FREQUENCIES_FFT 		3

typedef double imageProClassParamDataType;

typedef struct{
	CrashFloatArray 			dataVector;
	CrashFloatArray 			realVector;
	CrashFloatArray 			imagVector;
	CrashFloatArray 			freqVector;
	CrashFloatArray 			ampVector;
	imageProClassParamDataType	maxAmp;
	imageProClassParamDataType 	freq0;
	imageProClassParamDataType 	amp0;
	int 						numberUniqueFreq;
	int 						numberDataPoints;
	imageProClassParamDataType 	freqErrorSum;
	imageProClassParamDataType 	freqErrorAverage;
} fftStruct;

class imageProClass: public CrashBaseObject
{

	public: 
		// contructors and destructor
		imageProClass(void);
		imageProClass(int numRows, int numCols);
		imageProClass(int numRows, int numCols, int numGauss);
		virtual ~imageProClass(void);
		void 	construct(void);

		// override crash base methods
		virtual CrashResult Initialize(void);
		// virtual CrashResult SetParam(long paramID, imageProClassParamDataType value);

		// application specific
		void setSize(int numRows, int numCols);

		// data location vectors and maps
		void setDataCounter( int n){m_dataCounter=n;};
		int getDataCounter(void){return m_dataCounter;};
		void setDataLoc(matrixClass& in, int num){m_dataLoc=in; m_dataCounter=num;};
		matrixClass& getDataLoc(void){return m_dataLoc;};
		void clrDataLocMap(void){m_dataLocMap.zeros();};
		void setDataLocMap(matrixClass& in){m_dataLocMap=in;};
		void onesDataLocMap(void){m_dataLocMap.ones();};

		// set & get data/image 
		void setImage(const matrixClass& A)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = A(r,c);
				}
			}
		};

		template <class ptrType>
		void setImageColRoi(ptrType ptr, int imageHeight, int imageWidth, int roiXOffset, 
					int roiYOffset, int roiHeight, int roiWidth)
		{
			m_numRows = roiHeight;
			m_numCols = roiWidth;
			int ind=0;
			ind += roiYOffset;
			for (int r=1; r<=roiHeight; r++)
			{
				ind += roiXOffset;
				for (int c=1; c<=roiWidth; c++)
				{
					m_image(r,c) = (imageProClassParamDataType) ptr[ind];
					ind++;
				}	
				ind += imageWidth-(roiXOffset+roiWidth);
			}
		};

		// Legacy API: Should be deleted. Use template interface instead.
		void setImageColRoi(uint8_t * ptr, int imageHeight, int imageWidth, int roiXOffset, 
					int roiYOffset, int roiHeight, int roiWidth)
		{
			setImageColRoi<uint8_t *>(ptr, imageHeight, imageWidth, roiXOffset, roiYOffset, roiHeight, roiWidth);
		};

		void setImageColRoi(uint16_t * ptr, int imageHeight, int imageWidth, int roiXOffset, 
					int roiYOffset, int roiHeight, int roiWidth)
		{
			setImageColRoi<uint16_t *>(ptr, imageHeight, imageWidth, roiXOffset, roiYOffset, roiHeight, roiWidth);
		};

		void setImageColRoi(const matrixClass &A, int roiXOffset, int roiYOffset, int roiHeight,
					int roiWidth)
		{
			m_numRows = roiHeight;
			m_numCols = roiWidth;
			for (int r=1; r<=roiHeight; r++)
			{
				for (int c=1; c<=roiWidth; c++)
				{
					m_image(r,c) = A(r+roiYOffset, c+roiXOffset);
				}
			}
		};

		void setImageCol(uint8_t * ptr, int height, int width)
		{
			setImageColRoi<uint8_t *>(ptr, height, width, 0, 0, height, width);
		};

		void setImageCol(uint16_t * ptr, int height, int width)
		{
			setImageColRoi<uint16_t *>(ptr, height, width, 0, 0, height, width);
		};
		void setImageCol(imageProClassParamDataType * ptr, int height, int width)
		{
			setImageColRoi<imageProClassParamDataType *>(ptr, height, width, 0, 0, height, width);
		};

		template <class ptrType>
		void setImageRow(ptrType ptr, int height, int width)
		{
			m_numRows = height;
			m_numCols = width;

			int ind=0;
			for (int c=1; c<=width; c++)
			{
				for (int r=1; r<=height; r++)
				{
					m_image(r,c) = (imageProClassParamDataType) ptr[ind];
					ind++;
				}	
			}

		}
		void setImageRow(uint8_t * ptr, int height, int width)
		{
			setImageRow<uint8_t *>(ptr, height, width);
		};

		void setImageRow(uint16_t * ptr, int height, int width)
		{
			setImageRow<uint16_t *>(ptr, height, width);
		};

		void setImageRow(imageProClassParamDataType * ptr, int height, int width)
		{
			setImageRow<imageProClassParamDataType *>(ptr, height, width);
		};

		void setData(int r, int c, uint8_t data){m_image(r,c) = data;};
		void setData(int r, int c, uint16_t data){m_image(r,c) = data;};
		void setData(int r, int c, imageProClassParamDataType data){m_image(r,c) = data;};
		imageProClassParamDataType getData(int r, int c){return m_image(r,c);};
		matrixClass& getImage(void){return m_image;};
		int getRows(void){return m_numRows;};
		int getCols(void){return m_numCols;};
		
		// image thresholding, cutting, setting ROI, etc
		void thresImage( imageProClassParamDataType level );									// slices the image into 0s and 1s
		void cutImage( imageProClassParamDataType level );		
		void offsetImage(imageProClassParamDataType level){m_image-=level; };				// removes an offset from image
		void offsetThresImage(imageProClassParamDataType level){m_imageResult-=level; };		// removes an offset from image
		void remNeg_thresImage(void);
		void remNeg_image(void);
		void setCircROI(imageProClassParamDataType R, imageProClassParamDataType C, imageProClassParamDataType rad);					// creates a pixelated circle around point (U,V) with radius rad
		void sumBorderAndStoreInArray(void);
		void removeBorder(imageProClassParamDataType level);
		void removeBorderMean(void){removeBorder(1.0);};
		void removeBorderMedian(void);
		imageProClassParamDataType getBorderMax(void){return m_borderMax;};

		template<class ptrType>
		void setBackgroundImage(ptrType ptr, int height, int width)
		{
			int ind=0;
			for (int r=1; r<=height; r++)
			{
				for (int c=1; c<=width; c++)
				{
					m_backgroundImage(r,c) = ptr[ind];
					ind++;
				}	
			}
		};

		void setBackgroundImage(imageProClassParamDataType * ptr, int height, int width)
		{
			setBackgroundImage<imageProClassParamDataType *>(ptr, height, width);
		};
		void setBackgroundImage(uint8_t * ptr, int height, int width)
		{
			setBackgroundImage<uint8_t *>(ptr, height, width);
		};
		void setBackgroundImage(uint16_t * ptr, int height, int width)
		{
			setBackgroundImage<uint16_t *>(ptr, height, width);
		};
		void setBackgroundImage(CrashFloatArray &rValue, int height, int width)
		{
			setBackgroundImage<CrashFloatArray &>(rValue, height, width);
		};

		void subtractBackgroundImage(void);
		void initializeLowpassStates(void)
		{
			m_lowpassInitialized = true;
			m_lowpassStates = m_image;
		};
		void lowpassFilterOnImage(imageProClassParamDataType beta);
		void setImage2LowpassStates(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_lowpassStates(r,c);
				}
			}
		};
		bool getLowpassInitialized(void){return m_lowpassInitialized;};

		void addImageToSumRow(imageProClassParamDataType * newImage, int imageHeight, int imageWidth, int roiXOffset, 
					int roiYOffset, int roiHeight, int roiWidth)
		{
			m_numRows = roiHeight;
			m_numCols = roiWidth;
			int ind=0;
			ind += roiXOffset;
			for (int c=1; c<=roiWidth; c++)
			{
				ind += roiXOffset;
				for (int r=1; r<=roiHeight; r++)
				{
					m_imageSum(r,c) += (imageProClassParamDataType) newImage[ind];
					ind++;
				}	
				ind += imageHeight-(roiYOffset+roiHeight);
			}
		};

		void addImageToSumCol(imageProClassParamDataType * newImage, int imageHeight, int imageWidth, int roiXOffset, 
					int roiYOffset, int roiHeight, int roiWidth)
		{
			m_numRows = roiHeight;
			m_numCols = roiWidth;
			int ind=0;
			ind += roiYOffset;
			for (int r=1; r<=roiHeight; r++)
			{
				ind += roiXOffset;
				for (int c=1; c<=roiWidth; c++)
				{
					m_imageSum(r,c) += (imageProClassParamDataType) newImage[ind];
					ind++;
				}	
				ind += imageWidth-(roiXOffset+roiWidth);
			}
		};
		
		void addImageToSum(matrixClass &newImage, int roiXOffset, int roiYOffset, int height, int width)
		{
			m_numRows = height;
			m_numCols = width;
			for (int r=1; r<=height; r++)
			{
				for (int c=1; c<=width; c++)
				{
					m_imageSum(r,c) += newImage(r+roiYOffset, c+roiXOffset);
				}
			}
			m_numberImagesSummed += 1;
		};

		void setImage2ImageSum(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_imageSum(r,c);
				}
			}
			m_numberImagesSummed = 0;
			m_imageSum.zeros();
		};
		
		void setImage2ImageAverage(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_imageSum(r,c)/m_numberImagesSummed;
				}
			}
			m_numberImagesSummed = 0;
			m_imageSum.zeros();
		};

		int getNumberImagesSummed(void){return m_numberImagesSummed;};
		// set imageResult to image and vice versa
		void setImage2ImageResult(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_imageResult(r,c);
				}
			}
		};			// sets image to thresholded image
		void setImageResult2Image(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_imageResult(r,c) = m_image(r,c);
				}
			}
		};			// sets thresholded image to image
		
		// filtering
		void medianFilter(int padSize);									// median filter with mask size 2*padSize+1
		void smoothHistVert(int numSmooth);
		void smoothHistHorz(int numSmooth);
		void smoothHistVert(CrashFloatArray &smoother);
		void smoothHistHorz(CrashFloatArray &smoother);
		void setConvolutionMatrix(matrixClass &inputMatrix, int nrows, int ncols)
		{
			if (nrows <= M_CONVOLUTION_MATRIX_MAX_HEIGHT && ncols <= M_CONVOLUTION_MATRIX_MAX_WIDTH)
			{
				for (int r=1; r<=nrows; r++)
				{
					for (int c=1; c<=ncols; c++)
					{
						m_convolutionMask(r,c) = inputMatrix(r,c);
					}
				}
			}
		};
		void setConvolutionMatrix(CrashFloatArray &inputCrashFloatArray, int nrows, int ncols)
		{
			int ind = 0;
			if (nrows <= M_CONVOLUTION_MATRIX_MAX_HEIGHT && ncols <= M_CONVOLUTION_MATRIX_MAX_WIDTH)
			{
				m_convolutionMaskHeight = nrows;
				m_convolutionMaskWidth = ncols;
				for (int r=1; r<=nrows; r++)
				{
					for (int c=1; c<=ncols; c++)
					{
						m_convolutionMask(r,c) = inputCrashFloatArray[ind];
					}
				}
			}
		};
		void setConvolutionMatrix(imageProClassParamDataType * inputDoubleArray, int nrows, int ncols)
		{
			int ind = 0;
			if (nrows <= M_CONVOLUTION_MATRIX_MAX_HEIGHT && ncols <= M_CONVOLUTION_MATRIX_MAX_WIDTH)
			{
				m_convolutionMaskHeight = nrows;
				m_convolutionMaskWidth = ncols;
				for (int r=1; r<=nrows; r++)
				{
					for (int c=1; c<=ncols; c++)
					{
						m_convolutionMask(r,c) = inputDoubleArray[ind];
						ind++;
					}
				}
			}
		};
		void convolutionFilter(void);
		void setImage2ConvolutedImage(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_convolutedImage(r,c);
				}
			}
		};
		void setImage2OriginalImage(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_image(r,c) = m_originalImage(r,c);
				}
			}
		};
		void setOriginalImage2Image(void)
		{
			for (int r=1; r<=m_numRows; r++)
			{
				for (int c=1; c<=m_numCols; c++)
				{
					m_originalImage(r,c) = m_image(r,c);
				}
			}
		};

		void setEllipticalRoi(imageProClassParamDataType padding);


		// moments and simple metrics
		void findMoments(void);
		void leastSquaresLogFit(imageProClassParamDataType lsFitDelta);							
		imageProClassParamDataType getLsFitRes(int r){return m_theta(r,1);};
		void calcLSfitMetrics(void);
		void leastSquares1D(int offset, fftStruct &inputStruct);
		void calcleastSquares1D(void);
		void calcSimpMetrics(void);

		void correctForThresholding(imageProClassParamDataType level);
		imageProClassParamDataType getLsCOV(int r, int c){return	m_lsCOV(r,c);};
		imageProClassParamDataType getLsCOM(int r){return			m_lsCOM(r,1);};
		imageProClassParamDataType getLsVxx(void){return			m_lsVxx;};
		imageProClassParamDataType getLsVyy(void){return			m_lsVyy;};
		imageProClassParamDataType getLsBeamDiam_x(void){return		m_lsBeamDiam_x;};
		imageProClassParamDataType getLsBeamDiam_y(void){return		m_lsBeamDiam_y;};
		imageProClassParamDataType getLsEllip(void){return			m_ellip;};
		imageProClassParamDataType getLsAngle(void){return			m_angle;};
		imageProClassParamDataType getLsMaxIntens(void){return		m_MaxIntens;};
		imageProClassParamDataType getLsPerfMetric(void){return m_lsPerfMetric;};

		imageProClassParamDataType getSimpCOV(int r, int c)
		{
			if(r==1 && c==1)
			{
				return m_M2x/m_Vol - m_CMx*m_CMx;
			}
			else if (r==2 && c==2)
			{
				return m_M2y/m_Vol - m_CMy*m_CMy;
			}
			else
				return m_M1xy/m_Vol - m_CMx*m_CMy;
		};
		imageProClassParamDataType getSimpCOM(int r)
		{
			if(r==1)
				return  m_CMx;
			else
				return  (m_numRows-1-m_CMy);
		
		};
		imageProClassParamDataType getSimpCOMUV(int r)
		{
			if(r==1)
				return  m_CMx;
			else
				return  m_CMy;
		
		};
		imageProClassParamDataType getSimpVxx(void){return				m_SimpVxx;};
		imageProClassParamDataType getSimpVyy(void){return				m_SimpVyy;};
		imageProClassParamDataType getSimpBeamDiamMajor(void){return		m_SimpBeamDiam_x;};
		imageProClassParamDataType getSimpBeamDiamMinor(void){return		m_SimpBeamDiam_y;};
		imageProClassParamDataType getSimpEllip(void){return			m_SimpEllip;};
		imageProClassParamDataType getSimpAngle(void){return			-m_SimpAngle;};
		imageProClassParamDataType getSimpMaxIntens(void){return		m_SimpMaxIntens;};
		imageProClassParamDataType getSimpEnergyInEllipse(void){return  m_SimpEnergyInEllipse;};
		imageProClassParamDataType getSimpEnergyRatio(void){return		m_SimpEnergyRatio;};
		imageProClassParamDataType getSimpEnergyTotal(void){return		m_Vol;};
		imageProClassParamDataType getSimpPerfMetric(void){return		m_SimpPerfMetric;};
		void calcSimpPerfMetric(void);
		void calcCentroidsOnly(void);
		imageProClassParamDataType getSkewX(void){return				m_skewX;};
		imageProClassParamDataType getSkewY(void){return				m_skewY;};
		void calcEffectiveMetrics(imageProClassParamDataType threshold, imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel);
		void calcDivergenceAngle(imageProClassParamDataType focalLength);
		void calcCWPower(imageProClassParamDataType wattPerCount);
		void calcPowerDensity(int xOffset, int yOffset, int width, int height, imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel);
		void calcPowerDensityEllipse(int xCenter, int yCenter, imageProClassParamDataType radA,
				imageProClassParamDataType radB, imageProClassParamDataType angle, imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel);
		imageProClassParamDataType calcEnergyInEllipse(int xCenter, int yCenter, 
				imageProClassParamDataType radA, imageProClassParamDataType radB, imageProClassParamDataType angle);
		void calcMaximumPower(imageProClassParamDataType wattPerCount);
		void calcGoodnessOfFitMetric(void);
		void initBeamPositionalStability(void);
		void calcBeamPositionalStability(imageProClassParamDataType beta);
		void calcE2Width(void);
		void runFFT(fftStruct &inputStruct);
		void calcXSectionFFT(void);
		void calcRThetaTopHat(int numLines, imageProClassParamDataType percent);
		void calcContourMetrics(int numLines, imageProClassParamDataType percent);
		void calcPearsonCorrelation(void);
		imageProClassParamDataType getSimpCrossSectionalArea(void){return m_SimpCrossSectionalArea;};
		imageProClassParamDataType getSimpDivergenceAngle(int ind)
		{
			if (ind==0)
			{
				return m_SimpDivergenceAngleX;
			}
			else
			{
				return m_SimpDivergenceAngleY;
			}
		};
		imageProClassParamDataType getSimpCWPower(void){return m_SimpCWPower;}
		imageProClassParamDataType getSimpPowerDensity(void){return m_SimpPowerDensity;};
		imageProClassParamDataType getSimpAzimuthAngle(void){return m_SimpAzimuthAngle;};
		imageProClassParamDataType getSimpBeamPositionalStabilityCentX(void){return m_SimpBeamPositionalStabilityCentX;};
		imageProClassParamDataType getSimpBeamPositionalStabilityCentY(void){return m_SimpBeamPositionalStabilityCentY;};
		imageProClassParamDataType getSimpBeamPositionalStabilityDiamX(void){return m_SimpBeamPositionalStabilityDiamX;};
		imageProClassParamDataType getSimpBeamPositionalStabilityDiamY(void){return m_SimpBeamPositionalStabilityDiamY;};
		imageProClassParamDataType getSimpMaximumPower(void){return m_SimpMaximumPower;};
		imageProClassParamDataType getSimpEffectiveIrradiationArea(void){return m_SimpEffectiveIrradiationArea;};
		imageProClassParamDataType getSimpEffectiveAveragePower(void){return m_SimpEffectiveAveragePower;};
		imageProClassParamDataType getSimpFlatnessFactor(void){return m_SimpFlatnessFactor;};
		imageProClassParamDataType getSimpBeamUniformity(void){return m_SimpBeamUniformity;};
		imageProClassParamDataType getSimpPlateauUniformity(void){return m_SimpPlateauUniformity;};
		imageProClassParamDataType getSimpEdgeSteepness(void){return m_SimpEdgeSteepness;};
		imageProClassParamDataType getSimpRoughnessOfFit(void){return m_SimpRoughnessOfFit;};
		imageProClassParamDataType getSimpGoodnessOfFit(void){return m_SimpGoodnessOfFit;};
		imageProClassParamDataType getPearsonCorrelation(void){return m_SimpPearsonCorrelation;};
		imageProClassParamDataType getSimpleE2Diameter(int ind)
		{
			if (ind == 0)
			{
				return m_simpleE2DiameterX;
			}
			else if (ind == 1)
			{
				return m_simpleE2DiameterY;
			}
			else
			{
				return 0;
			}
		};
		imageProClassParamDataType getXSectionFFTMajorFreqErrorSum(void){return m_xSectionMajorStruct.freqErrorSum;};
		imageProClassParamDataType getXSectionFFTMajorFreqErrorAverage(void){return m_xSectionMajorStruct.freqErrorAverage;};
		imageProClassParamDataType getXSectionFFTMajorFreq0(void){return m_xSectionMajorStruct.freq0;};
		imageProClassParamDataType getXSectionFFTMajorAmp0(void){return m_xSectionMajorStruct.amp0/m_xSectionMajorStruct.maxAmp;};
		imageProClassParamDataType getXSectionFFTMinorFreqErrorSum(void){return m_xSectionMinorStruct.freqErrorSum;};
		imageProClassParamDataType getXSectionFFTMinorFreqErrorAverage(void){return m_xSectionMinorStruct.freqErrorAverage;};
		imageProClassParamDataType getXSectionFFTMinorFreq0(void){return m_xSectionMinorStruct.freq0;};
		imageProClassParamDataType getXSectionFFTMinorAmp0(void){return m_xSectionMinorStruct.amp0/m_xSectionMinorStruct.maxAmp;};
		int getXSectionFFTMajorFreqSize(void){return m_xSectionMajorStruct.freqVector.GetSize();};
		int getXSectionFFTMajorAmpSize(void){return m_xSectionMajorStruct.ampVector.GetSize();};
		int getXSectionFFTMinorFreqSize(void){return m_xSectionMinorStruct.freqVector.GetSize();};
		int getXSectionFFTMinorAmpSize(void){return m_xSectionMinorStruct.ampVector.GetSize();};
		const float* getXSectionFFTMajorFreqVec(void){return m_xSectionMajorStruct.freqVector.GetBuffer();};
		const float* getXSectionFFTMajorAmpVec(void){return m_xSectionMajorStruct.ampVector.GetBuffer();};
		const float* getXSectionFFTMajorDataVec(void){return m_xSectionMajorStruct.dataVector.GetBuffer();};
		const float* getXSectionFFTMinorFreqVec(void){return m_xSectionMinorStruct.freqVector.GetBuffer();};
		const float* getXSectionFFTMinorAmpVec(void){return m_xSectionMinorStruct.ampVector.GetBuffer();};
		const float* getXSectionFFTMinorDataVec(void){return m_xSectionMinorStruct.dataVector.GetBuffer();};
		const float* getRThetaHatE2Data(void){return m_rThetaHatE2Struct.dataVector.GetBuffer();};
		const float* getRThetaHatEData(void){return m_rThetaHatEStruct.dataVector.GetBuffer();};
		const float* getRThetaHatPercentData(void){return m_rThetaHatPercStruct.dataVector.GetBuffer();};
		int getRThetaHatESize(void){return m_rThetaHatEStruct.numberDataPoints;};
		int getRThetaHatE2Size(void){return m_rThetaHatE2Struct.numberDataPoints;};
		int getRThetaHatPercSize(void){return m_rThetaHatPercStruct.numberDataPoints;};
		int getXSectionFFTMajorSize(void){return m_xSectionMajorStruct.numberUniqueFreq;};
		int getXSectionFFTMinorSize(void){return m_xSectionMinorStruct.numberUniqueFreq;};
		double getRThetaHatE2Contrast(void){return m_rThetaHatE2Contrast;};
		double getRThetaHatEContrast(void){return m_rThetaHatEContrast;};
		double getRThetaHatPercContrast(void){return m_rThetaHatPercContrast;};	
		double getRThetaHatE2PkToPk(void){return m_rThetaHatE2PkToPk;};
		double getRThetaHatEPkToPk(void){return m_rThetaHatEPkToPk;};
		double getRThetaHatPercPkToPk(void){return m_rThetaHatPercPkToPk;};
		double getRThetaHatE2Average(void){return m_rThetaHatE2Average;};
		double getRThetaHatEAverage(void){return m_rThetaHatEAverage;};
		double getRThetaHatPercAverage(void){return m_rThetaHatPercAverage;};
		double getRThetaHatRadiusE(void){return m_rThetaHatRadiusE;};
		double getRThetaHatRadiusE2(void){return m_rThetaHatRadiusE2;};
		double getRThetaHatRadiusPerc(void){return m_rThetaHatRadiusPerc;};
		bool getBeamPositionalStabilityInitialized(void){return m_SimpBeamPositionalStabilityInitialized;};


		// Symmetry
		void calcSymmetryMetric(int numLines, imageProClassParamDataType radMax);
		void calcRadialSymmetry(int numLines, imageProClassParamDataType radMax);
		void calcAxialSymmetry(int width, int height, int axis);
		imageProClassParamDataType getSymmetryMetric(void){return		m_SymmetryMetric;};
		imageProClassParamDataType getRadialSymmetry(void){return		m_radialRMS;};
		imageProClassParamDataType getAxialSymmetry(void){return		m_axialRMS;};

		// histograms and raw image metrics
		void maxHistVert(void);
		void maxHistHorz(void);
		void aveHistVert(void);
		void aveHistHorz(void);
		matrixClass& getHistVert(void){return m_vertHist;};
		matrixClass& getHistHorz(void){return m_horzHist;};
		imageProClassParamDataType getHistVertMax(void){return m_vertHistMax;};
		imageProClassParamDataType getHistHorzMax(void){return m_horzHistMax;};
		imageProClassParamDataType getHistVertBottomEdge(void){return m_vertHist(1,1);};
		imageProClassParamDataType getHistVertTopEdge(void){return m_vertHist(m_numRows,1);};
		imageProClassParamDataType getHistHorzLeftEdge(void){return m_vertHist(1,1);};
		imageProClassParamDataType getHistHorzRightEdge(void){return m_vertHist(m_numCols,1);};
		void calcRawMetrics(void);
		imageProClassParamDataType 	getMaxRawIntens(void){return m_maxRawIntens;};
		imageProClassParamDataType	getMaxRawIndx(void){return m_maxRawIndx;};
		imageProClassParamDataType	getMaxRawIndy(void){ return (m_numRows-1-m_maxRawIndy);};
		imageProClassParamDataType	getMinRawIntens(void){ return m_minRawIntens;};
		imageProClassParamDataType	getMeanRawIntens(void){ return m_meanRawIntens;};
		imageProClassParamDataType	getMedianRawIntens(void){ return m_medianRawIntens;};
		imageProClassParamDataType	getSumRawIntens(void){ return m_sumRawIntens;};
		imageProClassParamDataType	getBorderLevel(void){ return m_borderLevel;};

		// EM GMM
		void initEMfit(imageProClassParamDataType * initMu, imageProClassParamDataType * initCOV, imageProClassParamDataType * initMix);
		void calcEMfitMetrics(void);
		void setEMLogLike2EMLogLikePrev(void){m_emLogLikePrev = m_emLogLike;};
		void mStep(void);
		void eStep(void);
		void logLike(void);
		imageProClassParamDataType getEMCOV(int r, int c){return m_emCOV(r,c);};
		imageProClassParamDataType getEMCOM(int r, int c){return m_emCOM(r,c);};
		imageProClassParamDataType getEMMix(int r){return m_emMix(r,1);};
		imageProClassParamDataType getLogLike(void){return m_emLogLike;};
		imageProClassParamDataType getLogLikePrev(void){return m_emLogLikePrev;};
		int getMaxIterations(void){return M_MAX_ITERATIONS;};

		// sorting
		void heapSort(matrixClass &numbers, int array_size);
		void heapSort(CrashFloatArray &numbers, int array_size);
		void heapSort2(CrashFloatArray &numbersIndep, CrashFloatArray &numbersDep, int array_size);
		void partialHeapSort(CrashFloatArray &numbers, int array_size, int index);
		void partialHeapSort(matrixClass &numbers, int array_size, int index);
		void setTempVec(imageProClassParamDataType * ptr, int array_size)
		{
			for(int ind=0; ind<array_size; ind++)
			{
				m_tempVec[ind] = (float) ptr[ind];
			}
		};
		imageProClassParamDataType getTempVec(int ind){return m_tempVec[ind];};
		imageProClassParamDataType getResp(int r, int c){return m_emResp(r,c);};

	protected:
		bool m_initialized;

	private:
		matrixClass m_image;		// matrix for intensity image (N x M) 
		matrixClass m_imageResult;   // matrix for thresholded or cut image (N x M) 
		matrixClass m_originalImage; // matrix for holding the original raw image 
		matrixClass m_dataLoc;		// 2 x N*M matrix for location of data points
		matrixClass m_dataLocMap;   // NxM matrix for valid data locations
		matrixClass m_backgroundImage;
		matrixClass m_lowpassStates;
		matrixClass m_imageSum;
		matrixClass m_convolutedImage;
		matrixClass m_convolutionMask;
		int 		m_numberImagesSummed;
		bool		m_lowpassInitialized;
		int			m_convolutionMaskWidth;
		int			m_convolutionMaskHeight;

		// raw metrics
		matrixClass m_imageHistogram;
		CrashFloatArray m_borderArray;
		imageProClassParamDataType m_maxRawIntens;
		imageProClassParamDataType m_maxRawIndx;
		imageProClassParamDataType m_maxRawIndy;
		imageProClassParamDataType m_minRawIntens;
		imageProClassParamDataType m_meanRawIntens;
		imageProClassParamDataType m_medianRawIntens;
		imageProClassParamDataType m_sumRawIntens;
		imageProClassParamDataType m_borderLevel;
		imageProClassParamDataType m_borderSum;
		imageProClassParamDataType m_borderMax;
		int m_numBorderPixels;

		// simple metrics and COM
		imageProClassParamDataType 	m_Vol;
		imageProClassParamDataType 	m_CMx;
		imageProClassParamDataType 	m_CMy;
		imageProClassParamDataType 	m_M1x;
		imageProClassParamDataType 	m_M1y;
		imageProClassParamDataType 	m_M1xy;
		imageProClassParamDataType 	m_M2x;
		imageProClassParamDataType 	m_M2y;
		imageProClassParamDataType 	m_M3x;
		imageProClassParamDataType 	m_M3y;
		imageProClassParamDataType 	m_M4x;
		imageProClassParamDataType 	m_M4y;
		imageProClassParamDataType 	m_Vol2;
		imageProClassParamDataType 	m_SimpAngle;
		imageProClassParamDataType 	m_var_xy;
		imageProClassParamDataType 	m_var_yy;
		imageProClassParamDataType 	m_var_xx;
		imageProClassParamDataType 	m_SimpVxx;
		imageProClassParamDataType 	m_SimpVyy;
		imageProClassParamDataType 	m_SimpBeamDiam_x;
		imageProClassParamDataType 	m_SimpBeamDiam_y; 
		imageProClassParamDataType 	m_SimpBeamRad_x; 
		imageProClassParamDataType 	m_SimpBeamRad_y; 
		imageProClassParamDataType 	m_SimpEllip;
		imageProClassParamDataType 	m_SimpMaxIntens;
		imageProClassParamDataType 	m_SimpEnergyInEllipse;
		imageProClassParamDataType 	m_SimpEnergyRatio;
		imageProClassParamDataType 	m_SimpPerfMetric;
		imageProClassParamDataType 	m_kurtX;
		imageProClassParamDataType 	m_kurtY;
		imageProClassParamDataType 	m_skewX;
		imageProClassParamDataType 	m_skewY;

		// beamgage metrics based on simple metrics
		imageProClassParamDataType 	m_SimpCrossSectionalArea;
		imageProClassParamDataType 	m_SimpDivergenceAngleX;
		imageProClassParamDataType 	m_SimpDivergenceAngleY;
		imageProClassParamDataType 	m_SimpCWPower;
		imageProClassParamDataType 	m_SimpPowerDensity;
		imageProClassParamDataType 	m_SimpAzimuthAngle;
		bool 						m_SimpBeamPositionalStabilityInitialized;
		imageProClassParamDataType 	m_SimpBeamPositionalStabilityCentX;
		imageProClassParamDataType 	m_SimpBeamPositionalStabilityCentY;
		imageProClassParamDataType 	m_SimpBeamPositionalStabilityDiamX;
		imageProClassParamDataType 	m_SimpBeamPositionalStabilityDiamY;
		imageProClassParamDataType 	m_SimpBeamSSCentX;
		imageProClassParamDataType 	m_SimpBeamSSCentY;
		imageProClassParamDataType 	m_SimpBeamSSDiamX;
		imageProClassParamDataType 	m_SimpBeamSSDiamY;
		imageProClassParamDataType 	m_simpleE2DiameterX;
		imageProClassParamDataType 	m_simpleE2DiameterY;
		imageProClassParamDataType 	m_simpleE2Ellipticity;
		imageProClassParamDataType 	m_SimpMaximumPower;
		imageProClassParamDataType 	m_SimpEffectiveIrradiationArea;
		imageProClassParamDataType 	m_SimpEffectiveAveragePower;
		imageProClassParamDataType 	m_SimpFlatnessFactor;
		imageProClassParamDataType 	m_SimpBeamUniformity;
		imageProClassParamDataType 	m_SimpPlateauUniformity;
		imageProClassParamDataType 	m_SimpEdgeSteepness;
		imageProClassParamDataType 	m_SimpRoughnessOfFit;
		imageProClassParamDataType 	m_SimpGoodnessOfFit;
		imageProClassParamDataType 	m_rThetaTopHat;
		imageProClassParamDataType 	m_SimpPearsonCorrelation;
		CrashFloatArray 			m_radE2;

		// rtheta metrics
		fftStruct 					m_rThetaHatEStruct;
		fftStruct 					m_rThetaHatE2Struct;
		fftStruct 					m_rThetaHatPercStruct;

		int 						m_rThetaHatESize;
		int 						m_rThetaHatE2Size;
		int 						m_rThetaHatPercSize;
		imageProClassParamDataType	m_rThetaHatRadiusE;
		imageProClassParamDataType	m_rThetaHatRadiusE2;
		imageProClassParamDataType	m_rThetaHatRadiusPerc;


		imageProClassParamDataType 	m_rThetaHatMeanE2;
		imageProClassParamDataType 	m_rThetaHatStdE2;
		imageProClassParamDataType 	m_rThetaHatMeanE;
		imageProClassParamDataType 	m_rThetaHatStdE;
		imageProClassParamDataType 	m_rThetaHatMeanPerc;
		imageProClassParamDataType 	m_rThetaHatStdPerc;
		imageProClassParamDataType 	m_rThetaHatE2Contrast;
		imageProClassParamDataType 	m_rThetaHatEContrast;
		imageProClassParamDataType 	m_rThetaHatPercContrast;
		imageProClassParamDataType 	m_rThetaHatE2PkToPk;
		imageProClassParamDataType 	m_rThetaHatEPkToPk;
		imageProClassParamDataType 	m_rThetaHatPercPkToPk;
		imageProClassParamDataType 	m_rThetaHatE2Average;
		imageProClassParamDataType 	m_rThetaHatEAverage;
		imageProClassParamDataType 	m_rThetaHatPercAverage;

		fftStruct 					m_xSectionMajorStruct;
		fftStruct 					m_xSectionMinorStruct;
		fftStruct					m_xSectionMajorActStruct;
		fftStruct					m_xSectionMinorActStruct;


		// symmetry metrics
		imageProClassParamDataType m_SymmetryMetric;
		imageProClassParamDataType m_radialRMS;
		imageProClassParamDataType m_axialRMS;

		// em mixture matrices
		matrixClass m_psi;
		matrixClass m_logGauss;
		matrixClass m_logGaussTemp;
		matrixClass m_theta;
		matrixClass m_M;
		matrixClass m_lambda;
		matrixClass m_psiTrans;
		matrixClass m_temp6x6;
		matrixClass m_temp6x6_2;
		matrixClass m_temp6x1;
		matrixClass m_G;

		// 1D Gaussian Fit
		matrixClass m_psiFull;
		matrixClass m_psiFullTrans;
		matrixClass m_thetaFull;
		matrixClass m_psiInverse;
		matrixClass m_zData;
		matrixClass m_tempMatrix0;
		matrixClass m_tempMatrix1;

		// ls fit metrics
		matrixClass m_lsCOM;
		matrixClass m_lsCOV;
		imageProClassParamDataType m_lsVxx;
		imageProClassParamDataType m_lsVyy;
		imageProClassParamDataType m_lsBeamDiam_x;
		imageProClassParamDataType m_lsBeamDiam_y;
		imageProClassParamDataType m_ellip;
		imageProClassParamDataType m_angle;
		imageProClassParamDataType m_MaxIntens;
		imageProClassParamDataType m_lsPerfMetric;
		
		int m_dataCounter;          // number of valid data points is m_dataLoc.
		matrixClass m_vertHist;     
		matrixClass m_horzHist;
		matrixClass m_vertHistTemp;
		matrixClass m_horzHistTemp;
		imageProClassParamDataType m_vertHistMax;
		imageProClassParamDataType m_horzHistMax;
		int m_numRows;
		int m_numCols;

		CrashFloatArray m_tempVec;

		// EM mixture of Gaussians
		int m_numGauss;
		matrixClass m_emResp;
		matrixClass m_emMix;
		matrixClass m_emCOM;
		matrixClass m_emCOV;
		matrixClass m_emNk;
		imageProClassParamDataType m_emLogLike;
		imageProClassParamDataType m_emLogLikePrev;
		matrixClass m_emGauss;

		void siftDown(matrixClass &numbers, int root, int bottom);
		void siftDown(CrashFloatArray &numbers, int root, int bottom);
		void siftDown2(CrashFloatArray &numbersIndep, CrashFloatArray &numbersDep, int start, int end);
		imageProClassParamDataType interpolatePixel(imageProClassParamDataType r, imageProClassParamDataType c, int r1, int c1);
		void correctVariance(imageProClassParamDataType &var, imageProClassParamDataType level);
		void bitrev(CrashFloatArray &vector, unsigned int N);
		void bitrev2(CrashFloatArray &fftVectorReal, CrashFloatArray &fftVectorImag, unsigned int N);
		void fft(CrashFloatArray &fftVectorReal, CrashFloatArray &fftVectorImag, unsigned int N);

		imageProClass(const imageProClass&);
		imageProClass& operator=(const imageProClass&);
};

#endif // __ALGOLIB_IMAGEPROCLASS
