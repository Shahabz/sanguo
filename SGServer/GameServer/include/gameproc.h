#ifndef __GAMEPROC_H
#define __GAMEPROC_H

typedef struct PACKET
{
	const char *szMsg;
	int actor_index;
	int msgsize;

	char tmpbuf[2048];
	char LogMsg[200];
	int tmpsize;
	char *ptr, *ptrsize;
}PACKET;

int process_client( int client_index, char *tmpbuf, int readsize, int exec_code );
int process_init( int max_connection );
int process_logic();
void process_close();
int process_onclose( int client_index );
int process_error( int client_index, int err );
int process_ondestory();
// 设定服务器自动退出时间
int process_set_exit( int weekday, int hour, int min, int isnotify );
// 数据表重读
int process_dbreload();

#endif
