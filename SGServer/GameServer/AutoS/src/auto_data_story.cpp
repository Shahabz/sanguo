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

	sprintf( szSQL, "select max(`id`) from story;" );
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

	sprintf( szSQL, "select `id`,`chapter`,`rank`,`type`,`preid`,`nextid`,`unlock_preid`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3`,`exp`,`silver`,`awardgroup`,`body`,`lose_body`,`skip`,`color`,`shapeback`,`shape`,`star_saveoffset`,`hero_kind0`,`hero_kind1`,`hero_odds`,`hero_token`,`hero_saveoffset`,`restype`,`rescount`,`resnum`,`resnum_saveoffset`,`ressec`,`ressec_saveoffset`,`res_reset_num`,`res_reset_saveoffset`,`res_reset_token0`,`res_reset_token1`,`res_reset_token2`,`res_reset_token3`,`res_reset_token4`,`res_reset_token5`,`item_awardkind`,`item_awardodds`,`itemnum`,`itemnum_saveoffset`,`drawing_kind`,`drawing_token`,`drawing_saveoffset` from story;" );
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
		if ( id < 0 || id >= g_storyinfo_maxnum  )
			continue;
		g_storyinfo[id].id = atoi(row[offset++]);
		g_storyinfo[id].chapter = atoi(row[offset++]);
		g_storyinfo[id].rank = atoi(row[offset++]);
		g_storyinfo[id].type = atoi(row[offset++]);
		g_storyinfo[id].preid = atoi(row[offset++]);
		g_storyinfo[id].nextid = atoi(row[offset++]);
		g_storyinfo[id].unlock_preid = atoi(row[offset++]);
		g_storyinfo[id].monsterid[0] = atoi(row[offset++]);
		g_storyinfo[id].monsterid[1] = atoi(row[offset++]);
		g_storyinfo[id].monsterid[2] = atoi(row[offset++]);
		g_storyinfo[id].monsterid[3] = atoi(row[offset++]);
		g_storyinfo[id].exp = atoi(row[offset++]);
		g_storyinfo[id].silver = atoi(row[offset++]);
		g_storyinfo[id].awardgroup = atoi(row[offset++]);
		g_storyinfo[id].body = atoi(row[offset++]);
		g_storyinfo[id].lose_body = atoi(row[offset++]);
		g_storyinfo[id].skip = atoi(row[offset++]);
		g_storyinfo[id].color = atoi(row[offset++]);
		g_storyinfo[id].shapeback = atoi(row[offset++]);
		g_storyinfo[id].shape = atoi(row[offset++]);
		g_storyinfo[id].star_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].hero_kind[0] = atoi(row[offset++]);
		g_storyinfo[id].hero_kind[1] = atoi(row[offset++]);
		g_storyinfo[id].hero_odds = atoi(row[offset++]);
		g_storyinfo[id].hero_token = atoi(row[offset++]);
		g_storyinfo[id].hero_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].restype = atoi(row[offset++]);
		g_storyinfo[id].rescount = atoi(row[offset++]);
		g_storyinfo[id].resnum = atoi(row[offset++]);
		g_storyinfo[id].resnum_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].ressec = atoi(row[offset++]);
		g_storyinfo[id].ressec_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].res_reset_num = atoi(row[offset++]);
		g_storyinfo[id].res_reset_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[0] = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[1] = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[2] = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[3] = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[4] = atoi(row[offset++]);
		g_storyinfo[id].res_reset_token[5] = atoi(row[offset++]);
		g_storyinfo[id].item_awardkind = atoi(row[offset++]);
		g_storyinfo[id].item_awardodds = atoi(row[offset++]);
		g_storyinfo[id].itemnum = atoi(row[offset++]);
		g_storyinfo[id].itemnum_saveoffset = atoi(row[offset++]);
		g_storyinfo[id].drawing_kind = atoi(row[offset++]);
		g_storyinfo[id].drawing_token = atoi(row[offset++]);
		g_storyinfo[id].drawing_saveoffset = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int storyinfo_reload_auto()
{
	if ( g_storyinfo )
	{
		free( g_storyinfo );
		g_storyinfo = NULL;
	}
	g_storyinfo_maxnum = 0;
	storyinfo_init_auto();
	return 0;
}

