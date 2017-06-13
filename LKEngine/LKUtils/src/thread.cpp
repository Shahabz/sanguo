#include "utils.h"
#include "thread.h"
/*
#ifdef WIN32
#else
#endif
*/
Mthr *mthr_open()
{
	Mthr *mth;
	mth=(Mthr *)my_malloc(sizeof(Mthr));
	memset(mth,0,sizeof(Mthr));
	return mth;
}

int mthr_create(Mthr *mth,void *func_addr,void *arg)
{
#ifdef WIN32
	if((mth->hthread = CreateThread(mth->attr,mth->stacksize,(LPTHREAD_START_ROUTINE)func_addr,(LPVOID)arg,mth->cflags,(LPDWORD)&mth->thread_id))==NULL)
#else
	pthread_attr_init (&mth->attr);
	if(pthread_create(&mth->thread_id, &mth->attr,(void*(*)(void*))func_addr, (void *)arg)!=0)
//	if(pthread_create(&mth->thread_id, &mth->attr,(void *)func_addr, (void *)arg)!=0)
#endif
		return -1;
	return 0;
}

int mthr_attr_setdetachstate(Mthr *mth, char stat )
{
#ifdef WIN32
	return 0;
#else
	if( stat )
		return pthread_attr_setdetachstate(&mth->attr, PTHREAD_CREATE_DETACHED );
	else
		return pthread_attr_setdetachstate(&mth->attr, PTHREAD_CREATE_JOINABLE );
#endif
}

int mthr_detach( Mthr *mth )
{
#ifdef WIN32
	return 0;
#else
	return pthread_detach( mth->thread_id );
#endif
}

int mthr_join(Mthr *mth)
{
#ifdef WIN32
	if(WaitForSingleObject(mth->hthread, INFINITE)==WAIT_FAILED)
		return -1;
#else
	if(pthread_join(mth->thread_id, NULL)!=0)
		return -1;
#endif
	return 0;
}

int mthr_set_priority(Mthr *mth,int prio)
{
#ifdef WIN32
	int priotmp;
	switch(prio)
	{
	case 1:
		priotmp = THREAD_PRIORITY_TIME_CRITICAL;
		break;
	case -1:
		priotmp = THREAD_PRIORITY_LOWEST;
		break;
	default:
		priotmp = THREAD_PRIORITY_NORMAL;
	}
	SetThreadPriority( mth->hthread, priotmp );
#endif
	return 0;
}

int mthr_close(Mthr *mth)
{
	if(mth==NULL)
		return -1;
#ifdef WIN32
	if( mth->hthread )
	{
		CloseHandle( mth->hthread );
		mth->hthread = NULL;
	}
	if( mth->cond )
	{
		CloseHandle( mth->cond );
		mth->cond = NULL;
	}
#endif
	my_free(mth);
	return 0;
}

ThrMux *mmux_open()
{
	ThrMux *mmux;
	mmux=(ThrMux *)my_malloc(sizeof(ThrMux));
	memset(mmux,0,sizeof(ThrMux));
#ifdef WIN32
	mmux->mux=CreateMutex(NULL,FALSE,NULL);		//如果为TRUE线程才有互斥体的所有权
	if(mmux->mux==NULL)
	{
		my_free(mmux);
		return NULL;
	}
#else
	memset(&mmux->mux,0,sizeof(pthread_mutex_t));
//	mmux->mux.__m_kind=PTHREAD_MUTEX_FAST_NP;
//	mmux->mux.__align = PTHREAD_MUTEX_INITIALIZER;

	if( pthread_mutex_init(&mmux->mux, NULL)<0)
	{
		my_free(mmux);
		return NULL;
	}
#endif
	return mmux;
}

int mmux_close(ThrMux *mmux)
{
#ifdef WIN32
	CloseHandle(mmux->mux);
#else
	if(pthread_mutex_destroy(&mmux->mux)<0)
		return -1;
#endif
	if(mmux==NULL)
		return -1;
	my_free(mmux);
	return 0;
}

int mmux_lock(ThrMux *mmux)
{
	if( mmux == NULL )
		return -1;
#ifdef WIN32
	if(WaitForSingleObject(mmux->mux,INFINITE)==WAIT_FAILED)	//INFINITE无限等待
		return -1;
#else
	if( pthread_mutex_lock(&mmux->mux) <0 )
		return -1;
//	int pthread_mutex_trylock(&mmux->mux);
#endif
	return 0;
}

int mmux_unlock(ThrMux *mmux)
{
	if( mmux == NULL )
		return -1;
#ifdef WIN32
	if(!ReleaseMutex(mmux->mux))
		return -1;
#else
	if( pthread_mutex_unlock(&mmux->mux)<0)
		return -1;
#endif
	return 0;
}

int mcond_init(Mthr *mth)
{
#ifdef WIN32
	mth->cond = CreateEvent(NULL, FALSE, FALSE, NULL);
	return 0;
#else
	memset(&mth->cond,0,sizeof(pthread_cond_t));

	if( pthread_cond_init(&mth->cond, NULL)<0)
	{
		return -1;
	}
	return 0;
#endif
}

int mcond_destroy(Mthr *mth)
{
#ifdef WIN32
	if( mth->cond )
	{
		CloseHandle( mth->cond );
		mth->cond = NULL;
	}
	return 0;
#else
	if(pthread_cond_destroy(&mth->cond)<0)
		return -1;
#endif
	return 0;
}

// 线程挂起
int mcond_wait(Mthr *mth,ThrMux *mmux)
{
#ifdef WIN32
	if( mth->cond == NULL )
		return -1;
	if(!ReleaseMutex(mmux->mux))
		return -1;
//	SuspendThread(mth->hthread);
//	WaitForSingleObject( mth->cond, INFINITE );
	WaitForSingleObject( mth->cond, 1000 );
	ResetEvent( mth->cond );

	if(WaitForSingleObject(mmux->mux,200)==WAIT_FAILED)	//INFINITE无限等待
		return -1;
	return 0;
#else
//	if(pthread_cond_wait(&mth->cond,&mmux->mux)<0)
//		return -1;
	struct timeval now;
	struct timespec timeout;
	gettimeofday(&now,0);
	timeout.tv_sec = now.tv_sec + 2;
	timeout.tv_nsec = now.tv_usec * 1000;
	if( pthread_cond_timedwait(&mth->cond, &mmux->mux, &timeout) == ETIMEDOUT )
		return -1;
#endif
	return 0;
}

// 唤醒线程
int mcond_broadcast(Mthr *mth)
{
#ifdef WIN32
	SetEvent(mth->cond);
//	ResumeThread(mth->hthread);
#else
	if(pthread_cond_broadcast(&mth->cond)<0)
		return -1;
#endif
	return 0;
}
