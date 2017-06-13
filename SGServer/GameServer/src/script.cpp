#ifdef WIN32
#include <conio.h>
#endif
#include <string.h>
#include <setjmp.h>
#include <time.h>

#include "script.h"
#include "actor.h"
#include "utils.h"
#include "define.h"
#include "system.h"
#include "actor_notify.h"

lua_State* servL;

extern int g_debug_mode;	// 调试模式
extern Actor *g_actors;
extern int g_maxactornum;

#ifdef WIN32
extern jmp_buf g_mark;
#endif

static int _GetCondSql_ref;

static int lua_myerror(lua_State *servL)
{
	lua_Debug ar;
	int eno;
	char *msg;
	int num = lua_gettop(servL);
	char buf[1024];
	int safecount, max_safecount;
	int ret;
	int i;

	safecount = max_safecount = 2;

	eno = (int)lua_tonumber(servL,1);
	msg = (char *)lua_tostring(servL,2);
	sprintf( buf, "[Lua Error]:(%d)%s[num:%d]\n", eno, msg, num );

	memset (&ar, 0, sizeof(ar));
	while( (ret=lua_getstack(servL, max_safecount-safecount, &ar)) != 0 && safecount-- > 0 )
	{
		lua_getinfo (servL, "Snl", &ar);

		sprintf (buf, "%s - in %s(Line:%d) - at %s\n", buf,ar.name,ar.currentline, ar.short_src);

		lua_pop(servL, 1);
		memset (&ar, 0, sizeof(ar));
	}
	//output_setmsg( buf );

	// 给所有gm发送出错信息
	msg = "server Lua error! Please give the details of the lower left corner to the developer";
	for( i = 0; i < g_maxactornum; i++ )
	{
		if( g_actors[i].admin >= 10 )
		{
			actor_notify( i, NOTIFY_NORMAL, msg );
			//system_talkto( i, buf );
		}
	}
#ifdef WIN32
	MessageBox( NULL, buf, "Error", MB_OK );
	longjmp( g_mark, 1 );
#endif

	write_gamelog( buf );
	return 0;
}

int lua_init( int totle_minute )
{
	// 先获得文件列表
	FILE *fp;
	int tmpi;
	char *pbuf;
	char *ptr;
	int filesize;
	int bufsize;

	char szLuaFile[256];

	fp = fopen( "./script/include.txt", "rb" );
	if( fp == NULL )
		return -1;
	fseek( fp, 0, SEEK_END );
	filesize = ftell( fp );
	bufsize = filesize;
	if( bufsize <= 0 )
	{
		fclose( fp );
		return -1;
	}
	pbuf = (char *)malloc( bufsize );
	if( pbuf == NULL )
	{
		fclose( fp );
		return -1;
	}
	fseek( fp, 0, SEEK_SET );
	if( fread( pbuf, 1, bufsize, fp ) != bufsize )
	{
		fclose( fp );
		return -1;
	}
	fclose( fp );

	servL = luaL_newstate();
	luaL_openlibs(servL);

	// 默认栈大小为20，修改栈大小到200试试
	if( !lua_checkstack(servL, 200) )
		return -1;

	lua_atpanic(servL,lua_myerror);

	lua_func_register();
//	lua_register(servL, "get_itemname", lua_get_itemname);

	ptr = pbuf;
	for( tmpi = 0; tmpi < bufsize; tmpi++ )
	{
		if( (*(pbuf+tmpi) == 0x0d || *(pbuf+tmpi) == 0x0a) )
		{
			*(pbuf+tmpi) = 0;
			if( *ptr != 0 )
			{
				sprintf( szLuaFile, "./script/%s", ptr );
				luaL_dofile(servL, szLuaFile );
			}
			ptr = pbuf+tmpi+1;
		}
	}
	free( pbuf );

	lua_func_function();
	lua_getglobal(servL, "IN_GetCondSql");
	_GetCondSql_ref = luaL_ref( servL, LUA_REGISTRYINDEX );

	// 
	//script_systeaminit( servL, totle_minute );
	//script_config( servL );
	return 0;
}

//其他函数
int lua_exit()
{
	lua_close(servL);
	return 0;
}

int lua_reload( int settimer )
{
	int totle_minute = 0;
//	lua_rawgeti(servL, LUA_REGISTRYINDEX, _getGlobalValue_ref );
	// 调用函数 0个参数和1个返回值
//	lua_call(servL, 0, 1);

	// 获得返回值
//	totle_minute = (int)lua_tonumber(servL, -1);
//	lua_pop(servL, 1);

	lua_exit();
	if( settimer < 0 )
		lua_init( totle_minute );
	else
		lua_init( settimer );
	return totle_minute;
}


int sc_local_GetCondSql( int cond, char *pMsg )
{
	int nResult;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _GetCondSql_ref );
	lua_pushinteger( servL, cond );
	lua_call( servL, 1, 2 );
	nResult = (int)lua_tonumber( servL, -2 );
	strncpy( pMsg, lua_tostring( servL, -1 ), 2048 );
	lua_pop( servL, 2 );
	return nResult;
}
