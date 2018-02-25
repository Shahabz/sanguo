extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "auto_data_teacher_award.h"
extern MYSQL *myData;
extern lua_State* servL;
TeacherAward *g_teacher_award = NULL;
int g_teacher_award_maxnum = 0;

int teacheraward_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from teacher_award;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if ( row[0] )
			g_teacher_award_maxnum = atoi( row[0] ) + 1;
		else
			g_teacher_award_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_teacher_award = (TeacherAward *)malloc( sizeof(TeacherAward)*g_teacher_award_maxnum );
	memset( g_teacher_award, 0, sizeof(TeacherAward)*g_teacher_award_maxnum );

	sprintf( szSQL, "select `id`,`student_level`,`maxnum`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardkind4`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3`,`awardnum4` from teacher_award;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		int id = atoi( row[0] );
		if ( id < 0 || id >= g_teacher_award_maxnum  )
			continue;
		g_teacher_award[id].id = atoi(row[offset++]);
		g_teacher_award[id].student_level = atoi(row[offset++]);
		g_teacher_award[id].maxnum = atoi(row[offset++]);
		g_teacher_award[id].awardkind[0] = atoi(row[offset++]);
		g_teacher_award[id].awardkind[1] = atoi(row[offset++]);
		g_teacher_award[id].awardkind[2] = atoi(row[offset++]);
		g_teacher_award[id].awardkind[3] = atoi(row[offset++]);
		g_teacher_award[id].awardkind[4] = atoi(row[offset++]);
		g_teacher_award[id].awardnum[0] = atoi(row[offset++]);
		g_teacher_award[id].awardnum[1] = atoi(row[offset++]);
		g_teacher_award[id].awardnum[2] = atoi(row[offset++]);
		g_teacher_award[id].awardnum[3] = atoi(row[offset++]);
		g_teacher_award[id].awardnum[4] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int teacheraward_reload_auto()
{
	if ( g_teacher_award )
	{
		free( g_teacher_award );
		g_teacher_award = NULL;
	}
	g_teacher_award_maxnum = 0;
	teacheraward_init_auto();
	return 0;
}

