extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "script.h"
#include "define.h"
#include "thread.h"
#include "utils.h"

extern lua_State* servL;
//--Automatically define function

static int _Script_Init_ref;
static int _Script_Timer_ref;
static int _Script_Command_ref;
static int _Script_Exec_ref;
static int _Script_Hour_ref;
static int _OnClockProcess_ref;
static int _ClientMsg_ref;
static int _ActivityOnOpen_ref;
static int _ActivityOnLogic_ref;
static int _ActivityOnEnd_ref;
static int _ActivityOnClose_ref;
static int _UseItem_ref;
static int _OnActorIn_ref;
static int _OnMailReadAttach_ref;
static int _TimeString_ref;
static int _OnWorldMapInit_ref;
static int _OnWorldMapBlock_ref;
static int _OnWorldMapBrush_ref;

void lua_func_function()
{
	lua_getglobal(servL, "IN_Script_Init");
	_Script_Init_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Timer");
	_Script_Timer_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Command");
	_Script_Command_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Exec");
	_Script_Exec_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Hour");
	_Script_Hour_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnClockProcess");
	_OnClockProcess_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_ClientMsg");
	_ClientMsg_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_ActivityOnOpen");
	_ActivityOnOpen_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_ActivityOnLogic");
	_ActivityOnLogic_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_ActivityOnEnd");
	_ActivityOnEnd_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_ActivityOnClose");
	_ActivityOnClose_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_UseItem");
	_UseItem_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnActorIn");
	_OnActorIn_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnMailReadAttach");
	_OnMailReadAttach_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_TimeString");
	_TimeString_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnWorldMapInit");
	_OnWorldMapInit_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnWorldMapBlock");
	_OnWorldMapBlock_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_OnWorldMapBrush");
	_OnWorldMapBrush_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
}

int sc_Script_Init()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Init_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Timer()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Timer_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Command( int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg, int actor_index )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Command_ref );
	lua_pushinteger( servL, nValue1 );
	lua_pushinteger( servL, nValue2 );
	lua_pushinteger( servL, nValue3 );
	lua_pushinteger( servL, nValue4 );
	lua_pushstring( servL, pMsg );
	lua_pushinteger( servL, actor_index );
	lua_call( servL, 6, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Exec( int id, int value1, int value2 )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Exec_ref );
	lua_pushinteger( servL, id );
	lua_pushinteger( servL, value1 );
	lua_pushinteger( servL, value2 );
	lua_call( servL, 3, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Hour()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Hour_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_OnClockProcess( int hour )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnClockProcess_ref );
	lua_pushinteger( servL, hour );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_ClientMsg( int nPlayerIndex, int nCmd, int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _ClientMsg_ref );
	lua_pushinteger( servL, nPlayerIndex );
	lua_pushinteger( servL, nCmd );
	lua_pushinteger( servL, nValue1 );
	lua_pushinteger( servL, nValue2 );
	lua_pushinteger( servL, nValue3 );
	lua_pushinteger( servL, nValue4 );
	lua_pushstring( servL, pMsg );
	lua_call( servL, 7, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_ActivityOnOpen( int activityid )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _ActivityOnOpen_ref );
	lua_pushinteger( servL, activityid );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_ActivityOnLogic( int activityid )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _ActivityOnLogic_ref );
	lua_pushinteger( servL, activityid );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_ActivityOnEnd( int activityid )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _ActivityOnEnd_ref );
	lua_pushinteger( servL, activityid );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_ActivityOnClose( int activityid )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _ActivityOnClose_ref );
	lua_pushinteger( servL, activityid );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_UseItem( int nPlayerIndex, int nMember, int nItemIndex, int nItemKind, int nUseNum )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _UseItem_ref );
	lua_pushinteger( servL, nPlayerIndex );
	lua_pushinteger( servL, nMember );
	lua_pushinteger( servL, nItemIndex );
	lua_pushinteger( servL, nItemKind );
	lua_pushinteger( servL, nUseNum );
	lua_call( servL, 5, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_OnActorIn( int nPlayerIndex )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnActorIn_ref );
	lua_pushinteger( servL, nPlayerIndex );
	lua_call( servL, 1, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_OnMailReadAttach( int nPlayerIndex, char *szAttach )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnMailReadAttach_ref );
	lua_pushinteger( servL, nPlayerIndex );
	lua_pushstring( servL, szAttach );
	lua_call( servL, 2, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_TimeString( int timestamp , char *out, int *len )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _TimeString_ref );
	lua_pushinteger( servL, timestamp );
	lua_call( servL, 1, 2 );
	strcpy( out, (char*)lua_tostring( servL, -2 ) );
	*len = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 2 );
	return nResult;
}

int sc_OnWorldMapInit( int nMaxWidth, int nMaxHeight )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnWorldMapInit_ref );
	lua_pushinteger( servL, nMaxWidth );
	lua_pushinteger( servL, nMaxHeight );
	lua_call( servL, 2, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_OnWorldMapBlock( short posx, short posy )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnWorldMapBlock_ref );
	lua_pushinteger( servL, posx );
	lua_pushinteger( servL, posy );
	lua_call( servL, 2, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_OnWorldMapBrush()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _OnWorldMapBrush_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

