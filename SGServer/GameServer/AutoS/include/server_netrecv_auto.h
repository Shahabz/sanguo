#ifndef __NETRECV_AUTO_H0
#define __NETRECV_AUTO_H0

#include "server_structrecv_auto.h"
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

int netrecv_wqueue_create_S( int client_index, char *data, int size, int exec_code );
int netrecv_wqueue_list_S( int client_index, char *data, int size, int exec_code );
int netrecv_wqueue_entergame_S( int client_index, char *data, int size, int exec_code );

#endif
