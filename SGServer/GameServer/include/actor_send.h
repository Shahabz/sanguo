#ifndef __ACTOR_SEND_H
#define __ACTOR_SEND_H


#define SENDTYPE_ACTOR		0	// 发给单个玩家
#define SENDTYPE_TEAM		1	// 发给队伍成员
#define SENDTYPE_AREA		2	// 发给当前区域
#define SENDTYPE_FIGHTAREA	3	// 发给当前区域/战场房间
#define SENDTYPE_NPCAREA	4	// 发给当前区域/战场房间
#define SENDTYPE_MAP		5	// 发给当前地图
#define SENDTYPE_WORLD		6	// 发给当前服务器
#define SENDTYPE_FIGHT		7	// 发给战斗的双方
#define SENDTYPE_CLUB		8	// 发给联盟

int readtoqueue( int client_index, char *pBuf, int readsize, char stat );
int actor_senddata( int actor_index, char send_type, char *data, int datasize );

#endif
