
#ifndef _LOCKCONTROLLER_H__
#define _LOCKCONTROLLER_H__
#include "CriticalSectionMap.h"
class LockController
{
 public:
	 LockController( void * pValue, BOOL clearAfterUnlock = TRUE );
     ~LockController();

public:
	BOOL 							TryNext();	 

private:
	void 							Init(void * pValue, BOOL clearAfterUnlock);
	static CCriticalSectionMap *	 __GetCriticalSectionMap();
      
private:
	void* 						m_pCriticalSection;
	BOOL 						m_bHasNext;
	BOOL 						m_bClearAfterUnlock;
};
#endif