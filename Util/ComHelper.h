#ifndef __UTIL__COM_HELPER__H__
#define __UTIL__COM_HELPER__H__

#pragma once

#include "UtilHelper.h"

#ifndef COM_BOOL
#define COM_BOOL(x) ((x) ? VARIANT_TRUE : VARIANT_FALSE)
#endif

#ifndef COM_IS_TRUE
#define COM_IS_TRUE(x)  ( (x) == VARIANT_TRUE )
#endif

#ifndef COM_IS_FALSE
#define COM_IS_FALSE(x)  ( (x) != VARIANT_TRUE )
#endif

#ifndef INVALID_NULL
#define INVALID_NULL(x) do { if( (x) == NULL) return E_INVALIDARG; } while(FALSE)
#endif

#ifndef POINTER_NULL
#define POINTER_NULL(x) do { if( (x) == NULL) return E_POINTER; } while(FALSE)
#endif

#ifndef EXPECTED_TRUE
#define EXPECTED_TRUE(x) do { if( !(x) ) return E_UNEXPECTED; } while(FALSE)
#endif

#ifndef INVALID_INDEX
#define INVALID_INDEX(x,s)  do{ if( (x) < 0 || (x) >= (s) ) return DISP_E_BADINDEX; } while(FALSE)
#endif

#ifndef INVALID_RANGE
#define INVALID_RANGE(beginIndex, count, capacity) \
	do { if( (beginIndex) < 0 || (beginIndex) + (count) > (capacity) || (count) <=0 ) return DISP_E_BADINDEX;} while(FALSE)
#endif

#ifndef INVALID_VAR_NAME
#define INVALID_VAR_NAME(name) do { if(!CStringHelper::IsVariantName(name)) return E_UNEXPECTED; } while(FALSE)
#endif

#ifndef MAKE_GUID
#define MAKE_GUID(l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
		{l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif


#ifndef BSTR2STRING
#define BSTR2STRING(s) ( ( (s) == NULL) ? String() : String(s) )
#endif

#ifndef STRING2BSTR
#define STRING2BSTR(s) ( SysAllocString( (s).c_str() ) )
#endif

#ifndef FREE_BSTR
#define FREE_BSTR(s) do { if( (s) != NULL ) SysFreeString(s); (s) = NULL; } while(FALSE)
#endif

#ifndef FREE_SAFEARRAY
#define FREE_SAFEARRAY(s) do { if( (s) != NULL ) SafeArrayDestroy(s); (s) = NULL; } while(FALSE)
#endif

#ifndef COM_FAILED
#define COM_FAILED(x, ...) do{ HRESULT __hr = (x); if(FAILED(__hr))  { __VA_ARGS__;  return __hr; }}while(FALSE)
#endif

#ifndef OWNER_POINTER
#define OWNER_POINTER(OwnerType, member, pVal)   ( reinterpret_cast<OwnerType *>((size_t)(pVal) - offsetof(OwnerType, member)) )
#endif

#ifndef VARIANT_DEFAULT
#define VARIANT_DEFAULT CComVariant()
#endif

#ifndef VARIANT_BSTR
#define VARIANT_BSTR(x) (CComBSTR(x).m_str)
#endif



#ifndef COM_CONTAINS

#define USE_COM_CONTAINS \
	VARIANT_BOOL __useConstainVariant = COM_BOOL(FALSE); \
	(void)__useConstainVariant;\

#define COM_CONTAINS(proc, ...)	\
	( __useConstainVariant = COM_BOOL(FALSE), proc(__VA_ARGS__,&__useConstainVariant), COM_IS_TRUE(__useConstainVariant) )

#endif

#ifndef COM_GETSTRING

#define USE_COM_GETSTRING \
	CComBSTR __useGetStringVariant;\
	(void)__useGetStringVariant\

#define COM_GETSTRING(proc, ...) \
	( proc(__VA_ARGS__, &__useGetStringVariant), BSTR2STRING(__useGetStringVariant.m_str) )

#endif

#ifndef PROPERTY_GET
#define PROPERTY_GET(x) (x)
#endif

#ifndef PROPERTY_PUT
#define PROPERTY_PUT(x) do { x = newVal; } while(FALSE)
#endif

#ifndef PROPERTY_GET_WSTRING
#define PROPERTY_GET_WSTRING(x) SysAllocString((x).c_str())
#endif

#ifndef PROPERTY_PUT_WSTRING
#define PROPERTY_PUT_WSTRING(x)  do{ x = newVal == NULL ? String() : String(newVal); } while(FALSE)
#endif

#ifndef PROPERTY_GET_UNKNOWN
#define PROPERTY_GET_UNKNOWN(x) ( (x != NULL) ? x->AddRef(): 0 , x )
#endif

#ifndef PROPERTY_PUT_UNKNOWN
#define PROPERTY_PUT_UNKNOWN(x)  do { if( newVal != NULL ) newVal->AddRef(); if( (x) != NULL) (x)->Release(); (x) = newVal; } while(FALSE)
#endif


#ifndef INLINE_GET_PROPERTY
#define INLINE_GET_PROPERTY(propertyName, type, oper)\
STDMETHOD(get_##propertyName)(type * pVal)\
{\
	INVALID_NULL(pVal);\
	*pVal = oper;\
	return S_OK;\
}
#endif

#ifndef INLINE_PUT_PROPERTY
#define INLINE_PUT_PROPERTY(propertyName, type, oper)\
STDMETHOD(put_##propertyName)(type newVal)\
{	\
	oper;\
	return S_OK;\
}
#endif

#define HRESULT_FROM_REGEX_ERROR(x)  CSysEnvHelper::SearchHresult(x, \
	S_OK, \
	E_OUTOFMEMORY,\
	DISP_E_EXCEPTION, \
	DISP_E_EXCEPTION, \
	DISP_E_EXCEPTION, \
	DISP_E_EXCEPTION, \
	DISP_E_BADINDEX, \
	E_INVALIDARG, \
	TYPE_E_OUTOFBOUNDS, \
	TYPE_E_OUTOFBOUNDS,	\
	E_INVALIDARG \
	)



#ifdef _ATL_FREE_THREADED
#undef _ATL_FREE_THREADED
#endif

#ifndef DEFAULT_THREAD_MODEL

	#ifdef _ATL_FREE_THREADED
		#define DEFAULT_THREAD_MODEL CComMultiThreadModel
	#else
		#define DEFAULT_THREAD_MODEL CComSingleThreadModel
	#endif

//#define DEFAULT_THREAD_MODEL CComSingleThreadModel

#endif

template<
typename TBase, 
typename TInterface, 
typename TInterfaceEx = TInterface,
typename TThread = DEFAULT_THREAD_MODEL
>
struct ATL_NO_VTABLE CAtlObjectT : 
	public CComObjectRootEx<TThread> ,
	public CComCoClass<TBase, NULL>,
	public TInterfaceEx
{	
	typedef CAtlObjectT<TBase, TInterface, TInterfaceEx, TThread> TAtlObject;
	typedef TAtlObject TParent;

	BEGIN_COM_MAP(TBase)
		COM_INTERFACE_ENTRY(TInterface)
	END_COM_MAP()

	DECLARE_CLASSFACTORY_AUTO_THREAD();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:

	HRESULT FinalConstruct()
	{
		return OnInitialize();
	}

	void FinalRelease()
	{
		OnDispose();
	}	

public:

	STDMETHOD(OnInitialize)()
	{
		return S_OK;
	}

	STDMETHOD(OnDispose)()
	{
		return S_OK;
	}
};

template<
typename TBase, 
typename TInterface, 
typename TInterfaceEx = TInterface,
typename const IID* PIID = &__uuidof(TInterface),
typename TThread = DEFAULT_THREAD_MODEL
>
struct ATL_NO_VTABLE CAtlDispatchT :
	public CComObjectRootEx<TThread> ,
	public CComCoClass<TBase, NULL>,
	public IDispatchImpl<TInterfaceEx, PIID>
{	

	typedef CAtlDispatchT<TBase,TInterface,TInterfaceEx, PIID,  TThread> TAtlDispatch;
	typedef TAtlDispatch TParent;

	BEGIN_COM_MAP(TBase)
		COM_INTERFACE_ENTRY(TInterface)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_CLASSFACTORY_AUTO_THREAD();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:

	HRESULT FinalConstruct()
	{
		return OnInitialize();
	}

	void FinalRelease()
	{
		OnDispose();
	}	

public:

	STDMETHOD(OnInitialize)()
	{
		return S_OK;
	}

	STDMETHOD(OnDispose)()
	{
		return S_OK;
	}
};


class ATL_NO_VTABLE CInnerAggregateSubstitute:
	public CAtlDispatchT<CInnerAggregateSubstitute, IDispatch>
{

	IUnknown * m_pUnk;

public:

	STDMETHOD(OnInitialize)()
	{
		m_pUnk = NULL;
		return S_OK;
	}

	STDMETHOD(OnDispose)()
	{
		if(m_pUnk != NULL)
		{
			m_pUnk->Release();
			m_pUnk = NULL;
		}
		return S_OK;
	}

	STDMETHOD(_InternalQueryInterface)(REFIID riid, void** ppvObject)
	{
		INVALID_NULL(ppvObject);

		if(riid == IID_IUnknown || riid == IID_IDispatch)
		{
			if(m_pUnk != NULL)
			{
				*ppvObject = m_pUnk;				
			}else
			{
				*ppvObject = this;
				this->AddRef();
			}
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	static HRESULT CreateInstance2(IUnknown * substituteUnk, IUnknown ** ppvObject)
	{
		HRESULT hr = CreateInstance(ppvObject);
		if(FAILED(hr)) return hr;

		dynamic_cast<CInnerAggregateSubstitute *>(*ppvObject)->m_pUnk = substituteUnk;

		if(substituteUnk != NULL) substituteUnk->AddRef();

		return S_OK;
	}
};


#define BEGIN_QUERYINTERFACE(tBase)\
	STDMETHOD (_InnerQueryInterface) (REFIID riid, void** ppvObject)\
{\
	INVALID_NULL(ppvObject);\
	*ppvObject = NULL;\
	if(riid == IID_IUnknown)\
{\
	*ppvObject =reinterpret_cast <IUnknown *>((CComInnerObject<tBase> *)(this) );\
	_InnerAddRef();\
	return S_OK;\
} else


#define COM_QUERYINTERFACE(t) 	if(riid == __uuidof(t))\
{\
	*ppvObject = dynamic_cast<t *>( static_cast<TDefaultInterface *>(this) );\
	if(*ppvObject == NULL) return E_NOINTERFACE;\
	_InnerAddRef();\
	return S_OK;\
} else

#define COM_AGG_QUERYINTERFACE(t, inVar) if(riid == __uuidof(t))\
{\
	return inVar->QueryInterface(riid, ppvObject);\
}\

#define END_QUERYINTERFACE()  {	return E_NOINTERFACE; } }


template<typename TBase>
struct ATL_NO_VTABLE CComDispatchUsingFactory;

template<typename TBase>
class ATL_NO_VTABLE CComInnerObject
{
protected:

	BEGIN_INTERFACE

		STDMETHOD (_InnerQueryInterface) ( REFIID riid,
		__RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) = 0;

		virtual ULONG STDMETHODCALLTYPE _InnerAddRef( void)
		{
			return ::InterlockedIncrement(&m_nRefCount);  
		}

		virtual ULONG STDMETHODCALLTYPE _InnerRelease( void)
		{
			ULONG  nCount  = 0;   
			nCount = ::InterlockedDecrement(&m_nRefCount);   

			if (nCount == 0)   
			{
				TBase * pThis = static_cast<TBase *>(this);
				pThis->FinalRelease();
				delete pThis;   
			}
			return nCount;   
		}

	END_INTERFACE

protected:

	volatile LONG m_nRefCount;
	IUnknown * m_pOuterUnknown;		

	CComInnerObject() : m_nRefCount(0), m_pOuterUnknown(NULL) {}

	virtual ~CComInnerObject() {}

};


template<typename T>
struct ATL_NO_VTABLE CComFactoryT
	: public IClassFactory
{
	friend struct CComDispatchUsingFactory<T>;

private:

	typedef IClassFactory TDefaultInterface;

	CComFactoryT(){}

	struct T2 : public T
	{	
		friend struct CComFactoryT;

		STDMETHOD_(ULONG,AddRef)( void)
		{
			if(m_pOuterUnknown != NULL) return m_pOuterUnknown->AddRef();
			else return _InnerAddRef(); 
		}

		STDMETHOD_(ULONG, Release)( void)
		{
			if(m_pOuterUnknown != NULL) return m_pOuterUnknown->Release();
			else return _InnerRelease(); 	
		}

		STDMETHOD (QueryInterface) (REFIID riid, void** ppvObject) 
		{
			if(m_pOuterUnknown != NULL) return m_pOuterUnknown->QueryInterface(riid, ppvObject);
			else return _InnerQueryInterface(riid, ppvObject); 	
		}	
	};

public:

	STDMETHOD (QueryInterface) (REFIID riid, void** ppvObject) 
	{
		INVALID_NULL(ppvObject);
		*ppvObject = NULL;
		if(riid == IID_IUnknown || riid == IID_IClassFactory)
		{
			*ppvObject =static_cast <IClassFactory *>((this) );
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	STDMETHOD_(ULONG, AddRef)(void) { return 1; };
	STDMETHOD_(ULONG, Release)(void) { return 1; }	

	STDMETHOD (CreateInstance) (LPUNKNOWN punkOuter, REFIID iid, void **ppvObject)
	{
		INVALID_NULL(ppvObject);

		if (punkOuter != NULL && iid != IID_IUnknown)
			return CLASS_E_NOAGGREGATION;

		T2 * t = new T2;
		if(t==NULL) return E_OUTOFMEMORY;

		t->m_pOuterUnknown = punkOuter;		
		t->_InnerAddRef();

		HRESULT hr = t->FinalConstruct();
		if(FAILED(hr)) 
		{
			t->_InnerRelease();
			return hr;
		}		

		hr = t->_InnerQueryInterface(iid, ppvObject);		
		t->_InnerRelease();		

		return hr;
	}

	STDMETHOD(LockServer)(BOOL fLock) { return E_FAIL; };

};

template<typename TBase>
struct ATL_NO_VTABLE CComDispatchUsingFactory
{
	static CComFactoryT<TBase> s_factory;
};

template<typename TBase>
CComFactoryT<TBase> CComDispatchUsingFactory<TBase>::s_factory;


template<
typename TBase, 
typename TInterface, 
typename TInterfaceEx = TInterface,
typename const IID* PIID = &__uuidof(TInterface)
>
struct ATL_NO_VTABLE CComDispatchT :
	public CComDispatchUsingFactory<TBase>,
	public IDispatchImpl<TInterfaceEx, PIID>,
	public CComInnerObject<TBase>
{	
public:

	typedef CComDispatchT<TBase,TInterface,TInterfaceEx, PIID> TComDispatch;
	typedef TComDispatch TParent;

	static HRESULT GetClassFactory(IClassFactory ** pFactory)
	{
		INVALID_NULL(pFactory);
		pFactory = &s_factory;
		s_factory.AddRef();
		return S_OK;
	}

	typedef TInterface TDefaultInterface;

public:

	static HRESULT CreateInstance(IUnknown* punkOuter, REFIID riid, void ** ppvObject)
	{
		return s_factory.CreateInstance(punkOuter, riid, ppvObject);
	}

	template <class Q>
	static HRESULT CreateInstance(IUnknown* punkOuter, Q** ppvObject)
	{
		return CreateInstance(punkOuter, __uuidof(Q), (void**) ppvObject);
	}

	template <class Q>
	static HRESULT CreateInstance(Q** ppvObject)
	{
		return CreateInstance(NULL, __uuidof(Q), (void**)ppvObject);
	}

public:

	BEGIN_QUERYINTERFACE(TBase)			
		COM_QUERYINTERFACE(IDispatch)
		COM_QUERYINTERFACE(TInterface)
	END_QUERYINTERFACE()

	HRESULT FinalConstruct()
	{
		return OnInitialize();
	}

	void FinalRelease()
	{
		OnDispose();
	}	

public:	

	STDMETHOD(OnInitialize)()
	{
		return S_OK;
	}

	STDMETHOD(OnDispose)()
	{
		return S_OK;
	}

};

namespace ATL
{
	template <class T, UINT nServiceNameID>
	class ATL_NO_VTABLE CAtlServiceModuleT ;
}

typedef void ( *AtlLogEventExProc)(int id, LPCTSTR pszMessage, WORD type) throw();

extern AtlLogEventExProc _AtlLogEventExProc;

class CThreadMessage;
extern CThreadMessage _ModuleThreadMsg;

template<typename T,UINT nServiceNameID>
class ATL_NO_VTABLE CAtlAutoServiceModuleT : 
	public CAtlServiceModuleT< T, nServiceNameID>
{
public:

	typedef CAtlServiceModuleT< T, nServiceNameID> TServiceModule;
	typedef CAtlAutoServiceModuleT<T, nServiceNameID> TParent;

public:

#define DECLARE_STARTTYPE(nStartType) \
	DWORD GetStartType()\
	{\
	return (nStartType);\
	  }

#define DECLARE_DESCRIPTION(description)\
	String GetServerDescription()\
	{\
	return  LPTSTR2STRING(VARIANT_BSTR(description));\
	  }

	DECLARE_STARTTYPE(SERVICE_AUTO_START)
	DECLARE_DESCRIPTION("")

protected:	

	static void _LogEventEx(int id, LPCTSTR pszMessage, WORD type) throw ()
	{
		T * pT = static_cast<T *>(_pAtlModule);
		pT->LogEventEx(id, pszMessage, type);
	}

	struct WorkThreadInfo
	{
		HANDLE threadHandle;
		DWORD threadID;
		void * pThis;
	};

	CAtlAutoServiceModuleT(){}

public:

	STDMETHOD(PreWorkThreadLoop)(HANDLE threadHandle, DWORD threadID)
	{
		return S_OK;
	}
	
	STDMETHOD(PostWorkThreadLoop)(HANDLE threadHandle, DWORD threadID)
	{
		return S_OK;
	}
	
	HRESULT OnWorkThread(HANDLE threadHandle, DWORD threadID)
	{
		if(!_ModuleThreadMsg.Initialize()) return E_FAIL;
		
		HRESULT hr = PreWorkThreadLoop(threadHandle, threadID);
		if(FAILED(hr)) return hr;

		_ModuleThreadMsg.LoopMessages();
						
		return PreWorkThreadLoop(threadHandle, threadID);
	}

	static DWORD WINAPI WorkThreadCallback(LPVOID lpParameter)
	{
		if(lpParameter == NULL) return -1;

		WorkThreadInfo * pInfo = (WorkThreadInfo *)lpParameter;

		T * pThis = (T *)(pInfo->pThis);
		HANDLE threadHandle = pInfo->threadHandle;
		DWORD threadID = pInfo->threadID;

		delete pInfo;

		if(pThis == NULL) return -1;

		HRESULT hr = T::InitializeCom();

		if(FAILED(hr)) 
		{
			LOG_FILE_ERROR(0, _T("工作线程初始化COM失败!"));
			return -1;
		}

		hr = pThis->OnWorkThread(threadHandle, threadID);
		
		LOG_FILE_INFORMATION(_T("工作线程退出!"));

		if(FAILED(hr))
		{
			LOG_FILE_ERROR(0, _T("工作线程回调发生错误!"));
		}

		T::UninitializeCom();

		return FAILED(hr) ? -1 : 0;
	}

	int WinMain(int nShowCmd) throw()
	{
		_AtlLogEventExProc = T::_LogEventEx;

		CLog::DefaultInstance.Open( CLog::GetDefaultPath().c_str());
		return TServiceModule::WinMain(nShowCmd);
	}

	HRESULT PreMessageLoop(int nShowCmd) throw()
	{
		HRESULT hr = TServiceModule::PreMessageLoop(nShowCmd);
		if(FAILED(hr)) return hr;	

		WorkThreadInfo * pInfo = new WorkThreadInfo();

		pInfo->pThis = this;
		pInfo->threadHandle = NULL;
		pInfo->threadID = 0;

		pInfo->threadHandle = CreateThread(NULL,0,T::WorkThreadCallback, pInfo, 0, &pInfo->threadID);

		if(pInfo->threadHandle == NULL)
		{
			DWORD lastError = GetLastError();
			hr = HRESULT_FROM_WIN32(lastError);		
			delete pInfo;

			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				lastError,
				0,
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);

			LOG_FILE_ERROR(0, (LPCTSTR)lpMsgBuf);

			LocalFree( lpMsgBuf );
		}

		return hr;
	}

	inline HRESULT RegisterAppId(bool bService = false) throw()
	{
		HRESULT hr = TServiceModule::RegisterAppId(bService);		
		if(FAILED(hr)) return hr;

		if (bService && IsInstalled())
		{       
			SC_HANDLE hSCM = ::OpenSCManagerW(NULL, NULL, SERVICE_CHANGE_CONFIG);
			SC_HANDLE hService = NULL;
			if (hSCM == NULL)
			{
				return AtlHresultFromLastError();
			}
			else
			{
				hService = ::OpenServiceW(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
				if (hService != NULL)
				{
					T * p = static_cast<T *>(this);

					DWORD startType = p->GetStartType();
					if (! ChangeServiceConfig( hService, SERVICE_NO_CHANGE, startType, SERVICE_NO_CHANGE,
						NULL, NULL, NULL, NULL,NULL, NULL, NULL) )          
					{
						//hr = AtlHresultFromLastError();
					}

					String description = p->GetServerDescription();

					if(!description.empty())
					{
						SERVICE_DESCRIPTION sdBuf = { const_cast<TCHAR *>(description.c_str()) };

						if( ! ChangeServiceConfig2W(hService, SERVICE_CONFIG_DESCRIPTION, &sdBuf))
						{
							//hr = AtlHresultFromLastError();
						}
					}					
				}
				else
					hr = AtlHresultFromLastError();
				::CloseServiceHandle(hSCM);
			}
		}

		if(FAILED(hr)) return hr;
		return S_OK;
	}
};

class CAtlCopyBSTR
{
public:
	static HRESULT copy(BSTR* p1, const BSTR* p2) 
	{
		if(p2==NULL) return S_OK;

		*p1 = const_cast<BSTR>(*p2);// SysAllocString(*p2);

		return S_OK;
	}
	static void init(BSTR* p) 
	{
		*p = NULL;
	}

	static void destroy(BSTR* p) 
	{
		SysFreeString(*p);
	}
};

template<typename T>
struct ATL_NO_VTABLE CAtlObjectLifeT
	: public CAtlObjectT<CAtlObjectLifeT<T>,IUnknown>
{
	T * m_pData;
	void (*m_pReleaseDataProc)(T * data);

	STDMETHOD(OnDispose)()
	{
		if(m_pReleaseDataProc != NULL && m_pData != NULL)
		{
			m_pReleaseDataProc(m_pData);
			m_pData = NULL;
		}

		return S_OK;
	}

	CAtlObjectLifeT()
	{
		m_pData  = NULL;
	}

	~CAtlObjectLifeT()
	{
		OnDispose();
	}
};

template< typename TBaseInterface, typename TStl, typename TElem, typename TCopy = ATL::_Copy<TElem>, BOOL DestroySTL = TRUE>
struct ATL_NO_VTABLE CAtlSTLEnumT
{
	typedef ATL::CComEnumOnSTL<TBaseInterface, &__uuidof(TBaseInterface), TElem , TCopy,  TStl > TEnumStl;

	static void DestorySTL(TStl * pData)
	{
		if(pData == NULL) return;

		for(TStl::iterator it = pData->begin(), end = pData->end();it != end; it++)
		{
			TCopy::destroy(&*it);
		}

		if(DestroySTL)
		{
			delete pData;
		}
	}

	template<typename T>
	static HRESULT CreateEnumT(TStl * stl,  T ** ppUnk )
	{	
		HRESULT hr;

		IUnknown * pLife = NULL;
		hr = CAtlObjectLifeT<TStl>::CreateInstance(&pLife);
		if(FAILED(hr)) return hr;

		CAtlObjectLifeT<TStl> * life = dynamic_cast<CAtlObjectLifeT<TStl > *>( pLife);

		life->m_pReleaseDataProc = DestorySTL;
		life->m_pData = stl;

		CComObject<TEnumStl> * pEnum = NULL;
		hr = CComObject<TEnumStl>::CreateInstance(&pEnum);

		if(FAILED(hr)) 
		{
			pLife->Release();
			return hr;
		}

		hr = pEnum->Init(pLife, *(life->m_pData));

		if(FAILED(hr))
		{
			pLife->Release();
			pEnum->Release();

			return hr;
		}

		return pEnum->QueryInterface(__uuidof(T), (void **)ppUnk);
	}

};

template<typename TElem, typename TCopy , BOOL DestroySTL, typename TSTL, typename TBaseInterface>
HRESULT CreateSTLEnumT(TSTL * stl, TBaseInterface ** ppUnk )
{
	return CAtlSTLEnumT<TBaseInterface, TSTL , TElem, TCopy, DestroySTL>
		::CreateEnumT(stl,ppUnk);
}



template<typename TClass, typename TInterface>
HRESULT CoCreateLocalInstanceT(TInterface ** ppvObject)
{
	INVALID_NULL(ppvObject);

	return CoCreateInstance(__uuidof(TClass),
		NULL, 
		CLSCTX_ALL,
		__uuidof(TInterface),
		(void **)ppvObject);
}

class CComHelper;

template<typename TClass, typename TInterface>
HRESULT CoCreateRemoteInstanceT(TInterface **ppvObject, LPWSTR hostName = NULL, COAUTHIDENTITY * pAuthEntity = NULL)
{		
	return CComHelper::CreateRemoteInstance(__uuidof(TClass), __uuidof(TInterface), (void **)ppvObject , hostName,pAuthEntity);
}

#ifndef MAKE_COAUTHIDENTITY
#define MAKE_COAUTHIDENTITY(user,password,domain) {\
	reinterpret_cast<USHORT *>(user),\
	STRLEN(user),\
	reinterpret_cast<USHORT *>(domain),\
	STRLEN(domain),\
	reinterpret_cast<USHORT *>(password),\
	STRLEN(password)\
}
#endif


template< typename TInterface, typename T>
BOOL CoIsTypeOfT(T * p)
{
	if(p==NULL) return FALSE;

	TInterface * temp = NULL;
	HRESULT hr = p->QueryInterface(__uuidof(TInterface), reinterpret_cast<void **>(&temp));
	if(temp != NULL) temp->Release();

	return SUCCEEDED(hr);
}


template<const IID & iid, typename T>
BOOL CoIsTypeOfT(T * p)
{
	if(p==NULL) return FALSE;

	IUnknown * temp = NULL;
	HRESULT hr = p->QueryInterface(iid, reinterpret_cast<void **>(&temp));
	if(temp != NULL) temp->Release();

	return SUCCEEDED(hr);
}

template<typename TInterface, typename T>
CComQIPtr<TInterface *> CoQueryCastT(T * p)
{
	if(p == NULL) return NULL;

	TInterface * temp = NULL;
	p->QueryInterface(__uuidof(TInterface), reinterpret_cast<void **>(&temp));

	CComQIPtr<TInterface *> ptr = temp;
	if(temp != NULL) temp->Release();
	return ptr;
}

#ifndef IS_TYPE_OF
#define IS_TYPE_OF(p,T) (CoIsTypeOfT<T>(p))
#endif

#ifndef QUERY_CAST
#define QUERY_CAST(p,T) (CoQueryCastT<T>(p))
#endif

template<int R>
int VarCmpResult2CompareResult(HRESULT r)
{
	return (LONG)r == VARCMP_LT ? -1 :
		(
		(LONG)r == VARCMP_EQ  ?  0 : 
		(
		(LONG)r == VARCMP_GT ? 1 : -2
		)

		);
}

#define VARCMPRESULT2COMPARERESULT VarCmpResult2CompareResult<0>

#ifndef VARIANT_COMPARE
#define VARIANT_COMPARE(v0,v1) ( VARCMPRESULT2COMPARERESULT(VarCmp(const_cast<VARIANT *>(&(v0)), const_cast<VARIANT *>(&(v1)), DEFAULT_LCID, 0) ) )
#endif

#ifndef VARIANT_EQUAL
#define VARIANT_EQUAL(v0,v1) ( VARIANT_COMPARE( v0, v1) == 0)
#endif

#ifdef SINGLETON_DEFAULT_INSTNANCE

template<typename T, typename TInterface>
struct CAtlDefaultInstanceT : CComQIPtr<TInterface>
{	
	BOOL IsNull() const
	{
		return p == NULL;
	}

	CAtlDefaultInstanceT()
	{
		CoCreateInstance(__uuidof(T));
	}

	static CAtlDefaultInstanceT Instance;
};

template<typename T, typename TInterface>
CAtlDefaultInstanceT<T,TInterface> CAtlDefaultInstanceT<T,TInterface>::Instance;

#define CO_GET_DEFAULT_INSTNANCE(tBase, tInterface) \
	(CAtlDefaultInstanceT<tBase, tInterface>::Instance)

#else

template<typename T,typename TInterface>
CComQIPtr<TInterface> CreateDefaultInstanceT()
{
	static CComQIPtr<TInterface> ptr;
	if(ptr.p == NULL)
	{
		HRESULT hr = ptr.CoCreateInstance(__uuidof(T));
		(void)hr;
	}
	return ptr;
}

#define CO_GET_DEFAULT_INSTNANCE(tBase, tInterface) \
	CreateDefaultInstanceT<tBase, tInterface>()

#endif

#ifndef ESSENCE_BUILDER
#define ESSENCE_BUILDER() CO_GET_DEFAULT_INSTNANCE(DefaultBuilder, IDefaultBuilder)
#endif

#ifndef PROFILE_PARSER
#define PROFILE_PARSER() CO_GET_DEFAULT_INSTNANCE(DataParser, IDataParser)
#endif

#ifndef JSON_PARSER
#define JSON_PARSER() CO_GET_DEFAULT_INSTNANCE(JsonParser, IJsonParser)
#endif

class CComHelper
{
public:
	CComHelper(void);
	~CComHelper(void);

public:

	static HRESULT CreateRemoteInstance(REFCLSID rclsid, 
		REFIID refIID,
		void **ppvObject, 
		LPWSTR hostName = NULL,
		COAUTHIDENTITY * pAuthEntity = NULL);
};


#endif