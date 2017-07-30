#ifndef _CITY_TECH_H_
#define _CITY_TECH_H_
#include "define.h"

#define CITY_TECH_MAX 40

// 城池科技研究
int city_tech_upgrade( int actor_index, short kind );
int city_tech_quick( int actor_index );
int city_tech_freequick( int actor_index );
int city_tech_finish( City *pCity, Building *pBuilding );
int city_tech_get( int actor_index );
#endif
