#ifndef __MAP_H_
#define __MAP_H_

#include "area.h"
#define MAP_SIZEOFFSET 4

#define MAP_W	300
#define MAP_H	300
#define WORLDMAPBLOCKTILEDID	83 // ��ͼ�赲��Ƭ

// һ���������ݽṹ
typedef struct _tile
{
	char nation;			// ������������
	short townid;			// �ĸ�����ķ��䷶Χ
	char unit_type;			// �����ϵ���ʾ��������
	int unit_index;			// �����ϵ���ʾ��������
}Tile;


// �����ͼ
typedef struct _map
{
	int				m_nMaxWidth;	// ��ͼ�Ŀ��(���鵥λ)
	int				m_nMaxHeight;	// ��ͼ�ĸ߶�(���鵥λ)
	short			m_nAreaXNum;
	short			m_nAreaYNum;
	int				m_nAreaMaxCount;
	Area			*m_aArea;
	Tile			**m_aTileData;		// ÿ�����ӵ�����
}Map;

typedef struct _pos
{
	short x;
	short y;
}Pos;

int map_init();
void map_logic();
void map_sendinfo( int actor_index, short tposx, short tposy );
void map_areaenter( int actor_index, int areaindex, short posx, short posy, char areaupdate );

// ��Ӷ���
int map_addobject( int type, int index, short posx, short posy );

// �Ƴ�����
int map_delobject( int type, int index, short posx, short posy );

// ��ȡռ�ظ���
int map_getobject_grid( int type, int index );

// ���ø�����������
int map_tile_setnation( short posx, short posy, int range, short townid, char nation );
char map_tile_getnation( short posx, short posy );
short map_tile_gettownid( short posx, short posy );

// ���һ����ָ���ص�����Ŀ�Ǩ�ǿհ׵� ��������
int map_getcanmovenearest( short *pPosx, short *pPosy );

// ���һ���հ׵�
int map_getrandpos( int grid, short *pPosx, short *pPosy );

// ����ָ����ķ�Χ��ȡһ�����������
int map_getrandpos_withrange( short posx, short posy, int range, short *pPosx, short *pPosy );

// �����ҳǳ�λ��
int map_getrandcitypos( short *pPosx, short *pPosy );

// �������
int map_getrandpos_withtype( int type, short *pPosx, short *pPosy );

// �ж�����ص��Ƿ���Ǩ��
int map_canmove( short posX, short posY );

// ��ת����
int map_goto_withpos( int actor_index, char op, short posx, short posy );

// ����
int map_distance( short fposx, short fposy, short tposx, short tposy );

// dump��ͼ����
void map_tile_dump();
#endif
