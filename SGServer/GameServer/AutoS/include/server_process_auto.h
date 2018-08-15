#ifndef __PROCESS_AUTO_H0
#define __PROCESS_AUTO_H0
#include "server_structrecv_auto.h"

int proc_command_S( int client_index, short cmd, char *ptr, int tmpsize, int exec_code );
void proc_invitecodeed_S( int client_index, SLK_NetU_InviteCodeed *pValue );
void proc_userawarded_S( int client_index, SLK_NetU_UserAwarded *pValue );
void proc_gmlocalcmd_S( int client_index, SLK_NetU_Gmlocalcmd *pValue );
void proc_logined_S( int client_index, SLK_NetU_Logined *pValue );
void proc_timer_S( int client_index, char *pValue );
void proc_login_S( int client_index, SLK_NetC_Login *pValue );
void proc_create_S( int client_index, SLK_NetC_Create *pValue );
void proc_list_S( int client_index, char *pValue );
void proc_entergame_S( int client_index, int *pValue );
void proc_delete_S( int client_index, int *pValue );
void proc_heart_S( int client_index, SLK_NetC_Heart *pValue );
void proc_gmcmd_S( int client_index, SLK_NetC_Gmcmd *pValue );
void proc_askinfo_S( int client_index, SLK_NetC_AskInfo *pValue );
void proc_entercity_S( int client_index, SLK_NetC_EnterCity *pValue );
void proc_chat_S( int client_index, SLK_NetC_Chat *pValue );
void proc_storybattle_S( int client_index, SLK_NetC_StoryBattle *pValue );
void proc_worldmapask_S( int client_index, SLK_NetC_WorldMapAsk *pValue );
void proc_worldmapareaindex_S( int client_index, SLK_NetC_WorldMapAreaIndex *pValue );
void proc_mapbattle_S( int client_index, SLK_NetC_MapBattle *pValue );
void proc_mailask_S( int client_index, SLK_NetC_MailAsk *pValue );
void proc_mailop_S( int client_index, SLK_NetC_MailOp *pValue );
void proc_mailalldel_S( int client_index, SLK_NetC_MailAllDel *pValue );
void proc_mailshare_S( int client_index, SLK_NetS_MailShare *pValue );
void proc_mailsend_S( int client_index, SLK_NetC_MailSend *pValue );
void proc_mailreply_S( int client_index, SLK_NetC_MailReply *pValue );
void proc_worldbossbattle_S( int client_index, SLK_NetC_WorldBossBattle *pValue );
void proc_heroguardsort_S( int client_index, SLK_NetC_HeroGuardSort *pValue );
void proc_questtalknext_S( int client_index, SLK_NetC_QuestTalkNext *pValue );
void proc_ranklistask_S( int client_index, SLK_NetC_RankAsk *pValue );
void proc_friendop_S( int client_index, SLK_NetC_FriendOp *pValue );

#endif
