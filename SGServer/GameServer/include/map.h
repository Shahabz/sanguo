#ifndef __MAP_H_
#define __MAP_H_

#include "area.h"
#define MAP_SIZEOFFSET 4

#define MAP_W	500
#define MAP_H	500

// 一个格子数据结构
typedef struct _tile
{
	char nation;			// 格子所属国家
	char unit_type;			// 格子上的显示对象类型
	int unit_index;			// 格子上的显示对象索引
	char owner_unit_type;	// 格子的主人
	int owner_unit_index;	// 格子的主人
}Tile;


// 世界地图
typedef struct _map
{
	int				m_nMaxWidth;	// 地图的宽度(方块单位)
	int				m_nMaxHeight;	// 地图的高度(方块单位)
	short			m_nAreaXNum;
	short			m_nAreaYNum;
	int				m_nAreaMaxCount;
	Area			*m_aArea;
	Tile			**m_aTileData;		// 每个格子的数据
}Map;

typedef struct _pos
{
	short x;
	short y;
}Pos;

int map_init();
void map_logic();
void map_sendinfo( int actor_index, short tposx, short tposy );
void map_sendthumbinfo( int actor_index );
void map_areaenter( int actor_index, int areaindex, short posx, short posy );

// 坐标是否在指定地区里
char map_zone_inrange( int zoneid, short posx, short posy );

// 获取地区id
char map_zone_getid( short posx, short posy );

// 检查是不是在同一个区域
char map_zone_checksame( short posx, short posy, short tposx, short tposy );

// 添加对象
int map_addobject( int type, int index, short posx, short posy );

// 移除对象
int map_delobject( int type, int index, short posx, short posy );

// 获取占地格子
int map_getobject_grid( int type, int index );

// 获得一个离指定地点最近的可迁城空白点 螺旋查找
int map_getcanmovenearest( short *pPosx, short *pPosy );

// 随机一个空白点
int map_getrandpos( int type, short *pPosx, short *pPosy );

// 根据指定点的范围获取一个可用坐标点
int map_getrandpos_withrange( short posx, short posy, int range, short *pPosx, short *pPosy );

// 随机玩家城池位置
int map_getrandcitypos( short *pPosx, short *pPosy );

// 指定地区随机一个空坐标
int map_zone_randpos( short zoneid, short *pPosx, short *pPosy );

// 判定这个地点是否能迁城
int map_canmove( short posX, short posY );

// dump地图数据
void map_tile_dump();
#endif
