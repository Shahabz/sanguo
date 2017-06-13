#ifndef __GLOBAL_NETPROCESS_H
#define __GLOBAL_NETPROCESS_H

#define INTERSERVICE 8
// ¿ç·þÆ¥Åä
typedef struct _inter_service
{
	short serverid;
	char type;
	short target_sevid[INTERSERVICE];
	char target_sevcount;
}InterService;
int inter_service_init();
int inter_service_reload();
short inter_service_getindex();
short inter_service_getserverid();

void global_process_init();

#endif
