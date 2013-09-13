#pragma once

#include "UtilHelper.h"
#include "Nullable.h"

enum LogType
{
	LT_SUCCEED,
	LT_ERROR,
	LT_SEVERE,
	LT_WARNING,
	LT_INFORMATION,
	LT_DEBUG,
	LT_EXCEPTION,
};

struct LogInfo
{
	LogType type;
	String message;
	String particular;
	INT64 time;

	LONG errorCode;

	String sourceFile;
	INT64 sourceLine;
	
};

class CLog
{
	String m_szPath;

	FileOutputStream m_stream;

	map<int, CNullable<LogInfo> > m_lastLogs;

public:

	CLog(void);
	~CLog(void);

	static CLog DefaultInstance;

	static const String LOG_FILE_NAME;

	static String GetDefaultPath();

public:

	BOOL Open(LPCTSTR path);

	const String & GetLogPath() const;

	void Close();

	BOOL HasOpen() const;

	const LogInfo * GetLast(LogType type) const;

	BOOL Log(LogInfo info);

	BOOL Log(LogType type, 
		LPCWSTR sourceFile, 
		INT64 sourceLIne, 
		LONG errorCode, 
		LPCWSTR message = NULL , 
		LPCWSTR particular = NULL);
};

