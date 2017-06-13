#ifndef __TIMEGMCMD_H
#define __TIMEGMCMD_H

#include "timecalc.h"
#define COMMAND_STRVALUE_MAX	1024
typedef struct _command_entry
{
	int m_id;
	int m_cond;
	int m_cmd;
	int m_value[4];
	char m_strvalue[COMMAND_STRVALUE_MAX];		// 到哪个时间
	char m_timestr[128];	// 时间的字符串表示方法
	TimeEntry m_time;
} CommandEntry;

int time_gmcmd_init();
int time_gmcmd_logic();
void time_gmcmd_reset();

// 距离下次启动最近的时间
int time_gmcmd_getnexttime( int cmd, int value );
#endif
