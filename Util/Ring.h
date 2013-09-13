#ifndef __UTIL__RING__H__
#define __UTIL__RING__H__

#pragma once

#include "Collection.h"

template<typename T, typename TCopy =CopyT<T> >
class CRingT
{
	T * m_objects;

	int m_nFirstIndex;
	int m_nEndIndex;
	int m_nCapacity;

public:

	CRingT()
	{
		m_objects = NULL;
		m_nFirstIndex = 0;
		m_nEndIndex = 0;
		m_nCapacity = 0;
	}

	~CRingT()
	{
		Clear();		

		if(m_objects != NULL)
		{
			CoTaskMemFree(m_objects);
			m_objects = NULL;
		}
	}

private:

	int _GetNextIndex(int index, int step)
	{
		return (index + step + m_nCapacity + 1) % (m_nCapacity + 1);
	}	

public:

	T & GetFirst()
	{
		return m_objects[ m_nFirstIndex];
	}

	T & GetLast()
	{
		return m_objects[ _GetNextIndex(m_nEndIndex,-1) ];
	}

	BOOL IsFull()
	{
		return GetCount() == m_nCapacity ;
	}

	BOOL IsEmpty()
	{
		return GetCount() == 0;
	}

	int GetCount()
	{
		return m_nCapacity == 0 ? 0 : (m_nEndIndex - m_nFirstIndex + m_nCapacity + 1) % (m_nCapacity + 1);
	}

	int GetCapacity()
	{
		return m_nCapacity;
	}

	HRESULT SetCapacity(int capacity)
	{
		if(GetCount() > capacity) 
			return E_UNEXPECTED; 

		if(m_nCapacity == capacity)
			return FALSE;

		T * newObjects = (T *)CoTaskMemAlloc( sizeof(T) * (capacity +1) );

		if(newObjects == NULL)
			return E_OUTOFMEMORY;

		for(int i=0; i< capacity + 1; i++)
		{
			TCopy::Init(newObjects + i);
		}

		if(m_objects != NULL)
		{
			for(int i=0; i<m_nCapacity;i++)
			{
				TCopy::Copy(&m_objects[i], &newObjects[i]);
			}

			CoTaskMemFree(m_objects);
		}

		m_objects = newObjects;
		m_nCapacity = capacity;

		return S_OK;
	}

	T & GetAt(int index)
	{
		return m_objects[ _GetNextIndex(m_nFirstIndex, index )];
	}

	void SetAt(int index, const T & newVal)
	{
		TCopy::Copy(&newVal, &m_objects[ _GetNextIndex(m_nFirstIndex, index)] );
	}

	void Clear()
	{
		if(m_objects != NULL)
		{
			for(int i =0;i<m_nCapacity;i++)
			{
				TCopy::Distory(&m_objects[i]);
			}

			m_nFirstIndex = 0;
			m_nEndIndex = 0;
		}
		
	}

	void PushFront(const T & newVal)
	{
		int firstIndex = _GetNextIndex(m_nFirstIndex, -1);
		TCopy::Copy(&newVal, &m_objects[firstIndex]);   

		if (GetCount() == m_nCapacity)
		{
			m_nEndIndex = _GetNextIndex(m_nEndIndex, -1);
		}

		m_nFirstIndex = firstIndex;
	}

	void PushBack(const T & newVal)
	{
		TCopy::Copy(m_objects + m_nEndIndex, &newVal);    
	
		if (GetCount() == m_nCapacity)
		{
			m_nFirstIndex = _GetNextIndex(m_nFirstIndex, 1);
		}

		m_nEndIndex = _GetNextIndex(m_nEndIndex, 1);

		return S_OK;
	}

	void PopFront(T & t)
	{
		TCopy::Copy(m_objects+ m_nFirstIndex, &t);

		TCopy::Distory(m_objects+ m_nFirstIndex);

		m_nFirstIndex = _GetNextIndex(m_nFirstIndex, 1);
	}

	void PopBack(T & t)
	{	
		int endIndex = _GetNextIndex(m_nEndIndex, -1);

		TCopy::Copy(m_objects + endIndex, &t);
		
		TCopy::Distory(m_objects + endIndex);
		
		m_nEndIndex = endIndex;
	}

	int IndexOf(int beginIndex, const T & val)
	{
		for(LONG size = GetCount() ; beginIndex <size; beginIndex ++)
		{
			if( TCopy::Compare( &GetAt(beginIndex) , &val ) == 0) return beginIndex;
		}
		return -1;	
	}

	int LastIndexOf(const T & val)
	{
		for(LONG i = GetCount() -1; i>=0; i--)
		{
			if( TCopy::Compare( &GetAt(beginIndex) , &val) == 0) return i;
		}

		return -1;
	}

	BOOL Contains(const T & val)
	{
		return IndexOf(0, val) != -1;
	}

	int SubArray(int beginIndex, int count, T values[])
	{	
		int size = GetCount();

		for(int i = 0 ; i <count && beginIndex + i <size  ; i++)
		{
			TCopy::Copy( &GetAt(i), values+ i );
		}

		return beginIndex + count <= size ? count : size- beginIndex;

	}
	
};


#endif