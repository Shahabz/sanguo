#ifndef _WORLD_QUEST_H_
#define _WORLD_QUEST_H_
#include "define.h"

#define WORLDQUEST_ID1			1	//	��ɱ����5��
#define WORLDQUEST_ID2			2	//	��ɱ����10��
#define WORLDQUEST_ID3			3	//	��ɱ����20��
#define WORLDQUEST_ID4			4	//	�����س�
#define WORLDQUEST_ID5			5	//	���˿���
#define WORLDQUEST_ID6			6	//	��������
#define WORLDQUEST_WORLDBOSS1	8	//	����boss���ι�-֮�����ǰ��˾��
#define WORLDQUEST_WORLDBOSS2	9	//	����boss��׿-֮����Ѫս�ʳǻ
#define WORLDQUEST_LUOYANG		10	//	�ʳ�Ѫս����

int worldquest_init();
int worldquest_reload();
int worldquest_give( int actor_index );
int worldquest_addvalue( int questid, int value );
int worldquest_setvalue( int questid, int value );
int worldquest_setcomplete( int actor_index, int questid );
int worldquest_getcomplete( int actor_index, int questid, int *value );
int worldquest_check( int actor_index, int questid, int *value );
int worldquest_check_server( int questid );
int worldquest_checkcomplete( int actor_index, char type );
int worldquest_sendinfo( int actor_index );
int worldquest_sendaward( int actor_index, int questid );
int worldquest_getaward( int actor_index, int questid );
int worldquest_updateopen();
#endif
