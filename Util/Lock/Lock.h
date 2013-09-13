
#ifndef _LOCK_H__
#define _LOCK_H__
#include "LockController.h"

//防止多线程对同一资源同时进行访问  同步数据

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