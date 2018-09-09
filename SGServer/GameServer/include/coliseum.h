#ifndef _COLISEUM_H_
#define _COLISEUM_H_
#define COLISEUMLIST_MAX 3
#define COLISEUMLIST_HERO_MAX 3

int coliseum_init();
int coliseum_enter( int actor_index );
int coliseum_sendlist( int actor_index );
int coliseum_getrank( int actorid );
int coliseum_setrank( int actorid, int rank, SLK_NetS_ColiseumCity *pInfo );
void coliseum_checkrank();
int coliseum_save( int actor_index );
int coliseum_list_save( int actor_index, char *pTab );
int coliseum_list_load( int actor_index, char *pTab );
int coliseum_match_attr_delete( int actorid, char *pTab );
int coliseum_match_attr_save( int actorid, int offset, Hero *pHero, char *pTab );
int coliseum_match_attr_load( int actorid, Hero *pHero, int count, char *pTab );
int coliseum_match( int actor_index );
int coliseum_random( int actor_index );
int coliseum_todaynum_reset( int actor_index );
int coliseum_update( int actor_index );
int coliseum_replaceteam( int actor_index, int kind0, int kind1, int kind2 );
int coliseum_fight( int actor_index, int index );
int coliseum_change_rank( int actor_index, int index, char winflag );
int coliseum_check_Historyrank( int actor_index );
int coliseum_fight_insert( char *json, int optime );
int coliseum_log_insert( SLK_NetS_ColiseumCity *pAttack, SLK_NetS_ColiseumCity *pDefense, char result, int fightid );
int coliseum_log_overdue();
int coliseum_loglist( int actor_index, int lastid );
int coliseum_load_ranklist();
int coliseum_ranklist( int actor_index );
int coliseum_serieswin_set( int actor_index, char flag );
int coliseum_award_everyday();
int coliseum_awardget( int actorid, int rank );

// 机器人
#define COLISEUM_ROBOT_COUNT			2000	// 竞技场机器人数量
#define COLISEUM_ROBOT_ACTORID_OFFSET	1000	// 机器人角色id起始
/* 竞技场机器人检查 */
int coliseum_robot_checkinit();

/* 竞技场机器人初始化 */
int coliseum_robot_init();

/* 检查竞技场列表里是否有数据 */
int coliseum_robot_check();

/* 清空竞技场列表里的所有数据 */
int coliseum_robot_delete( char *pTab );

/* 竞技场机器人创建 */
int coliseum_robot_create( int robotcount );

// 随机名称
int coliseum_randomname( char *name );
#endif
