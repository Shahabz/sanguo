extern "C" {
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
#include "auto_data_hero_skill.h"
extern MYSQL *myData;
extern lua_State* servL;
HeroSkillInfo *g_hero_skill = NULL;
int g_hero_skill_maxnum = 0;

int heroskillinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`skillid`) from hero_skill;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if ( row[0] )
			g_hero_skill_maxnum = atoi( row[0] ) + 1;
		else
			g_hero_skill_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_hero_skill = (HeroSkillInfo *)malloc( sizeof(HeroSkillInfo)*g_hero_skill_maxnum );
	memset( g_hero_skill, 0, sizeof(HeroSkillInfo)*g_hero_skill_maxnum );

	sprintf( szSQL, "select `skillid`,`name`,`value`,`randmin`,`randmax` from hero_skill;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		int skillid = atoi( row[0] );
		if ( skillid < 0 || skillid >= g_hero_skill_maxnum  )
			continue;
		g_hero_skill[skillid].skillid = atoi(row[offset++]);
		memcpy( g_hero_skill[skillid].name, row[offset++], 255 ); g_hero_skill[skillid].name[254]=0;
		g_hero_skill[skillid].value = atoi(row[offset++]);
		g_hero_skill[skillid].randmin = atoi(row[offset++]);
		g_hero_skill[skillid].randmax = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int heroskillinfo_reload_auto()
{
	if ( g_hero_skill )
	{
		free( g_hero_skill );
		g_hero_skill = NULL;
	}
	g_hero_skill_maxnum = 0;
	heroskillinfo_init_auto();
	return 0;
}

