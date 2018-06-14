#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "define.h"

int robot_randomname( char *name );
int robot_create( char ai, char nation );
void robot_logic_hour();
void robot_logic( City *pCity );

// 机器人创建队，每秒执行1个
#define	ROBOTCREATE_QUEUE_MAXNUM	20000
typedef struct _robotcreate_queue
{
	char ai;
	char nation;
}RobotCreateQueue;

int robotcreate_queue_init();
int robotcreate_queue_add( char ai, char nation );
int robotcreate_queue_fetch();
#endif
