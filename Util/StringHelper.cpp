#include "StringHelper.h"
#include "atlrx.h"
#include "ComHelper.h"

CStringHelper::CStringHelper(void)
{
}

CStringHelper::~CStringHelper(void)
{
}

BOOL CStringHelper::ToInt64(LPCTSTR text, INT64 * pVal)
{
	if(pVal == NULL) return FALSE;

	TCHAR * end = NULL;	
	*pVal = _tcstoi64(text, &end, 10);
	return TRUE;
}

BOOL CStringHelper::ToDouble(LPCTSTR text, DOUBLE * pVal)
{
	if(pVal == NULL) return FALSE;

	TCHAR * end = NULL;
	*pVal = _tcstod(text,&end);
	return TRUE;
}

INT64 CStringHelper::ToInt64(LPCTSTR text, INT64 defaultValue)
{
	INT64 i = 0;
	if(!ToInt64(text, &i))
	{
		i = defaultValue;
	}

	return i;
}

DOUBLE CStringHelper::ToDouble(LPCTSTR text, DOUBLE defaultValue )
{
	DOUBLE d = 0;
	if(!ToDouble(text, &d ))
	{
		d = defaultValue;
	}
	return d;
}


String CStringHelper::ToString(INT64 i)
{
	TCHAR text[32] = { TZERO };

	::_stprintf_s(text, 32, _T("%ld"),i);

	return text;
}

String CStringHelper::ToString(DOUBLE d)
{
	TCHAR text[64] = { TZERO };

	::_stprintf_s(text, 64, _T("%f"),d);

	return text;
}

int CStringHelper::IndexOf(LPCTSTR text, TCHAR c)
{	
	LPCTSTR temp = text;

	while(*temp != _T('\0') && *temp != c) temp++;
	return *temp == _T('\0') ? -1 : temp - text;
}

int CStringHelper::IndexOf(LPCTSTR text,LPCTSTR sub)
{

	int len=STRLEN(sub);
	int *next=new int[len];

	KMPCreateNext(text,len,next);
	return KMPFind(text, STRLEN(text) ,sub ,len,next);
}


int CStringHelper::LastIndexOf(LPCTSTR text, TCHAR c)
{
	int index = -1;	
	LPCTSTR temp = text;
	while(*temp!= _T('\0'))
	{	
		if(*temp == c) index = temp - text;		
		temp++;
	}

	return index;
}

BOOL CStringHelper::Contains(LPCTSTR text, TCHAR c)
{
	return IndexOf(text,c) !=-1;
}

BOOL CStringHelper::Split(LPCTSTR text, TCHAR c, vector<String> & subs)
{
	LPTSTR tempText = const_cast<LPTSTR>(text);

	LPTSTR sub = tempText;
	while(*tempText != _T('\0'))
	{
		if(*tempText == c)
		{
			TCHAR t = *tempText;
			*tempText = _T('\0');
			subs.push_back(sub);
			*tempText = t;
			sub = tempText +1;
		}

		tempText ++;
	}

	if(sub[0] !=_T('\0')) subs.push_back(sub);

	return TRUE;
}

BOOL CStringHelper::LeftEqualsWith(LPCTSTR text, LPCTSTR sub)
{
	while( *(text++) != TZERO && *(sub++) != TZERO)
	{
		if(*text != *sub) return FALSE;
	}

	return *sub == TZERO;
}

BOOL CStringHelper::Equals(LPCTSTR s0, LPCTSTR s1, BOOL ignoreCase)
{
	if(s0 == s1) return TRUE;
	if(s0 == NULL || s0 == NULL) return FALSE;

	while( (*s0 != TZERO) && ( *s1 != TZERO ) )
	{
		TCHAR c0 = *s0;
		TCHAR c1 = *s1;

		if( ( ignoreCase && ::tolower(c0) != ::tolower(c1))
			|| (!ignoreCase && c0 != c1 ) )
		{
			return FALSE;
		}

		s0 ++;
		s1 ++;
	}

	return *s0 == *s1;
}

String CStringHelper::TrimStart(LPCTSTR text, TCHAR c)
{
	while( *text == c) text ++;

	return text;
}

String CStringHelper::TrimEnd(LPCTSTR text, TCHAR c)
{
	LPTSTR temp = const_cast<LPTSTR>(text);

	int count = STRLEN(temp);

	for(int i=count -1;i>=0;i--)
	{
		if(temp[i] != c)
		{
			TCHAR t = temp[i + 1];
			temp[ i +1] = TZERO;

			String tempStr = temp;
			temp[ i +1] = t;

			return temp;
		}
	}

	return String();
}

String CStringHelper::Trim(LPCTSTR text, TCHAR c )
{
	LPTSTR temp = const_cast<LPTSTR>(text);
	while( *temp == c) temp ++;

	int count = STRLEN(temp);

	for(int i=count -1;i>=0;i--)
	{
		if(temp[i] != c)
		{
			TCHAR t = temp[i + 1];
			temp[ i +1] = TZERO;

			String tempStr = temp;
			temp[ i +1] = t;

			return temp;
		}
	}

	return String();
}

BOOL CStringHelper::IsUpper(TCHAR c)
{
	return c >= _T('A') && c<= _T('Z');
}

BOOL CStringHelper::IsLower(TCHAR c)
{
	return c >= _T('a') && c<= _T('z');
}


LPCTSTR CStringHelper::ToUpper(LPCTSTR srcText, TCHAR destText[])
{
	while(*srcText !=TZERO)
	{
		*destText = isupper( *srcText ) ? *srcText : (TCHAR)toupper(*srcText) ;
		destText ++;
		srcText ++;
	}

	*destText = TZERO;

	return destText;
}

LPCTSTR CStringHelper::ToLower(LPCTSTR srcText, TCHAR destText[])
{
	while(*srcText !=TZERO)
	{
		*destText = islower( *srcText ) ? *srcText : (TCHAR)tolower(*srcText) ;
		destText ++;
		srcText ++;
	}

	*destText = TZERO;
	return destText;
}

String CStringHelper::SubString(LPCTSTR srcText, int beginIndex, int count)
{
	TCHAR * text = new TCHAR[count + 1];

	int index = 0;
	for(index = 0;index<count ;index++)
	{
		if( srcText[beginIndex + index ] == TZERO ) break;

		text[index ] = srcText[beginIndex + index];
	}

	text[index] = TZERO;

	String temp = text;
	delete [] text;

	return temp;
}

void CStringHelper::KMPCreateNext(LPCTSTR match, int len, int next[])
{
	int i, j;

	next[0] = 0;
	for (i = 1, j = 0; i < len; i ++) {
		while (j > 0 && match[j] != match[i])
			j = next[j - 1];
		if (match[j] == match[i])
			j ++;
		next[i] = j;
	}
}

// ²éÕÒ×Ö´®
int CStringHelper::KMPFind(LPCTSTR text, int strLen,  LPCTSTR match,
						   int matchLen, int next[])
{
	int i, j;	

	for (i = 0, j = 0; i < strLen; i ++ ) {
		while (j > 0 && text[i] != match[j])
			j = next[j - 1];
		if (text[i] == match[j])
			j ++;
		if (j == matchLen)
			return i + 1 - matchLen;
	}

	return -1;
}

//string CStringHelper::ToAString(const WCHAR * str)
//{
//	if(str==NULL) return std::string();
//
//	size_t len = ::wcslen(str);
//	if(len==0) return std::string();
//
//	USES_CONVERSION_EX;
//	int nConvertedLen = ::WideCharToMultiByte(_acp_ex,0, str, -1, NULL, 0, NULL, NULL);
//
//	char * dest = (char *)CoTaskMemAlloc(nConvertedLen + 1);
//	memset(dest,0,nConvertedLen + 1);
//
//	WideCharToMultiByte(_acp_ex,0, str,-1, dest , len * 4 +2, NULL, NULL);
//
//	std::string s = dest;
//
//	CoTaskMemFree(dest);
//	return s;
//
//}
//
//String CStringHelper::ToWString(const char * str)
//{
//	USES_CONVERSION;
//	return A2W(str);
//
//}

BOOL CStringHelper::IsVariantName(LPCTSTR text)
{
	static BOOL hasParsed = FALSE;
	static CAtlRegExp<> reUrl;
	if(!hasParsed)
	{
		REParseError status = reUrl.Parse(_T("^[_|(a-zA-Z)]+[_|(a-zA-Z)|(0-9)]*$"));

		if (REPARSE_ERROR_OK != status)
		{
			return FALSE;
		}

		hasParsed = TRUE;
	}

	CAtlREMatchContext<> mcUrl;
	return reUrl.Match(text, &mcUrl);	

}

HRESULT CStringHelper::GUID2String(const GUID & guid, String & text)
{
	LPOLESTR temp = NULL;
	HRESULT hr = StringFromCLSID(guid, &temp);	 

	text = W2TSTRING(temp);
	CoTaskMemFree(temp);

	return hr;
}

HRESULT CStringHelper::String2GUID(LPCTSTR text, GUID & guid)
{
	return CLSIDFromString(VARIANT_BSTR(text), &guid);
}

