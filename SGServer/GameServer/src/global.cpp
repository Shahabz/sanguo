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
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "global.h"
extern lua_State* servL;
extern MYSQL *myGame;
extern MYSQL *myData;
extern int g_serverpoint;
extern int g_server_citylevel5_count;

Global global;
int g_world_data[WORLD_DATA_MAX] = {0};

int global_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	sprintf( szSQL, "select id, value from global" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	res = mysql_store_result( myData );

	lua_newtable( servL );
	while ( (row = mysql_fetch_row( res )) )
	{	
		int id = atoi( row[0] );
		float value = (float)atof( row[1] );
		if ( id == 1 )
		{
			global.actorlevel_max = atoi( row[1] );
			lua_pushstring( servL, "actorlevel_max" );
			lua_pushinteger( servL, global.actorlevel_max );
			lua_rawset( servL, -3 );
		}	
		else if ( id == 2 )
		{
			global.body_max = atoi( row[1] );
		}
		else if ( id == 3 )
		{
			global.body_sec = atoi( row[1] );
		}
		else if ( id == 4 )
		{
			global.levy_sec = atoi( row[1] );
		}
		else if ( id == 5 )
		{
			global.levy_max = atoi( row[1] );
		}
		else if ( id == 6 )
		{
			global.building_delete_cd = atoi( row[1] );
		}
		else if ( id == 7 )
		{
			global.equip_wash_max = atoi( row[1] );
		}
		else if ( id == 8 )
		{
			global.equip_wash_sec = atoi( row[1] );
		}
		else if ( id == 9 )
		{
			global.hero_wash_max = atoi( row[1] );
		}
		else if ( id == 10 )
		{
			global.hero_wash_sec = atoi( row[1] );
		}
		else if ( id == 11 )
		{
			global.buildinglevel_max = atoi( row[1] );
		}
		else if ( id == 12 )
		{
			global.city_guard_sec = atoi( row[1] );
		}
		else if ( id == 13 )
		{
			global.city_guard_sec_token = value;
		}
		else if ( id == 14 )
		{
			global.city_guard_up_token = atoi( row[1] );
		}
		else if ( id == 15 )
		{
			global.worker_freetime = atoi( row[1] );
		}
		else if ( id == 16 )
		{
			global.trainfood = atoi( row[1] );
		}
		else if ( id == 17 )
		{
			global.equipext_token = atoi( row[1] );
		}
		else if ( id == 18 )
		{
			global.nequip_open_silver = atoi( row[1] );
		}
		else if ( id == 19 )
		{
			global.nequip_open_wood = atoi( row[1] );
		}
		else if ( id == 20 )
			global.battlepower_attack = value;
		else if ( id == 21 )
			global.battlepower_defense = value;
		else if ( id == 22 )
			global.battlepower_troops = value;
		else if ( id == 23 )
			global.battlepower_attack_increase = value;
		else if ( id == 24 )
			global.battlepower_defense_increase = value;
		else if ( id == 25 )
			global.battlepower_assault = value;
		else if ( id == 26 )
			global.battlepower_defend = value;
		else if ( id == 27 )
			global.techtree_level = atoi( row[1] );
		else if ( id == 28 )
			global.techquick_token = value;
		else if ( id == 29 )
			global.forgingquick_token = value;
		else if ( id == 30 )
			global.upgradequick_token = value;
		else if ( id == 31 )
			global.worker_expire_1 = atoi( row[1] );
		else if ( id == 32 )
			global.worker_expire_7 = atoi( row[1] );
		else if ( id == 33 )
			global.worker_token_1 = atoi( row[1] );
		else if ( id == 34 )
			global.worker_token_7 = atoi( row[1] );
		else if ( id == 35 )
			global.city_guard_level = atoi( row[1] );
		else if ( id == 36 )
			global.city_guard_color_min[1] = atoi( row[1] );
		else if ( id == 37 )
			global.city_guard_color_min[2] = atoi( row[1] );
		else if ( id == 38 )
			global.city_guard_color_min[3] = atoi( row[1] );
		else if ( id == 39 )
			global.city_guard_color_min[4] = atoi( row[1] );
		else if ( id == 40 )
			global.city_guard_color_min[5] = atoi( row[1] );
		else if ( id == 41 )
			global.nation_award_token = atoi( row[1] );
		else if ( id == 42 )
			global.hero_wash_token = atoi( row[1] );
		else if ( id == 43 )
			global.hero_wash_free_rand = atoi( row[1] );
		else if ( id == 44 )
			global.hero_wash_token_rand = atoi( row[1] );
		else if ( id == 45 )
			global.hero_wash_ratio_down = atoi( row[1] );
		else if ( id == 46 )
			global.hero_wash_ratio_up = atoi( row[1] );
		else if ( id == 47 )
			global.hero_wash_token_full_base = atoi( row[1] );
		else if ( id == 48 )
			global.hero_wash_token_full_odds = atoi( row[1] );
		else if ( id == 49 )
			global.hero_colorup_actorlevel = atoi( row[1] );
		else if ( id == 50 )
			global.hero_god_actorlevel = atoi( row[1] );
		else if ( id == 51 )
			global.equip_wash_super_token = atoi( row[1] );
		else if ( id == 52 )
			global.people_sec = atoi( row[1] );
		else if ( id == 53 )
			global.material_make_value1 = value;
		else if ( id == 54 )
			global.material_make_value2 = atoi( row[1] );
		else if ( id == 55 )
			global.people_add_v1 = atoi( row[1] );
		else if ( id == 56 )
			global.people_add_v2 = atoi( row[1] );
		else if ( id == 57 )
			global.people_sub_v1 = atoi( row[1] );
		else if ( id == 58 )
			global.people_sub_v2 = atoi( row[1] );
		else if ( id == 59 )
			global.fight_crit_odds = atoi( row[1] );
		else if ( id == 60 )
			global.fight_crit_damage = atoi( row[1] );
		else if ( id == 61 )
			global.fight_dodge = atoi( row[1] );
		else if ( id == 62 )
			global.fight_control_value = atoi( row[1] );
		else if ( id == 63 )
			global.fight_attackmin_value = atoi( row[1] );
		else if ( id == 64 )
			global.fight_v1 = atoi( row[1] );
		else if ( id == 65 )
			global.fight_v2 = atoi( row[1] );
		else if ( id == 66 )
			global.fight_damage_randmin = atoi( row[1] );
		else if ( id == 67 )
			global.fight_damage_randmax = atoi( row[1] );
		else if ( id == 68 )
		{
			global.brush_enemy_minute = atoi( row[1] );
			lua_pushstring( servL, "brush_enemy_minute" );
			lua_pushinteger( servL, global.brush_enemy_minute );
			lua_rawset( servL, -3 );
		}
		else if ( id == 69 )
		{
			global.brush_res_minute = atoi( row[1] );
			lua_pushstring( servL, "brush_res_minute" );
			lua_pushinteger( servL, global.brush_res_minute );
			lua_rawset( servL, -3 );
		}
		else if ( id == 70 )
			global.map_gather_sec = atoi( row[1] );
	}
	lua_setglobal( servL, "global" );
	mysql_free_result( res );
	return 0;
}

void global_reload()
{
	global_init();
	world_data_init();
}

// 全局数据初始化
int world_data_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int		index = 0;

	memset( &g_world_data, 0, sizeof( int ) * WORLD_DATA_MAX );

	sprintf( szSQL, "select id, value from world_data" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		index = atoi( row[0] );
		if ( index >= 0 && index < WORLD_DATA_MAX )
		{
			g_world_data[index] = atoi( row[1] );
		}
	}
	mysql_free_result( res );

	return 0;
}

// 从数据库获取全局数据
int world_data_get( int index, char *strvalue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int nValue = -1;

	sprintf( szSQL, "select value,strvalue from world_data where id='%d'", index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
		{
			nValue = atoi( row[0] );
			if ( strvalue && row[1] )
			{
				strcpy( strvalue, row[1] );
			}
		}
	}
	mysql_free_result( res );

	return nValue;
}

int world_data_set( int index, int value, char *strvalue, FILE *fp )
{
	char szSQL[1024];

	if ( index >= 0 && index < WORLD_DATA_MAX )
	{
		g_world_data[index] = value;
	}

	if ( strvalue )
		sprintf( szSQL, "replace into world_data ( id, value, strvalue) values('%d','%d','%s')", index, value, strvalue );
	else
		sprintf( szSQL, "replace into world_data ( id, value, strvalue) values('%d','%d','')", index, value );

	if ( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	return 0;
}
