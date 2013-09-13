#ifndef __UTIL__SYS_ENV_HELPER__H__
#define __UTIL__SYS_ENV_HELPER__H__

#pragma once

#include "UtilHelper.h"

class CSysEnvHelper
{
public:
	CSysEnvHelper(void);

	~CSysEnvHelper(void);

public:

	static String GetExeFolder();

	static String GetExePath();

	static String GetConfigFolder();

	static INT64 GetNowTime();

	static void LogEvent(int id,  WORD type, LPCTSTR format, ... ) throw();

	static BOOL IsObjectOnHeap(LPVOID pObject);

	static String GetSysErrorMessage(DWORD errorCode);

	static HRESULT SearchHresult(int value, ...);
	
};

#endif
