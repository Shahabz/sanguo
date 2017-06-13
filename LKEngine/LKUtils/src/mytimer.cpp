#include "thread.h"
#include "mytimer.h"
#ifndef WIN32
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#endif

typedef struct _stimer_info {
	long m_counter;		// 流逝了多少个tick
	unsigned long m_speed;			// 每次间隔多少个tick
	void *m_parameter;
	void (* m_processfunc) (void *) ;
} STimerInfo;

static STimerInfo s_timer[MAX_TIMER];
static Mthr *s_timer_pthr;

#ifdef WIN32
static HANDLE s_timer_stop_event = NULL;
static LARGE_INTEGER s_counter_freq;
static BOOL s_is_high_timer;
#else
static int s_timer_stop_event;

static void block_all_signals(void)
{
   sigset_t mask;
   sigfillset(&mask);
   pthread_sigmask(SIG_BLOCK, &mask, NULL);
}
#endif

#define CLOCK_PER_SECOND 1193181L
// 时钟周期的tick和usec时间系统的转换(select)
#define CLOCK_TO_USEC(x)  ((long)((x) / 1.193181))
#define USEC_TO_CLOCK(x)  ((long)((x) * (CLOCK_PER_SECOND / 1000000.)))
// 时钟周期的tick和msec时间系统的转换(timeGetTime,WaitForSingleObject)
#define CLOCK_TO_MSEC(x) ((unsigned long)(x) / (CLOCK_PER_SECOND / 1000))
#define MSEC_TO_CLOCK(x)      ((long)(x) * (CLOCK_PER_SECOND / 1000))
// 时钟周期的tick和conuter时间系统的转换(QueryPerformanceCounter)
#define COUNTER_TO_CLOCK(x) ((unsigned long)(x * CLOCK_PER_SECOND / s_counter_freq.QuadPart))

void timer_tick_thread();

#ifdef _DEBUG
int GlobalGetSize_mytimer()
{
	int size = sizeof(s_timer)+sizeof(s_counter_freq)+sizeof(s_is_high_timer)+sizeof(s_timer_pthr)+sizeof(s_timer_stop_event);
	return size;
}
#endif


void init_timer()//初始化计时器
{
#ifdef WIN32
	s_is_high_timer = TRUE;
	// 获得高性能记数器的频率,如果失败可能是硬件不支持. tick/s
	if( !QueryPerformanceFrequency(&s_counter_freq) )
	{
		s_is_high_timer = FALSE;
	}
	s_timer_stop_event = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
	s_timer_stop_event = 0;
#endif
	s_timer_pthr = mthr_open();
	mthr_create( s_timer_pthr, (void*)timer_tick_thread, NULL );
	// 记时器线程高优先权
	mthr_set_priority( s_timer_pthr, THREAD_HIGH );
}

void timer_tick_thread()
{
	int tmpi;
	unsigned long delay_clock;	// 延迟的tick数,每秒钟会有1193181L个tick数
	unsigned long used_clock;

#ifdef WIN32
	DWORD prev_time;	// 毫秒
	DWORD curr_time;
	DWORD diff_time;

	LARGE_INTEGER curr_counter;
	LARGE_INTEGER prev_counter;
	LARGE_INTEGER diff_counter;

	DWORD result;
	DWORD dwDelayMilliseceonds;

	if( s_is_high_timer )
		QueryPerformanceCounter(&prev_counter);
	else
		prev_time = timeGetTime(); //1,193,181L

#else
	struct timeval prev_time;
	struct timeval curr_time;
	struct timeval diff_time;
	struct timeval delay;

	block_all_signals();
	gettimeofday(&prev_time, 0);
#endif

	delay_clock = used_clock = 0x8000;

	while( 1 )
	{
#ifdef WIN32

		dwDelayMilliseceonds = CLOCK_TO_MSEC(delay_clock);
		if ( dwDelayMilliseceonds < 15 )
			dwDelayMilliseceonds = 15;
		result = WaitForSingleObject( s_timer_stop_event, dwDelayMilliseceonds );
		if (result != WAIT_TIMEOUT)
			return;

		if( s_is_high_timer )
		{
			QueryPerformanceCounter(&curr_counter);
			diff_counter.QuadPart = curr_counter.QuadPart - prev_counter.QuadPart;
			prev_counter.QuadPart = curr_counter.QuadPart;

			used_clock = COUNTER_TO_CLOCK( diff_counter.QuadPart );
		}
		else
		{
			curr_time = timeGetTime(); //1,193,181L
			diff_time = curr_time - prev_time;
			prev_time = curr_time;

			used_clock = MSEC_TO_CLOCK( diff_time );
		}

#else
		if( s_timer_stop_event )
			return;
		delay.tv_sec = delay_clock / CLOCK_PER_SECOND;
		delay.tv_usec = CLOCK_TO_USEC(delay_clock) % 1000000L;
		select(0, NULL, NULL, NULL, &delay);

		gettimeofday(&curr_time, 0);
		diff_time.tv_sec = curr_time.tv_sec - prev_time.tv_sec;
		diff_time.tv_usec = curr_time.tv_usec - prev_time.tv_usec;
		prev_time = curr_time;

		used_clock = USEC_TO_CLOCK(diff_time.tv_sec * 1000000L + diff_time.tv_usec);
#endif
		delay_clock = 0x8000;
		for( tmpi = 0; tmpi<MAX_TIMER; tmpi++ )
		{
			if ( (s_timer[tmpi].m_processfunc) && (s_timer[tmpi].m_speed > 0) )
			{
				s_timer[tmpi].m_counter -= used_clock;

				while( s_timer[tmpi].m_counter <= 0 )
				{
					s_timer[tmpi].m_counter += s_timer[tmpi].m_speed;
					s_timer[tmpi].m_processfunc( s_timer[tmpi].m_parameter );
				}

				if ( (unsigned long)s_timer[tmpi].m_counter < delay_clock )
				{
					delay_clock = s_timer[tmpi].m_counter;
				}
			}
		}
	}

}

void append_timer( void *proc, void *para, int speed )
{
	int tmpi;
	for( tmpi = 0; tmpi < MAX_TIMER; tmpi++ )
	{
		if ( s_timer[tmpi].m_processfunc == proc )
		{
			s_timer[tmpi].m_counter = s_timer[tmpi].m_counter - s_timer[tmpi].m_speed + speed;
			s_timer[tmpi].m_parameter = para;
			s_timer[tmpi].m_speed = speed;
			return;
		}
	}
	for( tmpi = 0; tmpi < MAX_TIMER; tmpi++ )
	{
		if ( !s_timer[tmpi].m_processfunc )
		{
			s_timer[tmpi].m_counter = speed;
			s_timer[tmpi].m_parameter = para;
			s_timer[tmpi].m_speed = speed;
			s_timer[tmpi].m_processfunc = (void (*)(void *))proc;
			return;
		}
	}
}
void exit_timer()
{
#ifdef WIN32
	SetEvent( s_timer_stop_event );
#else
	s_timer_stop_event = 1;
#endif
	mthr_join( s_timer_pthr );
	mthr_close( s_timer_pthr );
#ifdef WIN32
	CloseHandle( s_timer_stop_event );
#endif
}

int delete_timer( void *proc )
{
	int tmpi;
	for( tmpi = 0; tmpi < MAX_TIMER; tmpi++ )
	{
		if ( s_timer[tmpi].m_processfunc == proc )
		{
			s_timer[tmpi].m_processfunc = 0;
			return 0;
		}
	}
	return -1;
}

