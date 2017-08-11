#ifndef __STRUCT_AUTO0_H
#define __STRUCT_AUTO0_H

#define CMDS_BASE	0
#define CMDC_BASE	0

#define CMDS_LOGIN	(CMDS_BASE+1)
#define CMDS_LIST	(CMDS_BASE+2)
#define CMDS_CREATE	(CMDS_BASE+3)
#define CMDS_ENTERINFO	(CMDS_BASE+4)
#define CMDS_DELETE	(CMDS_BASE+5)
#define CMDS_HEART	(CMDS_BASE+6)
#define CMDS_NOTIFY	(CMDS_BASE+7)
#define CMDS_ACTORINFO	(CMDS_BASE+8)
#define CMDS_ITEMUSE	(CMDS_BASE+9)
#define CMDS_ITEMPUT	(CMDS_BASE+10)
#define CMDS_ITEMSETTLE	(CMDS_BASE+11)
#define CMDS_LOSTITEM	(CMDS_BASE+12)
#define CMDS_GETITEM	(CMDS_BASE+13)
#define CMDS_ITEMLIST	(CMDS_BASE+14)
#define CMDS_ITEMINFO	(CMDS_BASE+15)
#define CMDS_ADDMAPUNIT	(CMDS_BASE+16)
#define CMDS_DELMAPUNIT	(CMDS_BASE+17)
#define CMDS_UPDATEMAPUNIT	(CMDS_BASE+18)
#define CMDS_MAPUNITSINGLE	(CMDS_BASE+19)
#define CMDS_WORLDMAPINFO	(CMDS_BASE+20)
#define CMDS_MAPUNITCORRDINATE	(CMDS_BASE+21)
#define CMDS_ADDMARCHROUTE	(CMDS_BASE+22)
#define CMDS_DELMARCHROUTE	(CMDS_BASE+23)
#define CMDS_AWARDINFOLIST	(CMDS_BASE+24)
#define CMDS_EXPERIENCE	(CMDS_BASE+25)
#define CMDS_BODY	(CMDS_BASE+26)
#define CMDS_BUILDINGLIST	(CMDS_BASE+27)
#define CMDS_CHANGELEVY	(CMDS_BASE+28)
#define CMDS_CHANGESILVER	(CMDS_BASE+29)
#define CMDS_CHANGEWOOD	(CMDS_BASE+30)
#define CMDS_CHANGEFOOD	(CMDS_BASE+31)
#define CMDS_CHANGEIRON	(CMDS_BASE+32)
#define CMDS_CHANGEPEOPLE	(CMDS_BASE+33)
#define CMDS_CHANGEPRESTIGE	(CMDS_BASE+34)
#define CMDS_CHANGEFRIENDSHIP	(CMDS_BASE+35)
#define CMDS_CHANGEVIP	(CMDS_BASE+36)
#define CMDS_CHANGTOKEN	(CMDS_BASE+37)
#define CMDS_EQUIP	(CMDS_BASE+38)
#define CMDS_EQUIPLIST	(CMDS_BASE+39)
#define CMDS_EQUIPGET	(CMDS_BASE+40)
#define CMDS_EQUIPLOST	(CMDS_BASE+41)
#define CMDS_HERO	(CMDS_BASE+42)
#define CMDS_HEROLIST	(CMDS_BASE+43)
#define CMDS_HEROEXP	(CMDS_BASE+44)
#define CMDS_HEROSOLDIERS	(CMDS_BASE+45)
#define CMDS_HEROSTATE	(CMDS_BASE+46)
#define CMDS_HEROREPLACE	(CMDS_BASE+47)
#define CMDS_BUILDINGUPGRADEINFO	(CMDS_BASE+48)
#define CMDS_BUILDING	(CMDS_BASE+49)
#define CMDS_BUILDINGBARRACKS	(CMDS_BASE+50)
#define CMDS_BUILDINGRES	(CMDS_BASE+51)
#define CMDS_WORKER	(CMDS_BASE+52)
#define CMDS_HEROGET	(CMDS_BASE+53)
#define CMDS_BUILDINGGET	(CMDS_BASE+54)
#define CMDS_BUILDINGBARRACKSGET	(CMDS_BASE+55)
#define CMDS_BUILDINGRESGET	(CMDS_BASE+56)
#define CMDS_SOLDIERS	(CMDS_BASE+57)
#define CMDS_TRAININFO	(CMDS_BASE+58)
#define CMDS_QUEST	(CMDS_BASE+59)
#define CMDS_QUESTLIST	(CMDS_BASE+60)
#define CMDS_QUESTAWARD	(CMDS_BASE+61)
#define CMDS_FUNCTION	(CMDS_BASE+62)
#define CMDS_CITYGUARD	(CMDS_BASE+63)
#define CMDS_CITYGUARDLIST	(CMDS_BASE+64)
#define CMDS_CITYGUARDSEC	(CMDS_BASE+65)
#define CMDS_BUILDINGSMITHY	(CMDS_BASE+66)
#define CMDS_CHANGENAME	(CMDS_BASE+67)
#define CMDS_BUILDINGACTION	(CMDS_BASE+68)
#define CMDS_LEVYINFO	(CMDS_BASE+69)
#define CMDS_CHAT	(CMDS_BASE+70)
#define CMDS_CHATLIST	(CMDS_BASE+71)
#define CMDS_SYSTALKID	(CMDS_BASE+72)
#define CMDS_SYSTALK	(CMDS_BASE+73)
#define CMDS_BATTLEPOWER	(CMDS_BASE+74)
#define CMDS_TECHCHANGE	(CMDS_BASE+75)
#define CMDS_CITYEVENTLIST	(CMDS_BASE+76)
#define CMDS_OFFICIALHIRECHANGE	(CMDS_BASE+77)
#define CMDS_CITYPROTECT	(CMDS_BASE+78)
#define CMDS_HEROEQUIP	(CMDS_BASE+79)
#define CMDS_HEROWASH	(CMDS_BASE+80)

#define CMDC_USERAWARDED	(CMDC_BASE-4)
#define CMDC_GMLOCALCMD	(CMDC_BASE-3)
#define CMDC_LOGINED	(CMDC_BASE-2)
#define CMDC_TIMER	(CMDC_BASE-1)
#define CMDC_LOGIN	(CMDC_BASE+1)
#define CMDC_CREATE	(CMDC_BASE+2)
#define CMDC_LIST	(CMDC_BASE+3)
#define CMDC_ENTERGAME	(CMDC_BASE+4)
#define CMDC_DELETE	(CMDC_BASE+5)
#define CMDC_HEART	(CMDC_BASE+6)
#define CMDC_GMCMD	(CMDC_BASE+7)
#define CMDC_ASKINFO	(CMDC_BASE+9)
#define CMDC_ENTERCITY	(CMDC_BASE+10)
#define CMDC_CHAT	(CMDC_BASE+11)
struct _slk_NetU_Logined {
	int m_result;	//用户服务器返回-登录返回值
	int m_client_index;	//用户服务器返回-角色索引
	int m_authid;	//用户服务器返回-认证ID
	char m_userid[21];	//用户服务器返回-用户ID
	char m_username[51];	//用户服务器返回-用户名
	short m_usertype;	//用户服务器返回-用户类型
	char m_checkflag;	//用户服务器返回-检测标记
	char m_isdeep;	//用户服务器返回-要防沉迷
	char m_access_token[65];	//用户服务器返回-360的用户令牌
	int m_expires_in;	//用户服务器返回-360的令牌有效期
	char m_refresh_token[65];	//用户服务器返回-360的刷新令牌
	char m_deviceid[65];	//用户服务器返回-传回的设备ID
};
typedef struct _slk_NetU_Logined SLK_NetU_Logined;	//用户服务器返回登陆

struct _slk_NetU_UserAwarded {
	int m_result;	//用户服务器返回-兑换结果返回值
	int m_client_index;	//用户服务器返回-角色索引
	int m_authid;	//用户服务器返回-认证ID
	short m_cdkey_index;	//用户服务器返回-兑换码存档索引
	int m_awardgroup;	//用户服务器返回-兑换码奖励组
	char m_cardnumber[32];	//用户服务器返回-兑换码卡号
};
typedef struct _slk_NetU_UserAwarded SLK_NetU_UserAwarded;	//用户服务器返回兑换奖励

struct _slk_NetU_Gmlocalcmd {
	short m_cmd;	//指令
	int m_value[4];	//数值参数
	short m_msglen;	//字符串长度
	char m_msg[1024];	//字符串参数
};
typedef struct _slk_NetU_Gmlocalcmd SLK_NetU_Gmlocalcmd;	//本地GM信息

int struct_NetU_Logined_send( char **pptr, int *psize, SLK_NetU_Logined *pValue );
int struct_NetU_Logined_recv( char **pptr, int *psize, SLK_NetU_Logined *pValue );
int struct_NetU_UserAwarded_send( char **pptr, int *psize, SLK_NetU_UserAwarded *pValue );
int struct_NetU_UserAwarded_recv( char **pptr, int *psize, SLK_NetU_UserAwarded *pValue );
int struct_NetU_Gmlocalcmd_send( char **pptr, int *psize, SLK_NetU_Gmlocalcmd *pValue );
int struct_NetU_Gmlocalcmd_recv( char **pptr, int *psize, SLK_NetU_Gmlocalcmd *pValue );

#endif
