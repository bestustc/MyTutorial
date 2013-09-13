#pragma once

#include "UtilHelper.h"
#include "Lock/Lock.h"
#include "IndexMap.h"

typedef void (* OnMessageProc)(void * pVoid) ;

class CThreadMessage
{
public:

	struct IMessageHandle
	{
		virtual void OnMessage(void * pVoid) = 0;

		virtual BOOL Equals(const IMessageHandle * msg) const = 0;

		virtual BOOL IsTypeOf(const type_info & info) const = 0;
	};

	struct GlobalMessageHandle : IMessageHandle
	{
		OnMessageProc proc;

		GlobalMessageHandle(OnMessageProc proc)
		{
			this->proc = proc;
		}

		virtual void OnMessage(void * pVoid)
		{
			if(proc != NULL)
			{
				proc(pVoid);
			}
		}

		virtual BOOL Equals(const IMessageHandle * handle) const 
		{
			if(handle == this) return TRUE;

			if(!handle->IsTypeOf(typeid(GlobalMessageHandle))) return FALSE;

			const GlobalMessageHandle * h = static_cast<const GlobalMessageHandle * >(handle);

			return h->proc == proc;
		}

		virtual BOOL IsTypeOf(const type_info & info) const 
		{
			return info == typeid(IMessageHandle) &&
				info == typeid(GlobalMessageHandle);
		}
	};

	template<typename T>
	struct MemberMessageHandleT : IMessageHandle
	{
		typedef typename void (T::*Proc)(void * pVoid);	

		Proc proc;

		T * obj;

		MemberMessageHandleT(Proc proc, T * obj)
		{
			this->proc = proc;
			this->obj = obj;
		}

		virtual void OnMessage(void * pVoid)
		{
			if(proc != NULL && obj != NULL)
			{
				(obj->* proc)(pVoid);
			}
		}

		virtual BOOL Equals(const IMessageHandle * handle) const 
		{
			if(handle == this) return TRUE;

			if(!handle->IsTypeOf(typeid(*this))) return FALSE;

			const MemberMessageHandleT<T> * h = static_cast<const MemberMessageHandleT<T> * >(handle);

			return h->proc == proc &&
				h->obj == obj;
		}

		virtual BOOL IsTypeOf(const type_info & info) const 
		{
			return info == typeid(IMessageHandle) &&
				info == typeid(MemberMessageHandleT<T>);
		}
	};


	typedef vector<IMessageHandle *> MessageHandles;
	typedef CIndexMapT<UINT, MessageHandles> Msg2HandleMap;

private:

	Msg2HandleMap m_msg2Handles;
	DWORD m_nThreadID;

public:

	CThreadMessage(void);

	~CThreadMessage(void);

private:

	int InnerIndexOf(UINT msg, IMessageHandle * handle);

	BOOL InnerRegHandle(UINT msg, IMessageHandle * handle);	
	
	BOOL InnerUnregHandle(UINT msg, IMessageHandle * handle);

public:

	BOOL Initialize();

	BOOL Post(UINT msg, void * pVoid);

	BOOL LoopMessages();

	BOOL QuitLoop();

	int IndexOf(UINT msg, IMessageHandle * handle);

	BOOL ContainsHandle(UINT msg, OnMessageProc proc);

	BOOL RegHandle(UINT msg, OnMessageProc proc)
	{
		return InnerRegHandle(msg, new GlobalMessageHandle(proc));
	}

	BOOL UnregHandle(UINT msg, OnMessageProc proc)
	{
		return InnerUnregHandle(msg, new GlobalMessageHandle(proc));
	}

	template<typename T>
	BOOL ContainsHandle(UINT msg, void (T::*Proc)(void * pVoid), T * obj)
	{
		MemberMessageHandleT<T> * handle = new  MemberMessageHandleT<T>(proc, obj);
		BOOL b = IndexOf(handle);

		delete handle;
		return b;
	}

	template<typename T>
	BOOL RegHandle(UINT msg, void (T::*proc)(void * pVoid), T * obj)
	{
		return InnerRegHandle(msg, new  MemberMessageHandleT<T>(proc, obj));	
	};

	template<typename T>
	BOOL UnregHandle(UINT msg, void (T::*Proc)(void * pVoid), T * obj)
	{
		return InnerUnregHandle(msg, new  MemberMessageHandleT<T>(proc, obj));	
	};

	
};
