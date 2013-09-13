#ifndef __UTIL__STRING_HELPER__H__
#define __UTIL__STRING_HELPER__H__

#pragma once

#include "UtilHelper.h"

class CStringHelper
{
public:
	CStringHelper(void);
	~CStringHelper(void);

public:

	static BOOL ToInt64(LPCTSTR text, INT64 * pVal);

	static BOOL ToDouble(LPCTSTR text, DOUBLE * pVal);

	static INT64 ToInt64(LPCTSTR text, INT64 defaultValue = 0);

	static DOUBLE ToDouble(LPCTSTR text, DOUBLE defaultValue = 0.0);

	static String ToString(INT64 i);

	static String ToString(DOUBLE d);

	static int IndexOf(LPCTSTR text, TCHAR c);

	static int IndexOf(LPCTSTR text,LPCTSTR sub);

	static int LastIndexOf(LPCTSTR text, TCHAR c);

	static String TrimStart(LPCTSTR text, TCHAR c = TSPACE);

	static String TrimEnd(LPCTSTR text, TCHAR c = TSPACE);

	static String Trim(LPCTSTR text, TCHAR c = TSPACE);

	static BOOL IsUpper(TCHAR c);

	static BOOL IsLower(TCHAR c);

	static LPCTSTR ToUpper(LPCTSTR srcText, TCHAR destText[]);

	static LPCTSTR ToLower(LPCTSTR srcText, TCHAR destText[]);

	static String SubString(LPCTSTR srcText, int beginIndex, int count = INT_MAX);

	static BOOL Contains(LPCTSTR text, TCHAR c);

	static BOOL Split(LPCTSTR text, TCHAR c, vector<String> & subs);

	static BOOL LeftEqualsWith(LPCTSTR text, LPCTSTR sub);

	static BOOL Equals(LPCTSTR s0, LPCTSTR s1, BOOL ignoreCase = FALSE);

	static void KMPCreateNext(LPCTSTR match, int len, int next[]);

	// ²éÕÒ×Ö´®
	static int KMPFind(LPCTSTR text, int strLen,  LPCTSTR match,
		int matchLen, int next[]);

	//static string ToAString(const WCHAR * str);

	//static String ToWString(const char * str);


	static string CStringHelper::ToAString(const WCHAR * str)
	{
		if(str==NULL) return std::string();

		size_t len = ::wcslen(str);
		if(len==0) return std::string();

		USES_CONVERSION_EX;
		int nConvertedLen = ::WideCharToMultiByte(_acp_ex,0, str, -1, NULL, 0, NULL, NULL);

		char * dest = (char *)CoTaskMemAlloc(nConvertedLen + 1);
		memset(dest,0,nConvertedLen + 1);

		WideCharToMultiByte(_acp_ex,0, str,-1, dest , len * 4 +2, NULL, NULL);

		std::string s = dest;

		CoTaskMemFree(dest);
		return s;

	}

	static wstring CStringHelper::ToWString(const char * str)
	{
		USES_CONVERSION;
		return A2W(str);
	}

	static BOOL IsVariantName(LPCTSTR text);

	static HRESULT GUID2String(const GUID & guid, String & text);

	static HRESULT String2GUID(LPCTSTR text, GUID & guid);

};


#endif