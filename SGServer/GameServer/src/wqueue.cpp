#include <string.h>
#include "netfunc.h"
#include "client.h"
#include "wqueue.h"
#include "utils.h"
#include "script_auto.h"
#include "map_zone.h"
#include "activity.h"

extern char g_open_town3;
extern char g_open_town6;
extern char g_open_townking;

// ����
SWQueue *g_wqueue[2];
int g_nWQueueHead[2];
int g_nWQueueTail[2];

void init_wqueue()
{
	int tmpi;
	for( tmpi = 0; tmpi < 2; tmpi++ )
	{
		g_nWQueueHead[tmpi] = 0;
		g_nWQueueTail[tmpi] = 0;
		g_wqueue[tmpi] = (SWQueue *)malloc( sizeof(SWQueue)*MAX_WQUEUE_NUM );
	}
}

// ������ݵ�����
// ����-1��ʾ�������ˣ��޷�������
int add_wqueue( int client_index, short cmd, const char *pBuf, int wsize, int exec_code, int line )
{
	int queue_tail;

	if( line < 0 || line > 1 )
		return -1;

	if( wsize > MAX_CMDPACKAGE_SIZE )
		return -1;

	// ��ס����
	// ��ȡβ��
	queue_tail = g_nWQueueTail[line] + 1;
	if( queue_tail >= MAX_WQUEUE_NUM )
	{
		queue_tail = 0;
	}
	
	// �������Ѿ�����
	if( g_nWQueueHead[line] == queue_tail )
	{
		return -1;
	}

	// �����ݸ��ƽ���
	g_wqueue[line][g_nWQueueTail[line]].client_index = client_index;
	g_wqueue[line][g_nWQueueTail[line]].buf_size = wsize;
	g_wqueue[line][g_nWQueueTail[line]].cmd = cmd;
	g_wqueue[line][g_nWQueueTail[line]].exec_code = exec_code;
	if( wsize > 0 )
		memcpy( g_wqueue[line][g_nWQueueTail[line]].buf, pBuf, wsize );

	// β������
	g_nWQueueTail[line] = queue_tail;

	// ��������
	// ֪ͨ�����߳̿�ʼ����
	return 0;
}

// ����0��ʾ�����һ��ָ������ʾ����Ϊ��
// ��õ����ݿ����Ѿ��Ͽ����ӣ���Ҫ����
int fetch_wqueue( int *pclient_index, short *pcmd, char *pBuf, int *pwsize, int *pexec_code, int line )
{
	if( line < 0 || line > 1 )
		return -1;
	if( g_nWQueueTail[line] == g_nWQueueHead[line] )
	{
		return -1;
	}
	// ������, Ӧ�þ��пɽ��յ�������
	// �Ӷ�����ȡ��һ��
	*pclient_index = g_wqueue[line][g_nWQueueHead[line]].client_index;
	*pwsize = g_wqueue[line][g_nWQueueHead[line]].buf_size;
	*pcmd = g_wqueue[line][g_nWQueueHead[line]].cmd;
	*pexec_code = g_wqueue[line][g_nWQueueHead[line]].exec_code;
	if( g_wqueue[line][g_nWQueueHead[line]].buf_size > 0 )
		memcpy( pBuf, g_wqueue[line][g_nWQueueHead[line]].buf, g_wqueue[line][g_nWQueueHead[line]].buf_size );

	// ͷ������
	g_nWQueueHead[line]++;
	if( g_nWQueueHead[line] >= MAX_WQUEUE_NUM )
	{
		g_nWQueueHead[line] = 0;
	}
	// ��������
	// ��������
	return 0;
}


// ִ�ж���
ExecQueue *g_exec_queue = NULL;
int g_nExecQueueHead;
int g_nExecQueueTail;
int exec_queue_init()
{
	g_nExecQueueHead = 0;
	g_nExecQueueTail = 0;
	g_exec_queue = (ExecQueue *)malloc( sizeof(ExecQueue)*EXEC_QUEUE_MAXNUM );
	return 0;
}

int exec_queue_add( char type, int value1, int value2 )
{
	// ��ȡβ��
	int queue_tail = g_nExecQueueTail + 1;
	if ( queue_tail >= EXEC_QUEUE_MAXNUM )
	{
		queue_tail = 0;
	}

	// �������Ѿ�����
	if ( g_nExecQueueHead == queue_tail )
	{
		return -1;
	}

	g_exec_queue[g_nExecQueueTail].type = type;
	g_exec_queue[g_nExecQueueTail].value[0] = value1;
	g_exec_queue[g_nExecQueueTail].value[1] = value2;

	// β������
	g_nExecQueueTail = queue_tail;
	return 0;
}

int exec_queue_fetch()
{
	if ( g_nExecQueueTail == g_nExecQueueHead )
	{
		return -1;
	}

	// �Ӷ�����ȡ��һ��
	char type = g_exec_queue[g_nExecQueueHead].type;
	int value1 = g_exec_queue[g_nExecQueueHead].value[0];
	int value2 = g_exec_queue[g_nExecQueueHead].value[1];

	if ( type == EXEC_QUEUE_TYPE_MAPEVENT_CREATE )
	{
		sc_Script_Exec( 10, value1, value2 );
	}

	// ͷ������
	g_nExecQueueHead++;
	if ( g_nExecQueueHead >= EXEC_QUEUE_MAXNUM )
	{
		g_nExecQueueHead = 0;
	}
	return 0;
}




// ִ�ж���
BrushEnemyQueue *g_brushenemy_queue = NULL;
int g_nBrushEnemyQueueHead;
int g_nBrushEnemyQueueTail;
int brush_enemy_queue_init()
{
	g_nBrushEnemyQueueHead = 0;
	g_nBrushEnemyQueueTail = 0;
	g_brushenemy_queue = (BrushEnemyQueue *)malloc( sizeof( BrushEnemyQueue )*BRUSH_ENEMY_QUEUE_MAXNUM );
	return 0;
}

int brush_enemy_queue_add( char type, char zoneid )
{	
	// ��ȡβ��
	int queue_tail = g_nBrushEnemyQueueTail + 1;
	if ( queue_tail >= BRUSH_ENEMY_QUEUE_MAXNUM )
	{
		queue_tail = 0;
	}

	// �������Ѿ�����
	if ( g_nBrushEnemyQueueHead == queue_tail )
	{
		return -1;
	}

	g_brushenemy_queue[g_nBrushEnemyQueueTail].type = type;
	g_brushenemy_queue[g_nBrushEnemyQueueTail].zoneid = zoneid;

	// β������
	g_nBrushEnemyQueueTail = queue_tail;
	return 0;
}

int brush_enemy_queue_fetch()
{
	if ( g_nBrushEnemyQueueTail == g_nBrushEnemyQueueHead )
	{
		return -1;
	}

	// �Ӷ�����ȡ��һ��
	char type = g_brushenemy_queue[g_nBrushEnemyQueueHead].type;
	char zoneid = g_brushenemy_queue[g_nBrushEnemyQueueHead].zoneid;
	if ( type == BRUSH_ENEMY_QUEUE_ENEMY )
	{
		sc_Script_Exec( 1, zoneid, 0 );
	}
	else if ( type == BRUSH_ENEMY_QUEUE_RES )
	{
		sc_Script_Exec( 2, zoneid, 0 );
	}
	else if ( type == BRUSH_ENEMY_QUEUE_PICKUP )
	{
		sc_Script_Exec( 3, zoneid, 0 );
	}
	else if ( type == BRUSH_ENEMY_QUEUE_ACTIVITY27 )
	{
		activity_27_brush_withzoneid( zoneid );
	}
	
	// ͷ������
	g_nBrushEnemyQueueHead++;
	if ( g_nBrushEnemyQueueHead >= BRUSH_ENEMY_QUEUE_MAXNUM )
	{
		g_nBrushEnemyQueueHead = 0;
	}
	return 0;
}
