#ifndef __SCRIPT_H
#define __SCRIPT_H
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "script_auto.h"

int lua_init( int totle_minute );
int lua_exit();
int lua_reload( int settimer );
int sc_local_GetCondSql( int cond, char *pMsg );


#endif
