#pragma once

#include "UtilHelper.h"

template<typename T>
class CBigNumber
{
protected:

	TCHAR * m_szValue;	

public:

	const static T NAN;

	const static T POSITIVE_INFINITE;

	const static T NEGATIVE_INFINITE;

	const static T ZERO;

	const static T ONE;

	const static T INCERTITUDE;

protected:

	CBigNumber(void)
	{
		m_szValue = NULL;
	}

	~CBigNumber()
	{
		if(m_szValue != NULL) 
		{
			delete [] m_szValue;
			m_szValue = NULL;
		}
	}

protected:

	void SetString(const TCHAR * value);

public:

	virtual T & Add(const T & t) = 0;

	virtual T & Multi(const T & t) = 0;

	virtual T & Div(const T & t) = 0;

public:

	bool IsPositive() const { return IsNegative() && !IsZero() ;}

	bool IsNegative() const { return  m_szValue[0] == TNEGATIVE ; }

	bool IsZero() const { return m_szValue[0] == TNUMBER0 ;}

	bool IsNan() const { return m_szValue[0] == 1; }	

	bool IsPositiveInfinify() const { return m_szValue[0] == 2; }

	bool IsNegativeInfinify() const { return m_szValue[0] == 3; }

	bool IsInfinify() const { return IsNegativeInfinify() && IsPositiveInfinify(); }

	bool IsIncertitude() const { return m_szValue[0] == 4; }

	size_t Length() const { return STRLEN(m_szValue); }

public:

	const TCHAR * ToString() const { return m_szValue; }

	TCHAR operator[](size_t index) const { return m_szValue[index]; }

public:

	T operator - () const;

	operator bool() const { return !IsZero();}

	bool operator !() const { return IsZero();}

	virtual int Compare(const T & other) const = 0;

	bool operator > (const T & other) const { return Compare(other) > 0;}

	bool operator < (const T & other) const { return Compare(other) < 0;}

	bool operator >= (const T & other) const { return Compare(other) >= 0;}

	bool operator <= (const T & other) const { return Compare(other) <= 0;}

	bool operator == (const T & other) const { return Compare(other) == 0;}

	bool operator != (const T & other) const { return Compare(other) != 0;}

public:

	T & operator += (const T & t) { return  Add(t); }

	T operator + (const T & t) const;

	T & operator -= (const T & t) { return  Add(-t); }

	T operator - (const T & t) const { return *this + (-t) ; }

	T & operator *= (const T & t) { return Multi(t);}

	T operator * (const T & t) const;

	T & operator /= (const T & t) { return Div(t);}

	T operator / (const T & t) const;

public:

	T & operator ++ () ;

	T operator ++ (int) ;

protected:

	T * ExceptAdd(const T & other);

	T * ExceptMulti(const T & other);

	T * ExceptDiv(const T & other);
};

template<typename T>
const T CBigNumber<T>::NAN = _T("\1");

template<typename T>
const T CBigNumber<T>::POSITIVE_INFINITE= _T("\2");

template<typename T>
const T CBigNumber<T>::NEGATIVE_INFINITE= _T("\3");

template<typename T>
const T CBigNumber<T>::INCERTITUDE= _T("\4");

template<typename T>
const T CBigNumber<T>::ZERO= _T("0");

template<typename T>
const T CBigNumber<T>::ONE= _T("1");


template<typename T>
void CBigNumber<T>::SetString(const TCHAR * value)
{
	if(m_szValue != NULL) delete [] m_szValue;

	size_t size = STRLEN(value);

	m_szValue = new TCHAR[ size+1];
	_tcscpy_s(m_szValue, size+1, value);
}

template<typename T>
T CBigNumber<T>::operator - () const
{
	if(IsZero() || IsNan()) return static_cast<const T &>(*this);

	if(IsNegativeInfinify())
	{
		return POSITIVE_INFINITE;
	}
	else if(IsPositiveInfinify())
	{
		return NEGATIVE_INFINITE;
	}
	else if(IsNegative())
	{
		return T(m_szValue + 1);
	}
	else
	{
		size_t length = Length();

		TCHAR * p = new TCHAR[ length +2];

		_tcscpy_s(p+1, length+1, m_szValue);
		p[0] = TNUMBER0;

		CBigInteger temp = p;
		delete [] p;
		return temp;
	}
}

template<typename T>
T CBigNumber<T>::operator + (const T & t) const
{
	T t2 = t;
	t2 += static_cast<T &>(*this);

	return t2;
}

template<typename T>
T CBigNumber<T>::operator * (const T & t) const
{
	T t2 = t;
	t2 *= static_cast<T &>(*this);
	return t2;
}

template<typename T>
T CBigNumber<T>::operator / (const T &t) const
{
	T t2 = static_cast<T &>(*this);;
	t2 /= t;

	return t2;
}

template<typename T>
T * CBigNumber<T>::ExceptAdd(const T & other)
{
	if(IsNan() || other.IsZero()) return static_cast<T *>(this);

	if(other.IsNan())
	{
		static_cast<T &>(*this) = NAN;
		return static_cast<T *>(this);
	}

	if(IsPositiveInfinify() && other.IsNegativeInfinify()
		|| IsNegativeInfinify() && other.IsPositiveInfinify())
	{
		static_cast<T &>(*this) = INCERTITUDE;
		return static_cast<T *>(this);
	}

	if(IsPositiveInfinify() || IsNegativeInfinify()) return static_cast<T *>(this);

	if(other.IsPositiveInfinify() || other.IsNegativeInfinify())
	{
		static_cast<T &>(*this) = other;
		return static_cast<T *>(this);
	}

	if(IsZero())
	{
		static_cast<T &>(*this) = other;
		return static_cast<T *>(this);
	}

	return NULL;
}

template<typename T>
T * CBigNumber<T>::ExceptMulti(const T & other)
{
	if(IsNan()) return static_cast<T *>(this);

	if(other.IsNan())
	{
		static_cast<T &>(*this) = NAN;
		return static_cast<T *>(this);
	}

	if(IsZero() && other.IsInfinify()
		|| IsInfinify() && other.IsNan())
	{
		static_cast<T &>(*this) = INCERTITUDE;
		return static_cast<T *>(this);
	}

	if(IsZero()) return static_cast<T *>(this);

	if(other.IsZero()) 
	{
		static_cast<T &>(*this)= ZERO;
		return static_cast<T *>(this);
	}

	if(IsInfinify() && other.IsPositiveInfinify())
	{
		return static_cast<T *>(this);
	}

	if(IsNegativeInfinify() && other.IsNegativeInfinify())
	{
		static_cast<T &>(*this) = POSITIVE_INFINITE;
		return static_cast<T *>(this);
	}

	if(IsPositiveInfinify() && other.IsNegativeInfinify())
	{
		static_cast<T &>(*this) = NEGATIVE_INFINITE;
		return static_cast<T *>(this);
	}

	return NULL;
}

template<typename T>
T * CBigNumber<T>::ExceptDiv(const T & other)
{
	if(IsNan()) return static_cast<T *>(this);

	if(other.IsNan() || other.IsZero())
	{
		static_cast<T &>(*this) = NAN;
		return static_cast<T *>(this);
	}

	if(IsInfinify() && other.IsInfinify())
	{
		static_cast<T &>(*this) = INCERTITUDE;
		return static_cast<T *>(this);
	}

	if(other.IsInfinify())
	{
		static_cast<T &>(*this) = ZERO;
		return static_cast<T *>(this);
	}

	if(IsZero() || IsInfinify()) return static_cast<T *>(this);

	return NULL;
}

template<typename T>
T & CBigNumber<T>::operator ++ ()
{
	return Add(ONE);
}

template<typename T>
T  CBigNumber<T>::operator ++ (int) 
{
	T t = this->m_szValue;
	Add(ONE);

	return t;
}