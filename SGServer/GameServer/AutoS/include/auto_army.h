#ifndef __ARMY_AUTO_H
#define __ARMY_AUTO_H
#include "define.h"

struct _army {
//--Automatically generated
				int index;	//部队索引
				int id;	//部队唯一ID,合法性验证
				int unit_index;	//显示索引
				char state;	//状态
				int statetime;	//状态时间戳
				int stateduration;	//状态持续多少秒
				int action;	//行为
				short from_type;	//出发的对象类型
				int from_id;	//出发的对象ID
				int from_index;	//出发的对象索引
				short from_posx;	//出发的对象坐标
				short from_posy;	//出发的对象坐标
				short to_type;	//目的对象类型
				int to_id;	//目的对象ID
				int to_index;	//目的对象索引
				short to_posx;	//目的对象坐标
				short to_posy;	//目的对象坐标
				short posx;	//当前坐标
				short posy;	//当前坐标
				short herokind[4];	//武将
};
typedef struct _army Army;

typedef Army * (*LPCB_GETARMY)( int index );
typedef int (*LPCB_LOADARMY)( int index );
int army_load_auto( LPCB_GETARMY pCB_GetArmy, LPCB_LOADARMY pCB_LoadArmy, const char *pTab );
int army_save_auto( Army *pArmy, const char *pTab, FILE *fp );
int army_batch_save_auto( Army *pArmy, int maxcount,  const char *pTab, FILE *fp );

#endif
