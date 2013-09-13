#ifndef __UTIL__JSON_HELPER__H__
#define __UTIL__JSON_HELPER__H__

#include "UtilHelper.h"

struct IJsonValue;
class CJsonHelper
{

public:

	static HRESULT GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, String & value);

	static HRESULT GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, BOOL & value);

	static HRESULT GetJsonValue(IJsonValue * jsonParent, LPCTSTR name, LONG & value);

public:

	
};

#endif