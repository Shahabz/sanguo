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
#include "world_quest.h"
extern lua_State* servL;
extern MYSQL *myGame;
extern MYSQL *myData;
extern int g_serverpoint;
extern int g_server_citylevel5_count;

extern char g_open_town3;
extern char g_open_town6;
extern char g_open_townking;

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
			global.hero_battle_minhp = atoi( row[1] );
		else if ( id == 71 )
			global.army_move = atoi( row[1] );
		else if ( id == 72 )
			global.army_march_food_v1 = value;
		else if ( id == 73 )
			global.army_march_food_v2 = value;
		else if ( id == 74 )
			global.army_march_food_v3 = value;
		else if ( id == 75 )
			global.hero_gather_duration[0] = atoi( row[1] );
		else if ( id == 76 )
			global.hero_gather_duration[1] = atoi( row[1] );
		else if ( id == 77 )
			global.hero_gather_duration[2] = atoi( row[1] );
		else if ( id == 78 )
			global.hero_gather_duration[3] = atoi( row[1] );
		else if ( id == 79 )
			global.hero_gather_duration[4] = atoi( row[1] );
		else if ( id == 80 )
			global.hero_gather_duration[5] = atoi( row[1] );
		else if ( id == 81 )
			global.army_move_kingzone = atoi( row[1] );
		else if ( id == 82 )
			global.mapevent_sec = atoi( row[1] );
		else if ( id == 83 )
			global.spy_token = atoi( row[1] );
		else if ( id == 84 )
			global.nation_mail_token = atoi( row[1] );
		else if ( id == 85 )
			global.spy_add_techlevel1 = atoi( row[1] );
		else if ( id == 86 )
			global.spy_add_techlevel2 = atoi( row[1] );
		else if ( id == 87 )
			global.spy_add_techlevel3 = atoi( row[1] );
		else if ( id == 88 )
			global.spy_res_odds = atoi( row[1] );
		else if ( id == 89 )
			global.spy_wall_odds = atoi( row[1] );
		else if ( id == 90 )
			global.spy_hero_odds = atoi( row[1] );
		else if ( id == 91 )
			global.cityhelp_level = atoi( row[1] );
		else if ( id == 92 )
			global.cityhelp_sec = atoi( row[1] );
		else if ( id == 93 )
			global.cityfight_sec_limit[0] = atoi( row[1] );
		else if ( id == 94 )
			global.cityfight_sec_limit[1] = atoi( row[1] );
		else if ( id == 95 )
			global.cityfight_sec_limit[2] = atoi( row[1] );
		else if ( id == 96 )
			global.cityfight_sec_group[0] = atoi( row[1] );
		else if ( id == 97 )
			global.cityfight_sec_group[1] = atoi( row[1] );
		else if ( id == 98 )
			global.cityfight_sec_group[2] = atoi( row[1] );
		else if ( id == 99 )
			global.cityfight_body_cost[0] = atoi( row[1] );
		else if ( id == 100 )
			global.cityfight_body_cost[1] = atoi( row[1] );
		else if ( id == 101 )
			global.cityfight_body_cost[2] = atoi( row[1] );
		else if ( id == 102 )
			global.cityfight_item_cost[0] = atoi( row[1] );
		else if ( id == 103 )
			global.cityfight_item_cost[1] = atoi( row[1] );
		else if ( id == 104 )
			global.cityfight_item_cost[2] = atoi( row[1] );

		if ( id == 105 )
		{
			global.cityfight_rob_v1 = value;
		}
		else if ( id == 106 )
		{
			global.cityfight_rob_v2 = value;
		}
		else if ( id == 107 )
		{
			global.cityfight_rob_v3 = value;
		}
		else if ( id == 108 )
		{
			global.cityfight_rob_v4 = value;
		}
		else if ( id == 109 )
		{
			global.town_owner_award = atoi( row[1] );
		}
		else if ( id == 110 )
		{
			global.town_dev_expmax[0] = atoi( row[1] );
		}
		else if ( id == 111 )
		{
			global.town_dev_expmax[1] = atoi( row[1] );
		}
		else if ( id == 112 )
		{
			global.town_dev_expmax[2] = atoi( row[1] );
		}
		else if ( id == 113 )
		{
			global.town_attackcd = atoi( row[1] );
		}
		else if ( id == 114 )
		{
			global.town_devcd = atoi( row[1] );
		}
		else if ( id == 115 )
		{
			global.town_dev_killenemy = atoi( row[1] );
		}
		else if ( id == 116 )
		{
			global.town_dev_gather = atoi( row[1] );
		}
		else if ( id == 117 )
		{
			global.town_dev_occupytown = atoi( row[1] );
		}
		else if ( id == 118 )
		{
			global.town_dev_contribute = atoi( row[1] );
		}
		else if ( id == 119 )
		{
			global.town_dev_silver = atoi( row[1] );
		}
		else if ( id == 120 )
		{
			global.town_dev_wood = atoi( row[1] );
		}
		else if ( id == 121 )
		{
			global.nationfight_actorlevel = atoi( row[1] );
		}
		else if ( id == 122 )
		{
			global.lost_rebuild_v1 = atoi( row[1] );
		}
		else if ( id == 123 )
		{
			global.lost_rebuild_v2 = value;
		}
		else if ( id == 124 )
		{
			global.lost_rebuild_v3 = atoi( row[1] );
		}
		else if ( id == 125 )
		{
			global.lost_rebuild_v4 = value;
		}
		else if ( id == 126 )
		{
			global.kingwar_activity_week = atoi( row[1] );
		}
		else if ( id == 127 )
		{
			global.kingwar_activity_hour = atoi( row[1] );
		}
		else if ( id == 128 )
		{
			global.kingwar_activity_minute = atoi( row[1] );
		}
		else if ( id == 129 )
		{
			global.kingwar_activity_duration = atoi( row[1] );
		}
		else if ( id == 130 )
		{
			global.kingwar_activity_duration_max = atoi( row[1] );
		}
		else if ( id == 131 )
		{
			global.kingwar_activity_addsec = atoi( row[1] );
		}
		else if ( id == 132 )
		{
			global.kingwar_dead_cd = atoi( row[1] );
		}
		else if ( id == 133 )
		{
			global.kingwar_token_def = atoi( row[1] );
		}
		else if ( id == 134 )
		{
			global.kingwar_token_sneak = atoi( row[1] );
		}
		else if ( id == 135 )
		{
			global.kingwar_token_pk = atoi( row[1] );
		}
		//else if ( id == 136 )
		//{
		//	global.kingwar_treasure_hour = atoi( row[1] );
		//}
		//else if ( id == 137 )
		//{
		//	global.kingwar_treasure_hour = atoi( row[1] );
		//}
		else if ( id == 138 )
			global.hero_visit_mainlevel = atoi( row[1] );
		else if ( id == 139 )
			global.hero_visit_actorlevel = atoi( row[1] );
		else if ( id == 140 )
			global.hero_visit_free_cd = atoi( row[1] );
		else if ( id == 141 )
			global.hero_visit_high_sec = atoi( row[1] );
		else if ( id == 142 )
			global.hero_visit_low_token = atoi( row[1] );
		else if ( id == 143 )
			global.hero_visit_low_token10 = atoi( row[1] );
		else if ( id == 144 )
			global.hero_visit_high_token = atoi( row[1] );
		else if ( id == 145 )
			global.hero_visit_high_token10 = atoi( row[1] );
		else if ( id == 146 )
			global.hero_visit_low_itemnum = atoi( row[1] );
		else if ( id == 147 )
			global.hero_visit_low_itemnum10 = atoi( row[1] );
		else if ( id == 148 )
			global.hero_visit_high_itemnum = atoi( row[1] );
		else if ( id == 149 )
			global.hero_visit_high_itemnum10 = atoi( row[1] );
		else if ( id == 150 )
			global.hero_visit_low_max = atoi( row[1] );
		else if ( id == 151 )
			global.hero_visit_high_max = atoi( row[1] );
		else if ( id == 152 )
			global.hero_visit_low_normal_award = atoi( row[1] );
		else if ( id == 153 )
			global.hero_visit_low_hero_award = atoi( row[1] );
		else if ( id == 154 )
			global.hero_visit_high_normal_award = atoi( row[1] );
		else if ( id == 155 )
			global.hero_visit_high_hero_award = atoi( row[1] );
		else if ( id == 156 )
			global.hero_visit_progress_normal = atoi( row[1] );
		else if ( id == 157 )
			global.hero_visit_progress_color1 = atoi( row[1] );
		else if ( id == 158 )
			global.hero_visit_progress_color2 = atoi( row[1] );
		else if ( id == 159 )
			global.hero_visit_progress_color3 = atoi( row[1] );
		else if ( id == 160 )
			global.hero_visit_progress_color4 = atoi( row[1] );
		else if ( id == 161 )
			global.hero_visit_progress_max = atoi( row[1] );
		else if ( id == 162 )
			global.autobuild_max = atoi( row[1] );
		else if ( id == 163 )
			global.hero_cabinet_level1 = atoi( row[1] );
		else if ( id == 164 )
			global.hero_cabinet_level2 = atoi( row[1] );
		else if ( id == 165 )
			global.hero_cabinet_level3 = atoi( row[1] );
		else if ( id == 166 )
			global.hero_cabinet_level4 = atoi( row[1] );
		else if ( id == 167 )
			global.hero_cabinet_guard_hp = atoi( row[1] );
		else if ( id == 168 )
			global.hero_cabinet_guard_token = atoi( row[1] );
		else if ( id == 169 )
			global.story_sweep_vip = atoi( row[1] );
		else if ( id == 170 )
			global.buff_train = atoi( row[1] );
		else if ( id == 171 )
			global.buff_wind = atoi( row[1] );
		else if ( id == 172 )
			global.buff_fire = atoi( row[1] );
		else if ( id == 173 )
			global.buff_mountain = atoi( row[1] );
		else if ( id == 174 )
			global.buff_forest = atoi( row[1] );
		else if ( id == 175 )
			global.autoguard_max = atoi( row[1] );
		else if ( id == 176 )
			global.nequip_crit_odds[0] = atoi( row[1] );
		else if ( id == 177 )
			global.nequip_crit_odds[1] = atoi( row[1] );
		else if ( id == 178 )
			global.nequip_crit_odds[2] = atoi( row[1] );
		else if ( id == 179 )
			global.nequip_crit_odds[3] = atoi( row[1] );
		else if ( id == 180 )
			global.nequip_crit_odds[4] = atoi( row[1] );
		else if ( id == 181 )
			global.nequip_make_quick_token = value;
		else if ( id == 182 )
			global.nequip_remake_quick_token = value;
		else if ( id == 183 )
			global.nation_quest_timer[0] = atoi( row[1] );
		else if ( id == 184 )
			global.nation_quest_timer[1] = atoi( row[1] );
		else if ( id == 185 )
			global.nation_quest_timer[2] = atoi( row[1] );
		else if ( id == 186 )
			global.nation_rank_vote[0] = atoi( row[1] );
		else if ( id == 187 )
			global.nation_rank_vote[1] = atoi( row[1] );
		else if ( id == 188 )
			global.nation_rank_vote[2] = atoi( row[1] );
		else if ( id == 189 )
			global.nation_rank_vote[3] = atoi( row[1] );
		else if ( id == 190 )
			global.nation_rank_vote[4] = atoi( row[1] );
		else if ( id == 191 )
			global.nation_place_max = atoi( row[1] );
		else if ( id == 192 )
			global.nation_official_term = atoi( row[1] );
		else if ( id == 193 )
			global.nation_official_select = atoi( row[1] );
		else if ( id == 194 )
			global.nation_vote_actorlevel = atoi( row[1] );
		else if ( id == 195 )
			global.nation_vote_basenum = atoi( row[1] );
		else if ( id == 196 )
			global.nation_change_notice_sec = atoi( row[1] );
		else if ( id == 197 )
			global.nation_online_notify_sec = atoi( row[1] );
		else if ( id == 198 )
			global.mapcall_range = atoi( row[1] );
		else if ( id == 199 )
			global.mapcall_nearby = atoi( row[1] );

		if ( id == 200 )
			global.hero_god_troops = atoi( row[1] );
		else if ( id == 201 )
			global.hero_god_attack = atoi( row[1] );
		else if ( id == 202 )
			global.hero_god_defense = atoi( row[1] );
		else if ( id == 203 )
			global.hero_god_level = atoi( row[1] );
		else if ( id == 204 )
			global.wishing_opencd = atoi( row[1] );
		else if ( id == 205 )
			global.wishing_silver_to_wood = value;
		else if ( id == 206 )
			global.wishing_silver_to_food = value;
		else if ( id == 207 )
			global.wishing_wood_to_silver = value;
		else if ( id == 208 )
			global.wishing_wood_to_food = value;
		else if ( id == 209 )
			global.wishing_food_to_silver = value;
		else if ( id == 210 )
			global.wishing_food_to_wood = value;
		else if ( id == 211 )
			global.wishing_green_to_draw = atoi( row[1] );
		else if ( id == 212 )
			global.wishing_change_cd = atoi( row[1] );
		else if ( id == 213 )
			global.wishing_change_cdmax = atoi( row[1] );
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
			if ( index == WORLD_DATA_NATION_OFFICIAL_STATE )
			{
				extern char g_nation_official_state;
				g_nation_official_state = g_world_data[index];
			}
			else if ( index == WORLD_DATA_NATION_OFFICIAL_STATETIME )
			{
				extern int g_nation_official_statetime;
				g_nation_official_statetime = g_world_data[index];
			}
		}
	}
	mysql_free_result( res );

	g_open_town3 = worldquest_check_server( WORLDQUEST_ID6 );
	g_open_town6 = worldquest_check_server( WORLDQUEST_ID9 );
	g_open_townking = worldquest_check_server( WORLDQUEST_WORLDBOSS2 );
	return 0;
}

// 从数据库获取全局数据
int world_data_get( int index, char *strvalue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int nValue = 0;

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

int world_data_getcache( int index )
{
	if ( index >= 0 && index < WORLD_DATA_MAX )
	{
		return g_world_data[index];
	}
	return -1;
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
