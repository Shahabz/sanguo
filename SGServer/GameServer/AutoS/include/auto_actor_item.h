#ifndef __ITEM_AUTO_H
#define __ITEM_AUTO_H
#include "define.h"

struct _actor_item {
//--Automatically generated
				i64 m_itemid;	//物品id
				int actorid;	//角色ID
				short offset;	//物品所在位置
				int m_kind;	//物品种类
				short m_num;	//物品个数
				short m_ability[4];	//属性
				int m_value[4];	//属性值
				char m_color_level;	//颜色等级
};
typedef struct _actor_item Item;

typedef Item * (*LPCB_GETITEM)( int actorid, int offset );
int actor_item_load_auto( int actorid, int actor_index, LPCB_GETITEM pCB_GetItem, char *pTab );
int actor_item_save_auto( Item *pItem, char *pTab, FILE *fp );

#endif
