#ifndef _ACTOR_FRIEND_H_
#define _ACTOR_FRIEND_H_
#include "define.h"
#define ACTOR_FRIEND_MAXCOUNT		100 // 好友数量
#define ACTOR_STUDENT_MAXCOUNT		30  // 徒弟数量

#define ACTOR_FRIEND_EVENT_ASK			1	// 对方请求添加我为好友
#define ACTOR_FRIEND_EVENT_AGREE		2	// 对方同意添加我为好友
#define ACTOR_FRIEND_EVENT_REJECT		3	// 对方拒绝添加我为好友
#define ACTOR_FRIEND_EVENT_DEL			4	// 对方删除我

// 好友结构指针
ActorFriend *actor_friend_getptr( int actor_index, int offset );

// 添加离线事件
int actor_friend_addevent( int event, int actorid, int target_actorid );

// 读取好友离线事件
int actor_friend_loadevent( int actor_index );

// 离线事件处理
int actor_friend_enevt( int actor_index, int event, int target_actorid );

// 申请加好友
int actor_friend_ask( int actor_index, int target_actorid, const char *target_name );

// 同意添加好友
int actor_friend_agree( int actor_index, int target_actorid, int target_city_index );

// 拒绝添加好友
int actor_friend_reject( int actor_index, int target_actorid, int target_city_index );

// 删除好友
int actor_friend_delete( int actor_index, int target_actorid, int target_city_index );
int actor_friend_delete_db( int actorid, int target_actorid );
// 好友列表
int actor_friend_sendlist( int actor_index );


// 师徒
// 徒弟列表
int actor_student_sendlist( int actor_index );

// 拜师
int actor_take_teacher( int actor_index, int teacher_actorid );

#endif // !_ACTOR_FRIEND_H_
