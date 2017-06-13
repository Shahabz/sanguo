#ifndef __ITEM_DATA_AUTO_H
#define __ITEM_DATA_AUTO_H
#include "define.h"

struct _itemkind {
//--Automatically generated
				int m_kind;	//
				char m_name[32];	//
				short m_level;	//
				int m_price;	//
				short m_type;	//
				short m_situation;	//
				short m_overlap;	//
				short m_base_ability[2];	//
				int m_base_value[2];	//
				short m_ability[4];	//
				int m_value_min[4];	//
				int m_value_max[4];	//
				short m_color_level;	//
};
typedef struct _itemkind ItemKind;

int itemkind_init_auto();
int itemkind_reload_auto();
int itemkind_luatable_auto();

#endif
