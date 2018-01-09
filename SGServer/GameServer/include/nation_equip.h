#ifndef _NATION_EQUIP_H_
#define _NATION_EQUIP_H_
#include "define.h"

int nation_equip_open( int actor_index, int kind );
int nation_equip_upgrade( int actor_index, int kind );
int nation_equip_remake( int actor_index, int kind );
void nation_equip_over( City *pCity, int index );
int nation_equip_sendinfo( int actor_index, int kind );
int nation_equip_sendlist( int actor_index );
#endif
