#ifndef __CITYGUARD_AUTO_H
#define __CITYGUARD_AUTO_H
#include "define.h"

struct _city_guard {
//--Automatically generated
				char monsterid;	//
				char color;	//
				char corps;	//
				unsigned char level;	//
				char shape;	//
				int soldiers;	//
};
typedef struct _city_guard CityGuard;

typedef CityGuard * (*LPCB_GETCITYGUARD)( int actorid, int offset );
int city_guard_load_auto( int actorid, int city_index, LPCB_GETCITYGUARD pCB_GetCityGuard, const char *pTab );
int city_guard_save_auto( int actorid, int offset, CityGuard *pCityGuard, const char *pTab, FILE *fp );
int city_guard_batch_save_auto( int actorid, CityGuard *pCityGuard, int maxcount, const char *pTab, FILE *fp );

#endif
