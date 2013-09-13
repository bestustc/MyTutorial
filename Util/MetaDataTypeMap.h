#ifndef __UTIL__META_DATA_TYPE_MAP__H__
#define __UTIL__META_DATA_TYPE_MAP__H__


#pragma once

#include "DataProfile.h"
#include "UtilHelper.h"

class CMetaDataTypeMap
{
	hash_map<String, MetaDataType> m_name2Types;
	hash_map<MetaDataType, String> m_type2Names;

private:

	CMetaDataTypeMap(void);

public:

	static CMetaDataTypeMap Instance;

public:


	void GetTypes(vector<MetaDataType> & types) const;

	void GetNames(vector<String> & names) const;


	const String & GetName(MetaDataType type) const;

	MetaDataType GetType(const String & name) const;

	MetaDataType GetType(const type_info & info) const;


	BOOL Contains(const String & name) const;

	BOOL Contains(MetaDataType type) const;

};


#endif