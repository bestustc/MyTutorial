#ifndef __UTIL_META_VARIANT__H__
#define __UTIL_META_VARIANT__H__

#pragma once

#include "UtilHelper.h"
#include "DataProfile.h"

typedef struct tagMetaVariant
{
	MetaDataType type;

	union 
	{
		bool b;

		INT8 n8;
		UINT8 u8;
		
		INT16 n16;
		UINT16 u16;

		INT32 n32;
		UINT32 u32;

		INT64 n64;
		UINT64 u64;

		FLOAT f;
		DOUBLE d;

		time_t t;		
		
		WCHAR * sz;
		BYTE * bs;

		IDataNode * pNode;

	} value;

} MetaVariant;

struct CMetaVariant : public tagMetaVariant
{	
public:

	void Init();

	CMetaVariant(void);

	CMetaVariant(bool b);

	CMetaVariant(INT8 i);

	CMetaVariant(UINT8 u);

	CMetaVariant(INT16 i);

	CMetaVariant(UINT16 u);

	CMetaVariant(INT32 i);

	CMetaVariant(UINT32 u);

	CMetaVariant(INT64 i);

	CMetaVariant(UINT64 u);

	CMetaVariant(FLOAT f);

	CMetaVariant(DOUBLE d);

	CMetaVariant(const tm & t);

	CMetaVariant(const tm & t, BOOL hasTime , BOOL hasDate );

	CMetaVariant(const time_t t, BOOL hasTime , BOOL hasDate );

	CMetaVariant(const tm & t, int ms, BOOL hasTime , BOOL hasDate );

	CMetaVariant(LPCWSTR sz);

	CMetaVariant(LPCSTR sz);	

	CMetaVariant(const String & sz);	

	CMetaVariant(int count, const BYTE *bytes);

	CMetaVariant(const tagMetaVariant & other);

	CMetaVariant(IDataNode * pNode);

	CMetaVariant(const VARIANT & other);

	CComVariant ToComVariant() const;

	CMetaVariant & operator =(const tagMetaVariant & other);

	CMetaVariant & operator =(const CMetaVariant & other);

	BOOL operator ==(const tagMetaVariant & other) const;

	BOOL operator !=(const tagMetaVariant & other) const;

	BOOL TryGetBytes(int & count, BYTE * bytes) const;

	void Dispose();

	String ToString() const;

	static HRESULT Parse(MetaDataType type, LPCTSTR text, CMetaVariant & var);

	HRESULT Attach(tagMetaVariant * pSrc);

	HRESULT Detach(tagMetaVariant * pDest);	

	~CMetaVariant(void);

	HRESULT CopyTo(bool & b);

	HRESULT CopyTo(INT8 & i);

	HRESULT CopyTo(UINT8 & u);

	HRESULT CopyTo(INT16 & i);

	HRESULT CopyTo(UINT16 & u);

	HRESULT CopyTo(INT32  &i);

	HRESULT CopyTo(UINT32 & u);

	HRESULT CopyTo(INT64 & i);

	HRESULT CopyTo(UINT64 & u);

	HRESULT CopyTo(FLOAT & f);

	HRESULT CopyTo(DOUBLE  &d);

	HRESULT CopyTo(tm & t, int & ms);

	HRESULT CopyTo(String & sz);	

	HRESULT CopyTo(BYTE bytes[]);
	
	HRESULT CopyTo(IDataNode ** pNode);

};


#endif