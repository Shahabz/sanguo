#ifndef __AREA_H
#define __AREA_H

#define MAX_AREAMSGSIZE	2048

#define AREA_WIDTH		7
#define AREA_HEIGHT		7

// 世界地图一个区域结构
typedef struct _area
{
	int unit_head;
	int unit_tail;
	short sendcount;
	int pos;							// 确定那个消息需要发送
    char actor_msg[2][MAX_AREAMSGSIZE];	// 当前要发送的消息缓冲,一个是背面缓冲
	int actor_size[2];

	int *unit_queue;
	int nMaxUnitQueueNum;
}Area;
typedef void( *LPCALLBACK_UNITACTION )( int army_index, int areax, int areay );
typedef void (*p_actor_func)(int actor_index, void *data);

int area_initqueue( Area *pArea );
int area_addtoqueue( int area_index, int army_index );
int area_delfromqueue( int area_index, int army_index );
void area_effect( int army_index, int posx1, int posy1, int posx2, int posy2, LPCALLBACK_UNITACTION pUnitAction );

// 区域逻辑，由地图驱动
void area_logic( int area_index );

// 获取区域格子
int area_getoffset( int area_index, int *px, int *py );

// 根据区域格子获取区域编号
int area_getindex_fromgrid( int areax, int areay );

// 根据世界坐标获取所在区域编号
int area_getindex( int posx, int posy );

// 将显示单元添加到区域
int area_addmapunit( int unit_index, int area_index );

// 将显示单元移除出区域
int area_delmapunit( int unit_index, int area_index );

// 一个玩家移动屏幕进入到区域需要获得区域中的数据
int area_enter( int actor_index, int area_index );

// 一个玩家移动屏幕离开区域需要获得区域离开的数据
int area_leave( int actor_index, int area_index );

// 显示单元位置变化，要更新到区域
int mapunit_area_change( int unit_index, int new_posx, int new_posy, char flag );

// 视口变化，区域信息更新到视口
int view_area_change( int unit_index, int new_area_index );

int area_clearmsg( int area_index );
int area_sendunitinfo( int client_index, int posx, int posy );
int area_sendmsg(int area_index, int datasize, char *databuf);
int area_send(int area_index, int datasize, char *databuf);

#endif
