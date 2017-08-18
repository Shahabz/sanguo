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
#include "auto_data_story.h"
extern MYSQL *myData;
extern lua_State* servL;
StoryInfo *g_storyinfo = NULL;
int g_storyinfo_maxnum = 0;

int storyinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(chapter) from story;" );
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
			g_storyinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_storyinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_storyinfo = (StoryInfo *)malloc( sizeof(StoryInfo)*g_storyinfo_maxnum );
	memset( g_storyinfo, 0, sizeof(StoryInfo)*g_storyinfo_maxnum );

	sprintf( szSQL, "select chapter, max( rank ) from story group by chapter;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int chapter = atoi( row[0] );
		if ( chapter < 0 || chapter >= g_storyinfo_maxnum  )
			continue;
		g_storyinfo[chapter].maxnum = atoi( row[1] ) + 1;
		g_storyinfo[chapter].config = (StoryInfoConfig *)malloc( sizeof(StoryInfoConfig)*g_storyinfo[chapter].maxnum );
		memset( g_storyinfo[chapter].config, 0, sizeof(StoryInfoConfig)*g_storyinfo[chapter].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `chapter`,`rank`,`id`,`saveoffset`,`type`,`preid`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3`,`exp`,`silver`,`awardgroup`,`ranknum`,`ranknum_saveoffset`,`ranksec`,`ranksec_saveoffset`,`body`,`returnbody`,`skip`,`color` from story;" );
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
		int chapter = atoi( row[0] );
		if ( chapter < 0 || chapter >= g_storyinfo_maxnum  )
			continue;
		int rank = atoi( row[1] );
		if ( rank < 0 || rank >= g_storyinfo[chapter].maxnum )
			continue;
		g_storyinfo[chapter].config[rank].chapter = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].rank = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].id = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].saveoffset = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].type = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].preid = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].monsterid[0] = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].monsterid[1] = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].monsterid[2] = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].monsterid[3] = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].exp = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].silver = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].awardgroup = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].ranknum = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].ranknum_saveoffset = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].ranksec = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].ranksec_saveoffset = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].body = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].returnbody = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].skip = atoi(row[offset++]);
		g_storyinfo[chapter].config[rank].color = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int storyinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_storyinfo_maxnum; tmpi++ )
	{
		if ( g_storyinfo[tmpi].config )
		{
			free( g_storyinfo[tmpi].config );
			g_storyinfo[tmpi].config = NULL;
		}
	}
	if ( g_storyinfo )
	{
		free( g_storyinfo );
		g_storyinfo = NULL;
	}
	g_storyinfo_maxnum = 0;
	storyinfo_init_auto();
	return 0;
}

