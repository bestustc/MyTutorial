#pragma once

#include "UtilHelper.h"

template<typename TFirst, typename TSecond>
class CPairEntry
{
public:

	TFirst m_first;
	TSecond m_second;	

public:

	CPairEntry(void)
	{
		m_first = DEFAULT_EX(TFirst);
		m_second = DEFAULT_EX(TSecond);
	}

	CPairEntry(const TFirst & first, const TSecond & second)
	{
		m_first = first;
		m_second = second;
	}

	CPairEntry(const CPairEntry & other)
	{
		m_first = other.m_first;
		m_second = other.m_second;
	}

	CPairEntry & operator = (const CPairEntry & other)
	{
		if(&other == this) return *this;

		m_first = other.m_first;
		m_second = other.m_second;

		return *this;
	}

	BOOL operator == (const CPairEntry & other) const
	{
		if(&other == this) return TRUE;

		return m_first == other.m_first && m_second == other.m_second;
	}

	BOOL operator != (const CPairEntry & other) const
	{
		return !( *this == (other) );
	}
};
