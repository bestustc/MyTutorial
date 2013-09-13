#pragma once

#include "BigInteger.h"

template<unsigned int RADIX = UINT_MAX -1>
class CRadixInteger
{
	unsigned int * m_value;

	size_t m_nSize;

	enum
	{
		MODE_ZERO,

		MODE_NAN,

		MODE_POSITIVE_INFINITE,

		MODE_NEGATIVE_INFINITE,

		MODE_INCERTITUDE,

		MODE_POSITIVE,

		MODE_NEGATIVE,

	} m_nMode;


public:

	CRadixInteger(void)
	{		
	}

	~CRadixInteger(void)
	{
	}

	CRadixInteger(const CBigInteger & i)
	{
		if(CBigInteger::IsNan())
		{
			m_nMode = MODE_NAN;
			return;
		}
		else if(CBigInteger::IsIncertitude())
		{
			m_nMode = MODE_INCERTITUDE;
			return;

		}
		else if(CBigInteger::IsNegativeInfinify())
		{
			m_nMode = MODE_NEGATIVE_INFINITE;
			return;

		}
		else if(CBigInteger::IsPositiveInfinify())
		{
			m_nMode = MODE_POSITIVE_INFINITE;
			return;

		}
		else if(CBigInteger::IsPositive())
		{
			m_nMode = MODE_POSITIVE;
		}
		else if(CBigInteger::IsNegative())
		{
			m_nMode = MODE_NEGATIVE;

		}

		CBigInteger radix = RADIX;
		CBigInteger total = i;

		if(m_nMode == MODE_NEGATIVE) total = -total;		

		CBigInteger residue = 0;

		vector<unsigned int> vec;

		while(total)
		{
			total.Div(radix, &residue);

			vec.push_back(residue);
		}

		while( vec[vec.size()-1] == 0 )
		{
			vec.pop_back();
		}

		m_nSize = vec.size();

		m_value = new unsigned int[m_nSize];
		memset(m_value,0,sizeof(unsigned int)*m_nSize);

		for(size_t i=0;i<m_nSize;++i)
		{
			m_value[i] = vec[m_nSize-i-1];
		}

	}

	CBigInteger ToBigInteger() const
	{
		CBigInteger total = 0;

		CBigInteger radix = RADIX;
		
		for(int i=0;i<m_nSize;++i)
		{
			CBigInteger v = m_value[i];

			total *= radix;
			total+=v;
		}

		return total;
	}

};
