#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "system.h"
#include "actor.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "map.h"
#include "map_zone.h"
#include "global.h"
#include "mapunit.h"
#include "script_auto.h"
#include "city.h"
#include "world_quest.h"
#include "quest.h"
#include "auto_data_map_zoneinfo.h"
#include "auto_data_map_towninfo.h"

extern Global global;
extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

Map g_map;
int g_nUnitQueueNumLimit;		// �������еļ��ޣ��������ٷ���
int *g_pTmpEnterArmy;
int *g_pTmpLeaveArmy;

//-----------------------------------------------------------------------------
// map_init
// ����˵��: �����ͼ��ʼ��
//-----------------------------------------------------------------------------
int map_init()
{
	g_nUnitQueueNumLimit = 200;

	g_pTmpEnterArmy = (int *)malloc( sizeof(int)*g_nUnitQueueNumLimit );
	g_pTmpLeaveArmy = (int *)malloc( sizeof(int)*g_nUnitQueueNumLimit );

	g_map.m_nMaxWidth = MAP_W;
	g_map.m_nMaxHeight = MAP_H;

	// ��������
	g_map.m_nAreaXNum = (g_map.m_nMaxWidth - 1) / AREA_WIDTH + 1;
	g_map.m_nAreaYNum = (g_map.m_nMaxHeight - 1) / AREA_HEIGHT + 1;
	g_map.m_nAreaMaxCount = g_map.m_nAreaXNum*g_map.m_nAreaYNum;
	g_map.m_aArea = (Area*)malloc( sizeof(Area)*(g_map.m_nAreaMaxCount) );
	memset( g_map.m_aArea, 0, sizeof(Area)*(g_map.m_nAreaMaxCount) );
	for ( int tmpi = 0; tmpi < g_map.m_nAreaMaxCount; tmpi++ )
	{
		g_map.m_aArea[tmpi].unit_head = -1;
		g_map.m_aArea[tmpi].unit_tail = -1;
		area_initqueue( &g_map.m_aArea[tmpi] );
	}


	// �����������
	g_map.m_aTileData = (Tile **)malloc( sizeof( Tile * )* g_map.m_nMaxWidth );
	for ( int tmpi = 0; tmpi < g_map.m_nMaxWidth; tmpi++ )
	{
		g_map.m_aTileData[tmpi] = (Tile *)malloc( sizeof( Tile )* g_map.m_nMaxHeight );
		memset( g_map.m_aTileData[tmpi], 0, sizeof( Tile )*(g_map.m_nMaxHeight) );
	}
	
	for ( int tmpi = 0; tmpi < g_map.m_nMaxWidth; tmpi++ )
	{
		for ( int tmpj = 0; tmpj < g_map.m_nMaxHeight; tmpj++ )
		{
			g_map.m_aTileData[tmpi][tmpj].nation = -1;
			g_map.m_aTileData[tmpi][tmpj].townid = 0;
			g_map.m_aTileData[tmpi][tmpj].unit_type = 0;
			g_map.m_aTileData[tmpi][tmpj].unit_index = -1;
		}
	}

	// �����ͼ�ű�
	sc_OnWorldMapInit( g_map.m_nMaxWidth, g_map.m_nMaxHeight );
	return 0; 
}

// ��ͼ֡�߼�
void map_logic()
{
	for ( int tmpi = 0; tmpi < g_map.m_nAreaXNum*g_map.m_nAreaYNum; tmpi++ )
	{
		area_logic( tmpi );
	}
}

// �����ͼ������Ϣ
void map_sendinfo( int actor_index, short tposx, short tposy )
{
	SLK_NetS_WorldMapInfo info = { 0 };
	info.m_area_width = AREA_WIDTH;
	info.m_area_height = AREA_HEIGHT;
	info.m_map_area_xnum = g_map.m_nAreaXNum;
	info.m_map_area_ynum = g_map.m_nAreaYNum;
	info.m_target_posx = tposx;
	info.m_target_posy = tposy;
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		// �������ҳǳ�λ����У��
		short posx = -1, posy = -1;
		mapunit_getpos( pCity->unit_index, &posx, &posy );
		if ( posx != pCity->posx || posy != pCity->posy )
		{// ����λ�ã����¼�������
			write_gamelog( "CityPosError: city(%d,%d,%d) unit(%d,%d,%d)", pCity->index, pCity->posx, pCity->posy, pCity->unit_index, posx, posy );
			mapunit_del( MAPUNIT_TYPE_CITY, pCity->index, pCity->unit_index );
			pCity->unit_index = mapunit_add( MAPUNIT_TYPE_CITY, pCity->index );
		}
		
		info.m_citystate = pCity->state;
		info.m_my_city_unit_index = pCity->unit_index;
		info.m_my_city_posx = pCity->posx;
		info.m_my_city_posy = pCity->posy;
	}
	netsend_worldmapinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// ������Ϣ
void map_areaenter( int actor_index, int areaindex, short posx, short posy, char areaupdate )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( areaindex >= g_map.m_nAreaMaxCount )
		return;
	if ( g_actors[actor_index].view_areaindex != areaindex )
	{
		map_zone_change( actor_index, posx, posy, areaupdate );
		if ( areaupdate == 1 )
		{
			view_area_change( actor_index, areaindex );
			if ( areaindex < 0 )
				g_actors[actor_index].view_areaindex = -1;
			else
				g_actors[actor_index].view_areaindex = areaindex;
		}
	}
}

// ��ȡռ�ظ���
inline int map_getobject_grid( int type, int index )
{
	int grid = 1;
	switch ( type )
	{
	//case MAPUNIT_TYPE_CITY:	// ��ҳǳ�
	//	grid = 1;
	//	break;
	case MAPUNIT_TYPE_TOWN://  ����
	{
		if ( index <= 0 || index >= g_towninfo_maxnum )
			return 1;
		grid = g_towninfo[index].grid;
	}
		break;
	default:
		return 1;
	}
	return grid;
}

// ��Ӷ���
int map_addobject( int type, int index, short posx, short posy )
{
	if ( posx < 0 || posy < 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxHeight )
	{
		return -1;
	}
	int grid = map_getobject_grid( type, index );
	switch ( grid )
	{
	// ռ3*3����
	case 3:
		if ( posx <= 0 || posy <= 0 || posx >= g_map.m_nMaxWidth-1 || posy >= g_map.m_nMaxHeight-1 )
		{
			return -1;
		}
		g_map.m_aTileData[posx][posy].unit_type = type;
		g_map.m_aTileData[posx][posy].unit_index = index;
		g_map.m_aTileData[posx - 1][posy].unit_type = type;
		g_map.m_aTileData[posx - 1][posy].unit_index = index;
		g_map.m_aTileData[posx - 1][posy - 1].unit_type = type;
		g_map.m_aTileData[posx - 1][posy - 1].unit_index = index;
		g_map.m_aTileData[posx][posy - 1].unit_type = type;
		g_map.m_aTileData[posx][posy - 1].unit_index = index;
		g_map.m_aTileData[posx + 1][posy - 1].unit_type = type;
		g_map.m_aTileData[posx + 1][posy - 1].unit_index = index;
		g_map.m_aTileData[posx + 1][posy].unit_type = type;
		g_map.m_aTileData[posx + 1][posy].unit_index = index;
		g_map.m_aTileData[posx + 1][posy + 1].unit_type = type;
		g_map.m_aTileData[posx + 1][posy + 1].unit_index = index;
		g_map.m_aTileData[posx][posy + 1].unit_type = type;
		g_map.m_aTileData[posx][posy + 1].unit_index = index;
		g_map.m_aTileData[posx - 1][posy + 1].unit_type = type;
		g_map.m_aTileData[posx - 1][posy + 1].unit_index = index;
		break;
	// ռ2*2������
	case 2:
		if ( posy <= 0 || posx >= g_map.m_nMaxWidth - 1 )
		{
			return -1;
		}
		if ( g_map.m_aTileData[posx][posy].unit_type		> 0 ||
			g_map.m_aTileData[posx][posy - 1].unit_type		> 0 ||
			g_map.m_aTileData[posx + 1][posy - 1].unit_type	> 0 ||
			g_map.m_aTileData[posx + 1][posy].unit_type		> 0 )
		{
			return -1;
		}
		g_map.m_aTileData[posx][posy].unit_type = type;
		g_map.m_aTileData[posx][posy].unit_index = index;
		g_map.m_aTileData[posx][posy - 1].unit_type = type;
		g_map.m_aTileData[posx][posy - 1].unit_index = index;
		g_map.m_aTileData[posx + 1][posy - 1].unit_type = type;
		g_map.m_aTileData[posx + 1][posy - 1].unit_index = index;
		g_map.m_aTileData[posx + 1][posy].unit_type = type;
		g_map.m_aTileData[posx + 1][posy].unit_index = index;
		break;

	// ռ1*1������
	case 1:
		if ( g_map.m_aTileData[posx][posy].unit_type > 0 )
		{
			return -1;
		}
		g_map.m_aTileData[posx][posy].unit_type = type;
		g_map.m_aTileData[posx][posy].unit_index = index;
		break;
	default:
		break;
	}
	return 0;
}

// �Ƴ�����
int map_delobject( int type, int index, short posx, short posy )
{
	if ( posx < 0 || posy < 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxHeight )
	{
		return -1;
	}
	int grid = map_getobject_grid( type, index );
	switch ( grid )
	{
		// ռ3*3����
	case 3:
		if ( posx <= 0 || posy <= 0 || posx >= g_map.m_nMaxWidth - 1 || posy >= g_map.m_nMaxHeight - 1 )
		{
			return -1;
		}
		g_map.m_aTileData[posx][posy].unit_type = 0;
		g_map.m_aTileData[posx][posy].unit_index = -1;
		g_map.m_aTileData[posx - 1][posy].unit_type = 0;
		g_map.m_aTileData[posx - 1][posy].unit_index = -1;
		g_map.m_aTileData[posx - 1][posy - 1].unit_type = 0;
		g_map.m_aTileData[posx - 1][posy - 1].unit_index = -1;
		g_map.m_aTileData[posx][posy - 1].unit_type = 0;
		g_map.m_aTileData[posx][posy - 1].unit_index = -1;
		g_map.m_aTileData[posx + 1][posy - 1].unit_type = 0;
		g_map.m_aTileData[posx + 1][posy - 1].unit_index = -1;
		g_map.m_aTileData[posx + 1][posy].unit_type = 0;
		g_map.m_aTileData[posx + 1][posy].unit_index = -1;
		g_map.m_aTileData[posx + 1][posy + 1].unit_type = 0;
		g_map.m_aTileData[posx + 1][posy + 1].unit_index = -1;
		g_map.m_aTileData[posx][posy + 1].unit_type = 0;
		g_map.m_aTileData[posx][posy + 1].unit_index = -1;
		g_map.m_aTileData[posx - 1][posy + 1].unit_type = 0;
		g_map.m_aTileData[posx - 1][posy + 1].unit_index = -1;
		break;

	// ռ2*2������
	case 2: 
		if ( posy <= 0 || posx >= g_map.m_nMaxWidth - 1 )
		{
			return -1;
		}
		g_map.m_aTileData[posx][posy].unit_type = 0;
		g_map.m_aTileData[posx][posy].unit_index = -1;
		g_map.m_aTileData[posx][posy - 1].unit_type = 0;
		g_map.m_aTileData[posx][posy - 1].unit_index = -1;
		g_map.m_aTileData[posx + 1][posy - 1].unit_type = 0;
		g_map.m_aTileData[posx + 1][posy - 1].unit_index = -1;
		g_map.m_aTileData[posx + 1][posy].unit_type = 0;
		g_map.m_aTileData[posx + 1][posy].unit_index = -1;
		break;

	// ռ1*1������
	case 1:
		g_map.m_aTileData[posx][posy].unit_type = 0;
		g_map.m_aTileData[posx][posy].unit_index = -1;
		break;
	default:
		break;
	}
	return 0;
}

// ���ø�����������
int map_tile_setnation( short posx, short posy, int range, short townid, char nation )
{
	if ( range == 3 )
	{
		for ( int tmpi = -1; tmpi <= 1; tmpi++ )
		{
			for ( int tmpj = -1; tmpj <= 1; tmpj++ )
			{
				short x = posx + tmpi;
				short y = posy + tmpj;
				if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
					continue;
				g_map.m_aTileData[x][y].nation = nation;
				g_map.m_aTileData[x][y].townid = townid;
			}
		}
	}
	else if ( range == 4 )
	{
		for ( int tmpi = -1; tmpi <= 2; tmpi++ )
		{
			for ( int tmpj = -2; tmpj <= 1; tmpj++ )
			{
				short x = posx + tmpi;
				short y = posy + tmpj;
				if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
					continue;
				g_map.m_aTileData[x][y].nation = nation;
				g_map.m_aTileData[x][y].townid = townid;
			}
		}
	}
	else if ( range == 20 )
	{
		for ( int tmpi = -10; tmpi <= 9; tmpi++ )
		{
			for ( int tmpj = -10; tmpj <= 9; tmpj++ )
			{
				short x = posx + tmpi;
				short y = posy + tmpj;
				if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
					continue;
				g_map.m_aTileData[x][y].nation = nation;
				g_map.m_aTileData[x][y].townid = townid;
			}
		}
	}
	return 0;
}
char map_tile_getnation( short posx, short posy )
{
	if ( posx < 0 || posy < 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxHeight )
		return -1;
	return g_map.m_aTileData[posx][posy].nation;
}

short map_tile_gettownid( short posx, short posy )
{
	if ( posx < 0 || posy < 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxHeight )
		return -1;
	return g_map.m_aTileData[posx][posy].townid;
}

// �ж�����ص��Ƿ���Ǩ��
int map_canmove( short posX, short posY )
{
	if( posX < 0 || posY < 0 || posX >= g_map.m_nMaxWidth || posY >= g_map.m_nMaxWidth )
	{
		return 0;
	}

	// ����µر�
	if ( g_map.m_aTileData[posX][posY].unit_type == 0 )
	{
		if ( g_map.m_aTileData[posX][posY].unit_index >= WORLDMAPBLOCKTILEDID )
			return 0;
		else
			return 1;
	}

	return 0;
}

// ���һ����ָ���ص�����Ŀ�Ǩ�ǿհ׵� ��������
int map_getcanmovenearest( short *pPosx, short *pPosy )
{
	if( *pPosx < 0 || *pPosy < 0 || *pPosx >= g_map.m_nMaxWidth || *pPosy >= g_map.m_nMaxWidth )
	{
		return -1;
	}

	/* ����������˵���һȦ�������Ͽ�ʼ*/
	// ����
	short stepX = 3;
	short stepY = 4;

	// ������������
	short stepXForward = 1;
	short stepYForward = -1;

	// ���
	short posX = *pPosx - 1;
	short posY = *pPosy + 2;
	// ��������
	if ( map_canmove( posX, posY ) )
	{
		*pPosx = posX;
		*pPosy = posY;
		return 0;
	}

	// ���գ�������1000������
	short count = 0;

	while( count < 1000 )
	{
		// ����X����
		for( int i = 0; i < stepX; i++ )
		{
			posX += stepXForward;
			if ( map_canmove( posX, posY ) )
			{
				*pPosx = posX;
				*pPosy = posY;
				return 0;
			}
		}
		stepXForward *= -1;

		// ����Y����
		for( int i = 0; i < stepY; i++ )
		{
			posY += stepYForward;
			if ( map_canmove( posX, posY ) )
			{
				*pPosx = posX;
				*pPosy = posY;
				return 0;
			}
		}
		stepYForward *= -1;

		// ѭ��һ������һ�β���
		stepX++;
		stepY++;

		// ��ֹ��ѭ��
		count += 1;
	}

	return -1;
}

// ���һ���հ׵�
int map_getrandpos( int grid, short *pPosx, short *pPosy )
{
	*pPosx = rand() % (g_map.m_nMaxWidth - MAP_SIZEOFFSET) + 1;
	*pPosy = rand() % (g_map.m_nMaxHeight - MAP_SIZEOFFSET) + 1;

	switch ( grid )
	{
		// ռ3*3����
	case 3:
		if ( g_map.m_aTileData[*pPosx][*pPosy].unit_type == 0 &&
			g_map.m_aTileData[*pPosx - 1][*pPosy].unit_type == 0 &&
			g_map.m_aTileData[*pPosx - 1][*pPosy - 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx][*pPosy - 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx + 1][*pPosy - 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx + 1][*pPosy].unit_type == 0 &&
			g_map.m_aTileData[*pPosx + 1][*pPosy + 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx][*pPosy + 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx - 1][*pPosy + 1].unit_type == 0 )
		{
			return 0;
		}
		else
		{
			while ( g_map.m_aTileData[*pPosx][*pPosy].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx - 1][*pPosy].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx - 1][*pPosy - 1].unit_type	> 0 ||
				g_map.m_aTileData[*pPosx][*pPosy - 1].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx + 1][*pPosy - 1].unit_type	> 0 ||
				g_map.m_aTileData[*pPosx + 1][*pPosy].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx + 1][*pPosy + 1].unit_type	> 0 ||
				g_map.m_aTileData[*pPosx][*pPosy + 1].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx - 1][*pPosy + 1].unit_type	> 0 )
			{
				*pPosx = rand() % (g_map.m_nMaxWidth - MAP_SIZEOFFSET) + 1;
				*pPosy = rand() % (g_map.m_nMaxHeight - MAP_SIZEOFFSET) + 1;
			}
			return 0;
		}
	// ռ2*2������
	case 2:
		if ( g_map.m_aTileData[*pPosx][*pPosy].unit_type == 0 &&
			g_map.m_aTileData[*pPosx][*pPosy - 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx + 1][*pPosy - 1].unit_type == 0 &&
			g_map.m_aTileData[*pPosx + 1][*pPosy].unit_type == 0 )
		{
			return 0;
		}
		else
		{
			while ( g_map.m_aTileData[*pPosx][*pPosy].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx][*pPosy - 1].unit_type		> 0 ||
				g_map.m_aTileData[*pPosx + 1][*pPosy - 1].unit_type	> 0 ||
				g_map.m_aTileData[*pPosx + 1][*pPosy].unit_type		> 0 )
			{
				*pPosx = rand() % (g_map.m_nMaxWidth - MAP_SIZEOFFSET) + 1;
				*pPosy = rand() % (g_map.m_nMaxHeight - MAP_SIZEOFFSET) + 1;
			}
			return 0;
		}
		break;

	// ռ1*1������
	case 1:
		if ( g_map.m_aTileData[*pPosx][*pPosy].unit_type == 0 )
		{
			return 0;
		}
		else
		{
			while ( g_map.m_aTileData[*pPosx][*pPosy].unit_type > 0 )
			{
				*pPosx = rand() % (g_map.m_nMaxWidth - MAP_SIZEOFFSET) + 1;
				*pPosy = rand() % (g_map.m_nMaxHeight - MAP_SIZEOFFSET) + 1;
			}
			return 0;
		}
		break;
	default:
		break;
	}

	return -1;
}

// ����ָ����ķ�Χ��ȡһ�����������1*1��
int map_getrandpos_withrange( short posx, short posy, int range, short *pPosx, short *pPosy )
{
	short findlistx[512] = { 0 };
	short findlisty[512] = { 0 };
	short offset = 0;
	int zoneid = map_zone_getid( posx, posy );
	for ( int tmpi = -range; tmpi <= range; tmpi++ )
	{
		for ( int tmpj = -range; tmpj <= range; tmpj++ )
		{
			short x = posx + tmpi;
			short y = posy + tmpj;
			if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
				continue;
			if ( g_map.m_aTileData[x][y].unit_type > 0 )
				continue;
			if ( map_zone_getid( x, y ) != zoneid )
				continue;
			//if ( map_addobject( type, x, y ) < 0 )
			//	continue;
			// �ҵ����еĿ����
			findlistx[offset] = x;
			findlisty[offset] = y;
			offset += 1;
			if ( offset >= 500 )
				break;
		}
		if ( offset >= 500 )
			break;
	}

	if ( offset > 0 )
	{
		int index = rand() % offset;
		*pPosx = findlistx[index];
		*pPosy = findlisty[index];
	}
	else
	{
		*pPosx = -1;
		*pPosy = -1;
	}
	return 0;
}

// �����ҳǳ�λ��
int map_getrandcitypos( short *pPosx, short *pPosy )
{
	short zoneidlist[8] = { 1, 2, 3, 4, 6, 7, 8, 9 };
	int openday = system_getopentime() / 86400;

	// ���ҵ�һ�������ݣ�Ȼ����������������λ�������1�����Ȼû��λ�ã���ô���������һ��һ���ң�������ˣ�����û�ҵ�����ô����һ������
	int step = 0;
	while ( true )
	{
		short zoneid = 0;
		zoneid = zoneidlist[random( 0, 7 )];
		if ( zoneid >= g_zoneinfo_maxnum )
		{
			continue;
		}

		short findlistx[10001] = { 0 };
		short findlisty[10001] = { 0 };
		short offset = 0;
		short beginx,beginy;
		short endx, endy;
		for ( int tmpi = g_zoneinfo[zoneid].top_left_posx; tmpi <= g_zoneinfo[zoneid].bottom_right_posx; tmpi++ )
		{
			for ( int tmpj = g_zoneinfo[zoneid].top_left_posy; tmpj <= g_zoneinfo[zoneid].bottom_right_posy; tmpj++ )
			{
				short x = tmpi;
				short y = tmpj;
				if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
					continue;
				if ( g_map.m_aTileData[x][y].unit_type > 0 )
					continue;
				// �ҵ����еĿ����
				findlistx[offset] = x;
				findlisty[offset] = y;
				offset += 1;
				if ( offset >= 10000 )
					break;
			}
			if ( offset >= 10000 )
				break;
		}

		if ( offset > 0 )
		{
			int index = rand() % offset;
			*pPosx = findlistx[index];
			*pPosy = findlisty[index];
		}
		else
		{
			*pPosx = -1;
			*pPosy = -1;
		}
		if ( *pPosx >= 0 && *pPosy >= 0 )
		{
			break;
		}
		step += 1;
		if ( step >= 8 )
		{// ����
			*pPosx = 0;
			*pPosy = 0;
			break;
		}
	}
	
	return 0;
}

// �������
int map_getrandpos_withtype( int type, short *pPosx, short *pPosy )
{
	static short s_zoneidlist0[8] = { 1, 2, 3, 4, 6, 7, 8, 9 };
	static short s_zoneidlist1[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	short zoneid = 0;
	if ( type == 0 )
	{
		zoneid = s_zoneidlist0[random( 0, 7 )];
	}
	else if ( type == 1 )
	{
		zoneid = s_zoneidlist1[random( 0, 8 )];
	}
	else if ( type == 2 )
	{
		zoneid = 5;
	}
	if ( zoneid == 0 )
		return -1;
	map_zone_randpos( zoneid, pPosx, pPosy, 4096 );
	return 0;
}

int map_goto_withpos( int actor_index, char op, short posx, short posy )
{
	if ( posx <= 0 || posy <= 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxHeight )
		return -1;
	SLK_NetS_GotoAsyn pValue = { 0 };
	pValue.m_op = op;
	pValue.m_posx = posx;
	pValue.m_posy = posy;
	pValue.m_type = g_map.m_aTileData[posx][posy].unit_type;
	pValue.m_unit_index = g_map.m_aTileData[posx][posy].unit_index;
	netsend_gotoasyn_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

bool ptInLine( Pos point, Pos lineStartPoint, Pos lineEndPoint, double fTolerance )
{
	double L, R, S;
	bool bResult = false;

	L = sqrt( (double)((lineEndPoint.x - lineStartPoint.x)*(lineEndPoint.x - lineStartPoint.x) + (lineEndPoint.y - lineStartPoint.y)*(lineEndPoint.y - lineStartPoint.y)) );
	if ( L != 0 )
	{
		R = ((lineStartPoint.y - point.y)*(lineStartPoint.y - lineEndPoint.y) - (lineStartPoint.x - point.x)*(lineEndPoint.x - lineStartPoint.x)) / (L*L);
		S = ((lineStartPoint.y - point.y)*(lineEndPoint.x - lineStartPoint.x) - (lineStartPoint.x - point.x)*(lineEndPoint.y - lineStartPoint.y)) / (L*L);
		if ( R > 0 && R < 1 )
		{
			if ( abs( (int)(S*L) ) <= fTolerance )
				bResult = true;
		}
	}

	return bResult;
}

// ����
int map_distance( short fposx, short fposy, short tposx, short tposy )
{
	return (int)sqrt( (float)((fposx - tposx)*(fposx - tposx)) + (float)((fposy - tposy)*(fposy - tposy)) );
}

// dump��ͼ����
void map_tile_dump()
{
	FILE *fp = fopen( "./map_tile.txt", "wb" );
	if ( !fp )
		return;
	for ( short posy = 0; posy < g_map.m_nMaxHeight; posy++ )
	{
		for ( short posx = 0; posx < g_map.m_nMaxWidth; posx++ )
		{
			fprintf( fp, "%d\t", g_map.m_aTileData[posx][posy].unit_type );
		}
		fprintf( fp, "\n" );
	}
	fclose( fp );
}
