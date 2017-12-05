#include "server_struct_auto.h"
#include "server_process_auto.h"
#include "server_netrecv_auto.h"
#include "actor_send.h"
#include "client.h"
#include "wqueue.h"
#include "utils.h"
extern int g_mysql_wait;

int netrecv_userawarded_S( int client_index, char *data, int size )
{
	SLK_NetU_UserAwarded Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetU_UserAwarded_recv( &ptr, &tmpsize, &Value );

	proc_userawarded_S( client_index, &Value );

	return 0;
}

int netrecv_gmlocalcmd_S( int client_index, char *data, int size )
{
	SLK_NetU_Gmlocalcmd Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetU_Gmlocalcmd_recv( &ptr, &tmpsize, &Value );

	proc_gmlocalcmd_S( client_index, &Value );

	return 0;
}

int netrecv_logined_S( int client_index, char *data, int size )
{
	SLK_NetU_Logined Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetU_Logined_recv( &ptr, &tmpsize, &Value );

	proc_logined_S( client_index, &Value );

	return 0;
}

int netrecv_timer_S( int client_index, char *data, int size )
{
	char Value = 0;
	int tmpsize = size;
	char *ptr = data;

	LKSET_SBYTE_RECV( &Value, ptr, tmpsize );

	proc_timer_S( client_index, &Value );

	return 0;
}

int netrecv_login_S( int client_index, char *data, int size )
{
	SLK_NetC_Login Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_Login_recv( &ptr, &tmpsize, &Value );

	proc_login_S( client_index, &Value );

	return 0;
}

int netrecv_create_S( int client_index, char *data, int size )
{
	SLK_NetC_Create Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_Create_recv( &ptr, &tmpsize, &Value );

	proc_create_S( client_index, &Value );

	return 0;
}

int netrecv_list_S( int client_index, char *data, int size )
{
	char Value = 0;
	int tmpsize = size;
	char *ptr = data;

	LKSET_SBYTE_RECV( &Value, ptr, tmpsize );

	proc_list_S( client_index, &Value );

	return 0;
}

int netrecv_entergame_S( int client_index, char *data, int size )
{
	int Value = 0;
	int tmpsize = size;
	char *ptr = data;

	LKSET_DWORD_RECV( &Value, ptr, tmpsize );

	proc_entergame_S( client_index, &Value );

	return 0;
}

int netrecv_delete_S( int client_index, char *data, int size )
{
	int Value = 0;
	int tmpsize = size;
	char *ptr = data;

	LKSET_DWORD_RECV( &Value, ptr, tmpsize );

	proc_delete_S( client_index, &Value );

	return 0;
}

int netrecv_heart_S( int client_index, char *data, int size )
{
	SLK_NetC_Heart Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_Heart_recv( &ptr, &tmpsize, &Value );

	proc_heart_S( client_index, &Value );

	return 0;
}

int netrecv_gmcmd_S( int client_index, char *data, int size )
{
	SLK_NetC_Gmcmd Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_Gmcmd_recv( &ptr, &tmpsize, &Value );

	proc_gmcmd_S( client_index, &Value );

	return 0;
}

int netrecv_askinfo_S( int client_index, char *data, int size )
{
	SLK_NetC_AskInfo Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_AskInfo_recv( &ptr, &tmpsize, &Value );

	proc_askinfo_S( client_index, &Value );

	return 0;
}

int netrecv_entercity_S( int client_index, char *data, int size )
{
	SLK_NetC_EnterCity Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_EnterCity_recv( &ptr, &tmpsize, &Value );

	proc_entercity_S( client_index, &Value );

	return 0;
}

int netrecv_chat_S( int client_index, char *data, int size )
{
	SLK_NetC_Chat Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_Chat_recv( &ptr, &tmpsize, &Value );

	proc_chat_S( client_index, &Value );

	return 0;
}

int netrecv_storybattle_S( int client_index, char *data, int size )
{
	SLK_NetC_StoryBattle Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_StoryBattle_recv( &ptr, &tmpsize, &Value );

	proc_storybattle_S( client_index, &Value );

	return 0;
}

int netrecv_worldmapask_S( int client_index, char *data, int size )
{
	SLK_NetC_WorldMapAsk Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_WorldMapAsk_recv( &ptr, &tmpsize, &Value );

	proc_worldmapask_S( client_index, &Value );

	return 0;
}

int netrecv_worldmapareaindex_S( int client_index, char *data, int size )
{
	SLK_NetC_WorldMapAreaIndex Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_WorldMapAreaIndex_recv( &ptr, &tmpsize, &Value );

	proc_worldmapareaindex_S( client_index, &Value );

	return 0;
}

int netrecv_mapbattle_S( int client_index, char *data, int size )
{
	SLK_NetC_MapBattle Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MapBattle_recv( &ptr, &tmpsize, &Value );

	proc_mapbattle_S( client_index, &Value );

	return 0;
}

int netrecv_mailask_S( int client_index, char *data, int size )
{
	SLK_NetC_MailAsk Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MailAsk_recv( &ptr, &tmpsize, &Value );

	proc_mailask_S( client_index, &Value );

	return 0;
}

int netrecv_mailop_S( int client_index, char *data, int size )
{
	SLK_NetC_MailOp Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MailOp_recv( &ptr, &tmpsize, &Value );

	proc_mailop_S( client_index, &Value );

	return 0;
}

int netrecv_mailalldel_S( int client_index, char *data, int size )
{
	SLK_NetC_MailAllDel Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MailAllDel_recv( &ptr, &tmpsize, &Value );

	proc_mailalldel_S( client_index, &Value );

	return 0;
}

int netrecv_mailshare_S( int client_index, char *data, int size )
{
	SLK_NetS_MailShare Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetS_MailShare_recv( &ptr, &tmpsize, &Value );

	proc_mailshare_S( client_index, &Value );

	return 0;
}

int netrecv_mailsend_S( int client_index, char *data, int size )
{
	SLK_NetC_MailSend Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MailSend_recv( &ptr, &tmpsize, &Value );

	proc_mailsend_S( client_index, &Value );

	return 0;
}

int netrecv_mailreply_S( int client_index, char *data, int size )
{
	SLK_NetC_MailReply Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_MailReply_recv( &ptr, &tmpsize, &Value );

	proc_mailreply_S( client_index, &Value );

	return 0;
}

int netrecv_worldbossbattle_S( int client_index, char *data, int size )
{
	SLK_NetC_WorldBossBattle Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_WorldBossBattle_recv( &ptr, &tmpsize, &Value );

	proc_worldbossbattle_S( client_index, &Value );

	return 0;
}

int netrecv_heroguardsort_S( int client_index, char *data, int size )
{
	SLK_NetC_HeroGuardSort Value = {0};
	int tmpsize = size;
	char *ptr = data;

	struct_NetC_HeroGuardSort_recv( &ptr, &tmpsize, &Value );

	proc_heroguardsort_S( client_index, &Value );

	return 0;
}

int netrecv_wqueue_create_S( int client_index, char *data, int size, int exec_code )
{
	client_setwait( client_index, 1 );
	if( g_mysql_wait )
	{
		if( add_wqueue( client_index, WCMD_CREATE, data, size, exec_code, 0 ) < 0 )
		{
			write_gamelog( "add_wqueue full" );
			net_closesock_direct( client_index );
			return -1;
		}
		return 0;
	}
	else
	{
		return netrecv_create_S( client_index, data, size );
	}
}

int netrecv_wqueue_list_S( int client_index, char *data, int size, int exec_code )
{
	client_setwait( client_index, 1 );
	if( g_mysql_wait )
	{
		if( add_wqueue( client_index, WCMD_LIST, data, size, exec_code, 0 ) < 0 )
		{
			write_gamelog( "add_wqueue full" );
			net_closesock_direct( client_index );
			return -1;
		}
		return 0;
	}
	else
	{
		return netrecv_list_S( client_index, data, size );
	}
}

int netrecv_wqueue_entergame_S( int client_index, char *data, int size, int exec_code )
{
	client_setwait( client_index, 1 );
	if( g_mysql_wait )
	{
		if( add_wqueue( client_index, WCMD_ENTERGAME, data, size, exec_code, 0 ) < 0 )
		{
			write_gamelog( "add_wqueue full" );
			net_closesock_direct( client_index );
			return -1;
		}
		return 0;
	}
	else
	{
		return netrecv_entergame_S( client_index, data, size );
	}
}

int proc_command_S( int client_index, short cmd, char *ptr, int tmpsize, int exec_code )
{
	switch( cmd )
	{
	case CMDC_USERAWARDED:
		netrecv_userawarded_S( client_index, ptr, tmpsize );
		break;
	case CMDC_GMLOCALCMD:
		netrecv_gmlocalcmd_S( client_index, ptr, tmpsize );
		break;
	case CMDC_LOGINED:
		netrecv_logined_S( client_index, ptr, tmpsize );
		break;
	case CMDC_TIMER:
		netrecv_timer_S( client_index, ptr, tmpsize );
		break;
	case CMDC_LOGIN:
		netrecv_login_S( client_index, ptr, tmpsize );
		break;
	case CMDC_CREATE:
		netrecv_wqueue_create_S( client_index, ptr, tmpsize, exec_code );
		break;
	case CMDC_LIST:
		netrecv_wqueue_list_S( client_index, ptr, tmpsize, exec_code );
		break;
	case CMDC_ENTERGAME:
		netrecv_wqueue_entergame_S( client_index, ptr, tmpsize, exec_code );
		break;
	case CMDC_DELETE:
		netrecv_delete_S( client_index, ptr, tmpsize );
		break;
	case CMDC_HEART:
		netrecv_heart_S( client_index, ptr, tmpsize );
		break;
	case CMDC_GMCMD:
		netrecv_gmcmd_S( client_index, ptr, tmpsize );
		break;
	case CMDC_ASKINFO:
		netrecv_askinfo_S( client_index, ptr, tmpsize );
		break;
	case CMDC_ENTERCITY:
		netrecv_entercity_S( client_index, ptr, tmpsize );
		break;
	case CMDC_CHAT:
		netrecv_chat_S( client_index, ptr, tmpsize );
		break;
	case CMDC_STORYBATTLE:
		netrecv_storybattle_S( client_index, ptr, tmpsize );
		break;
	case CMDC_WORLDMAPASK:
		netrecv_worldmapask_S( client_index, ptr, tmpsize );
		break;
	case CMDC_WORLDMAPAREAINDEX:
		netrecv_worldmapareaindex_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAPBATTLE:
		netrecv_mapbattle_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILASK:
		netrecv_mailask_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILOP:
		netrecv_mailop_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILALLDEL:
		netrecv_mailalldel_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILSHARE:
		netrecv_mailshare_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILSEND:
		netrecv_mailsend_S( client_index, ptr, tmpsize );
		break;
	case CMDC_MAILREPLY:
		netrecv_mailreply_S( client_index, ptr, tmpsize );
		break;
	case CMDC_WORLDBOSSBATTLE:
		netrecv_worldbossbattle_S( client_index, ptr, tmpsize );
		break;
	case CMDC_HEROGUARDSORT:
		netrecv_heroguardsort_S( client_index, ptr, tmpsize );
		break;
	default:
		return -1;
	}
	return 0;
}

