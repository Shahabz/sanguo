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

int netsend_buildinglist_S( int actor_index, char send_type, SLK_NetS_BuildingList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changelevy_S( int actor_index, char send_type, SLK_NetS_Levy *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGELEVY;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGELEVY; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Levy_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changesilver_S( int actor_index, char send_type, SLK_NetS_Silver *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGESILVER;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGESILVER; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Silver_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changewood_S( int actor_index, char send_type, SLK_NetS_Wood *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEWOOD;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEWOOD; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Wood_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changefood_S( int actor_index, char send_type, SLK_NetS_Food *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEFOOD;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEFOOD; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Food_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changeiron_S( int actor_index, char send_type, SLK_NetS_Iron *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEIRON;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEIRON; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Iron_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changepeople_S( int actor_index, char send_type, SLK_NetS_People *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEPEOPLE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEPEOPLE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_People_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changeprestige_S( int actor_index, char send_type, SLK_NetS_Prestige *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEPRESTIGE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEPRESTIGE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Prestige_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changefriendship_S( int actor_index, char send_type, SLK_NetS_Friendship *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEFRIENDSHIP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEFRIENDSHIP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Friendship_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changevip_S( int actor_index, char send_type, SLK_NetS_Vip *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGEVIP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGEVIP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Vip_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changtoken_S( int actor_index, char send_type, SLK_NetS_Token *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGTOKEN;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGTOKEN; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Token_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_equip_S( int actor_index, char send_type, SLK_NetS_Equip *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EQUIP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EQUIP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Equip_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_equiplist_S( int actor_index, char send_type, SLK_NetS_EquipList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EQUIPLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EQUIPLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EquipList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_equipget_S( int actor_index, char send_type, SLK_NetS_EquipGet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EQUIPGET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EQUIPGET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EquipGet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_equiplost_S( int actor_index, char send_type, SLK_NetS_EquipLost *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EQUIPLOST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EQUIPLOST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EquipLost_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_hero_S( int actor_index, char send_type, SLK_NetS_Hero *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HERO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HERO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Hero_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_herolist_S( int actor_index, char send_type, SLK_NetS_HeroList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroexp_S( int actor_index, char send_type, SLK_NetS_HeroExp *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROEXP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROEXP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroExp_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_herosoldiers_S( int actor_index, char send_type, SLK_NetS_HeroSoldiers *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROSOLDIERS;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROSOLDIERS; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroSoldiers_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_herostate_S( int actor_index, char send_type, SLK_NetS_HeroState *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROSTATE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROSTATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroState_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroreplace_S( int actor_index, char send_type, SLK_NetS_HeroReplace *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROREPLACE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROREPLACE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroReplace_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingupgradeinfo_S( int actor_index, char send_type, SLK_NetS_BuildingUpgradeInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGUPGRADEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGUPGRADEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingUpgradeInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_building_S( int actor_index, char send_type, SLK_NetS_Building *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDING;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDING; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Building_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingbarracks_S( int actor_index, char send_type, SLK_NetS_BuildingBarracks *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGBARRACKS;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGBARRACKS; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingBarracks_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingres_S( int actor_index, char send_type, SLK_NetS_BuildingRes *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGRES;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGRES; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingRes_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_worker_S( int actor_index, char send_type, SLK_NetS_Worker *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_WORKER;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_WORKER; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Worker_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroget_S( int actor_index, char send_type, SLK_NetS_HeroGet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROGET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROGET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroGet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingget_S( int actor_index, char send_type, SLK_NetS_BuildingGet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGGET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGGET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingGet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingbarracksget_S( int actor_index, char send_type, SLK_NetS_BuildingBarracksGet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGBARRACKSGET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGBARRACKSGET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingBarracksGet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingresget_S( int actor_index, char send_type, SLK_NetS_BuildingResGet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGRESGET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGRESGET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingResGet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_soldiers_S( int actor_index, char send_type, SLK_NetS_Soldiers *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_SOLDIERS;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_SOLDIERS; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Soldiers_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_traininfo_S( int actor_index, char send_type, SLK_NetS_TrainInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_TRAININFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_TRAININFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_TrainInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_quest_S( int actor_index, char send_type, SLK_NetS_Quest *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_QUEST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_QUEST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Quest_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_questlist_S( int actor_index, char send_type, SLK_NetS_QuestList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_QUESTLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_QUESTLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_QuestList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_questaward_S( int actor_index, char send_type, SLK_NetS_QuestAward *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_QUESTAWARD;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_QUESTAWARD; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_QuestAward_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_function_S( int actor_index, char send_type, SLK_NetS_Function *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FUNCTION;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FUNCTION; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Function_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityguard_S( int actor_index, char send_type, SLK_NetS_CityGuard *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYGUARD;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYGUARD; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityGuard_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityguardlist_S( int actor_index, char send_type, SLK_NetS_CityGuardList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYGUARDLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYGUARDLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityGuardList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityguardsec_S( int actor_index, char send_type, SLK_NetS_CityGuardSec *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYGUARDSEC;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYGUARDSEC; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityGuardSec_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingsmithy_S( int actor_index, char send_type, SLK_NetS_BuildingSmithy *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGSMITHY;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGSMITHY; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingSmithy_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_changename_S( int actor_index, char send_type, SLK_NetS_ChangeName *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHANGENAME;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHANGENAME; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ChangeName_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingaction_S( int actor_index, char send_type, SLK_NetS_BuildingAction *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGACTION;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGACTION; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingAction_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_levyinfo_S( int actor_index, char send_type, SLK_NetS_LevyInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LEVYINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LEVYINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_LevyInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_chat_S( int actor_index, char send_type, SLK_NetS_Chat *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHAT;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHAT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Chat_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_chatlist_S( int actor_index, char send_type, SLK_NetS_ChatList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHATLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHATLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ChatList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_systalkid_S( int actor_index, char send_type, SLK_NetS_Systalkid *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_SYSTALKID;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_SYSTALKID; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Systalkid_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_systalk_S( int actor_index, char send_type, SLK_NetS_Systalk *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_SYSTALK;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_SYSTALK; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Systalk_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_battlepower_S( int actor_index, char send_type, SLK_NetS_Battlepower *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BATTLEPOWER;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BATTLEPOWER; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Battlepower_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_techchange_S( int actor_index, char send_type, SLK_NetS_TechChange *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_TECHCHANGE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_TECHCHANGE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_TechChange_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityeventlist_S( int actor_index, char send_type, SLK_NetS_EventList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYEVENTLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYEVENTLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EventList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_officialhirechange_S( int actor_index, char send_type, SLK_NetS_OfficialHireChange *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_OFFICIALHIRECHANGE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_OFFICIALHIRECHANGE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_OfficialHireChange_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityprotect_S( int actor_index, char send_type, SLK_NetS_CityProtect *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYPROTECT;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYPROTECT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityProtect_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroequip_S( int actor_index, char send_type, SLK_NetS_HeroEquip *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROEQUIP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROEQUIP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroEquip_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_herowash_S( int actor_index, char send_type, SLK_NetS_HeroWash *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROWASH;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROWASH; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroWash_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_herocolorup_S( int actor_index, char send_type, SLK_NetS_HeroColorup *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROCOLORUP;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROCOLORUP; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroColorup_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityattr_S( int actor_index, char send_type, SLK_NetS_CityAttr *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYATTR;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYATTR; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityAttr_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_equipwash_S( int actor_index, char send_type, SLK_NetS_EquipWash *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_EQUIPWASH;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_EQUIPWASH; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EquipWash_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_materiallist_S( int actor_index, char send_type, SLK_NetS_MaterialList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MATERIALLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MATERIALLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MaterialList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_materialwilllist_S( int actor_index, char send_type, SLK_NetS_MaterialWillList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MATERIALWILLLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MATERIALWILLLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MaterialWillList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_storylist_S( int actor_index, char send_type, SLK_NetS_StoryList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_STORYLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_STORYLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_StoryList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_storyrank_S( int actor_index, char send_type, SLK_NetS_StoryRank *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_STORYRANK;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_STORYRANK; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_StoryRank_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_storystate_S( int actor_index, char send_type, SLK_NetS_StoryState *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_STORYSTATE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_STORYSTATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_StoryState_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_storyranknum_S( int actor_index, char send_type, SLK_NetS_StoryRanknum *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_STORYRANKNUM;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_STORYRANKNUM; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_StoryRanknum_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_storyranktime_S( int actor_index, char send_type, SLK_NetS_StoryRanktime *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_STORYRANKTIME;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_STORYRANKTIME; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_StoryRanktime_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapzonechange_S( int actor_index, char send_type, SLK_NetS_MapZoneChange *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPZONECHANGE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPZONECHANGE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MapZoneChange_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapzoneunitlist_S( int actor_index, char send_type, SLK_NetS_MapZoneUnitList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPZONEUNITLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPZONEUNITLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MapZoneUnitList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapzoneunit_S( int actor_index, char send_type, SLK_NetS_ZoneUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPZONEUNIT;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPZONEUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ZoneUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_armyspeedupdate_S( int actor_index, char send_type, SLK_NetS_ArmySpeedUpdate *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ARMYSPEEDUPDATE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ARMYSPEEDUPDATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ArmySpeedUpdate_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_battlelist_S( int actor_index, char send_type, SLK_NetS_BattleList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BATTLELIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BATTLELIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BattleList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_battleinfo_S( int actor_index, char send_type, SLK_NetS_BattleInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BATTLEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BATTLEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BattleInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapresinfo_S( int actor_index, char send_type, SLK_NetS_MapResInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPRESINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPRESINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MapResInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_weatherchange_S( int actor_index, char send_type, SLK_NetS_WeatherChange *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_WEATHERCHANGE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_WEATHERCHANGE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_WeatherChange_send( &ptr, &tmpsize, pValue );

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

