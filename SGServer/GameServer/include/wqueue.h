#ifndef __WQUEUE_H
#define __WQUEUE_H


#define MAX_WQUEUE_NUM			4096
#define MAX_CMDPACKAGE_SIZE		2048

#define WCMD_ENTERGAME			1
#define WCMD_LEAVE				2
#define WCMD_LIST				3
#define WCMD_CREATE				4
#define WCMD_SAVE				5

// 登陆队列
struct _swqueue
{
	short cmd;						// 指令
	int client_index;				// 客户端索引
	int buf_size;					// 网络包长度
	char buf[MAX_CMDPACKAGE_SIZE];	// 网络包数据
	int exec_code;
};
typedef struct _swqueue SWQueue;

void init_wqueue();
int add_wqueue( int client_index, short cmd, const char *pBuf, int wsize, int exec_code, int line );
int fetch_wqueue( int *pclient_index, short *pcmd, char *pBuf, int *pwsize, int *pexec_code, int line );



// 工作队列所有耗时的操作可以加入这里，每帧执行N个
#define	EXEC_QUEUE_MAXNUM				800000		// 工作队列最大数

// 创建队列防止同一帧做太多事情
typedef struct _exec_queue
{
	char	type;			// 类型
	int		value[2];		// 附加值
}ExecQueue;

int exec_queue_init();
int exec_queue_add( char type, int value1, int value2 );
int exec_queue_fetch();

#endif
