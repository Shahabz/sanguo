#ifndef _HERO_H_
#define _HERO_H_
#include "define.h"

#define HERO_STATE_NORMAL	0	// ÏÐ
#define HERO_STATE_FIGHT	1	// Õ½¶·
#define HERO_STATE_GATHER	2	// ²É¼¯
#define HERO_STATE_KINGWAR	3	// ÑªÕ½

int actor_hero_getindex( int actor_index, int herokind );
Hero* actor_hero_getptr( int actor_index, int offset );
Hero* actor_hero_getptr_temp( int actor_index, int offset );

int city_hero_getindex( int city_index, int herokind );
Hero* city_hero_getptr( int city_index, int offset );
Hero* city_hero_getptr_withkind( int city_index, int herokind );

Hero *hero_getptr( int actor_index, int herokind );
char hero_getcolor( City *pCity, int herokind );

int hero_maxid_init();
HeroInfoConfig *hero_getconfig( int kind, int color );
char hero_defaultcolor( int kind );

int hero_gethero( int actor_index, int kind, short path );
int hero_up_auto( int actor_index, int offset );
int hero_up( int actor_index, int selectkind, int upkind, int replace_equip, char uptype );
int hero_down( int actor_index, int kind, char equip_down );
int hero_down_offline( int city_index, int actorid, int kind, char equip_down );
int hero_guard_sort( int actor_index, SLK_NetC_HeroGuardSort *list );

int hero_useexpitem( int actor_index, int herokind, int itemkind );
int hero_addexp( City *pCity, Hero *pHero, int exp, short path );
int hero_addsoldiers( int actor_index, int herokind, char path );
int hero_addsoldiers_audo( City *pCity );
int hero_changesoldiers( City *pCity, Hero *pHero, int value, short path );
void hero_guard_soldiers_auto( City *pCity );
int hero_guard_soldiers_token( int actor_index, int herokind );
int hero_changestate( int city_index, int herokind, char state );
char hero_getline( City *pCity, char state );
//int hero_attack( City *pCity, Hero *pHero );
//int hero_defense( City *pCity, Hero *pHero );
//int hero_troops( City *pCity, Hero *pHero );
//int hero_attack_increase( City *pCity, Hero *pHero );
//int hero_defense_increase( City *pCity, Hero *pHero );
//int hero_assault( City *pCity, Hero *pHero );
//int hero_defend( City *pCity, Hero *pHero );
int hero_attr_calc( City *pCity, Hero *pHero );
int hero_attr_calc_all( City *pCity, char path );

int hero_getexp_max( int level, int color );
void hero_makestruct( City *pCity, int offset, Hero *pHero, SLK_NetS_Hero *pValue );
int hero_list( int actor_index );
int hero_sendinfo( int actor_index, Hero *pHero );

// Ó¢ÐÛÏ´Ëè
int hero_wash_sendinfo( int actor_index );
int hero_wash_free( int actor_index, int herokind );
int hero_wash_token( int actor_index, int herokind );

// Á¼½«Í»ÆÆ
int hero_colorup_sendinfo( int actor_index, int herokind );
int hero_colorup_reset( int actor_index );
int hero_colorup( int actor_index, int herokind );

// Éñ¼¶Í»ÆÆ
int hero_godup( int actor_index, int herokind );

// Îä½«Ñ°·Ã
#define HERO_VISIT_AWARDMAX			32
#define HERO_VISIT_AWARDGROUPMAX	5
typedef struct _hero_visit_award
{
	int id;
	int kind;
	int num;
	char color;
	int value;
}HeroVisitAward;
typedef struct _hero_visit_awardgroup
{
	HeroVisitAward award[HERO_VISIT_AWARDMAX];
	char allcount;
	int allvalue;
}HeroVisitAwardGroup;
int hero_visit_init();
int hero_visit_setprogress( int actor_index, int value );
int hero_visit_sendinfo( int actor_index );
int hero_visit_snedflag( int actor_index );
int hero_visit_low( int actor_index );
int hero_visit_low10( int actor_index);
int hero_visit_high( int actor_index );
int hero_visit_high10( int actor_index );

// 
int hero_gm_level( City *pCity, int level );
int hero_gm_soldiers( City *pCity );
int hero_gm_getallhero( City *pCity );
#endif
