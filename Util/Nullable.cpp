#ifndef __UTIL__NULLABLE__H__
#define __UTIL__NULLABLE__H__

#pragma once

#include "UtilHelper.h"

template<typename T>
class CNullable
{
	T m_value;
	BOOL m_bHasValue ;

public:

	CNullable(void)
	{
		m_value = DEFAULT(T);
		m_bHasValue = FALSE;
	}

	CNullable(const T & v)
	{
		m_value = v;
		m_bHasValue = TRUE;
	}

	~CNullable(void)
	{
	}

public:

	BOOL HasValue() const
	{
		return m_bHasValue;
	}

	const T & GetValue() const
	{
		return m_value;
	}

	T & GetValue() 
	{
		return m_value;
	}

	operator const T &() const
	{
		return m_value;
	}

	operator T & () 
	{
		return m_value;
	}
};


#endif