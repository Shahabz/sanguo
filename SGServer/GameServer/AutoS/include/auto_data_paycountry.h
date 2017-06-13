#ifndef __PAYCOUNTRY_DATA_AUTO_H
#define __PAYCOUNTRY_DATA_AUTO_H
#include "define.h"

struct _paycountry {
//--Automatically generated
				int country_code;	//
				char country_str[3];	//
				short offset;	//
};
typedef struct _paycountry PayCountry;

int paycountry_init_auto();
int paycountry_reload_auto();
int paycountry_luatable_auto();

#endif
