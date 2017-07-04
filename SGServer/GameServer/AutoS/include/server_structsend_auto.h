#ifndef __STRUCTSEND0_AUTO_H
#define __STRUCTSEND0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetS_Login {
	int m_result;	//服务器返回-返回值
	short m_username_length;	//服务器返回-用户名长度
	char m_username[51];	//服务器返回-用户名
	short m_token_length;	//服务器返回-token长度
	char m_token[256];	//服务器返回-token
	short m_usertype;	//服务器返回-用户类型
};
typedef struct _slk_NetS_Login SLK_NetS_Login;	//服务器端发送登录结果给客户端

struct _slk_ListInfo {
	int m_actorid;	//角色ID
	char m_name[22];	//角色名字
	char m_nation;	//角色国家
	short m_level;	//角色等级
	char m_offset;	//角色位置
	char m_lockstat;	//锁定状态
	int m_delete_stoptime;	//删除时间
	int m_lock_endtime;	//锁定时间
};
typedef struct _slk_ListInfo SLK_ListInfo;	//单个角色的列表信息

struct _slk_NetS_List {
	char m_actor_num;	//服务端返回-角色个数
	SLK_ListInfo m_listinfo[8];	//服务端返回-角色数组
};
typedef struct _slk_NetS_List SLK_NetS_List;	//服务器端发送角色列表

struct _slk_NetS_Create {
	char m_result;	//服务端返回-创建角色的结果
	int m_actorid;	//服务端返回-创建角色的编号
	int m_createtime;	//服务端返回-创建角色的时间
};
typedef struct _slk_NetS_Create SLK_NetS_Create;	//服务器返回创建角色结果

struct _slk_NetS_EnterInfo {
	int m_actorid;	//服务端返回-进入游戏角色ID
	int m_data_int;	//服务端返回-进入游戏额外数据[备用]
	short m_data_short;	//服务端返回-进入游戏额外数据[备用]
	char m_data_char;	//服务端返回-进入游戏额外数据[备用]
	short m_serverid;	//服务端返回-进入的服务器ID
	int m_servertime;	//服务端返回-进入的服务器时间戳
	int m_createtime;	//服务端返回-创建角色时间
};
typedef struct _slk_NetS_EnterInfo SLK_NetS_EnterInfo;	//服务器返回进入游戏后的信息

struct _slk_NetS_Delete {
	int m_result;	//服务器返回-删除返回值
	int m_actorid;	//服务器返回-删除的角色编号
};
typedef struct _slk_NetS_Delete SLK_NetS_Delete;	//服务器返回删除角色

struct _slk_NetS_Heart {
	int m_value[2];	//心跳-数值
};
typedef struct _slk_NetS_Heart SLK_NetS_Heart;	//心跳

struct _slk_NetS_Notify {
	short m_msgid;	//服务器发送-短消息ID
	short m_valuenum;	//服务器发送-短消息参数个数
	int m_value[10];	//服务器发送-短消息数值参数
	short m_msg_length;	//服务器发送-短消息字符串消息长度
	char m_msg[256];	//服务器发送-短消息字符串参数
};
typedef struct _slk_NetS_Notify SLK_NetS_Notify;	//通用短消息

struct _slk_NetS_Building {
	char m_kind;	//普通建筑-种类
	char m_offset;	//普通建筑-位置
	char m_level;	//普通建筑-等级
	int m_sec;	//普通建筑-操作剩余时间
	int m_needsec;	//
	char m_quick;	//普通建筑-是否有加速(科技等)
};
typedef struct _slk_NetS_Building SLK_NetS_Building;	//普通建筑信息

struct _slk_NetS_BuildingBarracks {
	char m_kind;	//兵营建筑-种类
	char m_offset;	//兵营建筑-位置
	char m_level;	//兵营建筑-等级
	int m_sec;	//兵营建筑-募兵剩余时间
	int m_needsec;	//
	char m_quick;	//兵营建筑-是否有加速
};
typedef struct _slk_NetS_BuildingBarracks SLK_NetS_BuildingBarracks;	//兵营建筑信息

struct _slk_NetS_BuildingRes {
	char m_kind;	//资源建筑-种类
	char m_offset;	//资源建筑-索引(对应指定地块)
	char m_level;	//资源建筑-等级
};
typedef struct _slk_NetS_BuildingRes SLK_NetS_BuildingRes;	//资源建筑信息

struct _slk_NetS_BuildingList {
	short m_building_count;	//服务器发送-普通建筑数量
	SLK_NetS_Building m_building[8];	//服务器发送-普通建筑列表
	short m_barracks_count;	//服务器发送-兵营建筑数量
	SLK_NetS_BuildingBarracks m_barracks[8];	//服务器发送-兵营建筑列表
	short m_res_count;	//服务器发送-资源建筑数量
	SLK_NetS_BuildingRes m_res[64];	//服务器发送-资源建筑列表
	char m_levynum;	//服务器发送-当前可征收次数
	char m_worker_kind;	//服务器发送-建筑队列种类
	char m_worker_offset;	//服务器发送建筑队列索引
	char m_worker_op;	//服务器发送-建筑队列建筑操作
	int m_worker_sec;	//服务器发送-建筑队列剩余时间
	int m_worker_needsec;	//
	char m_worker_kind_ex;	//服务器发送-建筑队列种类(商用)
	char m_worker_offset_ex;	//服务器发送-建筑队列索引(商用)
	char m_worker_op_ex;	//服务器发送-建筑队列操作(商用)
	int m_worker_sec_ex;	//服务器发送-建筑队列剩余时间(商用)
	int m_worker_needsec_ex;	//
	int m_worker_expire_ex;	//服务器发送-商用建造队列到期时间
	int m_function;	//服务器发送-功能是否开启
};
typedef struct _slk_NetS_BuildingList SLK_NetS_BuildingList;	//建筑列表

struct _slk_NetS_ActorInfo {
	int m_actorid;	//服务器发送玩家基本信息-角色编号
	char m_name[22];	//服务器发送玩家基本信息-角色名称
	char m_nation;	//服务器发送玩家基本信息-国家
	char m_shape;	//服务器发送玩家基本信息-形象
	short m_level;	//服务器发送玩家基本信息-级别
	int m_exp;	//服务器发送玩家基本信息-经验
	int m_exp_max;	//服务器发送玩家基本信息-经验上限
	int m_token;	//服务器发送玩家基本信息-钻石
	char m_viplevel;	//服务器发送玩家基本信息-VIP等级
	int m_vipexp;	//服务器发送玩家基本信息-VIP经验
	int m_vipexp_max;	//服务器发送玩家基本信息-VIP经验上限
	short m_body;	//服务器发送玩家基本信息-体力
	char m_place;	//服务器发送玩家基本信息-爵位
	char m_official;	//服务器发送玩家基本信息-官职
	short m_zone;	//服务器发送玩家基本信息-区域
	int m_battlepower;	//服务器发送玩家基本信息-战力
	short m_silver;	//服务器发送玩家基本信息-银币
	short m_wood;	//服务器发送玩家基本信息-木材
	short m_food;	//服务器发送玩家基本信息-粮食
	short m_iron;	//服务器发送玩家基本信息-镔铁
	int m_infantry_num;	//服务器发送玩家基本信息-步兵数
	int m_cavalry_num;	//服务器发送玩家基本信息-骑兵数
	int m_archer_num;	//服务器发送玩家基本信息-弓兵数
	short m_mokilllv;	//服务器发送玩家基本信息-击杀野怪最高级别
	int m_sflag;	//服务器发送玩家基本信息-标志位
};
typedef struct _slk_NetS_ActorInfo SLK_NetS_ActorInfo;	//角色基本信息

struct _slk_Corrdinate {
	short m_posx;	//坐标
	short m_posy;	//坐标
};
typedef struct _slk_Corrdinate SLK_Corrdinate;	//坐标

struct _slk_NetS_ItemUse {
	short m_itemoffset;	//物品位置
	short m_usenum;	//使用个数或装备位置
	int m_effres;	//影响，一般用于特效显示
};
typedef struct _slk_NetS_ItemUse SLK_NetS_ItemUse;	//道具使用

struct _slk_NetS_ItemPut {
	short m_res_offset;	//物品源
	short m_res_num;	//物品源个数
	short m_target_offset;	//物品目标
	short m_target_num;	//目标个数
};
typedef struct _slk_NetS_ItemPut SLK_NetS_ItemPut;	//道具放置|卸下

struct _slk_NetS_ItemSettle {
	short m_itemnum;	//整理背包的个数
	short m_itemoffset[100];	//重置的物品位置
};
typedef struct _slk_NetS_ItemSettle SLK_NetS_ItemSettle;	//道具整理

struct _slk_NetS_LostItem {
	short m_itemoffset;	//物品位置
	short m_itemnum;	//数量
	int m_targetid;	//赋予的对象id
	short m_path;	//途径
};
typedef struct _slk_NetS_LostItem SLK_NetS_LostItem;	//丢失道具

struct _slk_NetS_GetItem {
	short m_itemoffset;	//物品位置
	short m_kind;	//物品种类
	short m_type;	//物品类型
	char m_color;	//颜色
	short m_num;	//数量
	int m_targetid;	//赋予的对象id
	short m_path;	//途径
	unsigned char m_situation;	//使用状态
};
typedef struct _slk_NetS_GetItem SLK_NetS_GetItem;	//获得道具

struct _slk_ItemSmpInfo {
	short m_offset;	//物品位置
	short m_kind;	//物品种类
	short m_num;	//数量
	unsigned char m_situation;	//使用状态
	char m_color_level;	//颜色
};
typedef struct _slk_ItemSmpInfo SLK_ItemSmpInfo;	//道具简易信息

struct _slk_NetS_ItemList {
	char m_itemext;	//扩展的格子
	short m_kindnum;	//物品数量
	SLK_ItemSmpInfo m_item[300];	//物品
};
typedef struct _slk_NetS_ItemList SLK_NetS_ItemList;	//道具列表

struct _slk_ItemAttr {
	short m_type;	//物品属性所属类型
	short m_ability;	//物品属性
	int m_value;	//物品数据
	int m_addvalue;	//物品增加数据
};
typedef struct _slk_ItemAttr SLK_ItemAttr;	//道具属性

struct _slk_NetS_ItemInfo {
	short m_itemoffset;	//物品位置
	short m_itemkind;	//装备kind,校验
	short m_type;	//物品类型
	short m_level;	//一般等级
	char m_color_level;	//颜色分级
	int m_price;	//售价
	short m_attr_num;	//属性个数
	SLK_ItemAttr m_attr[10];	//属性
};
typedef struct _slk_NetS_ItemInfo SLK_NetS_ItemInfo;	//道具信息

struct _slk_WalkPath {
	short m_pathx[100];	//路径点结构
	short m_pathy[100];	//路径点结构
	short m_len;	//路径点结构
};
typedef struct _slk_WalkPath SLK_WalkPath;	//移动路径

struct _slk_NetS_AddMapUnit {
	char m_type;	//地图单元-类型
	char m_state;	//地图单元-状态
	short m_posx;	//地图单元-当前坐标
	int m_unit_index;	//地图单元-序号
	short m_posy;	//地图单元-当前坐标
	char m_namelen;	//地图单元-名称长度
	char m_name[32];	//地图单元-名称
	char m_char_value_count;	//地图单元-通用字段长度
	char m_char_value[16];	//地图单元-通用字段
	char m_short_value_count;	//地图单元-通用字段长度
	short m_short_value[8];	//地图单元-通用字段
	char m_int_value_count;	//地图单元-通用字段长度
	int m_int_value[8];	//地图单元-通用字段
	char m_prefixlen;	//地图单元-称号长度
	char m_prefix[22];	//地图单元-称号
};
typedef struct _slk_NetS_AddMapUnit SLK_NetS_AddMapUnit;	//显示单元进入地图

struct _slk_NetS_DelMapUnit {
	int m_unit_index;	//地图单元-序号
};
typedef struct _slk_NetS_DelMapUnit SLK_NetS_DelMapUnit;	//显示单元离开地图

struct _slk_NetS_WorldMapInfo {
	short m_map_width;	//世界地图大小（格子数）
	short m_map_height;	//世界地图大小（格子数）
	char m_area_width;	//区域大小（一个区域几个地图格子）
	char m_area_height;	//区域大小（一个区域几个地图格子）
	short m_map_area_xnum;	//一个地图有多少区域
	short m_map_area_ynum;	//一个地图有多少区域
	short m_my_city_posx;	//我的城池位置
	short m_my_city_posy;	//我的城池位置
	int m_my_city_unit_index;	//我的城池地图单元-序号
	char m_citystate;	//我的城池状态
	short m_target_posx;	//目的点
	short m_target_posy;	//目的点
};
typedef struct _slk_NetS_WorldMapInfo SLK_NetS_WorldMapInfo;	//世界地图信息

struct _slk_NetS_MapUnitCorrdinate {
	int m_unit_index;	//地图单元-序号
	short m_posx;	//地图单元-当前坐标
	short m_posy;	//地图单元-当前坐标
};
typedef struct _slk_NetS_MapUnitCorrdinate SLK_NetS_MapUnitCorrdinate;	//地图显示单元位置

struct _slk_NetS_AddMarchRoute {
	char m_from_type;	//行军路线-出发单元类型
	short m_from_posx;	//行军路线-出发位置
	short m_from_posy;	//行军路线-出发位置
	char m_to_type;	//行军路线-到达单元类型
	short m_to_posx;	//行军路线-到达位置
	short m_to_posy;	//行军路线-到达位置
	char m_state;	//行军路线-状态
	int m_from_actorid;	//行军路线-城市
	int m_from_clubid;	//行军路线-联盟
	int m_to_actorid;	//行军路线-目标城市
	int m_to_clubid;	//行军路线-目标联盟
	int m_army_index;	//行军路线-所属部队
	char m_action;	//行军路线-行为
};
typedef struct _slk_NetS_AddMarchRoute SLK_NetS_AddMarchRoute;	//行军路线

struct _slk_NetS_DelMarchRoute {
	int m_army_index;	//删除线-部队索引
};
typedef struct _slk_NetS_DelMarchRoute SLK_NetS_DelMarchRoute;	//删除行军路线

struct _slk_NetS_UpdateMapUnit {
	SLK_NetS_AddMapUnit m_info;	//更新地图显示单元信息
};
typedef struct _slk_NetS_UpdateMapUnit SLK_NetS_UpdateMapUnit;	//更新地图显示单元信息

struct _slk_NetS_Equip {
	short m_offset;	//装备索引
	short m_kind;	//装备种类
	short m_washid[4];	//洗练属性
};
typedef struct _slk_NetS_Equip SLK_NetS_Equip;	//装备

struct _slk_NetS_EquipList {
	short m_equipext;	//装备栏扩展
	short m_count;	//装备数量
	SLK_NetS_Equip m_list[100];	//装备
};
typedef struct _slk_NetS_EquipList SLK_NetS_EquipList;	//装备列表

struct _slk_NetS_EquipGet {
	int m_offset;	//获得一件装备
	short m_kind;	//获得一件装备
	short m_path;	//获得一件装备
};
typedef struct _slk_NetS_EquipGet SLK_NetS_EquipGet;	//获得一件装备

struct _slk_NetS_EquipLost {
	int m_offset;	//丢掉一件装备
	short m_kind;	//丢掉一件装备
	short m_path;	//丢掉一件装备
};
typedef struct _slk_NetS_EquipLost SLK_NetS_EquipLost;	//丢掉一件装备

struct _slk_NetS_AwardInfo {
	int m_kind;	//种类
	int m_num;	//数量
};
typedef struct _slk_NetS_AwardInfo SLK_NetS_AwardInfo;	//奖励信息

struct _slk_NetS_AwardInfoList {
	short m_count;	//奖励数量
	SLK_NetS_AwardInfo m_list[32];	//奖励列表
	short m_callback_code;	//哪个系统要显示，回传
};
typedef struct _slk_NetS_AwardInfoList SLK_NetS_AwardInfoList;	//奖励信息列表

struct _slk_NetS_Experience {
	int m_addexp;	//服务器发送-获取经验
	int m_curexp;	//服务器发送-获取经验
	char m_isup;	//服务器发送-获取经验
	short m_path;	//服务器发送-获取经验
	short m_level;	//服务器发送-获取经验
	int m_expmax;	//服务器发送-获取经验
};
typedef struct _slk_NetS_Experience SLK_NetS_Experience;	//改变经验

struct _slk_NetS_Body {
	short m_total;	//体力
	short m_add;	//获取体力
	short m_path;	//途径
};
typedef struct _slk_NetS_Body SLK_NetS_Body;	//改变体力

struct _slk_NetS_Levy {
	short m_total;	//改变征收次数
	short m_add;	//改变征收次数
	short m_max;	//改变征收次数
	int m_sec;	//改变征收次数
	short m_path;	//改变征收次数
};
typedef struct _slk_NetS_Levy SLK_NetS_Levy;	//改变征收次数

struct _slk_NetS_Silver {
	int m_total;	//改变银币
	int m_add;	//改变银币
	short m_path;	//改变银币
};
typedef struct _slk_NetS_Silver SLK_NetS_Silver;	//改变银币

struct _slk_NetS_Wood {
	int m_total;	//改变木材
	int m_add;	//改变木材
	short m_path;	//改变木材
};
typedef struct _slk_NetS_Wood SLK_NetS_Wood;	//改变木材

struct _slk_NetS_Food {
	int m_total;	//改变粮食
	int m_add;	//改变粮食
	short m_path;	//改变粮食
};
typedef struct _slk_NetS_Food SLK_NetS_Food;	//改变粮食

struct _slk_NetS_Iron {
	int m_total;	//改变镔铁
	int m_add;	//改变镔铁
	short m_path;	//改变镔铁
};
typedef struct _slk_NetS_Iron SLK_NetS_Iron;	//改变镔铁

struct _slk_NetS_People {
	int m_total;	//改变人口
	int m_add;	//改变人口
	short m_path;	//改变人口
};
typedef struct _slk_NetS_People SLK_NetS_People;	//改变人口

struct _slk_NetS_Prestige {
	int m_total;	//改变威望值
	int m_add;	//改变威望值
	short m_path;	//改变威望值
};
typedef struct _slk_NetS_Prestige SLK_NetS_Prestige;	//改变威望值

struct _slk_NetS_Friendship {
	int m_total;	//改变友谊积分
	int m_add;	//改变友谊积分
	short m_path;	//改变友谊积分
};
typedef struct _slk_NetS_Friendship SLK_NetS_Friendship;	//改变友谊积分

struct _slk_NetS_Vip {
	int m_addexp;	//改变VIP经验
	int m_curexp;	//改变VIP经验
	int m_expmax;	//改变VIP经验
	char m_isup;	//改变VIP经验
	char m_level;	//改变VIP经验
	short m_path;	//改变VIP经验
};
typedef struct _slk_NetS_Vip SLK_NetS_Vip;	//改变VIP经验

struct _slk_NetS_Token {
	int m_total;	//改变钻石
	int m_add;	//改变钻石
	short m_path;	//改变钻石
};
typedef struct _slk_NetS_Token SLK_NetS_Token;	//改变钻石

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue );
int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue );
int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue );
int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue );
int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue );
int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue );
int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue );
int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue );
int struct_NetS_Building_send( char **pptr, int *psize, SLK_NetS_Building *pValue );
int struct_NetS_BuildingBarracks_send( char **pptr, int *psize, SLK_NetS_BuildingBarracks *pValue );
int struct_NetS_BuildingRes_send( char **pptr, int *psize, SLK_NetS_BuildingRes *pValue );
int struct_NetS_BuildingList_send( char **pptr, int *psize, SLK_NetS_BuildingList *pValue );
int struct_NetS_ActorInfo_send( char **pptr, int *psize, SLK_NetS_ActorInfo *pValue );
int struct_Corrdinate_send( char **pptr, int *psize, SLK_Corrdinate *pValue );
int struct_NetS_ItemUse_send( char **pptr, int *psize, SLK_NetS_ItemUse *pValue );
int struct_NetS_ItemPut_send( char **pptr, int *psize, SLK_NetS_ItemPut *pValue );
int struct_NetS_ItemSettle_send( char **pptr, int *psize, SLK_NetS_ItemSettle *pValue );
int struct_NetS_LostItem_send( char **pptr, int *psize, SLK_NetS_LostItem *pValue );
int struct_NetS_GetItem_send( char **pptr, int *psize, SLK_NetS_GetItem *pValue );
int struct_ItemSmpInfo_send( char **pptr, int *psize, SLK_ItemSmpInfo *pValue );
int struct_NetS_ItemList_send( char **pptr, int *psize, SLK_NetS_ItemList *pValue );
int struct_ItemAttr_send( char **pptr, int *psize, SLK_ItemAttr *pValue );
int struct_NetS_ItemInfo_send( char **pptr, int *psize, SLK_NetS_ItemInfo *pValue );
int struct_WalkPath_send( char **pptr, int *psize, SLK_WalkPath *pValue );
int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue );
int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue );
int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue );
int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue );
int struct_NetS_AddMarchRoute_send( char **pptr, int *psize, SLK_NetS_AddMarchRoute *pValue );
int struct_NetS_DelMarchRoute_send( char **pptr, int *psize, SLK_NetS_DelMarchRoute *pValue );
int struct_NetS_UpdateMapUnit_send( char **pptr, int *psize, SLK_NetS_UpdateMapUnit *pValue );
int struct_NetS_Equip_send( char **pptr, int *psize, SLK_NetS_Equip *pValue );
int struct_NetS_EquipList_send( char **pptr, int *psize, SLK_NetS_EquipList *pValue );
int struct_NetS_EquipGet_send( char **pptr, int *psize, SLK_NetS_EquipGet *pValue );
int struct_NetS_EquipLost_send( char **pptr, int *psize, SLK_NetS_EquipLost *pValue );
int struct_NetS_AwardInfo_send( char **pptr, int *psize, SLK_NetS_AwardInfo *pValue );
int struct_NetS_AwardInfoList_send( char **pptr, int *psize, SLK_NetS_AwardInfoList *pValue );
int struct_NetS_Experience_send( char **pptr, int *psize, SLK_NetS_Experience *pValue );
int struct_NetS_Body_send( char **pptr, int *psize, SLK_NetS_Body *pValue );
int struct_NetS_Levy_send( char **pptr, int *psize, SLK_NetS_Levy *pValue );
int struct_NetS_Silver_send( char **pptr, int *psize, SLK_NetS_Silver *pValue );
int struct_NetS_Wood_send( char **pptr, int *psize, SLK_NetS_Wood *pValue );
int struct_NetS_Food_send( char **pptr, int *psize, SLK_NetS_Food *pValue );
int struct_NetS_Iron_send( char **pptr, int *psize, SLK_NetS_Iron *pValue );
int struct_NetS_People_send( char **pptr, int *psize, SLK_NetS_People *pValue );
int struct_NetS_Prestige_send( char **pptr, int *psize, SLK_NetS_Prestige *pValue );
int struct_NetS_Friendship_send( char **pptr, int *psize, SLK_NetS_Friendship *pValue );
int struct_NetS_Vip_send( char **pptr, int *psize, SLK_NetS_Vip *pValue );
int struct_NetS_Token_send( char **pptr, int *psize, SLK_NetS_Token *pValue );

#endif
