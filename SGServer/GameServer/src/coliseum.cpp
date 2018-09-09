#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "global.h"
#include "city.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_times.h"
#include "award.h"
#include "rank.h"
#include "nation.h"
#include "hero.h"
#include "system.h"
#include "fight.h"
#include "server_netsend_auto.h"
#include "coliseum.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;
extern Fight g_fight;

extern LastName *g_lastname;
extern int g_lastname_maxnum;

extern FirstName *g_firstname;
extern int g_firstname_maxnum;

int g_maxrank = 1;
SLK_NetS_ColiseumRankList g_ColiseumRankList = { 0 };

int coliseum_init()
{
	// A.获取最大的名次
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	sprintf( szSQL, "select max(rank) from match_list" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		return -1;
	}
	if ( row[0] )
		g_maxrank = atoi( row[0] ) + 1;
	else
		g_maxrank = 1;
	mysql_free_result( res );

	// 初始化排行榜
	coliseum_load_ranklist();
	return 0;
}

// 进入竞技场
int coliseum_enter( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	_check_fday( actor_index );
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_ENTER ) > 0 )
	{//	出入竞技场太过频繁了
		actor_notify_pop( actor_index, 4301 );
		return -1;
	}

	// 获取自己最新排名
	g_actors[actor_index].coliseum_rank = coliseum_getrank( g_actors[actor_index].actorid );
	// 保存自己的存档
	coliseum_save( actor_index );
	// 检查排序
	coliseum_checkrank();
	// 匹配对手
	if ( g_actors[actor_index].coliseum_list[0].m_actorid <= 0 )
	{
		coliseum_match( actor_index );
	}
	coliseum_sendlist( actor_index );

	// 频繁限制
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_ENTER, 3 );
	return 0;
}

// 发送列表
int coliseum_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_ColiseumList pValue = { 0 };
	pValue.m_updatecd = actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE );
	pValue.m_todaynum = actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );
	pValue.m_maxtodaynum = global.coliseum_todaynum;
	pValue.m_maxrank = g_actors[actor_index].coliseum_maxrank;
	pValue.m_myrank = g_actors[actor_index].coliseum_rank;
	memcpy( pValue.m_list, g_actors[actor_index].coliseum_list, sizeof( SLK_NetS_ColiseumCity ) * 3 );
	pValue.m_count = COLISEUMLIST_MAX;
	pValue.m_myteam[0] = g_actors[actor_index].coliseum_team[0];
	pValue.m_myteam[1] = g_actors[actor_index].coliseum_team[1];
	pValue.m_myteam[2] = g_actors[actor_index].coliseum_team[2];
	netsend_coliseumlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获得排名
int coliseum_getrank( int actorid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	int rank = -1;

	sprintf( szSQL, "select rank from match_list where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		rank = g_maxrank++;
		return rank;
	}
	if ( row[0] )
		rank = atoi( row[0] );
	mysql_free_result( res );

	if ( rank < 0 )
	{
		rank = g_maxrank++;
	}
	return rank;
}

// 设置排名
int coliseum_setrank( int actorid, int rank )
{
	char szSQL[2048];

	sprintf( szSQL, "update match_list set rank='%d' where actorid = '%d'", rank, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 检查排序是否不正确，一段时间调用一次吧
void coliseum_checkrank()
{
	char szSQL[2048];
	sprintf( szSQL, "set @rownum=0" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return;
	}
	sprintf( szSQL, "update match_list set rank=(@rownum:=@rownum+1) order by rank" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return;
	}
}

// 保存自己的竞技场信息
int coliseum_save( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// 自己队伍，默认上阵武将
	char empty = 1;
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].coliseum_team[tmpi] > 0 )
		{
			empty = 0;
			break;
		}
	}
	if ( empty == 1 )
	{
		for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
		{
			g_actors[actor_index].coliseum_team[tmpi] = pCity->hero[tmpi].kind;
		}
	}

	// 基本信息存到列表
	coliseum_list_save( actor_index, "match_list" );
	return 0;
}

// 保存列表
int coliseum_list_save( int actor_index, char *pTab )
{
	char szSQL[2048];
	char tmpstr[256];
	int member = 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int totalpower = 0;
	// 获取当前阵容最新数据
	Hero tempHero[COLISEUMLIST_HERO_MAX] = { 0 };
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		int herokind = g_actors[actor_index].coliseum_team[tmpi];
		Hero *pHero = hero_getptr( actor_index, herokind );
		if ( !pHero )
			continue;
		memcpy( &tempHero[tmpi], pHero, sizeof( Hero ) );
		tempHero->offset = HERO_COLISEUMOFFSET + tmpi;
		hero_attr_calc( pCity, &tempHero[tmpi] );
		tempHero[tmpi].colorup = hero_getline( pCity, HERO_STATE_FIGHT ); // 特殊处理
		totalpower += tempHero[tmpi].bp_hero + tempHero[tmpi].bp_equip + tempHero[tmpi].bp_tech + tempHero[tmpi].bp_nequip + tempHero[tmpi].bp_girl;
	}

	// 主角
	char szText_name[64] = { 0 };
	sprintf( szSQL, "REPLACE INTO %s VALUES( '%d','%d','%s','%d','%d','%d','%d'", pTab, 
		g_actors[actor_index].actorid,
		g_actors[actor_index].coliseum_rank,
		db_escape( (const char *)g_actors[actor_index].name, szText_name, 0 ),
		strlen(g_actors[actor_index].name),
		g_actors[actor_index].nation,
		g_actors[actor_index].level,
		totalpower );

	// 英雄基本信息
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( tempHero[member].kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", tempHero[member].kind,tempHero[member].color,tempHero[member].level );
		strcat( szSQL, tmpstr );
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	strcat( szSQL, ");" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 属性信息
	coliseum_match_attr_delete( g_actors[actor_index].actorid, "match_attr" );
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( tempHero[tmpi].kind > 0 )
		{
			coliseum_match_attr_save( g_actors[actor_index].actorid, tmpi, &tempHero[tmpi], "match_attr" );
		}
	}
	return 0;
}

// 加载列表
int coliseum_list_load( int actor_index, char *pTab )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int index;
	memset( g_actors[actor_index].coliseum_list, 0, sizeof( SLK_NetS_ColiseumCity ) * COLISEUMLIST_MAX );
	// 随机匹配相近
	if ( coliseum_random( actor_index ) < 0 )
		return -1;
	for ( int tmpi = 0; tmpi < COLISEUMLIST_MAX; tmpi++ )
	{
		sprintf( szSQL, "SELECT actorid,rank,name,namelen,nation,level,bpower,herokind0,herocolor0,herolevel0,herokind1,herocolor1,herolevel1,herokind2,herocolor2,herolevel2 from %s where rank='%d'", pTab, g_actors[actor_index].coliseum_list[tmpi].m_rank );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			return -1;
		}
		res = mysql_store_result( myGame );

		if ( !((row = mysql_fetch_row( res ))) )
		{
			mysql_free_result( res );
			continue;
		}
		index = 0;
		// 主角基本信息
		g_actors[actor_index].coliseum_list[tmpi].m_actorid = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_rank = atoi( row[index++] );
		strcpy( g_actors[actor_index].coliseum_list[tmpi].m_name, row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_namelen = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_nation = atoi( row[index++] );
		short level = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_bpower = atoi( row[index++] );
		// 英雄信息
		for ( int i = 0; i < COLISEUMLIST_HERO_MAX; i++ )
		{
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_kind = atoi( row[index++] );
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_level = atoi( row[index++] );
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_color = atoi( row[index++] );
		}
		mysql_free_result( res );
	}
	return 0;
}

// 玩家属性数据库删除
int coliseum_match_attr_delete( int actorid, char *pTab )
{
	char	szSQL[2048] = { 0 };
	char reconnect_flag = 0;
	char sz64_partnerid[21] = { 0 };
RE_DELETE_DELETE:
	sprintf( szSQL, "DELETE FROM %s WHERE `actorid`='%d'", pTab, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_DELETE_DELETE;
		}
		return -1;
	}
	return 0;
}

// 英雄信息存档
int coliseum_match_attr_save( int actorid, int offset, Hero *pHero, char *pTab )
{
	char szSQL[512] = { 0 };
	char reconnect_flag = 0;
	if ( pHero == NULL || pTab == NULL )
		return -1;
RE_ACTORATTR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (actorid,offset,kind,color,level,attack,defense,troops,attack_increase,defense_increase,line) "
		"Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')", pTab, actorid, offset,
		pHero->kind, pHero->color, pHero->level, pHero->attack, pHero->defense, pHero->troops, pHero->attack_increase, pHero->defense_increase, pHero->colorup );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_ACTORATTR_UPDATE;
		}
		return -1;
	}
	return 0;
}

// 读取英雄属性
int coliseum_match_attr_load( int actorid, Hero *pHero, int count, char *pTab )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int index;

	sprintf( szSQL, "SELECT offset,kind,color,level,attack,defense,troops,attack_increase,defense_increase,line from %s where actorid='%d'", pTab, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ))
	{
		int offset = atoi( row[0] );
		if ( offset < 0 || offset >= count )
			break;
		index = 1;
		pHero[offset].offset = HERO_COLISEUMOFFSET + offset;
		pHero[offset].kind = atoi( row[index++] );
		pHero[offset].color = atoi( row[index++] );
		pHero[offset].level = atoi( row[index++] );
		pHero[offset].attack = atoi( row[index++] );
		pHero[offset].defense = atoi( row[index++] );
		pHero[offset].troops = atoi( row[index++] );
		pHero[offset].attack_increase = atoi( row[index++] );
		pHero[offset].defense_increase = atoi( row[index++] );
		pHero[offset].colorup = atoi( row[index++] );
	}
	mysql_free_result( res );
	return 0;
}

// 随机对手
int coliseum_random( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int rank = g_actors[actor_index].coliseum_rank;

	/* 前10名特殊排名的固定处理 */
	if ( rank == 1 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 2;
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 2 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 3 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 2;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 4 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 2;
		g_actors[actor_index].coliseum_list[2].m_rank = 3;
	}
	else if ( rank == 5 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 6 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[2].m_rank = 5;
	}
	else if ( rank == 7 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 5, 6 );
	}
	else if ( rank == 8 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 2, 3 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 4, 5 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 6, 7 );
	}
	else if ( rank == 9 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 5, 6 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 7, 8 );
	}
	else
	{
		g_actors[actor_index].coliseum_list[0].m_rank = (int)(random( rank * 3, rank * 5 ) / 10);
		g_actors[actor_index].coliseum_list[1].m_rank = (int)(random( rank * 6, rank * 8 ) / 10);
		g_actors[actor_index].coliseum_list[2].m_rank = (int)(random( rank * 9, rank * 10 ) / 10);
		if ( g_actors[actor_index].coliseum_list[2].m_rank == g_actors[actor_index].coliseum_rank )
			g_actors[actor_index].coliseum_list[2].m_rank -= 1;
	}
	return 0;
}

int coliseum_match( int actor_index )
{
	// 获取相近排名玩家数据
	coliseum_list_load( actor_index, "match_list" );
	return 0;
}

// 换一批
int coliseum_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE ) > 0 )
	{ // 请等待冷却结束
		actor_notify_alert( actor_index, 4300 );
		return -1;
	}
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE, 30 );

	// 重新匹配
	coliseum_match( actor_index );

	coliseum_sendlist( actor_index );
	return 0;
}

// 替换阵容
int coliseum_replaceteam( int actor_index, int kind0, int kind1, int kind2 )
{
	ACTOR_CHECK_INDEX( actor_index );
	return 0;
}

int coliseum_fight( int actor_index, int index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM ) >= global.coliseum_todaynum )
	{
		return -1;
	}
	if ( index < 0 || index >= COLISEUMLIST_MAX )
	{
		return -1;
	}

	// 我的属性
	Hero pAttack[COLISEUMLIST_HERO_MAX] = {0};
	coliseum_match_attr_load( g_actors[actor_index].actorid, pAttack, COLISEUMLIST_HERO_MAX, "match_attr" );

	Hero pDefense[COLISEUMLIST_HERO_MAX] = { 0 };
	coliseum_match_attr_load( g_actors[actor_index].coliseum_list[index].m_actorid, pDefense, COLISEUMLIST_HERO_MAX, "match_attr" );

	if ( fight_start_bycoliseum( actor_index, pAttack, pDefense, COLISEUMLIST_HERO_MAX ) < 0 )
		return -1;

	if ( g_fight.result == FIGHT_WIN )
	{
		// 战斗胜利，要给自己加连胜场次
		coliseum_serieswin_set( actor_index, 1 );
	}
	else
	{
		// 战斗失败，要终结自己的连胜
		coliseum_serieswin_set( actor_index, 0 );

	}
	g_actors[actor_index].coliseum_rank = coliseum_getrank( g_actors[actor_index].actorid );
	g_actors[actor_index].coliseum_list[index].m_rank = coliseum_getrank( g_actors[actor_index].coliseum_list[index].m_actorid );

	// 交换排名以及记录消息
	coliseum_change_rank( actor_index, index, g_fight.result );

	actor_add_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );
	return 0;
}

// 交换排名
int coliseum_change_rank( int actor_index, int index, char winflag )
{
	ACTOR_CHECK_INDEX( actor_index );
	int winner1 = 1;
	int winner2 = 0;
	int rank_change1 = 0;
	int rank_change2 = 0;
	int rank_transform = 0;
	int fight_index = -1;
	bool topchange = false;

	if ( winflag == FIGHT_WIN )
	{
		winner1 = 0;
		winner2 = 1;
		rank_change1 = g_actors[actor_index].coliseum_list[index].m_rank;
		rank_change2 = g_actors[actor_index].coliseum_rank;
		if ( rank_change1 >= rank_change2 )
		{
			// 对手排名如果低于自己(可能发生在其他人的战斗影响了排名)，就不更改排名了
			rank_change1 = 0;
			rank_change2 = 0;
		}
		else
		{
			/* 第一次打 */
			if ( g_actors[actor_index].coliseum_maxrank <= 0 )
			{
				g_actors[actor_index].coliseum_maxrank = g_actors[actor_index].coliseum_rank;
			}

			rank_transform = rank_change2 - rank_change1;
			g_actors[actor_index].coliseum_rank = rank_change1;
			g_actors[actor_index].coliseum_list[index].m_rank = rank_change2;

			coliseum_setrank( g_actors[actor_index].actorid, g_actors[actor_index].coliseum_rank );
			coliseum_setrank( g_actors[actor_index].coliseum_list[index].m_actorid, g_actors[actor_index].coliseum_list[index].m_rank );
			if ( g_actors[actor_index].coliseum_rank <= 5 )
			{
				/*char szMsg[MAX_PATH];
				sprintf( szMsg, "&r&g%s&r击败了&r&g%s&r，成功夺得&y第%d名&r的头衔！现已加入“精彩对决”！", g_actors[actor_index].pattr[0]->name,
					g_actors[actor_index].coliseum_list[offset].m_name, g_actors[actor_index].rank );
				system_talk( szMsg, 3 );*/
			}

			if ( g_actors[actor_index].coliseum_rank <= 10 )
			{
				/* // 如果是上升至前十名的战斗则多保存一则不过期录像
				int recordid = fight_record_savedb( g_actors[actor_index].fight_index, 1 );
				if ( recordid < 0 )
					return -1;
				coliseum_wonderfight_insert( recordid, g_actors[actor_index].pattr[0]->name, g_actors[actor_index].pattr[0]->level, rank_change2,
					g_actors[actor_index].coliseum_list[offset].m_name, g_actors[actor_index].coliseum_list[offset].m_level, rank_change1 );*/
			}

			/* 检查玩家历史最高排名 */
			topchange = coliseum_check_Historyrank( actor_index ) == 1;
		}
	}

	/* 把录像记录下来 */
	//fight_record_savedb( fight_index, 0 );

	/*if ( coliseum_battlelog( 0, g_actors[actor_index].actorid, g_actors[actor_index].coliseum_list[offset].m_actorid, g_actors[actor_index].coliseum_list[offset].m_shape,
		g_actors[actor_index].coliseum_list[offset].m_name, g_actors[actor_index].coliseum_list[offset].m_level, winner1, rank_transform, fight_index, topchange ) < 0 )
		return -1;
	coliseum_award( actor_index, winflag );

	if ( coliseum_battlelog( 1, g_actors[actor_index].coliseum_list[offset].m_actorid, g_actors[actor_index].actorid, g_actors[actor_index].pattr[0]->shape,
		g_actors[actor_index].pattr[0]->name, g_actors[actor_index].pattr[0]->level, winner2, rank_transform, fight_index, topchange ) < 0 )
		return -1;*/

	if ( winflag == FIGHT_WIN )
	{
		coliseum_match( actor_index );
	}
	return 0;
}

// 历史最高排名检查
int coliseum_check_Historyrank( int actor_index )
{
	double fcurvalue;
	double fhistoryvalue;
	int token = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	/* 检查名次 */
	if ( g_actors[actor_index].coliseum_maxrank > g_actors[actor_index].coliseum_rank )
	{
		/* 名次都<=1000 */
		if ( g_actors[actor_index].coliseum_maxrank <= 1000 && g_actors[actor_index].coliseum_rank <= 1000 )
		{
			fcurvalue = 10 - (g_actors[actor_index].coliseum_rank - 1)*0.01;
			fhistoryvalue = 10 - (g_actors[actor_index].coliseum_maxrank - 1)*0.01;
			token = int( ((fcurvalue + fhistoryvalue)*(g_actors[actor_index].coliseum_maxrank - g_actors[actor_index].coliseum_rank) + 1) / 2 );
			token /= 10;
		}
		/* 名次都>1000 */
		else if ( g_actors[actor_index].coliseum_maxrank > 1000 && g_actors[actor_index].coliseum_rank > 1000 )
		{
			token = int( (g_actors[actor_index].coliseum_maxrank - g_actors[actor_index].coliseum_rank)*0.03 );
			token /= 10;
		}
		/* 只有上次名次>1000 */
		else if ( g_actors[actor_index].coliseum_maxrank > 1000 )
		{
			fcurvalue = 10 - (g_actors[actor_index].coliseum_rank - 1)*0.01;
			token = (int)(((fcurvalue + 0.01)*(1000 - g_actors[actor_index].coliseum_rank) + 1) / 2 + (g_actors[actor_index].coliseum_maxrank - 1000)*0.03);
			token /= 10;
		}

		if ( token <= 0 )
			token = 1;
		/* 发送奖励邮件 */
		/*{
			char szMsg[MAX_PATH];
			sprintf( szMsg, "恭喜您本次排名&g(%d)&w超过您的历史最高排名&g(%d)&w，为了表彰您在斗法台中的突出表现，以及超越自我，特送奖励如下：|-13|%d|",
				g_actors[actor_index].rank, g_actors[actor_index].coliseum_maxrank, token );
			mail_send( -158, g_actors[actor_index].actorid, szMsg, 2, 0, 0, 5, 0, 0 );
		}*/
		/* 设置最新最高排名 */
		g_actors[actor_index].coliseum_maxrank = g_actors[actor_index].coliseum_rank;
		return 1;
	}
	return 0;
}

// 读取排行榜列表
int coliseum_load_ranklist()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int index = 0;
	sprintf( szSQL, "select actorid,rank,name,namelen,nation,level,bpower,herokind0,herocolor0,herolevel0,herokind1,herocolor1,herolevel1,herokind2,herocolor2,herolevel2 from match_list where rank <= 30 and rank > 0" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( row = mysql_fetch_row( res ) )
	{
		if ( row[0] == NULL || row[1] == NULL )
			continue;
		int rank = atoi( row[1] ) - 1;
		if ( rank < 0 || rank >= 30 )
			continue;
		index = 0;
		// 主角基本信息
		int actorid = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_rank = atoi( row[index++] );
		strcpy( g_ColiseumRankList.m_list[rank].m_name, row[index++] );
		g_ColiseumRankList.m_list[rank].m_namelen = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_nation = atoi( row[index++] );
		short level = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_bpower = atoi( row[index++] );
		g_ColiseumRankList.m_count += 1;
	}
	mysql_free_result( res );
	return 0;
}

// 排行榜列表
int coliseum_ranklist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	netsend_coliseumranklist_S( actor_index, SENDTYPE_ACTOR, &g_ColiseumRankList );
	return 0;
}

// 连胜
int coliseum_serieswin_set( int actor_index, char flag )
{
	ACTOR_CHECK_INDEX( actor_index );
	//if ( flag == 1 )
	//{// 增加自己的连胜场次
	//	actor_add_today_times( actor_index, TODAY_INDEX_COLISEUMWIN );
	//}
	//else
	//{
	//	actor_set_today_times( actor_index, TODAY_INDEX_COLISEUMWIN, 0 );
	//}
	return 0;
}

// ---------------------------------------------机器人---------------------------------------------------------
int coliseum_robot_checkinit()
{
	return coliseum_robot_init();
}
/* 竞技场机器人初始化 */
int coliseum_robot_init()
{
	/* 先检查竞技场列表中是否有数据 */
	if ( coliseum_robot_check() > 0 )
		return -1;

	/* 1.先清空一次，以免有垃圾数据 */
	coliseum_robot_delete( "match_list" );
	coliseum_robot_delete( "match_attr" );

	/* 2.开始创建 */
	printf( "coliseum robot create begin \n" );
	coliseum_robot_create( COLISEUM_ROBOT_COUNT );
	printf( "coliseum robot create end \n" );
	return 0;
}

/* 检查竞技场列表里是否有数据 */
int coliseum_robot_check()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int matchcount = 0;
	sprintf( szSQL, "select count(*) from match_list;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		if ( row[0] )
			matchcount = atoi( row[0] );
	}
	else
	{
		mysql_free_result( res );
		return 0;
	}
	mysql_free_result( res );
	return matchcount;
}

/* 清空竞技场列表里的所有数据 */
int coliseum_robot_delete( char *szTable )
{
	char szSQL[256];
	sprintf( szSQL, "TRUNCATE `%s`", szTable );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	return 0;
}

/* 竞技场角色列表创建 */
int coliseum_robot_match_list_insert( Actor *pActor, City *pCity, Hero *pHero, int count, char *pTab )
{
	char szSQL[2048];
	char tmpstr[256];
	int member = 0;
	int totalpower = 0;

	// 获取当前阵容最新数据
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		int herokind = pActor->coliseum_team[tmpi];
		hero_attr_calc( pCity, &pHero[tmpi] );
		pHero[tmpi].colorup = hero_getline( pCity, HERO_STATE_FIGHT ); // 特殊处理
		totalpower += pHero[tmpi].bp_hero + pHero[tmpi].bp_equip + pHero[tmpi].bp_tech + pHero[tmpi].bp_nequip + pHero[tmpi].bp_girl;
	}

	// 主角
	char szText_name[64] = { 0 };
	sprintf( szSQL, "REPLACE INTO %s VALUES( '%d','%d','%s','%d','%d','%d','%d'", pTab,
		pActor->actorid,
		pActor->coliseum_rank,
		db_escape( (const char *)pActor->name, szText_name, 0 ),
		strlen( pActor->name ),
		pActor->nation,
		pActor->level,
		totalpower );

	// 英雄基本信息
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( pHero[member].kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", pHero[member].kind, pHero[member].color, pHero[member].level );
		strcat( szSQL, tmpstr );
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	strcat( szSQL, ");" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 属性信息
	coliseum_match_attr_delete( pActor->actorid, "match_attr" );
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( pHero[tmpi].kind > 0 )
		{
			coliseum_match_attr_save( pActor->actorid, tmpi, &pHero[tmpi], "match_attr" );
		}
	}
	return 0;
}

/* 创建数据 */
int coliseum_robot_create( int robotcount )
{
	int partnerid = 1;
	int itemid = 1;
	for ( int tmpi = 0; tmpi < robotcount; tmpi++ )
	{
		Actor actor = { 0 };
		City city = { 0 };
		Hero hero[COLISEUMLIST_HERO_MAX] = { 0 };
		int nation_level = 0;
		int nation_count = 0;
		char hero_randomtype = 0;
		/***************************************************** 角色部分 ******************************************************/
		/* 角色竞技场排名 */
		actor.coliseum_rank = tmpi + 1;
		/* 角色id */
		actor.actorid = COLISEUM_ROBOT_ACTORID_OFFSET + tmpi;
		/* 主角等级 */
		if ( tmpi <= robotcount * 1 / 5 )
		{
			actor.level = random( 95, 105 );
			nation_level = random( actor.level - 10, actor.level );
			nation_count = 5;
			city.place = random( 10, 20 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 1;
		}
		else if ( tmpi <= robotcount * 2 / 5 )
		{
			actor.level = random( 85, 95 );
			nation_level = random( actor.level - 20, actor.level - 5 );
			nation_count = 4;
			city.place = random( 8, 15 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 2;
		}
		else if ( tmpi <= robotcount * 3 / 5 )
		{
			actor.level = random( 75, 85 );
			nation_level = random( actor.level - 30, actor.level - 10 );
			nation_count = 3;
			city.place = random( 5, 10 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 3;
		}
		else if ( tmpi <= robotcount * 4 / 5 )
		{
			actor.level = random( 65, 75 );
			nation_level = random( actor.level - 30, actor.level - 15 );
			nation_count = 2;
			city.place = random( 3, 8 );
			city.attr.hero_row_num = 1;
			hero_randomtype = 4;
		}
		else
		{
			actor.level = random( 50, 65 );
			nation_level = random( actor.level - 40, actor.level - 20 );
			nation_count = 1;
			city.place = random( 1, 5 );
			city.attr.hero_row_num = 1;
			hero_randomtype = 5;
		}
		/* 角色名称 */
		coliseum_randomname( actor.name );
		/* 国家 */
		actor.nation = random( 1, 3 );

		/***************************************************** 城池部分 ******************************************************/
		// 国器
		for ( int type = 0; type < nation_count; type++ )
		{
			nation_level = nation_level <= 0 ? 1 : nation_level;
			city.neq_lv[type] = nation_level;
		}


		/***************************************************** 武将部分 ******************************************************/
		static int s_herokindlist1[] = { 14, 15, 17, 22, 23, 25, 26, 27, 28, 29, 30, 31, 41, 42, 44, 45, 46, 47, 48, 49, 50, 56, 57, 51, 52, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 97, 99 };
		static int s_herokindlist2[] = { 31, 41, 42, 44, 45, 46, 47, 48, 49, 50, 56, 57, 51, 52, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 97, 99 };
		static int s_herokindlist_token[] = { 50, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 95, 96, 97, 98, 99, 71 };

		int herokind[3] = { 0, 0, 0 };
		int herocolor[3] = { 0, 0, 0 };
		int index = 0;
		if ( hero_randomtype == 1 )
		{
			ruffle( s_herokindlist_token, sizeof( s_herokindlist_token ) / sizeof( s_herokindlist_token[0] ) );
			herokind[0] = s_herokindlist_token[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist_token[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist_token[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		else if ( hero_randomtype == 2 )
		{
			ruffle( s_herokindlist2, sizeof( s_herokindlist2 ) / sizeof( s_herokindlist2[0] ) );
			herokind[0] = s_herokindlist2[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist2[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist2[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		else
		{
			ruffle( s_herokindlist1, sizeof( s_herokindlist1 ) / sizeof( s_herokindlist1[0] ) );
			herokind[0] = s_herokindlist1[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist1[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist1[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		

		int sortindex[3] = { 0, 1, 2 };
		ruffle( sortindex, 3 );
		for ( int member = 0; member < 3; member++ )
		{
			hero[member].offset = HERO_COLISEUMOFFSET + member;
			hero[member].kind = herokind[sortindex[member]];
			hero[member].color = herocolor[sortindex[member]];
			hero[member].level = random( actor.level - 3, actor.level );
		}

		/* 插入列表 */
		coliseum_robot_match_list_insert( &actor, &city, hero, 3, "match_list" );
		char szMsg[16] = { 0 };
		sprintf( szMsg, "%d\r", tmpi );
		puts( szMsg );
	}
	return 0;
}

// 随机名称
int coliseum_randomname( char *name )
{
	int lastindex = random( 1, g_lastname_maxnum - 1 );
	int firstindex = random( 1, g_firstname_maxnum - 1 );
	snprintf( name, NAME_SIZE, "%s%s", g_lastname[lastindex].str, g_firstname[lastindex].str );
	name[NAME_SIZE - 1] = 0;
	return 0;
}
