#ifndef __WQUEUE_H
#define __WQUEUE_H


#define MAX_WQUEUE_NUM			4096
#define MAX_CMDPACKAGE_SIZE		2048

#define WCMD_ENTERGAME			1
#define WCMD_LEAVE				2
#define WCMD_LIST				3
#define WCMD_CREATE				4
#define WCMD_SAVE				5

// ��½����
struct _swqueue
{
	short cmd;						// ָ��
	int client_index;				// �ͻ�������
	int buf_size;					// ���������
	char buf[MAX_CMDPACKAGE_SIZE];	// ���������
	int exec_code;
};
typedef struct _swqueue SWQueue;

void init_wqueue();
int add_wqueue( int client_index, short cmd, const char *pBuf, int wsize, int exec_code, int line );
int fetch_wqueue( int *pclient_index, short *pcmd, char *pBuf, int *pwsize, int *pexec_code, int line );



// �����������к�ʱ�Ĳ������Լ������ÿִ֡��N��
#define	EXEC_QUEUE_MAXNUM				20480		// �������������
#define	EXEC_QUEUE_TYPE_MAPEVENT_CREATE		1		// ����¼�����

// �������з�ֹͬһ֡��̫������
typedef struct _exec_queue
{
	char	type;			// ����
	int		value[2];		// ����ֵ
}ExecQueue;

int exec_queue_init();
int exec_queue_add( char type, int value1, int value2 );
int exec_queue_fetch();



// ˢ���ܶ��У�ÿִ֡��1������
#define	BRUSH_ENEMY_QUEUE_MAXNUM	64
#define	BRUSH_ENEMY_QUEUE_ENEMY		0
#define	BRUSH_ENEMY_QUEUE_RES		1
#define	BRUSH_ENEMY_QUEUE_PICKUP	2
#define	BRUSH_ENEMY_QUEUE_ACTIVITY27	3

// �������з�ֹͬһ֡��̫������
typedef struct _brush_enemy_queue
{
	char	type;		// ����
	char	zoneid;		// ����ֵ
}BrushEnemyQueue;

int brush_enemy_queue_init();
int brush_enemy_queue_add( char type, char zoneid );
int brush_enemy_queue_fetch();

#endif
