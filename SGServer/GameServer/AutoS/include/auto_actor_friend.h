#ifndef __ACTORFRIEND_AUTO_H
#define __ACTORFRIEND_AUTO_H
#include "define.h"

struct _actor_friend {
//--Automatically generated
				int actorid;	//角色编号
				char offset;	//索引
				int friend_actorid;	//好友id
				int jointime;	//加入时间
				int city_index;	//好友城池索引
};
typedef struct _actor_friend ActorFriend;

typedef ActorFriend * (*LPCB_GETACTORFRIEND)( int actorid, int offset );
int actor_friend_load_auto( int actorid, int actor_index, LPCB_GETACTORFRIEND pCB_GetActorFriend, const char *pTab );
int actor_friend_save_auto( ActorFriend *pActorFriend, const char *pTab, FILE *fp );
int actor_friend_batch_save_auto( ActorFriend *pActorFriend, int maxcount,  const char *pTab, FILE *fp );

#endif
