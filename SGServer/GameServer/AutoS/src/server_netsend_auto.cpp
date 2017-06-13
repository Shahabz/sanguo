#include "server_struct_auto.h"
#include "server_netsend_auto.h"
#include "netfunc.h"
#include "actor_send.h"


extern int g_maxactornum;

int netsend_login_S( int actor_index, char send_type, SLK_NetS_Login *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LOGIN;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LOGIN; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Login_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_list_S( int actor_index, char send_type, SLK_NetS_List *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_List_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_create_S( int actor_index, char send_type, SLK_NetS_Create *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CREATE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CREATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Create_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_enterinfo_S( int actor_index, char send_type, SLK_NetS_EnterInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ENTERINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ENTERINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EnterInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_delete_S( int actor_index, char send_type, SLK_NetS_Delete *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_DELETE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_DELETE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Delete_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heart_S( int actor_index, char send_type, SLK_NetS_Heart *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEART;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEART; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Heart_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_notify_S( int actor_index, char send_type, SLK_NetS_Notify *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_NOTIFY;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_NOTIFY; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Notify_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_actorinfo_S( int actor_index, char send_type, SLK_NetS_ActorInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ACTORINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ACTORINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ActorInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemuse_S( int actor_index, char send_type, SLK_NetS_ItemUse *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMUSE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMUSE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemUse_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemput_S( int actor_index, char send_type, SLK_NetS_ItemPut *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMPUT;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMPUT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemPut_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemsettle_S( int actor_index, char send_type, SLK_NetS_ItemSettle *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMSETTLE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMSETTLE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemSettle_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_lostitem_S( int actor_index, char send_type, SLK_NetS_LostItem *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LOSTITEM;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LOSTITEM; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_LostItem_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_getitem_S( int actor_index, char send_type, SLK_NetS_GetItem *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_GETITEM;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_GETITEM; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_GetItem_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemlist_S( int actor_index, char send_type, SLK_NetS_ItemList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_iteminfo_S( int actor_index, char send_type, SLK_NetS_ItemInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_addmapunit_S( char *databuf, int size, SLK_NetS_AddMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ADDMAPUNIT;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ADDMAPUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_AddMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_delmapunit_S( char *databuf, int size, SLK_NetS_DelMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_DELMAPUNIT;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_DELMAPUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_DelMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_updatemapunit_S( char *databuf, int size, SLK_NetS_UpdateMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_UPDATEMAPUNIT;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_UPDATEMAPUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_UpdateMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_mapunitsingle_S( int actor_index, char send_type, SLK_NetS_AddMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPUNITSINGLE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPUNITSINGLE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_AddMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_worldmapinfo_S( int actor_index, char send_type, SLK_NetS_WorldMapInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_WORLDMAPINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_WORLDMAPINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_WorldMapInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapunitcorrdinate_S( char *databuf, int size, SLK_NetS_MapUnitCorrdinate *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPUNITCORRDINATE;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPUNITCORRDINATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MapUnitCorrdinate_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_addmarchroute_S( char *databuf, int size, SLK_NetS_AddMarchRoute *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ADDMARCHROUTE;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ADDMARCHROUTE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_AddMarchRoute_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_delmarchroute_S( char *databuf, int size, SLK_NetS_DelMarchRoute *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_DELMARCHROUTE;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_DELMARCHROUTE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_DelMarchRoute_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_awardinfolist_S( int actor_index, char send_type, SLK_NetS_AwardInfoList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_AWARDINFOLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_AWARDINFOLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_AwardInfoList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_experience_S( int actor_index, char send_type, SLK_NetS_Experience *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EXPERIENCE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EXPERIENCE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Experience_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_body_S( int actor_index, char send_type, SLK_NetS_Body *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BODY;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BODY; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Body_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}


int netsend_userawarded_S( int actor_index, char send_type, SLK_NetU_UserAwarded *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_USERAWARDED;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_USERAWARDED; ptr+=sizeof(short); tmpsize+=sizeof(short);

	struct_NetU_UserAwarded_send( &ptr, &tmpsize, pValue );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

int netsend_gmlocalcmd_S( int actor_index, char send_type, SLK_NetU_Gmlocalcmd *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_GMLOCALCMD;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_GMLOCALCMD; ptr+=sizeof(short); tmpsize+=sizeof(short);

	struct_NetU_Gmlocalcmd_send( &ptr, &tmpsize, pValue );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

int netsend_logined_S( int actor_index, char send_type, SLK_NetU_Logined *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_LOGINED;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_LOGINED; ptr+=sizeof(short); tmpsize+=sizeof(short);

	struct_NetU_Logined_send( &ptr, &tmpsize, pValue );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

int netsend_timer_S( int actor_index, char send_type, char *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_TIMER;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_TIMER; ptr+=sizeof(short); tmpsize+=sizeof(short);

	LKSET_SBYTE_SEND( ptr, pValue, tmpsize );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

