#include "matrixClass.h"
#include "float.h"

matrixClass::matrixClass():
	m_initialized(false)
{
/******************************************************************************
*
*   Function Name: matrixClass (constructor)
*
*   Description: 
*
******************************************************************************/
    // create a Matrix object without content
    data = NULL;
    m_numRows = 0;
    m_numCols = 0;
	
} // end of constructor


matrixClass::matrixClass(matrixClassConstDataType nrows, matrixClassConstDataType ncols):
	m_initialized(false)
{
/******************************************************************************
*
*   Function Name: matrixClass (constructor)
*
*   Description: 
*
******************************************************************************/
    // create a Matrix object without content
	data = NULL;
	m_numRows = 0;
	m_numCols = 0;
    setSize(nrows,ncols);

} // end of constructor

void matrixClass::setSize(matrixClassConstDataType nrows, matrixClassConstDataType ncols)
{
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
    // create a Matrix object without content
    
	if (m_initialized == false)
	{
		if (data != NULL)
		{
			for (int r = 0; r < m_numRows; r++)
		    {
		      delete [] data[r];
		    }
		    delete [] data;
			data = NULL;
		}

		// create a Matrix object with given number of rows and columns

		if (nrows > 0 && ncols > 0)
		{
		  m_numRows = nrows;
		  m_numCols = ncols;

		  data = new matrixClassParamDataType*[m_numRows];
		  for (int r = 0; r < m_numRows; r++)
		  {
			data[r] = new matrixClassParamDataType[m_numCols];

			// initially fill in zeros for all values in the matrix;
			for (int c = 0; c < m_numCols; c++)
			{
			  data[r][c] = 0;
			}
		  }
		}
	}
 
} // end of setSize()

matrixClass::~matrixClass()
{
	 // clean up allocated memory
	if (data != NULL)
	{
	    for (int r = 0; r < m_numRows; r++)
	    {
	      delete [] data[r];
	    }
	    delete [] data;
	    data = NULL;
	}
}  // end of destructor

CrashResult matrixClass::Initialize()
{
	if (m_initialized==false)
	{
		m_initialized = true;
		return CrashBaseObject::Initialize();
	}
	else
	{
		return ALREADY_INITIALIZED;
	}

} // end of Initialize


matrixClassParamDataType matrixClass::det (void){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	
  matrixClassParamDataType detm = 0;    // value of the determinant
  matrixClassConstDataType rows = m_numRows;
  matrixClassConstDataType cols = m_numCols;

  if (rows == cols)
  {
		// this is a square matrix
		if (rows == 1)
		{
		  // this is a 1 x 1 matrix
		  detm = data[0][0];
		}
		else if (rows == 2)
		{
		  // this is a 2 x 2 matrix
		  // the determinant of [a11,a12;a21,a22] is det = a11*a22-a21*a12
		  detm = data[0][0] * data[1][1] - data[1][0] * data[0][1];
		}
		else if(rows==3)
		{
		 matrixClassParamDataType a, b, c, d, e, f, g, h, i;
		 a = data[0][0];
		 b = data[0][1];
		 c = data[0][2];
		 d = data[1][0];
		 e = data[1][1];
		 f = data[1][2];
		 g = data[2][0];
		 h = data[2][1];
		 i = data[2][2];

		 detm = (a*e*i+b*f*g+c*d*h)-(c*e*g+b*d*i+a*f*h);
		}
		else
		{
		  // "Not calculated for larger than 3x3"
		}
  }
  else
  {
    // "Matrix must be square"
  }
  return detm;

}

void matrixClass::diag(matrixClassParamDataType d){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	
	if (m_numRows==m_numCols)
	{
		for (matrixClassConstDataType r=0; r<m_numRows; r++)
		{
			for (matrixClassConstDataType c=0; c<m_numCols; c++)
			{
				if (r==c)
				{
					data[r][c] = d;
				}
				else
				{
					data[r][c] = 0;
				}
			}
		}
	}
	else
	{
		// "matrix is not square"
	}

}

void matrixClass::eye(void){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	diag(static_cast<matrixClassParamDataType>(1.0));

} // end of eye()

void matrixClass::zeros(void){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	for (matrixClassConstDataType r=0; r<m_numRows; r++)
	{
		for (matrixClassConstDataType c=0; c<m_numCols; c++)
		{
			data[r][c] = 0;
		}
	}

}// end of zeros()

void matrixClass::ones(void){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	for (matrixClassConstDataType r=0; r<m_numRows; r++)
	{
		for (matrixClassConstDataType c=0; c<m_numCols; c++)
		{
			data[r][c] = 1;
		}
	}

}// end of ones()

matrixClassParamDataType matrixClass::trace(void){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
	matrixClassParamDataType result=0;
	if (m_numRows==m_numCols && m_numRows>0 && data!=NULL)
	{
		for (matrixClassConstDataType r=0; r<m_numRows; r++)
		{
			result+=data[r][r];
		}
	}
	else
	{
		// "matrix not square"
	}

	return result;
}// end of trace()

void mcMult(matrixClass& result, matrixClass& left, matrixClass& right)
{
	result.zeros();
	matrixClassConstDataType rows		= left.getNumRows();
	matrixClassConstDataType cols		= right.getNumCols();
	matrixClassConstDataType leftCols	= left.getNumCols();
	matrixClassConstDataType rightRows  = right.getNumRows();

	if (leftCols > 0 && leftCols==rightRows)
	{
		for (int r=0; r < rows; r++)
		{
			for (int c=0; c < cols; c++)
			{
				for(int k=0; k < leftCols; k++)
				{
					result(r+1,c+1) += left(r+1,k+1)*right(k+1,c+1);
				}
			}
		}
	}
	else
	{
		// "dimensions not compatible"
	}
}

void mcInv(matrixClass& res, matrixClass& Acopy){
/******************************************************************************
*
*   Function Name: 
*
*   Description: 
*
******************************************************************************/
matrixClassConstDataType resRows = res.getNumRows();
matrixClassConstDataType resCols = res.getNumCols();

matrixClassConstDataType aiRows = Acopy.getNumRows();
matrixClassConstDataType aiCols = Acopy.getNumCols();

matrixClassConstDataType numRows=0;
matrixClassConstDataType numCols=0;

if (resRows !=aiRows || resCols!=aiCols)
{
	// "incorrect dimensions"
}
else
{
	numRows = aiRows;
	numCols = aiCols;
	if (numRows!=numCols)
	{
		// "matrix must be square"
	}
}


res.eye();
  
matrixClassParamDataType temp=0;   // used to swap rows
matrixClassParamDataType detm=0;      // for det()
 
// this is a square matrix
if (numRows == 1)
{
	// this is a 1 x 1 matrix
	if (Acopy(1,1)!=0)
	{
		res(1,1) = 1 / Acopy(1,1);
	}
	else
	{
		// "not invertible"
	}
}
else if (numRows == 2)
{
	// this is a 2 x 2 matrix
	detm = Acopy.det();
	// Note:: change 0 to epsilon for which below the matrix is invertible
	if (detm > 0 || detm < 0)
	{
		res(1,1) = Acopy(2,2)/detm;
		res(1,2) = -Acopy(1,2)/detm;
		res(2,1) = -Acopy(2,1)/detm;
		res(2,2) = Acopy(2,2)/detm;
	}
	else
	{
		// "not invertible"
	}
}
else if (numRows == 3)
{
	// this is a 3 x 3 matrix
	detm = Acopy.det();
	matrixClassParamDataType a, b, c, d, e, f, g, h, i;
	// Note:: change 0 to epsilon for which below the matrix is invertible
	if (detm > 0 || detm < 0)
	{
		a = Acopy(1,1);
		b = Acopy(1,2);
		c = Acopy(1,3);
		d = Acopy(2,1);
		e = Acopy(2,2);
		f = Acopy(2,3);
		g = Acopy(3,1);
		h = Acopy(3,2);
		i = Acopy(3,3);

	res(1,1) = (e*i-f*h)/detm;
	res(1,2) = -(b*i-c*h)/detm;
	res(1,3) = (b*f-c*e)/detm;
	res(2,1) = -(d*i-f*g)/detm;
	res(2,2) = (a*i-c*g)/detm;
	res(2,3) = -(a*f-c*d)/detm;
	res(3,1) = (d*h-e*g)/detm;
	res(3,2) = -(a*h-b*g)/detm;
	res(3,3) = (a*e-b*d)/detm;
	}
	else
	{
		// "not invertible"
	}
}
else
{
	// this is a matrix of 4 x 4 or larger
	// calculate inverse using gauss-jordan elimination
	res.eye();   // a diagonal matrix with ones at the diagonal
	
	matrixClassConstDataType r;
	for (matrixClassConstDataType c = 0; c < numCols; c++)
	{
		// element (c, c) should be non zero. if not, swap content
		// of lower rows
		for (r = c; r < numRows && Acopy(r+1,c+1) == 0; r++)
		{
		}
		if (r != c)
		{
			// swap rows
			
			for (matrixClassConstDataType s = 0; s < numCols; s++)
			{
				//Swap(Acopy(c, s), Acopy(r, s));
				temp=0;
				temp = Acopy(c+1,s+1);
				Acopy(c+1,s+1) = Acopy(r+1,s+1);
				Acopy(r+1,s+1) = temp;

				//Swap(res(c, s), res(r, s));
				temp=0;
				temp = res(c+1,s+1);
				res(c+1,s+1) = res(r+1,s+1);
				res(r+1,s+1) = temp;

			}
		}

	// eliminate non-zero values on the other rows at column c
		for (r = 0; r < numRows; r++)
		{
			if(r != c)
			{
			// eleminate value at column c and row r
			// Note:: change 0 to epsilon for which below the matrix is invertible
			if (Acopy(r+1,c+1) > 0 || Acopy(r+1,c+1) < 0)
			{
				matrixClassParamDataType f = - Acopy(r+1,c+1) / Acopy(c+1,c+1);

				// add (f * row c) to row r to eleminate the value
				// at column c
				for (matrixClassConstDataType s = 0; s < numCols; s++)
				{
				Acopy(r+1,s+1) += f * Acopy(c+1,s+1);
				res(r+1,s+1) += f * res(c+1,s+1);
				}
			}
			}
			else
			{
			// make value at (c, c) one,
			// divide each value on row r with the value at Acopy(c,c)
				matrixClassParamDataType f = Acopy(c+1,c+1);
				if (f>=FLT_MIN)
				{
					for (matrixClassConstDataType s = 0; s < numCols; s++)
					{
						Acopy(r+1,s+1) /= f;
						res(r+1,s+1) /= f;
					}
				}
				else
				{
					// "not invertible"
				}
			}
		}
	}
}

}


void mcAdd(matrixClass& res,matrixClass& A,matrixClass& B)
{
	if (A.getNumRows()==B.getNumRows() && A.getNumCols()==B.getNumCols() && A.getNumRows()==res.getNumRows() && A.getNumCols()==res.getNumCols())
	{
		res.zeros();

		for (int r=1; r<=A.getNumRows(); r++)
		{
			for (int c=1; c<=A.getNumCols(); c++)
			{
				res(r,c) = A(r,c)+B(r,c);
			}
		}
	}
	else
	{
		// "incorrect dimension"
	}
} // end of mcAdd()

void mcSub(matrixClass& res,matrixClass& A,matrixClass& B)
{
	if (A.getNumRows()==B.getNumRows() && A.getNumCols()==B.getNumCols() && A.getNumRows()==res.getNumRows() && A.getNumCols()==res.getNumCols())
	{
		res.zeros();

		for (int r=1; r<=A.getNumRows(); r++)
		{
			for (int c=1; c<=A.getNumCols(); c++)
			{
				res(r,c) = A(r,c)-B(r,c);
			}
		}
	}
	else
	{
		// "incorrect dimension"
	}
} // end of mcSub

matrixClassParamDataType& matrixClass::operator()(matrixClassConstDataType r, matrixClassConstDataType c)
{
	if (data != NULL && r > 0 && r <= m_numRows && c > 0 && c <= m_numCols)
	{
	  return data[r-1][c-1];
	}
	else
	{
	  // "Subscript out of range"
	  return data[0][0];
	}
}

matrixClassParamDataType matrixClass::operator()(matrixClassConstDataType r, matrixClassConstDataType c) const
{
	if (data != NULL && r > 0 && r <= m_numRows && c > 0 && c <= m_numCols)
	{
	  return data[r-1][c-1];
	}
	else
	{
	  // "Subscript out of range"
	  return data[0][0];
	}
}
