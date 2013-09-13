#include "JsonHelper.h"
#include "Json.h"
#include "ComHelper.h"

HRESULT CJsonHelper::GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, String & value)
{
	IJsonValue * jsonValue = NULL;
	HRESULT hr = jsonParent->GetByName( VARIANT_BSTR(name), &jsonValue);

	if(FAILED(hr) ) return hr;

	BSTR str= NULL;
	hr = jsonValue->AsString(&str);
	jsonValue->Release();

	if(FAILED(hr)) return hr;
	value = BSTR2STRING(str);

	if(str != NULL) SysFreeString(str);

	return S_OK;	
}

HRESULT CJsonHelper::GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, BOOL & value)
{
	IJsonValue * jsonValue = NULL;
	HRESULT hr = jsonParent->GetByName( VARIANT_BSTR(name), &jsonValue);

	if(FAILED(hr) ) return hr;

	VARIANT_BOOL v= COM_BOOL(FALSE);
	hr = jsonValue->AsBoolean(&v);
	jsonValue->Release();

	value = COM_IS_TRUE(v);

	return hr;
}


HRESULT CJsonHelper::GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, LONG & value)
{
	IJsonValue * jsonValue = NULL;
	HRESULT hr = jsonParent->GetByName( VARIANT_BSTR(name), &jsonValue);

	if(FAILED(hr) ) return hr;


	hr = jsonValue->AsInt(&value);
	jsonValue->Release();

	return hr;
}