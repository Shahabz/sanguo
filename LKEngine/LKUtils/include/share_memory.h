#ifndef __SHARE_MEMORY_H
#define __SHARE_MEMORY_H

#ifndef WIN32
#include <sys/ipc.h> 
#include <sys/shm.h> 
#endif
#include "utils.h"

#ifdef	WIN32
#define INVALID_SM_HANDLE	 ((VOID*)0)
#else
#define INVALID_SM_HANDLE	 -1
#endif

/* 共享内存句柄 */
#ifdef WIN32
typedef		VOID*	SM_HANDLE;
#else
typedef		int		SM_HANDLE;
#endif

/* 键值 */
#ifdef WIN32
typedef	ULONG SM_KEY;
#else
typedef int SM_KEY;
#endif

/* 创建共享内存区 */
SM_HANDLE share_memory_create( SM_KEY key, unsigned int size );

/* 打开共享内存区 */
SM_HANDLE share_memory_open( SM_KEY key, unsigned int size );

/* 映射共享内存区*/
char* share_memory_mapping( SM_HANDLE smh );

/* 关闭映射共享内存区 */
void share_memory_unmapping( char* memptr );

/*	关闭共享内存区 */
void share_memory_close( SM_HANDLE smh );
#endif
