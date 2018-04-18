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
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "city.h"
#include "city_attr.h"
#include "equip.h"
#include "building.h"
#include "vip.h"
#include "hero.h"
#include "quest.h"
#include "actor_friend.h"
#include "activity_04.h"
#include "girl.h"

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
	sprintf( szSQL, "select awardgroup,kind,minnum,maxnum,color,value,isshow,type,appendnum from award_group order by kind desc" );
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

int awardgroup_sendinfo_makestruct( int awardgroup, SLK_NetS_AwardInfoList *list, int limitcount )
{
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;
	if ( list->m_count >= limitcount )
		return -1;
	for ( int tmpi = 0; tmpi < g_awardgroup[awardgroup].allcount; tmpi++ )
	{
		if ( g_awardgroup[awardgroup].show[tmpi] == 0 )
			continue;
		if ( g_awardgroup[awardgroup].kind[tmpi] == 0 )
			continue;
		if ( g_awardgroup[awardgroup].kind[tmpi] < 0 )
		{
			awardgroup_sendinfo_makestruct( -g_awardgroup[awardgroup].kind[tmpi], list, limitcount );
		}
		else
		{
			list->m_list[list->m_count].m_kind = g_awardgroup[awardgroup].kind[tmpi];
			list->m_list[list->m_count].m_num = g_awardgroup[awardgroup].minnum[tmpi];
			list->m_count += 1;
			if ( limitcount > 0 && list->m_count >= limitcount )
			{
				break;
			}
		}
	}
	return 0;
}

// 发送给客户端显示的奖励组
int awardgroup_sendinfo( int actor_index, int awardgroup, int callback_code, int value, int limitcount )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( awardgroup <= 0 || awardgroup >= g_awardgroup_count )
		return -1;
	SLK_NetS_AwardInfoList list = { 0 };
	list.m_callback_code = callback_code;
	list.m_value = value;
	list.m_count = 0;
	awardgroup_sendinfo_makestruct( awardgroup, &list, limitcount );
	netsend_awardinfolist_S( actor_index, SENDTYPE_ACTOR, &list );
	return 0;
}

// 随机奖励组，不直接给
int awardgroup_random( int awardgroup, int level, AwardGetInfo *getinfo )
{
	if ( level <= 0 )
		level = 1;
	if ( awardgroup >= g_awardgroup_count )
		return -1;
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
			else if( kind > 0 )
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
				else if ( kind > 0 )
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

// 奖励结构序列化字符串
int awardgroup_makestr( AwardGetInfo *awardInfo, char *awardContent )
{
	if ( awardInfo->count > 0 )
	{
		for ( int tmpi = 0; tmpi < awardInfo->count; tmpi++ )
		{
			if ( awardInfo->kind[tmpi] <= 0 )
				continue;
			char tempitem[64] = { 0 };
			sprintf( tempitem, "%d,%d@", awardInfo->kind[tmpi], awardInfo->num[tmpi] );
			strcat( awardContent, tempitem );
		}
	}
	return 0;
}

// 发放奖励到邮件 ,通过邮件领取
int awardgroup_mail( int awardgroup, int level, char *itemContent )
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
			char tempitem[64] = { 0 };
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
	switch ( kind )
	{
	case AWARDKIND_VALUEBASE:// 数值
		building_give( g_actors[actor_index].city_index, BUILDING_Wall, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_StoreHouse, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Tech, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Craftsman, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Infantry, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Cavalry, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Archer, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Smithy, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Wash, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Shop, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Hero, 1 );
		building_give( g_actors[actor_index].city_index, BUILDING_Wishing, 1 );
		break;
	case AWARDKIND_SILVER:// 银币
		city_changesilver( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_WOOD:// 木材
		city_changewood( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_FOOD:// 粮食
		city_changefood( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_IRON:// 镔铁
		city_changeiron( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_TOKEN:// 元宝
		actor_change_token( actor_index, num, PATH_QUEST, 0 );
		break;
	case AWARDKIND_BODY:// 体力
		city_changebody( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_INFANTRY:// 步兵
		city_changesoldiers( g_actors[actor_index].city_index, 0, num, path );
		break;
	case AWARDKIND_CAVALRY:// 骑兵
		city_changesoldiers( g_actors[actor_index].city_index, 1, num, path );
		break;
	case AWARDKIND_ARCHER:// 弓兵
		city_changesoldiers( g_actors[actor_index].city_index, 2, num, path );
		break;
	case AWARDKIND_EXP:// 角色经验
		city_actorexp( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_VIPEXP:// VIP经验
		vip_exp( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_AUTOBUILD:// 自动建造次数
	{
		if ( num == 0 )
		{
			num = vip_attr_autobuild( city_getptr( actor_index ) );
		}
		city_change_autobuild( g_actors[actor_index].city_index, num, path );
		break;
	}
	case AWARDKIND_LEVYNUM:// 征收次数
		city_changelevy( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_PEOPLE:// 人口
		city_changepeople( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_PRESTIGE:// 威望值
		city_changeprestige( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_FRIENDSHIP:// 友谊积分
		city_changefriendship( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_TRAIN_QUICK:// 募兵加速
		city_train_awardquick( g_actors[actor_index].city_index, num );
		break;
	case AWARDKIND_BUILD_QUICK:// 建造加速
		building_awardquick( g_actors[actor_index].city_index, num );
		break;
	case AWARDKIND_HERO_WASH:// 免费洗髓次数
		city_change_herowash( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_EQUIP_WASH:// 免费洗练次数
		city_change_equipwash( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_LOSTREBUILD:// 高级重建次数
	{
		City *pCity = city_getptr( actor_index );
		if ( pCity )
			pCity->rb_num += num;
		break;
	}
	case AWARDKIND_CITYGUARDNUM:// 城防补充次数
		city_change_autoguard( g_actors[actor_index].city_index, num, path );
		break;
	case AWARDKIND_RES_SILVER:// 奖励未启用资源点（民居）
		building_giveres( g_actors[actor_index].city_index, BUILDING_Silver );
		break;
	case AWARDKIND_RES_WOOD:// 奖励未启用资源点（木材厂）
		building_giveres( g_actors[actor_index].city_index, BUILDING_Wood );
		break;
	case AWARDKIND_RES_FOOD:// 奖励未启用资源点（农田）
		building_giveres( g_actors[actor_index].city_index, BUILDING_Food );
		break;
	case AWARDKIND_RES_IRON:// 奖励未启用资源点（矿厂）
		building_giveres( g_actors[actor_index].city_index, BUILDING_Iron );
		break;
	case AWARDKIND_QUESTFIGHT:// 任务战斗
		quest_fight( actor_index, num );
		break;
	case AWARDKIND_CHANGENAME:
		quest_changename( actor_index );
		break;
	case AWARDKIND_BUFF_TRAIN:// 武卒官加速N%，时间1天
	{
		City *pCity = city_getptr( actor_index );
		if ( pCity->buffsec[CITY_BUFF_TRAIN] <= 0 )
		{
			pCity->bufftrain = num;
		}
		city_change_buff( g_actors[actor_index].city_index, CITY_BUFF_TRAIN, 86400, path );
		break;
	}
	case AWARDKIND_BUFF_MARCH:// 行军耗时降低N%，时间1天
		city_change_buff( g_actors[actor_index].city_index, CITY_BUFF_MARCH, 86400, path );
		break;
	case AWARDKIND_LEVY_SILVER:// 奖励N次银币征收量
	{
		int silver = city_yield_total( city_getptr( actor_index ), BUILDING_Silver, num );
		if ( silver > 0 )
			city_changesilver( g_actors[actor_index].city_index, silver, path );
		break;
	}
	case AWARDKIND_LEVY_WOOD:// 奖励N次木材征收量
	{
		int wood = city_yield_total( city_getptr( actor_index ), BUILDING_Wood, num );
		if ( wood > 0 )
			city_changewood( g_actors[actor_index].city_index, wood, path );
		break;
	}
	case AWARDKIND_LEVY_FOOD:// 奖励N次粮食征收量
	{
		int food = city_yield_total( city_getptr( actor_index ), BUILDING_Food, num );
		if ( food > 0 )
			city_changefood( g_actors[actor_index].city_index, food, path );
		break;
	}
	case AWARDKIND_LEVY_IRON:// 奖励N次镔铁征收量
	{
		int iron = city_yield_total( city_getptr( actor_index ), BUILDING_Iron, num );
		if ( iron > 0 )
			city_changeiron( g_actors[actor_index].city_index, iron, path );
		break;
	}
	case AWARDKIND_TEACHERAWARDNUM:// 师徒奖励次数
		actor_studentlevel_awardadd( actor_index, num );
		break;
	case AWARDKIND_ACTIVITY05_XW:// 陈宫信物
		g_actors[actor_index].act05_xw += num;
		break;
	case AWARDKIND_ACTIVITY04_VALUE_ADD:// 七日数值
		activity_04_addvalue( g_actors[actor_index].city_index, color, num );
		break;
	case AWARDKIND_ACTIVITY04_VALUE_SET:// 七日数值
		activity_04_setvalue( g_actors[actor_index].city_index, color, num );
		break;
	case AWARDKIND_FORBIDTALK:// 禁言
		actor_forbidtalk( actor_index, num );
		break;
	case AWARDKIND_PERMISSION_4:// 科技快研
		actor_set_sflag( actor_index, ACTOR_SFLAG_OFFICIAL_TECH, 1 );
		break;
	case AWARDKIND_PERMISSION_5:// 装备回收图纸
		actor_set_sflag( actor_index, ACTOR_SFLAG_EQUPIPDRAWING, 1 );
		break;
	case AWARDKIND_PERMISSION_6:// 作坊预设
		actor_set_sflag( actor_index, ACTOR_SFLAG_MATERIAL_MAKEWILL, 1 );
		break;
	default:
		if ( kind > 0 && kind < AWARDKIND_EQUIPBASE )
		{ // 道具
			item_getitem( actor_index, kind, num, color, path );
		}
		else if ( kind >= AWARDKIND_EQUIPBASE && kind < AWARDKIND_HEROBASE )// 装备（10000+装备编号）
		{
			for ( int i = 0; i < num; i++ )
				equip_getequip( actor_index, (kind - AWARDKIND_EQUIPBASE), path );
		}
		else if ( kind >= AWARDKIND_HEROBASE && kind < AWARDKIND_BUILDINGBASE ) // 英雄（20000+英雄编号）
		{
			hero_gethero( actor_index, (kind - AWARDKIND_HEROBASE), path );
		}
		else if ( kind >= AWARDKIND_BUILDINGBASE && kind < AWARDKIND_FUNCTION )// 建筑（30000+建筑编号）
		{
			building_give( g_actors[actor_index].city_index, (kind - AWARDKIND_BUILDINGBASE), num );
		}
		else if ( kind >= AWARDKIND_FUNCTION && kind < AWARDKIND_VALUEBASE )// 功能（40000+功能编号）
		{
			city_function_open( city_getptr( actor_index ), (kind - AWARDKIND_FUNCTION) );
		}
		else if ( kind >= AWARDKIND_SHAPE_0 && kind < AWARDKIND_SHAPE_END ) // 形象
		{
			actor_change_open( actor_index, kind - AWARDKIND_SHAPE_0 );
		}
		else if ( kind > AWARDKIND_GIRLBASE && kind < AWARDKIND_GIRLSOULBASE ) // 女将
		{
			girl_getgirl( city_getptr( actor_index ), kind - AWARDKIND_GIRLBASE, path );
		}
		else if ( kind > AWARDKIND_GIRLSOULBASE && kind < AWARDKIND_GIRLSOULBASE + 1000 ) // 女将碎片
		{
			girl_getsoul( city_getptr( actor_index ), kind - AWARDKIND_GIRLSOULBASE, num, path );
		}
		else if ( kind < 0 )
		{ // 道具组
			awardgroup_withindex( actor_index, -kind, city_mainlevel( g_actors[actor_index].city_index ), path, getinfo );
		}
		break;
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

// 权重随机
int weight_random( int *kind, int *num, int *weight, int count, int *totalweight, int *outkind, int *outnum )
{
	int allvalue = *totalweight;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		// 按照评价值方式随机
		int awardkind = kind[tmpi];
		int awardnum = num[tmpi];
		if ( awardkind <= 0 || awardnum <= 0 )
			continue;
		curvalue = weight[tmpi];
		if ( curvalue > 0 && curvalue > odds )
		{
			*outkind = awardkind;
			*outnum = awardnum;
			kind[tmpi] = 0;
			num[tmpi] = 0;
			*totalweight -= curvalue;
			break;
		}
		odds -= curvalue;
	}
	return 0;
}
