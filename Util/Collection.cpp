#include "Collection.h"
#include "UtilHelper.h"
#include "SafeVector.h"


HRESULT StringArray2SafeArray(const vector<String> & vec, SAFEARRAY ** pSafeArray)
{
	INVALID_NULL(pSafeArray);

	LONG count = vec.size();

	CSafeVector safeVec(VT_BSTR, count);

	for(LONG i=0;i<count;i++)
	{
		CComBSTR s = CComBSTR(vec[i].c_str());

		HRESULT hr = safeVec.SetAt(i, (void *)*&s);
		if(FAILED(hr)) return hr;
	}

	return safeVec.Detach( pSafeArray);
}

HRESULT StringArray2SafeArray(LONG count, const String * strs, SAFEARRAY ** pSafeArray)
{
	INVALID_NULL(pSafeArray);
	POINTER_NULL(strs);
	EXPECTED_TRUE( count >=0);

	CSafeVector safeVec(VT_BSTR, count);

	for(LONG i=0;i<count; ++i)
	{
		CComBSTR s = CComBSTR(strs[i].c_str());

		HRESULT hr = safeVec.SetAt(i,(void *)*&s);
		if(FAILED(hr)) return hr;
	}

	return safeVec.Detach( pSafeArray);
}


HRESULT SafeArray2StringArray(SAFEARRAY * pArray, vector<String> & vec)
{
	INVALID_NULL(pArray);

	CSafeVector safeVec;
	HRESULT hr = safeVec.Attach(pArray);
	if(FAILED(hr)) return hr;

	do
	{
		VARTYPE type = VT_EMPTY;
		hr = safeVec.GetVarType(&type);
		if(FAILED(hr)) break;

		if(type != VT_BSTR)
		{
			hr = DISP_E_BADVARTYPE;
			break;
		}

		LONG len = 0;
		hr = safeVec.GetLength(&len);

		for(LONG i=0;i<len;++i)
		{
			BSTR str = NULL;
			hr = safeVec.GetAt(i, (void *)&str);
			if(FAILED(hr)) break;

			vec.push_back( BSTR2STRING(str) );

			FREE_BSTR(str);

		}

	}while(FALSE);

	SAFEARRAY * p = NULL;
	return safeVec.Detach(&p);
}

HRESULT StringArray2BSTRArray(LONG count, const String * arr, vector<BSTR> & vec)
{
	for(LONG i=0;i<count;++i)
	{
		vec.push_back(STRING2BSTR(arr[i]));
	}

	return S_OK;
}

HRESULT SafeArray2BSTRArray(SAFEARRAY * pArray, vector<BSTR> & vec)
{
	INVALID_NULL(pArray);

	CSafeVector safeVec;
	HRESULT hr = safeVec.Attach(pArray);
	if(FAILED(hr)) return hr;

	do
	{
		VARTYPE type = VT_EMPTY;
		hr = safeVec.GetVarType(&type);
		if(FAILED(hr)) break;

		if(type != VT_BSTR)
		{
			hr = DISP_E_BADVARTYPE;
			break;
		}

		LONG len = 0;
		hr = safeVec.GetLength(&len);

		for(LONG i=0;i<len;++i)
		{
			BSTR str = NULL;
			hr = safeVec.GetAt(i, (void *)&str);
			if(FAILED(hr)) break;

			vec.push_back(str);
		}

	}while(FALSE);

	SAFEARRAY * p = NULL;
	return safeVec.Detach(&p);
}

HRESULT FreeBSTRArray(LONG count, BSTR * arr)
{
	for(LONG i=0;i<count;++i)
	{
		FREE_BSTR(arr[i]);
	}

	return S_OK;
}