#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <limits.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "actor.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "system.h"
#include "award.h"
#include "item.h"
#include "global.h"
#include "mapunit.h"
#include "map.h"
#include "script.h"
#include "city.h"
#include "building.h"
#include "vip.h"
#include "quest.h"
#include "actor_times.h"
#include "world_quest.h"
#include "nation_hero.h"
#include "nation.h"
#include "activity.h"
#include "activity_04.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern Map g_map;
extern int g_world_data[WORLD_DATA_MAX];

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern BodyTokenInfo *g_bodytoken;
extern int g_bodytoken_maxnum;
extern int g_nation_actornum[3];

extern City *g_city;
extern int g_city_maxcount;

extern char g_game_day;
extern char g_game_weather;
extern char g_game_day_loop;

extern char g_open_zone_sili;
extern char g_open_zone_luoyang;
//-----------------------------------------------------------------------------
// actor_getoffline_userid
// 函数说明: 根据索引获取用户id
//-----------------------------------------------------------------------------
i64 actor_getoffline_userid( int actorid )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	i64 userid = -1;

	sprintf( szSQL, "select userid from actor_list where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		userid = atoll( row[0] );
	}
	mysql_free_result( res );
	return userid;
}

//-----------------------------------------------------------------------------
// actor_getindex_withid
// 函数说明: 根据角色ID返回角色索引
//-----------------------------------------------------------------------------
int actor_getindex_withid( int actorid )
{
	int tmpi;

	if ( actorid <= 0 )
		return -1;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid == actorid )
			return tmpi;
	}
	return -1;
}

// 那个国家人最少
static char _nation_award()
{
	char nation = 1;
	int num = 10000000;
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		if ( num > g_nation_actornum[tmpi] )
		{
			num = g_nation_actornum[tmpi];
			nation = tmpi + 1;
		}
	}
	return nation;
}

//-----------------------------------------------------------------------------
// actor_getlist
// 函数说明: 返回他的角色列表
//-----------------------------------------------------------------------------
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	char reconnect_flag = 0;
	int thistime = (int)time( NULL );
	int locktime;
	char szUserID[21];
	char num = 0;
	int index;
	pValue->m_nation_award = _nation_award();
	pValue->m_actor_num = 0;
	lltoa( userid, szUserID, 10 );
RE_QUERY:
	//sprintf( szSQL, "select actorid,name,nation,level,lock_time,delete_stoptime from actor_list where userid='%s' and platid='%d' and offset >= 0 ", szUserID, platid );
	sprintf( szSQL, "select actorid,name,nation,level,lock_time,delete_stoptime from actor_list where userid='%s' and offset >= 0 ", szUserID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_QUERY;
		}
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( (row = mysql_fetch_row( res )) )
	{
		index = 0;
		pValue->m_listinfo[pValue->m_actor_num].m_actorid = atoi( row[index++] );
		memcpy( pValue->m_listinfo[pValue->m_actor_num].m_name, row[index++], NAME_SIZE*sizeof(char) );
		pValue->m_listinfo[pValue->m_actor_num].m_nation = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_level = atoi( row[index++] );
		locktime = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime = atoi( row[index++] );
		if ( pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime > 0 && thistime >= pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime )
		{
			// 如果时间到了,则删除这个角色,并不发送
			if ( actor_delete( userid, pValue->m_listinfo[pValue->m_actor_num].m_actorid ) >= 0 ) // 删除这个角色
			{
				continue;
			}
		}
		if ( locktime == -1 || locktime > thistime )
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 1;
		else
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 0;
		pValue->m_listinfo[pValue->m_actor_num].m_lock_endtime = locktime;

		pValue->m_actor_num++;
		if ( pValue->m_actor_num >= 8 )
		{
			break;
		}
	}
	mysql_free_result( res );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_verifyid
// 函数说明: 验证一下这个角色id是不是属于登陆的这个用户
//-----------------------------------------------------------------------------
int actor_verifyid( i64 userid, int actorid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int verify = 0;
	char szUserID[21] = {0};
	lltoa( userid, szUserID, 10 );
	sprintf( szSQL, "select actorid from actor_list where actorid='%d' and userid='%s'", actorid, szUserID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		verify = 0;
	}
	else
	{
		verify = -1;
	}
	mysql_free_result( res );
	return verify;
}

// 角色钻石变化
int actor_change_token( int actor_index, int token, char path, int path_value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( g_actors[actor_index].token + token < 0 )
	{
		int value = 0;
		//actor_notify_value( actor_index, NOTIFY_WARNING, 1, &value, NULL ); // 钻石不够
		return -2;
	}
	g_actors[actor_index].token += token;
	if ( token > 0 && path == PATH_PAY )
	{ // 充值量
		int viplevel = 0;
		g_actors[actor_index].total_charge += token;

		// vip经验
		vip_exp( g_actors[actor_index].city_index, token, PATH_PAY );

		// 全服返利活动
		if ( activity_intime( ACTIVITY_6 ) )
		{
			City *pCity = city_getptr( actor_index );
			if ( pCity )
			{
				// 本国玩家充值
				nation_paytoken_add( pCity->nation, token );
			}
			// 全服充值
			int serv_paytoken = world_data_getcache( WORLD_DATA_ACTIVITY06_PAYTOKEN ) + token;
			world_data_set( WORLD_DATA_ACTIVITY06_PAYTOKEN, serv_paytoken, NULL, NULL );
		}
		// 七日狂欢
		activity_04_addvalue( g_actors[actor_index].city_index, ACTIVITY_SEVENDAY_TYPE1, token );
	}

	if ( token > 0 )
	{ // 总拥有
	}
	else if ( token < 0 )
	{ // 消耗量
		// 记录消耗
		wlog_token( 0, LOGOP_TOKEN, path, g_actors[actor_index].token, token, g_actors[actor_index].total_charge, g_actors[actor_index].actorid, path_value, g_actors[actor_index].userid );
	}

	// 发送变化
	SLK_NetS_Token Value = {};
	Value.m_total = g_actors[actor_index].token;
	Value.m_add = token;
	Value.m_path = path;
	netsend_changtoken_S( actor_index, SENDTYPE_ACTOR, &Value );

	// 记录log
	wlog( 0, LOGOP_TOKEN, path, token, g_actors[actor_index].token, g_actors[actor_index].total_charge, g_actors[actor_index].actorid, path_value );
	return 0;
}

// 修改名称
int actor_changename( int actor_index, char *pname, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( !pname )
		return -1;
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char szSQL[1024];

	int namelen = (int)strlen( pname );
	if ( namelen <= 0 || namelen >= NAME_SIZE )
		return -1;

	int costtype = 0;// 使用道具模式
	int itemkind = 172;
	if ( type == 1 )
	{ // 有道具使用道具，无道具使用钻石
		if ( item_getitemnum( actor_index, itemkind ) <= 0 )
		{
			if ( g_actors[actor_index].token < item_gettoken( itemkind ) )
			{			
				return -1;
			}
			costtype = 1; // 使用钻石模式
		}
	}

	char szText_newname[NAME_SIZE * 2 + 1] = {0};
	db_escape( (const char *)pname, szText_newname, 0 );
	sprintf( szSQL, "SELECT count(*) FROM actor_list WHERE name='%s'", szText_newname );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		if ( atoi( row[0] ) >= 1 )
		{
			actor_notify_pop( actor_index, 787 ); // 该名字已经有人使用
			mysql_free_result( res );
			return -1;
		}
	}
	mysql_free_result( res );

	strncpy( g_actors[actor_index].name, pname, NAME_SIZE );
	g_actors[actor_index].name[NAME_SIZE - 1] = 0;

	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	strncpy( pCity->name, pname, NAME_SIZE );
	pCity->name[NAME_SIZE - 1] = 0;

	sprintf( szSQL, "UPDATE actor_list SET name='%s' WHERE actorid=%d", szText_newname, g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	
	if ( type == 1 )
	{
		if ( costtype == 0 )
		{
			item_lost( actor_index, itemkind, 1, PATH_CHANGENAME );
		}
		else
		{
			actor_change_token( actor_index, -item_gettoken( itemkind ), PATH_CHANGENAME, 0 );
		}
	}
	SLK_NetS_ChangeName pValue = { 0 };
	strcpy( pValue.m_name, g_actors[actor_index].name );
	pValue.m_name_length = strlen( pValue.m_name );
	pValue.m_type = 0;
	netsend_changename_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 任务
	//quest_addvalue( pCity, QUEST_DATATYPE_CREATENAME, 0, 0, 1 );
	quest_talk_next( actor_index, g_actors[actor_index].quest_talkid );

	// 通知到城外
	mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	return 0;
}

// 修改名称
int actor_changename_gm( int actorid, char *pname )
{
	//if ( !pname )
	//	return -1;
	//MYSQL_RES * res;
	//MYSQL_ROW	row;
	//char szSQL[1024];

	//int namelen = (int)strlen( pname );
	//if ( namelen <= 0 || namelen >= NAME_SIZE )
	//	return -1;
	//char szText_newname[NAME_SIZE * 2 + 1] = {0};
	//db_escape( (const char *)pname, szText_newname, 0 );
	//sprintf( szSQL, "SELECT count(*) FROM actor_list WHERE name='%s'", szText_newname );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//res = mysql_store_result( myGame );
	//if ( row = mysql_fetch_row( res ) )
	//{
	//	if ( atoi( row[0] ) >= 1 )
	//	{
	//		// 该名字已经有人使用
	//		mysql_free_result( res );
	//		return -1;
	//	}
	//}
	//mysql_free_result( res );

	//City *pCity = city_getptr_withactorid( actorid );
	//if ( !pCity )
	//	return -1;

	//int actor_index = actor_getindex_withid( actorid );
	//if ( actor_index >= 0 && actor_index < g_maxactornum )
	//{
	//	strncpy( g_actors[actor_index].name, pname, NAME_SIZE );
	//	g_actors[actor_index].name[NAME_SIZE - 1] = 0;
	//	actor_getinfo( actor_index );
	//}

	//strncpy( pCity->laird_name, pname, NAME_SIZE );
	//pCity->laird_name[NAME_SIZE - 1] = 0;

	//sprintf( szSQL, "UPDATE actor_list SET name='%s' WHERE actorid=%d", szText_newname, actorid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//sprintf( szSQL, "UPDATE club_asklist SET name='%s' WHERE cityid=%d", szText_newname, pCity->cityid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}

	//Club *pClub = club_getptr_withcity( pCity );
	//if ( pClub )
	//{
	//	if ( pClub->m_leaderid == pCity->laird_actorid )
	//	{
	//		strncpy( pClub->m_leadername, pname, NAME_SIZE );
	//		pClub->m_leadername[NAME_SIZE - 1] = 0;
	//	}

	//}

	//// 通知到城外
	//mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	return 0;
}

// 修改侍女名称
int actor_maid_changename( int actor_index, char *pname, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( !pname )
		return -1;
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char szSQL[1024];

	int namelen = (int)strlen( pname );
	if ( namelen <= 0 || namelen >= NAME_SIZE )
		return -1;

	strncpy( g_actors[actor_index].maidname, pname, NAME_SIZE );
	g_actors[actor_index].maidname[NAME_SIZE - 1] = 0;

	SLK_NetS_ChangeName pValue = { 0 };
	strcpy( pValue.m_name, g_actors[actor_index].maidname );
	pValue.m_name_length = strlen( pValue.m_name );
	pValue.m_type = 1;
	netsend_changename_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 任务
	quest_talk_next( actor_index, g_actors[actor_index].quest_talkid );
	return 0;
}

// 修改头像
int actor_changeshape( int actor_index, char shape )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( shape < 0 || shape >= SHAPE_MAX )
		return -1;
	if ( (g_actors[actor_index].shape_bag & (1 << shape)) == 0 )
	{
		actor_notify_pop( actor_index, 2424 );
		return -1;
	}
	
	g_actors[actor_index].shape = shape;
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		pCity->shape = g_actors[actor_index].shape;

		// 发更新
		int value[2] = { 0 };
		value[0] = 0;
		value[1] = shape;
		actor_notify_value( actor_index, NOTIFY_CHANGESHAPE, 2, value, NULL );
	}
	return 0;
}

// 头像获得
void actor_change_open( int actor_index, char shape )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( shape < 0 || shape >= SHAPE_MAX )
		return;

	g_actors[actor_index].shape_bag |= (1 << shape);
	
	// 发更新
	int value[2] = { 0 };
	value[0] = 1;
	value[1] = g_actors[actor_index].shape_bag;
	actor_notify_value( actor_index, NOTIFY_CHANGESHAPE, 2, value, NULL );
}

// 修改签名
int actor_changsign( int actor_index, char *sign )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( !sign )
		return -1;
	int signlen = (int)strlen( sign );
	if ( signlen <= 0 || signlen >= SIGNATURE_SIZE )
		return -1;

	strncpy( pCity->signature, sign, SIGNATURE_SIZE );
	pCity->signature[SIGNATURE_SIZE - 1] = 0;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_getinfo
// 函数说明: 角色信息
//-----------------------------------------------------------------------------
int actor_getinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_ActorInfo info = { 0 };
	info.m_actorid = g_actors[actor_index].actorid;
	strncpy( info.m_name, g_actors[actor_index].name, sizeof( char )*NAME_SIZE );
	strncpy( info.m_maidname, g_actors[actor_index].maidname, sizeof( char )*NAME_SIZE );
	info.m_maidname_len = strlen( info.m_maidname );
	info.m_nation = g_actors[actor_index].nation;
	info.m_shape = g_actors[actor_index].shape;
	info.m_level = g_actors[actor_index].level;
	info.m_token = g_actors[actor_index].token;
	info.m_actor_sflag = g_actors[actor_index].sflag;
	info.m_storyid = g_actors[actor_index].storyid;
	info.m_shape_bag = g_actors[actor_index].shape_bag;
	info.m_game_day = g_game_day;
	info.m_game_weather = g_game_weather;
	info.m_game_day_loop = g_game_day_loop;
	info.m_open_zone_sili = g_open_zone_sili;
	info.m_open_zone_luoyang = g_open_zone_luoyang;
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		info.m_state = pCity->state;
		info.m_posx = pCity->posx;
		info.m_posy = pCity->posy;
		info.m_exp = pCity->exp;
		info.m_exp_max = g_upgradeinfo[pCity->level].exp;
		info.m_viplevel =  pCity->viplevel;
		info.m_vipexp =  pCity->vipexp;
		info.m_vipexp_max = vip_expmax_get( pCity->viplevel );
		info.m_body =  pCity->body;
		info.m_bodysec = pCity->bodysec;
		info.m_place =  pCity->place;
		info.m_official =  pCity->official;
		info.m_zone =  pCity->zone;
		info.m_battlepower =  pCity->battlepower;
		info.m_silver =  pCity->silver;
		info.m_wood =  pCity->wood;
		info.m_food =  pCity->food;
		info.m_iron =  pCity->iron;
		info.m_infantry_num = pCity->soldiers[0];
		info.m_cavalry_num = pCity->soldiers[1];
		info.m_archer_num = pCity->soldiers[2];
		info.m_mokilllv =  pCity->mokilllv;
		info.m_sflag =  pCity->sflag;
		info.m_autobuild = pCity->autobuild;
		info.m_autobuildopen = pCity->autobuildopen;
		info.m_autoguard = pCity->atgu;
		info.m_autoguardopen = pCity->atgu_op;
		info.m_ptsec = pCity->ptsec;
		info.m_forgingkind = pCity->forgingkind;
		info.m_forgingsec = pCity->forgingsec;
		info.m_guardnum = pCity->guardnum;
		info.m_guardsec = pCity->guardsec;
		info.m_questid = pCity->questid[0];
		for ( int i = 0; i < CITY_TECH_MAX; i++ )
		{
			info.m_techlevel[i] = pCity->techlevel[i];
			info.m_techprogress[i] = pCity->techprogress[i];
		}
		for ( int i = 0; i < 3; i++ )
		{
			info.m_officialhire[i].m_ofkind = pCity->ofkind[i];
			info.m_officialhire[i].m_ofsec = pCity->ofsec[i];
			info.m_officialhire[i].m_offree = pCity->offree[i];
			info.m_officialhire[i].m_ofquick = pCity->ofquick[i];
		}
		for ( int i = 0; i < 6; i++ )
		{
			if ( pCity->buffsec[i] > 0 )
			{
				info.m_buff_endtime[i] = (int)time( NULL ) + pCity->buffsec[i];
			}
		}
		info.m_bufftrain = pCity->bufftrain;
	}
	netsend_actorinfo_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}

// 角色配置
int actor_configinfo( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	//SLK_NetS_ActorConfig info = { 0 };
	//for ( int tmpi = 0; tmpi < 8; tmpi++ )
	//{
	//	info.m_config[tmpi] = g_actors[actor_index].config[tmpi];
	//}
	//info.m_sysopenlist[0] = g_Config.showflag;
	//info.m_sysopenlist[1] = g_Config.showcdkey;
	//info.m_sysopenlist[2] = g_Config.showvip;
	//info.m_sysopenlist[3] = g_Config.showmcard;
	//info.m_syscount = 4;
	////info.m_remote_push = city_get_sflag( city_getptr( actor_index ), CITY_SFLAG_CLOSEPUSH ); // 0开启 1关闭
	//netsend_actorconfig_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}
int actor_setconfig( int actor_index, int index, char value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	g_actors[actor_index].config[index] = value;
	return 0;
}
int actor_getconfig( int actor_index, int index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	return g_actors[actor_index].config[index];
}

// 购买体力
int actor_buybody( int actor_index, int ask )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_BODY_FREEBUY ) == 0 )
	{
		actor_set_sflag( actor_index, ACTOR_SFLAG_BODY_FREEBUY, 1 );
		city_changebody( pCity->index, 100, PATH_BODYBUY );
		return 0;
	}
	// 最大购买次数
	int maxtimes = pCity->attr.everyday_body_buymax + vip_attr_bodybuy( pCity );
	// 今天购买体力次数
	int times  = actor_get_today_char_times( actor_index, TODAY_CHAR_BUYBODY );
	if ( times >= maxtimes )
	{
		int value[4] = { 0 };
		value[0] = 1;
		value[1] = 0;
		value[2] = times;
		value[3] = maxtimes;
		actor_notify_value( actor_index, NOTIFY_ACTOR, 4, value, NULL );
		return 0;
	}

	int _times = times + 1;
	if ( _times <= 0 )
		_times = 1;
	else if ( _times >= g_bodytoken_maxnum )
		_times = g_bodytoken_maxnum - 1;

	int token = g_bodytoken[_times].token;
	if ( ask == 1 )
	{	
		int value[4] = { 0 };
		value[0] = 0;
		value[1] = token;
		value[2] = times;
		value[3] = maxtimes;
		actor_notify_value( actor_index, NOTIFY_ACTOR, 4, value, NULL );
		return 0;
	}
	
	if ( actor_change_token( actor_index, -token, PATH_BODYBUY, 0 ) < 0 )
		return -1;
	city_changebody( pCity->index, 100, PATH_BODYBUY );
	actor_add_today_char_times( actor_index, TODAY_CHAR_BUYBODY );
	return 0;
}

// 查询玩家信息
int actor_search( int actor_index, int target_actorid, int target_city_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( target_city_index >= 0 && target_city_index < g_city_maxcount )
	{
		if ( target_actorid != g_city[target_city_index].actorid )
		{
			target_city_index = city_getindex_withactorid( target_actorid );
		}
	}
	else
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
		return -1;

	SLK_NetS_ActorSearch pValue = { 0 };
	pValue.m_actorid = target_actorid;
	pValue.m_city_index = target_city_index;
	strncpy( pValue.m_name, g_city[target_city_index].name, NAME_SIZE );
	pValue.m_namelen = strlen( pValue.m_name );
	strncpy( pValue.m_sign, g_city[target_city_index].signature, SIGNATURE_SIZE );
	pValue.m_signlen = strlen( pValue.m_sign );
	pValue.m_shape = g_city[target_city_index].shape;
	pValue.m_level = g_city[target_city_index].level;
	pValue.m_official = g_city[target_city_index].official;
	pValue.m_place = g_city[target_city_index].place;
	pValue.m_zone = g_city[target_city_index].zone;
	pValue.m_nation = g_city[target_city_index].nation;
	pValue.m_battlepower = g_city[target_city_index].battlepower;
	pValue.m_killcity = data_record_getvalue( &g_city[target_city_index], DATA_RECORD_KILLCITY );
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		short kind = g_city[target_city_index].hero[tmpi].kind;
		if ( kind > 0 )
		{
			pValue.m_hero[pValue.m_herocount].m_kind = kind;
			pValue.m_hero[pValue.m_herocount].m_color = g_city[target_city_index].hero[tmpi].color;
			pValue.m_hero[pValue.m_herocount].m_level = g_city[target_city_index].hero[tmpi].level;

			NationHeroInfo *config = nation_hero_getconfig( kind );
			if ( config )
			{ // 是名将
				if ( config->herokind > 0 && config->offset >= 0 && config->offset < NATIONHERO_MAX )
				{
					if ( g_city[target_city_index].nation_hero[config->offset].forever == 0 && g_city[target_city_index].hero[tmpi].god == 1 )
						pValue.m_hero[pValue.m_herocount].m_type = 3;
					else if ( g_city[target_city_index].nation_hero[config->offset].forever == 0 )
						pValue.m_hero[pValue.m_herocount].m_type = 1;
					else
						pValue.m_hero[pValue.m_herocount].m_type = 2;
				}
			}
			pValue.m_herocount += 1;
		}
	}
	
	pValue.m_bp_hero = g_city[target_city_index].battlepower_hero;
	pValue.m_bp_equip = g_city[target_city_index].battlepower_equip;
	pValue.m_bp_tech = g_city[target_city_index].battlepower_tech;
	pValue.m_bp_nequip = g_city[target_city_index].battlepower_nequip;
	pValue.m_bp_place = g_city[target_city_index].battlepower_place;
	pValue.m_bp_girl = g_city[target_city_index].battlepower_girl;
	pValue.m_my_bp_hero = pCity->battlepower_hero;
	pValue.m_my_bp_equip = pCity->battlepower_equip;
	pValue.m_my_bp_tech = pCity->battlepower_tech;
	pValue.m_my_bp_nequip = pCity->battlepower_nequip;
	pValue.m_my_bp_place = pCity->battlepower_place;
	pValue.m_my_bp_girl = pCity->battlepower_girl;
	netsend_actorsearch_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 功能获取
void city_function_open( City *pCity, int offset );
void city_function_close( City *pCity, int offset );
int city_function_check( City *pCity, int offset );