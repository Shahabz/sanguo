#ifndef __SCRIPT_auto_H
#define __SCRIPT_auto_H

void lua_func_register();
void lua_func_function();

int sc_Script_Init();
int sc_Script_Timer();
int sc_Script_Command( int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg, int actor_index );
int sc_Script_Exec( int id, int value1, int value2 );
int sc_Script_Hour();
int sc_OnClockProcess( int hour );
int sc_ClientMsg( int nPlayerIndex, int nCmd, int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg );
int sc_ActivityOnOpen( int activityid );
int sc_ActivityOnLogic( int activityid );
int sc_ActivityOnEnd( int activityid );
int sc_ActivityOnClose( int activityid );
int sc_UseItem( int nPlayerIndex, int nMember, int nItemIndex, int nItemKind, int nUseNum );
int sc_OnActorIn( int nPlayerIndex );
int sc_OnMailReadAttach( int nPlayerIndex, char *szAttach );
int sc_TimeString( int timestamp , char *out, int *len );
int sc_OnWorldMapInit( int nMaxWidth, int nMaxHeight );
int sc_OnWorldMapBlock( short posx, short posy );
int sc_OnWorldMapBrush();

#endif
