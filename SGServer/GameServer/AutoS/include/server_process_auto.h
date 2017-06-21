#ifndef __PROCESS_AUTO_H0
#define __PROCESS_AUTO_H0
#include "server_structrecv_auto.h"

int proc_command_S( int client_index, short cmd, char *ptr, int tmpsize, int exec_code );
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

#endif
