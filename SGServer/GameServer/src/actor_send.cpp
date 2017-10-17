#include "actor.h"
#include "actor_send.h"
#include "gameproc.h"
#include "server_netsend_auto.h"
#include "area.h"
#include "city.h"

extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;

int zone_sendmsg( int zone, int datasize, char *databuf )
{
	if ( datasize > 0 )
	{
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid <= 0 )
				continue;
			City *pCity = city_getptr( tmpi );
			if ( !pCity )
				continue;
			if ( pCity->zone == zone )
			{
				sendtoclient( tmpi, databuf, datasize + sizeof( short ) );
			}
		}
	}
	return 0;
}

// 指定区域的国家
int nation_sendmsg( int zone, int nation, int datasize, char *databuf )
{
	if ( datasize > 0 )
	{
		nation = nation - SENDTYPE_NATION;
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid <= 0 )
				continue;
			City *pCity = city_getptr( tmpi );
			if ( !pCity )
				continue;

			if ( zone > 0 )
			{// 指定区域了
				if ( pCity->zone == zone && pCity->nation == nation )
				{
					sendtoclient( tmpi, databuf, datasize + sizeof( short ) );
				}
			}
			else if ( pCity->nation == nation )
			{
				sendtoclient( tmpi, databuf, datasize + sizeof( short ) );
			}
		}
	}
	return 0;
}

// 功能同上，过略掉城内的，只发在野外的玩家
int nation_worldmap_sendmsg( int zone, int nation, int datasize, char *databuf )
{
	if ( datasize > 0 )
	{
		nation = nation - SENDTYPE_WORLDMAP_NATION;
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid <= 0 )
				continue;
			if ( g_actors[tmpi].view_areaindex < 0 )
				continue;
			City *pCity = city_getptr( tmpi );
			if ( !pCity )
				continue;

			if ( zone > 0 )
			{// 指定区域了
				if ( pCity->zone == zone && pCity->nation == nation )
				{
					sendtoclient( tmpi, databuf, datasize + sizeof( short ) );
				}
			}
			else if ( pCity->nation == nation )
			{
				sendtoclient( tmpi, databuf, datasize + sizeof( short ) );
			}
		}
	}
	return 0;
}

// 世界信息发送,信息立即发送
int world_sendmsg( int datasize, char *databuf )
{
	if ( datasize > 0 )
	{
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
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
	case SENDTYPE_AREA:
		{
			area_sendmsg( actor_index, datasize, data + sizeof(short) );
			//area_clearmsg( actor_index );
		}
		break;
	case SENDTYPE_ZONE:
		zone_sendmsg( actor_index, datasize, data );
		break;
	case SENDTYPE_WORLD:
		world_sendmsg( datasize, data );
		break;
	case SENDTYPE_NATION1:
	case SENDTYPE_NATION2:
	case SENDTYPE_NATION3:
		nation_sendmsg( actor_index, send_type, datasize, data );
		break;
	case SENDTYPE_WORLDMAP_NATION1:
	case SENDTYPE_WORLDMAP_NATION2:
	case SENDTYPE_WORLDMAP_NATION3:
		nation_worldmap_sendmsg( actor_index, send_type, datasize, data );
		break;
	}
	return 0;
}
