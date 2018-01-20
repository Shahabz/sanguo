#include "actor_notify.h"
#include "actor.h"
#include "actor_send.h"
#include "gameproc.h"
#include "server_netsend_auto.h"
#include "mail.h"
#include "king_war.h"

extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;

// 发送给角色短消息 无参数
int actor_notify( int actor_index, short msgid, const char *msg )
{
	SLK_NetS_Notify Value = { 0 };
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	Value.m_msgid = msgid;
	Value.m_valuenum = 0;
	if ( msg )
	{
		Value.m_msg_length = (short)strlen( msg ) + 1;
		strcpy( Value.m_msg, msg );
	}
	netsend_notify_S( actor_index, SENDTYPE_ACTOR, &Value );

	return 0;
}

// 发送给角色短消息 带参数
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg )
{
	SLK_NetS_Notify Value = { 0 };
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	Value.m_msgid = msgid;
	Value.m_valuenum = count;
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		Value.m_value[tmpi] = data[tmpi];
	}
	if ( msg )
	{
		Value.m_msg_length = (short)strlen( msg ) + 1;
		strcpy( Value.m_msg, msg );
	}
	netsend_notify_S( actor_index, SENDTYPE_ACTOR, &Value );

	return 0;
}

// 发消息提示，通过消息ID
int actor_notify_pop( int actor_index, int textid )
{
	actor_notify_value( actor_index, NOTIFY_POP, 1, &textid, NULL );
	return 0;
}

int actor_notify_pop_v( int actor_index, int textid, char *v1, char *v2 )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	char content[128] = { 0 };
	if ( v1 && v2 )
	{
		sprintf( content, "{\"v1\":\"%s\",\"v2\":\"%s\"}", v1, v2 );
		actor_notify_value( actor_index, NOTIFY_POP, 1, &textid, content );
	}
	else if ( v1 )
	{
		sprintf( content, "{\"v1\":\"%s\"}", v1 );
		actor_notify_value( actor_index, NOTIFY_POP, 1, &textid, content );
	}
	else
	{
		actor_notify_value( actor_index, NOTIFY_POP, 1, &textid, NULL );
	}
	return 0;
}

// npc对话
int npc_talk( int actor_index, int textid, int btntextid )
{
	actor_notify_value( actor_index, NOTIFY_NPCTALK, 1, &textid, NULL );
	return 0;
}

// 武将说话
int hero_talk( int actor_index, int herokind, int textid )
{
	return 0;
}

// 弹框消息
int actor_notify_box( int actor_index, int textid )
{
	actor_notify_value( actor_index, NOTIFY_BOX, 1, &textid, NULL );
	return 0;
}
int actor_notify_box_v( int actor_index, int textid, char *v1, char *v2 )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	char content[128] = { 0 };
	if ( v1 && v2 )
	{
		sprintf( content, "{\"v1\":\"%s\",\"v2\":\"%s\"}", v1, v2 );
		actor_notify_value( actor_index, NOTIFY_BOX, 1, &textid, content );
	}
	else if ( v1 )
	{
		sprintf( content, "{\"v1\":\"%s\"}", v1 );
		actor_notify_value( actor_index, NOTIFY_BOX, 1, &textid, content );
	}
	else
	{
		actor_notify_value( actor_index, NOTIFY_BOX, 1, &textid, NULL );
	}
	return 0;
}

// 弹出确定消息
int actor_notify_alert( int actor_index, int textid )
{
	actor_notify_value( actor_index, NOTIFY_ALERT, 1, &textid, NULL );
	return 0;
}

// 弹出确定消息带参数
int actor_notify_alert_v( int actor_index, int textid, char *v1, char *v2 )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	char content[128] = { 0 };
	if ( v1 && v2 )
	{
		sprintf( content, "{\"v1\":\"%s\",\"v2\":\"%s\"}", v1, v2 );
		actor_notify_value( actor_index, NOTIFY_ALERT, 1, &textid, content );
	}
	else if ( v1 )
	{
		sprintf( content, "{\"v1\":\"%s\"}", v1 );
		actor_notify_value( actor_index, NOTIFY_ALERT, 1, &textid, content );
	}
	else
	{
		actor_notify_value( actor_index, NOTIFY_ALERT, 1, &textid, NULL );
	}
	return 0;
}

// 弹出消息选择框
int actor_notify_msgbox_v( int actor_index, int msgid, int value1, int value2, int textid, char *v1, char *v2 )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	char content[128] = { 0 };
	int value[3] = { 0 };
	value[0] = msgid;
	value[1] = value1;
	value[2] = value2;
	if ( v1 && v2 )
	{
		sprintf( content, "{\"textid\":\"%s%d\",\"v1\":\"%s\",\"v2\":\"%s\"}", TAG_TEXTID, textid, v1, v2 );
		actor_notify_value( actor_index, NOTIFY_MSGBOX, 3, value, content );
	}
	else if ( v1 )
	{
		sprintf( content, "{\"textid\":\"%s%d\",\"v1\":\"%s\"}", TAG_TEXTID, textid, v1 );
		actor_notify_value( actor_index, NOTIFY_MSGBOX, 3, value, content );
	}
	else
	{
		sprintf( content, "{\"textid\":\"%s%d\"}", TAG_TEXTID, textid );
		actor_notify_value( actor_index, NOTIFY_MSGBOX, 3, value, NULL );
	}
	return 0;
}

// 消息框回调
int actor_notify_msgbox_callback( int actor_index, int msgid, int value1, int value2 )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	switch ( msgid )
	{
	case MSGBOX_CALLBACK_KINGWAR_DEFENSE:
		kingwar_army_defense( actor_index, value1, value2 );
		break;
	case MSGBOX_CALLBACK_KINGWAR_SNEAK:
		kingwar_army_sneak( actor_index, value1, value2 );
		break;
	default:
		break;
	}
	return 0;
}

// 通知客户端更新ui
void ui_update( int actor_index, char send_type, char ui )
{
	SLK_NetS_DialogUpdate pValue = { 0 };
	pValue.m_ui = ui;
	netsend_dialogupdate_S( actor_index, send_type, &pValue );
}
