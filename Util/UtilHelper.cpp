#ifndef __UTIL__UTIL_HELPER__H__
#define __UTIL__UTIL_HELPER__H__

#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <string>
#include <vector>
#include <map>
#include <hash_map>
#include <fstream>
#include <time.h>

using namespace ATL;
using namespace std;
using namespace stdext;

#ifndef CURR_TEMP_TYPE
#define CURR_TEMP_TYPE
#undef CURR_TEMP_TYPE
#endif

#ifndef TODO_MARK
#define TODO_MARK(...)
#endif

#ifdef UNICODE

typedef wstring String;
#define STRLEN(x) wcslen(x)

typedef std::wifstream FileInputStream;
typedef std::wofstream FileOutputStream;

#ifndef _T
#define _T(x) L##x
#endif

#else

typedef string String;
#define STRLEN(x) strlen(x)

typedef std::ifstream FileInputStream;
typedef std::ofstream FileOutputStream;

#ifndef _T
#define _T(x) x
#endif

#endif

#ifndef DEFAULT_LCID
#define DEFAULT_LCID LOCALE_USER_DEFAULT
#endif

#ifndef LONGDOUBLE
#define LONGDOUBLE long double
#endif

#ifndef __cplusplus
#define __cplusplus
#endif

#ifndef TZERO
#define TZERO (_T('\0'))
#endif

#ifndef TDOT
#define TDOT (_T('.'))
#endif

#ifndef TDOLLAR
#define TDOLLAR (_T('$'))
#endif


#ifndef TQUOTES
#define TQUOTES (_T('\"'))
#endif

#ifndef TQUOTE
#define TQUOTE (_T('\''))
#endif

#ifndef TSLASH
#define TSLASH (_T('\\'))
#endif

#ifndef TBACKSLASH
#define TBACKSLASH (_T('/'))
#endif

#ifndef TSPACE
#define TSPACE (_T(' '))
#endif

#ifndef TTAB
#define TTAB (_T('\t'))
#endif

#ifndef TNEGATIVE
#define TNEGATIVE (_T('-'))
#endif

#ifndef TPOSITIVE
#define TPOSITIVE (_T('+'))
#endif

#ifndef TNUMBER0
#define TNUMBER0 (_T('0'))
#endif

#ifndef TNUMBER1
#define TNUMBER1 (_T('1'))
#endif

#ifndef TNUMBER2
#define TNUMBER2 (_T('2'))
#endif

#ifndef TNUMBER3
#define TNUMBER3 (_T('3'))
#endif

#ifndef TNUMBER4
#define TNUMBER4 (_T('4'))
#endif

#ifndef TNUMBER5
#define TNUMBER5 (_T('5'))
#endif

#ifndef TNUMBER6
#define TNUMBER6 (_T('6'))
#endif

#ifndef TNUMBER7
#define TNUMBER7 (_T('7'))
#endif

#ifndef TNUMBER8
#define TNUMBER8 (_T('8'))
#endif

#ifndef TNUMBER9
#define TNUMBER9 (_T('9'))
#endif

#ifndef IM_DLL_PUBLIC
#define IM_DLL_PUBLIC _declspec(dllimport) 
#endif

#ifndef EX_DLL_PUBLIC
#define EX_DLL_PUBLIC _declspec(dllexport) 
#endif

#ifndef DLL_PUBLIC
#define DLL_PUBLIC IM_DLL_PUBLIC 
#endif

#ifndef USE_VAR
#define USE_VAR(x) x
#endif

#ifndef WM_USER_THREAD
#define WM_USER_THREAD (WM_USER + 0x1000)
#endif 

#ifndef WM_QUIT_THREAD_LOOP
#define WM_QUIT_THREAD_LOOP (WM_USER_THREAD - 1)
#endif

#ifndef UNUSED_TYPE
#define UNUSED_TYPE InnerTypedefT<void *>::_Type
#endif

#ifndef DEFAULT
#define DEFAULT(T) (T())
#endif

template<typename T>
struct InnerTypedefT
{
	typedef typename T _Type;
};


#ifndef DEFAULT_EX
#define DEFAULT_EX(T) (InnerTypedefT<T>::_Type())
#endif

#ifndef SET_ZERO
#define SET_ZERO(x) do{ memset( &(x) , 0 , sizeof( (x) ) ); } while(FALSE)
#endif

#ifndef STL_MAP_EXISTS
#define STL_MAP_EXISTS(_map, key) ( (_map).find(key) != (_map).end())
#endif

#ifndef STL_MAP_EARSE
#define STL_MAP_EARSE(_map, key) do { (_map).erase( (_map).find(key) ) ; } while(FALSE)
#endif

#ifndef STL_VECTOR_EARSE
#define STL_VECTOR_EARSE(vec, index) do{ (vec).erase( (vec).begin() + index ) ; } while(FALSE)
#endif

#ifndef VECTOR2ARRAY
#define VECTOR2ARRAY(v)   ( & ( *(v).begin() ) )
#endif

#ifndef STL_SIZE
#define STL_SIZE(c) ( (c).size() )
#endif

#ifndef BIT_IS_TRUE
#define BIT_IS_TRUE(v, b) (((v) & (b)) == (b))
#endif


#ifndef COMPARE_VALUE
#define COMPARE_VALUE(x0,x1) ((x0 < x1) ? -1 : (x0 == x1) ? 0 : 1)
#endif

#ifndef INLINE_COMPARE_OPERATOR
#define INLINE_COMPARE_OPERATOR(op, t)  \
	bool operator op (const t & x) const\
{\
	return Compare(x) op 0;\
}
#endif

#ifndef W2ASTRING
#define W2ASTRING(s) CStringHelper::ToAString(s)
#endif

#ifndef A2WSTRING
#define A2WSTRING(s) CStringHelper::ToWString(s)
#endif


#ifdef UNICODE

#define W2TSTRING(s) (s)
#define A2TSTRING(s) A2WSTRING(s)

#define T2WSTRING(s) (s)
#define T2ASTRING(s) W2ASTRING(s)

#else

#define W2TSTRING(s) W2ASTRING(s);
#define A2TSTRING(s) (s)

#define T2WSTRING(s) A2WSTRING(s)
#define T2ASTRING(s) (s)

#endif

#ifndef LPTSTR2STRING
#define LPTSTR2STRING(s) ( ( (s) == NULL) ? String() : String( T2CW(s) ) )
#endif

#define FILE_LOG_INSTANCE CLog::DefaultInstance

#define LOG_FILE(type,errorCode, ...) FILE_LOG_INSTANCE.Log(type, A2TSTRING(__FILE__).c_str(), (INT64)__LINE__, errorCode, __VA_ARGS__)

#define LOG_FILE_ERROR(errorCode, message,...) LOG_FILE( LT_ERROR, errorCode, message, __VA_ARGS__)
#define LOG_FILE_SERVERE(errorCode, message,...) LOG_FILE( LT_SEVERE, errorCode, message, __VA_ARGS__)
#define LOG_FILE_WARNING(warnCode, message,...) LOG_FILE( LT_WARNING, warnCode, message, __VA_ARGS__)
#define LOG_FILE_INFORMATION(message,...) LOG_FILE( LT_INFORMATION, 0, message, __VA_ARGS__)
#define LOG_FILE_DEBUG(message, particular) LOG_FILE(LT_DEBUG, 0, message, particular)
#define LOG_FILE_EXCEPTION(message, particular) LOG_FILE(LT_EXCEPTION, 0, message, particular)
#define LOG_FILE_SUCCESS(message, ...) LOG_FILE(LT_SUCCESS, 0, message, __VA_ARGS__)


#define LOG_EVENT(id, type, format, ...) CSysEnvHelper::LogEvent(id, type, format, __VA_ARGS__)


#define MAKE_EVENT_IDENTIFIER(ser,facility, code) \
	( (((unsigned long)(ser)) <<(30 -0) ) |  ( ((unsigned long)1) <<(29 -0) ) |  (((unsigned long)(facility)) <<(15-0) ) | ((unsigned long)code) )

#define LOG_EVENT_ERROR(format, ...) LOG_EVENT( MAKE_EVENT_IDENTIFIER(3, FACILITY_NULL, 0) , EVENTLOG_ERROR_TYPE, format, __VA_ARGS__)
#define LOG_EVENT_WARNING(format, ...) LOG_EVENT(MAKE_EVENT_IDENTIFIER(2, FACILITY_NULL, 0), EVENTLOG_WARNING_TYPE, format, __VA_ARGS__)
#define LOG_EVENT_INFORMATION(format, ...) LOG_EVENT(MAKE_EVENT_IDENTIFIER(1, FACILITY_NULL, 0), EVENTLOG_ERROR_TYPE, format, __VA_ARGS__)
#define LOG_EVENT_AUDIT_FAILURE(format, ...) LOG_EVENT(MAKE_EVENT_IDENTIFIER(3, FACILITY_NULL, 0), EVENTLOG_AUDIT_FAILURE, format, __VA_ARGS__)
#define LOG_EVENT_AUDIT_SUCCESS(format, ...) LOG_EVENT(0, EVENTLOG_AUDIT_SUCCESS, format, __VA_ARGS__)
#define LOG_EVENT_SUCCESS(format, ...) LOG_EVENT(0, EVENTLOG_SUCCESS, format, __VA_ARGS__)

template<typename T>
void ArrayMoveToT(vector<T> & vec, int oldIndex, int newIndex)
{
	if(oldIndex != newIndex)
	{
		int dir = oldIndex < newIndex ? 1 : -1;

		T v = vec[oldIndex];
		for(int i=oldIndex; dir == 1? i< newIndex : i> newIndex;i+=dir)
		{
			vec[i] = vec[i + dir];
		}

		vec[newIndex] = v;
	}
}

template<typename T>
void ArrayMoveToT(T * arr , int oldIndex, int newIndex)
{
	if(oldIndex != newIndex)
	{
		int dir = oldIndex < newIndex ? 1 : -1;

		T v = arr[oldIndex];
		for(int i=oldIndex; dir == 1? i< newIndex : i> newIndex;i+=dir)
		{
			arr[i] = arr[i + dir];
		}

		arr[newIndex] = v;
	}
}


#ifndef ARRAY_MOVE_TO
#define ARRAY_MOVE_TO(arr, oldIndex, newIndex) ArrayMoveToT( (arr), (oldIndex), (newIndex) )
#endif

template<typename T>
struct DeletePtr
{	
	static void Dispose(T * ptr)
	{
		if(ptr != NULL) 
			delete ptr;
	}
};

template<typename T>
struct FreePtrT
{
	static void Dispose(T * ptr)
	{
		if(ptr != NULL) 
			free(ptr);
	}
};

template<typename T, typename TDispose = DeletePtr<T> >
struct AutoLifePtrT
{
private:

	T * ptr;

private:

	void Free()
	{
		TDelete::Delete(ptr);
	}

public:

	AutoLifePtrT(T * ptr = NULL)
	{
		this->ptr = ptr;
	}	

	~AutoLifePtrT()
	{
		Free();	
	}

public:

	AutoLifePtrT<T> operator = (T * ptr)
	{
		Free();
		this->ptr = ptr;		
	}

	T * operator -> ()
	{
		return ptr;	
	}

private:

	AutoLifePtrT<T> operator = (AutoLifePtrT<T> & ptr) = 0;
};

#endif
