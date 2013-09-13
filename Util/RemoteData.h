#ifndef __UTIL__REMOTE_DATA__H__
#define __UTIL__REMOTE_DATA__H__

#pragma once

#include "UtilHelper.h"

enum RemoteDataType
{
	RDT_EMPTY,
	RDT_STRING,
	RDT_BYTES,
	RDT_KEYVALUES,
};

class CRemoteData;
typedef map<String, CRemoteData> RemoteKeyValueMap;

class CRemoteData
{
	RemoteDataType m_nType;

	void * m_pData;

public:

	CRemoteData();

	CRemoteData(const CRemoteData & other);

	CRemoteData(int count, const BYTE bytes[]);

	CRemoteData(LPCTSTR text);

	CRemoteData(LPCTSTR firstName, CRemoteData & firstData);

	CRemoteData & operator()(LPCTSTR name, CRemoteData & data);

	CRemoteData & TryAddKeyValue(LPCTSTR name, CRemoteData & data);

	RemoteDataType GetDataType() const { return m_nType; }

	int TryGetByteCount() const;

	const BYTE * TryGetBytes() const;

	LPCTSTR TryGetText() const;

	const RemoteKeyValueMap * TryGetKeyValues() const;

	RemoteKeyValueMap * TryGetKeyValues();

	CRemoteData & operator = (const CRemoteData & other);

	BOOL operator == (const CRemoteData & other) const;

	BOOL operator != (const CRemoteData & other) const;

	void Dispose();

	~CRemoteData(void);
};


#endif