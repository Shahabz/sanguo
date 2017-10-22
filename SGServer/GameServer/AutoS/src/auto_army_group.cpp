#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_army_group.h"
extern MYSQL *myGame;
int g_army_group_maxindex = 0;

int army_group_load_auto( LPCB_GETARMYGROUP pCB_GetArmyGroup, LPCB_LOADARMYGROUP pCB_LoadArmyGroup, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	ArmyGroup *pArmyGroup;
	int index = 0;

	sprintf( szSQL, "select `index`,`id`,`state`,`statetime`,`stateduration`,`type`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`from_nation`,`from_helpnum`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`to_nation`,`to_helpnum`,`leader_index`,`attack_armyindex`,`defense_armyindex` from %s ", pTab );
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
		pArmyGroup = pCB_GetArmyGroup( atoi(row[0]) );
		if( pArmyGroup == NULL )
			continue;
		pArmyGroup->index = atoi(row[offset++]);
		pArmyGroup->id = atoi(row[offset++]);
		pArmyGroup->state = atoi(row[offset++]);
		pArmyGroup->statetime = atoi(row[offset++]);
		pArmyGroup->stateduration = atoi(row[offset++]);
		pArmyGroup->type = atoi(row[offset++]);
		pArmyGroup->from_type = atoi(row[offset++]);
		pArmyGroup->from_id = atoi(row[offset++]);
		pArmyGroup->from_index = atoi(row[offset++]);
		pArmyGroup->from_posx = atoi(row[offset++]);
		pArmyGroup->from_posy = atoi(row[offset++]);
		pArmyGroup->from_nation = atoi(row[offset++]);
		pArmyGroup->from_helpnum = atoi(row[offset++]);
		pArmyGroup->to_type = atoi(row[offset++]);
		pArmyGroup->to_id = atoi(row[offset++]);
		pArmyGroup->to_index = atoi(row[offset++]);
		pArmyGroup->to_posx = atoi(row[offset++]);
		pArmyGroup->to_posy = atoi(row[offset++]);
		pArmyGroup->to_nation = atoi(row[offset++]);
		pArmyGroup->to_helpnum = atoi(row[offset++]);
		pArmyGroup->leader_index = atoi(row[offset++]);
		memcpy( pArmyGroup->attack_armyindex, row[offset++], sizeof(int)*128 );
		memcpy( pArmyGroup->defense_armyindex, row[offset++], sizeof(int)*128 );
		if( pCB_LoadArmyGroup )
			pCB_LoadArmyGroup( pArmyGroup->index );
		index = pArmyGroup->index;
		if ( index >= g_army_group_maxindex )
		{
			g_army_group_maxindex = index + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int army_group_save_auto( ArmyGroup *pArmyGroup, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pArmyGroup == NULL )
		return -1;

	char szText_attack_armyindex[sizeof(int)*128*2+1]={0};
	char szText_defense_armyindex[sizeof(int)*128*2+1]={0};
RE_ARMYGROUP_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`id`,`state`,`statetime`,`stateduration`,`type`,`from_type`,`from_id`,`from_index`,`from_posx`,`from_posy`,`from_nation`,`from_helpnum`,`to_type`,`to_id`,`to_index`,`to_posx`,`to_posy`,`to_nation`,`to_helpnum`,`leader_index`,`attack_armyindex`,`defense_armyindex`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%s')",pTab,pArmyGroup->index,pArmyGroup->id,pArmyGroup->state,pArmyGroup->statetime,pArmyGroup->stateduration,pArmyGroup->type,pArmyGroup->from_type,pArmyGroup->from_id,pArmyGroup->from_index,pArmyGroup->from_posx,pArmyGroup->from_posy,pArmyGroup->from_nation,pArmyGroup->from_helpnum,pArmyGroup->to_type,pArmyGroup->to_id,pArmyGroup->to_index,pArmyGroup->to_posx,pArmyGroup->to_posy,pArmyGroup->to_nation,pArmyGroup->to_helpnum,pArmyGroup->leader_index,db_escape((const char *)pArmyGroup->attack_armyindex,szText_attack_armyindex,sizeof(int)*128),db_escape((const char *)pArmyGroup->defense_armyindex,szText_defense_armyindex,sizeof(int)*128));
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
			goto RE_ARMYGROUP_UPDATE;
		}
		return -1;
	}
	return 0;
}

