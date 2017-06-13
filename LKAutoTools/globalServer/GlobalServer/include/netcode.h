#ifndef __NETCODE_H
#define __NETCODE_H

unsigned int netcode_create();
void netcode_encode( int client_index, char *dst, const char *src, int size );
void netcode_decode( int client_index, char *dst, const char *src, int size );
short netcode_decode_size( int client_index, short datasize );

short netcode_lookrecvkey( int client_index );
short netcode_looksendkey( int client_index );

#endif
