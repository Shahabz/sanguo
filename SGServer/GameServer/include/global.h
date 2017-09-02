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
}Global;
int global_init();
void global_reload();

// 单独属于这个服务器的全局数据
#define WORLD_DATA_BRUSH_ENEMY_TIMER	1		// 刷新流寇计时
#define WORLD_DATA_BRUSH_RES_TIMER		2		// 刷新资源点计时
#define WORLD_DATA_SAVEFLAG				99		// 存档的标记时间
#define WORLD_DATA_SAVEFILE_FLAG		100		// 崩溃执行完毕的标记时间
#define WORLD_DATA_MAX					128		
int world_data_init();
int world_data_get( int index, char *strvalue );
int world_data_set( int index, int value, char *strvalue, FILE *fp );
#endif
