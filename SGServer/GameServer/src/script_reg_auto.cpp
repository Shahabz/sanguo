extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "script.h"
#include "define.h"
#include "thread.h"
#include "utils.h"
#include "item.h"
#include "actor_notify.h"
#include "auto_actor.h"
#include "map.h"
#include "system.h"
#include "global.h"
#include "wqueue.h"
extern lua_State* servL;

static int lua_c_item_name( lua_State *servL );
static int lua_c_item_hasone( lua_State *servL );
static int lua_c_item_emptycount( lua_State *servL );
static int lua_c_item_lost( lua_State *servL );
static int lua_c_item_lostitem( lua_State *servL );
static int lua_c_item_getitem( lua_State *servL );
static int lua_c_item_get_base_ability( lua_State *servL );
static int lua_c_item_get_base_value( lua_State *servL );
static int lua_c_item_get_ability_value( lua_State *servL );
static int lua_c_item_set_abilityvalue_offset( lua_State *servL );
static int lua_c_item_get_abilityvalue_offset( lua_State *servL );
static int lua_c_item_getinfo( lua_State *servL );
static int lua_c_actor_info( lua_State *servL );
static int lua_c_actor_notify( lua_State *servL );
static int lua_c_actor_notify_value( lua_State *servL );
static int lua_c_actor_add_today_char_times( lua_State *servL );
static int lua_c_actor_get_today_char_times( lua_State *servL );
static int lua_c_actor_set_today_char_times( lua_State *servL );
static int lua_c_actor_add_today_int_times( lua_State *servL );
static int lua_c_actor_get_today_int_times( lua_State *servL );
static int lua_c_actor_set_today_int_times( lua_State *servL );
static int lua_c_actor_set_sflag( lua_State *servL );
static int lua_c_actor_get_sflag( lua_State *servL );
static int lua_c_actor_set_uselimit_cd( lua_State *servL );
static int lua_c_actor_get_uselimit_cd( lua_State *servL );
static int lua_c_actor_add_week_times( lua_State *servL );
static int lua_c_actor_get_week_times( lua_State *servL );
static int lua_c_map_addobject( lua_State *servL );
static int lua_c_map_delobject( lua_State *servL );
static int lua_c_map_getrandpos( lua_State *servL );
static int lua_c_world_data_get( lua_State *servL );
static int lua_c_world_data_set( lua_State *servL );
static int lua_c_system_getruntime( lua_State *servL );
static int lua_c_system_getopentime( lua_State *servL );
static int lua_c_system_getfday( lua_State *servL );
static int lua_c_system_gettoday( lua_State *servL );
static int lua_c_system_getweek( lua_State *servL );
static int lua_c_system_getmonth( lua_State *servL );
static int lua_c_award_getaward( lua_State *servL );

void lua_func_register()
{
	lua_register(servL, "c_item_name", lua_c_item_name);
	lua_register(servL, "c_item_hasone", lua_c_item_hasone);
	lua_register(servL, "c_item_emptycount", lua_c_item_emptycount);
	lua_register(servL, "c_item_lost", lua_c_item_lost);
	lua_register(servL, "c_item_lostitem", lua_c_item_lostitem);
	lua_register(servL, "c_item_getitem", lua_c_item_getitem);
	lua_register(servL, "c_item_get_base_ability", lua_c_item_get_base_ability);
	lua_register(servL, "c_item_get_base_value", lua_c_item_get_base_value);
	lua_register(servL, "c_item_get_ability_value", lua_c_item_get_ability_value);
	lua_register(servL, "c_item_set_abilityvalue_offset", lua_c_item_set_abilityvalue_offset);
	lua_register(servL, "c_item_get_abilityvalue_offset", lua_c_item_get_abilityvalue_offset);
	lua_register(servL, "c_item_getinfo", lua_c_item_getinfo);
	lua_register(servL, "c_actor_info", lua_c_actor_info);
	lua_register(servL, "c_actor_notify", lua_c_actor_notify);
	lua_register(servL, "c_actor_notify_value", lua_c_actor_notify_value);
	lua_register(servL, "c_actor_add_today_char_times", lua_c_actor_add_today_char_times);
	lua_register(servL, "c_actor_get_today_char_times", lua_c_actor_get_today_char_times);
	lua_register(servL, "c_actor_set_today_char_times", lua_c_actor_set_today_char_times);
	lua_register(servL, "c_actor_add_today_int_times", lua_c_actor_add_today_int_times);
	lua_register(servL, "c_actor_get_today_int_times", lua_c_actor_get_today_int_times);
	lua_register(servL, "c_actor_set_today_int_times", lua_c_actor_set_today_int_times);
	lua_register(servL, "c_actor_set_sflag", lua_c_actor_set_sflag);
	lua_register(servL, "c_actor_get_sflag", lua_c_actor_get_sflag);
	lua_register(servL, "c_actor_set_uselimit_cd", lua_c_actor_set_uselimit_cd);
	lua_register(servL, "c_actor_get_uselimit_cd", lua_c_actor_get_uselimit_cd);
	lua_register(servL, "c_actor_add_week_times", lua_c_actor_add_week_times);
	lua_register(servL, "c_actor_get_week_times", lua_c_actor_get_week_times);
	lua_register(servL, "c_map_addobject", lua_c_map_addobject);
	lua_register(servL, "c_map_delobject", lua_c_map_delobject);
	lua_register(servL, "c_map_getrandpos", lua_c_map_getrandpos);
	lua_register(servL, "c_world_data_get", lua_c_world_data_get);
	lua_register(servL, "c_world_data_set", lua_c_world_data_set);
	lua_register(servL, "c_system_getruntime", lua_c_system_getruntime);
	lua_register(servL, "c_system_getopentime", lua_c_system_getopentime);
	lua_register(servL, "c_system_getfday", lua_c_system_getfday);
	lua_register(servL, "c_system_gettoday", lua_c_system_gettoday);
	lua_register(servL, "c_system_getweek", lua_c_system_getweek);
	lua_register(servL, "c_system_getmonth", lua_c_system_getmonth);
	lua_register(servL, "c_award_getaward", lua_c_award_getaward);
}

static int lua_c_item_name( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int itemkind = (int )lua_tointeger( servL, 1 );
	//--Process script
	char *itemname = 0;
	lua_pushstring( servL, itemname );
	return 1;
}

static int lua_c_item_hasone( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int itemkind = (int )lua_tointeger( servL, 2 );
	//--Process script
	char rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_item_emptycount( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	//--Process script
	int count = 0;
	lua_pushinteger( servL, count );
	return 1;
}

static int lua_c_item_lost( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num < 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int itemkind = (int )lua_tointeger( servL, 2 );
	int count = (int )lua_tointeger( servL, 3 );
	int path = 0; if( num > 3 ) path = (int )lua_tointeger( servL, 4 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_item_lostitem( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 4 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int itemoffset = (int )lua_tointeger( servL, 2 );
	int count = (int )lua_tointeger( servL, 3 );
	int path = (int )lua_tointeger( servL, 4 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_item_getitem( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 5 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int itemkind = (int )lua_tointeger( servL, 2 );
	int count = (int )lua_tointeger( servL, 3 );
	char color = (char )lua_tointeger( servL, 4 );
	char path = (char )lua_tointeger( servL, 5 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_item_get_base_ability( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int itemkind = (int )lua_tointeger( servL, 1 );
	char pos = (char )lua_tointeger( servL, 2 );
	//--Process script
	short ability = 0;
	lua_pushinteger( servL, ability );
	return 1;
}

static int lua_c_item_get_base_value( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int itemkind = (int )lua_tointeger( servL, 1 );
	char pos = (char )lua_tointeger( servL, 2 );
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_item_get_ability_value( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int itemkind = (int )lua_tointeger( servL, 1 );
	short ability = (short )lua_tointeger( servL, 2 );
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_item_set_abilityvalue_offset( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 5 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int item_offset = (int )lua_tointeger( servL, 2 );
	char pos = (char )lua_tointeger( servL, 3 );
	short ability = (short )lua_tointeger( servL, 4 );
	int value = (int )lua_tointeger( servL, 5 );
	//--Process script
	return 0;
}

static int lua_c_item_get_abilityvalue_offset( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int item_offset = (int )lua_tointeger( servL, 2 );
	char pos = (char )lua_tointeger( servL, 3 );
	//--Process script
	short rtn_ability = 0;
	int rtn_value = 0;
	lua_pushinteger( servL, rtn_ability );
	lua_pushinteger( servL, rtn_value );
	return 2;
}

static int lua_c_item_getinfo( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int itemkind = (int )lua_tointeger( servL, 1 );
	//--Process script
	char *name = 0;
	short level = 0;
	char color = 0;
	int price = 0;
	int type = 0;
	lua_pushstring( servL, name );
	lua_pushinteger( servL, level );
	lua_pushinteger( servL, color );
	lua_pushinteger( servL, price );
	lua_pushinteger( servL, type );
	return 5;
}

static int lua_c_actor_info( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	//--Process script
	int actorid = 0;
	int aclass = 0;
	int level = 0;
	char *name = 0;
	short language = 0;
	lua_pushinteger( servL, actorid );
	lua_pushinteger( servL, aclass );
	lua_pushinteger( servL, level );
	lua_pushstring( servL, name );
	lua_pushinteger( servL, language );
	return 5;
}

static int lua_c_actor_notify( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num < 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	char *msg = (char *)lua_tostring( servL, 2 );
	int msgid = 0; if( num > 2 ) msgid = (int )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_actor_notify_value( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num < 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	char *msg = (char *)lua_tostring( servL, 2 );
	int msgid = (int )lua_tointeger( servL, 3 );
	int value1 = 0; if( num > 3 ) value1 = (int )lua_tointeger( servL, 4 );
	int value2 = 0; if( num > 4 ) value2 = (int )lua_tointeger( servL, 5 );
	int value3 = 0; if( num > 5 ) value3 = (int )lua_tointeger( servL, 6 );
	int value4 = 0; if( num > 6 ) value4 = (int )lua_tointeger( servL, 7 );
	int value5 = 0; if( num > 7 ) value5 = (int )lua_tointeger( servL, 8 );
	int value6 = 0; if( num > 8 ) value6 = (int )lua_tointeger( servL, 9 );
	int value7 = 0; if( num > 9 ) value7 = (int )lua_tointeger( servL, 10 );
	int value8 = 0; if( num > 10 ) value8 = (int )lua_tointeger( servL, 11 );
	int value9 = 0; if( num > 11 ) value9 = (int )lua_tointeger( servL, 12 );
	int value10 = 0; if( num > 12 ) value10 = (int )lua_tointeger( servL, 13 );
	//--Process script
	return 0;
}

static int lua_c_actor_add_today_char_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	//--Process script
	return 0;
}

static int lua_c_actor_get_today_char_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_actor_set_today_char_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	int value = (int )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_actor_add_today_int_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	//--Process script
	return 0;
}

static int lua_c_actor_get_today_int_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_actor_set_today_int_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	int value = (int )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_actor_set_sflag( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	short value = (short )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_actor_get_sflag( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	short offset = (short )lua_tointeger( servL, 2 );
	//--Process script
	short value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_actor_set_uselimit_cd( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	char index = (char )lua_tointeger( servL, 2 );
	int cdtime = (int )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_actor_get_uselimit_cd( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	char index = (char )lua_tointeger( servL, 2 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_actor_add_week_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int offset = (int )lua_tointeger( servL, 2 );
	//--Process script
	return 0;
}

static int lua_c_actor_get_week_times( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int offset = (int )lua_tointeger( servL, 2 );
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_map_addobject( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 4 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	char type = (char )lua_tointeger( servL, 1 );
	short posx = (short )lua_tointeger( servL, 2 );
	short posy = (short )lua_tointeger( servL, 3 );
	char unittype = (char )lua_tointeger( servL, 4 );
	//--Process script
	int rtn = 0;
	lua_pushinteger( servL, rtn );
	return 1;
}

static int lua_c_map_delobject( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 3 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	char type = (char )lua_tointeger( servL, 1 );
	short posx = (short )lua_tointeger( servL, 2 );
	short posy = (short )lua_tointeger( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_map_getrandpos( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	char type = (char )lua_tointeger( servL, 1 );
	//--Process script
	short posx = 0;
	short posy = 0;
	lua_pushinteger( servL, posx );
	lua_pushinteger( servL, posy );
	return 2;
}

static int lua_c_world_data_get( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 1 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int id = (int )lua_tointeger( servL, 1 );
	//--Process script
	int value = 0;
	char *strvalue = 0;
	lua_pushinteger( servL, value );
	lua_pushstring( servL, strvalue );
	return 2;
}

static int lua_c_world_data_set( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num < 2 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int id = (int )lua_tointeger( servL, 1 );
	int value = (int )lua_tointeger( servL, 2 );
	char *strvalue = 0; if( num > 2 ) strvalue = (char *)lua_tostring( servL, 3 );
	//--Process script
	return 0;
}

static int lua_c_system_getruntime( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int time = 0;
	lua_pushinteger( servL, time );
	return 1;
}

static int lua_c_system_getopentime( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int time = 0;
	lua_pushinteger( servL, time );
	return 1;
}

static int lua_c_system_getfday( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_system_gettoday( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_system_getweek( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_system_getmonth( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 0 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	//--Process script
	int value = 0;
	lua_pushinteger( servL, value );
	return 1;
}

static int lua_c_award_getaward( lua_State *servL )
{
	int num = lua_gettop(servL);
	if ( num != 5 )
	{
		char szErrorMsg[128];
		sprintf( szErrorMsg, "Incorrect argument to function '%s'", __FUNCTION__ );
		lua_pushstring( servL, szErrorMsg );
		lua_error( servL );
		return 0;
	}
	int actor_index = (int )lua_tointeger( servL, 1 );
	int kind = (int )lua_tointeger( servL, 2 );
	int count = (int )lua_tointeger( servL, 3 );
	char color = (char )lua_tointeger( servL, 4 );
	char path = (char )lua_tointeger( servL, 5 );
	//--Process script
	return 0;
}

