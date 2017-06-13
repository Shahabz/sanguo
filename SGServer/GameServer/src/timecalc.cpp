#include <string.h>
#include <math.h>
#include "utils.h"
#include "timecalc.h"

/*
声明
TimeEntry yourTimeEntry;
初始化：解析时间串
分 时 日 月 星期
time_format( &yourTimeEntry, "1 19-7/2,8 * * *" );
logic_minite() 最好按分钟 若低于一分钟执行也不会出问题
{
...
	if( time_checktime( &yourTimeEntry ) == 0 )
		// process action
...
}
*/
int time_format( TimeEntry *pTimeEntry, char *pString )
{
	int tmpi;
	int tmpj;
	char **pproot;
	char **ppitem;
	char **ppspace;
	char **ppfromto;
	char tmpstr[256];
	Entry *pCurEntry;

	if( pTimeEntry == NULL || pString == NULL )
		return -1;

	pTimeEntry->min.point_min = 0;
	pTimeEntry->min.point_max = 59;
	pTimeEntry->hour.point_min = 0;
	pTimeEntry->hour.point_max = 23;
	pTimeEntry->day.point_min = 1;
	pTimeEntry->day.point_max = 31;
	pTimeEntry->mon.point_min = 1;
	pTimeEntry->mon.point_max = 12;
	pTimeEntry->week.point_min = 0;
	pTimeEntry->week.point_max = 6;
	pTimeEntry->mtime = 0;

	pproot = u_strncut( pString, ' ', 4 );
	if( pproot == NULL )
		return -1;

	for( tmpi = 0; tmpi < 5; tmpi++ )
	{
		if( pproot[tmpi] == NULL )
		{
			u_free_vec( pproot );
			return -1;
		}
		switch( tmpi )
		{
		case 0:
			pCurEntry = &pTimeEntry->min;
			break;
		case 1:
			pCurEntry = &pTimeEntry->hour;
			break;
		case 2:
			pCurEntry = &pTimeEntry->day;
			break;
		case 3:
			pCurEntry = &pTimeEntry->mon;
			break;
		case 4:
			pCurEntry = &pTimeEntry->week;
			break;
		}
		pCurEntry->point_count = 0;
		pCurEntry->any = 0;

		ppitem = u_strncut( pproot[tmpi], ',', 32 );
		if( ppitem == NULL )
			return -1;
		for( tmpj = 0; tmpj < 32; tmpj++ )
		{
			if( ppitem[tmpj] == NULL )
				break;
			ppspace = u_strncut( ppitem[tmpj], '/', 1 );
			if( ppspace == NULL )
				break;
			if( ppspace[0] == NULL )
			{
				u_free_vec( ppspace );
				break;
			}
			if( ppspace[1] )
				pCurEntry->point_space[tmpj] = atoi(ppspace[1]);
			else
				pCurEntry->point_space[tmpj] = 1;

			strcpy( tmpstr, ppspace[0] );
			u_free_vec( ppspace );

			ppfromto = u_strncut( tmpstr, '-', 1 );
			if( ppfromto == NULL )
				break;
			if( ppfromto[0] == NULL )
			{
				u_free_vec( ppfromto );
				break;
			}
			if( strcmp( ppfromto[0], "*" ) == 0 )
			{
				if( pCurEntry->point_space[tmpj] == 1 )
					pCurEntry->any = 1;
				pCurEntry->point_from[tmpj] = pCurEntry->point_min;
				pCurEntry->point_to[tmpj] = pCurEntry->point_max;
			}
			else
			{
				pCurEntry->point_from[tmpj] = atoi( ppfromto[0] );
				if( pCurEntry->point_from[tmpj] > pCurEntry->point_max )
					pCurEntry->point_from[tmpj] = pCurEntry->point_max;
				else if( pCurEntry->point_from[tmpj] < pCurEntry->point_min )
					pCurEntry->point_from[tmpj] = pCurEntry->point_min;
				if( ppfromto[1] == NULL )
					pCurEntry->point_to[tmpj] = pCurEntry->point_from[tmpj];
				else
					pCurEntry->point_to[tmpj] = atoi( ppfromto[1] );
			}
			u_free_vec( ppfromto );

			pCurEntry->point_count++;
		}
		u_free_vec( ppitem );
	}
	u_free_vec( pproot );
	return 0;
}

char time_getentry_value( char value, Entry *pEntry )
{
	char tmpvalue;
	char isin;
	char checkvalue;
	char pointmin,pointmax;
	int tmpi;

	for( tmpi = 0; tmpi < pEntry->point_count; tmpi++ )
	{
		checkvalue = value;
		if( checkvalue == pEntry->point_from[tmpi] || checkvalue == pEntry->point_to[tmpi] )	// 直接相等[安全]
			return checkvalue;

		isin = 0;
		pointmin = pEntry->point_from[tmpi];
		pointmax = pEntry->point_to[tmpi];
		if( pointmin > pointmax )
		{
			if( checkvalue > pointmin )
			{
				pointmax += pEntry->point_max+1;
				isin = 2;
			}
			else if( checkvalue < pointmax )
			{
				pointmax += pEntry->point_max+1;
				checkvalue = pointmax+checkvalue;
				isin = 3;
			}
		}
		else
		{
			if( checkvalue > pointmin && checkvalue < pointmax )
				isin = 1;
		}
		if( isin > 0 )
		{
			// 在区间内，排除从区间内按间隔跳出了区间的情况
			tmpvalue = pointmin;
			for( tmpvalue = pointmin; tmpvalue < pointmax; tmpvalue += pEntry->point_space[tmpi] )
			{
				// 相等是命中，大于是(范围接近)[不安全]
				if( tmpvalue >= checkvalue )
				{
					if( tmpvalue > checkvalue )
						pEntry->unsafe = 1;
					return tmpvalue;
				}
			}
		}
	}

	tmpvalue = pEntry->point_max+1;
	for( tmpi = 0; tmpi < pEntry->point_count; tmpi++ )
	{
		// 找到第一个大于当前的时间(范围最接近)[不安全]
		if( pEntry->point_from[tmpi] > value && pEntry->point_from[tmpi] < tmpvalue )
		{
			tmpvalue = pEntry->point_from[tmpi];
			pEntry->unsafe = 1;
		}
	}
	if( tmpvalue == pEntry->point_max+1 )
	{
		for( tmpi = 0; tmpi < pEntry->point_count; tmpi++ )
		{
			// 找到第一个小于当前的时间(范围接近小值)[安全]
			if( pEntry->point_from[tmpi] < value && pEntry->point_from[tmpi] < tmpvalue )
				tmpvalue = pEntry->point_from[tmpi];
		}
		return tmpvalue+pEntry->point_max+1;
	}
	else
		return tmpvalue;
}

char time_getentry_minvalue( Entry *pEntry )
{
	char checkvalue;
	int tmpi;

	checkvalue = 127;
	for( tmpi = 0; tmpi < pEntry->point_count; tmpi++ )
	{
		if( pEntry->point_from[tmpi] < checkvalue )
			checkvalue = pEntry->point_from[tmpi];
	}
	if( checkvalue == 127 )
		checkvalue = 0;
	return checkvalue;
}

int time_getmdaymax( char mon, int year )
{
	if( mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12 )
		return 31;
	else if( mon == 4 || mon == 6 || mon == 9 || mon == 11 )
		return 30;

	if( year%4 == 0 )
	{
		if( year%100 == 0 && year%400 != 0 )
			return 28;
		return 29;
	}
	return 28;
}

int time_reset( TimeEntry *pTimeEntry )
{
	time_t timestamp;
	struct tm *newtime;

	char min;
	char hour;
	char mon;
	char day;
	char week;

	char min_min;
	char min_hour;
	char min_day;
	char min_week;

	int tmpmon;

	if( pTimeEntry == NULL )
		return -1;

	time(&timestamp);
	timestamp += 60;
	newtime = localtime( &timestamp );

	// 1.分钟
	pTimeEntry->min.unsafe = 0;
	min = time_getentry_value( newtime->tm_min, &pTimeEntry->min );
	if( min > pTimeEntry->min.point_max )
	{
		pTimeEntry->min.unsafe = 1;
		min -= pTimeEntry->min.point_max+1;
		timestamp += 60*60;	// 加一个小时
	}
	timestamp += 60*( min - newtime->tm_min );
	newtime = localtime( &timestamp );

	// 2.小时
	pTimeEntry->hour.unsafe = 0;
	hour = time_getentry_value( newtime->tm_hour, &pTimeEntry->hour );
	if( hour > pTimeEntry->hour.point_max )
	{
		pTimeEntry->hour.unsafe = 1;
		hour -= pTimeEntry->hour.point_max+1;
		timestamp += 60*60*24;	// 加一天
	}
	timestamp += 60*60*( hour - newtime->tm_hour );
	newtime = localtime( &timestamp );

	if( pTimeEntry->week.any )
	{
		// 3.天(月)
		pTimeEntry->day.unsafe = 0;
		pTimeEntry->day.point_max = time_getmdaymax( newtime->tm_mon+1, newtime->tm_year+1900 );
		day = time_getentry_value( newtime->tm_mday, &pTimeEntry->day );
		if( day > pTimeEntry->day.point_max )
		{
			pTimeEntry->day.unsafe = 1;
			day -= pTimeEntry->day.point_max+1;
			timestamp += 60*60*24*pTimeEntry->day.point_max;	// 加一月
		}
		timestamp += 60*60*24*( day - newtime->tm_mday );
		newtime = localtime( &timestamp );
	}
	else
	{
		// 3.天(星期)
		pTimeEntry->week.unsafe = 0;
		week = time_getentry_value( newtime->tm_wday, &pTimeEntry->week );
		if( week > pTimeEntry->week.point_max )
		{
			pTimeEntry->week.unsafe = 1;
			week -= pTimeEntry->week.point_max+1;
			timestamp += 60*60*24*7;	// 加七天
		}
		timestamp += 60*60*24*( week - newtime->tm_wday );
		newtime = localtime( &timestamp );
	}

	// 4.月
	pTimeEntry->mon.unsafe = 0;
	mon = time_getentry_value( newtime->tm_mon+1, &pTimeEntry->mon );
	if( mon > pTimeEntry->mon.point_max )
	{
		pTimeEntry->mon.unsafe = 1;
		mon -= pTimeEntry->mon.point_max+1;
	}
	tmpmon = newtime->tm_mon+1;
	while( tmpmon != mon )
	{
		timestamp += 60*60*24*time_getmdaymax( tmpmon, newtime->tm_year+1900 );
		tmpmon++;
		if( tmpmon > 12 )
			tmpmon = 1;
	}

	if( pTimeEntry->mon.unsafe > 0 )
	{
		min_min = time_getentry_minvalue( &pTimeEntry->min );
		if( min > min_min )
		{
			timestamp -= 60*(min-min_min);
			min = min_min;
		}
		min_hour = time_getentry_minvalue( &pTimeEntry->hour );
		if( hour > min_hour )
		{
			timestamp -= 60*60*(hour-min_hour);
			hour = min_hour;
		}
		if( pTimeEntry->week.any )
		{
			min_day = time_getentry_minvalue( &pTimeEntry->day );
			if( day > min_day )
			{
				timestamp -= 60*60*24*(day-min_day);
				day = min_day;
			}
		}
		else
		{
			min_week = time_getentry_minvalue( &pTimeEntry->week );
			if( week > min_week )
			{
				timestamp -= 60*60*24*(week-min_week);
				week = min_week;
			}
		}
	}
	else if(( pTimeEntry->week.any && pTimeEntry->day.unsafe > 0 ) || ( pTimeEntry->week.any == 0 && pTimeEntry->week.unsafe > 0 ))
	{
		min_min = time_getentry_minvalue( &pTimeEntry->min );
		if( min > min_min )
		{
			timestamp -= 60*(min-min_min);
			min = min_min;
		}
		min_hour = time_getentry_minvalue( &pTimeEntry->hour );
		if( hour > min_hour )
		{
			timestamp -= 60*60*(hour-min_hour);
			hour = min_hour;
		}
	}
	else if( pTimeEntry->hour.unsafe > 0 )
	{
		min_min = time_getentry_minvalue( &pTimeEntry->min );
		if( min > min_min )
		{
			timestamp -= 60*(min-min_min);
			min = min_min;
		}
	}

	pTimeEntry->mtime = timestamp;
	return 0;
}

int time_reset2( TimeEntry *pTimeEntry )
{
	time_t t;
	struct tm *newtime;

	char min;
	char hour;
	char mon;
	char day;
	char week;

	int tmpmon;

	if( pTimeEntry == NULL )
		return -1;

	time(&t);
	t += 60;
	newtime = localtime( &t );

	// 月
	mon = time_getentry_value( newtime->tm_mon+1, &pTimeEntry->mon );
	if( mon > pTimeEntry->mon.point_max )
	{
		mon -= pTimeEntry->mon.point_max+1;
	}
	tmpmon = newtime->tm_mon+1;
	while( tmpmon != mon )
	{
		t += 60*60*24*time_getmdaymax( tmpmon, newtime->tm_year+1900 );
		tmpmon++;
		if( tmpmon > 12 )
			tmpmon = 1;
	}

	// 日
	if( pTimeEntry->week.any )
	{
		pTimeEntry->day.point_max = time_getmdaymax( newtime->tm_mon+1, newtime->tm_year+1900 );
		day = time_getentry_value( newtime->tm_mday, &pTimeEntry->day );
		if( day > pTimeEntry->day.point_max )
		{
			day -= pTimeEntry->day.point_max+1;
			t += 60*60*24*pTimeEntry->day.point_max;	// 加一月
		}
		t += 60*60*24*( day - newtime->tm_mday );
		newtime = localtime( &t );
	}
	else
	{
		week = time_getentry_value( newtime->tm_wday, &pTimeEntry->week );
		if( week > pTimeEntry->week.point_max )
		{
			week -= pTimeEntry->week.point_max+1;
			t += 60*60*24*7;	// 加七天
		}
		t += 60*60*24*( week - newtime->tm_wday );
		newtime = localtime( &t );
	}

	// 时
	hour = time_getentry_value( newtime->tm_hour, &pTimeEntry->hour );
	if( hour > pTimeEntry->hour.point_max )
	{
		hour -= pTimeEntry->hour.point_max+1;
		t += 60*60*24;	// 加一天
	}
	t += 60*60*( hour - newtime->tm_hour );
	newtime = localtime( &t );

	// 分
	min = time_getentry_value( newtime->tm_min, &pTimeEntry->min );
	if( min > pTimeEntry->min.point_max )
	{
		min -= pTimeEntry->min.point_max+1;
		t += 60*60;	// 加一个小时
	}
	t += 60*( min - newtime->tm_min );
	newtime = localtime( &t );

	pTimeEntry->mtime = t;
	return 0;
}

// 返回0 表示到达执行点
int time_checktime( TimeEntry *pTimeEntry )
{
	time_t t;
//#ifdef _DEBUG
	struct tm *newtime;
//#endif
	if( pTimeEntry == NULL )
		return -1;
	if( pTimeEntry->mtime == 0 )
	{
		time_reset( pTimeEntry );
//#ifdef _DEBUG
		newtime = localtime( &pTimeEntry->mtime );
		//printf_msg( "%.4d/%.2d/%.2d %.2d:%.2d:%.2d %d", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, newtime->tm_wday );
//#endif
		write_gamelog( "First:%.4d/%.2d/%.2d %.2d:%.2d:%.2d %d", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, newtime->tm_wday );
		// 设定完以后还是0，就返回退出
		if( pTimeEntry->mtime == 0 )
			return -1;
	}

	time(&t);
	if( t >= pTimeEntry->mtime )
	{
		time_reset( pTimeEntry );
//#ifdef _DEBUG
		newtime = localtime( &pTimeEntry->mtime );
		//printf_msg( "%.4d/%.2d/%.2d %.2d:%.2d:%.2d %d", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, newtime->tm_wday );
//#endif
		write_gamelog( "Next:%.4d/%.2d/%.2d %.2d:%.2d:%.2d %d", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, newtime->tm_wday );
		return 0;
	}
//	newtime = localtime( &pTimeEntry->mtime );
//	printf_msg( "%.4d/%.2d/%.2d %.2d:%.2d:%.2d %d\n", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, newtime->tm_wday );
	return -1;
}
