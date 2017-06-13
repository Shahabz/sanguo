#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include "netfunc.h"
#include "db.h"
#include "dbredis.h"
#include "hiredis.h"

extern SConfig g_Config;

redisContext *pRedisContext = NULL;

int dbredis_init()
{
	redisReply *reply;

	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	pRedisContext = redisConnectWithTimeout( g_Config.redis_host, g_Config.redis_port, timeout );
	if ( pRedisContext == NULL || pRedisContext->err ) 
	{
		if ( pRedisContext ) 
		{
			printf_msg( "Redis Connection error: %s\n", pRedisContext->errstr );
			redisFree( pRedisContext );
		}
		else 
		{
			printf( "Connection error: can't allocate redis context\n" );
		}
		return -1;
	}

	/* PING server */
	reply = (redisReply*)redisCommand( pRedisContext, "PING" );
	printf( "PING: %s\n", reply->str );
	freeReplyObject( reply );

	///* Set a key */
	//reply = redisCommand( c, "SET %s %s", "foo", "hello world" );
	//printf( "SET: %s\n", reply->str );
	//freeReplyObject( reply );

	///* Set a key using binary safe API */
	//reply = redisCommand( c, "SET %b %b", "bar", (size_t)3, "hello", (size_t)5 );
	//printf( "SET (binary API): %s\n", reply->str );
	//freeReplyObject( reply );

	///* Try a GET and two INCR */
	//reply = redisCommand( c, "GET foo" );
	//printf( "GET foo: %s\n", reply->str );
	//freeReplyObject( reply );

	//reply = redisCommand( c, "INCR counter" );
	//printf( "INCR counter: %lld\n", reply->integer );
	//freeReplyObject( reply );
	///* again ... */
	//reply = redisCommand( c, "INCR counter" );
	//printf( "INCR counter: %lld\n", reply->integer );
	//freeReplyObject( reply );

	///* Create a list of numbers, from 0 to 9 */
	//reply = redisCommand( c, "DEL mylist" );
	//freeReplyObject( reply );
	//for ( j = 0; j < 10; j++ ) {
	//	char buf[64];

	//	snprintf( buf, 64, "%d", j );
	//	reply = redisCommand( c, "LPUSH mylist element-%s", buf );
	//	freeReplyObject( reply );
	//}

	///* Let's check what we have inside the list */
	//reply = redisCommand( c, "LRANGE mylist 0 -1" );
	//if ( reply->type == REDIS_REPLY_ARRAY ) {
	//	for ( j = 0; j < reply->elements; j++ ) {
	//		printf( "%u) %s\n", j, reply->element[j]->str );
	//	}
	//}
	//freeReplyObject( reply );

	///* Disconnects and frees the context */
	//redisFree( c );
	return 0;
}

void dbredis_close()
{
	if ( pRedisContext )
	{
		redisFree( pRedisContext );
		pRedisContext = NULL;
	}
}
