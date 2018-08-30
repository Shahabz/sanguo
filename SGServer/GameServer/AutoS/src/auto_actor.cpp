#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor.h"
extern MYSQL *myGame;
int actor_load_auto( int actorid, Actor *pActor, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	if ( pActor == NULL )
		return -1;

	sprintf( szSQL, "select `actorid`,`admin`,`lastip`,`createtime`,`forbidtime`,`name`,`nation`,`shape`,`level`,`token`,`total_charge`,`charge_dollar`,`itemext`,`equipext`,`quest_talkid`,`worldquest_complete`,`blacklist`,`sflag`,`fdate`,`today_char`,`today_short`,`today_int0`,`today_int1`,`today_int2`,`today_int3`,`today_int4`,`today_int5`,`today_int6`,`today_int7`,`cd0`,`cd1`,`cd2`,`cd3`,`cd4`,`cd5`,`cd6`,`cd7`,`config`,`permission`,`storyid`,`story_star`,`story_hero`,`story_resnum`,`story_restime`,`story_resreset`,`story_itemnum`,`story_drawing`,`story_sweephero`,`lastpeople_c`,`lastpeople_n`,`cdkey1`,`cdkey2`,`cdkey3`,`cdkey4`,`hv_ln`,`hv_hn`,`hv_pro`,`vipbag`,`shop_useitem`,`charge_point`,`pay_maxtier`,`shop_saleitem`,`neq_crit`,`mapcallfree`,`wishingid`,`wishingopen`,`wishingday`,`wishingcd`,`shape_bag`,`student`,`te_award`,`te_awarded`,`te_shop`,`act02_state`,`act03_state`,`act04_state`,`act04_value`,`act05_item`,`act05_isbuy`,`act05_buynum`,`act05_xw`,`act05_upnum`,`act05_upstamp`,`act08_state`,`fs_nodeid`,`fs_awardkind`,`fs_awardnum`,`fs_awardfday`,`fs_weekkind`,`fs_weeknum`,`fs_fweek`,`girlshop`,`girlshop_buy`,`girlshop_fday`,`maidname`,`token_sale`,`token_ret`,`act25_point`,`act25_isget`,`edquest_point` from %s where actorid='%d'", pTab, actorid );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		pActor->actorid = atoi(row[offset++]);
		pActor->admin = atoi(row[offset++]);
		memcpy( pActor->lastip, row[offset++], 16 ); pActor->lastip[15]=0;
		pActor->createtime = atoi(row[offset++]);
		pActor->forbidtime = atoi(row[offset++]);
		memcpy( pActor->name, row[offset++], 22 ); pActor->name[21]=0;
		pActor->nation = atoi(row[offset++]);
		pActor->shape = atoi(row[offset++]);
		pActor->level = atoi(row[offset++]);
		pActor->token = atoi(row[offset++]);
		pActor->total_charge = atoi(row[offset++]);
		pActor->charge_dollar = (float)atof(row[offset++]);
		pActor->itemext = atoi(row[offset++]);
		pActor->equipext = atoi(row[offset++]);
		pActor->quest_talkid = atoi(row[offset++]);
		pActor->worldquest_complete = atoi(row[offset++]);
		memcpy( pActor->blacklist, row[offset++], sizeof(int)*50 );
		pActor->sflag = atoi(row[offset++]);
		pActor->fdate = atoi(row[offset++]);
		memcpy( pActor->today_char, row[offset++], sizeof(char)*128 );
		memcpy( pActor->today_short, row[offset++], sizeof(short)*32 );
		pActor->today_int[0] = atoi(row[offset++]);
		pActor->today_int[1] = atoi(row[offset++]);
		pActor->today_int[2] = atoi(row[offset++]);
		pActor->today_int[3] = atoi(row[offset++]);
		pActor->today_int[4] = atoi(row[offset++]);
		pActor->today_int[5] = atoi(row[offset++]);
		pActor->today_int[6] = atoi(row[offset++]);
		pActor->today_int[7] = atoi(row[offset++]);
		pActor->cd[0] = atoi(row[offset++]);
		pActor->cd[1] = atoi(row[offset++]);
		pActor->cd[2] = atoi(row[offset++]);
		pActor->cd[3] = atoi(row[offset++]);
		pActor->cd[4] = atoi(row[offset++]);
		pActor->cd[5] = atoi(row[offset++]);
		pActor->cd[6] = atoi(row[offset++]);
		pActor->cd[7] = atoi(row[offset++]);
		memcpy( pActor->config, row[offset++], sizeof(char)*8 );
		pActor->permission = atoi(row[offset++]);
		pActor->storyid = atoi(row[offset++]);
		memcpy( pActor->story_star, row[offset++], sizeof(char)*128 );
		memcpy( pActor->story_hero, row[offset++], sizeof(char)*32 );
		memcpy( pActor->story_resnum, row[offset++], sizeof(char)*32 );
		memcpy( pActor->story_restime, row[offset++], sizeof(int)*32 );
		memcpy( pActor->story_resreset, row[offset++], sizeof(char)*32 );
		memcpy( pActor->story_itemnum, row[offset++], sizeof(char)*64 );
		memcpy( pActor->story_drawing, row[offset++], sizeof(char)*16 );
		memcpy( pActor->story_sweephero, row[offset++], sizeof(short)*4 );
		pActor->lastpeople_c = atoi(row[offset++]);
		pActor->lastpeople_n = atoi(row[offset++]);
		pActor->cdkey1 = atoi(row[offset++]);
		pActor->cdkey2 = atoi(row[offset++]);
		pActor->cdkey3 = atoi(row[offset++]);
		pActor->cdkey4 = atoi(row[offset++]);
		pActor->hv_ln = atoi(row[offset++]);
		pActor->hv_hn = atoi(row[offset++]);
		pActor->hv_pro = atoi(row[offset++]);
		pActor->vipbag = atoi(row[offset++]);
		pActor->shop_useitem = atoi(row[offset++]);
		pActor->charge_point = atoi(row[offset++]);
		pActor->pay_maxtier = atoi(row[offset++]);
		memcpy( pActor->shop_saleitem, row[offset++], sizeof(short)*6 );
		memcpy( pActor->neq_crit, row[offset++], sizeof(char)*6 );
		pActor->mapcallfree = atoi(row[offset++]);
		memcpy( pActor->wishingid, row[offset++], sizeof(short)*9 );
		memcpy( pActor->wishingopen, row[offset++], sizeof(char)*9 );
		pActor->wishingday = atoi(row[offset++]);
		pActor->wishingcd = atoi(row[offset++]);
		pActor->shape_bag = atoi(row[offset++]);
		memcpy( pActor->student, row[offset++], sizeof(int)*30 );
		memcpy( pActor->te_award, row[offset++], sizeof(char)*10 );
		memcpy( pActor->te_awarded, row[offset++], sizeof(char)*10 );
		pActor->te_shop = atoi(row[offset++]);
		pActor->act02_state = atoi(row[offset++]);
		pActor->act03_state = atoi(row[offset++]);
		memcpy( pActor->act04_state, row[offset++], sizeof(char)*128 );
		memcpy( pActor->act04_value, row[offset++], sizeof(int)*40 );
		memcpy( pActor->act05_item, row[offset++], sizeof(short)*6 );
		pActor->act05_isbuy = atoi(row[offset++]);
		pActor->act05_buynum = atoi(row[offset++]);
		pActor->act05_xw = atoi(row[offset++]);
		pActor->act05_upnum = atoi(row[offset++]);
		pActor->act05_upstamp = atoi(row[offset++]);
		pActor->act08_state = atoi(row[offset++]);
		pActor->fs_nodeid = atoi(row[offset++]);
		memcpy( pActor->fs_awardkind, row[offset++], sizeof(int)*14 );
		memcpy( pActor->fs_awardnum, row[offset++], sizeof(int)*14 );
		pActor->fs_awardfday = atoi(row[offset++]);
		memcpy( pActor->fs_weekkind, row[offset++], sizeof(int)*5 );
		memcpy( pActor->fs_weeknum, row[offset++], sizeof(int)*5 );
		pActor->fs_fweek = atoi(row[offset++]);
		memcpy( pActor->girlshop, row[offset++], sizeof(short)*9 );
		pActor->girlshop_buy = atoi(row[offset++]);
		pActor->girlshop_fday = atoi(row[offset++]);
		memcpy( pActor->maidname, row[offset++], 22 ); pActor->maidname[21]=0;
		pActor->token_sale = atoi(row[offset++]);
		pActor->token_ret = atoi(row[offset++]);
		pActor->act25_point = atoi(row[offset++]);
		pActor->act25_isget = atoi(row[offset++]);
		pActor->edquest_point = atoi(row[offset++]);
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}
int actor_save_auto( Actor *pActor, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pActor == NULL )
		return -1;

	char szText_lastip[MAX_PATH]={0};
	char szText_name[MAX_PATH]={0};
	char szText_blacklist[sizeof(int)*50*2+1]={0};
	char szText_today_char[sizeof(char)*128*2+1]={0};
	char szText_today_short[sizeof(short)*32*2+1]={0};
	char szText_config[sizeof(char)*8*2+1]={0};
	char szText_story_star[sizeof(char)*128*2+1]={0};
	char szText_story_hero[sizeof(char)*32*2+1]={0};
	char szText_story_resnum[sizeof(char)*32*2+1]={0};
	char szText_story_restime[sizeof(int)*32*2+1]={0};
	char szText_story_resreset[sizeof(char)*32*2+1]={0};
	char szText_story_itemnum[sizeof(char)*64*2+1]={0};
	char szText_story_drawing[sizeof(char)*16*2+1]={0};
	char szText_story_sweephero[sizeof(short)*4*2+1]={0};
	char szText_shop_saleitem[sizeof(short)*6*2+1]={0};
	char szText_neq_crit[sizeof(char)*6*2+1]={0};
	char szText_wishingid[sizeof(short)*9*2+1]={0};
	char szText_wishingopen[sizeof(char)*9*2+1]={0};
	char szText_student[sizeof(int)*30*2+1]={0};
	char szText_te_award[sizeof(char)*10*2+1]={0};
	char szText_te_awarded[sizeof(char)*10*2+1]={0};
	char szText_act04_state[sizeof(char)*128*2+1]={0};
	char szText_act04_value[sizeof(int)*40*2+1]={0};
	char szText_act05_item[sizeof(short)*6*2+1]={0};
	char szText_fs_awardkind[sizeof(int)*14*2+1]={0};
	char szText_fs_awardnum[sizeof(int)*14*2+1]={0};
	char szText_fs_weekkind[sizeof(int)*5*2+1]={0};
	char szText_fs_weeknum[sizeof(int)*5*2+1]={0};
	char szText_girlshop[sizeof(short)*9*2+1]={0};
	char szText_maidname[MAX_PATH]={0};
RE_ACTOR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`lastip`,`createtime`,`forbidtime`,`name`,`nation`,`shape`,`level`,`token`,`total_charge`,`charge_dollar`,`itemext`,`equipext`,`quest_talkid`,`worldquest_complete`,`blacklist`,`sflag`,`fdate`,`today_char`,`today_short`,`today_int0`,`today_int1`,`today_int2`,`today_int3`,`today_int4`,`today_int5`,`today_int6`,`today_int7`,`cd0`,`cd1`,`cd2`,`cd3`,`cd4`,`cd5`,`cd6`,`cd7`,`config`,`permission`,`storyid`,`story_star`,`story_hero`,`story_resnum`,`story_restime`,`story_resreset`,`story_itemnum`,`story_drawing`,`story_sweephero`,`lastpeople_c`,`lastpeople_n`,`cdkey1`,`cdkey2`,`cdkey3`,`cdkey4`,`hv_ln`,`hv_hn`,`hv_pro`,`vipbag`,`shop_useitem`,`charge_point`,`pay_maxtier`,`shop_saleitem`,`neq_crit`,`mapcallfree`,`wishingid`,`wishingopen`,`wishingday`,`wishingcd`,`shape_bag`,`student`,`te_award`,`te_awarded`,`te_shop`,`act02_state`,`act03_state`,`act04_state`,`act04_value`,`act05_item`,`act05_isbuy`,`act05_buynum`,`act05_xw`,`act05_upnum`,`act05_upstamp`,`act08_state`,`fs_nodeid`,`fs_awardkind`,`fs_awardnum`,`fs_awardfday`,`fs_weekkind`,`fs_weeknum`,`fs_fweek`,`girlshop`,`girlshop_buy`,`girlshop_fday`,`maidname`,`token_sale`,`token_ret`,`act25_point`,`act25_isget`,`edquest_point`) Values('%d','%s','%d','%d','%s','%d','%d','%d','%d','%d','%f','%d','%d','%d','%d','%s','%d','%d','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d','%s','%s','%s','%s','%s','%s','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%s','%d','%s','%s','%d','%d','%d','%s','%s','%s','%d','%d','%d','%s','%s','%s','%d','%d','%d','%d','%d','%d','%d','%s','%s','%d','%s','%s','%d','%s','%d','%d','%s','%d','%d','%d','%d','%d')",pTab,pActor->actorid,db_escape((const char *)pActor->lastip,szText_lastip,0),pActor->createtime,pActor->forbidtime,db_escape((const char *)pActor->name,szText_name,0),pActor->nation,pActor->shape,pActor->level,pActor->token,pActor->total_charge,pActor->charge_dollar,pActor->itemext,pActor->equipext,pActor->quest_talkid,pActor->worldquest_complete,db_escape((const char *)pActor->blacklist,szText_blacklist,sizeof(int)*50),pActor->sflag,pActor->fdate,db_escape((const char *)pActor->today_char,szText_today_char,sizeof(char)*128),db_escape((const char *)pActor->today_short,szText_today_short,sizeof(short)*32),pActor->today_int[0],pActor->today_int[1],pActor->today_int[2],pActor->today_int[3],pActor->today_int[4],pActor->today_int[5],pActor->today_int[6],pActor->today_int[7],pActor->cd[0],pActor->cd[1],pActor->cd[2],pActor->cd[3],pActor->cd[4],pActor->cd[5],pActor->cd[6],pActor->cd[7],db_escape((const char *)pActor->config,szText_config,sizeof(char)*8),pActor->permission,pActor->storyid,db_escape((const char *)pActor->story_star,szText_story_star,sizeof(char)*128),db_escape((const char *)pActor->story_hero,szText_story_hero,sizeof(char)*32),db_escape((const char *)pActor->story_resnum,szText_story_resnum,sizeof(char)*32),db_escape((const char *)pActor->story_restime,szText_story_restime,sizeof(int)*32),db_escape((const char *)pActor->story_resreset,szText_story_resreset,sizeof(char)*32),db_escape((const char *)pActor->story_itemnum,szText_story_itemnum,sizeof(char)*64),db_escape((const char *)pActor->story_drawing,szText_story_drawing,sizeof(char)*16),db_escape((const char *)pActor->story_sweephero,szText_story_sweephero,sizeof(short)*4),pActor->lastpeople_c,pActor->lastpeople_n,pActor->cdkey1,pActor->cdkey2,pActor->cdkey3,pActor->cdkey4,pActor->hv_ln,pActor->hv_hn,pActor->hv_pro,pActor->vipbag,pActor->shop_useitem,pActor->charge_point,pActor->pay_maxtier,db_escape((const char *)pActor->shop_saleitem,szText_shop_saleitem,sizeof(short)*6),db_escape((const char *)pActor->neq_crit,szText_neq_crit,sizeof(char)*6),pActor->mapcallfree,db_escape((const char *)pActor->wishingid,szText_wishingid,sizeof(short)*9),db_escape((const char *)pActor->wishingopen,szText_wishingopen,sizeof(char)*9),pActor->wishingday,pActor->wishingcd,pActor->shape_bag,db_escape((const char *)pActor->student,szText_student,sizeof(int)*30),db_escape((const char *)pActor->te_award,szText_te_award,sizeof(char)*10),db_escape((const char *)pActor->te_awarded,szText_te_awarded,sizeof(char)*10),pActor->te_shop,pActor->act02_state,pActor->act03_state,db_escape((const char *)pActor->act04_state,szText_act04_state,sizeof(char)*128),db_escape((const char *)pActor->act04_value,szText_act04_value,sizeof(int)*40),db_escape((const char *)pActor->act05_item,szText_act05_item,sizeof(short)*6),pActor->act05_isbuy,pActor->act05_buynum,pActor->act05_xw,pActor->act05_upnum,pActor->act05_upstamp,pActor->act08_state,pActor->fs_nodeid,db_escape((const char *)pActor->fs_awardkind,szText_fs_awardkind,sizeof(int)*14),db_escape((const char *)pActor->fs_awardnum,szText_fs_awardnum,sizeof(int)*14),pActor->fs_awardfday,db_escape((const char *)pActor->fs_weekkind,szText_fs_weekkind,sizeof(int)*5),db_escape((const char *)pActor->fs_weeknum,szText_fs_weeknum,sizeof(int)*5),pActor->fs_fweek,db_escape((const char *)pActor->girlshop,szText_girlshop,sizeof(short)*9),pActor->girlshop_buy,pActor->girlshop_fday,db_escape((const char *)pActor->maidname,szText_maidname,0),pActor->token_sale,pActor->token_ret,pActor->act25_point,pActor->act25_isget,pActor->edquest_point);
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myGame) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_ACTOR_UPDATE;
		}
		return -1;
	}
	return 0;
}

