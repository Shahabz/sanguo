#ifndef _RANK_H_
#define _RANK_H_

// ÅÅĞĞ°ñ½á¹¹
typedef struct _actor_rank
{
	int city_index;
	int battlepower;
	short level;
	char place;
	char official;
}ActorRank;

int rank_init();
int rank_reset();
int rank_server();
void rank_sort_check( City *pCity );
int rank_server_sendlist( int actor_index, int page, int myrange );
int rank_zone_sendlist( int actor_index, int page, int myrange );
int rank_nation_sendlist( int actor_index, int page, int myrange );
#endif // !_RANK_H_
