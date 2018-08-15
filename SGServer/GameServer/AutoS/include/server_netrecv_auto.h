#ifndef __NETRECV_AUTO_H0
#define __NETRECV_AUTO_H0

#include "server_structrecv_auto.h"
int netrecv_invitecodeed_S( int client_index, char *data, int size );
int netrecv_userawarded_S( int client_index, char *data, int size );
int netrecv_gmlocalcmd_S( int client_index, char *data, int size );
int netrecv_logined_S( int client_index, char *data, int size );
int netrecv_timer_S( int client_index, char *data, int size );
int netrecv_login_S( int client_index, char *data, int size );
int netrecv_create_S( int client_index, char *data, int size );
int netrecv_list_S( int client_index, char *data, int size );
int netrecv_entergame_S( int client_index, char *data, int size );
int netrecv_delete_S( int client_index, char *data, int size );
int netrecv_heart_S( int client_index, char *data, int size );
int netrecv_gmcmd_S( int client_index, char *data, int size );
int netrecv_askinfo_S( int client_index, char *data, int size );
int netrecv_entercity_S( int client_index, char *data, int size );
int netrecv_chat_S( int client_index, char *data, int size );
int netrecv_storybattle_S( int client_index, char *data, int size );
int netrecv_worldmapask_S( int client_index, char *data, int size );
int netrecv_worldmapareaindex_S( int client_index, char *data, int size );
int netrecv_mapbattle_S( int client_index, char *data, int size );
int netrecv_mailask_S( int client_index, char *data, int size );
int netrecv_mailop_S( int client_index, char *data, int size );
int netrecv_mailalldel_S( int client_index, char *data, int size );
int netrecv_mailshare_S( int client_index, char *data, int size );
int netrecv_mailsend_S( int client_index, char *data, int size );
int netrecv_mailreply_S( int client_index, char *data, int size );
int netrecv_worldbossbattle_S( int client_index, char *data, int size );
int netrecv_heroguardsort_S( int client_index, char *data, int size );
int netrecv_questtalknext_S( int client_index, char *data, int size );
int netrecv_ranklistask_S( int client_index, char *data, int size );
int netrecv_friendop_S( int client_index, char *data, int size );

int netrecv_wqueue_create_S( int client_index, char *data, int size, int exec_code );
int netrecv_wqueue_list_S( int client_index, char *data, int size, int exec_code );
int netrecv_wqueue_entergame_S( int client_index, char *data, int size, int exec_code );

#endif
