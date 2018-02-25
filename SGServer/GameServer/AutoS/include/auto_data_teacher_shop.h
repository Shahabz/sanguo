#ifndef __TEACHER_SHOP_DATA_AUTO_H
#define __TEACHER_SHOP_DATA_AUTO_H
#include "define.h"

struct _teachershop {
//--Automatically generated
				int id;	//
				short itemkind;	//
				short itemnum;	//
				short showlevel;	//
				short buylevel;	//
				int point;	//
};
typedef struct _teachershop TeacherShop;

int teachershop_init_auto();
int teachershop_reload_auto();

#endif
