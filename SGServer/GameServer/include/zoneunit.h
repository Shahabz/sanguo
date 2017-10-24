#ifndef __ZONEUNIT_H_
#define __ZONEUNIT_H_

#define ZONEUNIT_TYPE_CITY			1	// 城池
#define ZONEUNIT_TYPE_ARMY			2	// 部队
#define ZONEUNIT_TYPE_TOWN			3	// 城镇
#define ZONEUNIT_TYPE_ENEMY			4	// 流寇
#define ZONEUNIT_TYPE_RES			5	// 资源
#define ZONEUNIT_TYPE_EVENT			6	// 事件

// 世界地图显示单元
typedef struct _zoneunit
{
	char type;					// 类型1=City 2=ARMY 3...
	short lastadd_zoneid;		// 上次进入的区域编号
	int index;					// 类型对应索引
	int pre_index;				// 前一个单元的索引
	int next_index;				// 下一个单元的索引
}ZoneUnit;

int zoneunit_init();

// 根据类型和对应索引确定单元索引
int zoneunit_getindex( char type, int index );

// 获取显示单元对应的属性信息
int zoneunit_getattr( int unit_index, SLK_NetS_ZoneUnit *pAttr );

// 获取显示单元对应的位置
int zoneunit_getpos( int unit_index, short *posx, short *posy );

// 将需要显示的城池或军队添加到显示单元
int zoneunit_add( char type, int index );

// 将已经关联的地图单元删除掉
int zoneunit_del( char type, int index, int unit_index );

// 更新地图单元数据
int zoneunit_update( char type, int index, int unit_index );

// 显示单元进入世界地图
int zoneunit_enterworld( int unit_index, short posx, short posy );

#endif
