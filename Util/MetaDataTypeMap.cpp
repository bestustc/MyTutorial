#include "MetaDataTypeMap.h"

#define LOG_TYPE(type, name) \
	m_name2Types[_T(name)] = type;\
	m_type2Names[type] = _T(name);\


CMetaDataTypeMap CMetaDataTypeMap::Instance;

static String DefaultTypeName = _T("null");
static MetaDataType DefaultType = MDT_NULL;

CMetaDataTypeMap::CMetaDataTypeMap(void)
{	
	LOG_TYPE(MDT_NULL, "Null");

	LOG_TYPE(MDT_BOOL, "Bool");

	LOG_TYPE(MDT_INT8 , "Int8");
	LOG_TYPE(MDT_UINT8 , "UInt8");

	LOG_TYPE(MDT_INT16, "Int16");
	LOG_TYPE(MDT_UINT16, "UInt16");

	LOG_TYPE(MDT_INT32, "Int32");
	LOG_TYPE(MDT_UINT32, "UInt32");

	LOG_TYPE(MDT_INT64, "Int64");
	LOG_TYPE(MDT_UINT64, "UInt64");

	LOG_TYPE(MDT_FLOAT, "Float");
	LOG_TYPE(MDT_DOUBLE, "Double");

	LOG_TYPE(MDT_TIME, "Time");
	LOG_TYPE(MDT_DATE, "Date");
	LOG_TYPE(MDT_DATETIME, "Datetime");

	LOG_TYPE(MDT_STRING, "String");
	LOG_TYPE(MDT_BYTES, "Bytes");
	LOG_TYPE(MDT_PROFILE, "Profile");
}



void CMetaDataTypeMap::GetTypes(vector<MetaDataType> & types) const
{
	for(hash_map<MetaDataType, String>::const_iterator it = m_type2Names.begin(),
		end = m_type2Names.end(); it != end; it++)
	{
		types.push_back( it->first);
	}
}

void CMetaDataTypeMap::GetNames(vector<String> & names) const
{
	for(hash_map<MetaDataType, String>::const_iterator it = m_type2Names.begin(),
		end = m_type2Names.end(); it != end; it++)
	{
		names.push_back( it->second);
	}
}

const String & CMetaDataTypeMap::GetName(MetaDataType type) const
{
	hash_map<MetaDataType,String>::const_iterator it = m_type2Names.find(type);

	return it == m_type2Names.end() ? DefaultTypeName : it->second;
}

MetaDataType CMetaDataTypeMap::GetType(const String & name) const
{
	hash_map<String, MetaDataType>::const_iterator it = m_name2Types.find(name);
	return it == m_name2Types.end() ? DefaultType : it->second;
}

MetaDataType CMetaDataTypeMap::GetType(const type_info & info) const
{
	if(info == typeid(bool)) return MDT_BOOL;

	else if(info == typeid(INT8)) return MDT_INT8;
	else if(info == typeid(UINT8)) return MDT_UINT8;

	else if(info == typeid(INT16)) return MDT_INT16;
	else if(info == typeid(UINT16)) return MDT_UINT16;

	else if(info == typeid(INT32)) return MDT_INT32;
	else if(info == typeid(UINT32)) return MDT_UINT32;

	else if(info == typeid(INT64)) return MDT_INT64;
	else if(info == typeid(UINT64)) return MDT_UINT64;

	else if(info == typeid(FLOAT)) return MDT_FLOAT;
	else if(info == typeid(DOUBLE)) return MDT_DOUBLE;

	else if(info == typeid(String)) return MDT_STRING;
	else if(info == typeid(LPCTSTR)) return MDT_STRING;

	else if(info == typeid(BYTE *)) return MDT_BYTES;
	else if(info == typeid(IDataNode *)) return MDT_PROFILE;

	return MDT_NULL;

}


BOOL CMetaDataTypeMap::Contains(const String & name) const
{
	return m_name2Types.find(name) != m_name2Types.end();
}

BOOL CMetaDataTypeMap::Contains(MetaDataType type) const
{
	return m_type2Names.find(type) != m_type2Names.end();
}