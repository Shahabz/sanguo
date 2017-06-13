#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "define.h"
#include "dserver.h"
#include "area.h"
#include "actor.h"
extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
int mem2struct_actor( char *filename, int maxcount )
{
	//char *pBuf = u_load_binary( filename );
	//if ( pBuf == NULL )
	//	return -1;
	//FILE *fp = fopen( "actor.sql", "wb" );
	//if ( fp == NULL )
	//	return -2;

	//if ( db_init() < 0 )
	//{
	//	return -3;
	//}

	//Actor *pActor = (Actor*)malloc( sizeof(Actor)*maxcount );
	//memcpy( pActor, pBuf, sizeof(Actor)*maxcount );

	//for ( int actor_index = 0; actor_index < maxcount; actor_index++ )
	//{
	//	if ( pActor[actor_index].actorid > 0 )
	//	{
	//		printf( "pActor->actorid:%d\n", pActor[actor_index].actorid );

	//		actor_save_auto( &pActor[actor_index], "actor", fp );
	//		actor_quest_save_auto( pActor[actor_index].actorid, &pActor[actor_index].quest, "actor_quest", fp );

	//		for ( int tmpi = 0; tmpi < MAX_DEFAULT_ITEMNUM; tmpi++ )
	//		{
	//			if ( pActor[actor_index].item[tmpi].m_kind <= 0 )
	//				continue;
	//			pActor[actor_index].item[tmpi].actorid = pActor[actor_index].actorid;
	//			pActor[actor_index].item[tmpi].offset = tmpi;
	//			actor_item_save_auto( &pActor[actor_index].item[tmpi], "actor_item", fp );
	//		}
	//		for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM; tmpi++ )
	//		{
	//			if ( pActor[actor_index].equip[tmpi].m_kind <= 0 )
	//				continue;
	//			pActor[actor_index].equip[tmpi].actorid = pActor[actor_index].actorid;
	//			pActor[actor_index].equip[tmpi].offset = tmpi + EQUIP_OFFSETBASE;
	//			actor_item_save_auto( &pActor[actor_index].equip[tmpi], "actor_item", fp );
	//		}
	//	}
	//}

	//if ( pBuf )
	//{
	//	free( pBuf );
	//	pBuf = NULL;
	//}
	//fclose( fp );
	//db_closedata();
	//db_closegame();
	return 0;
}

int mem2struct_city( char *filename, int maxcount )
{
	/*char *pBuf = u_load_binary( filename );
	if ( pBuf == NULL )
		return -1;
	FILE *fp = fopen( "city.sql", "wb" );
	if ( fp == NULL )
		return -2;

	if ( db_init() < 0 )
	{
		return -3;
	}

	City *pCity = (City*)malloc( sizeof(City)*maxcount );
	memcpy( pCity, pBuf, sizeof(City)*maxcount );

	for ( int city_index = 0; city_index < maxcount; city_index++ )
	{
		if ( pCity[city_index].cityid > 0 )
		{
			city_single_save( &pCity[city_index], fp );
		}
	}

	if ( pBuf )
	{
		free( pBuf );
		pBuf = NULL;
	}
	fclose( fp );
	db_closedata();
	db_closegame();*/
	return 0;
}
