//--------------------------------------------------------------------
// created:       2013/05/07   10:13
// Filename:      Lock.h
// author:        Neo
// Copyright(c):  �Ϻ����⵼���������޹�˾. All rights reserved.
//--------------------------------------------------------------------
#ifndef _LOCK_H__
#define _LOCK_H__
#include "LockController.h"

//��ֹ���̶߳�ͬһ��Դͬʱ���з���  ͬ������

#if defined(_MSC_VER) && _MSC_VER<1200

#define LOCK_VARIANT(x, ...)\
	if(TRUE)\
	for(CLockController ________LockController((void *)(x), __VA_ARGS__) ;  ________LockController.TryNext();  )

#else

#define LOCK_VARIANT(x, ...)\
	for(LockController ________LockController((void *)(x), __VA_ARGS__) ;  ________LockController.TryNext();  )

#endif

#define LOCK LOCK_VARIANT

#define BREAK_LOCK break

#endif