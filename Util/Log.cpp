#include "Log.h"

#include "Lock/Lock.h"
#include "SysEnvHelper.h"

CLog::CLog(void)
{
}

CLog::~CLog(void)
{
	Close();
}

CLog CLog::DefaultInstance;

const String CLog::LOG_FILE_NAME = _T("local.log");

String CLog::GetDefaultPath()
{
	String path = CSysEnvHelper::GetExeFolder();
	return path + _T("\\") + LOG_FILE_NAME;
}

String _GetLogTypeString(LogType type)
{
	switch(type)
	{

	case LT_ERROR:
		return _T("错误");

	case LT_SEVERE:
		return _T("严重错误");

	case LT_WARNING:
		return _T("警告");

	case LT_INFORMATION:
		return _T("提示");

	case LT_DEBUG:
		return _T("调试信息");

	case LT_EXCEPTION:
		return _T("程序异常");

	case LT_SUCCEED:
	default:
		return _T("正常");
	}
}

String _GetTimeString(INT64 t)
{	
	time_t dt=(time_t)(t/1000);

	tm tt;
	errno_t err = localtime_s(&tt,&dt);

	if(err)
		return _T("");

	TCHAR text[128] = { TZERO };

	_stprintf_s(text,128, _T("%d-%02d-%02d %02d:%02d:%02d.%03d"), 
		tt.tm_year + 1900, tt.tm_mon +1, tt.tm_mday, tt.tm_hour, tt.tm_min,tt.tm_sec, (int)(t%1000));

	return text;
}


BOOL CLog::Open(LPCTSTR path)
{
	m_stream.imbue(locale("chs"));
	m_stream.open(path, std::ios_base::app);
	m_szPath = path;

	return m_stream.is_open();
}

const String & CLog::GetLogPath() const
{
	return m_szPath;
}

void CLog::Close()
{
	if(HasOpen())
		m_stream.close();
}

BOOL CLog::HasOpen() const
{
	return m_stream.is_open();
}

const LogInfo * CLog::GetLast(LogType type) const
{	
	LOCK(this, FALSE)
	{
		map<int, CNullable<LogInfo> > ::const_iterator it = m_lastLogs.find((int)type);

		if(it != m_lastLogs.end() ) 
		{
			const CNullable<LogInfo> & info = it->second;// m_lastLogs[(int)type];
			return info.HasValue() ? &info.GetValue() : NULL;
		}
	}

	return NULL;
}

BOOL CLog::Log(LogInfo info)
{
	if(!HasOpen()) return FALSE; 
	String typeString = _GetLogTypeString(info.type);

	TCHAR codeString[32] ={ TZERO };
	_itot_s(info.errorCode, codeString, 32, 10);

	String timeString = _GetTimeString(info.time);

	TCHAR lineString[32] = { TZERO };
	_i64tot_s(info.sourceLine, lineString, 32, 10);

	String text;
	text.append(_T("======================================================\r\n"));

	text.append(_T("级别:")).append(typeString).append(_T("\r\n"));
	text.append(_T("代码:")).append(codeString).append(_T("\r\n"));
	text.append(_T("消息:")).append(info.message).append(_T("\r\n"));
	text.append(_T("时间:")).append(timeString).append(_T("\r\n"));
	text.append(_T("文件:")).append(info.sourceFile).append(_T("\r\n"));
	text.append(_T("行号:")).append(lineString).append(_T("\r\n"));
	text.append(_T("描述:")).append(info.particular).append(_T("\r\n"));

	LOCK(this, FALSE)
	{
		m_stream<< text<<endl;
		m_stream.flush();

		m_lastLogs[(int)info.type] = info;
	}

	return TRUE;
}

BOOL CLog::Log(LogType type, LPCWSTR sourceFile, INT64 sourceLine, LONG errorCode, LPCWSTR message, LPCWSTR particular)
{
	LogInfo info ;
	info.type = type;
	info.sourceFile = LPTSTR2STRING(sourceFile);
	info.sourceLine = sourceLine;
	info.errorCode = errorCode;
	info.message = LPTSTR2STRING(message);
	info.particular = LPTSTR2STRING(particular);

	info.time = CSysEnvHelper::GetNowTime();

	return Log(info);
}