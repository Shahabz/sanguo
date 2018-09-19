#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "vip.h"
#include "actor_notify.h"
#include "actor_times.h"
#include "story.h"
#include "fight.h"
#include "quest.h"
#include "auto_data_upgrade.h"
#include "auto_data_story.h"
#include "auto_data_monster.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern StoryInfo *g_storyinfo;
extern int g_storyinfo_maxnum;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern Fight g_fight;

StoryInfo *story_getconfig( int id )
{
	if ( id <= 0 || id >= g_storyinfo_maxnum )
		return NULL;
	return &g_storyinfo[id];
}

// ��һ�����������ĸ���
int story_nextid( int actor_index, int id )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum ) 
		return 0;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return 0;
	int nextid = config->nextid;;
	int step = 1;
	while ( step <= g_storyinfo_maxnum )
	{
		StoryInfo *nextConfig = story_getconfig( nextid );
		if ( nextConfig )
		{
			if ( nextConfig->type != STORY_TYPE_RES && nextConfig->type != STORY_TYPE_DRAWING && nextConfig->type != STORY_TYPE_ITEM )
			{
				break;
			}
			nextid = nextConfig->nextid;
		}
		step += 1;
	}
	return nextid;
}

int story_actor_setid( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( id <= g_actors[actor_index].storyid )
		return -1;
	int maxnum = g_actors[actor_index].storyid + 10;
	if ( maxnum > g_storyinfo_maxnum )
		maxnum = g_storyinfo_maxnum;
	for ( int tmpi = g_actors[actor_index].storyid; tmpi < maxnum; tmpi++ )
	{
		if ( g_storyinfo[tmpi].unlock_preid <= 0 )
			continue;
		if ( g_actors[actor_index].storyid == g_storyinfo[tmpi].preid )
		{
			if ( g_storyinfo[tmpi].type == STORY_TYPE_RES )
			{ // ����ʱ
				if ( g_storyinfo[tmpi].ressec_saveoffset >= 0 && g_storyinfo[tmpi].ressec_saveoffset < STORY_RESTIME_OFFSETMAX )
				{
					g_actors[actor_index].story_restime[g_storyinfo[tmpi].ressec_saveoffset] = (int)time( NULL ) + g_storyinfo[tmpi].ressec;
					story_sendrankstate( actor_index, tmpi, 3, g_storyinfo[tmpi].ressec_saveoffset );
				}
			} 
		}
	}

	g_actors[actor_index].storyid = id;
	return 0;
}

// �������и�����Ϣ
int story_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_actors[actor_index].storyid == 0 )
		story_actor_setid( actor_index, 11 );

	SLK_NetS_StoryList pValue = { 0 };
	pValue.m_storyid = g_actors[actor_index].storyid;
	for ( int tmpi = 0; tmpi < STORY_STAR_OFFSETMAX; tmpi++ )
	{// ��Ӣ�����Ǽ�
		pValue.m_story_star[tmpi] = g_actors[actor_index].story_star[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_HERO_OFFSETMAX; tmpi++ )
	{// ��ļ����
		pValue.m_story_hero[tmpi] = g_actors[actor_index].story_hero[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESTIME_OFFSETMAX; tmpi++ )
	{// ��Դ����ʱ��
		pValue.m_story_restime[tmpi] = g_actors[actor_index].story_restime[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESNUM_OFFSETMAX; tmpi++ )
	{// ��Դ��������
		pValue.m_story_resnum[tmpi] = g_actors[actor_index].story_resnum[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESRESET_OFFSETMAX; tmpi++ )
	{// ��Դ�������ô���
		pValue.m_story_resreset[tmpi] = g_actors[actor_index].story_resreset[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_ITEMNUM_OFFSETMAX; tmpi++ )
	{// ���߸����������
		pValue.m_story_itemnum[tmpi] = g_actors[actor_index].story_itemnum[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_DRAWING_OFFSETMAX; tmpi++ )
	{// װ��ͼֽ�����Ƿ���
		pValue.m_story_drawing[tmpi] = g_actors[actor_index].story_drawing[tmpi];
	}

	// ɨ���佫
	int maxhero = 2 + pCity->attr.hero_up_num;
	if ( maxhero > 4 )
		maxhero = 4;
	for ( int tmpi = 0; tmpi < maxhero; tmpi++ )
	{
		if ( g_actors[actor_index].story_sweephero[tmpi] == 0 )
		{// ��ʼĬ��ɨ���佫
			g_actors[actor_index].story_sweephero[tmpi] = pCity->hero[tmpi].kind;
		}
		pValue.m_sweep_herokind[tmpi] = g_actors[actor_index].story_sweephero[tmpi];
	}
	netsend_storylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ���͹ؿ���������Ϣ
int story_sendrank( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;

	SLK_NetS_StoryRank pValue = { 0 };
	pValue.m_exp = config->exp;
	pValue.m_body = (char)config->body;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		int monsterid = config->monsterid[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		pValue.m_list[pValue.m_count].m_monsterid = monsterid;
		pValue.m_list[pValue.m_count].m_level = g_monster[monsterid].level;
		pValue.m_list[pValue.m_count].m_shape = g_monster[monsterid].shape;
		pValue.m_list[pValue.m_count].m_color = (char)g_monster[monsterid].color;
		pValue.m_list[pValue.m_count].m_corps = (char)g_monster[monsterid].corps;
		pValue.m_list[pValue.m_count].m_hp = g_monster[monsterid].troops;
		pValue.m_count += 1;
	}
	netsend_storyrank_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ���¸���״̬
int story_sendrankstate( int actor_index, int id, int savetype, int saveoffset )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_StoryState pValue = { 0 };
	pValue.m_storyid = id;
	pValue.m_actor_storyid = g_actors[actor_index].storyid;
	pValue.m_savetype = savetype;
	pValue.m_saveoffset = saveoffset;
	if ( savetype == 1 )
	{ // �����Ǽ�
		if ( saveoffset < 0 || saveoffset >= STORY_STAR_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_star[saveoffset];
	}
	else if ( savetype == 2 )
	{ // ������ļ��Ϣ
		if ( saveoffset < 0 || saveoffset >= STORY_HERO_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_hero[saveoffset];
	}
	else if ( savetype == 3 )
	{ // ������Դ����ʱ��
		if ( saveoffset < 0 || saveoffset >= STORY_RESTIME_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_restime[saveoffset];
	}
	else if ( savetype == 4 )
	{ // ������Դ�����ɴ����
		if ( saveoffset < 0 || saveoffset >= STORY_RESNUM_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_resnum[saveoffset];
	}
	else if ( savetype == 5 )
	{ // ������Դ�������ô���
		if ( saveoffset < 0 || saveoffset >= STORY_RESRESET_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_resreset[saveoffset];
	}
	else if ( savetype == 6 )
	{ // ���µ��߸����������
		if ( saveoffset < 0 || saveoffset >= STORY_ITEMNUM_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_itemnum[saveoffset];
	}
	else if ( savetype == 7 )
	{ // װ��ͼֽ�����Ƿ���
		if ( saveoffset < 0 || saveoffset >= STORY_DRAWING_OFFSETMAX )
			return -1;
		pValue.m_state = g_actors[actor_index].story_drawing[saveoffset];
	}
	netsend_storystate_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}


// ս��
int story_battle( int actor_index, SLK_NetC_StoryBattle *pValue )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( pValue->m_storyid );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < pValue->m_storyid && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type == STORY_TYPE_NORMAL )
	{ // ��ͨ��������һ�ξͲ����ٴ�
		if ( g_actors[actor_index].storyid > pValue->m_storyid )
		{
			return -1;
		}
	}
	else if( config->type == STORY_TYPE_BOSS )
	{ // ��Ӣ���������Է�����

	}
	else if ( config->type == STORY_TYPE_ITEM )
	{ // ���߸��������ߴ���û�ˣ��Ͳ��ܴ���
		int saveoffset = config->itemnum_saveoffset;
		if ( saveoffset < 0 || saveoffset >= STORY_ITEMNUM_OFFSETMAX )
			return -1;
		if ( g_actors[actor_index].story_itemnum[saveoffset] >= config->itemnum )
			return -1;
	}
	else
		return -1;

	// ��������Ƿ��㹻
	if ( pCity->body < config->body )
	{
		return -1;
	}

	// ����ս��
	if ( fight_start_bystory( actor_index, pValue ) < 0 )
		return -1;

	// ����
	char content[1024] = { 0 };
	char star = 0;
	if ( g_fight.result == FIGHT_WIN )
	{ // ʤ��
		// �۳�����
		city_changebody( pCity->index, -config->body, PATH_STORY );

		// ������
		AwardGetInfo getinfo = { 0 };
		city_actorexp( pCity->index, config->exp, PATH_STORY );
		city_changesilver( pCity->index, config->silver, PATH_STORY );
		awardgroup_withindex( actor_index, config->awardgroup, 0, PATH_STORY, &getinfo );

		// ��ǰ���ȵ���һ������id
		int nextid = story_nextid( actor_index, pValue->m_storyid );
		story_actor_setid( actor_index, nextid );

		// ��ͨ����
		if ( config->type == STORY_TYPE_NORMAL )
		{
			story_sendrankstate( actor_index, pValue->m_storyid, 0, 0 );

		}
		// ��Ӣ����
		else if ( config->type == STORY_TYPE_BOSS && config->star_saveoffset >= 0 && config->star_saveoffset < STORY_STAR_OFFSETMAX )
		{ // ����ս������
			int lostper = (int)ceil( (g_fight.attack_total_hp / (float)g_fight.attack_total_maxhp) * 100 );
			if ( lostper >= 80 )
				star = 3;
			else if ( lostper >= 60 )
				star = 2;
			else
				star = 1;
			if ( g_actors[actor_index].story_star[config->star_saveoffset] < star )
			{
				g_actors[actor_index].story_star[config->star_saveoffset] = star;
				story_sendrankstate( actor_index, pValue->m_storyid, 1, config->star_saveoffset );
			}
		}
		// ���߸���
		else if ( config->type == STORY_TYPE_ITEM && config->itemnum_saveoffset >= 0 && config->itemnum_saveoffset < STORY_ITEMNUM_OFFSETMAX )
		{ 
			int odds = rand() % 100;
			if ( g_actors[actor_index].story_itemnum[config->itemnum_saveoffset] < config->itemnum && odds < config->item_awardodds )
			{
				item_getitem( actor_index, config->item_awardkind, 1, -1, PATH_STORY );
				g_actors[actor_index].story_itemnum[config->itemnum_saveoffset] += 1;
				story_sendrankstate( actor_index, pValue->m_storyid, 6, config->itemnum_saveoffset );
				if ( getinfo.count <= 23 )
				{
					getinfo.kind[getinfo.count] = config->item_awardkind;
					getinfo.num[getinfo.count] = 1;
					getinfo.count += 1;
				}
			}
		}

		// ս����ʾ����
		char award_content[256] = { 0 };
		awardgroup_makestr( &getinfo, award_content );
		sprintf( content, "{\"star\":%d,\"exp\":%d,\"silver\":%d,\"award\":\"%s\"}", star, config->exp, config->silver, award_content );
	}
	else
	{ // ʧ��
		// �۳�����
		city_changebody( pCity->index, -config->lose_body, PATH_STORY );
	}

	// ս�����㣬��Ҫ���佫����
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( g_fight.attack_unit[tmpi].offset < 0 )
			continue;
		int hero_index = city_hero_getindex( pCity->index, g_fight.attack_unit[tmpi].kind );
		if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
			continue;
		if ( g_fight.attack_unit[tmpi].exp > 0 )
		{
			hero_addexp( pCity, &pCity->hero[hero_index], g_fight.attack_unit[tmpi].exp, PATH_STORY );
		}
	}

	// ����ս��
	fight_play( actor_index, g_fight.unit_json, content );
	if ( g_fight.result == FIGHT_WIN )
	{
		// ����
		quest_addvalue( pCity, QUEST_DATATYPE_STORY, pValue->m_storyid, 0, 1 );

		// ��Ҫ����Դ����ʹ��
		if ( config->type != STORY_TYPE_ITEM && config->itemnum_saveoffset >= 0 && config->itemnum_saveoffset < STORY_ITEMNUM_OFFSETMAX )
		{
			int odds = rand() % 100;
			if ( g_actors[actor_index].story_itemnum[config->itemnum_saveoffset] < config->itemnum && odds < config->item_awardodds )
			{
				award_getaward( actor_index, config->item_awardkind, config->itemnum, -1, PATH_STORY, NULL );
				g_actors[actor_index].story_itemnum[config->itemnum_saveoffset] += config->itemnum;
			}
		}
	}
	return 0;
}

// ��Դ��������
int story_res_get( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_RES )
		return -1;
	int saveoffset = config->resnum_saveoffset;
	if ( saveoffset < 0 || saveoffset >= STORY_RESNUM_OFFSETMAX )
		return -1;
	if ( g_actors[actor_index].story_resnum[saveoffset] >= config->resnum )
		return -1;
	if ( config->restype == 1 )
	{
		city_changesilver( pCity->index, config->rescount, PATH_STORY_RES );
	}
	else if ( config->restype == 2 )
	{
		city_changewood( pCity->index, config->rescount, PATH_STORY_RES );
	}
	else if ( config->restype == 3 )
	{
		city_changefood( pCity->index, config->rescount, PATH_STORY_RES );
	}
	else if ( config->restype == 4 )
	{
		city_changeiron( pCity->index, config->rescount, PATH_STORY_RES );
	}
	g_actors[actor_index].story_resnum[saveoffset] += 1;
	story_sendrankstate( actor_index, id, 4, saveoffset );

	// ����
	quest_addvalue( pCity, QUEST_DATATYPE_STORY, id, 0, 1 );
	return 0;
}

 // ��Դ��������
int story_res_reset( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_RES )
		return -1;
	if ( config->resnum_saveoffset < 0 || config->resnum_saveoffset >= STORY_RESNUM_OFFSETMAX )
		return -1;
	if ( config->res_reset_saveoffset < 0 || config->res_reset_saveoffset >= STORY_RESRESET_OFFSETMAX )
		return -1;
	if ( config->ressec_saveoffset < 0 || config->ressec_saveoffset >= STORY_RESTIME_OFFSETMAX )
		return -1;
	if ( (int)time( NULL ) >= g_actors[actor_index].story_restime[config->ressec_saveoffset] ) // ʱ�䵽��
		return -1;

	int resetcount = g_actors[actor_index].story_resreset[config->res_reset_saveoffset];
	if ( resetcount >= config->res_reset_num ) // ���ô�������
		return -1;
	// ��ȡ���ô���������ʯ
	if ( resetcount >= 6 )
		resetcount = 5;
	int token = config->res_reset_token[resetcount];
	if ( actor_change_token( actor_index, -token, PATH_STORY_RES, 0 ) < 0 )
		return -1;
	g_actors[actor_index].story_resreset[config->res_reset_saveoffset] += 1;
	g_actors[actor_index].story_resnum[config->resnum_saveoffset] = 0;
	story_sendrankstate( actor_index, id, 5, config->res_reset_saveoffset );
	story_sendrankstate( actor_index, id, 4, config->resnum_saveoffset );
	return 0;
}

// ��ļ���������ļ
int story_hero_free( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_HERO )
		return -1;
	if ( config->hero_saveoffset < 0 || config->hero_saveoffset >= STORY_HERO_OFFSETMAX )
		return -1;
	if ( config->hero_kind[0] > 0 && config->hero_kind[1] > 0 )
	{ // ������Ӣ�ۣ����һ��
		if ( g_actors[actor_index].story_hero[config->hero_saveoffset] != 0 )
		{// ֮ǰ�Ѿ����һ���ˣ����߶��õ���
			return -1;
		}
		int odds = rand() % 100;
		if ( odds < config->hero_odds )
		{ // ��ļ��һ��
			hero_gethero( actor_index, config->hero_kind[0], PATH_STORY_HERO );
			g_actors[actor_index].story_hero[config->hero_saveoffset] = 1;
		}
		else
		{ // ��ļ�ڶ���
			hero_gethero( actor_index, config->hero_kind[1], PATH_STORY_HERO );
			g_actors[actor_index].story_hero[config->hero_saveoffset] = 10;
		}
		// ��ǰ���ȵ���һ������id
		int nextid = story_nextid( actor_index, id );
		story_actor_setid( actor_index, nextid );
	}
	else if ( config->hero_kind[0] > 0 )
	{ // ֱ����ļ��һ��
		if ( g_actors[actor_index].story_hero[config->hero_saveoffset] == 1 )
			return -1;
		hero_gethero( actor_index, config->hero_kind[0], PATH_STORY_HERO );
		g_actors[actor_index].story_hero[config->hero_saveoffset] = 1;
		// ��ǰ���ȵ���һ������id
		int nextid = story_nextid( actor_index, id );
		story_actor_setid( actor_index, nextid );
	}
	story_sendrankstate( actor_index, id, 2, config->hero_saveoffset );

	// ����
	quest_addvalue( pCity, QUEST_DATATYPE_STORY, id, 0, 1 );
	quest_addvalue( pCity, QUEST_DATATYPE_HERO_CALL_STORY, id, 0, 1 );
	return 0;
}

// ��ļ������Ǯ��ļ
int story_hero_token( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_HERO )
		return -1;
	if ( config->hero_saveoffset < 0 || config->hero_saveoffset >= STORY_HERO_OFFSETMAX )
		return -1;
	if ( config->hero_kind[0] > 0 && config->hero_kind[1] > 0 )
	{ // ������Ӣ�ۣ����Ӧ�û�Ǯ����
		if ( g_actors[actor_index].story_hero[config->hero_saveoffset] == 0 || g_actors[actor_index].story_hero[config->hero_saveoffset] == 11 )
		{// ������Ӣ�ۣ�һ����û������߶��õ���
			return -1;
		}
		if ( g_actors[actor_index].story_hero[config->hero_saveoffset] == 1 )
		{ // ֮ǰ�õ����ǵ�һ��������ڶ�����
			if ( actor_change_token( actor_index, -config->hero_token, PATH_STORY_HERO, 0 ) < 0 )
				return -1;
			hero_gethero( actor_index, config->hero_kind[1], PATH_STORY_HERO );
			g_actors[actor_index].story_hero[config->hero_saveoffset] = 11;
		}
		else if ( g_actors[actor_index].story_hero[config->hero_saveoffset] == 10 )
		{ // ֮ǰ�õ����ǵڶ����������һ����
			if ( actor_change_token( actor_index, -config->hero_token, PATH_STORY_HERO, 0 ) < 0 )
				return -1;
			hero_gethero( actor_index, config->hero_kind[0], PATH_STORY_HERO );
			g_actors[actor_index].story_hero[config->hero_saveoffset] = 11;
		}
	}
	else if ( config->hero_kind[0] > 0 && config->hero_token > 0 )
	{ // ֱ�ӹ����һ��
		if ( actor_change_token( actor_index, -config->hero_token, PATH_STORY_HERO, 0 ) < 0 )
			return -1;
		hero_gethero( actor_index, config->hero_kind[0], PATH_STORY_HERO );
		g_actors[actor_index].story_hero[config->hero_saveoffset] = 1;
	}
	story_sendrankstate( actor_index, id, 2, config->hero_saveoffset );
	// ����
	quest_addvalue( pCity, QUEST_DATATYPE_HERO_CALL_STORY, id, 0, 1 );
	return 0;
}

// ͼֽ��������
int story_drawing_buy( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_DRAWING )
		return -1;
	if ( config->drawing_saveoffset < 0 || config->drawing_saveoffset >= STORY_DRAWING_OFFSETMAX )
		return -1;
	if ( g_actors[actor_index].story_drawing[config->drawing_saveoffset] > 0 )
		return -1;
	if ( actor_change_token( actor_index, -config->drawing_token, PATH_STORY_DRAWING, 0 ) < 0 )
		return -1;
	g_actors[actor_index].story_drawing[config->drawing_saveoffset] = 1;
	story_sendrankstate( actor_index, id, 7, config->drawing_saveoffset );
	// ����
	quest_addvalue( pCity, QUEST_DATATYPE_STORY, id, 0, 1 );
	return 0;
}

// ͼֽ��������
int story_drawing_get( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_DRAWING )
		return -1;
	if ( config->drawing_saveoffset < 0 || config->drawing_saveoffset >= STORY_DRAWING_OFFSETMAX )
		return -1;
	if ( g_actors[actor_index].story_drawing[config->drawing_saveoffset] != 1 )
		return -1;
	item_getitem( actor_index, config->drawing_kind, 1, -1, PATH_STORY_DRAWING );
	g_actors[actor_index].story_drawing[config->drawing_saveoffset] = 2;
	story_sendrankstate( actor_index, id, 7, config->drawing_saveoffset );
	// ����
	quest_addvalue( pCity, QUEST_DATATYPE_STORY, id, 0, 1 );
	return 0;
}

// ����ɨ��
int story_sweep( int actor_index, int id, int herokind0, int herokind1, int herokind2, int herokind3 )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( g_actors[actor_index].storyid < id && g_actors[actor_index].storyid <= config->preid )
		return -1;
	if ( config->type != STORY_TYPE_BOSS )
		return -1;
	if ( config->star_saveoffset < 0 || config->star_saveoffset >= STORY_STAR_OFFSETMAX )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_STORYSWEEP ) == 0 )
	{ // û��Vip�ȼ�,����Ƿ�����
		if ( g_actors[actor_index].story_star[config->star_saveoffset] < 3 )
			return -1;
	}

	g_actors[actor_index].story_sweephero[0] = 0;
	g_actors[actor_index].story_sweephero[1] = 0;
	g_actors[actor_index].story_sweephero[2] = 0;
	g_actors[actor_index].story_sweephero[3] = 0;
	int offset = 0;
	if ( herokind0 > 0 )
	{
		g_actors[actor_index].story_sweephero[offset++] = (short)herokind0;
	}
	if ( herokind1 > 0 )
	{
		g_actors[actor_index].story_sweephero[offset++] = (short)herokind1;
	}
	if ( herokind2 > 0 )
	{
		g_actors[actor_index].story_sweephero[offset++] = (short)herokind2;
	}
	if ( herokind3 > 0 )
	{
		g_actors[actor_index].story_sweephero[offset++] = (short)herokind3;
	}

	if ( offset == 0 )
	{
		actor_notify_alert( actor_index, 2037 );
		return -1;
	}

	int maxcount = 5;
	// ��������Ƿ��㹻
	if ( pCity->body < config->body*maxcount )
	{
		return -1;
	}
	// �۳�����
	city_changebody( pCity->index, -config->body*maxcount, PATH_STORY_SWEEP );

	SLK_NetS_StorySweepResult pValue = { 0 };

	// �������ҽ���
	city_actorexp( pCity->index, config->exp*maxcount, PATH_STORY_SWEEP );
	city_changesilver( pCity->index, config->silver*maxcount, PATH_STORY_SWEEP );
	pValue.m_exp = config->exp*maxcount;
	pValue.m_silver = config->silver*maxcount;

	// ������
	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < maxcount; tmpi++ )
	{
		awardgroup_withindex( actor_index, config->awardgroup, 0, PATH_STORY_SWEEP, &getinfo );
	}
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		pValue.m_award[tmpi].m_kind = getinfo.kind[tmpi];
		pValue.m_award[tmpi].m_num = getinfo.num[tmpi];
		pValue.m_awardcount += 1;
	}

	// ɨ������£��佫��þ��� = Int( �ؿ��佫�ܱ��� / �ؿ��佫���� )
	int exp = 0;
	int monster_hero = 0;
	int monster_troops = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		int monsterid = config->monsterid[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		monster_hero += 1;
		monster_troops += g_monster[monsterid].troops;
	}
	if ( monster_hero > 0 )
	{
		exp = monster_troops / monster_hero;
	}

	// ս�����㣬��Ҫ���佫����
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		int herokind = g_actors[actor_index].story_sweephero[tmpi];
		if ( herokind <= 0 )
			continue;
		Hero *pHero = hero_getptr( actor_index, herokind );
		if ( !pHero )
			continue;
		pValue.m_hero[pValue.m_herocount].m_kind = herokind;
		pValue.m_hero[pValue.m_herocount].m_color = pHero->color;
		pValue.m_hero[pValue.m_herocount].m_level = pHero->level;
		pValue.m_hero[pValue.m_herocount].m_pre_exp = pHero->exp;
		pValue.m_hero[pValue.m_herocount].m_exp = exp;
		pValue.m_herocount += 1;
		hero_addexp( pCity, pHero, exp, PATH_STORY_SWEEP );
	}

	netsend_storysweepresult_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// GM
int story_gm( int actor_index, int storyid )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	g_actors[actor_index].storyid = storyid;
	story_sendinfo( actor_index );
	return -1;
}
