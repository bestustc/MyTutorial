#ifndef __UTIL__SAFE_VECTOR__H__
#define __UTIL__SAFE_VECTOR__H__

#pragma once

#include "UtilHelper.h"
#include "ComHelper.h"

class CSafeVector
{
	SAFEARRAY * m_pArray;
	LONG m_nLength;
	VARTYPE m_type;

public:

	CSafeVector()
	{
		m_pArray = NULL;
		m_nLength = 0;
		m_type = VT_EMPTY;
	}

	CSafeVector(VARTYPE vt, LONG length, VARIANT * values = NULL)
	{
		m_pArray = SafeArrayCreateVector(vt,0, length);
		m_nLength = length;
		m_type = vt;
	
		if(values != NULL)
		{
			for(LONG i=0; i<length;i++)
			{
				SetAt(i,(void *)(values + i));
			}
		}
	}

	HRESULT GetAt(LONG index, void * pVal)
	{
		return SafeArrayGetElement(m_pArray, &index, pVal);
	}
	
	HRESULT GetAt(LONG index, VARIANT * pVal)
	{
		INVALID_NULL(pVal);	

		void *p = NULL;
		switch(m_type)
		{
		case VT_RESERVED:
			 p = reinterpret_cast<void *>(reinterpret_cast<size_t>(pVal) + sizeof(VARTYPE) );
			 break;

		case VT_EMPTY:
			p= NULL;
			break;

		case VT_DECIMAL:
		case VT_VARIANT:
			p = reinterpret_cast<void *>(pVal);
			break;

		default:
			 p = reinterpret_cast<void *>(reinterpret_cast<size_t>(pVal) + sizeof(VARTYPE) + sizeof(WORD) * 3);
			 break;
		}		

		if(p == NULL) return DISP_E_BADVARTYPE;

		HRESULT hr =  GetAt(index, p);

		if(m_type != VT_DECIMAL && m_type != VT_VARIANT)		
			pVal->vt = m_type;

		return hr;
	}

	HRESULT SetAt(LONG index, void * pVal)
	{
		return SafeArrayPutElement(m_pArray, &index, pVal);
	}

	HRESULT SetAt(LONG index, VARIANT newVal)
	{		
		void *p = NULL;
		switch(m_type)
		{
		case VT_RESERVED:
			 p = reinterpret_cast<void *>(reinterpret_cast<size_t>(&newVal) + sizeof(VARTYPE) );
			 break;

		case VT_EMPTY:
			p= NULL;
			break;

		case VT_DECIMAL:
		case VT_VARIANT:
			p = reinterpret_cast<void *>(&newVal);
			break;

		default:
			 p = reinterpret_cast<void *>(reinterpret_cast<size_t>(&newVal) + sizeof(VARTYPE) + sizeof(WORD) * 3);
			 break;
		}		

		if(p == NULL) return DISP_E_BADVARTYPE;

		return SetAt(index, p);
	}

	HRESULT GetLength(LONG * pVal)
	{
		INVALID_NULL(pVal);

		*pVal = m_nLength;
		return S_OK;
	}

	HRESULT GetVarType(VARTYPE * pType)
	{
		INVALID_NULL(pType);

		*pType = m_type;
		return S_OK;
	}

	HRESULT GetLBound(LONG *pVal)
	{
		INVALID_NULL(pVal);

		return SafeArrayGetLBound(m_pArray,1,pVal);
	}

	HRESULT GetUBound(LONG *pVal)
	{
		INVALID_NULL(pVal);

		return SafeArrayGetUBound(m_pArray,1, pVal);
	}

	HRESULT Destroy()
	{
		if(m_pArray != NULL)
		{
			SafeArrayDestroy(m_pArray);
			m_pArray = NULL;
		}

		m_nLength = 0;

		return S_OK;
	}

	HRESULT Attach(SAFEARRAY * pArray)
	{
		HRESULT hr = Destroy();

		if(FAILED(hr)) return hr;

		m_pArray = pArray;

		if(pArray != NULL)
		{
			LONG uBound = 0;
			LONG lBound = 0;
			hr = GetUBound(&uBound);
			if(FAILED(hr)) return hr;

			hr = GetLBound(&lBound);
			if(FAILED(hr)) return hr;

			m_nLength = uBound - lBound + 1;

			SafeArrayGetVartype(pArray,&m_type);
		}

		return S_OK;
		
	}

	HRESULT Detach(SAFEARRAY ** ppArray)
	{
		INVALID_NULL(ppArray);

		*ppArray = m_pArray;
		
		m_pArray = NULL;
		m_nLength = 0 ;
		m_type = VT_EMPTY;

		return S_OK;
	}

	HRESULT SubArray(LONG beginIndex, LONG count, VARIANT values[], LONG * fetchedCount)
	{
		INVALID_NULL(values);
		INVALID_NULL(fetchedCount);
		INVALID_RANGE(beginIndex, count, m_nLength);

		LONG lBound = 0;
		HRESULT hr = GetLBound(&lBound);
		if(FAILED(hr)) return hr;

		*fetchedCount = 0;

		for(LONG i = 0 ; i<count && i + beginIndex < m_nLength ;i++)
		{
			hr = GetAt(i + beginIndex + lBound , values + i);

			if(FAILED(hr)) return hr;

			(*fetchedCount) ++;
		}

		return S_OK;
	}

	~CSafeVector()
	{
		Destroy();
	}

};


#endif