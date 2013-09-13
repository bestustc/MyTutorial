
#ifndef _CRITICALSECTIONMAP_H__
#define _CRITICALSECTIONMAP_H__

#define MAX_LOCK_COUNT 1024
#define FORCE_END_LOCK TRUE

#include <map>
#include <windows.h>
using namespace std;
struct CriticalSection
{
	CRITICAL_SECTION 	section;
	void *			 	key;
	int				 	refCount;

	CriticalSection()
	{
		refCount			= 0;
		section.LockCount	= 0;
	}
	BOOL HasLocked()
	{
		return section.LockCount >=0;
	}
};
class CCriticalSectionMap
{
 public:
      CCriticalSectionMap();
      ~CCriticalSectionMap();

public:
		void 				Release(void * p);
		void 				Trim();

		CriticalSection *	GetCriticalSection(void * p);
		void 				ReleaseSection(CriticalSection * section);

private:
	typedef map<LONG, CriticalSection *> CriticalSectionMap;

	CriticalSectionMap * 				m_pCriticalSections;

	CRITICAL_SECTION	 				m_criticalSection;

	BOOL								m_bForceEndSync;

};
#endif