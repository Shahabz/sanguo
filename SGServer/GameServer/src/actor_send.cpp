#include "actor.h"
#include "actor_send.h"
#include "gameproc.h"
#include "server_netsend_auto.h"
#include "area.h"

extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;


// 世界信息发送,信息立即发送
int world_sendmsg( int datasize, char *databuf )
{
	int tmpi;
	if ( datasize > 0 )
	{
		for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid > 0 )
				sendtoclient( tmpi, databuf, datasize + sizeof(short) );
		}
	}
	return 0;
}

int actor_senddata( int actor_index, char send_type, char *data, int datasize )
{
	switch( send_type )
	{
	case SENDTYPE_ACTOR:
		sendtoclient( actor_index, data, datasize+sizeof(short) );
		break;
	case SENDTYPE_FIGHT:
		break;
	case SENDTYPE_TEAM:
		break;
	case SENDTYPE_FIGHTAREA:
		break;
	case SENDTYPE_AREA:
		{
			area_sendmsg( actor_index, datasize, data + sizeof(short) );
			//area_clearmsg( actor_index );
		}
		break;
	case SENDTYPE_NPCAREA:
		break;
	case SENDTYPE_MAP:
		break;
	case SENDTYPE_WORLD:
		world_sendmsg( datasize, data );
		break;
	}
	return 0;
}
