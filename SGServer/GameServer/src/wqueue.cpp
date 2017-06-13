#include <string.h>
#include "netfunc.h"
#include "client.h"
#include "wqueue.h"
#include "utils.h"
#include "script_auto.h"

// 队列
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

// 添加数据到队列
// 返回-1表示队列满了，无法加入了
int add_wqueue( int client_index, short cmd, const char *pBuf, int wsize, int exec_code, int line )
{
	int queue_tail;

	if( line < 0 || line > 1 )
		return -1;

	if( wsize > MAX_CMDPACKAGE_SIZE )
		return -1;

	// 锁住队列
	// 获取尾部
	queue_tail = g_nWQueueTail[line] + 1;
	if( queue_tail >= MAX_WQUEUE_NUM )
	{
		queue_tail = 0;
	}
	
	// 当队列已经满了
	if( g_nWQueueHead[line] == queue_tail )
	{
		return -1;
	}

	// 将数据复制进来
	g_wqueue[line][g_nWQueueTail[line]].client_index = client_index;
	g_wqueue[line][g_nWQueueTail[line]].buf_size = wsize;
	g_wqueue[line][g_nWQueueTail[line]].cmd = cmd;
	g_wqueue[line][g_nWQueueTail[line]].exec_code = exec_code;
	if( wsize > 0 )
		memcpy( g_wqueue[line][g_nWQueueTail[line]].buf, pBuf, wsize );

	// 尾部步进
	g_nWQueueTail[line] = queue_tail;

	// 解锁队列
	// 通知接收线程开始工作
	return 0;
}

// 返回0表示获得了一条指令，否则表示队列为空
// 获得的数据可能已经断开连接，需要考虑
int fetch_wqueue( int *pclient_index, short *pcmd, char *pBuf, int *pwsize, int *pexec_code, int line )
{
	if( line < 0 || line > 1 )
		return -1;
	if( g_nWQueueTail[line] == g_nWQueueHead[line] )
	{
		return -1;
	}
	// 到这了, 应该就有可接收的数据了
	// 从队列中取出一项
	*pclient_index = g_wqueue[line][g_nWQueueHead[line]].client_index;
	*pwsize = g_wqueue[line][g_nWQueueHead[line]].buf_size;
	*pcmd = g_wqueue[line][g_nWQueueHead[line]].cmd;
	*pexec_code = g_wqueue[line][g_nWQueueHead[line]].exec_code;
	if( g_wqueue[line][g_nWQueueHead[line]].buf_size > 0 )
		memcpy( pBuf, g_wqueue[line][g_nWQueueHead[line]].buf, g_wqueue[line][g_nWQueueHead[line]].buf_size );

	// 头部步进
	g_nWQueueHead[line]++;
	if( g_nWQueueHead[line] >= MAX_WQUEUE_NUM )
	{
		g_nWQueueHead[line] = 0;
	}
	// 解锁队列
	// 处理数据
	return 0;
}


// 刷怪队列
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
	// 获取尾部
	int queue_tail = g_nExecQueueTail + 1;
	if ( queue_tail >= EXEC_QUEUE_MAXNUM )
	{
		queue_tail = 0;
	}

	// 当队列已经满了
	if ( g_nExecQueueHead == queue_tail )
	{
		return -1;
	}

	g_exec_queue[g_nExecQueueTail].type = type;
	g_exec_queue[g_nExecQueueTail].value[0] = value1;
	g_exec_queue[g_nExecQueueTail].value[1] = value2;

	// 尾部步进
	g_nExecQueueTail = queue_tail;
	return 0;
}

int exec_queue_fetch()
{
	if ( g_nExecQueueTail == g_nExecQueueHead )
	{
		return -1;
	}

	// 从队列中取出一项
	char type = g_exec_queue[g_nExecQueueHead].type;
	int value1 = g_exec_queue[g_nExecQueueHead].value[0];
	int value2 = g_exec_queue[g_nExecQueueHead].value[1];

	//if ( type == EXEC_QUEUE_TYPE_CREATERES )
	//{
	//	sc_Script_Exec( EXEC_QUEUE_TYPE_CREATERES, value1, value2 );
	//}

	// 头部步进
	g_nExecQueueHead++;
	if ( g_nExecQueueHead >= EXEC_QUEUE_MAXNUM )
	{
		g_nExecQueueHead = 0;
	}
	return 0;
}

