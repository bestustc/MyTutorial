
#include <sys/timeb.h>
#include "SysEnvHelper.h"
#include "ComHelper.h"
#include "ThreadMessage.h"

CSysEnvHelper::CSysEnvHelper(void)
{
}

CSysEnvHelper::~CSysEnvHelper(void)
{
}
AtlLogEventExProc _AtlLogEventExProc = NULL;
CThreadMessage _ModuleThreadMsg;

String CSysEnvHelper::GetExePath()
{
	TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
	DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 1, MAX_PATH);

	if( dwFLen == 0 || dwFLen == MAX_PATH )
		return FALSE;

	szFilePath[0] = _T('\"');
	szFilePath[dwFLen + 1] = _T('\"');
	szFilePath[dwFLen + 2] = 0;

	return szFilePath;
}

String CSysEnvHelper::GetExeFolder()
{
	String exePath = CSysEnvHelper::GetExePath();

	int pos = exePath.find_last_of(_T("\\"));

	if(pos ==-1) return exePath;

	TCHAR szPath [MAX_PATH + _ATL_QUOTES_SPACE] = { TZERO };

	for(LONG i=0;i<pos;i++)
	{
		szPath [i ] = exePath[i+1];
	}

	return String(szPath);

}

String CSysEnvHelper::GetConfigFolder()
{
	String folder = GetExeFolder();

	return folder + _T("\\Configs");
}

INT64 CSysEnvHelper::GetNowTime()
{
	timeb t;
	ftime(&t);				

	INT64 i=t.time;
	i*=1000;
	i+=t.millitm;

	return i;
}

void CSysEnvHelper::LogEvent(int id,  WORD type, LPCTSTR format, ... ) throw()
{
	if(_AtlLogEventExProc == NULL) return;

	const int LOG_EVENT_MSG_SIZE = 256;

	TCHAR chMsg[LOG_EVENT_MSG_SIZE] = { TZERO };
	va_list pArg;

	va_start(pArg, format);
	_vsntprintf_s(chMsg, LOG_EVENT_MSG_SIZE, LOG_EVENT_MSG_SIZE-1, format, pArg);
	va_end(pArg);

	chMsg[LOG_EVENT_MSG_SIZE - 1] = 0;

	_AtlLogEventExProc(id, chMsg, type);
}

BOOL CSysEnvHelper::IsObjectOnHeap(LPVOID pObject)
{
	BOOL bRet(FALSE);
	DWORD dwHeaps = GetProcessHeaps(0, NULL);
	LPHANDLE pHeaps = new HANDLE[dwHeaps];
	if(pHeaps != NULL)
	{
		MEMORY_BASIC_INFORMATION mi = {0};
		GetProcessHeaps(dwHeaps, pHeaps);

		for(DWORD i=0; i<dwHeaps; ++i)
		{
			VirtualQuery((LPVOID)pHeaps[i], &mi, sizeof(mi));

			if(pObject >= mi.BaseAddress 
				&& (DWORD)pObject < (DWORD)mi.BaseAddress + mi.RegionSize)
			{
				bRet = TRUE;
				break;
			}
		}
	}
	delete []pHeaps;
	return bRet;
}


String CSysEnvHelper::GetSysErrorMessage(DWORD errorCode)
{
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		0,
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	String message = LPTSTR2STRING( (LPCTSTR)lpMsgBuf );

	LocalFree( lpMsgBuf );

	return message;
}


HRESULT CSysEnvHelper::SearchHresult(int value, ...)
{
	va_list ap;

	va_start(ap, value);

	HRESULT hr = S_OK;
	for(LONG i = 0;i<value;i++)
	{
		hr = va_arg(ap,HRESULT);
	}
	va_end(ap);

	return hr;
}
