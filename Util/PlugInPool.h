#ifndef __UTIL__PLUG_IN_POOL__H__
#define __UTIL__PLUG_IN_POOL__H__

#pragma once

#include "PlugIn.h"


struct PlugInInfo
{
	String name;
	String function;
	String author;
	String description;
	String version;
	GUID guid;

	String path;
	HMODULE hModule;
	IPlugInFactory * pFactory;
};

class CPlugInPool 
{
private:

	typedef vector<PlugInInfo *> PlugInInfoVector;
	PlugInInfoVector m_plugInInfos;

public:

	static CPlugInPool Instance;

public:

	HRESULT Load();

	HRESULT Reload();

	HRESULT Unload() { return Clear(FALSE); }

	HRESULT SaveToFile();

public:

	HRESULT Add(LPCTSTR path, BOOL saveToFile = TRUE);

	HRESULT Remove(LPCTSTR path, BOOL saveToFile = TRUE);

	HRESULT Clear(BOOL saveToFile = TRUE);

	HRESULT GetCount(int * pVal);

	HRESULT List(vector<PlugInInfo *> ** infos) ;

	int Count() const;

	const vector<PlugInInfo *> & List() const;

public:

	BOOL ExistsName(LPCTSTR name);

	BOOL ExistsGuid(LPCTSTR guid);

	BOOL ExistsPath(LPCTSTR path);


public:

	HRESULT GetByName(LPCTSTR name, vector<PlugInInfo> * pInfos);

	HRESULT GetByGuid(LPCTSTR guid, PlugInInfo * pInfo);

	HRESULT GetByPath(LPCTSTR path, PlugInInfo * pInfo);

	HRESULT GetByFactoryType(const type_info & typeInfo, vector<PlugInInfo> * pInfos);

	template<typename T>
	HRESULT GetByFactoryType(vector<PlugInInfo> & infos)
	{
		return GetByFactoryType(typeid(T), &infos);
	}

private:

	CPlugInPool(void);

	~CPlugInPool(void);
};


#endif