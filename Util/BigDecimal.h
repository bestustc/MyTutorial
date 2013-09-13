#pragma once

#include "BigInteger.h"

template<int PRECISION = 100>
class CBigDecimal :
	public CBigNumber<CBigDecimal<PRECISION> >
{
	friend TCHAR * DecimalAdd(const TCHAR * src0, const TCHAR * src1, int percison);

	friend TCHAR * DecimalMulti(const TCHAR * src0, const TCHAR * src1, int percison);

	friend TCHAR * DecimalDiv(const TCHAR * src0, const TCHAR * src1, int percison);

private:

	CBigDecimal(bool) {}

public:

	CBigDecimal(const CBigDecimal & other);

	CBigDecimal(const CBigInteger & i);

	CBigDecimal(const TCHAR * value);

	CBigDecimal(double d);

	CBigDecimal(float f);

public:

	CBigDecimal & operator = (const CBigDecimal & other);

	CBigDecimal & operator = (const TCHAR * value);

	template<typename T>
	CBigDecimal & operator = (const T & value)
	{
		CBigDecimal d = CBigDecimal(value);
		*this = d.m_szValue;
		return *this;
	}

public:

	operator double () const;

	operator float () const;

	operator CBigInteger() const;

	template<int NEW_PRECISION>
	operator CBigDecimal<NEW_PRECISION>() const;

public:

	CBigDecimal & Add(const CBigDecimal & other);

	CBigDecimal & Multi(const CBigDecimal & other);

	CBigDecimal & Div(const CBigDecimal & other);

public:

	CBigDecimal OnlyDecimal() const;

public:

	size_t IntegerLength() const ;

	size_t DecimalLength() const ;

	int Compare(const CBigDecimal & other) const;
};

TCHAR * DecimalAdd(const TCHAR * src0, const TCHAR * src1, int percison);

TCHAR * DecimalMulti(const TCHAR * src0, const TCHAR * src1, int percison);

TCHAR * DecimalDiv(const TCHAR * src0, const TCHAR * src1, int percison);

int DecimalCompare(const TCHAR * src0, const TCHAR * src1, int percison);

template<int P>
CBigDecimal<P>::CBigDecimal(const CBigDecimal<P> & other)
{
	m_szValue = NULL;
	SetString(other.m_szValue);
}

template<int P>
CBigDecimal<P>::CBigDecimal(const TCHAR * value)
{
	m_szValue = NULL;
	SetString(value);
}

template<int P>
CBigDecimal<P>::CBigDecimal(double d)
{
	TCHAR t[128] ={ TZERO };

	::wsprintf(t,_T("%f"), d);
	m_szVlaue = NULL;
	SetString(t);
}

template<int P>
CBigDecimal<P>::CBigDecimal(float f)
{
	TCHAR t[128] ={ TZERO };

	::wsprintf(t,_T("%f"), f);
	m_szVlaue = NULL;
	SetString(t);
}


template<int P>
CBigDecimal<P>::CBigDecimal(const CBigInteger & i)
{
	m_szValue = NULL;
	SetString(i.m_szValue);
}

template<int P>
CBigDecimal<P> & CBigDecimal<P>::operator = (const CBigDecimal & other)
{
	if(&other == this) return *this;

	if(m_szValue != NULL) delete [] m_szValue;

	m_szValue = NULL;

	SetString(other.m_szValue);
	return *this;
}

template<int P>
CBigDecimal<P> & CBigDecimal<P>::operator = (const TCHAR * value)
{
	if(m_szValue != NULL) delete [] m_szValue;

	m_szValue = NULL;

	SetString(value);
	return *this;
}

template<int P>
size_t CBigDecimal<P>::IntegerLength() const 
{
	if(m_szValue == NULL) return 0;
	size_t i =0;
	while(m_szValue[i] != TDOT && m_szValue[i] != TZERO) ++i;
	return i;
}

template<int P>
size_t CBigDecimal<P>::DecimalLength() const
{
	size_t length = IntegerLength();

	const TCHAR  * temp = m_szValue + length;

	if(*temp[0] == TDOT) return 0;
	++temp;

	size_t i=0;

	while(temp[i] != TDOT && temp[i] != TZERO) ++i;
	return i;
}

template<int P>
CBigDecimal<P>::operator double () const
{
	if(m_szValue == NULL) return 0.0;
	TCHAR * end = NULL;
	return _tcstod(m_szValue, &end);
}

template<int P>
CBigDecimal<P>::operator float () const
{
	if(m_szValue == NULL) return 0.0;
	TCHAR * end = NULL;
	return (float)_tcstod(m_szValue, &end);
}

template<int P>
CBigDecimal<P>::operator CBigInteger() const
{
	size_t len = IntegerLength();

	TCHAR * value = new TCHAR[len +1];

	memcpy(value, m_szValue, sizeof(TCHAR)*(len+1));
	value[len] = TZERO;

	CBigInteger i(0.0);
	i.m_szValue = value;

	return i;
}

template<int P>
CBigDecimal<P> & CBigDecimal<P>::Add(const CBigDecimal<P> & other)
{
	if(ExceptAdd(other)!= NULL) return *this;

	TCHAR * value = DecimalAdd(m_szValue, other.m_szValue, P);
	if(m_szValue != NULL) delete m_szValue;
	m_szValue = value;

	return *this;
}

template<int P>
CBigDecimal<P> & CBigDecimal<P>::Multi(const CBigDecimal<P> & other)
{
	if(ExceptMulti(other)!= NULL) return *this;

	TCHAR * value = DecimalMulti(m_szValue, other.m_szValue, P);
	if(m_szValue != NULL) delete m_szValue;
	m_szValue = value;

	return *this;
}


template<int P>
CBigDecimal<P> & CBigDecimal<P>::Div(const CBigDecimal<P> & other)
{
	if(ExceptDiv(other)!= NULL) return *this;

	TCHAR * value = DecimalDiv(m_szValue, other.m_szValue, P);
	if(m_szValue != NULL) delete m_szValue;
	m_szValue = value;

	return *this;
}

template<int P>
int CBigDecimal<P>::Compare(const CBigDecimal<P> & other) const
{
	if(&other == this) return 0;

	return DecimalCompare(m_szValue, other.m_szValue, P);
}

template<int P>
CBigDecimal<P> CBigDecimal<P>::OnlyDecimal() const
{
	BOOL b = IsNegative();

	size_t integerLength = 0;
	
	while(m_szValue[integerLength] != TDOT && m_szValue[integerLength] != TZERO) ++integerLength;
	
	if(m_szValue[integerLength] == TZERO) return ZERO;

	size_t decimalLength = 0;
	while(m_szValue[integerLength + decimalLength + 1] != TZERO) ++decimalLength;

	TCHAR * value = new TCHAR[decimalLength + 4];
	memset(value, 0, sizeof(TCHAR) * (decimalLength + 4) );

	TCHAR * temp = value;
	if(IsNegative()) *temp++ == TNEGATIVE;

	*temp++ = TNUMBER0;
	*temp++ = TDOT;

	for(size_t i=0;i<decimalLength;++i)
	{
		*temp++ = m_szValue[integerLength +1 + i];
	}

	CBigDecimal b(true);
	b.m_szValue = value;
	return b;
}


template<int P>
template<int NEW_PRECISION>
CBigDecimal<P>::operator CBigDecimal<NEW_PRECISION>() const
{
	size_t integerLength = 0;
	
	while(m_szValue[integerLength] != TDOT && m_szValue[integerLength] != TZERO) ++integerLength;

	size_t decimalLength = 0;
	while(m_szValue[integerLength + decimalLength + 1] != TZERO) ++decimalLength;

	if(decimalLength < NEW_PRECISION)
	{
		return *this;
	}

	size_t newLength = integerLength + 1 + NEW_PRECISION;

	TCHAR * value = new TCHAR[newLength +1];
	memset(value,0,sizeof(TCHAR) *(newLength+1) );

	for(size_t i=0;i<newLength;++i)
	{
		value[i] = m_szValue[i];
	}
	
	CBigDecimal<NEW_PRECISION> b(true);
	b.m_szValue = value;
	return b;

}