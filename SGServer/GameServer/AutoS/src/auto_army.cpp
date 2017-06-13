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

	sprintf( szSQL, "select `index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`walk_len`,`name`,`soldiers`,`wounded`,`body`,`morale`,`attack`,`defense`,`attack_speed`,`move_speed`,`attrpoint`,`level`,`exp`,`equip0`,`equip1`,`equip2`,`equip3`,`equip4`,`equip5`,`skillid0`,`skillid1`,`skillid2`,`skillid3`,`food`,`money`,`itemkind0`,`itemkind1`,`itemkind2`,`itemkind3`,`itemnum0`,`itemnum1`,`itemnum2`,`itemnum3`,`sec` from %s ", pTab );
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
		pArmy->walk_len = atoi(row[offset++]);
		memcpy( pArmy->name, row[offset++], 22 ); pArmy->name[21]=0;
		pArmy->soldiers = atoi(row[offset++]);
		pArmy->wounded = atoi(row[offset++]);
		pArmy->body = atoi(row[offset++]);
		pArmy->morale = atoi(row[offset++]);
		pArmy->attack = atoi(row[offset++]);
		pArmy->defense = atoi(row[offset++]);
		pArmy->attack_speed = atoi(row[offset++]);
		pArmy->move_speed = atoi(row[offset++]);
		pArmy->attrpoint = atoi(row[offset++]);
		pArmy->level = atoi(row[offset++]);
		pArmy->exp = atoi(row[offset++]);
		pArmy->equip[0] = atoi(row[offset++]);
		pArmy->equip[1] = atoi(row[offset++]);
		pArmy->equip[2] = atoi(row[offset++]);
		pArmy->equip[3] = atoi(row[offset++]);
		pArmy->equip[4] = atoi(row[offset++]);
		pArmy->equip[5] = atoi(row[offset++]);
		pArmy->skillid[0] = atoi(row[offset++]);
		pArmy->skillid[1] = atoi(row[offset++]);
		pArmy->skillid[2] = atoi(row[offset++]);
		pArmy->skillid[3] = atoi(row[offset++]);
		pArmy->food = atoi(row[offset++]);
		pArmy->money = atoi(row[offset++]);
		pArmy->itemkind[0] = atoi(row[offset++]);
		pArmy->itemkind[1] = atoi(row[offset++]);
		pArmy->itemkind[2] = atoi(row[offset++]);
		pArmy->itemkind[3] = atoi(row[offset++]);
		pArmy->itemnum[0] = atoi(row[offset++]);
		pArmy->itemnum[1] = atoi(row[offset++]);
		pArmy->itemnum[2] = atoi(row[offset++]);
		pArmy->itemnum[3] = atoi(row[offset++]);
		pArmy->sec = atoi(row[offset++]);
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

	char szText_name[MAX_PATH]={0};
RE_ARMY_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`walk_len`,`name`,`soldiers`,`wounded`,`body`,`morale`,`attack`,`defense`,`attack_speed`,`move_speed`,`attrpoint`,`level`,`exp`,`equip0`,`equip1`,`equip2`,`equip3`,`equip4`,`equip5`,`skillid0`,`skillid1`,`skillid2`,`skillid3`,`food`,`money`,`itemkind0`,`itemkind1`,`itemkind2`,`itemkind3`,`itemnum0`,`itemnum1`,`itemnum2`,`itemnum3`,`sec`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pArmy->index,pArmy->id,pArmy->state,pArmy->statetime,pArmy->stateduration,pArmy->action,pArmy->from_type,pArmy->from_id,pArmy->from_index,pArmy->from_posx,pArmy->from_posy,pArmy->to_type,pArmy->to_id,pArmy->to_index,pArmy->to_posx,pArmy->to_posy,pArmy->posx,pArmy->posy,pArmy->walk_len,db_escape((const char *)pArmy->name,szText_name,0),pArmy->soldiers,pArmy->wounded,pArmy->body,pArmy->morale,pArmy->attack,pArmy->defense,pArmy->attack_speed,pArmy->move_speed,pArmy->attrpoint,pArmy->level,pArmy->exp,pArmy->equip[0],pArmy->equip[1],pArmy->equip[2],pArmy->equip[3],pArmy->equip[4],pArmy->equip[5],pArmy->skillid[0],pArmy->skillid[1],pArmy->skillid[2],pArmy->skillid[3],pArmy->food,pArmy->money,pArmy->itemkind[0],pArmy->itemkind[1],pArmy->itemkind[2],pArmy->itemkind[3],pArmy->itemnum[0],pArmy->itemnum[1],pArmy->itemnum[2],pArmy->itemnum[3],pArmy->sec);
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

	char szText_name[MAX_PATH]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pArmy[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`id`,`state`,`statetime`,`stateduration`,`action`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`posx`,`posy`,`walk_len`,`name`,`soldiers`,`wounded`,`body`,`morale`,`attack`,`defense`,`attack_speed`,`move_speed`,`attrpoint`,`level`,`exp`,`equip0`,`equip1`,`equip2`,`equip3`,`equip4`,`equip5`,`skillid0`,`skillid1`,`skillid2`,`skillid3`,`food`,`money`,`itemkind0`,`itemkind1`,`itemkind2`,`itemkind3`,`itemnum0`,`itemnum1`,`itemnum2`,`itemnum3`,`sec`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pArmy[index].index,pArmy[index].id,pArmy[index].state,pArmy[index].statetime,pArmy[index].stateduration,pArmy[index].action,pArmy[index].from_type,pArmy[index].from_id,pArmy[index].from_index,pArmy[index].from_posx,pArmy[index].from_posy,pArmy[index].to_type,pArmy[index].to_id,pArmy[index].to_index,pArmy[index].to_posx,pArmy[index].to_posy,pArmy[index].posx,pArmy[index].posy,pArmy[index].walk_len,db_escape((const char *)pArmy[index].name,szText_name,0),pArmy[index].soldiers,pArmy[index].wounded,pArmy[index].body,pArmy[index].morale,pArmy[index].attack,pArmy[index].defense,pArmy[index].attack_speed,pArmy[index].move_speed,pArmy[index].attrpoint,pArmy[index].level,pArmy[index].exp,pArmy[index].equip[0],pArmy[index].equip[1],pArmy[index].equip[2],pArmy[index].equip[3],pArmy[index].equip[4],pArmy[index].equip[5],pArmy[index].skillid[0],pArmy[index].skillid[1],pArmy[index].skillid[2],pArmy[index].skillid[3],pArmy[index].food,pArmy[index].money,pArmy[index].itemkind[0],pArmy[index].itemkind[1],pArmy[index].itemkind[2],pArmy[index].itemkind[3],pArmy[index].itemnum[0],pArmy[index].itemnum[1],pArmy[index].itemnum[2],pArmy[index].itemnum[3],pArmy[index].sec);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pArmy[index].index,pArmy[index].id,pArmy[index].state,pArmy[index].statetime,pArmy[index].stateduration,pArmy[index].action,pArmy[index].from_type,pArmy[index].from_id,pArmy[index].from_index,pArmy[index].from_posx,pArmy[index].from_posy,pArmy[index].to_type,pArmy[index].to_id,pArmy[index].to_index,pArmy[index].to_posx,pArmy[index].to_posy,pArmy[index].posx,pArmy[index].posy,pArmy[index].walk_len,db_escape((const char *)pArmy[index].name,szText_name,0),pArmy[index].soldiers,pArmy[index].wounded,pArmy[index].body,pArmy[index].morale,pArmy[index].attack,pArmy[index].defense,pArmy[index].attack_speed,pArmy[index].move_speed,pArmy[index].attrpoint,pArmy[index].level,pArmy[index].exp,pArmy[index].equip[0],pArmy[index].equip[1],pArmy[index].equip[2],pArmy[index].equip[3],pArmy[index].equip[4],pArmy[index].equip[5],pArmy[index].skillid[0],pArmy[index].skillid[1],pArmy[index].skillid[2],pArmy[index].skillid[3],pArmy[index].food,pArmy[index].money,pArmy[index].itemkind[0],pArmy[index].itemkind[1],pArmy[index].itemkind[2],pArmy[index].itemkind[3],pArmy[index].itemnum[0],pArmy[index].itemnum[1],pArmy[index].itemnum[2],pArmy[index].itemnum[3],pArmy[index].sec);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 300 )
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
