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
#include "auto_data_quest.h"
extern MYSQL *myData;
extern lua_State* servL;
QuestInfo *g_questinfo = NULL;
int g_questinfo_maxnum = 0;

int questinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(questid) from quest;" );
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
			g_questinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_questinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_questinfo = (QuestInfo *)malloc( sizeof(QuestInfo)*g_questinfo_maxnum );
	memset( g_questinfo, 0, sizeof(QuestInfo)*g_questinfo_maxnum );

	sprintf( szSQL, "select `questid`,`type`,`sort`,`preid`,`datatype`,`datakind`,`dataoffset`,`needvalue`,`brushlevel`,`brushnum`,`brushrange`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardkind4`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3`,`awardnum4` from quest;" );
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
		int questid = atoi( row[0] );
		if ( questid < 0 || questid >= g_questinfo_maxnum  )
			continue;
		g_questinfo[questid].questid = atoi(row[offset++]);
		g_questinfo[questid].type = atoi(row[offset++]);
		g_questinfo[questid].sort = atoi(row[offset++]);
		g_questinfo[questid].preid = atoi(row[offset++]);
		g_questinfo[questid].datatype = atoi(row[offset++]);
		g_questinfo[questid].datakind = atoi(row[offset++]);
		g_questinfo[questid].dataoffset = atoi(row[offset++]);
		g_questinfo[questid].needvalue = atoi(row[offset++]);
		g_questinfo[questid].brushlevel = atoi(row[offset++]);
		g_questinfo[questid].brushnum = atoi(row[offset++]);
		g_questinfo[questid].brushrange = atoi(row[offset++]);
		g_questinfo[questid].awardkind[0] = atoi(row[offset++]);
		g_questinfo[questid].awardkind[1] = atoi(row[offset++]);
		g_questinfo[questid].awardkind[2] = atoi(row[offset++]);
		g_questinfo[questid].awardkind[3] = atoi(row[offset++]);
		g_questinfo[questid].awardkind[4] = atoi(row[offset++]);
		g_questinfo[questid].awardnum[0] = atoi(row[offset++]);
		g_questinfo[questid].awardnum[1] = atoi(row[offset++]);
		g_questinfo[questid].awardnum[2] = atoi(row[offset++]);
		g_questinfo[questid].awardnum[3] = atoi(row[offset++]);
		g_questinfo[questid].awardnum[4] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	questinfo_luatable_auto();
	return 0;
}

int questinfo_reload_auto()
{
	if ( g_questinfo )
	{
		free( g_questinfo );
		g_questinfo = NULL;
	}
	g_questinfo_maxnum = 0;
	questinfo_init_auto();
	return 0;
}

int questinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int questid = 0; questid < g_questinfo_maxnum; questid++ )
	{
		lua_pushinteger( servL, questid );
		lua_newtable( servL );

		lua_pushstring( servL, "questid" );
		lua_pushinteger( servL, g_questinfo[questid].questid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "type" );
		lua_pushinteger( servL, g_questinfo[questid].type );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "sort" );
		lua_pushinteger( servL, g_questinfo[questid].sort );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "preid" );
		lua_pushinteger( servL, g_questinfo[questid].preid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "datatype" );
		lua_pushinteger( servL, g_questinfo[questid].datatype );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "datakind" );
		lua_pushinteger( servL, g_questinfo[questid].datakind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "dataoffset" );
		lua_pushinteger( servL, g_questinfo[questid].dataoffset );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "needvalue" );
		lua_pushinteger( servL, g_questinfo[questid].needvalue );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "brushlevel" );
		lua_pushinteger( servL, g_questinfo[questid].brushlevel );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "brushnum" );
		lua_pushinteger( servL, g_questinfo[questid].brushnum );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "brushrange" );
		lua_pushinteger( servL, g_questinfo[questid].brushrange );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "awardkind" );
		lua_newtable( servL );
		for ( int i = 0; i < 5; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_questinfo[questid].awardkind[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "awardnum" );
		lua_newtable( servL );
		for ( int i = 0; i < 5; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_questinfo[questid].awardnum[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_questinfo" );

	lua_pushinteger( servL, g_questinfo_maxnum );
	lua_setglobal( servL, "g_questinfo_maxnum" );
	return 0;
}
