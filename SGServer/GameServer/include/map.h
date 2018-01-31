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
	short townid;			// 哪个城镇的辐射范围
	char unit_type;			// 格子上的显示对象类型
	int unit_index;			// 格子上的显示对象索引
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
void map_areaenter( int actor_index, int areaindex, short posx, short posy, char areaupdate );

// 添加对象
int map_addobject( int type, int index, short posx, short posy );

// 移除对象
int map_delobject( int type, int index, short posx, short posy );

// 获取占地格子
int map_getobject_grid( int type, int index );

// 设置格子所属国家
int map_tile_setnation( short posx, short posy, int range, short townid, char nation );
char map_tile_getnation( short posx, short posy );
short map_tile_gettownid( short posx, short posy );

// 获得一个离指定地点最近的可迁城空白点 螺旋查找
int map_getcanmovenearest( short *pPosx, short *pPosy );

// 随机一个空白点
int map_getrandpos( int grid, short *pPosx, short *pPosy );

// 根据指定点的范围获取一个可用坐标点
int map_getrandpos_withrange( short posx, short posy, int range, short *pPosx, short *pPosy );

// 随机玩家城池位置
int map_getrandcitypos( short *pPosx, short *pPosy );

// 随机坐标
int map_getrandpos_withtype( int type, short *pPosx, short *pPosy );

// 判定这个地点是否能迁城
int map_canmove( short posX, short posY );

// 跳转坐标
int map_goto_withpos( int actor_index, char op, short posx, short posy );

// 距离
int map_distance( short fposx, short fposy, short tposx, short tposy );

// dump地图数据
void map_tile_dump();
#endif
