#ifndef __THREAD_H
#define __THREAD_H

#ifdef WIN32
#include <winsock2.h>
#include <windows.h> 
#else
#include <pthread.h>
#include <sys/time.h>
#endif

#define THREAD_HIGH		1
#define THREAD_NORMAL	0
#define THREAD_LOW		-1

struct _iomthr {
#ifdef WIN32
				LPSECURITY_ATTRIBUTES attr;
				DWORD stacksize;
				DWORD cflags;
				HANDLE hthread;
				int thread_id;
				HANDLE cond;
				int wakeup_count;
#else
				pthread_attr_t attr;
				pthread_t thread_id;
				pthread_cond_t	cond;
#endif
        };
typedef struct _iomthr Mthr;

struct _mmutex {
#ifdef WIN32
				HANDLE mux;
#else
				pthread_mutex_t	mux;
#endif
        };
typedef struct _mmutex ThrMux;

Mthr *mthr_open();
int mthr_create(Mthr *mth, void *func_addr, void *arg);
int mthr_join(Mthr *mth);
int mthr_attr_setdetachstate(Mthr *mth, char stat );
int mthr_detach( Mthr *mth );
int mthr_set_priority(Mthr *mth,int prio);
int mthr_close(Mthr *mth);

ThrMux *mmux_open();
int mmux_close(ThrMux *mmux);
int mmux_lock(ThrMux *mmux);
int mmux_unlock(ThrMux *mmux);

int mcond_init(Mthr *mth);
int mcond_destroy(Mthr *mth);
int mcond_wait(Mthr *mth,ThrMux *mmux);
int mcond_broadcast(Mthr *mth);

#endif
