#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_nation.h"
extern MYSQL *myGame;
int g_nation_maxindex = 0;

int nation_load_auto( LPCB_GETNATION pCB_GetNation, LPCB_LOADNATION pCB_LoadNation, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Nation *pNation;
	int nation = 0;

	sprintf( szSQL, "select `nation`,`level`,`exp`,`notice`,`official_actorid`,`tiance_point`,`questlevel`,`questkind`,`missionlevel`,`missionvalue` from %s ", pTab );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		pNation = pCB_GetNation( atoi(row[0]) );
		if( pNation == NULL )
			continue;
		pNation->nation = atoi(row[offset++]);
		pNation->level = atoi(row[offset++]);
		pNation->exp = atoi(row[offset++]);
		memcpy( pNation->notice, row[offset++], 512 ); pNation->notice[511]=0;
		memcpy( pNation->official_actorid, row[offset++], sizeof(int)*11 );
		pNation->tiance_point = atoi(row[offset++]);
		memcpy( pNation->questlevel, row[offset++], sizeof(char)*3 );
		memcpy( pNation->questkind, row[offset++], sizeof(char)*3 );
		pNation->missionlevel = atoi(row[offset++]);
		memcpy( pNation->missionvalue, row[offset++], sizeof(int)*4 );
		if( pCB_LoadNation )
			pCB_LoadNation( pNation->nation );
		nation = pNation->nation;
		if ( nation >= g_nation_maxindex )
		{
			g_nation_maxindex = nation + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int nation_save_auto( Nation *pNation, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pNation == NULL )
		return -1;

	char szText_notice[MAX_PATH]={0};
	char szText_official_actorid[sizeof(int)*11*2+1]={0};
	char szText_questlevel[sizeof(char)*3*2+1]={0};
	char szText_questkind[sizeof(char)*3*2+1]={0};
	char szText_missionvalue[sizeof(int)*4*2+1]={0};
RE_NATION_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`nation`,`level`,`exp`,`notice`,`official_actorid`,`tiance_point`,`questlevel`,`questkind`,`missionlevel`,`missionvalue`) Values('%d','%d','%d','%s','%s','%d','%s','%s','%d','%s')",pTab,pNation->nation,pNation->level,pNation->exp,db_escape((const char *)pNation->notice,szText_notice,0),db_escape((const char *)pNation->official_actorid,szText_official_actorid,sizeof(int)*11),pNation->tiance_point,db_escape((const char *)pNation->questlevel,szText_questlevel,sizeof(char)*3),db_escape((const char *)pNation->questkind,szText_questkind,sizeof(char)*3),pNation->missionlevel,db_escape((const char *)pNation->missionvalue,szText_missionvalue,sizeof(int)*4));
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myGame) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_NATION_UPDATE;
		}
		return -1;
	}
	return 0;
}

