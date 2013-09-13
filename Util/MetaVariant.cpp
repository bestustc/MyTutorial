#include "MetaVariant.h"

#include "SafeVector.h"
#include "StringHelper.h"
#include "ComHelper.h"

void CMetaVariant::Init()
{
	memset(&value, 0 ,sizeof(value));

	type = MDT_NULL;
}


CMetaVariant::CMetaVariant(void)
{
	Init();
}

CMetaVariant::CMetaVariant(bool b)
{
	Init();
	type = MDT_BOOL;
	value.b = b;
}

CMetaVariant::CMetaVariant(INT8 i)
{
	Init();
	type = MDT_INT8;
	value.n8 = i;
}

CMetaVariant::CMetaVariant(UINT8 u)
{
	Init();
	type = MDT_UINT8;
	value.u8 = u;
}

CMetaVariant::CMetaVariant(INT16 i)
{
	Init();
	type = MDT_INT16;
	value.n16 = i;
}

CMetaVariant::CMetaVariant(UINT16 u)
{
	Init();
	type = MDT_UINT16;
	value.u16 = u;
}


CMetaVariant::CMetaVariant(INT32 i)
{
	Init();
	type = MDT_INT32;
	value.n32 = i;
}

CMetaVariant::CMetaVariant(UINT32 u)
{
	Init();
	type = MDT_UINT32;
	value.u32 = u;
}

CMetaVariant::CMetaVariant(INT64 i)
{
	Init();
	type = MDT_INT64;
	value.n64 = i;
}

CMetaVariant::CMetaVariant(UINT64 u)
{
	Init();
	type = MDT_UINT64;
	value.u64 = u;
}

CMetaVariant::CMetaVariant(FLOAT f)
{
	Init();
	type = MDT_FLOAT;
	value.f = f;
}

CMetaVariant::CMetaVariant(DOUBLE d)
{
	Init();
	type = MDT_DOUBLE;
	value.d = d;
}


CMetaVariant::CMetaVariant(const tm & t)
{
	Init();

	type = MDT_DATETIME;
	value.t = ::mktime(const_cast<tm *>(&t)) * 1000;
}

CMetaVariant::CMetaVariant(const tm & t, BOOL hasTime, BOOL hasDate)
{
	Init();

	if(hasTime)
	{
		type = hasDate ? MDT_DATETIME : MDT_TIME;
	}
	else
	{
		type = hasDate ? MDT_DATE : MDT_NULL;
	}

	if(type != MDT_NULL)
	{
		value.t = ::mktime( const_cast<tm *> (&t)) * 1000;
	}
}

CMetaVariant::CMetaVariant(time_t t, BOOL hasTime , BOOL hasDate )
{
	Init();

	if(hasTime)
	{
		type = hasDate ? MDT_DATETIME : MDT_TIME;
	}
	else
	{
		type = hasDate ? MDT_DATE : MDT_NULL;
	}

	if(type != MDT_NULL)
	{
		value.t = t;
	}
}


CMetaVariant::CMetaVariant(const tm & t, int ms, BOOL hasTime , BOOL hasDate )
{
	Init();

	if(hasTime)
	{
		type = hasDate ? MDT_DATETIME : MDT_TIME;
	}
	else
	{
		type = hasDate ? MDT_DATE : MDT_NULL;
	}

	if(type != MDT_NULL)
	{
		value.t = ::mktime(const_cast<tm*>(&t)) * 1000 + ms;
	}
}


static void SetString(tagMetaVariant & var, LPCWSTR sz)
{
	var.type = MDT_STRING;

	if(sz==NULL) return;
	size_t size = STRLEN(sz);

	if(size == 0) return;

	WCHAR * temp = (WCHAR *)malloc(sizeof(WCHAR) * (size + 1));
	memset(temp,0, sizeof(WCHAR) * (size + 1));

	memcpy(temp,sz,sizeof(WCHAR) * size);

	temp[size] = _T('\0');

	var.value.sz = temp;
}


static void SetString(tagMetaVariant & var, LPCSTR sz)
{
	SetString(var,A2WSTRING(sz).c_str());	

}

CMetaVariant::CMetaVariant(LPCWSTR sz)
{
	Init();
	SetString(*this,sz);
}

CMetaVariant::CMetaVariant(const String & sz)
{
	Init();
	SetString(*this,sz.c_str());
}

CMetaVariant::CMetaVariant(LPCSTR sz)
{
	Init();
	SetString(*this,sz);
}

static BOOL TryGetBytes(const tagMetaVariant & var, int & count, BYTE * bytes)
{
	if(var.type != MDT_BYTES ) return FALSE;

	if(var.value.bs== NULL)
	{
		count = 0;
	}
	else
	{
		memcpy(&count,var.value.bs, sizeof(int));

		if(count != 0 && bytes != NULL)
		{
			memcpy(bytes, var.value.bs + sizeof(int), count);
		}
	}

	return TRUE;
}

BOOL CMetaVariant::TryGetBytes(int & count, BYTE * bytes) const
{
	return ::TryGetBytes(*this,count,bytes);	
}

static void SetBytes(tagMetaVariant & var, int count, const BYTE * bytes)
{
	var.type = MDT_BYTES;

	if(bytes==NULL || count == 0) return;		

	BYTE * temp = (BYTE *)malloc(count + sizeof(int));
	memset(temp,0, count + sizeof(int));

	memcpy(temp, &count, sizeof(int));

	memcpy(temp + sizeof(int), bytes,count);

	var.value.bs = temp;
}

CMetaVariant::CMetaVariant(int count, const BYTE * bytes)
{
	Init();
	SetBytes(*this,count,bytes);

}

CMetaVariant::CMetaVariant(IDataNode * pNode)
{
	Init();
	type = MDT_PROFILE;
	value.pNode = pNode;
	if(pNode != NULL) pNode->AddRef();
}


CMetaVariant::~CMetaVariant(void)
{
	Dispose();
}

void CMetaVariant::Dispose()
{
	if(type == MDT_BYTES && value.bs != NULL)
	{
		free(value.bs);
	}

	if(type == MDT_STRING && value.sz != NULL)
	{
		free(value.sz);
	}
	if(type == MDT_PROFILE && value.pNode != NULL)
	{
		value.pNode->Release();
	}

	Init();
}


CMetaVariant::CMetaVariant(const tagMetaVariant & other)
{
	*this = other;
}

CMetaVariant::CMetaVariant(const VARIANT & other)
{
	switch(other.vt)
	{
	case VT_BOOL:
		*this = CMetaVariant(other.boolVal == VARIANT_TRUE);
		break;

	case VT_BSTR:
		*this = CMetaVariant(other.bstrVal);
		break;

	case VT_I1:
		*this = CMetaVariant(other.cVal);
		break;

	case VT_I2:
		*this = CMetaVariant(other.iVal);
		break;

	case VT_I4:
		*this = CMetaVariant(other.intVal);
		break;

	case VT_I8:
		*this = CMetaVariant((INT64)other.llVal);
		break;

	case VT_HRESULT:
		*this = CMetaVariant(other.lVal);
		break;

	case VT_INT:
		*this = CMetaVariant(other.intVal);
		break;

	case VT_LPSTR:
		*this = CMetaVariant(other.pcVal);
		break;

	case VT_LPWSTR:
		*this = CMetaVariant(other.bstrVal);
		break;

	case VT_NULL:
		*this = CMetaVariant(NULL);
		break;

	case ::VT_R4:
		*this = CMetaVariant(other.fltVal);
		break;

	case ::VT_R8:
		*this = CMetaVariant(other.dblVal);
		break;

	case ::VT_UI1:
		*this = CMetaVariant(other.bVal);
		break;

	case ::VT_UI2:
		*this = CMetaVariant(other.uiVal);
		break;

	case ::VT_UI4:
		*this = CMetaVariant(other.uintVal);
		break;

	case ::VT_UI8:
		*this = CMetaVariant(other.ullVal);
		break;

	case ::VT_UINT:
		*this = CMetaVariant(other.ullVal);
		break;

	case ::VT_DISPATCH:
	case ::VT_UNKNOWN:

		if(other.punkVal != NULL)
		{
			IDataNode * pNode = NULL;
			other.punkVal->QueryInterface(&pNode);

			*this= CMetaVariant(pNode);
			if(pNode != NULL) pNode->Release();

			break;
		}	

	case ::VT_ARRAY:
		{
			CSafeVector vec;

			HRESULT hr = S_OK;
			do
			{
				hr = vec.Attach(other.parray);
				if(FAILED(hr)) break;

				VARTYPE vType;
				hr = vec.GetVarType(&vType);
				if(FAILED(hr)) break;

				if(vType != VT_UI1) break;

				LONG len = 0;
				hr = vec.GetLength(&len);
				if(FAILED(hr) || len == 0) break;

				BYTE * bytes = new BYTE[len];

				for(LONG i=0;i<len;i++)
				{
					vec.GetAt(i, (void *)(bytes + i));
				}

				*this = CMetaVariant(len, bytes);

				delete [] bytes;

			}while(false);

			SAFEARRAY * temp = NULL;
			vec.Detach(&temp);

			if(FAILED(hr))
			{
				*this = CMetaVariant();
			}

			break;
		}
	default:
		{
			*this = CMetaVariant();
			break;
		}

	}
}


CComVariant CMetaVariant::ToComVariant() const
{
	switch(type)
	{
	case MDT_NULL:
		return CComVariant();

	case MDT_BOOL:
		return CComVariant(value.b);

	case MDT_INT8:
		return CComVariant(value.n8);

	case MDT_UINT8:
		return CComVariant(value.u8);

	case MDT_INT16:
		return CComVariant(value.n16);

	case MDT_UINT16:
		return CComVariant(value.u16);

	case MDT_INT32:
		return CComVariant(value.n32);

	case MDT_UINT32:
		return CComVariant(value.u32);

	case MDT_INT64:
		return CComVariant(value.n64);

	case MDT_UINT64:
		return CComVariant(value.u64);


	case MDT_FLOAT:
		return CComVariant(value.f);

	case MDT_DOUBLE:
		return CComVariant(value.d);

	case MDT_STRING:
		return CComVariant(value.sz);

	case MDT_BYTES:
		{
			int count;		
			if(!TryGetBytes(count,NULL) || count == 0) return CComVariant();

			BYTE * bytes = new BYTE[count];
			TryGetBytes(count,bytes);

			SAFEARRAY * pArray = SafeArrayCreateVectorEx(VT_UI1,0,count,NULL);

			for(LONG i = 0;i<(LONG)count;i++)
			{
				::SafeArrayPutElement(pArray,&i,&bytes[i]);
			}

			delete [] bytes;

			VARIANT var;
			var.vt = VT_ARRAY;
			var.parray = pArray;
			CComVariant bc( pArray );
			return bc;

			return var;
		}
	case MDT_PROFILE:
		{
			return CComVariant(value.pNode);
		}

	default:

		return CComVariant();

	}
}

CMetaVariant & CMetaVariant::operator =(const tagMetaVariant & other)
{
	if(this == &other) return *this;

	Dispose();

	if(other.type == MDT_BYTES)
	{
		int count = 0;
		if(::TryGetBytes(other, count, NULL))
		{
			::SetBytes(*this,count,other.value.bs + sizeof(int));
		}
	}
	else if(other.type == MDT_STRING)
	{
		::SetString(*this,other.value.sz);
	}
	else if(other.type == MDT_PROFILE)
	{
		this->type = MDT_PROFILE;
		this->value = other.value;

		if(value.pNode != NULL) value.pNode ->AddRef();
	}
	else
	{
		type = other.type;
		value = other.value;
	}

	return *this;
}

CMetaVariant & CMetaVariant::operator =(const CMetaVariant & other)
{
	return this->operator =( (const tagMetaVariant &)other);
}

BOOL CMetaVariant::operator ==(const tagMetaVariant & other) const
{
	if(this == &other ) return TRUE;

	if(other.type == MDT_BYTES)
	{
		int count0 = 0;
		int count1 = 0;

		if(::TryGetBytes(other, count0, NULL)
			&& ::TryGetBytes(*this,count1, NULL))
		{
			if(count0 == count1) return TRUE;

			for(int i=0;i<count0;i++)
			{
				if(value.bs[i + sizeof(int)] != 
					other.value.bs[i + sizeof(int)] )
					return FALSE;
			}

			return TRUE;

		}
	}
	else if(other.type == MDT_STRING)
	{
		if(other.value.sz == value.sz) return TRUE;
		if(other.value.sz== NULL || value.sz == NULL) return FALSE;

		return ::wcscmp(other.value.sz,value.sz) == 0;
	}
	else
	{
		return type == other.type && 
			memcmp(&value,&other.value, sizeof(value))==0;
	}

	return FALSE;
}

BOOL CMetaVariant::operator !=(const tagMetaVariant & other) const
{
	return !(*this == other);
}

String CMetaVariant::ToString() const
{
	switch(type)
	{
	case MDT_BOOL:
		{
			return value.b ? String(L"True") : String(L"False");
		}
	case MDT_INT8:
	case MDT_UINT8:

	case MDT_INT16:
	case MDT_UINT16:

	case MDT_INT32:
	case MDT_UINT32:

	case MDT_INT64:
	case MDT_UINT64:

	case MDT_FLOAT:
	case MDT_DOUBLE:
		{
			CComVariant var = ToComVariant();
			HRESULT hr = var.ChangeType(VT_BSTR);
			if(FAILED(hr)) return String();
			return BSTR2STRING(var.bstrVal);
		}


	case MDT_TIME:
	case MDT_DATE:
	case MDT_DATETIME:
		{
			time_t dt=(time_t)(value.t/1000);

			tm tt;
			errno_t err = localtime_s(&tt,&dt);

			if(err)
				return String();

			WCHAR text[128] = { TZERO };

			if(type == MDT_TIME)
				swprintf_s(text,128, L"%02d:%02d:%02d.%03d", tt.tm_hour, tt.tm_min,tt.tm_sec, (int)(value.t%1000));

			if(type == MDT_DATE)

				swprintf_s(text,128, L"%d-%02d-%02d", tt.tm_year + 1900, tt.tm_mon +1, tt.tm_mday);

			if(type == MDT_DATETIME)

				swprintf_s(text,128, L"%d-%02d-%02d %02d:%02d:%02d.%03d", 
				tt.tm_year + 1900, tt.tm_mon +1, tt.tm_mday, tt.tm_hour, tt.tm_min,tt.tm_sec, (int)(value.t%1000));


			return text;
		}

	case MDT_STRING:
		{
			return String(value.sz);
		}
	case MDT_BYTES:
		{
			int count = 0;
			if(!TryGetBytes(count, NULL) || count == 0) return String();

			BYTE * bytes = new BYTE[count + 1];
			memset(bytes, 0, sizeof(BYTE) * (count + 1));

			if(! TryGetBytes(count, bytes))
			{
				delete [] bytes;
				return String();
			}

			WCHAR * strs = new WCHAR [ count * 2 + 2];
			memset(strs, 0 , sizeof(WCHAR) * (count * 2 + 2));

			for(int i=0;i<count;i++)
			{
				int h = (bytes[i] & 0xF0) >> 4;
				int l = (bytes[i] & 0x0f);

				strs[i * 2] = h <10 ? h : h - 10 + L'a';
				strs[i * 2 + l ] = l <10 ? l : l - 10 + L'a';
			}
			String temp = String(strs);

			delete [] bytes;
			delete [] strs;

			return temp;
		}

	case MDT_PROFILE:
		{
			if(value.pNode != NULL)
			{
				CComBSTR text = NULL;
				HRESULT hr = PROFILE_PARSER()->ToFullJson(value.pNode, COM_BOOL(FALSE), &text);
				if(SUCCEEDED(hr)) return String(text.m_str);
			}
			return String();
		}

	default:
		{
			return String();
		}

	}

}

HRESULT CMetaVariant::Parse(MetaDataType type, LPCTSTR text, CMetaVariant & var)
{
	if(text == NULL) return E_FAIL;

	switch(type)
	{
	case MDT_BOOL:
		{
			bool b = CStringHelper::Equals(text, _T("True"))
				|| CStringHelper::Equals(text, _T("Yes")) 
				|| CStringHelper::Equals(text, _T("1")) 
				|| CStringHelper::Equals(text, _T("ец")) 
				|| CStringHelper::Equals(text, _T("ЪЧ"));

			var = CMetaVariant(b);
			return S_OK;
		}

	case MDT_INT8 :
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((INT8)i);
			return S_OK;
		}
	case MDT_UINT8 :
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((UINT8)i);
			return S_OK;
		}

	case MDT_INT16:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((INT16)i);
			return S_OK;
		}
	case MDT_UINT16:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((UINT16)i);
			return S_OK;
		}

	case MDT_INT32:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((INT32)i);
			return S_OK;
		}
	case MDT_UINT32:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((UINT32)i);
			return S_OK;
		}

	case MDT_INT64:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((INT64)i);
			return S_OK;
		}
	case MDT_UINT64:
		{		
			INT64 i;
			if(!CStringHelper::ToInt64(text, &i)) return E_FAIL;

			var = CMetaVariant((UINT64)i);
			return S_OK;
		}

	case MDT_FLOAT:
		{
			DOUBLE d;
			if(!CStringHelper::ToDouble(text, &d)) return E_FAIL;

			var = CMetaVariant((FLOAT)d);
			return S_OK;			
		}
	case MDT_DOUBLE:
		{
			DOUBLE d;
			if(!CStringHelper::ToDouble(text, &d)) return E_FAIL;

			var = CMetaVariant((DOUBLE)d);
			return S_OK;			
		}


	case MDT_TIME:
		{
			tm tt;
			memset(&tt, 0 , sizeof(tm));
			int ms = 0;

			_stscanf_s(text, _T("%02d:%02d:%02d.%03d"), &tt.tm_hour,& tt.tm_min,&tt.tm_sec, &ms);

			var = CMetaVariant(tt,ms, TRUE, FALSE);

			return S_OK;

		}
	case MDT_DATE:
		{
			tm tt;
			memset(&tt, 0 , sizeof(tm));

			_stscanf_s(text, _T("%d-%02d-%02d"), &tt.tm_year , &tt.tm_mon, & tt.tm_mday);

			tt.tm_year -= 1900;
			tt.tm_mon -= 1;

			var = CMetaVariant(tt, FALSE, TRUE);

			return S_OK;
		}
	case MDT_DATETIME:
		{
			tm tt;
			memset(&tt, 0 , sizeof(tm));
			int ms = 0;

			_stscanf_s(text,_T("%d-%02d-%02d %02d:%02d:%02d.%03d"), 
				&tt.tm_year , &tt.tm_mon , &tt.tm_mday, &tt.tm_hour, &tt.tm_min, &tt.tm_sec, &ms);

			tt.tm_year -= 1900;
			tt.tm_mon -= 1;

			var = CMetaVariant(tt,ms, TRUE, FALSE);

			return S_OK;

		}

	case MDT_STRING:
		{
			var = CMetaVariant(text);
			return S_OK;
		}
	case MDT_BYTES:
		{
			int len = STRLEN( text);

			if(len ==0 )
			{
				var = CMetaVariant(0, NULL);
				return S_OK;
			}
			if(len %2 == 1) return E_FAIL;

			BYTE * bytes = new BYTE[len /2 ];

			memset( bytes, 0 , sizeof(BYTE) * (len / 2) );

			for(int i=0,size = len/2 ;i<size;i++)
			{
				int h = tolower( text[ i * 2] );
				int l = tolower( text[ i * 2 + 1] );


				if(h >= _T('0') && h <=_T('9')) h = h - _T('0');
				else if(h >= _T('a') && h <=_T('f')) h = h - _T('a');
				else return E_FAIL;

				if(l >= _T('0') && l <=_T('9')) l = l - _T('0');
				else if(l >= _T('a') && l <=_T('f')) l = l - _T('a');
				else return E_FAIL;

				bytes[i] = (BYTE) ( (h << 4) + l);
			}

			var = CMetaVariant(len /2, bytes);
			delete [] bytes;

			return S_OK;

		}
	case MDT_PROFILE:
		{
			IDataNode * pNode = NULL;

			HRESULT hr = PROFILE_PARSER()->ParseJson( CComBSTR(text), NULL, &pNode);
			if(FAILED(hr)) return hr;

			var = CMetaVariant(pNode);
			pNode->Release();
			return S_OK;
		}

	default:
		{
			return E_FAIL;
		}

	}
}


HRESULT CMetaVariant::Attach(tagMetaVariant * pSrc)
{
	INVALID_NULL(pSrc);

	Dispose();

	memcpy_s(this, sizeof(CComVariant), pSrc, sizeof(tagMetaVariant));
	pSrc->type = MDT_NULL;
	memset(&pSrc->value, 0 ,sizeof(pSrc->value));

	return S_OK;
}

HRESULT CMetaVariant::Detach(tagMetaVariant * pDest)
{
	INVALID_NULL(pDest);

	memcpy_s(pDest, sizeof(tagMetaVariant), this, sizeof(CMetaVariant));

	type = MDT_NULL;
	memset(&value, 0, sizeof(value));

	return S_OK;
}



HRESULT CMetaVariant::CopyTo(bool & b)
{
	EXPECTED_TRUE( type == MDT_BOOL);
	b = value.b;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(INT8 & i)

{
	EXPECTED_TRUE( type == MDT_INT8);
	i = value.n8;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(UINT8 & u)

{
	EXPECTED_TRUE( type == MDT_UINT8);
	u = value.u8;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(INT16 & i)

{
	EXPECTED_TRUE( type == MDT_INT16);
	i = value.n16;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(UINT16 & u)	
{
	EXPECTED_TRUE( type == MDT_UINT16);
	u = value.u16;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(INT32  &i)

{
	EXPECTED_TRUE( type == MDT_INT32);
	i = value.n32;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(UINT32 & u)	
{
	EXPECTED_TRUE( type == MDT_UINT32);
	u = value.u32;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(INT64 & i)	
{
	EXPECTED_TRUE( type == MDT_INT64);
	i = value.n64;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(UINT64 & u)	
{
	EXPECTED_TRUE( type == MDT_UINT64);
	u = value.u64;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(FLOAT & f)	
{
	EXPECTED_TRUE( type == MDT_FLOAT);
	f = value.f;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(DOUBLE  &d)	
{
	EXPECTED_TRUE( type == MDT_DOUBLE);
	d = value.d;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(tm & t, int & ms)	
{
	EXPECTED_TRUE( type == MDT_DATE 
		|| type == MDT_TIME 
		|| type == MDT_DATETIME);

	time_t tt = value.t / 1000;

	localtime_s(&t, &tt);
	ms = (int)value.t %1000;
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(String & sz)
{
	EXPECTED_TRUE( type == MDT_STRING);

	sz = LPTSTR2STRING( value.sz);
	return S_OK;
}

HRESULT CMetaVariant::CopyTo(BYTE bytes[])
{
	EXPECTED_TRUE( type == MDT_BYTES);

	int count = 0;
	if(!TryGetBytes(count, NULL)) return E_FAIL;
	if(TryGetBytes(count,bytes)) return E_FAIL;

	return S_OK;
}

HRESULT CMetaVariant::CopyTo(IDataNode ** pNode)
{
	EXPECTED_TRUE( type == MDT_PROFILE);

	INVALID_NULL(pNode);

	*pNode = value.pNode;

	if(value.pNode != NULL) value.pNode->AddRef();
	return S_OK;
}