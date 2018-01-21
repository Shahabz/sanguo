#ifndef _MAP_CALL_H_
#define _MAP_CALL_H_
#include "define.h"

int map_call_load();
int map_call_save( FILE *fp );
MapCall *map_call_getptr( int index );

// µÿÕº’ŸªΩ
int map_call( int actor_index, int type );

#endif
