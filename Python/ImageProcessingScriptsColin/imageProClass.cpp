#include "imageProClass.h"
#include <math.h>
#include <cfloat>
#include <iostream>

// imageProClass::imageProClass():
	// m_initialized(false),
	// m_PARAM1(0.0f),
	// m_output(0.0f)
imageProClass::imageProClass():
	m_initialized(false),
 	m_numberImagesSummed(0),
 	m_lowpassInitialized(false),
 	m_convolutionMaskWidth(0),
 	m_convolutionMaskHeight(0),
	m_numBorderPixels(0),
 	m_SimpBeamPositionalStabilityInitialized(false),
	m_rThetaHatESize(0),
	m_rThetaHatE2Size(0),
	m_rThetaHatPercSize(0),
	m_dataCounter(0),
	m_numRows(0),
	m_numCols(0),
	m_numGauss(0)
{
/******************************************************************************
*
*   Function Name: imageProClass (constructor)
*
*   Description: 
*
******************************************************************************/
    
	construct();
} // end of constructor

imageProClass::imageProClass(int numRows, int numCols):
	m_initialized(false),
 	m_numberImagesSummed(0),
 	m_lowpassInitialized(false),
 	m_convolutionMaskWidth(0),
 	m_convolutionMaskHeight(0),
	m_numBorderPixels(0),
 	m_SimpBeamPositionalStabilityInitialized(false),
	m_rThetaHatESize(0),
	m_rThetaHatE2Size(0),
	m_rThetaHatPercSize(0),
	m_dataCounter(0),
	m_numRows(0),
	m_numCols(0),
	m_numGauss(0)
{
/******************************************************************************
*
*   Function Name: imageProClass (constructor)
*
*   Description: 
*
******************************************************************************/
	construct();
	setSize(numRows,numCols);

} // end of constructor

void imageProClass::construct()
{
    // m_output = 0;
	// m_PARAM1 = 0;

	
	// Instantiate the parameter table
    // m_numberParams = NUMBER_PARAMS;
    // m_paramTable = m_localParams;
    
	
	// InsertParamEntry(PARAM1, "param1", FLOAT, &m_PARAM1 );
	
	m_maxRawIntens=0;
	m_maxRawIndx=0;
	m_maxRawIndy=0;
	m_minRawIntens=0;
	m_meanRawIntens=0;
	m_medianRawIntens=0;
	m_sumRawIntens=0;
	m_Vol=1;
	m_M1x=0;
	m_M1y=0;
	m_M1xy=0;
	m_M2x=0;
	m_M2y=0;
	m_M3x=0;
	m_M4x=0;
	m_M3y=0;
	m_M4y=0;
	m_Vol2=0;
	m_CMx=0;
	m_CMy=0;
	m_SimpAngle=0;
	m_var_xy=0;
	m_var_yy=0;
	m_var_xx=0;
	m_SimpVxx=0;
	m_SimpVyy=0;
	m_SimpBeamDiam_x=0;
	m_SimpBeamDiam_y=0; 
	m_SimpBeamRad_x=0; 
	m_SimpBeamRad_y=0; 
	m_SimpEllip=0;
	m_SimpMaxIntens=0;
	m_SimpEnergyInEllipse=0;
	m_SimpEnergyRatio=0;
	m_SimpPerfMetric=0;
	m_SimpCrossSectionalArea=0;
	m_SimpDivergenceAngleX=0;
	m_SimpDivergenceAngleY=0;
	m_SimpCWPower=0;
	m_SimpPowerDensity=0;
	m_SimpAzimuthAngle=0;
	m_SimpBeamPositionalStabilityCentX=0;
	m_SimpBeamPositionalStabilityCentY=0;
	m_SimpBeamPositionalStabilityDiamX=0;
	m_SimpBeamPositionalStabilityDiamY=0;
	m_SimpBeamSSCentX=0;
	m_SimpBeamSSCentY=0;
	m_SimpBeamSSDiamX=0;
	m_SimpBeamSSDiamY=0;
	m_simpleE2DiameterX=0;
	m_simpleE2DiameterY=0;
	m_simpleE2Ellipticity=0;
	m_SimpMaximumPower=0;
	m_SimpEffectiveIrradiationArea=0;
	m_SimpEffectiveAveragePower=0;
	m_SimpFlatnessFactor=0;
	m_SimpBeamUniformity=0;
	m_SimpPlateauUniformity=0;
	m_SimpEdgeSteepness=0;
	m_SimpRoughnessOfFit=0;
	m_SimpGoodnessOfFit=0;
	m_rThetaTopHat=0;
	m_SimpPearsonCorrelation=0;
	m_kurtX=0;
	m_kurtY=0;
	m_skewX=0;
	m_skewY=0;
	m_SymmetryMetric=0;
	m_radialRMS=0;
	m_axialRMS=0;


	m_rThetaHatMeanE2=0;
	m_rThetaHatStdE2=0;
	m_rThetaHatMeanE=0;
	m_rThetaHatStdE=0;
	m_rThetaHatMeanPerc=0;
	m_rThetaHatStdPerc=0;
	m_rThetaHatE2Contrast=0;
	m_rThetaHatEContrast=0;
	m_rThetaHatPercContrast=0;
	m_rThetaHatE2PkToPk=0;
	m_rThetaHatEPkToPk=0;
	m_rThetaHatPercPkToPk=0;
	m_rThetaHatE2Average=0;
	m_rThetaHatEAverage=0;
	m_rThetaHatPercAverage=0;
	m_rThetaHatRadiusE=0;
	m_rThetaHatRadiusE2=0;
	m_rThetaHatRadiusPerc=0;



	m_xSectionMajorStruct.maxAmp=0;
	m_xSectionMajorStruct.freq0=0;
	m_xSectionMajorStruct.amp0=0;
	m_xSectionMajorStruct.numberUniqueFreq=0;
	m_xSectionMajorStruct.numberDataPoints=0;
	m_xSectionMajorStruct.freqErrorSum=0;
	m_xSectionMajorStruct.freqErrorAverage=0;
	m_xSectionMinorStruct.maxAmp=0;
	m_xSectionMinorStruct.freq0=0;
	m_xSectionMinorStruct.amp0=0;
	m_xSectionMinorStruct.numberUniqueFreq=0;
	m_xSectionMinorStruct.numberDataPoints=0;
	m_xSectionMinorStruct.freqErrorSum=0;
	m_xSectionMinorStruct.freqErrorAverage=0;
	m_xSectionMajorActStruct.maxAmp=0;
	m_xSectionMajorActStruct.freq0=0;
	m_xSectionMajorActStruct.amp0=0;
	m_xSectionMajorActStruct.numberUniqueFreq=0;
	m_xSectionMajorActStruct.numberDataPoints=0;
	m_xSectionMajorActStruct.freqErrorSum=0;
	m_xSectionMajorActStruct.freqErrorAverage=0;
	m_xSectionMinorActStruct.maxAmp=0;
	m_xSectionMinorActStruct.freq0=0;
	m_xSectionMinorActStruct.amp0=0;
	m_xSectionMinorActStruct.numberUniqueFreq=0;
	m_xSectionMinorActStruct.numberDataPoints=0;
	m_xSectionMinorActStruct.freqErrorSum=0;
	m_xSectionMinorActStruct.freqErrorAverage=0;

	m_lsVxx=0;
	m_lsVyy=0;
	m_lsBeamDiam_x=0;
	m_lsBeamDiam_y=0;
	m_ellip=0;
	m_angle=0;
	m_MaxIntens=0;
	m_lsPerfMetric=0;

	m_numGauss = 1;
	m_borderLevel=0;
	m_borderSum = 0;
	m_borderMax = 0;

	m_vertHistMax = 0;
	m_horzHistMax = 0;



	m_emLogLike = 0;
	m_emLogLikePrev = 0;

}

void imageProClass::setSize(int numRows, int numCols)
{
/******************************************************************************
*
*   Function Name: setSize (constructor)
*
*   Description: 
*
******************************************************************************/
	m_numRows=numRows;
	m_numCols=numCols;
	if (m_initialized==false)
	{
		m_imageHistogram.setSize(M_IMAGE_DEPTH,1);
		m_image.setSize(numRows,numCols);
		m_imageResult.setSize(numRows,numCols);
		m_originalImage.setSize(numRows,numCols);
		m_dataLocMap.setSize(numRows,numCols);
		m_dataLoc.setSize(2,numCols*numRows);
		m_backgroundImage.setSize(numRows,numCols);
		m_lowpassStates.setSize(numRows,numCols);
		m_imageSum.setSize(numRows,numCols);
		m_convolutedImage.setSize(numRows,numCols);
		m_convolutionMask.setSize(M_CONVOLUTION_MATRIX_MAX_HEIGHT,M_CONVOLUTION_MATRIX_MAX_WIDTH);
		m_psi.setSize(1,6);
		m_logGauss.setSize(numRows*numCols,1);
		m_logGaussTemp.setSize(numRows*numCols,1);

		m_rThetaHatEStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatEStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatEStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatEStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatEStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatE2Struct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatE2Struct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatE2Struct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatE2Struct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatE2Struct.ampVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatPercStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatPercStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatPercStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatPercStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_rThetaHatPercStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);

		m_xSectionMajorStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorActStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorActStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorActStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorActStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMajorActStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorActStruct.dataVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorActStruct.realVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorActStruct.imagVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorActStruct.freqVector.SetSize(M_MAX_FFT_LENGTH);
		m_xSectionMinorActStruct.ampVector.SetSize(M_MAX_FFT_LENGTH);

		m_radE2.SetSize(4);
	
		m_theta.setSize(6,1);
		m_M.setSize(6,6);
		m_lambda.setSize(6,1);
		m_psiTrans.setSize(6,1);
		m_temp6x6.setSize(6,6);
		m_temp6x6_2.setSize(6,6);
		m_temp6x1.setSize(6,1);
		m_G.setSize(6,1);

		m_psiFull.setSize(100,3);
		m_psiFullTrans.setSize(3,100);
		m_thetaFull.setSize(3,1);
		m_psiInverse.setSize(3,3);
		m_zData.setSize(100,1);
		m_tempMatrix0.setSize(3,3);
		m_tempMatrix1.setSize(3,1);

		m_vertHist.setSize(numRows,1);
		m_horzHist.setSize(numCols,1);
		m_vertHistTemp.setSize(numRows,1);
		m_horzHistTemp.setSize(numCols,1);
		m_tempVec.SetSize(1000);

		m_lsCOM.setSize(2,1);
		m_lsCOV.setSize(2,2);

		m_borderArray.SetSize(m_numRows*2 + (m_numCols-2)*2);
	
	}
	

} // end of setSize()

// imageProClass::imageProClass(int numRows, int numCols, int numGauss):
	// m_initialized(false),
	// m_PARAM1(0.0f),
	// m_output(0.0f)
imageProClass::imageProClass(int numRows, int numCols, int numGauss):
	m_initialized(false),
 	m_numberImagesSummed(0),
 	m_lowpassInitialized(false),
 	m_convolutionMaskWidth(0),
 	m_convolutionMaskHeight(0),
	m_numBorderPixels(0),
 	m_SimpBeamPositionalStabilityInitialized(false),
	m_rThetaHatESize(0),
	m_rThetaHatE2Size(0),
	m_rThetaHatPercSize(0),
	m_dataCounter(0),
	m_numRows(0),
	m_numCols(0),
	m_numGauss(0)
{
/******************************************************************************
*
*   Function Name: imageProClass (constructor)
*
*   Description: 
*
******************************************************************************/

    construct();
	m_numGauss = numGauss;
	setSize(numRows, numCols);
	

	// EM mixture of Gaussians
	m_numGauss = numGauss;
	m_emResp.setSize(m_numRows*m_numCols,m_numGauss);
	m_emMix.setSize(m_numGauss,1);
	m_emCOM.setSize(2,m_numGauss);
	m_emCOV.setSize(2,2*m_numGauss);
	m_emNk.setSize(m_numGauss,1);
	m_emGauss.setSize(m_numRows*m_numCols,m_numGauss);

	m_emResp.zeros();
	m_emMix.zeros();
	m_emCOM.zeros();
	m_emCOV.eye();
	m_emNk.zeros();
	m_emGauss.zeros();
	
} // end of constructor



imageProClass::~imageProClass()
{

}  // end of destructor

CrashResult imageProClass::Initialize()
{
	if (m_initialized==false)
	{
		m_initialized = true;
		for(int k=0; k< m_borderArray.GetSize(); k++)
		{
			m_borderArray[k]=0;
		}

		for(int k=0; k<M_MAX_FFT_LENGTH; k++)
		{
			m_rThetaHatEStruct.dataVector[k] = 0;
			m_rThetaHatEStruct.realVector[k] = 0;
			m_rThetaHatEStruct.imagVector[k] = 0;
			m_rThetaHatEStruct.freqVector[k] = 0;
			m_rThetaHatEStruct.ampVector[k] = 0;
			m_rThetaHatE2Struct.dataVector[k] = 0;
			m_rThetaHatE2Struct.realVector[k] = 0;
			m_rThetaHatE2Struct.imagVector[k] = 0;
			m_rThetaHatE2Struct.freqVector[k] = 0;
			m_rThetaHatE2Struct.ampVector[k] = 0;
			m_rThetaHatPercStruct.dataVector[k] = 0;
			m_rThetaHatPercStruct.realVector[k] = 0;
			m_rThetaHatPercStruct.imagVector[k] = 0;
			m_rThetaHatPercStruct.freqVector[k] = 0;
			m_rThetaHatPercStruct.ampVector[k] = 0;

			m_xSectionMajorStruct.dataVector[k] = 0;
			m_xSectionMajorStruct.realVector[k] = 0;
			m_xSectionMajorStruct.imagVector[k] = 0;
			m_xSectionMajorStruct.freqVector[k] = 0;
			m_xSectionMajorStruct.ampVector[k] = 0;
			m_xSectionMinorStruct.dataVector[k] = 0;
			m_xSectionMinorStruct.realVector[k] = 0;
			m_xSectionMinorStruct.imagVector[k] = 0;
			m_xSectionMinorStruct.freqVector[k] = 0;
			m_xSectionMinorStruct.ampVector[k] = 0;
			m_xSectionMajorActStruct.dataVector[k] = 0;
			m_xSectionMajorActStruct.realVector[k] = 0;
			m_xSectionMajorActStruct.imagVector[k] = 0;
			m_xSectionMajorActStruct.freqVector[k] = 0;
			m_xSectionMajorActStruct.ampVector[k] = 0;
			m_xSectionMinorActStruct.dataVector[k] = 0;
			m_xSectionMinorActStruct.realVector[k] = 0;
			m_xSectionMinorActStruct.imagVector[k] = 0;
			m_xSectionMinorActStruct.freqVector[k] = 0;
			m_xSectionMinorActStruct.ampVector[k] = 0;
		}

		//set to zeros
		m_image.zeros();
		m_imageResult.zeros();
		m_imageHistogram.zeros();
		m_backgroundImage.zeros();
		m_lowpassStates.zeros();
		m_imageSum.zeros();
		m_dataLocMap.zeros();
		m_dataLoc.zeros();
		m_psi.zeros();
		m_logGauss.zeros();
		m_logGaussTemp.zeros();
	
		m_theta.zeros();
		m_M.zeros();
		m_lambda.zeros();
		m_psiTrans.zeros();
		m_temp6x6.zeros();
		m_temp6x6_2.zeros();
		m_temp6x1.zeros();
		m_G.zeros();

		m_psiFull.setSize(100,3);
		m_psiFullTrans.setSize(3,100);
		m_thetaFull.setSize(3,1);
		m_psiInverse.setSize(3,3);
		m_zData.setSize(100,1);
		m_tempMatrix0.setSize(3,3);
		m_tempMatrix1.setSize(3,1);

		m_vertHist.zeros();
		m_horzHist.zeros();
		m_vertHistTemp.zeros();
		m_horzHistTemp.zeros();
 

		m_lsCOM.zeros();
		m_lsCOV.zeros();

		return CrashBaseObject::Initialize();
	}
	else
	{
		return ALREADY_INITIALIZED;
	}

} // end of Initialize



// CrashResult imageProClass::SetParam(long paramID, imageProClassParamDataType value)
// {
// 	switch (paramID)
// 	{
// 		case PARAM1: m_PARAM1 = value;
// 			break;
// 		default: return INVALID_PARAM_ID;
// 			break;
// 	}

// 	return CrashBaseObject::SetParam(paramID, float(value));
// } // end of SetParam

void imageProClass::maxHistVert(void)
{
	m_vertHistMax = 0;
	for (int r=1; r< m_numRows+1; r++)
	{
		m_vertHist(r,1) = 0;

		for (int c=1; c < m_numCols+1; c++)
		{
			if (m_image(r,c) > m_vertHist(r,1))
			{
				m_vertHist(r,1) = m_image(r,c);
			}
		}
		if (m_vertHist(r,1) > m_vertHistMax)
		{
			m_vertHistMax = m_vertHist(r,1);
		}
	}


} // end of maxHistVert

void imageProClass::aveHistVert(void)
{
	m_vertHistMax = 0;
	for (int r=1; r< m_numRows+1; r++)
	{
		m_vertHist(r,1) = 0;

		for (int c=1; c < m_numCols+1; c++)
		{
			m_vertHist(r,1) += m_image(r,c);
		}
		m_vertHist(r,1) = m_vertHist(r,1)/m_numRows;
		if (m_vertHist(r,1) > m_vertHistMax)
		{
			m_vertHistMax = m_vertHist(r,1);
		}
	}


} // end of maxHistVert

void imageProClass::maxHistHorz(void)
{
	m_horzHistMax = 0;
	for (int c=1; c< m_numCols+1; c++)
	{
		m_horzHist(c,1) = 0;

		for (int r=1; r < m_numRows+1; r++)
		{
			if (m_image(r,c) > m_horzHist(c,1))
			{
				m_horzHist(c,1) = m_image(r,c);
			}
		}
		if (m_horzHist(c,1) > m_horzHistMax)
		{
			m_horzHistMax = m_vertHist(c,1);
		}

	}


} // end of maxHistHorz

void imageProClass::aveHistHorz(void)
{
	m_horzHistMax = 0;
	for (int c=1; c< m_numCols+1; c++)
	{
		m_horzHist(c,1) = 0;

		for (int r=1; r < m_numRows+1; r++)
		{
			m_horzHist(c,1) += m_image(r,c);
		}
		m_horzHist(c,1) = m_horzHist(c,1)/m_numCols;
		if (m_horzHist(c,1) > m_horzHistMax)
		{
			m_horzHistMax = m_vertHist(c,1);
		}
	}


} // end of maxHistHorz

void imageProClass::smoothHistVert(int numSmooth)
{
	m_vertHistTemp.zeros();
	for (int r=1; r< m_numRows+1; r++)
	{
		for (int k = -numSmooth; k < numSmooth+1; k++)
		{
			if (r+k>=1)
			{
				m_vertHistTemp(r,1) += m_vertHist(r+k,1);
			}
			
		}
		m_vertHistTemp(r,1)=m_vertHistTemp(r,1)/static_cast<double>(numSmooth);
	}
	m_vertHist=m_vertHistTemp;
}


void imageProClass::smoothHistHorz(int numSmooth)
{
	m_horzHistTemp.zeros();
	for (int c=1; c< m_numCols+1; c++)
	{
		for (int k = -numSmooth; k < numSmooth+1; k++)
		{
			if (c+k>=1 && c+k<=m_numCols)
			{
				m_horzHistTemp(c,1) += m_horzHist(c+k,1);
			}

		}
		m_horzHistTemp(c,1)=m_horzHistTemp(c,1)/static_cast<double>(numSmooth);
	}
	m_horzHist=m_horzHistTemp;
}



void imageProClass::smoothHistVert(CrashFloatArray &smoother)
{
	int L=smoother.GetSize();
	m_vertHistTemp.zeros();
	for (int r=1; r< m_numRows+1; r++)
	{
		for (int k = -L/2; k < L/2+1; k++)
		{
			if (r+k >0 && r+k <=m_numRows)
			{
				m_vertHistTemp(r,1) += m_vertHist(r+k,1)*smoother[k+L/2];
			}
		}
	}
	m_vertHist=m_vertHistTemp;
}


void imageProClass::smoothHistHorz(CrashFloatArray &smoother)
{
	int L=smoother.GetSize();
	m_horzHistTemp.zeros();
	for (int c=1; c< m_numCols+1; c++)
	{
		for (int k = -L/2; k < L/2+1; k++)
		{
			if (c+k>=1)
			{
				m_horzHistTemp(c,1) += m_horzHist(c+k,1)*smoother[k+L/2];
			}
			
		}
	}
	m_horzHist=m_horzHistTemp;
}

void imageProClass::thresImage( imageProClassParamDataType level )
{
	m_dataLoc.zeros();
	m_dataCounter=0;
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (m_image(r,c) > level)
			{
				m_dataCounter+=1;
				m_imageResult(r,c)=1;
				m_dataLoc(1,m_dataCounter)=r-1; // start at (0,0)
				m_dataLoc(2,m_dataCounter)=c-1; // start at (0,0)
				m_dataLocMap(r,c)=1;
			}
			else
			{
				m_imageResult(r,c) = 0;
				m_dataLocMap(r,c) = 0;
			}
		}
	}
}// end of thresImage

void imageProClass::cutImage( imageProClassParamDataType level )
{
	m_dataLoc.zeros();
	m_dataCounter=0;
	m_dataLocMap.zeros();
	imageProClassParamDataType temp=0;
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (m_image(r,c) > level)
			{
				m_dataCounter += 1;
				// m_imageResult(r,c) = m_image(r,c);
				m_dataLoc(1,m_dataCounter) = r-1; // start at (0,0)
				m_dataLoc(2,m_dataCounter) = c-1; // start at (0,0)
				m_dataLocMap(r,c) = 1;
			}
			else
			{
				// m_imageResult(r,c) = 0;
				m_image(r,c) = 0;
				m_dataLocMap(r,c) = 0;
			}
		}
	}
}// end of cutImage

void imageProClass::subtractBackgroundImage(void)
{
	m_dataLoc.zeros();
	m_dataCounter=0;
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (m_image(r,c) - m_backgroundImage(r,c) > 0)
			{
				m_dataCounter+=1;
				m_imageResult(r,c)= m_image(r,c) - m_backgroundImage(r,c);
				m_dataLoc(1,m_dataCounter)=r-1; // start at (0,0)
				m_dataLoc(2,m_dataCounter)=c-1; // start at (0,0)
				m_dataLocMap(r,c) =1;
			}
			else
			{
				m_imageResult(r,c) = 0;
				m_dataLocMap(r,c) = 0;
			}
		}
	}



}// end of subtractBackgroundImage

void imageProClass::lowpassFilterOnImage(imageProClassParamDataType beta)
{
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			m_lowpassStates(r,c) = beta*m_image(r,c) + (1-beta)*m_lowpassStates(r,c);
		}
	}
}

void imageProClass::setCircROI(imageProClassParamDataType R, imageProClassParamDataType C, imageProClassParamDataType rad)
{
	m_dataLoc.zeros();
	m_dataCounter=0;
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if ((r-1-R)*(r-1-R)+(c-1-C)*(c-1-C) < rad*rad)
			{
				m_dataCounter += 1;
				m_dataLoc(1,m_dataCounter) = r-1; // start at (0,0)
				m_dataLoc(2,m_dataCounter) = c-1; // start at (0,0)
				m_dataLocMap(r,c) = 1;
				// m_imageResult(r,c) = m_image(r,c);
				
			}
			else
			{
				m_image(r,c) = 0;
				// m_imageResult(r,c) = 0;
				m_dataLocMap(r,c) = 0;
			}
		}
	}


}// end of setCircROI



void imageProClass::medianFilter(int padSize)
{

	int ind=0;
	int flag=1;

	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			
			// get values 
			ind=0;

			for (int k=-padSize; k<= padSize; k++)
			{
				for (int kk=-padSize; kk<= padSize; kk++)
				{

					if (r+k>0 && c+kk>0 && r+k <=m_numRows && c+kk <=m_numCols)
					{
						m_tempVec[ind] = static_cast<float>(m_image(r+k,c+kk));
						ind++;
					}
					
				} // kk

			} // k

			// only partially sorts until it hits median (20-30% faster)
			partialHeapSort(m_tempVec, ind, ind/2);
			m_imageResult(r,c) = m_tempVec[0];

		}// cols
	} // row
}// end of medianFilter

void imageProClass::remNeg_thresImage(void)
{
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (m_imageResult(r,c)<0)
			{
				m_imageResult(r,c)=0;
			}
		}
	}
}// end of remNeg_thresImage


void imageProClass::remNeg_image(void)
{
	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (m_image(r,c)<0)
			{
				m_image(r,c)=0;
			}
		}
	}
}// end of remNeg_image

void imageProClass::leastSquares1D(int offset, fftStruct &inputStruct)
{
	m_psiFull.zeros();
	m_psiFullTrans.zeros();
	m_thetaFull.zeros();
	m_psiInverse.zeros();
	m_tempMatrix0.zeros();
	m_tempMatrix1.zeros();
	m_zData.zeros();

	imageProClassParamDataType x = 0;
	imageProClassParamDataType z = 0;

	for (int f=0; f<offset; f++)
	{
		// std::cout << inputStruct.freqVector[f] << "," << inputStruct.ampVector[f] << "\n";

		// if (  (inputStruct.ampVector[f] > .4*inputStruct.maxAmp) )
		x = static_cast<double>(inputStruct.freqVector[f]);
		z = log(inputStruct.ampVector[f]);

		m_psiFull(f+1,1) = x*x;
		m_psiFull(f+1,2) = x;
		m_psiFull(f+1,3) = 1;

		m_psiFullTrans(1,f+1) = x*x;
		m_psiFullTrans(2,f+1) = x;
		m_psiFullTrans(3,f+1) = 1;

		m_zData(f+1,1) = log(inputStruct.ampVector[f]);
	}

	mcMult(m_tempMatrix0,m_psiFullTrans,m_psiFull);
	mcInv(m_psiInverse,m_tempMatrix0);
	mcMult(m_tempMatrix1,m_psiFullTrans,m_zData);
	mcMult(m_thetaFull,m_psiInverse,m_tempMatrix1);

	imageProClassParamDataType varLS, muLS, ampLS, temp;
	imageProClassParamDataType sum = 0;
	imageProClassParamDataType sq_sum = 0;
	imageProClassParamDataType error_mean = 0;
	imageProClassParamDataType error_std = 0;
	int number_in_sum = 0;

	if(m_thetaFull(1,1) > 0 || m_thetaFull(1,1) < 0)
	{
		varLS = -1.0/2.0/m_thetaFull(1,1);
		muLS = -m_thetaFull(2,1)/2.0/m_thetaFull(1,1);
		ampLS = exp(m_thetaFull(3,1)-m_thetaFull(1,1)*muLS*muLS);
	}
	// std::cout << muLS << "," << varLS << "," << ampLS << "\n";
	for (int f=0; f<inputStruct.numberUniqueFreq; f++)
	{
		if (f>=offset)
		{
			temp = static_cast<double>(inputStruct.ampVector[f]) - ampLS*exp(-(static_cast<double>(inputStruct.freqVector[f])-muLS)*(static_cast<double>(inputStruct.freqVector[f])-muLS)/2.0/varLS);
			sum += temp;
			sq_sum += temp*temp;
			inputStruct.ampVector[f] = static_cast<float>(temp);
		}
		else
		{
			inputStruct.ampVector[f] = static_cast<float>(0);
		}
	}

	if (static_cast<double>(inputStruct.numberUniqueFreq - offset) > 0.0)
	{
		error_mean = sum/static_cast<double>(inputStruct.numberUniqueFreq - offset);
		error_std = sqrt(sq_sum/static_cast<double>(inputStruct.numberUniqueFreq - offset) - error_mean*error_mean);
	}
	
	sum = 0;

	for (int f=offset; f<inputStruct.numberUniqueFreq; f++)
	{
		if (abs(static_cast<double>(inputStruct.ampVector[f]) - error_mean) > 1.0*error_std)
		{
			sum += (static_cast<double>(inputStruct.ampVector[f]) - error_mean);
			number_in_sum++;
		}
	}

	inputStruct.freqErrorSum = sum;
	if (number_in_sum > 0)
	{
		inputStruct.freqErrorAverage = sum/static_cast<double>(number_in_sum);
	} 
	else
	{
		inputStruct.freqErrorAverage = 0.0;
	}

}// end of leastSquaresLogFit

void imageProClass::leastSquaresLogFit(imageProClassParamDataType lsFitDelta)
{
	m_M.diag(lsFitDelta);
	m_theta.zeros();
	m_G.zeros();
	m_lambda.zeros();
	m_psi.zeros();
	m_psiTrans.zeros();
	m_temp6x6_2.zeros();
	m_temp6x6.zeros();

	imageProClassParamDataType m_lG=0;
	int ind=1;
	imageProClassParamDataType temp=0;

	imageProClassParamDataType x = 0;
	imageProClassParamDataType y = 0;



	for (int r=1; r<= m_numRows; r++)
	{
		for (int c=1; c<= m_numCols; c++)
		{
			if (  (m_image(r,c) > 0.0) )
			{   
				x = static_cast<double>(c-1);
				y = static_cast<double>(r-1);
				m_lG = log(m_image(r,c));

				m_psi(1,1) = x*x;
				m_psi(1,2) = 2*x*y;
				m_psi(1,3) = y*y;
				m_psi(1,4) = x;
				m_psi(1,5) = y;
				m_psi(1,6) = 1;
				
				m_psiTrans(1,1) = x*x;
				m_psiTrans(2,1) = 2*x*y;
				m_psiTrans(3,1) = y*y;
				m_psiTrans(4,1) = x;
				m_psiTrans(5,1) = y;
				m_psiTrans(6,1) = 1;

				mcMult(m_lambda, m_M, m_psiTrans);
				temp = 1.0 + m_psi(1,1)*m_lambda(1,1) + m_psi(1,2)*m_lambda(2,1) + m_psi(1,3)*m_lambda(3,1)+
					         m_psi(1,4)*m_lambda(4,1) + m_psi(1,5)*m_lambda(5,1) + m_psi(1,6)*m_lambda(6,1);
				m_lambda /= temp;
				m_G = m_lambda;
				
				for (int rI=1; rI<=6; rI++)
				{
					for (int cI=1; cI<=6; cI++)
					{
						if (rI==cI)
						{
							m_temp6x6(rI,cI) = 1.0-m_G(rI,1)*m_psi(1,cI);
						}
						else
						{
							m_temp6x6(rI,cI) = 0.0-m_G(rI,1)*m_psi(1,cI);
						}
					}

				}
				mcMult(m_temp6x6_2,m_temp6x6,m_M);
				m_M=m_temp6x6_2;

				temp = m_lG - 
					    (m_psi(1,1)*m_theta(1,1) + m_psi(1,2)*m_theta(2,1) + m_psi(1,3)*m_theta(3,1)+
					     m_psi(1,4)*m_theta(4,1) + m_psi(1,5)*m_theta(5,1) + m_psi(1,6)*m_theta(6,1));

				m_G *= temp;
				m_theta += m_G;

				ind+=1;
			}
		}
	}

}// end of leastSquaresLogFit

void imageProClass::calcLSfitMetrics(void)
{
	imageProClassParamDataType a = m_theta(1,1);
	imageProClassParamDataType b = m_theta(2,1);
	imageProClassParamDataType c = m_theta(3,1);

	imageProClassParamDataType det = a*c - b*b;

	imageProClassParamDataType x = 0;
	imageProClassParamDataType y = 0;

	if (det>0)
	{
		m_lsCOV(1,1) = -0.5*c/det;
		m_lsCOV(2,1) = 0.5*b/det;
		m_lsCOV(1,2) = m_lsCOV(2,1);
		m_lsCOV(2,2) = -0.5*a/det;
	}
	m_lsCOM(1,1) = m_lsCOV(1,1)*m_theta(4,1) + m_lsCOV(1,2)*m_theta(5,1);
	m_lsCOM(2,1) = m_lsCOV(2,1)*m_theta(4,1) + m_lsCOV(2,2)*m_theta(5,1);
	

	m_MaxIntens = 
	exp( 
		m_theta(6,1) + 
		m_lsCOM(1,1)*m_lsCOM(1,1)*a+2*m_lsCOM(1,1)*m_lsCOM(2,1)*b + m_lsCOM(2,1)*m_lsCOM(2,1)*c
		+  m_theta(4,1)*m_lsCOM(1,1) + m_theta(5,1)*m_lsCOM(2,1) 
	   );
	
	if (((a+c)*(a+c)-4*(a*c-b*b)) > 0)
	{
		if ( (-(a+c)-sqrt((a+c)*(a+c)-4*(a*c-b*b))) != 0  )
		{
			m_lsVxx = 1/  ( -(a+c)-sqrt((a+c)*(a+c)-4*(a*c-b*b))  );
		}
		if ( (-(a+c)+sqrt((a+c)*(a+c)-4*(a*c-b*b))) != 0 )
		{
			m_lsVyy = 1/  ( -(a+c)+sqrt((a+c)*(a+c)-4*(a*c-b*b))  );
		}
	}
	if (m_lsVxx > 0)
	{
		m_lsBeamDiam_x = 4*sqrt(m_lsVxx);
	}
	if (m_lsVyy > 0)
	{
		m_lsBeamDiam_y = 4*sqrt(m_lsVyy);
	}

	if (m_lsVxx<m_lsVyy)
	{
		m_ellip = sqrt(m_lsVxx/m_lsVyy);
	}
	else
	{
		m_ellip = sqrt(m_lsVyy/m_lsVxx);
	}

	m_angle = -0.5*atan2(2*b,a-c);
	
	if (m_angle > -M_PI/4 && m_angle< M_PI/4)
	{

	}
	else if(m_angle>= M_PI/4 && m_angle <  3*M_PI/4)
	{
		m_angle-= M_PI/2;
	}
	else if(m_angle>= M_PI*3/4 && m_angle< M_PI)
	{
		m_angle= M_PI-m_angle;
	}
	else if(m_angle<= -M_PI/4 && m_angle> -3*M_PI/4)
	{
		m_angle+= M_PI/2;
	}
	else 
	{
		m_angle= M_PI+m_angle;
	}

	m_angle = -m_angle;

	m_lsPerfMetric = 0;
	int ind=0;
	imageProClassParamDataType temp=0;
	for (int r=1; r<= m_numRows; r++)
	{
		for (int cI=1; cI<= m_numCols; cI++)
		{
			if (  (m_image(r,cI) > 0.0) )
			{   
				x = static_cast<double>(cI-1);
				y = static_cast<double>(r-1);
				m_psi(1,1) = x*x;
				m_psi(1,2) = 2*x*y;
				m_psi(1,3) = y*y;
				m_psi(1,4) = x;
				m_psi(1,5) = y;
				m_psi(1,6) = 1;
				
				m_psiTrans(1,1) = x*x;
				m_psiTrans(2,1) = 2*x*y;
				m_psiTrans(3,1) = y*y;
				m_psiTrans(4,1) = x;
				m_psiTrans(5,1) = y;
				m_psiTrans(6,1) = 1;

				// log(y) = ap*r^2 + bp*r*c + cp*c^2 + dp*r +  ep * c + fp
				// log( exp(fp) * exp (ap*r^2 + bp*r*c + cp*c^2 + dp*r +  ep * cp) ) = fp + ap*r^2 + bp*r*c + cp*c^2 + dp*r +  ep * c
				// exp(fp) * exp (ap*r^2 + bp*r*c + cp*c^2 + dp*r +  ep * c) =  y

				temp = (m_image(r,cI) - 
					   (
					   exp(m_theta(6,1))*exp(
					   m_psi(1,1)*m_theta(1,1) + m_psi(1,2)*m_theta(2,1) + m_psi(1,3)*m_theta(3,1)+
					         m_psi(1,4)*m_theta(4,1) + m_psi(1,5)*m_theta(5,1) 
					    )
						));
				m_lsPerfMetric+=fabs(temp);
				ind++;
			}
		}

	}

	if (ind >0 && m_Vol >0)
	{
		m_lsPerfMetric = static_cast<double>(m_lsPerfMetric/static_cast<double>(ind)/m_Vol);
	}


}//end of calcLSfitMetrics

void imageProClass::mStep(void)
{
	int ind=0;
	imageProClassParamDataType temp = 0;
	imageProClassParamDataType dx = 0;
	imageProClassParamDataType dy = 0;
	m_emNk.zeros();
	m_emCOM.zeros();
	m_emCOV.zeros();
	imageProClassParamDataType M2[2][2]={{0}};
	for (int nG=1; nG<=m_numGauss; nG++)
	{
		ind=0;	
		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				ind+=1;
				m_emNk(nG,1) += m_emResp(ind,nG)*m_image(r,c);
				m_emCOM(1,nG) += (c-1)*m_emResp(ind,nG)*m_image(r,c);
				m_emCOM(2,nG) += (r-1)*m_emResp(ind,nG)*m_image(r,c);			
			}
		}
		if(m_emNk(nG,1) != 0)
		{
			m_emCOM(1,nG) = m_emCOM(1,nG)/m_emNk(nG,1);
			m_emCOM(2,nG) = m_emCOM(2,nG)/m_emNk(nG,1);
		}

		ind=0;	
		M2[0][0]=0;
		M2[0][1]=0;
		M2[1][0]=0;
		M2[1][1]=0;
		temp=0;
		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				ind+=1;
				dx = (c-1)-m_emCOM(1,nG);
				dy = (r-1)-m_emCOM(2,nG);
				M2[0][0] += dx*dx*m_emResp(ind,nG)*m_image(r,c);
				M2[0][1] += dx*dy*m_emResp(ind,nG)*m_image(r,c);
				M2[1][1] += dy*dy*m_emResp(ind,nG)*m_image(r,c);
			}
		}

		if(m_emNk(nG,1) != 0)
		{
			m_emCOV(1,2*(nG-1)+1) = M2[0][0]/m_emNk(nG,1);
			m_emCOV(1,2*(nG-1)+2) = M2[0][1]/m_emNk(nG,1);
			m_emCOV(2,2*(nG-1)+1) = m_emCOV(1,2*(nG-1)+2);
			m_emCOV(2,2*(nG-1)+2) = M2[1][1]/m_emNk(nG,1);

		}

		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				temp+=m_image(r,c);
			
			}
		}
		//m_emMix(1,1) = 0.15;
		//m_emMix(2,1) = 0.85;
		m_emMix = m_emNk;
		m_emMix /= temp;
	}
}// end of mStep


void imageProClass::eStep(void)
{
	int ind=0;
	imageProClassParamDataType temp2x2[2][2]={{0}};
	imageProClassParamDataType det=0;
	imageProClassParamDataType z = 0;
	for (int nG=1; nG<=m_numGauss; nG++)
	{
		ind=0;
		//take the inverse of the COV
		det = m_emCOV(1,2*(nG-1)+1)*m_emCOV(2,2*(nG-1)+2)-m_emCOV(2,2*(nG-1)+1)*m_emCOV(1,2*(nG-1)+2);
		if (det>0)
		{
			temp2x2[1][1]= m_emCOV(1,2*(nG-1)+1)/det;
			temp2x2[1][0]= -m_emCOV(2,2*(nG-1)+1)/det;
			temp2x2[0][1]= -m_emCOV(1,2*(nG-1)+2)/det;
			temp2x2[0][0]= m_emCOV(2,2*(nG-1)+2)/det;
		}

		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				ind+=1;
				z = 0.5*temp2x2[0][0]*(c-1-m_emCOM(1,nG))*(c-1-m_emCOM(1,nG)) + 
					(c-1-m_emCOM(1,nG))*(r-1-m_emCOM(2,nG))*temp2x2[1][0] + 
					0.5*temp2x2[1][1]*(r-1-m_emCOM(2,nG))*(r-1-m_emCOM(2,nG));

				if (det>0)
				{
					m_emResp(ind,nG) = 1/(sqrt(det)*2*M_PI)*exp(-z);
				}
				
				m_emGauss(ind,nG) = m_emResp(ind,nG);
				m_emResp(ind,nG) = m_emMix(nG,1)*m_emResp(ind,nG);
			}
		}
	}

	imageProClassParamDataType temp=0;
	for (int r=1; r<=m_emResp.getNumRows(); r++)
	{
		temp=0;
		for (int c=1; c<=m_emResp.getNumCols(); c++)
		{
			temp+=m_emResp(r,c);
		}
		for (int c=1; c<=m_emResp.getNumCols(); c++)
		{
			if(temp > 0)
			{
				m_emResp(r,c) /= temp;
			}
		}
	}
	
} // end of estep


void imageProClass::logLike(void)
{
	m_emLogLike=0;
	int ind=0;
	imageProClassParamDataType temp=0;
	for (int r=1; r<=m_numRows; r++)
	{
		for (int c=1; c<=m_numCols; c++)
		{
			if ( (m_image(r,c)<=0) )
			{
				continue;
			}

			ind+=1;
			temp=0;
			for (int nG=1; nG<=m_numGauss; nG++)
			{
				temp += m_emMix(nG,1)*m_emGauss(ind,nG);
			}
			m_emLogLike += log(temp)*m_image(r,c);
		}
	}
}// end of logLike

void imageProClass::initEMfit(imageProClassParamDataType * initMu, imageProClassParamDataType * initCOV, imageProClassParamDataType * initMix)
{
	for(int nG=1; nG<=m_numGauss; nG++)
	{
		m_emCOM(1,nG) = initMu[nG-1];
		m_emCOM(2,nG) = initMu[nG+1];
		m_emCOV(1,2*nG-1) = initCOV[4*(nG-1)];
		m_emCOV(1,2*nG) = initCOV[4*(nG-1)+1];
		m_emCOV(2,2*nG-1) = initCOV[4*(nG-1)+2];
		m_emCOV(2,2*nG) = initCOV[4*(nG-1)+3];
		m_emMix(nG,1) = initMix[nG-1];
	}
}

void imageProClass::calcEMfitMetrics(void)
{

	for(int k=1;k<=M_MAX_ITERATIONS;k++)
	{
		m_emLogLikePrev = m_emLogLike;
		logLike();

		if(k > 2 && m_emLogLike > FLT_MIN && (m_emLogLike - m_emLogLikePrev > 0))
		{
			if (log((m_emLogLike - m_emLogLikePrev)/fabs(m_emLogLike)) < -30.0)
			{
				m_tempVec[999] = static_cast<float>(k);
				break;
			}
		}

		eStep();
		mStep();
	}
}

void imageProClass::setEllipticalRoi(imageProClassParamDataType padding)
{
	imageProClassParamDataType cosTheta = cos(m_SimpAzimuthAngle);
	imageProClassParamDataType sinTheta = sin(m_SimpAzimuthAngle);

	imageProClassParamDataType x, y;

	m_dataLoc.zeros();
	m_dataCounter=0;

	imageProClassParamDataType radX = (m_SimpBeamDiam_x/2.0*(1+padding/100.0));
	imageProClassParamDataType radY = (m_SimpBeamDiam_y/2.0*(1+padding/100.0));
	// imageProClassParamDataType radX = (m_SimpBeamDiam_x/2.0+padding);
	// imageProClassParamDataType radY = (m_SimpBeamDiam_y/2.0+padding);

	if (radX > 0 && radY > 0 && padding > 0)
	{
		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				x = (c-1-m_CMx)*cosTheta - (r-1-m_CMy)*sinTheta;
				y = (c-1-m_CMx)*sinTheta + (r-1-m_CMy)*cosTheta;
				if ((x*x/(radX)/(radX) + y*y/(radY)/(radY)) <= 1)
				{
					m_dataCounter += 1;
					m_dataLoc(1,m_dataCounter) = r-1; // start at (0,0)
					m_dataLoc(2,m_dataCounter) = c-1; // start at (0,0)
					m_dataLocMap(r,c) = 1;
					// m_imageResult(r,c) = m_image(r,c);
				}
				else
				{
					// m_imageResult(r,c) = 0;
					m_image(r,c) = 0;
					m_dataLocMap(r,c) = 0;
				}
			}
		}
	}
}

void imageProClass::initBeamPositionalStability()
{
	m_SimpBeamSSCentX = m_CMx;
	m_SimpBeamSSCentY = m_CMy;
	m_SimpBeamSSDiamX = m_SimpBeamDiam_x;
	m_SimpBeamSSDiamY = m_SimpBeamDiam_y;
	m_SimpBeamPositionalStabilityInitialized = true;
}

void imageProClass::calcBeamPositionalStability(imageProClassParamDataType beta)
{
	// difference between calculated and steady state
	m_SimpBeamPositionalStabilityCentX = m_CMx - m_SimpBeamSSCentX;
	m_SimpBeamPositionalStabilityCentY = m_CMy - m_SimpBeamSSCentY;
	m_SimpBeamPositionalStabilityDiamX = m_SimpBeamDiam_x - m_SimpBeamSSDiamX;
	m_SimpBeamPositionalStabilityDiamY = m_SimpBeamDiam_y - m_SimpBeamSSDiamY;

	// lowpass for steady state values
	m_SimpBeamSSCentX = beta*m_CMx + (1-beta)*m_SimpBeamSSCentX;
	m_SimpBeamSSCentY = beta*m_CMy + (1-beta)*m_SimpBeamSSCentY;
	m_SimpBeamSSDiamX = beta*m_SimpBeamDiam_x + (1-beta)*m_SimpBeamSSDiamX;
	m_SimpBeamSSDiamY = beta*m_SimpBeamDiam_y + (1-beta)*m_SimpBeamSSDiamY;

}

void imageProClass::calcDivergenceAngle(imageProClassParamDataType focalLength)
{
	if(focalLength > FLT_MIN)
	{
		m_SimpDivergenceAngleX = m_SimpBeamDiam_x/focalLength;
		m_SimpDivergenceAngleY = m_SimpBeamDiam_y/focalLength;
	}
	else
	{
		m_SimpDivergenceAngleX = 0;
		m_SimpDivergenceAngleY = 0;
	}
}

void imageProClass::calcCWPower(imageProClassParamDataType wattPerCount)
{
	m_SimpCWPower = wattPerCount*m_Vol;
}

void imageProClass::calcPowerDensity(int xOffset, int yOffset, int width, int height, 
				imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel)
{
	imageProClassParamDataType sum = 0;
	m_SimpPowerDensity = 0;

	if (xOffset > 0 && yOffset > 0 && xOffset+width < m_numCols && yOffset+height < m_numRows)
	{
		for(int r=yOffset; r<=yOffset+height; r++)
		{
			for(int c=xOffset; c<=xOffset+width; c++)
			{
				sum += m_image(r,c);
			}
		}
	}
	if (m_numRows > 0 && m_numCols > 0 && mmPerPixel > 0)
	{
		m_SimpPowerDensity = static_cast<double>(sum*wattPerCount/(static_cast<double>(width)*mmPerPixel*static_cast<double>(height)*mmPerPixel));
	}
}

void imageProClass::calcPowerDensityEllipse(int xCenter, int yCenter, imageProClassParamDataType radA,
				imageProClassParamDataType radB, imageProClassParamDataType angle, imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel)
{
	m_SimpPowerDensity = 0;
	if (radA > 0 && radB > 0 && mmPerPixel > 0)
	{
		m_SimpPowerDensity = calcEnergyInEllipse(xCenter, yCenter, radA, radB, angle)*wattPerCount/(4*radA*mmPerPixel*radB*mmPerPixel);
	}
}

imageProClassParamDataType imageProClass::calcEnergyInEllipse(int xCenter, int yCenter, 
				imageProClassParamDataType radA, imageProClassParamDataType radB, imageProClassParamDataType angle)
{
	imageProClassParamDataType sum = 0;
	imageProClassParamDataType x,y;
	imageProClassParamDataType cosTheta = cos(angle);
	imageProClassParamDataType sinTheta = sin(angle);

	if (radA > 0 && radB > 0)
	{
		for (int r=1; r<=m_numRows; r++)
		{
			for (int c=1; c<=m_numCols; c++)
			{
				x = (c-1-xCenter)*cosTheta - (r-1-yCenter)*sinTheta;
				y = (c-1-xCenter)*sinTheta + (r-1-yCenter)*cosTheta;
				if ((x*x/(radA)/(radA/2.0) + y*y/(radB)/(radB)) <= 1)
				{
					sum += m_image(r,c);
				}
			}
		}
	}

	return sum;
}

void imageProClass::calcMaximumPower(imageProClassParamDataType wattPerCount)
{
	m_SimpMaximumPower = m_maxRawIntens*wattPerCount;
}

void imageProClass::calcEffectiveMetrics(imageProClassParamDataType threshold, imageProClassParamDataType wattPerCount, imageProClassParamDataType mmPerPixel)
{
	imageProClassParamDataType areaAboveTenPercent = 0;
	imageProClassParamDataType areaAboveNinetyPercent = 0;
	imageProClassParamDataType sum = 0;
	int numPixels = 0;
	m_SimpEffectiveIrradiationArea = 0;
	m_SimpEffectiveAveragePower = 0;
	m_SimpFlatnessFactor = 0;
	m_SimpBeamUniformity = 0;
	m_imageHistogram.zeros();
	
	for(int r=1; r<=m_numRows; r++)
	{
		for(int c=1; c<=m_numCols; c++)
		{
			m_imageHistogram(static_cast<int>(m_image(r,c)+1),1) += 1;
			if (m_image(r,c) > 0.1*m_maxRawIntens)
			{
				areaAboveTenPercent++;
			}
			if (m_image(r,c) > threshold*m_maxRawIntens)
			{
				sum += m_image(r,c);
				numPixels++;
			}
			if (m_image(r,c) > 0.9*m_maxRawIntens)
			{
				areaAboveNinetyPercent++;
			}
		}
	}
	m_SimpEffectiveIrradiationArea = static_cast<double>(static_cast<double>(numPixels)*mmPerPixel*mmPerPixel);


	if (m_SimpEffectiveIrradiationArea > FLT_MIN)
	{
		m_SimpEffectiveAveragePower = sum*wattPerCount/static_cast<double>(numPixels);
	}
	else
	{
		m_SimpEffectiveAveragePower = 0;
	}
	if (m_SimpMaximumPower > FLT_MIN)
	{
		m_SimpFlatnessFactor = m_SimpEffectiveAveragePower/m_SimpMaximumPower;
	}
	else
	{
		m_SimpFlatnessFactor = 0;
	}
	if (areaAboveTenPercent > 0)
	{
		m_SimpEdgeSteepness = (areaAboveTenPercent - areaAboveNinetyPercent)/areaAboveTenPercent;
	}
	else 
	{
		m_SimpEdgeSteepness = 0;
	}
	if (m_SimpEffectiveAveragePower > FLT_MIN && m_SimpEffectiveIrradiationArea > FLT_MIN)
	{
		m_SimpBeamUniformity = static_cast<double>(1.0/m_SimpEffectiveAveragePower*sqrt(1/m_SimpEffectiveIrradiationArea*(m_Vol2-2*m_SimpEffectiveAveragePower*m_Vol+m_SimpEffectiveAveragePower*m_SimpEffectiveAveragePower*static_cast<double>(m_numRows)*static_cast<double>(m_numCols))));
	}
	else
	{
		m_SimpBeamUniformity = 0;
	}

	imageProClassParamDataType max = 0.0;
	imageProClassParamDataType temp = 0.0;
	imageProClassParamDataType width0 = 0;
	imageProClassParamDataType width1 = 0;
	imageProClassParamDataType prev;
	int ind = 0;
	int prevInd, maxInd;

	for (ind=1; ind<M_IMAGE_DEPTH; ind++)
	{
		temp = m_imageHistogram(ind+1,1);
		if (max < static_cast<double>(temp))
		{
			max = temp;
			maxInd = ind;
		}
	}

	for (ind=maxInd; ind>0; ind--)
	{
		temp = m_imageHistogram(ind+1,1);
		if (temp < 0.5*max)
		{
			break;
		}
		prev = temp;
		prevInd = ind;
	}	
	if (temp-prev != 0)
	{
		width0 = static_cast<double>(static_cast<double>(prevInd) + (static_cast<double>(ind)-static_cast<double>(prevInd))*(0.5*max-prev)/(temp-prev));
	}

	for (ind=maxInd; ind<M_IMAGE_DEPTH; ind++)
	{
		temp = m_imageHistogram(ind+1,1);
		if (temp < 0.5*max)
		{
			break;
		}
		prev = temp;
		prevInd = ind;
	}	
	if (temp-prev != 0)
	{
		width1 = static_cast<double>(static_cast<double>(prevInd) + (static_cast<double>(ind)-static_cast<double>(prevInd))*(0.5*max-prev)/(temp-prev));
	}
	if (m_maxRawIntens > FLT_MIN)
	{
		m_SimpPlateauUniformity = (width0+width1)/m_maxRawIntens;
	}
}

void imageProClass::calcGoodnessOfFitMetric(void)
{
	if (m_dataCounter > 0 && m_SimpRoughnessOfFit > FLT_MIN)
	{
		m_SimpGoodnessOfFit = 1.0/(1.0+m_SimpRoughnessOfFit*sqrt(static_cast<double>(m_dataCounter)));
	}
	else
	{
		m_SimpGoodnessOfFit = 0;
	}
}

void imageProClass::calcXSectionFFT(void)
{
	int r1, c1;
	int ind;
	m_xSectionMajorStruct.numberDataPoints = 0;
	m_xSectionMinorStruct.numberDataPoints = 0;
	m_xSectionMajorActStruct.numberDataPoints = 0;
	m_xSectionMinorActStruct.numberDataPoints = 0;
	int majorOffset, minorOffset;
	imageProClassParamDataType r, c;
	imageProClassParamDataType temp;

	for (int k=0; k<M_MAX_FFT_LENGTH; k++)
	{
		m_xSectionMajorStruct.realVector[k] = 0;
		m_xSectionMajorStruct.imagVector[k] = 0;
		m_xSectionMajorStruct.dataVector[k] = 0;
		m_xSectionMinorStruct.realVector[k] = 0;
		m_xSectionMinorStruct.imagVector[k] = 0;
		m_xSectionMinorStruct.dataVector[k] = 0;
		m_xSectionMajorActStruct.realVector[k] = 0;
		m_xSectionMajorActStruct.imagVector[k] = 0;
		m_xSectionMajorActStruct.dataVector[k] = 0;
		m_xSectionMinorActStruct.realVector[k] = 0;
		m_xSectionMinorActStruct.imagVector[k] = 0;
		m_xSectionMinorActStruct.dataVector[k] = 0;
	}

	for (int k=0; k<=3; k++)
	{
		ind = 0;
		temp = 0;

		for (int rad=1; rad<=M_E2_MAX_RADIUS; rad++)
		{
			r = static_cast<double>(-static_cast<double>(rad)*sin(m_SimpAzimuthAngle+static_cast<double>(k)*M_PI/2.0+M_PI) + m_CMy);
			c = static_cast<double>(static_cast<double>(rad)*cos(m_SimpAzimuthAngle+static_cast<double>(k)*M_PI/2.0+M_PI) + m_CMx);
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);
			if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
			{
				temp = interpolatePixel(r, c, r1, c1);
				if (temp <= m_borderMax/2.0)
				{
					break;
				}
				m_tempVec[ind] = static_cast<float>(temp);
				ind++;
			}
			else
			{
				break;
			}
		}

		if (k==0)
		{
			for (int j=0; j<ind; j++)
			{
				m_xSectionMajorStruct.realVector[j] = m_tempVec[ind-1-j];
				m_xSectionMajorStruct.dataVector[j] = m_tempVec[ind-1-j];
				m_xSectionMajorStruct.numberDataPoints++;
			}
			majorOffset = m_xSectionMajorStruct.numberDataPoints;
		}
		else if (k==1)
		{
			for (int j=0; j<ind; j++)
			{
				m_xSectionMinorStruct.realVector[j] = m_tempVec[ind-1-j];
				m_xSectionMinorStruct.dataVector[j] = m_tempVec[ind-1-j];
				m_xSectionMinorStruct.numberDataPoints++;
			}
			minorOffset = m_xSectionMinorStruct.numberDataPoints;
		}
		else if (k==2)
		{
			for (int j=0; j<ind; j++)
			{
				m_xSectionMajorStruct.realVector[majorOffset-1+j] = m_tempVec[j];
				m_xSectionMajorStruct.dataVector[majorOffset-1+j] = m_tempVec[j];
				m_xSectionMajorStruct.numberDataPoints++;
			}

		}
		else if (k==3)
		{
			for (int j=0; j<ind; j++)
			{
				m_xSectionMinorStruct.realVector[minorOffset-1+j] = m_tempVec[j];
				m_xSectionMinorStruct.dataVector[minorOffset-1+j] = m_tempVec[j];
				m_xSectionMinorStruct.numberDataPoints++;
			}
		}
	}
		
	if (m_xSectionMajorStruct.numberDataPoints > M_NUM_SAMPLES_FFT_LS_FIT)
	{
		runFFT(m_xSectionMajorStruct);
	}
	if (m_xSectionMinorStruct.numberDataPoints > M_NUM_SAMPLES_FFT_LS_FIT)
	{
		runFFT(m_xSectionMinorStruct);
	}

	if (m_xSectionMajorActStruct.numberUniqueFreq > M_NUM_SAMPLES_FFT_LS_FIT)
	{
		leastSquares1D(M_NUM_SAMPLES_FFT_LS_FIT, m_xSectionMajorStruct);
	}

	if (m_xSectionMinorStruct.numberUniqueFreq > M_NUM_SAMPLES_FFT_LS_FIT)
	{
		leastSquares1D(M_NUM_SAMPLES_FFT_LS_FIT, m_xSectionMinorStruct);
	}
}

void imageProClass::calcE2Width(void)
{
	int r1, c1;
	imageProClassParamDataType r, c;
	imageProClassParamDataType tempE2, prevE2, radE2, prevRadE2;
	imageProClassParamDataType temp, prev, prevRad;
	bool found = false;

	for (int k=0; k<=3; k++)
	{
		temp = 0;
		prev = 0;
		prevRad = 0;
		tempE2 = 0;
		prevE2 = 0;
		radE2 = 0;
		prevRadE2 = 0;

		for (int rad=1; rad<=M_E2_MAX_RADIUS; rad++)
		{
			r = static_cast<double>(-static_cast<double>(rad)*sin(m_SimpAzimuthAngle+static_cast<double>(k)*M_PI/2.0) + m_maxRawIndy);
			c = static_cast<double>(static_cast<double>(rad)*cos(m_SimpAzimuthAngle+static_cast<double>(k)*M_PI/2.0) + m_maxRawIndx);
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);
			if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
			{
				temp = interpolatePixel(r, c, r1, c1);
				if (temp < m_borderMax)
				{
					break;
				}
				else if (temp < M_ONE_OVER_E2*m_maxRawIntens && !found)
				{
					tempE2 = temp;
					prevE2 = prev;
					radE2 = static_cast<double>(rad);
					prevRadE2 = prevRad;
					found = true;
					break;
				}
				prev = temp;
				prevRad = static_cast<double>(rad);
			}
			else
			{
				break;
			}
		}
		if ((tempE2 - prevE2) != 0)
		{
			m_radE2[k] = static_cast<float>(prevRadE2 + (radE2-prevRadE2)*(M_ONE_OVER_E2*m_maxRawIntens-prevE2)/(tempE2-prevE2));
		}
		else
		{
			m_radE2[k] = 0;	
		}
		found = false;
	}
	m_simpleE2DiameterX = static_cast<double>(m_radE2[0]+m_radE2[2]); 
	m_simpleE2DiameterY = static_cast<double>(m_radE2[1]+m_radE2[3]); 
	if (m_simpleE2DiameterY > 0 && m_simpleE2DiameterX < m_simpleE2DiameterY)
	{
		m_simpleE2Ellipticity = m_simpleE2DiameterX/m_simpleE2DiameterY;
	}
	else if (m_simpleE2DiameterY > 0 && m_simpleE2DiameterX < m_simpleE2DiameterY)
	{
		m_simpleE2Ellipticity = m_simpleE2DiameterX/m_simpleE2DiameterY;
	}
	else
	{
		m_simpleE2Ellipticity = 0;
	}
}

void imageProClass::bitrev(CrashFloatArray &vector, unsigned int N)
{    
    unsigned int forward, rev, toggle;
    unsigned int nodd, noddrev;               // to hold bitwise negated or odd values
    unsigned int halfn, quartn, nmin1;
    float temp;
    
    halfn = N>>1;            // frequently used 'constants'    
    quartn = N>>2;
    nmin1 = N-1;

    forward = halfn;         // variable initialisations
    rev = 1;
    
    while(forward)           // start of bitreversed permutation loop, N/4 iterations
    {
     
     // adaptation of the traditional bitreverse update method

     forward -= 2;                                    
     toggle = quartn;               // reset the toggle in every iteration
     rev ^= toggle;                 // toggle one bit in reversed unconditionally
     while(rev&toggle)              // check if more bits in reversed must be toggled
     {
         toggle >>= 1;
         rev ^= toggle;                            
     }

       
        if(forward<rev)                       // swap even and ~even conditionally
        {
            temp = vector[forward];                
            vector[forward] = vector[rev];
            vector[rev] = temp;

            nodd = nmin1 ^ forward;           // compute the bitwise negations
            noddrev = nmin1 ^ rev;        
            
            temp = vector[nodd];                // swap bitwise-negated pairs
            vector[nodd] = vector[noddrev];
            vector[noddrev] = temp;
        }
        
        nodd = forward ^ 1;                   // compute the odd values from the even
        noddrev = rev ^ halfn;
        
        temp = vector[nodd];                    // swap odd unconditionally
        vector[nodd] = vector[noddrev];
        vector[noddrev] = temp;
    }    
    // end of the bitreverse permutation loop
}

void imageProClass::bitrev2(CrashFloatArray &fftVectorReal, CrashFloatArray &fftVectorImag, unsigned int N)
{    
    unsigned int forward, rev, toggle;
    unsigned int nodd, noddrev;               // to hold bitwise negated or odd values
    unsigned int halfn, quartn, nmin1;
    float temp;
    
    halfn = N>>1;            // frequently used 'constants'    
    quartn = N>>2;
    nmin1 = N-1;

    forward = halfn;         // variable initialisations
    rev = 1;
    
    while(forward)           // start of bitreversed permutation loop, N/4 iterations
    {
     
     // adaptation of the traditional bitreverse update method

     forward -= 2;                                    
     toggle = quartn;               // reset the toggle in every iteration
     rev ^= toggle;                 // toggle one bit in reversed unconditionally
     while(rev&toggle)              // check if more bits in reversed must be toggled
     {
         toggle >>= 1;
         rev ^= toggle;                            
     }

       
        if(forward<rev)                       // swap even and ~even conditionally
        {
            temp = fftVectorReal[forward];                
            fftVectorReal[forward] = fftVectorReal[rev];
            fftVectorReal[rev] = temp;

            temp = fftVectorImag[forward];                
            fftVectorImag[forward] = fftVectorImag[rev];
            fftVectorImag[rev] = temp;

            nodd = nmin1 ^ forward;           // compute the bitwise negations
            noddrev = nmin1 ^ rev;        
            
            temp = fftVectorReal[nodd];                // swap bitwise-negated pairs
            fftVectorReal[nodd] = fftVectorReal[noddrev];
            fftVectorReal[noddrev] = temp;

            temp = fftVectorImag[nodd];                // swap bitwise-negated pairs
            fftVectorImag[nodd] = fftVectorImag[noddrev];
            fftVectorImag[noddrev] = temp;
        }
        
        nodd = forward ^ 1;                   // compute the odd values from the even
        noddrev = rev ^ halfn;
        
        temp = fftVectorReal[nodd];                    // swap odd unconditionally
        fftVectorReal[nodd] = fftVectorReal[noddrev];
        fftVectorReal[noddrev] = temp;

        temp = fftVectorImag[nodd];                    // swap odd unconditionally
        fftVectorImag[nodd] = fftVectorImag[noddrev];
        fftVectorImag[noddrev] = temp;
    }    
    // end of the bitreverse permutation loop
}

void imageProClass::fft(CrashFloatArray &fftVectorReal, CrashFloatArray &fftVectorImag, unsigned int N)
{
    unsigned int n=0;
    unsigned int nspan, span, submatrix, node;
    double temp, root, angle, realtwiddle, imtwiddle;


    for(span=N>>1; span; span>>=1)
    {
       root = static_cast<double>(-M_PI/static_cast<double>(span));
       for(submatrix=0; submatrix<(N>>1)/span; submatrix++)
       {
          for(node=0; node<span; node++)
          {
            nspan = n+span;
            temp = static_cast<double>(fftVectorReal[n] + fftVectorReal[nspan]);       // additions & subtractions
            fftVectorReal[nspan] = fftVectorReal[n]-fftVectorReal[nspan];
            fftVectorReal[n] = static_cast<float>(temp);
            temp = static_cast<double>(fftVectorImag[n] + fftVectorImag[nspan]);
            fftVectorImag[nspan] = fftVectorImag[n] - fftVectorImag[nspan];
            fftVectorImag[n] = static_cast<float>(temp);
            
            angle = static_cast<double>(root*static_cast<double>(node));      // rotations
            realtwiddle = cos(angle);
            imtwiddle = sin(angle);
            temp = realtwiddle * fftVectorReal[nspan] - imtwiddle * fftVectorImag[nspan];
            fftVectorImag[nspan] = static_cast<float>(realtwiddle * fftVectorImag[nspan] + imtwiddle * fftVectorReal[nspan]);
            fftVectorReal[nspan] = static_cast<float>(temp);
            n++;
          }
          n = (n+span) & (N-1);   // jump over the odd blocks
        }
     }
} // end of FFT function

void imageProClass::runFFT(fftStruct &inputStruct)
{
	imageProClassParamDataType temp;
	imageProClassParamDataType prev = DBL_MAX;
	bool slopePositive = false;
	inputStruct.amp0 = 0;
	imageProClassParamDataType fftVectorPower2 = static_cast<double>(pow(static_cast<double>(2.0), static_cast<int>(log(static_cast<double>(inputStruct.numberDataPoints-1))/log(static_cast<double>(2.0)) + 1)));

	inputStruct.numberUniqueFreq = static_cast<int>(fftVectorPower2/2+1);

	for (int k=0; k<inputStruct.numberUniqueFreq; k++)
	{
		inputStruct.freqVector[k] = 0;
		inputStruct.ampVector[k] = 0;
	}
	fft(inputStruct.realVector, inputStruct.imagVector, static_cast<unsigned int>(fftVectorPower2));
	bitrev2(inputStruct.realVector, inputStruct.imagVector, static_cast<unsigned int>(fftVectorPower2));

	for (int k=0; k<inputStruct.numberUniqueFreq; k++)
	{
		temp = static_cast<double>(2.0*sqrt(static_cast<double>(inputStruct.realVector[k]*inputStruct.realVector[k]+inputStruct.imagVector[k]*inputStruct.imagVector[k]))/static_cast<double>(inputStruct.numberDataPoints));
		inputStruct.ampVector[k] = static_cast<float>(temp);
		inputStruct.freqVector[k] = static_cast<float>(static_cast<float>(k)/fftVectorPower2);

		if (temp > inputStruct.maxAmp)
		{
			inputStruct.maxAmp = temp;
		}

		if (slopePositive && temp-prev < 0)
		{
			slopePositive = false;
			if (prev > inputStruct.amp0)
			{
				inputStruct.amp0 = prev;
				inputStruct.freq0 = static_cast<double>(static_cast<double>(k)/fftVectorPower2);
			}
		}
		else if (!slopePositive && temp-prev > 0)
		{
			slopePositive = true;
		}
		prev = temp;
		// for(int i=0; i<M_NUM_MAX_FREQUENCIES_FFT; i++)
		// {
		// 	if (temp > fftVectorReal[i])
		// 	{
		// 		for (int j=M_NUM_MAX_FREQUENCIES_FFT-1; j>i; j--)
		// 		{
		// 			fftVectorReal[j] = fftVectorReal[j-1];
		// 			fftVectorImag[j] = fftVectorImag[j-1];
		// 		}
		// 		fftVectorReal[i] = temp;
		// 		fftVectorImag[i] = (double) k/fftVectorPower2;
		// 		break;
		// 	}
		// }
	}
}

void imageProClass::calcRThetaTopHat(int numLines, imageProClassParamDataType percent)
{
	imageProClassParamDataType tempE2 = 0;
	imageProClassParamDataType prevE2 = 0;
	imageProClassParamDataType prevRE2 = 0;
	imageProClassParamDataType radE2 = 0;
	imageProClassParamDataType tempE = 0;
	imageProClassParamDataType prevE = 0;
	imageProClassParamDataType prevRE = 0;
	imageProClassParamDataType radE = 0;
	imageProClassParamDataType tempPerc = 0;
	imageProClassParamDataType prevPerc = 0;
	imageProClassParamDataType prevRPerc = 0;
	imageProClassParamDataType radPerc = 0;
	imageProClassParamDataType temp = 0;
	imageProClassParamDataType prev = 0;
	imageProClassParamDataType prevR = 0;
	imageProClassParamDataType tempAngle = 0;
	imageProClassParamDataType r, c;
	int rad, r1, c1;

	imageProClassParamDataType maxE = 0.0;
	imageProClassParamDataType maxE2 = 0.0;
	imageProClassParamDataType maxPerc = 0.0;
	imageProClassParamDataType minE = DBL_MAX;
	imageProClassParamDataType minE2 = DBL_MAX;
	imageProClassParamDataType minPerc = DBL_MAX;
	imageProClassParamDataType sumM0E2 = 0;
	imageProClassParamDataType sumM1E2 = 0;
	imageProClassParamDataType sumM2E2 = 0;
	imageProClassParamDataType sumM0E = 0;
	imageProClassParamDataType sumM1E = 0;
	imageProClassParamDataType sumM2E = 0;
	imageProClassParamDataType sumM0Perc = 0;
	imageProClassParamDataType sumM1Perc = 0;
	imageProClassParamDataType sumM2Perc = 0;

	bool percentFound = false;
	bool E2Found = false;
	bool EFound = false;

	for (rad=1; rad<=M_E2_MAX_RADIUS; rad++)
	{
		r = static_cast<double>(-static_cast<double>(rad)*sin(m_SimpAzimuthAngle) + static_cast<double>(m_maxRawIndy));
		c = static_cast<double>(static_cast<double>(rad)*cos(m_SimpAzimuthAngle) + static_cast<double>(m_maxRawIndx));
		r1 = static_cast<int>(r);
		c1 = static_cast<int>(c);
		if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
		{
			temp = interpolatePixel(r, c, r1, c1);
			if (temp <= 0)
			{
				break;
			} 
			else if (temp < percent*m_maxRawIntens && !percentFound)
			{
				tempPerc = temp;
				prevPerc = prev;
				prevRPerc = prevR;
				radPerc = static_cast<double>(rad);
				percentFound = true;
			} 

			else if (temp < M_ONE_OVER_E*m_maxRawIntens && !EFound)
			{
				tempE = temp;
				prevE = prev;
				prevRE = prevR;
				radE = static_cast<double>(rad);
				EFound = true;
			}
			else if (temp < M_ONE_OVER_E2*m_maxRawIntens && !E2Found)
			{
				tempE2 = temp;
				prevE2 = prev;
				prevRE2 = prevR;
				radE2 = static_cast<double>(rad);
				E2Found = true;
			}
			else if (percentFound && EFound && E2Found)
			{
				break;
			}
			prev = temp;
			prevR = static_cast<double>(rad);
		}
		else
		{
			break;
		}
	}

	if ((tempE2 - prevE2) != 0)
	{
		m_rThetaHatRadiusE2 = prevRE2 + (radE2-prevRE2)*(M_ONE_OVER_E2*m_maxRawIntens-prevE2)/(tempE2-prevE2);
	}
	if ((tempE - prevE) != 0)
	{
		m_rThetaHatRadiusE = prevRE + (radE-prevRE)*(M_ONE_OVER_E*m_maxRawIntens-prevE)/(tempE-prevE);
	}
	if ((tempPerc - prevPerc) != 0)
	{
		m_rThetaHatRadiusPerc = prevRPerc + (radPerc-prevRPerc)*(percent*m_maxRawIntens-prevPerc)/(tempPerc-prevPerc);
	}

	for (int k=0; k<numLines; k++)
	{
		tempAngle = static_cast<double>(k)/static_cast<double>(numLines)*M_PI*2.0;
		r = -m_rThetaHatRadiusE2*sin(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndy;
		c = m_rThetaHatRadiusE2*cos(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndx;
		r1 = static_cast<int>(r);
		c1 = static_cast<int>(c);
		if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
		{
			m_rThetaHatE2Struct.realVector[k] = static_cast<float>(interpolatePixel(r, c, r1, c1));
			m_rThetaHatE2Struct.dataVector[k] = m_rThetaHatE2Struct.realVector[k];
			sumM0E2 += m_rThetaHatE2Struct.realVector[k];
			sumM1E2 += static_cast<double>(k)*m_rThetaHatE2Struct.realVector[k];
			sumM2E2 += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatE2Struct.realVector[k];
			m_rThetaHatE2Struct.imagVector[k] = 0;
			if (maxE2 < m_rThetaHatE2Struct.realVector[k])
			{
				maxE2 = m_rThetaHatE2Struct.realVector[k];
			}
			if (minE2 > m_rThetaHatE2Struct.realVector[k])
			{
				minE2 = m_rThetaHatE2Struct.realVector[k];
			}
		}
		else
		{
			m_rThetaHatE2Struct.dataVector[k] = 0;
			m_rThetaHatE2Struct.realVector[k] = 0;
			m_rThetaHatE2Struct.imagVector[k] = 0;
		}

		r = -m_rThetaHatRadiusE*sin(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndy;
		c = m_rThetaHatRadiusE*cos(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndx;
		r1 = static_cast<int>(r);
		c1 = static_cast<int>(c);
		if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
		{
			m_rThetaHatEStruct.realVector[k] = static_cast<float>(interpolatePixel(r, c, r1, c1));
			m_rThetaHatEStruct.dataVector[k] = m_rThetaHatEStruct.realVector[k];
			sumM0E += m_rThetaHatEStruct.realVector[k];
			sumM1E += static_cast<double>(k)*m_rThetaHatEStruct.realVector[k];
			sumM2E += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatEStruct.realVector[k];
			m_rThetaHatEStruct.imagVector[k] = 0;
			if (maxE < m_rThetaHatEStruct.realVector[k])
			{
				maxE = m_rThetaHatEStruct.realVector[k];
			}
			if (minE > m_rThetaHatEStruct.realVector[k])
			{
				minE = m_rThetaHatEStruct.realVector[k];
			}
		}
		else
		{
			m_rThetaHatEStruct.dataVector[k] = 0;
			m_rThetaHatEStruct.realVector[k] = 0;
			m_rThetaHatEStruct.imagVector[k] = 0;
		}

		r = -m_rThetaHatRadiusPerc*sin(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndy;
		c = m_rThetaHatRadiusPerc*cos(m_SimpAzimuthAngle+tempAngle) + m_maxRawIndx;
		r1 = static_cast<int>(r);
		c1 = static_cast<int>(c);
		if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
		{
			m_rThetaHatPercStruct.realVector[k] = static_cast<float>(interpolatePixel(r, c, r1, c1));
			m_rThetaHatPercStruct.dataVector[k] = m_rThetaHatPercStruct.realVector[k];
			sumM0Perc += m_rThetaHatPercStruct.realVector[k];
			sumM1Perc += static_cast<double>(k)*m_rThetaHatPercStruct.realVector[k];
			sumM2Perc += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatPercStruct.realVector[k];
			m_rThetaHatPercStruct.imagVector[k] = 0;
			if (maxPerc < m_rThetaHatPercStruct.realVector[k])
			{
				maxPerc = m_rThetaHatPercStruct.realVector[k];
			}
			if (minPerc > m_rThetaHatPercStruct.realVector[k])
			{
				minPerc = m_rThetaHatPercStruct.realVector[k];
			}
		}
		else
		{
			m_rThetaHatPercStruct.dataVector[k] = 0;
			m_rThetaHatPercStruct.realVector[k] = 0;
			m_rThetaHatPercStruct.imagVector[k] = 0;
		}
	}

	m_rThetaHatE2PkToPk = maxE2 - minE2;
	m_rThetaHatEPkToPk = maxE - minE;
	m_rThetaHatPercPkToPk = maxPerc - minPerc;
	m_rThetaHatE2Contrast = m_rThetaHatE2PkToPk/maxE2;
	m_rThetaHatEContrast = m_rThetaHatEPkToPk/maxE;
	m_rThetaHatPercContrast = m_rThetaHatPercPkToPk/maxPerc;
	m_rThetaHatE2Average = sumM0E2/static_cast<double>(numLines);
	m_rThetaHatEAverage = sumM0E/static_cast<double>(numLines);
	m_rThetaHatPercAverage = sumM0Perc/static_cast<double>(numLines);


	int power2 = 0;
	
	power2 = static_cast<int>(pow(static_cast<double>(2.0), static_cast<int>(log(static_cast<double>(numLines-1))/log(static_cast<double>(2.0)) + 1)));
	for (int k=numLines; k<power2; k++)
	{
		m_rThetaHatEStruct.dataVector[k] = 0;
		m_rThetaHatEStruct.realVector[k] = 0;
		m_rThetaHatEStruct.imagVector[k] = 0;
		m_rThetaHatE2Struct.dataVector[k] = 0;
		m_rThetaHatE2Struct.realVector[k] = 0;
		m_rThetaHatE2Struct.imagVector[k] = 0;
		m_rThetaHatPercStruct.dataVector[k] = 0;
		m_rThetaHatPercStruct.realVector[k] = 0;
		m_rThetaHatPercStruct.imagVector[k] = 0;
	}

	if (sumM0E2 > 0)
	{
		m_rThetaHatMeanE2 = sumM1E2/sumM0E2;
		m_rThetaHatStdE2 = sumM2E2/sumM0E2 - m_rThetaHatMeanE2*m_rThetaHatMeanE2;
	}
	if (sumM0E > 0)
	{
		m_rThetaHatMeanE = sumM1E/sumM0E;
		m_rThetaHatStdE = sumM2E/sumM0E - m_rThetaHatMeanE*m_rThetaHatMeanE;
	}
	if (sumM0Perc > 0)
	{
		m_rThetaHatMeanPerc = sumM1Perc/sumM0Perc;
		m_rThetaHatStdPerc = sumM2Perc/sumM0Perc - m_rThetaHatMeanPerc*m_rThetaHatMeanPerc;
	}

	// runFFT(m_rThetaHatEStruct);
	// runFFT(m_rThetaHatE2Struct);
	// runFFT(m_rThetaHatPercStruct);
	m_rThetaHatEStruct.numberDataPoints = numLines;
	m_rThetaHatE2Struct.numberDataPoints = numLines;
	m_rThetaHatPercStruct.numberDataPoints = numLines;
}

void imageProClass::calcContourMetrics(int numLines, imageProClassParamDataType percent)
{
	imageProClassParamDataType tempE2, prevE2, prevRE2, radE2;
	imageProClassParamDataType tempE, prevE, prevRE, radE;
	imageProClassParamDataType tempPerc, prevPerc, prevRPerc, radPerc;
	imageProClassParamDataType temp, prev, prevR, tempAngle;
	imageProClassParamDataType r, c;
	int rad, r1, c1;

	imageProClassParamDataType maxE = 0.0;
	imageProClassParamDataType maxE2 = 0.0;
	imageProClassParamDataType maxPerc = 0.0;
	imageProClassParamDataType minE = DBL_MAX;
	imageProClassParamDataType minE2 = DBL_MAX;
	imageProClassParamDataType minPerc = DBL_MAX;
	imageProClassParamDataType sumM0E2 = 0;
	imageProClassParamDataType sumM1E2 = 0;
	imageProClassParamDataType sumM2E2 = 0;
	imageProClassParamDataType sumM0E = 0;
	imageProClassParamDataType sumM1E = 0;
	imageProClassParamDataType sumM2E = 0;
	imageProClassParamDataType sumM0Perc = 0;
	imageProClassParamDataType sumM1Perc = 0;
	imageProClassParamDataType sumM2Perc = 0;

	bool percentFound = false;
	bool E2Found = false;
	bool EFound = false;

	// TODO - this can be done faster if we know where the last point was found. can use each consecutive point to find the next area to start searching.

	for (int k=0; k<numLines; k++)
	{
		tempPerc = 0;
		prevPerc = 0;
		prevRPerc = 0;
		radPerc = 0;
		tempE = 0;
		prevE = 0;
		prevRE = 0;
		radE = 0;
		tempE2 = 0;
		prevE2 = 0;
		prevRE2 = 0;
		radE2 = 0;
		tempAngle = static_cast<double>(k)/static_cast<double>(numLines)*M_PI*2.0;
		for (rad=1; rad<=M_E2_MAX_RADIUS; rad++)
		{
			r = static_cast<double>(-static_cast<double>(rad)*sin(m_SimpAzimuthAngle+tempAngle) + static_cast<double>(m_maxRawIndy));
			c = static_cast<double>(static_cast<double>(rad)*cos(m_SimpAzimuthAngle+tempAngle) + static_cast<double>(m_maxRawIndx));
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);
			if (r1+1+1 <= m_numRows && r1+1 > 0 && c1+1+1 <= m_numCols && c1+1 >0)
			{
				temp = interpolatePixel(r, c, r1, c1);
				if (temp <= 0)
				{
					break;
				} 
				else if (temp < percent*m_maxRawIntens && !percentFound)
				{
					tempPerc = temp;
					prevPerc = prev;
					prevRPerc = prevR;
					radPerc = static_cast<double>(rad);
					percentFound = true;
				} 

				else if (temp < M_ONE_OVER_E*m_maxRawIntens && !EFound)
				{
					tempE = temp;
					prevE = prev;
					prevRE = prevR;
					radE = static_cast<double>(rad);
					EFound = true;
				}
				else if (temp < M_ONE_OVER_E2*m_maxRawIntens && !E2Found)
				{
					tempE2 = temp;
					prevE2 = prev;
					prevRE2 = prevR;
					radE2 = static_cast<double>(rad);
					E2Found = true;
				}
				else if (percentFound && EFound && E2Found)
				{
					break;
				}
				prev = temp;
				prevR = static_cast<double>(rad);
			}
			else
			{
				break;
			}
		}
		if ((tempE2 - prevE2) != 0)
		{
			m_rThetaHatE2Struct.realVector[k] = static_cast<float>(prevRE2 + (radE2-prevRE2)*(M_ONE_OVER_E2*m_maxRawIntens-prevE2)/(tempE2-prevE2));
			m_rThetaHatE2Struct.dataVector[k] = m_rThetaHatE2Struct.realVector[k];
			sumM0E2 += m_rThetaHatE2Struct.realVector[k];
			sumM1E2 += static_cast<double>(k)*m_rThetaHatE2Struct.realVector[k];
			sumM2E2 += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatE2Struct.realVector[k];
			m_rThetaHatE2Struct.imagVector[k] = 0;
			if (maxE2 < m_rThetaHatE2Struct.realVector[k])
			{
				maxE2 = m_rThetaHatE2Struct.realVector[k];
			}
			if (minE2 > m_rThetaHatE2Struct.realVector[k])
			{
				minE2 = m_rThetaHatE2Struct.realVector[k];
			}
		}
		else
		{
			m_rThetaHatE2Struct.dataVector[k] = 0;
			m_rThetaHatE2Struct.realVector[k] = 0;
			m_rThetaHatE2Struct.imagVector[k] = 0;
		}

		if ((tempE - prevE) != 0)
		{
			m_rThetaHatEStruct.realVector[k] = static_cast<float>(prevRE + (radE-prevRE)*(M_ONE_OVER_E*m_maxRawIntens-prevE)/(tempE-prevE));
			m_rThetaHatEStruct.dataVector[k] = m_rThetaHatEStruct.realVector[k];
			sumM0E += m_rThetaHatEStruct.realVector[k];
			sumM1E += static_cast<double>(k)*m_rThetaHatEStruct.realVector[k];
			sumM2E += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatEStruct.realVector[k];
			m_rThetaHatEStruct.imagVector[k] = 0;
			if (maxE < m_rThetaHatEStruct.realVector[k])
			{
				maxE = m_rThetaHatEStruct.realVector[k];
			}
			if (minE > m_rThetaHatEStruct.realVector[k])
			{
				minE = m_rThetaHatEStruct.realVector[k];
			}
		}
		else
		{
			m_rThetaHatEStruct.dataVector[k] = 0;
			m_rThetaHatEStruct.realVector[k] = 0;
			m_rThetaHatEStruct.imagVector[k] = 0;
		}

		if ((tempPerc - prevPerc) != 0)
		{
			m_rThetaHatPercStruct.realVector[k] = static_cast<float>(prevRPerc + (radPerc-prevRPerc)*(percent*m_maxRawIntens-prevPerc)/(tempPerc-prevPerc));
			m_rThetaHatPercStruct.dataVector[k] = m_rThetaHatPercStruct.realVector[k];
			sumM0Perc += m_rThetaHatPercStruct.realVector[k];
			sumM1Perc += static_cast<double>(k)*m_rThetaHatPercStruct.realVector[k];
			sumM2Perc += static_cast<double>(k)*static_cast<double>(k)*m_rThetaHatPercStruct.realVector[k];
			m_rThetaHatPercStruct.imagVector[k] = 0;
			if (maxPerc < m_rThetaHatPercStruct.realVector[k])
			{
				maxPerc = m_rThetaHatPercStruct.realVector[k];
			}
			if (minPerc > m_rThetaHatPercStruct.realVector[k])
			{
				minPerc = m_rThetaHatPercStruct.realVector[k];
			}
		}

		percentFound = false;
		E2Found = false;
		EFound = false;

	}

	m_rThetaHatE2PkToPk = maxE2 - minE2;
	m_rThetaHatEPkToPk = maxE - minE;
	m_rThetaHatPercPkToPk = maxPerc - minPerc;
	m_rThetaHatE2Contrast = m_rThetaHatE2PkToPk/maxE2;
	m_rThetaHatEContrast = m_rThetaHatEPkToPk/maxE;
	m_rThetaHatPercContrast = m_rThetaHatPercPkToPk/maxPerc;
	m_rThetaHatE2Average = m_rThetaHatE2PkToPk/2.0 + minE2;
	m_rThetaHatEAverage = m_rThetaHatEPkToPk/2.0 + minE;
	m_rThetaHatPercAverage = m_rThetaHatPercPkToPk/2.0 + minPerc;


	int power2 = 0;

	power2 = static_cast<int>(pow(static_cast<double>(2.0), static_cast<int>(log(static_cast<double>(numLines-1))/log(static_cast<double>(2.0)) + 1)));
	for (int k=numLines; k<power2; k++)
	{
		m_rThetaHatEStruct.dataVector[k] = 0;
		m_rThetaHatEStruct.realVector[k] = 0;
		m_rThetaHatEStruct.imagVector[k] = 0;
		m_rThetaHatE2Struct.dataVector[k] = 0;
		m_rThetaHatE2Struct.realVector[k] = 0;
		m_rThetaHatE2Struct.imagVector[k] = 0;
		m_rThetaHatPercStruct.dataVector[k] = 0;
		m_rThetaHatPercStruct.realVector[k] = 0;
		m_rThetaHatPercStruct.imagVector[k] = 0;
	}

	if (sumM0E2 > 0)
	{
		m_rThetaHatMeanE2 = sumM1E2/sumM0E2;
		m_rThetaHatStdE2 = sumM2E2/sumM0E2 - m_rThetaHatMeanE2*m_rThetaHatMeanE2;
	}
	if (sumM0E > 0)
	{
		m_rThetaHatMeanE = sumM1E/sumM0E;
		m_rThetaHatStdE = sumM2E/sumM0E - m_rThetaHatMeanE*m_rThetaHatMeanE;
	}
	if (sumM0Perc > 0)
	{
		m_rThetaHatMeanPerc = sumM1Perc/sumM0Perc;
		m_rThetaHatStdPerc = sumM2Perc/sumM0Perc - m_rThetaHatMeanPerc*m_rThetaHatMeanPerc;
	}

	// runFFT(m_rThetaHatEStruct);
	// runFFT(m_rThetaHatE2Struct);
	// runFFT(m_rThetaHatPercStruct);
	m_rThetaHatEStruct.numberDataPoints = numLines;
	m_rThetaHatE2Struct.numberDataPoints = numLines;
	m_rThetaHatPercStruct.numberDataPoints = numLines;
}

void imageProClass::calcPearsonCorrelation(void)
{
	imageProClassParamDataType x,y;
	imageProClassParamDataType sumX = 0;
	imageProClassParamDataType sumXY = 0;
	imageProClassParamDataType sumY = 0;
	imageProClassParamDataType sumX2 = 0;
	imageProClassParamDataType sumY2 = 0;
	imageProClassParamDataType temp;
	imageProClassParamDataType det = m_var_xx*m_var_yy - m_var_xy*m_var_xy;
	imageProClassParamDataType invCOV00;
	imageProClassParamDataType invCOV01;
	imageProClassParamDataType invCOV11;
	imageProClassParamDataType counter = 0;
	imageProClassParamDataType deviation;
	imageProClassParamDataType max = 0;

	m_SimpPearsonCorrelation = 0;
	m_SimpRoughnessOfFit = 0;

	if (det > 0)
	{
		invCOV00 = m_var_yy/det;
		invCOV01 = -m_var_xy/det;
		invCOV11 = m_var_xx/det;
	}
	for(int r=1; r<= m_numRows; r++)
	{
		for(int c=1; c<= m_numCols; c++)
		{
			if( m_image(r,c) > 0)
			{
				x = static_cast<double>(c-1) - m_CMx;
				y = static_cast<double>(r-1) - m_CMy;

				temp = m_SimpMaxIntens*exp(-0.5*(invCOV00*x*x + 2*invCOV01*x*y + invCOV11*y*y));

				sumX += m_image(r,c);
				sumXY += m_image(r,c)*temp;
				sumY += temp;
				sumX2 += m_image(r,c)*m_image(r,c);
				sumY2 += temp*temp;
				counter++;
				deviation = temp - m_image(r,c);
				if (deviation > max)
				{
					max = deviation;
				}
			}
		}
	}

	if (counter*sumX2-sumX*sumX > 0 && counter*sumY2-sumY*sumY > 0)
	{
		m_SimpPearsonCorrelation = (counter*sumXY - sumX*sumY)/(sqrt(counter*sumX2-sumX*sumX)*sqrt(counter*sumY2-sumY*sumY));
	}

	if (m_maxRawIntens > FLT_MIN)
	{
		m_SimpRoughnessOfFit = max/m_maxRawIntens;
	}
}

void imageProClass::calcCentroidsOnly(void)
{
	m_Vol=0;
	m_M1x=0; 
	m_M1y=0;

	imageProClassParamDataType x=0;
	imageProClassParamDataType y=0;

	for(int r=1; r<= m_numRows; r++)
	{
		for(int c=1; c<= m_numCols; c++)
		{
			if( m_image(r,c) > 0)
			{
				x = static_cast<double>(c-1);
				y = static_cast<double>(r-1);

				m_Vol += m_image(r,c);
				m_M1x += x*m_image(r,c);
				m_M1y += y*m_image(r,c);
			}
		}
	}

	m_CMx = m_M1x/m_Vol;
	m_CMy = m_M1y/m_Vol;
}

void imageProClass::findMoments(void)
{
	m_Vol=0;
	m_M1x=0; 
	m_M1y=0; 
	m_M1xy=0;
	
	m_M2x=0; 
	m_M2y=0;

	m_M3x=0;
	m_M3y=0;

	m_M4x=0;
	m_M4y=0;

	m_Vol2=0;

	imageProClassParamDataType x=0;
	imageProClassParamDataType y=0;

	for(int r=1; r<= m_numRows; r++)
	{
		for(int c=1; c<= m_numCols; c++)
		{
			x = static_cast<double>(c-1);
			y = static_cast<double>(r-1);

			m_Vol += m_image(r,c);
			m_M1x += x*m_image(r,c);
			m_M1y += y*m_image(r,c);
			m_M1xy += x*y*m_image(r,c);

			m_M2x += x*x*m_image(r,c);
			m_M2y += y*y*m_image(r,c);
		
			m_M3x += x*x*x*m_image(r,c);
			m_M3y += y*y*y*m_image(r,c);

			m_M4x += x*x*x*x*m_image(r,c);
			m_M4y += y*y*y*y*m_image(r,c);

			m_Vol2 += m_image(r,c)*m_image(r,c);
		}
	}
}// end of findMoments

void imageProClass::calcSimpMetrics(void)
{
	imageProClassParamDataType cosTheta, sinTheta;

	findMoments();
	if (m_Vol >= 0 || m_Vol2 >= 0)
	{
		m_CMx = m_M1x/m_Vol;
		m_CMy = m_M1y/m_Vol;

		m_var_xy  = m_M1xy/m_Vol - m_CMx*m_CMy;
		m_var_yy = m_M2y/m_Vol - m_CMy*m_CMy;
		m_var_xx = m_M2x/m_Vol - m_CMx*m_CMx;
		m_SimpAngle = -atan2(2*m_var_xy, m_var_xx - m_var_yy)/2.0;
		m_SimpAzimuthAngle = m_SimpAngle;

		if (m_SimpAngle > -M_PI/4 && m_SimpAngle< M_PI/4)
		{

		}
		else if(m_SimpAngle >= M_PI/4 && m_SimpAngle <  3*M_PI/4)
		{
			m_SimpAngle -= M_PI/2;
		}
		else if(m_SimpAngle >= M_PI*3/4 && m_SimpAngle< M_PI)
		{
			m_SimpAngle = M_PI-m_SimpAngle;
		}
		else if(m_SimpAngle <= -M_PI/4 && m_SimpAngle > -3*M_PI/4)
		{
			m_SimpAngle += M_PI/2;
		}
		else 
		{
			m_SimpAngle = M_PI+m_SimpAngle;
		}

		m_SimpAngle = -m_SimpAngle;

		cosTheta = cos(m_SimpAzimuthAngle);
		sinTheta = sin(m_SimpAzimuthAngle);

		m_SimpVxx = cosTheta*cosTheta*m_var_xx - 2*sinTheta*cosTheta*m_var_xy + sinTheta*sinTheta*m_var_yy;
		m_SimpVyy = sinTheta*sinTheta*m_var_xx + 2*sinTheta*cosTheta*m_var_xy + cosTheta*cosTheta*m_var_yy;
		
		if (m_SimpVxx > 0)
		{
			m_SimpBeamDiam_x = 4*sqrt(m_SimpVxx);
			m_SimpBeamRad_x = m_SimpBeamDiam_x/2.0;
		}
		if (m_SimpVyy > 0)
		{
			m_SimpBeamDiam_y = 4*sqrt(m_SimpVyy);
			m_SimpBeamRad_y = m_SimpBeamDiam_y/2.0;
		}
			
		if (m_SimpBeamDiam_x < m_SimpBeamDiam_y && m_SimpBeamDiam_y > 0)
		{
			m_SimpEllip = m_SimpBeamDiam_x/m_SimpBeamDiam_y;
		}
		else if(m_SimpBeamDiam_y > 0)
		{
			m_SimpEllip = m_SimpBeamDiam_y/m_SimpBeamDiam_x;
		}
		else
		{
			m_SimpEllip = 0;
		}

		m_SimpCrossSectionalArea = M_PI*m_SimpBeamDiam_x*m_SimpBeamDiam_y/4;
		
		imageProClassParamDataType energy = 0;
		imageProClassParamDataType x=0;
		imageProClassParamDataType y=0;
		imageProClassParamDataType psiSumSqr=0;
		imageProClassParamDataType yPsiSum=0;
		imageProClassParamDataType temp = 0;
		imageProClassParamDataType ind = 0;

		for(int r=1; r<= m_numRows; r++)
		{
			for(int c=1; c<= m_numCols; c++)
			{
				y = (static_cast<double>(c-1)-m_CMx)*sinTheta + (static_cast<double>(r-1)-m_CMy)*cosTheta;
				x = (static_cast<double>(c-1)-m_CMx)*cosTheta - (static_cast<double>(r-1)-m_CMy)*sinTheta;
				if( x*x/(m_SimpVxx*4) + y*y/(m_SimpVyy*4) <= 1.0 && m_SimpVxx>0 && m_SimpVyy>0)
				{
					energy += m_image(r,c);
					psiSumSqr += exp( -x*x/m_SimpVxx - y*y/m_SimpVyy );
					yPsiSum += exp( -0.5*x*x/m_SimpVxx - 0.5*y*y/m_SimpVyy ) * ( m_image(r,c) );
				}
			}
		}

		if(psiSumSqr>0)
		{
			m_SimpMaxIntens = (yPsiSum/psiSumSqr);
		}
		else
		{
			m_SimpMaxIntens = 0;
		}

		m_SimpEnergyInEllipse = energy;
		m_SimpEnergyRatio = energy/m_Vol;

		if( m_var_xx > 0 && m_var_yy > 0 && m_Vol >= FLT_MIN)
		{
			/*
			m_skewX = ( m_M3x - 3*m_CMx*m_M2x + 3*m_CMx*m_CMx*m_M1x - m_Vol*m_CMx*m_CMx*m_CMx)/var_xx/sqrt(var_xx)/m_Vol;
			m_skewY =  (m_M3y - 3*m_CMy*m_M2y + 3*m_CMy*m_CMy*m_M1y - m_Vol*m_CMy*m_CMy*m_CMy)/var_yy/sqrt(var_yy)/m_Vol;
			m_kurtX = (m_M4x - 4*m_CMx*m_M3x + 6*m_CMx*m_CMx*m_M2x - 4*m_CMx*m_CMx*m_CMx*m_M1x + m_CMx*m_CMx*m_CMx*m_CMx*m_Vol)/var_xx/var_xx/m_Vol;
			m_kurtY = (m_M4y - 4*m_CMy*m_M3y + 6*m_CMy*m_CMy*m_M2y - 4*m_CMy*m_CMy*m_CMy*m_M1y + m_CMy*m_CMy*m_CMy*m_CMy*m_Vol)/var_yy/var_yy/m_Vol;
			*/

			m_skewX = ( m_M3x - 3*m_CMx*m_M2x + 2*m_CMx*m_CMx*m_M1x )/ (m_var_xx*sqrt(m_var_xx)*m_Vol);
			m_skewY =  (m_M3y - 3*m_CMy*m_M2y + 2*m_CMy*m_CMy*m_M1y )/ (m_var_yy*sqrt(m_var_yy)*m_Vol);
			m_kurtX = (m_M4x - 4*m_CMx*m_M3x + 6*m_CMx*m_CMx*m_M2x - 3*m_CMx*m_CMx*m_CMx*m_M1x )/(m_var_xx*m_var_xx*m_Vol);
			m_kurtY = (m_M4y - 4*m_CMy*m_M3y + 6*m_CMy*m_CMy*m_M2y - 3*m_CMy*m_CMy*m_CMy*m_M1y )/(m_var_yy*m_var_yy*m_Vol);
		}


		calcSimpPerfMetric();
	}

}// end of calcSimpMetrics

void imageProClass::correctForThresholding(imageProClassParamDataType level)
{
	imageProClassParamDataType correct = 0;

	imageProClassParamDataType cosTheta = cos(m_SimpAzimuthAngle);
	imageProClassParamDataType sinTheta = sin(m_SimpAzimuthAngle);

	if( level > M_VARIANCE_CUT )
	{
		correctVariance(m_SimpVxx, level);
		correctVariance(m_SimpVyy, level);
		if (m_SimpVxx > 0)
		{
			m_SimpBeamDiam_x = 4*sqrt(m_SimpVxx);
			m_SimpBeamRad_x = m_SimpBeamDiam_x/2.0;
		}
		if (m_SimpVyy > 0)
		{
			m_SimpBeamDiam_y = 4*sqrt(m_SimpVyy);
			m_SimpBeamRad_y = m_SimpBeamDiam_y/2.0;
		}
		m_var_xx = cosTheta*cosTheta*m_SimpVxx + sinTheta*sinTheta*m_SimpVyy;
		m_var_xy = -cosTheta*sinTheta*m_SimpVxx + cosTheta*sinTheta*m_SimpVyy;
		m_var_yy = sinTheta*sinTheta*m_SimpVxx + cosTheta*cosTheta*m_SimpVyy;
	}

	// regularly kurtosis is 3 for gaussians, however, due to the cutoff, there needs to be a correction factor
	// correction factor was found by fitting an exponential to make kurtosis equal 3 for all cutoffs for a perfect Gaussian
	if( level >  M_KURTOSIS_CUT)
	{
		// correction for thresholding
		correct = M_KURTOSIS_PARAM1*level*level*level*level + M_KURTOSIS_PARAM2*level*level*level + M_KURTOSIS_PARAM3*level*level + M_KURTOSIS_PARAM4*level + M_KURTOSIS_PARAM5;
	}
	else
	{
		correct = 1;
	}
	// L2 of skewness and kurtosis in each direction
	m_kurtX = m_kurtX*correct;
	m_kurtY = m_kurtY*correct;

	calcSimpPerfMetric();

	imageProClassParamDataType energy = 0;
	imageProClassParamDataType x=0;
	imageProClassParamDataType y=0;
	imageProClassParamDataType psiSumSqr=0;
	imageProClassParamDataType yPsiSum=0;
	imageProClassParamDataType temp = 0;
	imageProClassParamDataType ind = 0;

	for(int r=1; r<= m_numRows; r++)
	{
		for(int c=1; c<= m_numCols; c++)
		{
			y = (static_cast<double>(c-1)-m_CMx)*sinTheta + (static_cast<double>(r-1)-m_CMy)*cosTheta;
			x = (static_cast<double>(c-1)-m_CMx)*cosTheta - (static_cast<double>(r-1)-m_CMy)*sinTheta;
			if( x*x/(m_SimpVxx*4) + y*y/(m_SimpVyy*4) <= 1.0 && m_SimpVxx>0 && m_SimpVyy>0)
			{
				energy += m_image(r,c);
				psiSumSqr += exp( -x*x/m_SimpVxx - y*y/m_SimpVyy );
				yPsiSum += exp( -0.5*x*x/m_SimpVxx - 0.5*y*y/m_SimpVyy ) * ( m_image(r,c) );
			}
		}
	}

	if(psiSumSqr>0)
	{
		m_SimpMaxIntens = (yPsiSum/psiSumSqr);
	}
	else
	{
		m_SimpMaxIntens = 0;
	}

	m_SimpEnergyInEllipse = energy;
	if (m_Vol >= FLT_MIN)
	{
		m_SimpEnergyRatio = energy/m_Vol;
	}
}

void imageProClass::correctVariance(imageProClassParamDataType &var, imageProClassParamDataType level)
{
	var = M_VARIANCE_PARAM1*level*level*level*level + M_VARIANCE_PARAM2*level*level*level*var + M_VARIANCE_PARAM3*level*level*level
						+ M_VARIANCE_PARAM4*level*level*var + M_VARIANCE_PARAM5*level*level + M_VARIANCE_PARAM6*level*var + M_VARIANCE_PARAM7*level
						+ M_VARIANCE_PARAM8*var + M_VARIANCE_PARAM9;
}

void imageProClass::calcSimpPerfMetric(void)
{
	m_SimpPerfMetric = sqrt( m_skewX*m_skewX + (m_kurtX-3)*(m_kurtX-3) + m_skewY*m_skewY + (m_kurtY-3)*(m_kurtY-3));
}

void imageProClass::calcRawMetrics(void)
{
	m_maxRawIntens = 0;
	m_minRawIntens = FLT_MAX;
	m_sumRawIntens = 0;
	int ind = 1;
	int medLoc = 0;

	for(int r=1; r<= m_numRows; r++)
	{
		for(int c=1; c<= m_numCols; c++)
		{
			m_imageHistogram(static_cast<int>(m_image(r,c)+1),1) += 1;
			ind++;
			m_sumRawIntens += m_image(r,c);
			if (m_image(r,c) > m_maxRawIntens)
			{
				m_maxRawIntens = m_image(r,c);
				m_maxRawIndx = static_cast<double>(c-1);
				m_maxRawIndy = static_cast<double>(r-1);
			}
			if (m_image(r,c) < m_minRawIntens)
			{
				m_minRawIntens = m_image(r,c);
			}
		}
	}

	if (m_numRows>0 && m_numCols > 0)
	{
		m_meanRawIntens = 	m_sumRawIntens / (m_numRows*m_numCols);
	}

	int indx = m_numRows*m_numCols/2;

	
	imageProClassParamDataType sum = 0;
	for (medLoc=0; medLoc<M_IMAGE_DEPTH; medLoc++)
	{
		sum += m_imageHistogram(medLoc+1,1);
		if (sum >= static_cast<double>(indx))
		{
			m_medianRawIntens = static_cast<double>(medLoc);
			break;
		}
	}
	
}// end of calcRawMetrics

void imageProClass::heapSort(matrixClass &numbers, int array_size)
{
	int i;
	imageProClassParamDataType temp;

	for (i = static_cast<int>(array_size/2-1); i >= 0; i--)
	{
		siftDown(numbers, i, array_size);
	}

	for (i = array_size-1; i >= 1; i--)
	{
		temp = numbers(1, 1);
		numbers(1, 1) = numbers(i+1, 1);
		numbers(i+1, 1) = temp;
		siftDown(numbers, 0, i-1);
	}

}// end of heapSort

void imageProClass::siftDown(matrixClass &numbers, int start, int end)
{
	int root = start;
	int child = root * 2 + 1;
	imageProClassParamDataType temp;
    while(child < end)
	{
        if(child + 1 <= end && numbers(child+1, 1) < numbers(child+1+1, 1))
        {
            child += 1;
        }
        if(numbers(root+1, 1) < numbers(child+1, 1))
		{
			temp = numbers(root+1, 1);
			numbers(root+1, 1) = numbers(child+1, 1);
			numbers(child+1, 1) = temp;
            root = child;
		}
        else
		{
			break;
		}
		child = root * 2 + 1;
	}

} // end of siftDown

void imageProClass::partialHeapSort(matrixClass &numbers, int array_size, int index)
{
	int i;
	imageProClassParamDataType temp;
	for (i = static_cast<int>((array_size-2)/2); i >= 0; i--)
	{
		siftDown(numbers, i, array_size -1);
	}

	for (i = array_size-1; i >= index+1; i--)
	{
		temp = numbers(1, 1);
		numbers(1, 1) = numbers(i+1, 1);
		numbers(i+1, 1) = temp;
		siftDown(numbers, 0, i-1);
	}

} // end of partialHeapSort

void imageProClass::heapSort(CrashFloatArray &numbers, int array_size)
{
	int i;
	imageProClassParamDataType temp;
	for (i = static_cast<int>((array_size-2)/2); i >= 0; i--)
	{
		siftDown(numbers, i, array_size -1);
	}

	for (i = array_size-1; i >= 1; i--)
	{
		temp = numbers[0];
		numbers[0] = numbers[i];
		numbers[i] = static_cast<float>(temp);
		siftDown(numbers, 0, i-1);
	}

}// end of heapSort

void imageProClass::siftDown(CrashFloatArray &numbers, int start, int end)
{
    int root = start;
	int child = root * 2 + 1;
	imageProClassParamDataType temp;
    while(child < end)
	{
        if(child + 1 <= end && numbers[child] < numbers[child+1])
        {
            child += 1;
        }
        if(numbers[root] < numbers[child])
		{
			temp = numbers[root];
			numbers[root] = numbers[child];
			numbers[child] = static_cast<float>(temp);
            root = child;
		}
        else
		{
			break;
		}
		child = root * 2 + 1;
	}

} // end of siftDown

void imageProClass::partialHeapSort(CrashFloatArray &numbers, int array_size, int index)
{
	int i;
	imageProClassParamDataType temp;
	for (i = static_cast<int>((array_size-2)/2); i >= 0; i--)
	{
		siftDown(numbers, i, array_size -1);
	}

	for (i = array_size-1; i >= index+1; i--)
	{
		temp = numbers[0];
		numbers[0] = numbers[i];
		numbers[i] = static_cast<float>(temp);
		siftDown(numbers, 0, i-1);
	}

} // end of partialHeapSort

void imageProClass::heapSort2(CrashFloatArray &numbersIndep, CrashFloatArray &numbersDep, int array_size)
{
	int i;
	imageProClassParamDataType temp;
	for (i = static_cast<int>((array_size-2)/2); i >= 0; i--)
	{
		siftDown2(numbersIndep, numbersDep, i, array_size -1);
	}

	for (i = array_size-1; i >= 1; i--)
	{
		temp = numbersIndep[0];
		numbersIndep[0] = numbersIndep[i];
		numbersIndep[i] = static_cast<float>(temp);
		temp = numbersDep[0];
		numbersDep[0] = numbersDep[i];
		numbersDep[i] = static_cast<float>(temp);
		siftDown2(numbersIndep, numbersDep, 0, i-1);
	}

}// end of heapSort

void imageProClass::siftDown2(CrashFloatArray &numbersIndep, CrashFloatArray &numbersDep, int start, int end)
{
    int root = start;
	int child = root * 2 + 1;
	imageProClassParamDataType temp;
    while(child < end)
	{
        if(child + 1 <= end && numbersIndep[child] < numbersIndep[child+1])
        {
            child += 1;
        }
        if(numbersIndep[root] < numbersIndep[child])
		{
			temp = numbersIndep[root];
			numbersIndep[root] = numbersIndep[child];
			numbersIndep[child] = static_cast<float>(temp);
			temp = numbersDep[root];
			numbersDep[root] = numbersDep[child];
			numbersDep[child] = static_cast<float>(temp);
            root = child;
		}
        else
		{
			break;
		}
		child = root * 2 + 1;
	}

} // end of siftDown

void imageProClass::sumBorderAndStoreInArray(void)
{
	int ind = 0;
	int r,c;

	m_borderMax = 0;
	m_borderSum = 0;

	for(int k=0; k< m_borderArray.GetSize(); k++)
	{
		m_borderArray[k]=0;
	}

	r=1;
	for (c=2; c<=m_numCols-1; c++)
	{
		m_borderArray[ind] = static_cast<float>(m_image(r,c));
		m_borderSum+=m_image(r,c);
		ind++;
		if (m_image(r,c) > m_borderMax)
		{
			m_borderMax = m_image(r,c);
		}
	}
	r=m_numRows;
	for (c=2; c<=m_numCols-1; c++)
	{
		m_borderArray[ind] = static_cast<float>(m_image(r,c));
		m_borderSum+=m_image(r,c);
		ind++;
		if (m_image(r,c) > m_borderMax)
		{
			m_borderMax = m_image(r,c);
		}
	}


	c=1;
	for (r=1; r<=m_numRows; r++)
	{
		m_borderArray[ind] = static_cast<float>(m_image(r,c));
		m_borderSum+=m_image(r,c);
		ind++;
		if (m_image(r,c) > m_borderMax)
		{
			m_borderMax = m_image(r,c);
		}
	}
	c=m_numCols;
	for (r=1; r<=m_numRows; r++)
	{
		m_borderArray[ind] = static_cast<float>(m_image(r,c));
		m_borderSum+=m_image(r,c);
		ind++;
		if (m_image(r,c) > m_borderMax)
		{
			m_borderMax = m_image(r,c);
		}
	}

	m_numBorderPixels = ind;

} // end fillBorderArray

void imageProClass::removeBorder(imageProClassParamDataType level)
{
	// Call sumBorderAndStoreInArray() first

	imageProClassParamDataType temp;

	if (m_numBorderPixels>0)
	{
		temp = static_cast<double>(m_borderSum*level/static_cast<double>(m_numBorderPixels));
	}
	else
	{
		temp = 0;
	}

	m_borderLevel = temp;
	m_image -= temp;
}// end of removeBorder

void imageProClass::removeBorderMedian(void)
{
	// Call sumBorderAndStoreInArray() first

	heapSort(m_borderArray, m_borderArray.GetSize());

	m_borderLevel = m_borderArray[m_numBorderPixels/2];
	for (int r=1; r<=m_numRows; r++)
	{
		for (int c=1; c<=m_numCols; c++)
		{
			if (m_image(r,c) - m_borderLevel > 0)
			{
				m_image(r,c) = m_image(r,c) - m_borderLevel;
			}
		}
	}
	

} // end removeBorderMedian

void imageProClass::calcSymmetryMetric(int numLines, imageProClassParamDataType radMax)
{
	imageProClassParamDataType dist = 0;
	imageProClassParamDataType ang = 0;
	imageProClassParamDataType sumIQRxMed = 0;
	imageProClassParamDataType sumMed = 0;
	imageProClassParamDataType r, c;
	int r1, c1;
	int n = m_tempVec.GetSize();

	for(int i=0; i<n; i++)
	{
		m_tempVec[i] = 0;
	}
	for(int i=1; i<=numLines; i++)
	{
		dist = static_cast<double>(radMax*static_cast<double>(i)/static_cast<double>(numLines));
		for(int j=0; j<numLines; j++)
		{
			ang = static_cast<double>(2.0*M_PI*static_cast<double>(j)/static_cast<double>(numLines));
			c = dist*cos(ang) + m_CMx;
			r = dist*sin(ang) + m_CMy;
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);
			if (m_maxRawIntens>0)
			{
				m_tempVec[j] = static_cast<float>(1/m_maxRawIntens*interpolatePixel(r, c, r1, c1));
			}
		}
		heapSort(m_tempVec, numLines);
		sumMed += m_tempVec[numLines/2];
		sumIQRxMed += static_cast<double>((m_tempVec[3*numLines/4] - m_tempVec[numLines/4])*m_tempVec[numLines/2]);
	}
	if(sumMed > 0)
	{
	m_SymmetryMetric = sumIQRxMed/sumMed;
	}
} // end calcSymmetryMetric

void imageProClass::calcRadialSymmetry(int numLines, imageProClassParamDataType radMax)
{
	imageProClassParamDataType dist = 0;
	imageProClassParamDataType ang = 0;
	imageProClassParamDataType radialSumSquared = 0;
	imageProClassParamDataType r, c;
	imageProClassParamDataType temp1, temp2;
	int r1, c1;

	for(int i=1; i<=numLines; i++)
	{
		dist = static_cast<double>(radMax*static_cast<double>(i)/static_cast<double>(numLines));
		for(int j=0; j<numLines/2; j++)
		{
			ang = static_cast<double>(2.0*M_PI*static_cast<double>(j)/static_cast<double>(numLines));
			c = dist*cos(ang) + m_CMx;
			r = dist*sin(ang) + m_CMy;
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);

			temp1 = interpolatePixel(r, c, r1, c1);

			ang = static_cast<double>(2.0*M_PI*(static_cast<double>(j)+static_cast<double>(numLines)/2)/static_cast<double>(numLines));
			c = dist*cos(ang) + m_CMx;
			r = dist*sin(ang) + m_CMy;
			r1 = static_cast<int>(r);
			c1 = static_cast<int>(c);

			temp2 = interpolatePixel(r, c, r1, c1);

			radialSumSquared += (temp1-temp2)*(temp1-temp2);

		}
	}

	m_radialRMS = static_cast<double>(sqrt(2.0/static_cast<double>(numLines)/static_cast<double>(numLines)*radialSumSquared)/m_Vol);
}

void imageProClass::calcAxialSymmetry(int width, int height, int axis)
{
	imageProClassParamDataType axialSumSquared = 0.0;
	int r1, c1; 
	imageProClassParamDataType temp, temp1 = 0;
	r1 = static_cast<int>(m_CMy);
	c1 = static_cast<int>(m_CMx);
	if(axis == 0)
	{
		for(int w=1; w<=width; w++)
		{
			for(int h=1; h<=height/2; h++)
			{
				temp = interpolatePixel(m_CMx+static_cast<double>(w)-static_cast<double>(width/2), m_CMy+static_cast<double>(h), c1+w-width/2, r1+h);
				temp1 = interpolatePixel(m_CMx+static_cast<double>(w)-static_cast<double>(width/2), m_CMy-static_cast<double>(h), c1+w-width/2, r1-h);
				axialSumSquared += (temp - temp1)*(temp - temp1);

			}
		}
	}
	else if(axis == 1)
	{
		for(int h=1; h<=height; h++)
		{
			for(int w=1; w<=width/2; w++)
			{
				temp = interpolatePixel(m_CMx+static_cast<double>(w), m_CMy+static_cast<double>(h)-static_cast<double>(height/2), c1+w, r1+h-height/2);
				temp1 = interpolatePixel(m_CMx-static_cast<double>(w), m_CMy+static_cast<double>(h)-static_cast<double>(height/2), c1-w, r1+h-height/2);
				axialSumSquared += (temp - temp1)*(temp - temp1);
			
			}
		}
	}
	m_axialRMS = static_cast<double>(sqrt(2.0/static_cast<double>(width)/static_cast<double>(height)*axialSumSquared)/m_Vol);

}

imageProClassParamDataType imageProClass::interpolatePixel(imageProClassParamDataType r, imageProClassParamDataType c, int r1, int c1)
{
	imageProClassParamDataType temp = 0;
	if (r1>=0 && r1+1+1<=m_numRows && c1>=0 && c1+1+1<=m_numCols)
	{
		temp = (m_image(r1+1,c1+1)*(static_cast<double>(c1)+1-c)*(static_cast<double>(r1)+1-r)+
			m_image(r1+1,c1+1+1)*(c-static_cast<double>(c1))*(static_cast<double>(r1)+1-r)+
			m_image(r1+1+1,c1+1)*(static_cast<double>(c1)+1-c)*(r-static_cast<double>(r1))+
			m_image(r1+1+1,c1+1+1)*(c-static_cast<double>(c1))*(r-static_cast<double>(r1)));
	}
	else
	{
		temp = 0;
	}
	return temp;
}

void imageProClass::convolutionFilter(void)
{
	imageProClassParamDataType sum;
	imageProClassParamDataType convolutedSum;
	int padHeight = (m_convolutionMaskHeight - 1)/2;
	int padWidth  = (m_convolutionMaskWidth - 1)/2;;

	for (int r=1; r<=m_numRows; r++)
	{
		for (int c=1; c<=m_numCols; c++)
		{
			sum = 0;
			convolutedSum = 0;
			for (int h=-padHeight; h<=padHeight; h++)
			{
				for (int w=-padWidth; w<=padWidth; w++)
				{
					if (r+h>0 && r+h<=m_numRows && c+w>0 && c+w<=m_numCols) 
					{
						sum += m_image(r+h,c+w);
						convolutedSum += m_image(r+h,c+w)*m_convolutionMask(h+padHeight+1,w+padWidth+1);
					}
				}
			}
			m_convolutedImage(r,c) = convolutedSum;
		}
	}
}