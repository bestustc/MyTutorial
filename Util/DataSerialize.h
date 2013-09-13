#ifndef __UTIL__DATA_SERIALIZE__H__
#define __UTIL__DATA_SERIALIZE__H__

#pragma once

#include "UtilHelper.h"
#include "IndexMap.h"
#include "DataProfile.h"
#include "MetaVariant.h"

struct IDataSerializable;

class CDataSerialize
{
	IDataNode * m_pNode;

public:

	BOOL m_bWrite;

public:
	
	CDataSerialize(BOOL write, IDataNode * pNode = NULL);

	~CDataSerialize();

	HRESULT GetNode(IDataNode ** pNode);

private:
	
	HRESULT _Write(LPCTSTR name, const CMetaVariant & value, IDataChildren * pChildren);

	HRESULT _Write(LPCTSTR name, IDataSerializable * p, IDataChildren * pChildren );

	// Ë÷Òý
	template<typename T>
	HRESULT _Write(LPCTSTR name, const T & t, const void * p, IDataChildren * pChildren)
	{
		USE_VAR(p);
		return _Write(name,  CMetaVariant(t), pChildren);
	}

	template<typename T>
	HRESULT _Write(LPCTSTR name, const T & t, const IDataSerializable * p, IDataChildren * pChildren)
	{
		USE_VAR(t);
		return _Write(name, const_cast<IDataSerializable *>(p), pChildren);
	}

	HRESULT _NewSubNode(LPCTSTR name, IDataChildren ** pChildren);

public:

	template<typename T>
	HRESULT Write(LPCTSTR name,const T & t)
	{
		return _Write(name, t, &t, NULL);
	}
	
	HRESULT Write(LPCTSTR name, int count ,const BYTE arr[]);

	template<typename T>
	HRESULT Write(LPCTSTR name, const vector<T> & arr)
	{		
		IDataChildren * pChildren = NULL;
		HRESULT hr = _NewSubNode(name, &pChildren);
		if(FAILED(hr)) return hr;
		
		for(int i=0,size = (int)arr.size(); i<size;i++)
		{
			const T & v = arr[i];

			TCHAR name[64] = { TZERO };
			_itot_s(i,name, 64, 10);
			hr = _Write(name, v, &v, pChildren);
			if(FAILED(hr)) return hr;
		}
		return S_OK;
	}

	template<typename T>
	HRESULT Write(LPCTSTR name, int count ,const T arr[])
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _NewSubNode(name, &pChildren);
		if(FAILED(hr)) return hr;
		
		for(int i=0 ; i<count; i++)
		{
			const T & v = arr[i];

			TCHAR name[64] = { TZERO };
			_itot_s(i,name, 64, 10);
			hr = _Write(name, v, &v, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Write(LPCTSTR name, const CIndexMapT<String, T> & kvs)
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _NewSubNode(name, &pChildren);
		if(FAILED(hr)) return hr;		
		
		for(CIndexMap<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;

			hr = _Write(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Write(LPCTSTR name, const std::map<String, T> & kvs)
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _NewSubNode(name, &pChildren);
		if(FAILED(hr)) return hr;		
		
		for(std::map<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;

			hr = _Write(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Write(LPCTSTR name, const hash_map<String, T> & kvs)
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _NewSubNode(name, &pChildren);
		if(FAILED(hr)) return hr;		
		
		for(hash_map<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;
	
			hr = _Write(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

private:


	HRESULT _Read(LPCTSTR name, MetaDataType dataType, CMetaVariant & value, IDataChildren * pChildren);	

	HRESULT _Read(LPCTSTR name, IDataSerializable * p, IDataChildren * pChildren);

	template<typename T>
	HRESULT _Read(LPCTSTR name, T & t, void * p, IDataChildren * pChildren)
	{
		USE_VAR(p);

		CMetaVariant var(DEFAULT(T));

		HRESULT hr = _Read(name, var.type, var, pChildren);
		if(FAILED(hr)) return hr;

		return var.CopyTo(t);		
	}

	template<typename T>
	HRESULT _Read(LPCTSTR name, T & t, IDataSerializable * p, IDataChildren * pChildren)
	{
		USE_VAR(t);

		return _Read(name, p, pChildren);
	}

	HRESULT _GetSubNode(LPCTSTR name, IDataChildren ** pChildren);

public:

	template<typename T>
	HRESULT Read(LPCTSTR name, T & v)
	{
		return _Read(name, v, &v, NULL);
	}

	template<typename T>
	HRESULT Read(LPCTSTR name, vector<T> & arr)
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _GetSubNode(VARIANT_BSTR(name), &pChildren);
		if(FAILED(hr)) return hr;
		
		for(int i=0,size = (int)arr.size(); i<size;i++)
		{
			T & v = arr[i];

			TCHAR name[64] = { TZERO };
			_itot_s(i,name, 64, 10);
			hr = _Read(name, v, &v, pChildren);

			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Read(LPCTSTR name, CIndexMapT<String, T> & kvs)		
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _GetSubNode(VARIANT_BSTR(name), &pChildren);
		if(FAILED(hr)) return hr;

		for(CIndexMapT<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;

			hr = _Read(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Read(LPCTSTR name, std::map<String, T> & kvs)		
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _GetSubNode(VARIANT_BSTR(name), &pChildren);
		if(FAILED(hr)) return hr;

		for(std::map<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;

			hr = _Read(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Read(LPCTSTR name, hash_map<String, T> & kvs)	
	{
		IDataChildren * pChildren = NULL;
		HRESULT hr = _GetSubNode(VARIANT_BSTR(name), &pChildren);
		if(FAILED(hr)) return hr;

		for(hash_map<String, T>::const_iterator it = kvs.begin(), end = kvs.end(); 
			it != end;it++)
		{
			String first = it->first;
			T t = it->second;

			hr = _Read(first.c_str(), t, &t, pChildren);
			if(FAILED(hr)) return hr;
		}

		return S_OK;
	}

	template<typename T>
	HRESULT Serialize(LPCTSTR name, T & v)
	{
		if(m_bWrite)
			return Write(name,v);

		else
			return Read(name,v);
	}

	template<typename T>
	HRESULT Serialize(LPCTSTR name, int count, T vs[])
	{
		if(m_bWrite)
			return Write(name,count, vs);
		else
			return Read(name,vs);
	}
};

struct IDataSerializable
{
	virtual HRESULT Write(CDataSerialize & serialize) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT Read(CDataSerialize & serialize)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT Serialize(CDataSerialize & serialize)
	{
		return E_NOTIMPL;
	}

	static HRESULT GatherLayout(CDataSerialize & serialize)
	{
		return E_NOTIMPL;
	}

};


#endif