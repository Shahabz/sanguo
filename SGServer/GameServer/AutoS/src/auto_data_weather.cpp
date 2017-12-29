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
#include "auto_data_weather.h"
extern MYSQL *myData;
extern lua_State* servL;
WeatherInfo *g_weather = NULL;
int g_weather_maxnum = 0;

int weatherinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`day`) from weather;" );
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
			g_weather_maxnum = atoi( row[0] ) + 1;
		else
			g_weather_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_weather = (WeatherInfo *)malloc( sizeof(WeatherInfo)*g_weather_maxnum );
	memset( g_weather, 0, sizeof(WeatherInfo)*g_weather_maxnum );

	sprintf( szSQL, "select `day`, max( `weather` ) from weather group by `day`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int day = atoi( row[0] );
		if ( day < 0 || day >= g_weather_maxnum  )
			continue;
		g_weather[day].maxnum = atoi( row[1] ) + 1;
		g_weather[day].config = (WeatherInfoConfig *)malloc( sizeof(WeatherInfoConfig)*g_weather[day].maxnum );
		memset( g_weather[day].config, 0, sizeof(WeatherInfoConfig)*g_weather[day].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `day`,`weather`,`ability`,`value`,`nextday` from weather;" );
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
		int day = atoi( row[0] );
		if ( day < 0 || day >= g_weather_maxnum  )
			continue;
		int weather = atoi( row[1] );
		if ( weather < 0 || weather >= g_weather[day].maxnum )
			continue;
		g_weather[day].config[weather].day = atoi(row[offset++]);
		g_weather[day].config[weather].weather = atoi(row[offset++]);
		g_weather[day].config[weather].ability = atoi(row[offset++]);
		g_weather[day].config[weather].value = atoi(row[offset++]);
		g_weather[day].config[weather].nextday = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int weatherinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_weather_maxnum; tmpi++ )
	{
		if ( g_weather[tmpi].config )
		{
			free( g_weather[tmpi].config );
			g_weather[tmpi].config = NULL;
		}
	}
	if ( g_weather )
	{
		free( g_weather );
		g_weather = NULL;
	}
	g_weather_maxnum = 0;
	weatherinfo_init_auto();
	return 0;
}

