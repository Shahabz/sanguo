#ifndef _MAP_RES_H_
#define _MAP_RES_H_
#include "define.h"

int map_res_load();
int map_res_save( FILE *fp );
int map_res_maxcount_set( int maxcount );
MapRes *map_res_getptr( int index );
MapResInfo *map_res_getconfig( int kind );

// 显示单元属性
void map_res_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_res_getpos( int index, short *posx, short *posy );

// 创建一个资源点
int map_res_create( short kind, short posx, short posy );

// 删除一个资源点
int map_res_delete( int index );

// 资源点数量
int map_res_num( int zoneid, int kind );

// 每分钟
int map_res_logic();

#endif
