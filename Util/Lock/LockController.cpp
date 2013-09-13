#include "LockController.h"

LockController::LockController( void * pValue, BOOL clearAfterUnlock /*= TRUE*/ ):m_bHasNext(TRUE)
{
	Init(pValue,clearAfterUnlock);

}

LockController::~LockController()
{

	if(m_pCriticalSection != NULL)
	{
		CriticalSection * pSection  = (CriticalSection *)m_pCriticalSection;

		LeaveCriticalSection(&pSection->section);
		if(m_bClearAfterUnlock)
		{
			//Add RefCount to prevent dupliacate delete for the same object  
			pSection->refCount--;
			if ( 0 == pSection->refCount )
			{
				__GetCriticalSectionMap()->Release(pSection->key);

			}
		}

	}
}

void LockController::Init( void * pValue, BOOL clearAfterUnlock )
{

	CriticalSection * pSection = __GetCriticalSectionMap()->GetCriticalSection(pValue);
	//Add RefCount
	pSection->refCount++;
	if( pSection != NULL )
	{
		EnterCriticalSection(&pSection->section);	
	}
	m_pCriticalSection = pSection;
	m_bClearAfterUnlock = clearAfterUnlock;
}

BOOL LockController::TryNext()
{
	BOOL bTemp = m_bHasNext;
	m_bHasNext = FALSE;
	return bTemp;
}

CCriticalSectionMap * LockController::__GetCriticalSectionMap()
{
	static CCriticalSectionMap criticalSectionMap;
	return &criticalSectionMap;

}

