#ifndef _MEM_DUMP_H
#define _MEM_DUMP_H

int mem2struct_actor( char *filename, int maxcount );
int mem2struct_city( char *filename, int maxcount );
int troop_resumed( struct _city *pCity );
#endif
