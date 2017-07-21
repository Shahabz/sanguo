#ifndef __SYSTEM_H
#define __SYSTEM_H

int system_getruntime();
int system_getopentime();

int system_getfday();
int system_gettoday();
int system_getmonth();
int system_getweek();
int system_getmaxday( int year, int month );
int system_getcurmonth_maxday();

int system_timestring( int timestamp, char *out );
int system_countdownstring( int interval, char *out );

int random( int min, int max );
void ruffle( int *list, int count ); 
void random_str( char *out, int length );

// —”≥Ÿ÷¥––
#define DELAYEXEC_MAX	2048
#define DELAYEXEC_CMD_AIHELP	1
typedef struct _delay_exec
{
	char cmd;
	void *ptr;
	int value;
	int exec_time;
}DelayExec;
void delay_exec_add( char cmd, void *p, int value, int cd );
void delay_exec_fetch();
#endif 
