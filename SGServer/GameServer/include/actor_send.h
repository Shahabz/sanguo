#ifndef __ACTOR_SEND_H
#define __ACTOR_SEND_H
#include "define.h"

#define SENDTYPE_ACTOR		0	// 发给单个玩家
#define SENDTYPE_AREA		1	// 发给当前逻辑区域
#define SENDTYPE_MAP		2	// 发给当前地图
#define SENDTYPE_ZONE		3	// 发给当前游戏区域
#define SENDTYPE_NATION		4	// 发给当前国家
#define SENDTYPE_WORLD		5	// 发给当前服务器

int readtoqueue( int client_index, char *pBuf, int readsize, char stat );
int actor_senddata( int actor_index, char send_type, char *data, int datasize );

#endif
