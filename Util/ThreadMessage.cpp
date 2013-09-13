#include "ThreadMessage.h"

CThreadMessage::CThreadMessage(void)
{
	m_nThreadID = 0;
}

CThreadMessage::~CThreadMessage(void)
{
}

BOOL CThreadMessage::Initialize()
{
	if(m_nThreadID != 0) return FALSE;

	m_nThreadID = GetCurrentThreadId();

	return m_nThreadID != 0;
}

BOOL CThreadMessage::Post(UINT msg, void * pVoid)
{
	if(m_nThreadID == 0) return FALSE;

	return PostThreadMessage(m_nThreadID, msg, 0, reinterpret_cast<LPARAM>(pVoid));
}

BOOL CThreadMessage::LoopMessages()
{
	if(m_nThreadID == 0) return FALSE;

	if( GetCurrentThreadId() != m_nThreadID) return FALSE;

	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);

		if(msg.message == WM_QUIT_THREAD_LOOP) break;

		BOOL hasHanded = FALSE;
		//test
		UINT temp = msg.message;
// 		if ( temp == 0 )
// 		{
// 			continue;
// 		}

// 		if(m_msg2Handles.Contains(msg.message))
// 		{
// 			MessageHandles handles = m_msg2Handles[msg.message];
// 
// 			for(int i=0, size = (int)handles.size(); i<size; ++i)
// 			{
// 				handles[i]->OnMessage(reinterpret_cast<void *>(msg.lParam));
// 
// 				hasHanded = TRUE;
// 			}				
// 		}

 		LOCK(&m_msg2Handles)
 		{
 			if(m_msg2Handles.Contains(msg.message))
 			{
 				MessageHandles handles = m_msg2Handles[msg.message];
 
 				for(int i=0, size = (int)handles.size(); i<size; ++i)
 				{
 					handles[i]->OnMessage(reinterpret_cast<void *>(msg.lParam));
 
 					hasHanded = TRUE;
 				}				
 			}
 		}

		if(!hasHanded)
		{
			DispatchMessage(&msg);
		}
	}	

	return TRUE;
}

BOOL CThreadMessage::QuitLoop()
{
	return Post(WM_QUIT_THREAD_LOOP, NULL);
}


int CThreadMessage::InnerIndexOf(UINT msg, IMessageHandle * handle)
{
	if(m_msg2Handles.Contains(msg)) return FALSE;

	MessageHandles & handles = m_msg2Handles[msg];

	for(int i = 0, size = (int)handles.size(); i<size; ++i)
	{
		if(handles[i] != NULL && handles[i]->Equals(handle)) return i;
	}

	return -1;	
}

int CThreadMessage::IndexOf(UINT msg, IMessageHandle * handle)
{
	LOCK(&m_msg2Handles)
	{
		return InnerIndexOf(msg, handle);
	}

	return -1;
}

BOOL CThreadMessage::InnerRegHandle(UINT msg, IMessageHandle * handle)
{
	if(handle == NULL) return FALSE;

	int index = InnerIndexOf(msg, handle);

	if(index == -1) 
	{
		m_msg2Handles[msg].push_back( handle);
	}
	else
	{
		delete handle;
	}

	return TRUE;
}


BOOL CThreadMessage::InnerUnregHandle(UINT msg, IMessageHandle * handle)
{
	if(handle == NULL) return FALSE;

	int index = InnerIndexOf(msg, handle);

	if(index != -1) 
	{
		IMessageHandle * temp = m_msg2Handles[msg][index];
		if(temp != NULL) delete temp;

		STL_VECTOR_EARSE(m_msg2Handles[msg], index);
	}

	delete handle;

	return TRUE;
}

BOOL CThreadMessage::ContainsHandle(UINT msg, OnMessageProc proc)
{
	LOCK(&m_msg2Handles)
	{
		GlobalMessageHandle * handle = new GlobalMessageHandle(proc);

		int index = InnerIndexOf(msg, handle);

		if(handle != NULL) delete handle;

		return index != -1;
	}

	return FALSE;
}