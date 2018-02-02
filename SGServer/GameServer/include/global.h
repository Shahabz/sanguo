#ifndef _GLOBAL_H_
#include "share_memory.h"

// 全局变量
typedef struct _global
{
	int actorlevel_max;
	short body_max;
	short body_sec;
	short levy_sec;
	char levy_max;
	int building_delete_cd;
	char equip_wash_max;
	short equip_wash_sec;
	char hero_wash_max;
	short hero_wash_sec;
	char buildinglevel_max;
	short city_guard_sec;
	float city_guard_sec_token;
	short city_guard_up_token;
	short worker_freetime;
	int trainfood;
	short equipext_token;
	int nequip_open_silver;
	int nequip_open_wood;
	float battlepower_attack;
	float battlepower_defense;
	float battlepower_troops;
	float battlepower_attack_increase;
	float battlepower_defense_increase;
	float battlepower_assault;
	float battlepower_defend;
	char techtree_level;
	float techquick_token;
	float forgingquick_token;
	float upgradequick_token;
	int worker_expire_1;
	int worker_expire_7;
	short worker_token_1;
	short worker_token_7;
	short city_guard_level;
	short city_guard_color_min[6];
	short nation_award_token;
	short hero_wash_token;
	short hero_wash_free_rand;
	short hero_wash_token_rand;
	short hero_wash_ratio_down;
	short hero_wash_ratio_up;
	short hero_wash_token_full_base;
	short hero_wash_token_full_odds;
	short hero_colorup_actorlevel;
	short hero_god_actorlevel;
	short equip_wash_super_token;
	short people_sec;
	float material_make_value1;
	short material_make_value2;
	int people_add_v1;
	int people_add_v2;
	int people_sub_v1;
	int people_sub_v2;
	int fight_crit_odds;
	int fight_crit_damage;
	int fight_dodge;
	int fight_control_value;
	int fight_attackmin_value;
	int fight_v1;
	int fight_v2;
	int fight_damage_randmin;
	int fight_damage_randmax;
	int brush_enemy_minute;
	int brush_res_minute;
	int hero_battle_minhp;
	int army_move;
	float army_march_food_v1;
	float army_march_food_v2;
	float army_march_food_v3;
	int hero_gather_duration[6];
	int army_move_kingzone;
	int mapevent_sec;
	int spy_token;
	int nation_mail_token;
	int spy_add_techlevel1;
	int spy_add_techlevel2;
	int spy_add_techlevel3;
	short spy_res_odds;
	short spy_wall_odds;
	short spy_hero_odds;
	short cityhelp_level;
	int cityhelp_sec;
	int cityfight_sec_limit[3];
	int cityfight_sec_group[3];
	short cityfight_body_cost[3];
	short cityfight_item_cost[3];
	float cityfight_rob_v1;
	float cityfight_rob_v2;
	float cityfight_rob_v3;
	float cityfight_rob_v4;
	int town_owner_award;
	int town_dev_expmax[3];
	int town_attackcd;
	int town_devcd;
	int town_dev_killenemy;
	int town_dev_gather;
	int town_dev_occupytown;
	int town_dev_contribute;
	int town_dev_silver;
	int town_dev_wood;
	short nationfight_actorlevel;
	int lost_rebuild_v1;
	float lost_rebuild_v2;
	short lost_rebuild_v3;
	float lost_rebuild_v4;
	char kingwar_activity_week;
	char kingwar_activity_hour;
	char kingwar_activity_minute;
	int kingwar_activity_duration;
	int kingwar_activity_duration_max;
	int	kingwar_activity_addsec;
	int	kingwar_dead_cd;
	char kingwar_token_def;
	char kingwar_token_sneak;
	char kingwar_token_pk;
	short hero_visit_mainlevel;
	short hero_visit_actorlevel;
	int hero_visit_free_cd;
	int hero_visit_high_sec;
	short hero_visit_low_token;
	short hero_visit_low_token10;
	short hero_visit_high_token;
	short hero_visit_high_token10;
	short hero_visit_low_itemnum;
	short hero_visit_low_itemnum10;
	short hero_visit_high_itemnum;
	short hero_visit_high_itemnum10;
	short hero_visit_low_max;
	short hero_visit_high_max;
	short hero_visit_low_normal_award;
	short hero_visit_low_hero_award;
	short hero_visit_high_normal_award;
	short hero_visit_high_hero_award;
	short hero_visit_progress_normal;
	short hero_visit_progress_color1;
	short hero_visit_progress_color2;
	short hero_visit_progress_color3;
	short hero_visit_progress_color4;
	short hero_visit_progress_max;
	char autobuild_max;
	short hero_cabinet_level1;
	short hero_cabinet_level2;
	short hero_cabinet_level3;
	short hero_cabinet_level4;
	short hero_cabinet_guard_hp;
	short hero_cabinet_guard_token;
	short story_sweep_vip;
	short buff_train;
	short buff_wind;
	short buff_fire;
	short buff_mountain;
	short buff_forest;
	short autoguard_max;
	short nequip_crit_odds[5];
	float nequip_make_quick_token;
	float nequip_remake_quick_token;
	char nation_quest_timer[3];
	char nation_rank_vote[5];
	char nation_place_max;
	char nation_official_term;
	char nation_official_select;
	short nation_vote_actorlevel;
	short nation_vote_basenum;
	short nation_change_notice_sec;
	int nation_online_notify_sec;
	char mapcall_range;
	char mapcall_nearby;
	short hero_god_troops;
	short hero_god_attack;
	short hero_god_defense;
	short hero_god_level;
	int wishing_opencd;
	float wishing_silver_to_wood;
	float wishing_silver_to_food;
	float wishing_wood_to_silver;
	float wishing_wood_to_food;
	float wishing_food_to_silver;
	float wishing_food_to_wood;
	char wishing_green_to_draw;
	short wishing_change_cd;
	int wishing_change_cdmax;
}Global;
int global_init();
void global_reload();

// 单独属于这个服务器的全局数据-读取直接都数据库的
#define WORLD_DATA_BRUSH_ENEMY_TIMER	1		// 刷新流寇计时
#define WORLD_DATA_BRUSH_RES_TIMER		2		// 刷新资源点计时
#define WORLD_DATA_GAME_DAY				3		// 游戏虚拟日期
#define WORLD_DATA_GAME_WEATHER			4		// 游戏虚拟天气
#define WORLD_DATA_GAME_DAY_LOOP		5		// 游戏虚拟日期循环用
#define WORLD_DATA_GAME_FDAY			6		// 游戏切换日期使用
#define WORLD_DATA_NATION_OFFICIAL_STATE		8 // 官员
#define WORLD_DATA_NATION_OFFICIAL_STATETIME	9 // 官员
#define WORLD_DATA_WORLDQUEST_BASE		10		// 世界任务起始
#define WORLD_DATA_WORLDQUEST_BASEEND	30		// 世界任务结束
#define WORLD_DATA_SAVEFLAG				99		// 存档的标记时间
#define WORLD_DATA_SAVEFILE_FLAG		100		// 崩溃执行完毕的标记时间
#define WORLD_DATA_MAX					128		
int world_data_init();
int world_data_get( int index, char *strvalue );
int world_data_getcache( int index );
int world_data_set( int index, int value, char *strvalue, FILE *fp );

#endif
