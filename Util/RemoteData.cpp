#include "RemoteData.h"


CRemoteData::CRemoteData()
{
	m_nType = RDT_EMPTY;
	m_pData = NULL;
}

CRemoteData::CRemoteData(const CRemoteData & other)
{
	m_nType = RDT_EMPTY;
	m_pData = NULL;

	this->operator = (other);
}

CRemoteData::CRemoteData(int count, const BYTE bytes[])
{
	BYTE * pBytes = new BYTE[count + sizeof(int)];

	memcpy_s(pBytes, count +  sizeof(int), &count,  sizeof(int));

	memcpy_s(pBytes + sizeof(int), count, bytes, count);

	m_pData = pBytes;
	m_nType = RDT_BYTES;
}

CRemoteData::CRemoteData(LPCTSTR text)
{
	int len = _tcslen(text);

	TCHAR * pText = new TCHAR[len + 1];

	_tcscpy_s(pText, len +1, text);

	m_pData = pText;
	m_nType = RDT_STRING;
}

CRemoteData::CRemoteData(LPCTSTR firstName, CRemoteData & firstData)
{
	TryAddKeyValue(firstName, firstData);
	RemoteKeyValueMap * pMap = new RemoteKeyValueMap;

	(*pMap)[firstName] = firstData;

	m_pData = pMap;
	m_nType = RDT_KEYVALUES;
}

CRemoteData & CRemoteData::operator()(LPCTSTR name, CRemoteData & data)
{
	return TryAddKeyValue(name, data);
}

CRemoteData & CRemoteData::TryAddKeyValue(LPCTSTR name, CRemoteData & data)
{
	if(m_nType == RDT_KEYVALUES && m_pData != NULL)
	{
		(* reinterpret_cast<RemoteKeyValueMap *>(m_pData) )[name] = data;
	}
	else if(m_nType == RDT_EMPTY && m_pData == NULL)
	{
		RemoteKeyValueMap * pMap = new RemoteKeyValueMap;

		(*pMap)[name] = data;

		m_pData = pMap;
		m_nType = RDT_KEYVALUES;
	}

	return *this;
}


int CRemoteData::TryGetByteCount() const
{
	if(m_nType == RDT_BYTES && m_pData != NULL)
	{
		int i;
		memcpy_s(&i,sizeof(int), m_pData, sizeof(int));
		return i;
	}

	return -1;
}

const BYTE * CRemoteData::TryGetBytes()const
{
	if(m_nType == RDT_BYTES && m_pData != NULL)
	{
		return reinterpret_cast<const BYTE *>(m_pData) + sizeof(int);
	}

	return NULL;
}

LPCTSTR CRemoteData::TryGetText()  const
{
	if(m_nType == RDT_STRING && m_pData != NULL)
	{
		return reinterpret_cast<LPCTSTR>(m_pData);
	}

	return NULL;
}

const RemoteKeyValueMap * CRemoteData::TryGetKeyValues() const
{
	if(m_nType == RDT_KEYVALUES && m_pData != NULL)
	{
		return reinterpret_cast<RemoteKeyValueMap *>(m_pData);
	}
	return NULL;
}

RemoteKeyValueMap * CRemoteData::TryGetKeyValues()
{
	if(m_nType == RDT_KEYVALUES && m_pData != NULL)
	{
		return reinterpret_cast<RemoteKeyValueMap *>(m_pData);
	}

	return NULL;
}

CRemoteData & CRemoteData::operator = (const CRemoteData & other)
{
	if(&other ==this) return *this;

	Dispose();

	switch(other.m_nType)
	{
	case RDT_STRING:

		if(other.m_pData != NULL)
		{
			LPCTSTR text = other.TryGetText();

			int len = _tcslen(text);

			TCHAR * pText = new TCHAR[len + 1];

			_tcscpy_s(pText, len +1, text);

			m_pData = pText;
			m_nType = RDT_STRING;
		}

		break;

	case RDT_BYTES:

		if(other.m_pData != NULL)
		{
			int count = other.TryGetByteCount();
			const BYTE * bytes = other.TryGetBytes();

			BYTE * pBytes = new BYTE[count + sizeof(int)];

			memcpy_s(pBytes, count +  sizeof(int), &count,  sizeof(int));

			memcpy_s(pBytes + sizeof(int), count, bytes, count);

			m_pData = pBytes;
			m_nType = RDT_BYTES;
		}

		break;

	case RDT_KEYVALUES:

		if(other.m_pData != NULL)
		{
			const RemoteKeyValueMap * map = other.TryGetKeyValues(); 

			RemoteKeyValueMap * pMap = new RemoteKeyValueMap;

			for(RemoteKeyValueMap::const_iterator it = map->begin(), end = map->end();
				it != end ;it++)
			{
				(*pMap)[it->first] = it->second;
			}

			m_pData = pMap;
			m_nType = RDT_KEYVALUES;
		}
		break;

	default:
		break;
	}

	return *this;
}

BOOL CRemoteData::operator == (const CRemoteData & other) const
{
	if(&other ==this) return TRUE;
	if(m_nType != other.m_nType) return FALSE;

	if(m_pData == other.m_pData) return TRUE;
	if(m_pData == NULL || other.m_pData == NULL) return FALSE;

	switch(m_nType)
	{
	case RDT_STRING:	
		{
			LPCTSTR text0 = TryGetText();
			LPCTSTR text1 = other.TryGetText();

			return wcscmp(text0,text1) == 0;
		}
	case RDT_BYTES:
		{
			int count0 = TryGetByteCount();
			int count1 = other.TryGetByteCount();

			if(count0 != count1) return FALSE;

			const BYTE * bytes0 = TryGetBytes();
			const BYTE * bytes1 = other.TryGetBytes();

			return ::memcmp(bytes0, bytes0, count0) == 0;		
		}
	case RDT_KEYVALUES:
		{
			const RemoteKeyValueMap * map0 = TryGetKeyValues(); 
			const RemoteKeyValueMap * map1 = other.TryGetKeyValues(); 

			int size0 = map0->size();
			int size1 = map1->size();


			if(size0 != size1) return FALSE;

			for(RemoteKeyValueMap::const_iterator it = map0->begin(), end = map0->end();
				it != end ;it++)
			{
				RemoteKeyValueMap::const_iterator it1 = map1->find(it->first);

				if(it1 == map1->end()) return FALSE;

				if(it1->second != it->second) return FALSE;

			}

			return TRUE;

		}
	default:

		return TRUE;
	}
}

BOOL CRemoteData::operator != (const CRemoteData & other) const
{
	return !( this->operator == (other));
}


void CRemoteData::Dispose()
{
	switch(m_nType)
	{
	case RDT_STRING:

		if(m_pData != NULL)
		{
			delete [] reinterpret_cast<LPTSTR>(m_pData);
		}

		break;

	case RDT_BYTES:

		if(m_pData != NULL)
		{
			delete [] reinterpret_cast<BYTE *>(m_pData);
		}

		break;

	case RDT_KEYVALUES:

		if(m_pData != NULL)
		{
			delete reinterpret_cast<RemoteKeyValueMap *>(m_pData);
		}
		break;

	default:
		break;
	}

	m_nType = RDT_EMPTY;
	m_pData = NULL;
}


CRemoteData::~CRemoteData(void)
{

}