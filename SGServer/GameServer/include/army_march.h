#ifndef _ARMY_MARCH_H_
#define _ARMY_MARCH_H_
#include "define.h"

// 计算部队行军所需时间
int army_marchtime_calc( City *pCity, short from_posx, short from_posy, short to_posx, short to_posy );
int army_march_time( int army_index );

// 部队行军
void army_march( int army_index );

// 部队行军返回
void army_reback( int army_index );

// 到达指定地点
void army_arrived( int army_index );

// 添加行军路线
int army_marchroute_add( int army_index );

// 删除行军路线
int army_marchroute_del( int army_index );

// 更新部队速度
int army_marchspeed_update( int army_index );

// 添加部队信息到区域
int army_enterinfo( int army_index, char *databuf, int *psize );
int army_leaveinfo( int army_index, char *databuf, int *psize );

// 操作返回
int actor_army_return( int actor_index, int army_index, int unit_index );

// 操作召回
int actor_army_callback( int actor_index, int army_index, int itemkind );

// 操作行军加速
int actor_army_march_speedup( int actor_index, int army_index, int itemkind );

// 部队援助 遣返
int actor_helparmy_repatriate( int actor_index, int army_index );

#endif