#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_army.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_army_maxindex = 0;

int army_load_auto( LPCB_GETARMY pCB_GetArmy, LPCB_LOADARMY pCB_LoadArmy, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Army *pArmy;
	int index = 0;

	sprintf( szSQL, "select `index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`herokind0`,`herokind1`,`herokind2`,`herokind3` from %s ", pTab );
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
		pArmy = pCB_GetArmy( index );
		if( pArmy == NULL )
			continue;
		pArmy->index = atoi(row[offset++]);
		pArmy->id = atoi(row[offset++]);
		pArmy->state = atoi(row[offset++]);
		pArmy->statetime = atoi(row[offset++]);
		pArmy->stateduration = atoi(row[offset++]);
		pArmy->action = atoi(row[offset++]);
		pArmy->from_type = atoi(row[offset++]);
		pArmy->from_id = atoi(row[offset++]);
		pArmy->from_index = atoi(row[offset++]);
		pArmy->from_posx = atoi(row[offset++]);
		pArmy->from_posy = atoi(row[offset++]);
		pArmy->to_type = atoi(row[offset++]);
		pArmy->to_id = atoi(row[offset++]);
		pArmy->to_index = atoi(row[offset++]);
		pArmy->to_posx = atoi(row[offset++]);
		pArmy->to_posy = atoi(row[offset++]);
		pArmy->posx = atoi(row[offset++]);
		pArmy->posy = atoi(row[offset++]);
		pArmy->herokind[0] = atoi(row[offset++]);
		pArmy->herokind[1] = atoi(row[offset++]);
		pArmy->herokind[2] = atoi(row[offset++]);
		pArmy->herokind[3] = atoi(row[offset++]);
		if( pCB_LoadArmy )
			pCB_LoadArmy( pArmy->index );
		index += 1;
	}
	g_army_maxindex = index;
	mysql_free_result( res );
	return 0;
}
int army_save_auto( Army *pArmy, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pArmy == NULL )
		return -1;

RE_ARMY_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`herokind0`,`herokind1`,`herokind2`,`herokind3`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pArmy->index,pArmy->id,pArmy->state,pArmy->statetime,pArmy->stateduration,pArmy->action,pArmy->from_type,pArmy->from_id,pArmy->from_index,pArmy->from_posx,pArmy->from_posy,pArmy->to_type,pArmy->to_id,pArmy->to_index,pArmy->to_posx,pArmy->to_posy,pArmy->posx,pArmy->posy,pArmy->herokind[0],pArmy->herokind[1],pArmy->herokind[2],pArmy->herokind[3]);
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
			goto RE_ARMY_UPDATE;
		}
		return -1;
	}
	return 0;
}

int army_batch_save_auto( Army *pArmy, int maxcount, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pArmy == NULL )
		return -1;

	char reconnect_flag = 0;
RE_ARMY_TRUNCATE:
	sprintf( szSQL, "TRUNCATE `%s`", pTab );
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag == 0 )
		{
			if( mysql_ping(myGame) != 0 )
			{
				db_reconnect_game();
				reconnect_flag = 1;
				goto RE_ARMY_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pArmy[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`herokind0`,`herokind1`,`herokind2`,`herokind3`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pArmy[index].index,pArmy[index].id,pArmy[index].state,pArmy[index].statetime,pArmy[index].stateduration,pArmy[index].action,pArmy[index].from_type,pArmy[index].from_id,pArmy[index].from_index,pArmy[index].from_posx,pArmy[index].from_posy,pArmy[index].to_type,pArmy[index].to_id,pArmy[index].to_index,pArmy[index].to_posx,pArmy[index].to_posy,pArmy[index].posx,pArmy[index].posy,pArmy[index].herokind[0],pArmy[index].herokind[1],pArmy[index].herokind[2],pArmy[index].herokind[3]);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pArmy[index].index,pArmy[index].id,pArmy[index].state,pArmy[index].statetime,pArmy[index].stateduration,pArmy[index].action,pArmy[index].from_type,pArmy[index].from_id,pArmy[index].from_index,pArmy[index].from_posx,pArmy[index].from_posy,pArmy[index].to_type,pArmy[index].to_id,pArmy[index].to_index,pArmy[index].to_posx,pArmy[index].to_posy,pArmy[index].posx,pArmy[index].posy,pArmy[index].herokind[0],pArmy[index].herokind[1],pArmy[index].herokind[2],pArmy[index].herokind[3]);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 256 )
		{
			count = 0;
			db_query( fp, g_batchsql );
			memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
		}
	}
	if ( count > 0 )
	{
		count = 0;
		db_query( fp, g_batchsql );
		memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	}
	return 0;
}
