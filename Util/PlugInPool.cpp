#include "PlugInPool.h"

#include "SysEnvHelper.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "ComHelper.h"

static String GetPlugInConfigPath()
{
	return CSysEnvHelper::GetConfigFolder() + _T("\\PlugIns.txt");
}


CPlugInPool CPlugInPool::Instance;

HRESULT CPlugInPool::Load()
{
	String path = GetPlugInConfigPath();

	vector<String> lines;
	HRESULT hr = CFileHelper::ReadLines(path.c_str(), lines);
	if(FAILED(hr)) return hr;

	for(vector<String>::const_iterator it = lines.begin() , end = lines.end(); it != end; it++)
	{
		hr = Add(it->c_str(), FALSE);
		if(FAILED(hr)) return hr;
	}

	return S_OK;
}

HRESULT CPlugInPool::Reload()
{
	HRESULT hr = Clear(FALSE);
	if(FAILED(hr)) return hr;
	return Load();
}

HRESULT CPlugInPool::SaveToFile()
{
	return E_NOTIMPL;
}

HRESULT CPlugInPool::Add(LPCTSTR path, BOOL saveToFile)
{
	HMODULE module = LoadLibrary(path);
	if(module == NULL) return CO_E_BAD_PATH;

	IPlugInFactory * factory = NULL;
	PlugInInfo * pInfo  = NULL;

	HRESULT hr = S_OK;

	do
	{
		CreatePlugInFactoryProc proc = (CreatePlugInFactoryProc)
			GetProcAddress(module, W2ASTRING(CREATE_PLUG_IN_FACTORY_PROC_NAME).c_str());

		if(proc == NULL) 
		{
			hr = CO_E_ERRORINDLL;
			break;
		}

		factory = proc();
		if(factory == NULL)
		{
			hr = CO_E_ERRORINDLL;
			break;
		}

		pInfo = new PlugInInfo();
		if(pInfo == NULL)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		CPlugInInfo info ;
		factory->GetInfo(info);

		pInfo->name = info.name;
		pInfo->function = info.function;
		pInfo->author = info.author;
		pInfo->description = info.description;
		pInfo->version = info.version;

		String guid = info.guid;
		if(guid.length() != 0 && guid[0] !=_T('{')) 
		{
			guid = String(_T("{")) + guid + _T("}");
		}
		hr = CLSIDFromString( const_cast<TCHAR*>(guid.c_str()), &pInfo->guid);
		if(FAILED(hr)) break;


		pInfo->path = path;
		pInfo->hModule = module;
		pInfo->pFactory = factory;

		this->m_plugInInfos.push_back(pInfo);

		return S_OK;

	}while(FALSE);

	if(pInfo != NULL) delete pInfo;
	if(factory != NULL) factory->Dispose();
	if(module != NULL) FreeLibrary(module);

	if(saveToFile && SUCCEEDED(hr) )
	{
		return SaveToFile();
	}

	return hr;

}

HRESULT CPlugInPool::Remove(LPCTSTR path, BOOL saveToFile)
{
	for(PlugInInfoVector::iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->path == path ) 
		{
			PlugInInfo * pInfo = *it;

			if(pInfo->pFactory != NULL)
			{
				pInfo->pFactory->Dispose();
				pInfo->pFactory = NULL;
			}

			if(pInfo->hModule != NULL)
			{
				FreeLibrary(pInfo->hModule);
				pInfo->hModule = NULL;
			}

			m_plugInInfos.erase(it);

			delete pInfo;

			if(saveToFile) return SaveToFile();

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CPlugInPool::Clear(BOOL saveToFile)
{
	for(PlugInInfoVector::iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		PlugInInfo * pInfo = *it;

		if(pInfo->pFactory != NULL)
		{
			pInfo->pFactory->Dispose();
			pInfo->pFactory = NULL;
		}

		if(pInfo->hModule != NULL)
		{
			FreeLibrary(pInfo->hModule);
			pInfo->hModule = NULL;
		}
		delete pInfo;

	}

	m_plugInInfos.clear();

	if(saveToFile)
		return SaveToFile();

	return S_OK;

}

int CPlugInPool::Count() const
{
	return (int)m_plugInInfos.size();
}

const vector<PlugInInfo *> & CPlugInPool::List() const
{
	return m_plugInInfos;
}

HRESULT  CPlugInPool::GetCount(int * pVal)
{
	INVALID_NULL(pVal);

	*pVal =(int)m_plugInInfos.size();
	return S_OK;
}

HRESULT  CPlugInPool::List(vector<PlugInInfo *> ** pInfos)
{
	INVALID_NULL(pInfos);
	*pInfos = &m_plugInInfos;

	return S_OK;
}

BOOL CPlugInPool::ExistsName(LPCTSTR name)
{
	for(PlugInInfoVector::const_iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->name == name ) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlugInPool::ExistsGuid(LPCTSTR guid)
{
	PlugInInfo info;
	return SUCCEEDED( GetByGuid(guid,&info) );
}

BOOL CPlugInPool::ExistsPath(LPCTSTR path)
{
	PlugInInfo info;
	return SUCCEEDED( GetByPath(path,&info) );
}


HRESULT CPlugInPool::GetByName(LPCTSTR name, vector<PlugInInfo> * pInfos)
{
	INVALID_NULL(pInfos);

	for(PlugInInfoVector::const_iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->name == name ) 
		{
			pInfos->push_back(**it);
		}
	}

	return S_OK;
}

HRESULT CPlugInPool::GetByGuid(LPCTSTR guid, PlugInInfo * pInfo)
{
	INVALID_NULL(pInfo);

	GUID g;
	HRESULT hr = CLSIDFromString(const_cast<TCHAR*>(guid), &g);
	if(FAILED(hr)) return hr;

	for(PlugInInfoVector::const_iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->guid == g ) 
		{
			*pInfo = **it;
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CPlugInPool::GetByPath(LPCTSTR path, PlugInInfo * pInfo)
{
	INVALID_NULL(pInfo);

	for(PlugInInfoVector::const_iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->path == path ) 
		{
			*pInfo = **it;
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CPlugInPool::GetByFactoryType(const type_info & typeInfo, vector<PlugInInfo> * pInfos)
{
	INVALID_NULL(pInfos);
	for(PlugInInfoVector::const_iterator it = m_plugInInfos.begin(), 
		end = m_plugInInfos.end() ;it != end; it++)
	{
		if( (*it)->pFactory->IsTypeOf(typeInfo)) 
		{
			pInfos->push_back(**it);			
		}
	}

	return S_OK;
}

CPlugInPool::CPlugInPool()
{
	///Load();
}

CPlugInPool::~CPlugInPool()
{
	Clear(FALSE);
}