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
#include "auto_data_paygoods.h"
extern MYSQL *myData;
extern lua_State* servL;
PayGoods *g_paygoods = NULL;
int g_paygoods_maxnum = 0;

int paygoods_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`goodsid`) from paygoods;" );
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
			g_paygoods_maxnum = atoi( row[0] ) + 1;
		else
			g_paygoods_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_paygoods = (PayGoods *)malloc( sizeof(PayGoods)*g_paygoods_maxnum );
	memset( g_paygoods, 0, sizeof(PayGoods)*g_paygoods_maxnum );

	sprintf( szSQL, "select `goodsid`,`tier`,`point`,`token`,`awardgroup`,`type`,`nameid`,`descid`,`icon`,`sale`,`worth`,`day`,`nextid`,`nextcd`,`nextcount`,`productid_a`,`productid_b`,`productid_c`,`productid_d`,`productid_e`,`productid_f`,`productid_g`,`productid_h`,`productid_i`,`productid_j`,`productid_k`,`productid_l`,`productid_m`,`productid_n`,`productid_o` from paygoods;" );
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
		int goodsid = atoi( row[0] );
		if ( goodsid < 0 || goodsid >= g_paygoods_maxnum  )
			continue;
		g_paygoods[goodsid].goodsid = atoi(row[offset++]);
		g_paygoods[goodsid].tier = atoi(row[offset++]);
		g_paygoods[goodsid].point = atoi(row[offset++]);
		g_paygoods[goodsid].token = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup = atoi(row[offset++]);
		g_paygoods[goodsid].type = atoi(row[offset++]);
		g_paygoods[goodsid].nameid = atoi(row[offset++]);
		g_paygoods[goodsid].descid = atoi(row[offset++]);
		g_paygoods[goodsid].icon = atoi(row[offset++]);
		g_paygoods[goodsid].sale = atoi(row[offset++]);
		g_paygoods[goodsid].worth = atoi(row[offset++]);
		g_paygoods[goodsid].day = atoi(row[offset++]);
		g_paygoods[goodsid].nextid = atoi(row[offset++]);
		g_paygoods[goodsid].nextcd = atoi(row[offset++]);
		g_paygoods[goodsid].nextcount = atoi(row[offset++]);
		memcpy( g_paygoods[goodsid].productid_a, row[offset++], 64 ); g_paygoods[goodsid].productid_a[63]=0;
		memcpy( g_paygoods[goodsid].productid_b, row[offset++], 64 ); g_paygoods[goodsid].productid_b[63]=0;
		memcpy( g_paygoods[goodsid].productid_c, row[offset++], 64 ); g_paygoods[goodsid].productid_c[63]=0;
		memcpy( g_paygoods[goodsid].productid_d, row[offset++], 64 ); g_paygoods[goodsid].productid_d[63]=0;
		memcpy( g_paygoods[goodsid].productid_e, row[offset++], 64 ); g_paygoods[goodsid].productid_e[63]=0;
		memcpy( g_paygoods[goodsid].productid_f, row[offset++], 64 ); g_paygoods[goodsid].productid_f[63]=0;
		memcpy( g_paygoods[goodsid].productid_g, row[offset++], 64 ); g_paygoods[goodsid].productid_g[63]=0;
		memcpy( g_paygoods[goodsid].productid_h, row[offset++], 64 ); g_paygoods[goodsid].productid_h[63]=0;
		memcpy( g_paygoods[goodsid].productid_i, row[offset++], 64 ); g_paygoods[goodsid].productid_i[63]=0;
		memcpy( g_paygoods[goodsid].productid_j, row[offset++], 64 ); g_paygoods[goodsid].productid_j[63]=0;
		memcpy( g_paygoods[goodsid].productid_k, row[offset++], 64 ); g_paygoods[goodsid].productid_k[63]=0;
		memcpy( g_paygoods[goodsid].productid_l, row[offset++], 64 ); g_paygoods[goodsid].productid_l[63]=0;
		memcpy( g_paygoods[goodsid].productid_m, row[offset++], 64 ); g_paygoods[goodsid].productid_m[63]=0;
		memcpy( g_paygoods[goodsid].productid_n, row[offset++], 64 ); g_paygoods[goodsid].productid_n[63]=0;
		memcpy( g_paygoods[goodsid].productid_o, row[offset++], 64 ); g_paygoods[goodsid].productid_o[63]=0;
	}
	mysql_free_result( res );
	return 0;
}

int paygoods_reload_auto()
{
	if ( g_paygoods )
	{
		free( g_paygoods );
		g_paygoods = NULL;
	}
	g_paygoods_maxnum = 0;
	paygoods_init_auto();
	return 0;
}

