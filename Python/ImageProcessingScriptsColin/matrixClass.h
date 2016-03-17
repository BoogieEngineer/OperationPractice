
/******************************************************************************
*
*   File Name:  
*
*   Description:  
*                
*
******************************************************************************/
#ifndef __ALGOLIB_MATRIXCLASS
#define __ALGOLIB_MATRIXCLASS

#include "CrashBaseObject.h"

typedef double matrixClassParamDataType;
typedef int matrixClassConstDataType;

class matrixClass;
void mcMult(matrixClass& result, matrixClass& left, matrixClass& right);
void mcInv(matrixClass& result, matrixClass& Acopy);
void mcAdd(matrixClass& res,matrixClass& A,matrixClass& B);
void mcSub(matrixClass& res,matrixClass& A,matrixClass& B);
//template<int row, int col> matrixClass mcTempAdd (matrixClass& A,matrixClass& B);
//matrixClass& operator+ (matrixClass& A, matrixClass& B);

class matrixClass: public CrashBaseObject
{

	public: 

		matrixClass(void);
		matrixClass(matrixClassConstDataType nrows, matrixClassConstDataType ncols);
		virtual ~matrixClass(void);//

		// override crash base methods
		virtual CrashResult Initialize();

		// application specific
		void zeros(void);
		void eye(void);
		void ones(void);
		void diag(matrixClassParamDataType d);
		matrixClassParamDataType det(void);
		matrixClassParamDataType trace(void);
		matrixClassConstDataType getNumRows(void){ return m_numRows;};
		matrixClassConstDataType getNumCols(void){ return m_numCols;};
		void setSize(matrixClassConstDataType nrows, matrixClassConstDataType ncols);
		// might use in the future, void setPrt(matrixClassParamDataType * ptr){data = &ptr;};
		// might use in the future, void setPrt(matrixClassParamDataType * ptr){memcpy(data, prt, m_numRows*m_numCols*sizeof*ptr);};

		void operator+=(matrixClass& A)
		{
			if(m_numRows==A.getNumRows() && m_numCols==A.getNumCols() && m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=A.getNumRows(); r++)
				{
					for (int c=1; c<=A.getNumCols(); c++)
					{
						data[r-1][c-1] += A(r,c);
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator *=(matrixClassParamDataType& a)
		{
			if(m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=m_numRows ; r++)
				{
					for (int c=1; c<=m_numCols ; c++)
					{
						data[r-1][c-1] = data[r-1][c-1]*a;
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator /=(matrixClassParamDataType& a)
		{
			if(m_numRows>0 && m_numCols>0 && data!=NULL && a>0)
			{
				for (int r=1; r<=m_numRows ; r++)
				{
					for (int c=1; c<=m_numCols ; c++)
					{
						data[r-1][c-1] = data[r-1][c-1]/a;
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator +=(matrixClassParamDataType& a)
		{
			if(m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=m_numRows ; r++)
				{
					for (int c=1; c<=m_numCols ; c++)
					{
						data[r-1][c-1] = data[r-1][c-1]+a;
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator -=(matrixClassParamDataType& a)
		{
			if(m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=m_numRows ; r++)
				{
					for (int c=1; c<=m_numCols ; c++)
					{
						data[r-1][c-1] = data[r-1][c-1]-a;
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator-=(matrixClass& A)
		{
			if(m_numRows==A.getNumRows() && m_numCols==A.getNumCols() && m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=A.getNumRows(); r++)
				{
					for (int c=1; c<=A.getNumCols(); c++)
					{
						data[r-1][c-1] -= A(r,c);
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		void operator=(matrixClass& A)
		{
			if(m_numRows==A.getNumRows() && m_numCols==A.getNumCols() && m_numRows>0 && m_numCols>0 && data!=NULL)
			{
				for (int r=1; r<=A.getNumRows(); r++)
				{
					for (int c=1; c<=A.getNumCols(); c++)
					{
						data[r-1][c-1] = A(r,c);
					}
				}


			}
			else
			{
				// "incorrect dimension for equals"
			}

		}

		// access with 1 as starting index... matlab notation :)
		matrixClassParamDataType& operator()(matrixClassConstDataType r, matrixClassConstDataType c);
		matrixClassParamDataType operator()(matrixClassConstDataType r, matrixClassConstDataType c) const;
	
		matrixClassParamDataType get(const matrixClassConstDataType r, const matrixClassConstDataType c) 
		  {
			if (data != NULL && r > 0 && r <= m_numRows && c > 0 && c <= m_numCols)
			{
			  return data[r-1][c-1];
			}
			else
			{
			  // "Subscript out of range"
			}
		  }

		void set(const matrixClassConstDataType r, const matrixClassConstDataType c, matrixClassParamDataType value) 
		  {
			if (data != NULL && r > 0 && r <= m_numRows && c > 0 && c <= m_numCols)
			{
			  data[r-1][c-1]=value;
			}
			else
			{
			  // "Subscript out of range"
			}
		  }

	protected:
		bool m_initialized;
        matrixClassConstDataType m_numRows;
		matrixClassConstDataType m_numCols;
		matrixClassParamDataType ** data;

	private:
		void Swap(matrixClassParamDataType& a, matrixClassParamDataType& b)
		{
		  matrixClassParamDataType temp = a;
		  a = b;
		  b = temp;
		}
 
};



#endif // __ALGOLIB_MATRIXCLASS
