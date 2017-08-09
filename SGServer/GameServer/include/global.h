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
}Global;
int global_init();
void global_reload();

// 单独属于这个服务器的全局数据
#define WORLD_DATA_LAST_CITYPOSX		1
#define WORLD_DATA_LAST_CITYPOSY		2
#define WORLD_DATA_SAVEFLAG				99		// 存档的标记时间
#define WORLD_DATA_SAVEFILE_FLAG		100		// 崩溃执行完毕的标记时间
#define WORLD_DATA_MAX					128		
int world_data_init();
int world_data_get( int index, char *strvalue );
int world_data_set( int index, int value, char *strvalue, FILE *fp );
#endif
