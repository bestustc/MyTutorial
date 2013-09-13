#include "CriticalSectionMap.h"

CCriticalSectionMap::CCriticalSectionMap()
{
	InitializeCriticalSection(&m_criticalSection);

	m_pCriticalSections = new CriticalSectionMap();

	m_bForceEndSync = FORCE_END_LOCK;
}

CCriticalSectionMap::~CCriticalSectionMap()
{

	EnterCriticalSection(&m_criticalSection);

	if(m_pCriticalSections == NULL) return;	

	CriticalSectionMap::iterator iter = m_pCriticalSections->begin();

	for( iter; iter != m_pCriticalSections->end(); ++iter )
	{
		if( iter->second != NULL )
		{
			ReleaseSection(iter->second);				
			delete iter->second;
		}
	}

	delete m_pCriticalSections;
	m_pCriticalSections = NULL;

	LeaveCriticalSection(&m_criticalSection);
	DeleteCriticalSection(&m_criticalSection);
}

void CCriticalSectionMap::Release( void * p )
{
	EnterCriticalSection(&m_criticalSection);

	if(m_pCriticalSections != NULL)
	{
		LONG key = reinterpret_cast<LONG>(p);

		CriticalSectionMap::iterator iter = m_pCriticalSections->find(key);

		if(iter != m_pCriticalSections->end())
		{
			if(iter->second != NULL)
			{					
				EnterCriticalSection(&iter->second->section);
				LeaveCriticalSection(&iter->second->section);

				DeleteCriticalSection(&iter->second->section);
				delete iter->second;
			}

			m_pCriticalSections->erase(iter);
		}
	}

	LeaveCriticalSection(&m_criticalSection);
}

void CCriticalSectionMap::Trim()
{
	CriticalSectionMap * pTemp = m_pCriticalSections;

	m_pCriticalSections = new CriticalSectionMap();		

	for(CriticalSectionMap::iterator iter = pTemp->begin();iter != pTemp->end(); iter++)
	{
		if(iter->second != NULL && iter->second->HasLocked())
		{
			(*m_pCriticalSections)[iter->first] = iter->second;
		}
		else if(iter->second != NULL)
		{
			DeleteCriticalSection( &iter->second->section);
			delete iter->second;
		}			
	}
	delete pTemp;
	pTemp = NULL;
}

CriticalSection * CCriticalSectionMap::GetCriticalSection( void * p )
{
	EnterCriticalSection(&m_criticalSection);
	CriticalSection * pSection = NULL;

	if(m_pCriticalSections != NULL)
	{
		LONG key = reinterpret_cast<LONG>(p);
		CriticalSectionMap::iterator iter = m_pCriticalSections->find(key);

		if(iter != m_pCriticalSections->end())
		{
			pSection = iter->second;
		}
		else
		{
			if(m_pCriticalSections->size() >= MAX_LOCK_COUNT )
			{
				Trim();
			}

			pSection = new CriticalSection();
			InitializeCriticalSection(&pSection->section);
			pSection->key = p;

			(*m_pCriticalSections)[key] = pSection;
		}		
	}
	LeaveCriticalSection(&m_criticalSection);
	return pSection;
}

void CCriticalSectionMap::ReleaseSection( CriticalSection * pSection )
{

	if(pSection == NULL) return;

	if(pSection->HasLocked())
	{
		if(m_bForceEndSync)
		{
			LeaveCriticalSection(&pSection->section);
		}
		else
		{
			EnterCriticalSection(&pSection->section);
			LeaveCriticalSection(&pSection->section);
		}
	}

	DeleteCriticalSection(&pSection->section);
}
