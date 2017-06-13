#ifndef _GLOBAL_H_
#include "share_memory.h"

// 全局变量
typedef struct _global
{
	int actorlevel_max;
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
