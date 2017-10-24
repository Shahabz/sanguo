#ifndef _MAP_ZONE_H_
#define _MAP_ZONE_H_
#include "define.h"

int map_zone_load();
int map_zone_save( FILE *fp );
MapZone *map_zone_getptr( int zoneid );

// 坐标是否在指定地区里
char map_zone_inrange( int zoneid, short posx, short posy );

// 获取地区id
char map_zone_getid( short posx, short posy );

// 将显示单元添加到地区
int map_zone_addunit( int unit_index, int zoneid );

// 将显示单元移除出地区
int map_zone_delunit( int unit_index );

// 检查是不是在同一个地区
char map_zone_checksame( short posx, short posy, short tposx, short tposy );

// 设置地区归属
int map_zone_setnation( int zoneid, char nation );

// 指定地区随机一个空坐标
int map_zone_randpos( short zoneid, short *pPosx, short *pPosy );

// 地区切换
void map_zone_change( int actor_index, short posx, short posy );

// 地区玩家列表
int map_zone_citylist( int actor_index, int zoneid );

// 地区城镇列表
int map_zone_townlist( int actor_index, int zoneid );

// 地区皇城区域都城和名城所属列表
int map_zone_center_townlist( int actor_index );

// 地区皇城区域都城和名城更新
int map_zone_center_townchange( int townid );
#endif 
