#ifndef __ACTOR_H
#define __ACTOR_H
#include "auto_actor.h"
#include "server_structsend_auto.h"
#include "award.h"

// ------------------------------- actor.c -----------------------------------
// 服务器所有角色初始化
int actors_init();

// 所有在线角色保存
int actors_save_everyone( char type );

// 服务器崩溃之后的存档
void actors_on_core();

// 保存角色数据
int actor_save( int actor_index, int savecity, FILE *fp );

// 开始保存角色数据
int actor_real_saveall( int actor_index, FILE *fp );

// 发送保存信息到队列
int actor_send_save( int actor_index );

// 角色发送下线信息到队列
int actor_send_remove( int actor_index );

// 角色下线保存并断开客户端
int actor_remove( int actor_index );

// 更新角色列表数据
int actor_update_list( int actor_index );

// 更新数据库中角色在线情况
int actor_update_online( int actorid, char isonline );

// 清除客户端
int actor_del( int client_index );

// 真正的删除角色
int actor_delete( i64 userid, int actorid );

// 设置删除标记
int actor_delete_stoptime( i64 userid, int actorid );

// 放弃删除
int actor_delete_cancel( int actorid );

// 所有角色逻辑
void actors_logic();

// 角色逻辑
void actor_logic( int actor_index );

// 角色进入游戏
int actor_entergame( int client_index, int actorid );

// 角色进入游戏世界
int actor_enterworld( int client_index, int actorid, int actor_index );

// 角色进入城池
int actor_entercity( int actor_index );

// 角色进入游戏后读取数据
int actor_load( int actor_index, int actorid );

// 第一次进入游戏要初始化角色信息
int actor_new( int actor_index );

// 创建角色
int actor_create( int client_index, int aclass, char *szActorName );

// 创建角色,数据库
int actor_db_create( int client_index, int platid, i64 userid, char *username, SLK_ListInfo *pListInfo );

// 角色重新开始
int actor_restart( int actor_index );

// 角色重新开始
int actor_restart_notonline( int actorid );

// 踢人
int actor_kick( int actorid, int kicktime );

// 禁言
int actor_forbidtalk( int actor_index, int forbidtime );

// 订阅一个及时通知的消息
int actor_subscribecmd_add( int actor_index, short cmd );
int actor_subscribecmd_del( int actor_index, short cmd );
int actor_subscribecmd_check( int actor_index, short cmd );

// ------------------------------- actor_func.c -----------------------------------
// 根据索引获取用户id
i64 actor_getoffline_userid( int actorid );

// 根据编号获取玩家索引
int actor_getindex_withid( int actorid );

// 返回角色列表
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue );

// 验证一下这个角色id是不是属于登陆的这个用户
int actor_verifyid( i64 userid, int actorid );

// 角色经验
int actor_exp( int actor_index, int experience, char path );

// 角色升级
int actor_upgrade( int actor_index, char path, AwardGetInfo *getinfo );

// 角色钻石
int actor_change_token( int actor_index, int token, char path, int path_value );

// 修改名称
int actor_changename( int actor_index, char *pname );

// 修改名称
int actor_changename_gm( int actorid, char *pname );

// 修改头像
int actor_changeshape( int actor_index, int shape );

// 角色信息
int actor_getinfo( int actor_index );

// 角色配置
int actor_configinfo( int actor_index );
int actor_setconfig( int actor_index, int index, char value );
int actor_getconfig( int actor_index, int index );

// ------------------------------- actor_gmcmd.c -----------------------------------
int actor_command( int actor_index, short cmd, int *pValue, char *pMsg );


#endif
