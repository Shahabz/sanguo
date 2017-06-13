#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "db.h"
#include "award.h"
#include "utils.h"
#include "define.h"
#include "system.h"
#include "item.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "server_netsend_auto.h"

extern MYSQL *myData;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

// 奖励组(可理解为掉落包)
AwardGroup *g_awardgroup;
// 奖励组数量
int g_awardgroup_count = 0;

// 奖励组初始化
int awardgroup_init()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	int tmpgroup;

	sprintf( szSQL, "select max(awardgroup) from award_group" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_awardgroup_count = atoi( row[0] ) + 1;
		else
			g_awardgroup_count = 0;
	}
	else
	{
		g_awardgroup_count = 0;
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_awardgroup = (AwardGroup *)malloc( sizeof(AwardGroup)*g_awardgroup_count );
	memset( g_awardgroup, 0, sizeof(AwardGroup)*g_awardgroup_count );

	// 确定每个目标组的个数并分配内存
	sprintf( szSQL, "select awardgroup,count(*) from award_group group by awardgroup" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		tmpgroup = atoi( row[0] );
		if ( tmpgroup < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		short allcount = atoi( row[1] );
		g_awardgroup[tmpgroup].kind = (int *)malloc( sizeof(int)*allcount );
		memset( g_awardgroup[tmpgroup].kind, 0, sizeof(int)*allcount );

		g_awardgroup[tmpgroup].minnum = (int *)malloc( sizeof(int)*allcount );
		memset( g_awardgroup[tmpgroup].minnum, 0, sizeof(int)*allcount );

		g_awardgroup[tmpgroup].maxnum = (int *)malloc( sizeof(int)*allcount );
		memset( g_awardgroup[tmpgroup].maxnum, 0, sizeof(int)*allcount );

		g_awardgroup[tmpgroup].color = (char *)malloc( sizeof(char)*allcount );
		memset( g_awardgroup[tmpgroup].color, 0, sizeof(char)*allcount );

		g_awardgroup[tmpgroup].value = (int *)malloc( sizeof(int)*allcount );
		memset( g_awardgroup[tmpgroup].value, 0, sizeof(int)*allcount );

		g_awardgroup[tmpgroup].show = (char *)malloc( sizeof(char)*allcount );
		memset( g_awardgroup[tmpgroup].show, 0, sizeof(char)*allcount );

		g_awardgroup[tmpgroup].type = (short *)malloc( sizeof(short)*allcount );
		memset( g_awardgroup[tmpgroup].type, 0, sizeof(short)*allcount );

		g_awardgroup[tmpgroup].appendnum = (int *)malloc( sizeof(int)*allcount );
		memset( g_awardgroup[tmpgroup].appendnum, 0, sizeof(int)*allcount );

		g_awardgroup[tmpgroup].allcount = 0;
		g_awardgroup[tmpgroup].allvalue = 0;
	}
	mysql_free_result( res );

	// 读取数据
	sprintf( szSQL, "select awardgroup,kind,minnum,maxnum,color,value,isshow,type,appendnum from award_group" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );

	while ( (row = mysql_fetch_row( res )) )
	{
		tmpgroup = atoi( row[0] );
		int allcount = g_awardgroup[tmpgroup].allcount;
		g_awardgroup[tmpgroup].kind[allcount] = atoi( row[1] );
		g_awardgroup[tmpgroup].minnum[allcount] = atoi( row[2] );
		g_awardgroup[tmpgroup].maxnum[allcount] = atoi( row[3] );
		g_awardgroup[tmpgroup].color[allcount] = atoi( row[4] );
		g_awardgroup[tmpgroup].value[allcount] = atoi( row[5] );
		g_awardgroup[tmpgroup].show[allcount] = atoi( row[6] );
		g_awardgroup[tmpgroup].type[allcount] = atoi( row[7] );
		g_awardgroup[tmpgroup].appendnum[allcount] = atoi( row[8] );
		if ( g_awardgroup[tmpgroup].value[allcount] > 0 )
			g_awardgroup[tmpgroup].allvalue += g_awardgroup[tmpgroup].value[allcount];
		g_awardgroup[tmpgroup].allcount++;
	}
	mysql_free_result( res );
	return 0;
}

// 重读
int awardgroup_reload()
{
	if ( g_awardgroup )
	{
		for ( int tmpgroup = 0; tmpgroup < g_awardgroup_count; tmpgroup++ )
		{
			if ( g_awardgroup[tmpgroup].kind )
			{
				free( g_awardgroup[tmpgroup].kind );
				g_awardgroup[tmpgroup].kind = NULL;
			}
			if ( g_awardgroup[tmpgroup].minnum )
			{
				free( g_awardgroup[tmpgroup].minnum );
				g_awardgroup[tmpgroup].minnum = NULL;
			}
			if ( g_awardgroup[tmpgroup].maxnum )
			{
				free( g_awardgroup[tmpgroup].maxnum );
				g_awardgroup[tmpgroup].maxnum = NULL;
			}
			if ( g_awardgroup[tmpgroup].color )
			{
				free( g_awardgroup[tmpgroup].color );
				g_awardgroup[tmpgroup].color = NULL;
			}
			if ( g_awardgroup[tmpgroup].value )
			{
				free( g_awardgroup[tmpgroup].value );
				g_awardgroup[tmpgroup].value = NULL;
			}
			if ( g_awardgroup[tmpgroup].show )
			{
				free( g_awardgroup[tmpgroup].show );
				g_awardgroup[tmpgroup].show = NULL;
			}
			if ( g_awardgroup[tmpgroup].type )
			{
				free( g_awardgroup[tmpgroup].type );
				g_awardgroup[tmpgroup].type = NULL;
			}
			if ( g_awardgroup[tmpgroup].appendnum )
			{
				free( g_awardgroup[tmpgroup].appendnum );
				g_awardgroup[tmpgroup].appendnum = NULL;
			}
		}
		free( g_awardgroup );
		g_awardgroup = NULL;
	}
	g_awardgroup_count = 0;
	awardgroup_init();
	return 0;
}

// 发送给客户端显示的奖励组
int awardgroup_sendinfo( int actor_index, int awardgroup, int callback_code, int unit_index, int limitcount )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;
	SLK_NetS_AwardInfoList list = { 0 };
	list.m_callback_code = callback_code;
	list.m_count = 0;
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		if ( g_awardgroup[awardgroup].show[tmpi] == 0 )
			continue;
		list.m_list[list.m_count].m_kind = g_awardgroup[awardgroup].kind[tmpi];
		/*list.m_list[list.m_count].m_num = g_awardgroup[awardgroup].minnum[tmpi];*/
		list.m_list[list.m_count].m_num = awardgroup_randnum( awardgroup,  0, tmpi );
		list.m_count += 1;
		if ( limitcount > 0 && list.m_count >= limitcount )
		{
			break;
		}
	}

	netsend_awardinfolist_S( actor_index, SENDTYPE_ACTOR, &list );
	return 0;
}

// 随机奖励组，不直接给
int awardgroup_random( int awardgroup, int level, AwardGetInfo *getinfo )
{
	if ( level <= 0 )
		level = 1;
	// 先给必得的
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		if ( g_awardgroup[awardgroup].value[tmpi] != -1 )
			continue;
		int kind = g_awardgroup[awardgroup].kind[tmpi];
		//int num = random( g_awardgroup[awardgroup].minnum[tmpi], g_awardgroup[awardgroup].maxnum[tmpi] );
		int num = awardgroup_randnum( awardgroup, level, tmpi );
		char color = g_awardgroup[awardgroup].color[tmpi];
		if ( getinfo )
		{
			if ( kind < 0 )
				awardgroup_random( -kind, level, getinfo );
			else
			{
				char ishave = 0;
				for ( int tmpj = 0; tmpj < getinfo->count; tmpj++ )
				{
					if ( getinfo->kind[tmpj] == kind && getinfo->color[tmpj] == color )
					{
						if ( getinfo->num[tmpj] < INT_MAX - num )
						{
							getinfo->num[tmpj] += num;
							ishave = 1;
							break;
						}
					}
				}
				if ( ishave == 0 && getinfo->count < AWARDGETINFO_MAXCOUNT )
				{
					getinfo->kind[getinfo->count] = kind;
					getinfo->num[getinfo->count] = num;
					getinfo->color[getinfo->count] = color;
					getinfo->count++;
				}
				if ( getinfo->count >= AWARDGETINFO_MAXCOUNT )
					break;
			}
		}
	}

	// 然后按照评价值随机
	int allvalue = g_awardgroup[awardgroup].allvalue;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		// 按照评价值方式随机
		curvalue = g_awardgroup[awardgroup].value[tmpi];
		if ( curvalue > 0 && curvalue > odds )
		{
			int kind = g_awardgroup[awardgroup].kind[tmpi];
			//int num = random( g_awardgroup[awardgroup].minnum[tmpi], g_awardgroup[awardgroup].maxnum[tmpi] );
			int num = awardgroup_randnum( awardgroup, level, tmpi );
			char color = g_awardgroup[awardgroup].color[tmpi];
			if ( getinfo )
			{
				if ( kind < 0 )
					awardgroup_random( -kind, level, getinfo );
				else
				{
					char ishave = 0;
					for ( int tmpj = 0; tmpj < getinfo->count; tmpj++ )
					{
						if ( getinfo->kind[tmpj] == kind && getinfo->color[tmpj] == color )
						{
							if ( getinfo->num[tmpj] < INT_MAX - num )
							{
								getinfo->num[tmpj] += num;
								ishave = 1;
								break;
							}
						}
					}
					if ( ishave == 0 && getinfo->count < AWARDGETINFO_MAXCOUNT )
					{
						getinfo->kind[getinfo->count] = kind;
						getinfo->num[getinfo->count] = num;
						getinfo->color[getinfo->count] = color;
						getinfo->count++;
					}
					if ( getinfo->count >= AWARDGETINFO_MAXCOUNT )
						break;;
				}
			}
			break;
		}
		odds -= curvalue;
	}
	return 0;
}

// 发放奖励到邮件 ,通过邮件领取
int awardgroup_mail( int awardgroup, int level, char * itemContent )
{
	if ( level <= 0 )
		level = 1;
	AwardGetInfo awardinfo = { 0 };
	awardgroup_random( awardgroup, level, &awardinfo );

	if ( awardinfo.count > 0 )
	{
		for ( int tmpi = 0; tmpi < awardinfo.count; tmpi++ )
		{
			if ( awardinfo.kind[tmpi] <= 0 )
				continue;
			char tempitem[128] = { 0 };
			sprintf( tempitem, "%d,%d@", awardinfo.kind[tmpi], awardinfo.num[tmpi] );
			strcat( itemContent, tempitem );
		}
	}
	return 0;
}
// 随机奖励组，通过索引，默认在线情况
int awardgroup_withindex( int actor_index, int awardgroup, int level, char path, AwardGetInfo *getinfo )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;

	// 先给必得的
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		if ( g_awardgroup[awardgroup].value[tmpi] != -1 )
			continue;
		int num = awardgroup_randnum( awardgroup, level, tmpi );
		award_getaward( actor_index, g_awardgroup[awardgroup].kind[tmpi], num, g_awardgroup[awardgroup].color[tmpi], path, getinfo );
	}

	// 然后按照评价值随机
	int allvalue = g_awardgroup[awardgroup].allvalue;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		// 按照评价值方式随机
		curvalue = g_awardgroup[awardgroup].value[tmpi];
		int num = awardgroup_randnum( awardgroup, level, tmpi );
		if ( curvalue > 0 && curvalue > odds )
		{
			award_getaward( actor_index, g_awardgroup[awardgroup].kind[tmpi], num, g_awardgroup[awardgroup].color[tmpi], path, getinfo );
			break;
		}
		odds -= curvalue;
	}
	return 0;
}


// 随机奖励组，通过玩家id，不确定是否在线
int awardgroup_withid( int actorid, int awardgroup, char path, AwardGetInfo *getinfo )
{
	if ( actorid < MINACTORID )
		return -1;
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;

	// 首先确定是否在线
	int actor_index = actor_getindex_withid( actorid );
	if ( actor_index >= 0 )
	{
		awardgroup_withindex( actor_index, awardgroup, 0,  path, getinfo );
		return 0;
	}
	awardgroup_random( awardgroup, 0, getinfo );
	for ( int tmpi = 0; tmpi < getinfo->count; tmpi++ )
	{
		if ( getinfo->kind[tmpi] <= 0 )
			continue;
		gift( actorid, getinfo->kind[tmpi], getinfo->num[tmpi], getinfo->color[tmpi], 0,0,-1, 0,0,-1, 0,0,-1, path );
	}
	return 0;
}

// 给奖励
int award_getaward( int actor_index, int kind, int num, char color, char path, AwardGetInfo *getinfo )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( kind > 0 && kind < AWARDKIND_RESBASE )
	{ // 道具
		item_getitem( actor_index, kind, num, color, path );
	}
	else if ( kind == AWARDKIND_WOOD )
	{
		
	}
	else if ( kind < 0 )
	{ // 道具组
		//awardgroup_withindex( actor_index, -kind, city_mainlevel( city_getptr( actor_index ) ), path, getinfo );
	}

	if ( getinfo && kind > 0 )
	{
		char ishave = 0;
		for ( int tmpj = 0; tmpj < getinfo->count; tmpj++ )
		{
			if ( getinfo->kind[tmpj] == kind && getinfo->color[tmpj] == color )
			{
				if ( getinfo->num[tmpj] < INT_MAX - num )
				{
					getinfo->num[tmpj] += num;
					ishave = 1;
					break;
				}
			}
		}
		if ( ishave == 0 && getinfo->count < AWARDGETINFO_MAXCOUNT )
		{
			getinfo->kind[getinfo->count] = kind;
			getinfo->num[getinfo->count] = num;
			getinfo->color[getinfo->count] = color;
			getinfo->count++;
		}
	}
	return 0;
}

// 离线奖励部分
int gift( int actorid, int kind1, int num1, char color1, int kind2, int num2, char color2, int kind3, int num3, char color3, int kind4, int num4, char color4, unsigned char path )
{
	if ( actorid < MINACTORID )
		return -1;
	char reconnect_flag = 0;
	char szSQL[1024] = {0};
	sprintf( szSQL, "INSERT INTO gift ( actorid, kind1, num1, color1, kind2, num2, color2, kind3, num3, color3, kind4, num4, color4, path) values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')", 
		actorid, kind1, num1, color1, kind2, num2, color2, kind3, num3, color3, kind4, num4, color4, path );
RE_GIFT_INSERT:
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_GIFT_INSERT;
		}
		return -1;
	}
	return 0;
}

// 领取过程
int gift_proc( int actor_index, int kind, int num, char color, unsigned char path )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( kind < AWARDKIND_OTHERBASE )
	{
		award_getaward( actor_index, kind, num, color, path, NULL );
	}
	else if ( kind == AWARDKIND_QUEST_SUCCESS )
	{
		
	}
	return 0;
}

// 获取礼物
int gift_get( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	sprintf( szSQL, "select kind1,num1,color1,kind2,num2,color2,kind3,num3,color3,kind4,num4,color4,path from gift where actorid='%d'", g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	int gift_count = 0;
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		gift_count++;
		unsigned char path = atoi( row[12] );
		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			int kind = atoi( row[index++] );
			int num = atoi( row[index++] );
			char color = atoi( row[index++] );
			if ( kind != 0 && num != 0 )
			{
				gift_proc( actor_index, kind, num, color, path );
			}
		}
	}
	
	mysql_free_result( res );
	if ( gift_count > 0 )
	{
		// 删除掉
		sprintf( szSQL, "delete from gift where actorid='%d'", g_actors[actor_index].actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
		}
	}
	
	return 0;
}

int gift_check( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int giftcount = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	sprintf( szSQL, "select count(*) from gift where actorid='%d'", g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		giftcount = atoi( row[0] );
		mysql_free_result( res );
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	if ( giftcount > 0 )
		gift_get( actor_index );
	return 0;
}

// 获取礼物
int gift_uid_get( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int giftcount = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	sprintf( szSQL, "select kind1,num1,color1,kind2,num2,color2,kind3,num3,color3,kind4,num4,color4 from gift_uid where uid='%s'",client_getusername(actor_index) );
	if ( mysql_query( myGame,szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ) )
	{
		int index = 0;
		giftcount++;
		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			int kind = atoi( row[index++] );
			int num = atoi( row[index++] );
			char color = atoi( row[index++] );
			if ( kind != 0 && num != 0 )
				gift_proc(actor_index,kind,num,color,PATH_SYSTEM);
		}
	}
	mysql_free_result( res );
	if ( giftcount > 0 )
	{
		// 删除掉
		sprintf( szSQL, "delete from gift_uid where uid='%s'", client_getusername(actor_index) );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
		}
	}
	return 0;
}

int gift_uid_check( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW  row;
	char szSQL[1024];
	int giftcount = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	sprintf(szSQL,"select count(*) from gift_uid where uid='%s'",client_getusername(actor_index));
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		giftcount = atoi( row[0] );
		mysql_free_result( res );
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	if ( giftcount > 0 )
		gift_uid_get( actor_index );
	return 0;
}

//随机数
int awardgroup_randnum( int awardgroup, int level, int index )
{
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;
	if ( level <= 0 )
		level = 1;
	if ( index < 0 || index >= g_awardgroup[awardgroup].allcount )
		return -1;
	int num = 0;
	if ( g_awardgroup[awardgroup].type[index] == 0 )
		num = random( g_awardgroup[awardgroup].minnum[index], g_awardgroup[awardgroup].maxnum[index] );
	else if ( g_awardgroup[awardgroup].type[index] == 1 )
	{
		num = random( g_awardgroup[awardgroup].minnum[index], g_awardgroup[awardgroup].maxnum[index] ) * level + g_awardgroup[awardgroup].appendnum[index];
		if ( num < 0 )
			num = 0;
	}
	return num;
}

