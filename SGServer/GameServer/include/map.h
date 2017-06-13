#ifndef __MAP_H_
#define __MAP_H_

#include "area.h"
#define MAP_SIZEOFFSET 4

#define MAP_W	960
#define MAP_H	960

// 世界地图
typedef struct _map
{
	int				m_nMaxWidth;	// 地图的宽度(方块单位)
	int				m_nMaxHeight;	// 地图的高度(方块单位)
	short			m_nAreaXNum;
	short			m_nAreaYNum;
	int				m_nAreaMaxCount;
	Area			*m_aArea;
	unsigned char	**m_aTileData;		// 每个格子的数据
}Map;

typedef struct _pos
{
	short x;
	short y;
}Pos;

int map_init();
void map_logic();
void map_sendinfo( int actor_index, short tposx, short tposy );
void map_sendinfo_global( int actor_index, short serverid, short tposx, short tposy );
void map_sendthumbinfo( int actor_index );

// 添加对象
int map_addobject( int type, short posx, short posy, char unittype );

// 移除对象
int map_delobject( int type, short posx, short posy );

// 获得一个离指定地点最近的可迁城空白点 螺旋查找
int map_getcanmovenearest( short *pPosx, short *pPosy );

// 随机一个空白点
int map_getrandpos( int type, short *pPosx, short *pPosy );

// 根据指定点的范围获取一个可用坐标点
int map_getrandpos_withrange( int type, short posx, short posy, int range, short *pPosx, short *pPosy );

// 随机玩家城池位置
int map_getrandcitypos( short *pPosx, short *pPosy );

// 地图沼泽距离
short map_swamp_distance( short f_posx, short f_posy, short t_posx, short t_posy, short distance );

// dump地图数据
void map_tile_dump();
#endif
