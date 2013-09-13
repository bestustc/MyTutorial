#pragma once

#include "BigNumber.h"

class CBigInteger:
	public CBigNumber<CBigInteger>
{
	template<int PERCISION>
	friend class CBigDecimal;

	template<int RADIX>
	friend class CRadixInteger;

private:

	CBigInteger(double) { m_szValue = NULL; }

	CBigInteger(float) { m_szValue = NULL; };


public:

	CBigInteger(INT64 i);

	CBigInteger(INT32 i);

	CBigInteger(INT16 i);

	CBigInteger(INT8 i);

	CBigInteger(UINT64 i);

	CBigInteger(UINT32 i);

	CBigInteger(UINT16 i);

	CBigInteger(UINT8 i);

	CBigInteger(const CBigInteger & i);

	CBigInteger(const TCHAR * other = NULL);	

	~CBigInteger(void);

public:
	
	CBigInteger & operator = (const CBigInteger & other) ;

	CBigInteger & operator = (const TCHAR * value);

	template<typename T>
	CBigInteger & operator = (const T & other)
	{
		*this = CBigInteger(other);
		return *this;
	}

public:

	CBigInteger & Add(const CBigInteger & i);

	CBigInteger & Multi(const CBigInteger & i);

	CBigInteger & Div(const CBigInteger & i) { return DivEx(i, NULL); }

	CBigInteger & DivEx(const CBigInteger & i, CBigInteger * pResidue);

public:

	int Compare(const CBigInteger & other) const;

public:

	CBigInteger & operator %= (const CBigInteger & i);

	CBigInteger operator % (const CBigInteger & i) const;

public:

	operator INT64() const;

	operator INT32() const;

	operator INT16() const;

	operator INT8() const;

};

int PositiveIntegerCompare(const TCHAR * s0, const TCHAR * s1);