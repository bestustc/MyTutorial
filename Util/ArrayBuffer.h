#pragma once

template<typename T>
struct NewArrayT
{
	static T * Alloc(size_t length)
	{
		if(length == 0) return NULL;

		return new T[length];
	}

	static void Free(size_t length, T * pArray)
	{
		if(pArray != NULL) delete [] pArray;
	}
};

template<typename T>
struct MallocArrayT
{
	static T * Alloc(size_t length)
	{
		if(length == 0) return NULL;

		return (T *)malloc(sizeof(T) * length);
	}

	static void Free(size_t length, T * pArray)
	{
		if(pArray != NULL) free(pArray);
	}
};

template<typename T>
struct ComAllocArrayT
{
	static T * Alloc(size_t length)
	{
		if(length == 0) return NULL;

		return (T *)CoTaskMemAlloc(sizeof(T) * length);
	}

	static void Free(size_t length, T * pArray)
	{
		if(pArray != NULL) CoTaskMemFree(pArray);
	}
};


template<typename T>
struct UnAllocArrayT
{
	static T * Alloc(size_t length)
	{
		return NULL;
	}

	static void Free(size_t length, T * pArray)
	{
		
	}
};

template<typename T, typename TArrayAlloc = ComAllocArrayT<T> >
class ArrayBufferT
{
	T * m_pBuffer;
	size_t m_nLength;

public:

	const T * ToArray() const
	{
		return m_pBuffer;
	}

	T * ToArray() 
	{
		return m_pBuffer;
	}

	size_t Length() const
	{
		return m_nLength;
	}

	size_t Size() const
	{
		return m_nLength;
	}

public:

	const T & operator [] (size_t index) const
	{
		return m_pBuffer[index];
	}

	T & operator [] (size_t index)
	{
		return m_pBuffer[index];
	}

public:

	ArrayBufferT(size_t length = 0)
	{
		m_pBuffer = NULL;
		m_nLength = 0;

		Realloc(length);
	}

	void Realloc(size_t length)
	{
		Free();

		m_pBuffer = NULL;
		m_pBuffer = TArrayAlloc::Alloc(length);
		if(m_pBuffer != NULL)
		{
			m_nLength = length;
		}
	}

	void Free()
	{
		if(m_pBuffer)
		{
			TArrayAlloc::Free(m_nLength, m_pBuffer);
		}

		m_nLength = 0;
		m_pBuffer = NULL;
	}

	~ArrayBufferT(void)
	{
		
	}
};

template<typename T>
class ArrayBufferNewT : public ArrayBufferT<T, NewArrayT<T> >
{
};


template<typename T>
class ArrayBufferAllocT : public ArrayBufferT<T, MallocArrayT<T> >
{
};

template<typename T>
class ArrayBufferComAllocT : public ArrayBufferT<T, ComAllocArrayT<T> >
{
};

template<typename T>
class ArrayBufferUnallocT : public ArrayBufferT<T, UnAllocArrayT<T> >
{
};


template<typename TElem, typename TArrayAlloc>
struct ArrayBufferEnumatorT : ICollectionEnumatorT<TElem>
{
	ArrayBufferT * pBuffer;
	size_t  nIndex;

	virtual BOOL MoveNext() 
	{
		return nIndex + 1 < pBuffer->Length();
	}

	virtual void Reset()
	{
		nIndex = 0;
	}

	virtual TElem GetCurrent() 
	{
		return m_pBuffer[nIndex -1];
	}
};

template<typename TElem, typename TArrayAlloc>
ICollectionEnumatorT<TElem> * CreateArrayEnumatorT(ArrayBufferEnumatorT<TElem, TArrayAlloc> & buffer)
{
	ArrayBufferEnumatorT<TElem, TArrayAlloc> * pCol = new ArrayBufferEnumatorT<TElem, TArrayAlloc>();
	pCol->pBuffer = &buffer;

	pCol->Reset();	
	return pCol;
}