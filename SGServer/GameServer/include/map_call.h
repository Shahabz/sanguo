#ifndef _MAP_CALL_H_
#define _MAP_CALL_H_
#include "define.h"

int map_call_load();
int map_call_save( FILE *fp );
MapCall *map_call_getptr( int index );

void map_call_logic();
void map_call_delete( int index );
int map_call( int actor_index, int level );
int map_call_notify( int actor_index );
int map_call_response( int actor_index, int index );
int map_call_sendinfo( int actor_index );
#endif
