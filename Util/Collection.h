#ifndef __UTIL__COLLECTION__H__
#define __UTIL__COLLECTION__H__

#include "UtilHelper.h"
#include "ComHelper.h"

template<typename T>
struct CopyT
{
	static int Compare(const T * v0, const T * v1)
	{
		return v0 == v1 ? 0 :  memcmp(v0,v1, sizeof(T)) ;		
	}

	static void Init(T * src)
	{
	}

	static HRESULT Copy(const T * src, T * dest)
	{
		if(src == dest) return S_OK;

		Checked::memcpy_s(dest, sizeof(T), src, sizeof(T));
		return S_OK;
	}

	static void Distory(T  v)
	{
	}
};

template<typename T>
struct OperatorCopyT
{
	static int Compare(const T * v0, const T * v1)
	{
		return (v0 == v1 || *v0 == *v1) ? 0:
			 ( (*v0 > *v1) : 1 : -1 );
	}

	static void Init(T * src)
	{
	}

	static HRESULT Copy(const T * src, T * dest)
	{
		if(src == dest) return S_OK;
		*dest = *src;
		return S_OK;
	}

	static void Distory(T * v)
	{
	}
};

template<>
struct CopyT<VARIANT>
{
	static int Compare(const VARIANT * v0, const VARIANT * v1)
	{
		return VARIANT_COMPARE( *v0, *v1 );
	}

	static void Init(VARIANT * src)
	{
		src->vt = VT_EMPTY;
	}

	static HRESULT Copy(const VARIANT * src, VARIANT * dest)
	{
		if(src == dest) return S_OK;

		dest->vt = VT_EMPTY;
		return VariantCopy(dest, const_cast<VARIANT*>(src));

	}

	static void Distory(VARIANT * v)
	{
		VariantClear(v);
	}
};


template<>
struct CopyT<BSTR>
{
	static BOOL Compare(const BSTR * v0, const BSTR * v1)
	{
		return  v0 == v1 ? 0 : wcscmp(*v0, *v1);
	}

	static void Init(BSTR * src)
	{
		*src = NULL;
	}

	static HRESULT Copy(const BSTR * src, BSTR * dest)
	{
		if(src == dest) return S_OK;

		SysFreeString(*src);
		*dest = SysAllocString(*src);
		return S_OK;
	}

	static void Distory(BSTR * v)
	{
		SysFreeString(*v);
	}
};

template<>
struct CopyT<String>
{
	static BOOL Compare(const String * v0, const String * v1)
	{
		return  v0 == v1 ? 0 : ::wcscmp(v0->c_str(), v1->c_str());
	}

	static void Init(String * src)
	{
		*src = String();
	}

	static HRESULT Copy(const String * src, String * dest)
	{
		if(src == dest) return S_OK;

		*dest = *src;
		return S_OK;
	}

	static void Distory(String * v)
	{
	
	}
};

template<typename T>
struct CopyUnknownT
{
	static BOOL Compare(const T ** v0, const T ** v1)
	{
		return  v0 == v1 ? 0 : memcmp(*v0,*v1,sizeof(T *));
	}

	static void Init(T ** src)
	{
	}

	static HRESULT Copy(const T ** src, T ** dest)
	{
		if(src == dest) return S_OK;
		
		if(*src != NULL) (*src)->AddRef();
		if(*dest != NULL) (*dest)->AddRef();

		*dest = *src;
		return S_OK;
	}

	static void Distory(String * v)
	{
	
	}
};


HRESULT StringArray2SafeArray(const vector<String> & vec, SAFEARRAY ** pSafeArray);

HRESULT StringArray2SafeArray(LONG count, const String * arr, SAFEARRAY ** pSafeArray);

HRESULT SafeArray2StringArray(SAFEARRAY * pArray, vector<String> & vec);

HRESULT StringArray2BSTRArray(LONG count, const String * arr, vector<BSTR> & vec);

HRESULT SafeArray2BSTRArray(SAFEARRAY * pArray, vector<BSTR> & vec);

HRESULT FreeBSTRArray(LONG count, BSTR * arr);

template<typename TElem>
struct ICollectionEnumatorT
{
	virtual BOOL MoveNext() = 0;

	virtual void Reset()= 0;

	virtual TElem GetCurrent() = 0;
};


template<typename TCol, typename TElem>
struct StlEnumatorT : ICollectionEnumatorT< TElem>
{
	typename typedef TCol::iterator Iterator;

	TCol * m_pCol;

	Iterator m_it;
	Iterator m_end;

	BOOL MoveNext() 
	{
		return m_it != m_end;
	}

	void Reset()
	{
		m_it = m_pCol->begin();
	}

	TElem & GetCurrent() 
	{
		return *m_it;
	}
};

template<typename TElem>
ICollectionEnumatorT<TElem> * CreateCollectionEnumatorT(vector<TElem> & col)
{
	StlEnumatorT<TCol> * pCol = new StlEnumatorT<TCol>();
	pCol->m_pCol = &col;

	pCol->Reset();	
	return pCol;
}

#define FOR_COLLECTION_ENUMATOR(VarType, var, col)\
for(AutoLifePtrT<ICollectionEnumatorT<VarType>  > ______enum = CreateCollectionEnumatorT<TElem>(col); ______proxy->MoveNext();)\
	for(BOOL ______Flag = TRUE; ______Flag;)\
			for(VarType & var = ______proxy->GetCurrent(); ______Flag; ______Flag = FALSE)\



#endif