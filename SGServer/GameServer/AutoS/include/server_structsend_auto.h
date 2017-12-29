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
	char m_nation_award;	//服务端返回-哪个国家有奖励
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
	int m_value;	//心跳-数值
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

struct _slk_NetS_DialogUpdate {
	char m_ui;	//服务器发送-有ui需要更新
};
typedef struct _slk_NetS_DialogUpdate SLK_NetS_DialogUpdate;	//有ui需要更新

struct _slk_NetS_OfficialHire {
	short m_ofkind;	//雇佣官
	int m_ofsec;	//雇佣官
	int m_offree;	//雇佣官
	int m_ofquick;	//雇佣官
};
typedef struct _slk_NetS_OfficialHire SLK_NetS_OfficialHire;	//雇佣官

struct _slk_NetS_Building {
	char m_kind;	//普通建筑-种类
	char m_offset;	//普通建筑-位置
	char m_level;	//普通建筑-等级
	int m_sec;	//普通建筑-操作剩余时间
	int m_needsec;	//普通建筑-操作需要时间
	char m_quick;	//普通建筑-是否有加速(科技等)
	int m_overvalue;	//普通建筑-完成后的值
	int m_value;	//普通建筑-未完成值
};
typedef struct _slk_NetS_Building SLK_NetS_Building;	//普通建筑信息

struct _slk_NetS_BuildingBarracks {
	char m_kind;	//兵营建筑-种类
	char m_offset;	//兵营建筑-位置
	char m_level;	//兵营建筑-等级
	int m_sec;	//兵营建筑-募兵剩余时间
	int m_needsec;	//兵营建筑-募兵需要时间
	char m_quick;	//兵营建筑-是否有加速
	int m_overvalue;	//兵营建筑-完成后的总数
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
	int m_worker_needsec;	//服务器发送-建筑队列需要时间
	char m_worker_free;	//服务器发送-建筑队列免费时间
	char m_worker_kind_ex;	//服务器发送-建筑队列种类(商用)
	char m_worker_offset_ex;	//服务器发送-建筑队列索引(商用)
	char m_worker_op_ex;	//服务器发送-建筑队列操作(商用)
	int m_worker_sec_ex;	//服务器发送-建筑队列剩余时间(商用)
	int m_worker_needsec_ex;	//服务器发送-建筑队列需要时间(商用)
	char m_worker_free_ex;	//服务器发送-建筑队列免费时间(商用)
	int m_worker_expire_ex;	//服务器发送-商用建造队列到期时间
	int m_function;	//服务器发送-功能是否开启
	short m_forgingkind;	//服务器发送-铁匠铺
	int m_forgingsec;	//服务器发送-铁匠铺
	int m_forgingsec_need;	//服务器发送-铁匠铺
	int m_wnquick;	//服务器发送-奖励的额外加速
	int m_wnquick_ex;	//服务器发送-奖励的额外加速(商用)
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
	int m_silver;	//服务器发送玩家基本信息-银币
	int m_wood;	//服务器发送玩家基本信息-木材
	int m_food;	//服务器发送玩家基本信息-粮食
	int m_iron;	//服务器发送玩家基本信息-镔铁
	int m_infantry_num;	//服务器发送玩家基本信息-步兵数
	int m_cavalry_num;	//服务器发送玩家基本信息-骑兵数
	int m_archer_num;	//服务器发送玩家基本信息-弓兵数
	short m_mokilllv;	//服务器发送玩家基本信息-击杀野怪最高级别
	int m_sflag;	//服务器发送玩家基本信息-标志位
	char m_autobuild;	//服务器发送玩家基本信息-自动建造
	char m_autobuildopen;	//服务器发送玩家基本信息-自动建造
	short m_techlevel[40];	//服务器发送玩家基本信息-科技等级
	short m_techprogress[40];	//服务器发送玩家基本信息-科技进度
	SLK_NetS_OfficialHire m_officialhire[3];	//服务器发送玩家基本信息-雇佣官
	int m_ptsec;	//服务器发送玩家基本信息-保护时间
	short m_forgingkind;	//服务器发送玩家基本信息-打造
	int m_forgingsec;	//服务器发送玩家基本信息-打造
	int m_actor_sflag;	//服务器发送玩家基本信息-玩家结构标志
	short m_bodysec;	//服务器发送玩家基本信息-体力恢复时间
	short m_game_day;	//服务器发送玩家基本信息-游戏虚拟日期
	short m_game_weather;	//服务器发送玩家基本信息-游戏虚拟天气
	char m_state;	//服务器发送玩家基本信息-城池状态
	char m_guardnum;	//服务器发送玩家基本信息-守卫数量
	short m_guardsec;	//服务器发送玩家基本信息-守卫数量
	int m_questid;	//服务器发送玩家基本信息-主线任务id
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

struct _slk_NetS_ZoneUnit {
	short m_posx;	//地图地区显示单元
	short m_posy;	//地图地区显示单元
	char m_nation;	//地图地区显示单元
	char m_level;	//地图地区显示单元
	int m_zoneunit_index;	//添加地图地区显示单元
};
typedef struct _slk_NetS_ZoneUnit SLK_NetS_ZoneUnit;	//地区单元

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
	short m_short_value[16];	//地图单元-通用字段
	char m_int_value_count;	//地图单元-通用字段长度
	int m_int_value[16];	//地图单元-通用字段
	char m_prefixlen;	//地图单元-称号长度
	char m_prefix[22];	//地图单元-称号
};
typedef struct _slk_NetS_AddMapUnit SLK_NetS_AddMapUnit;	//显示单元进入地图

struct _slk_NetS_DelMapUnit {
	int m_unit_index;	//地图单元-序号
};
typedef struct _slk_NetS_DelMapUnit SLK_NetS_DelMapUnit;	//显示单元离开地图

struct _slk_NetS_WorldMapInfo {
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
	char m_from_nation;	//行军路线-国家
	int m_to_actorid;	//行军路线-目标城市
	char m_to_nation;	//行军路线-目标国家
	int m_army_index;	//行军路线-所属部队
	char m_action;	//行军路线-行为
	char m_from_grid;	//行军路线-出发占地格数
	char m_to_grid;	//行军路线-目的占地格数
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

struct _slk_NetS_ArmySpeedUpdate {
	int m_unit_index;	//更新部队速度
	char m_state;	//更新部队速度
	int m_statetime;	//更新部队速度
	int m_stateduration;	//更新部队速度
};
typedef struct _slk_NetS_ArmySpeedUpdate SLK_NetS_ArmySpeedUpdate;	//部队加速

struct _slk_NetS_Equip {
	int m_offset;	//装备索引
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

struct _slk_NetS_HeroEquip {
	short m_herokind;	//英雄装备
	short m_count;	//英雄装备
	SLK_NetS_Equip m_list[6];	//英雄装备
};
typedef struct _slk_NetS_HeroEquip SLK_NetS_HeroEquip;	//英雄装备

struct _slk_NetS_Hero {
	short m_kind;	//
	char m_color;	//
	short m_level;	//
	char m_corps;	//
	int m_exp;	//
	int m_exp_max;	//
	int m_soldiers;	//
	char m_state;	//
	short m_attack_base;	//
	short m_attack_wash;	//
	short m_defense_base;	//
	short m_defense_wash;	//
	short m_troops_base;	//
	short m_troops_wash;	//
	int m_attack;	//
	int m_defense;	//
	int m_troops;	//
	int m_attack_increase;	//
	int m_defense_increase;	//
	short m_offset;	//
};
typedef struct _slk_NetS_Hero SLK_NetS_Hero;	//英雄信息

struct _slk_NetS_HeroList {
	short m_count;	//英雄发送数量
	SLK_NetS_Hero m_list[32];	//英雄列表
	char m_type;	//0上阵英雄1背包英雄
};
typedef struct _slk_NetS_HeroList SLK_NetS_HeroList;	//英雄列表

struct _slk_NetS_HeroExp {
	short m_kind;	//英雄种类
	int m_exp;	//英雄当前经验
	int m_exp_max;	//英雄经验上限
	short m_add;	//英雄获取经验
	char m_isup;	//是否升级
	short m_level;	//英雄等级
	short m_path;	//途径
};
typedef struct _slk_NetS_HeroExp SLK_NetS_HeroExp;	//英雄经验改变

struct _slk_NetS_HeroSoldiers {
	short m_kind;	//英雄种类
	int m_add;	//添加兵数量
	int m_soldiers;	//当前兵数
	int m_soldiers_max;	//兵数上限
	short m_path;	//途径
};
typedef struct _slk_NetS_HeroSoldiers SLK_NetS_HeroSoldiers;	//英雄兵力

struct _slk_NetS_HeroState {
	short m_kind;	//英雄种类
	char m_state;	//英雄状态
};
typedef struct _slk_NetS_HeroState SLK_NetS_HeroState;	//英雄状态

struct _slk_NetS_HeroReplace {
	short m_up_kind;	//上阵武将
	short m_down_kind;	//下阵武将
	short m_up_offset;	//
	short m_down_offset;	//
};
typedef struct _slk_NetS_HeroReplace SLK_NetS_HeroReplace;	//英雄上下阵替换

struct _slk_NetS_HeroGet {
	short m_kind;	//获取的武将
	short m_path;	//途径
	SLK_NetS_Hero m_hero;	//获取的英雄信息
	short m_itemnum;	//转换的道具数量
};
typedef struct _slk_NetS_HeroGet SLK_NetS_HeroGet;	//英雄获取

struct _slk_NetS_AwardInfo {
	int m_kind;	//种类
	int m_num;	//数量
};
typedef struct _slk_NetS_AwardInfo SLK_NetS_AwardInfo;	//奖励信息

struct _slk_NetS_AwardInfoList {
	short m_count;	//奖励数量
	SLK_NetS_AwardInfo m_list[32];	//奖励列表
	short m_callback_code;	//哪个系统要显示，回传
	int m_value;	//额外值
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
	short m_bodysec;	//体力回复时间
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

struct _slk_NetS_BuildingUpgradeInfo {
	short m_citylevel;	//建筑升级所需官府等级
	short m_actorlevel;	//建筑升级所需主角等级
	int m_silver;	//建筑升级所需银币
	int m_wood;	//所需木材建筑升级
	int m_food;	//建筑升级所需食物
	int m_iron;	//建筑升级所需镔铁
	int m_sec;	//建筑升级所需时间
	char m_maxlevel;	//建筑最大等级
};
typedef struct _slk_NetS_BuildingUpgradeInfo SLK_NetS_BuildingUpgradeInfo;	//建筑升级信息

struct _slk_NetS_Worker {
	char m_worker_kind;	//服务器发送-建筑队列种类
	char m_worker_offset;	//服务器发送建筑队列索引
	char m_worker_op;	//服务器发送-建筑队列建筑操作
	int m_worker_sec;	//服务器发送-建筑队列剩余时间
	int m_worker_needsec;	//服务器发送-建筑队列需要时间
	char m_worker_free;	//服务器发送-建筑队列免费时间
	char m_worker_kind_ex;	//服务器发送-建筑队列种类(商用)
	char m_worker_offset_ex;	//服务器发送-建筑队列索引(商用)
	char m_worker_op_ex;	//服务器发送-建筑队列操作(商用)
	int m_worker_sec_ex;	//服务器发送-建筑队列剩余时间(商用)
	int m_worker_needsec_ex;	//服务器发送-建筑队列需要时间(商用)
	char m_worker_free_ex;	//服务器发送-建筑队列免费时间(商用)
	int m_worker_expire_ex;	//服务器发送-商用建造队列到期时间
	int m_wnquick;	//服务器发送-奖励的额外加速
	int m_wnquick_ex;	//服务器发送-奖励的额外加速(商用)
};
typedef struct _slk_NetS_Worker SLK_NetS_Worker;	//建筑队列信息

struct _slk_NetS_BuildingGet {
	short m_path;	//获取建筑
	SLK_NetS_Building m_building;	//获取建筑
};
typedef struct _slk_NetS_BuildingGet SLK_NetS_BuildingGet;	//普通建筑获取

struct _slk_NetS_BuildingBarracksGet {
	short m_path;	//获取建筑
	SLK_NetS_BuildingBarracks m_barracks;	//获取建筑
};
typedef struct _slk_NetS_BuildingBarracksGet SLK_NetS_BuildingBarracksGet;	//兵营建筑获取

struct _slk_NetS_BuildingResGet {
	short m_path;	//获取建筑
	SLK_NetS_BuildingRes m_res;	//获取建筑
};
typedef struct _slk_NetS_BuildingResGet SLK_NetS_BuildingResGet;	//资源建筑获取

struct _slk_NetS_Soldiers {
	char m_corps;	//城内兵力改变
	int m_soldiers;	//城内兵力改变
	int m_add;	//城内兵力改变
	short m_path;	//城内兵力改变
};
typedef struct _slk_NetS_Soldiers SLK_NetS_Soldiers;	//兵力变化

struct _slk_NetS_TrainInfo {
	int m_soldiers;	//本营兵力
	int m_soldiers_max;	//兵营容量
	int m_trainnum;	//当前训练士兵数
	int m_trainsec;	//当前训练剩余时间(秒)
	int m_trainsec_need;	//当前训练需要时间(秒)
	int m_queuenum[16];	//队列
	char m_queue;	//扩建
	char m_trainlong;	//募兵加时
	int m_train_confnum;	//每五分钟训练士兵数
	int m_train_confsec;	//招募最大时长时间
	int m_train_conffood;	//单兵耗粮
};
typedef struct _slk_NetS_TrainInfo SLK_NetS_TrainInfo;	//训练士兵

struct _slk_NetS_Quest {
	short m_questid;	//编号
	char m_flag;	//完成标记
	char m_datatype;	//任务类型
	short m_datakind;	//任务种类
	short m_dataoffset;	//任务编号
	int m_value;	//值
	int m_needvalue;	//需要值
	int m_awardkind[5];	//奖励
	int m_awardnum[5];	//奖励
	int m_nameid;	//名称ID
	int m_descid;	//描述ID
};
typedef struct _slk_NetS_Quest SLK_NetS_Quest;	//任务

struct _slk_NetS_QuestList {
	short m_count;	//任务列表
	SLK_NetS_Quest m_list[16];	//任务列表
};
typedef struct _slk_NetS_QuestList SLK_NetS_QuestList;	//任务列表

struct _slk_NetS_QuestAward {
	short m_questid;	//任务编号
	char m_count;	//奖励数量
	SLK_NetS_AwardInfo m_list[5];	//任务奖励
	char m_datatype;	//任务类型
	short m_datakind;	//任务种类
	char m_dataoffset;	//任务编号
	int m_value;	//值
	int m_needvalue;	//需要值
	int m_nameid;	//名称ID
	char m_type;	//类型
};
typedef struct _slk_NetS_QuestAward SLK_NetS_QuestAward;	//任务奖励

struct _slk_NetS_Function {
	int m_function;	//开启的功能列表
	char m_openoffset;	//开启的功能列表
	short m_path;	//途径
};
typedef struct _slk_NetS_Function SLK_NetS_Function;	//开启的功能列表

struct _slk_NetS_CityGuard {
	char m_corps;	//守卫兵种
	char m_color;	//守卫颜色
	char m_shape;	//守卫形象
	short m_level;	//守卫等级
	int m_soldiers;	//守卫兵力
	int m_troops;	//守卫兵力
	char m_offset;	//守卫索引
};
typedef struct _slk_NetS_CityGuard SLK_NetS_CityGuard;	//守卫

struct _slk_NetS_CityGuardList {
	short m_count;	//守卫数量
	SLK_NetS_CityGuard m_list[32];	//守卫列表
	short m_guardsec;	//守卫召唤冷却
};
typedef struct _slk_NetS_CityGuardList SLK_NetS_CityGuardList;	//守卫列表

struct _slk_NetS_CityGuardSec {
	short m_guardsec;	//消除冷却结果
};
typedef struct _slk_NetS_CityGuardSec SLK_NetS_CityGuardSec;	//守卫冷却消除

struct _slk_NetS_BuildingSmithy {
	short m_forgingkind;	//服务器发送-铁匠铺
	int m_forgingsec;	//服务器发送-铁匠铺
	int m_forgingsec_need;	//服务器发送-铁匠铺
};
typedef struct _slk_NetS_BuildingSmithy SLK_NetS_BuildingSmithy;	//铁匠铺

struct _slk_NetS_ChangeName {
	short m_name_length;	//修改名称
	char m_name[32];	//修改名称
};
typedef struct _slk_NetS_ChangeName SLK_NetS_ChangeName;	//改名

struct _slk_NetS_BuildingAction {
	short m_kind;	//建筑种类
	short m_offset;	//建筑索引
	short m_action;	//执行动作
};
typedef struct _slk_NetS_BuildingAction SLK_NetS_BuildingAction;	//建筑动作

struct _slk_NetS_LevyInfo {
	int m_base[4];	//基础
	int m_tech[4];	//科技加成
	int m_weather[4];	//天气加成
	int m_activity[4];	//活动加成
	int m_offical[4];	//官职加成
	int m_sec;	//冷却倒计时
};
typedef struct _slk_NetS_LevyInfo SLK_NetS_LevyInfo;	//征收信息

struct _slk_NetS_Chat {
	int m_actorid;	//聊天信息-角色id
	char m_shape;	//聊天信息-形象
	short m_level;	//聊天信息-等级
	char m_namelen;	//聊天信息-名称
	char m_name[32];	//聊天信息-名称
	char m_frame;	//聊天信息-框体
	char m_zone;	//聊天信息-区域
	char m_place;	//聊天信息-职位
	short m_msglen;	//聊天信息-消息
	char m_msg[128];	//聊天信息-消息
	int m_optime;	//聊天信息-时间
	char m_channel;	//聊天信息-频道
	char m_nation;	//聊天信息-国家
	char m_msgtype;	//聊天信息-消息类型
};
typedef struct _slk_NetS_Chat SLK_NetS_Chat;	//聊天信息

struct _slk_NetS_ChatList {
	char m_count;	//聊天列表
	SLK_NetS_Chat m_list[10];	//聊天列表
	char m_channel;	//聊天列表
};
typedef struct _slk_NetS_ChatList SLK_NetS_ChatList;	//聊天信息列表

struct _slk_NetS_SystalkidValue {
	char m_vlen;	//系统消息拼接串长度
	char m_v[64];	//系统消息拼接串
};
typedef struct _slk_NetS_SystalkidValue SLK_NetS_SystalkidValue;	//文字表系统消息

struct _slk_NetS_Systalkid {
	char m_count;	//系统消息
	SLK_NetS_SystalkidValue m_msglist[8];	//系统消息
	int m_textid;	//文字表ID
	int m_optime;	//时间
};
typedef struct _slk_NetS_Systalkid SLK_NetS_Systalkid;	//文字表系统消息

struct _slk_NetS_Systalk {
	short m_msglen;	//直接文字的系统消息
	char m_msg[1024];	//直接文字的系统消息
	int m_optime;	//时间
	char m_roll;	//是否有滚动
};
typedef struct _slk_NetS_Systalk SLK_NetS_Systalk;	//直接文字系统消息

struct _slk_NetS_Battlepower {
	int m_total;	//改变战力
	short m_path;	//改变战力
};
typedef struct _slk_NetS_Battlepower SLK_NetS_Battlepower;	//改变战力

struct _slk_NetS_TechChange {
	char m_kind;	//科技变化
	char m_level;	//科技变化
	char m_progress;	//科技变化
};
typedef struct _slk_NetS_TechChange SLK_NetS_TechChange;	//科技变化

struct _slk_NetS_CityEvent {
	char m_type;	//城内事件
	short m_kind;	//城内事件
	int m_value;	//城内事件
	int m_optime;	//城内事件
};
typedef struct _slk_NetS_CityEvent SLK_NetS_CityEvent;	//城池事件

struct _slk_NetS_BattleEvent {
	char m_type;	//军事事件
	char m_name[22];	//军事事件
	char m_value;	//军事事件
	int m_optime;	//军事事件
	i64 m_mailid;	//军事事件
};
typedef struct _slk_NetS_BattleEvent SLK_NetS_BattleEvent;	//军事事件

struct _slk_NetS_EventList {
	short m_cevent_count;	//事件列表
	SLK_NetS_CityEvent m_cevent_list[4];	//事件列表
	short m_bevent_count;	//事件列表
	SLK_NetS_BattleEvent m_bevent_list[4];	//事件列表
};
typedef struct _slk_NetS_EventList SLK_NetS_EventList;	//城内事件列表

struct _slk_NetS_OfficialHireChange {
	char m_type;	//雇佣官信息变化
	SLK_NetS_OfficialHire m_info;	//雇佣官信息变化
};
typedef struct _slk_NetS_OfficialHireChange SLK_NetS_OfficialHireChange;	//官员变化

struct _slk_NetS_CityProtect {
	int m_total;	//改变保护时间
	int m_add;	//改变保护时间
	short m_path;	//改变保护时间
};
typedef struct _slk_NetS_CityProtect SLK_NetS_CityProtect;	//城池保护时间

struct _slk_NetS_HeroWash {
	char m_hero_washnum;	//洗髓次数
	int m_hero_washsec;	//洗髓时间
};
typedef struct _slk_NetS_HeroWash SLK_NetS_HeroWash;	//武将洗髓

struct _slk_NetS_HeroColorup {
	int m_value;	//突破值
	char m_isup;	//突破成功
};
typedef struct _slk_NetS_HeroColorup SLK_NetS_HeroColorup;	//武将突破

struct _slk_NetS_CityAttr {
	short m_protectres_per;	//
	short m_buildingsec_per;	//
	short m_materialsec_per[2];	//
	short m_movespeed_per[3];	//
	short m_gather_per[2];	//
	char m_hero_up_num;	//
	char m_hero_row_num;	//
	char m_everyday_body_buymax;	//
	char m_everyday_autobuild_buynum;	//
	char m_everyday_army_recall;	//
	char m_ability_open_201;	//
	char m_ability_open_203;	//
	char m_ability_open_204;	//
	char m_ability_open_205;	//
	char m_ability_open_206;	//
	char m_ability_open_207;	//
};
typedef struct _slk_NetS_CityAttr SLK_NetS_CityAttr;	//城池属性

struct _slk_NetS_EquipWash {
	char m_equip_washnum;	//装备洗练
	int m_equip_washsec;	//装备洗练
};
typedef struct _slk_NetS_EquipWash SLK_NetS_EquipWash;	//装备洗练

struct _slk_NetS_MaterialInfo {
	char m_matkind;	//材料生产
	char m_matnum;	//材料生产
	int m_matsec;	//材料生产
	int m_matsec_need;	//材料生产
};
typedef struct _slk_NetS_MaterialInfo SLK_NetS_MaterialInfo;	//材料生产信息

struct _slk_NetS_MaterialList {
	char m_count;	//材料生产
	SLK_NetS_MaterialInfo m_list[8];	//材料生产
	char m_matquenum;	//材料生产
	int m_nation_people;	//材料生产
	int m_city_people;	//材料生产
	int m_change_nation_people;	//材料生产
	int m_change_city_people;	//材料生产
};
typedef struct _slk_NetS_MaterialList SLK_NetS_MaterialList;	//材料生产列表

struct _slk_NetS_MaterialWillInfo {
	char m_matkind;	//预定生产
	int m_matsec;	//预定生产
	char m_matkind_will;	//预定生产
	char m_matnum_will;	//预定生产
};
typedef struct _slk_NetS_MaterialWillInfo SLK_NetS_MaterialWillInfo;	//预定材料生产信息

struct _slk_NetS_MaterialWillList {
	char m_count;	//预定生产
	SLK_NetS_MaterialWillInfo m_list[8];	//预定生产
};
typedef struct _slk_NetS_MaterialWillList SLK_NetS_MaterialWillList;	//预定材料生产列表

struct _slk_NetS_StoryList {
	short m_story_star[128];	//副本信息-副本状态
	short m_story_hero[32];	//副本信息-招募副本
	int m_story_restime[32];	//副本信息-资源副本时间
	short m_story_resnum[32];	//副本信息-资源副本次数
	short m_story_resreset[32];	//副本信息-资源副本重置次数
	short m_story_itemnum[64];	//副本信息-道具副本掉落次数
	short m_story_drawing[16];	//副本信息-装备图纸副本是否购买
	short m_sweep_herokind[4];	//副本信息-扫荡武将kind
	short m_storyid;	//副本信息-玩家进度
};
typedef struct _slk_NetS_StoryList SLK_NetS_StoryList;	//副本信息

struct _slk_NetS_MonsterInfo {
	int m_monsterid;	//怪物信息
	short m_shape;	//怪物信息
	short m_level;	//怪物信息
	char m_color;	//怪物信息
	char m_corps;	//怪物信息
	int m_hp;	//怪物信息
};
typedef struct _slk_NetS_MonsterInfo SLK_NetS_MonsterInfo;	//怪物信息

struct _slk_NetS_StoryRank {
	char m_count;	//副本关卡信息
	SLK_NetS_MonsterInfo m_list[16];	//副本关卡信息
	int m_exp;	//副本关卡信息
	char m_body;	//副本关卡信息
	char m_type;	//副本关卡信息
};
typedef struct _slk_NetS_StoryRank SLK_NetS_StoryRank;	//副本关卡

struct _slk_NetS_StoryState {
	int m_storyid;	//副本id
	int m_state;	//副本状态
	short m_saveoffset;	//存档索引
	short m_savetype;	//类型
	short m_actor_storyid;	//玩家副本进度id
};
typedef struct _slk_NetS_StoryState SLK_NetS_StoryState;	//副本状态更新

struct _slk_NetS_StoryRanknum {
	int m_storyid;	//副本id
	short m_num;	//副本次数
	short m_saveoffset;	//存档索引
};
typedef struct _slk_NetS_StoryRanknum SLK_NetS_StoryRanknum;	//副本关卡次数更新

struct _slk_NetS_StoryRanktime {
	int m_storyid;	//副本id
	int m_time;	//副本时间
	short m_saveoffset;	//存档索引
};
typedef struct _slk_NetS_StoryRanktime SLK_NetS_StoryRanktime;	//副本关卡时间更新

struct _slk_NetS_MapZoneChange {
	char m_zoneid;	//地区id变换
	char m_open;	//地区id是否开启
	char m_nation;	//地区所属
};
typedef struct _slk_NetS_MapZoneChange SLK_NetS_MapZoneChange;	//地图地区切换

struct _slk_NetS_MapZoneUnitList {
	short m_count;	//地图地区单元数量
	SLK_NetS_ZoneUnit m_list[256];	//地图地区单元列表
	char m_zoneid;	//地图地区id
};
typedef struct _slk_NetS_MapZoneUnitList SLK_NetS_MapZoneUnitList;	//地图地区单元列表

struct _slk_NetS_BattleInfo {
	int m_army_index;	//出征队列信息
	int m_unit_index;	//出征队列信息
	char m_state;	//出征队列信息
	int m_statetime;	//出征队列信息
	int m_stateduration;	//出征队列信息
	char m_action;	//出征队列信息
	short m_to_posx;	//出征队列信息
	short m_to_posy;	//出征队列信息
	short m_herokind[4];	//出征队列信息
	char m_to_type;	//出征队列信息
};
typedef struct _slk_NetS_BattleInfo SLK_NetS_BattleInfo;	//出征信息

struct _slk_NetS_BattleList {
	short m_count;	//出征队列列表
	SLK_NetS_BattleInfo m_list[8];	//出征队列列表
	int m_unit_index;	//出征队列列表
};
typedef struct _slk_NetS_BattleList SLK_NetS_BattleList;	//出征队列列表

struct _slk_NetS_MapResInfo {
	short m_kind;	//采集目标kind
	int m_totalnum;	//资源点总量
	int m_totalsec;	//资源点总采集时间
	int m_spacenum;	//采集剩余量
	short m_herokind;	//采集武将
	short m_herolevel;	//采集武将等级
	int m_herohp;	//采集武将兵力
	char m_herocolor;	//采集武将颜色
	short m_actorlevel;	//采集玩家等级
};
typedef struct _slk_NetS_MapResInfo SLK_NetS_MapResInfo;	//资源点详情

struct _slk_NetS_WeatherChange {
	short m_game_day;	//天气系统
	short m_game_weather;	//天气系统
};
typedef struct _slk_NetS_WeatherChange SLK_NetS_WeatherChange;	//天气改变

struct _slk_NetS_Mail {
	i64 m_mailid;	//邮件-id
	char m_type;	//邮件-类型
	short m_title_len;	//邮件
	char m_title[128];	//邮件标题
	short m_content_len;	//邮件
	char m_content[1024];	//邮件-内容
	short m_attach_len;	//邮件
	char m_attach[256];	//邮件-附件
	char m_attachget;	//邮件-附件读取
	char m_read;	//邮件-是否读取
	int m_recvtime;	//邮件-接收时间
	i64 m_fightid;	//邮件-关联战斗
	char m_lock;	//邮件-锁定状态
	int m_actorid;	//邮件-所属玩家
	char m_viewpath;	//邮件-查看途径
};
typedef struct _slk_NetS_Mail SLK_NetS_Mail;	//一封邮件

struct _slk_NetS_MailOpResult {
	char m_op;	//邮件操作返回结果
	i64 m_mailid;	//邮件操作返回结果
};
typedef struct _slk_NetS_MailOpResult SLK_NetS_MailOpResult;	//邮件操作结果

struct _slk_NetS_MailFight {
	char m_flag;	//战斗邮件详情
	short m_content_length;	//战斗邮件详情
	char m_content[1800];	//战斗邮件详情
	i64 m_mailid;	//战斗邮件详情
};
typedef struct _slk_NetS_MailFight SLK_NetS_MailFight;	//战斗邮件详情

struct _slk_NetS_MailView {
	SLK_NetS_Mail m_mail;	//邮件查看
};
typedef struct _slk_NetS_MailView SLK_NetS_MailView;	//邮件查看

struct _slk_NetS_CItyHelp {
	short m_level;	//驻防列表-等级
	char m_actorname_length;	//驻防列表-玩家名
	char m_actorname[32];	//驻防列表-玩家名
	short m_herokind;	//驻防列表-武将
	int m_soldiers;	//驻防列表-兵力
	int m_actorid;	//驻防列表-角色id
	int m_army_index;	//驻防列表-部队索引
};
typedef struct _slk_NetS_CItyHelp SLK_NetS_CItyHelp;	//驻防

struct _slk_NetS_CItyHelpList {
	short m_count;	//驻防列表
	SLK_NetS_CItyHelp m_list[32];	//驻防列表
	short m_walllevel;	//驻防列表
};
typedef struct _slk_NetS_CItyHelpList SLK_NetS_CItyHelpList;	//驻防列表

struct _slk_NetS_MapCItyHelpList {
	short m_count;	//地图查看驻防列表
	SLK_NetS_CItyHelp m_list[32];	//地图查看驻防列表
	short m_walllevel;	//地图查看驻防列表
};
typedef struct _slk_NetS_MapCItyHelpList SLK_NetS_MapCItyHelpList;	//地图查看驻防列表

struct _slk_NetS_CityState {
	char m_state;	//城池当前状态
	char m_change;	//城池当前状态
};
typedef struct _slk_NetS_CityState SLK_NetS_CityState;	//城池状态

struct _slk_NetS_CityArmyGroup {
	int m_group_index;	//城战信息-集结索引
	int m_group_id;	//城战信息-集结id
	char m_attack;	//城战信息-1攻击2防御
	int m_statetime;	//城战信息-倒计时
	int m_stateduration;	//城战信息-倒计时
	char m_nation;	//城战信息
	char m_t_nation;	//城战信息
	short m_level;	//城战信息
	short m_t_level;	//城战信息
	char m_name_length;	//城战信息
	char m_name[32];	//城战信息
	char m_t_name_length;	//城战信息
	char m_t_name[32];	//城战信息
	short m_posx;	//城战信息
	short m_posy;	//城战信息
	short m_t_posx;	//城战信息
	short m_t_posy;	//城战信息
	int m_actorid;	//城战信息
	int m_t_actorid;	//城战信息
	int m_total;	//城战信息
	int m_t_total;	//城战信息
	char m_type;	//城战信息
};
typedef struct _slk_NetS_CityArmyGroup SLK_NetS_CityArmyGroup;	//城战信息

struct _slk_NetS_CityArmyGroupList {
	short m_count;	//城战信息列表
	SLK_NetS_CityArmyGroup m_list[16];	//城战信息列表
	char m_nation;	//城战信息列表
	int m_unit_index;	//城战信息列表
	short m_totalcount;	//城战信息列表
	char m_flag;	//城战信息列表
};
typedef struct _slk_NetS_CityArmyGroupList SLK_NetS_CityArmyGroupList;	//城战列表

struct _slk_NetS_MapTownInfo {
	int m_protect_sec;	//城镇详情
	int m_produce_sec;	//城镇详情
	int m_own_actorid;	//城镇详情
	char m_own_namelen;	//城镇详情
	char m_own_name[32];	//城镇详情
	int m_own_sec;	//城镇详情
	int m_hp;	//城镇详情
	int m_maxhp;	//城镇详情
	char m_myask;	//城镇详情
	short m_produce_num;	//城镇详情
};
typedef struct _slk_NetS_MapTownInfo SLK_NetS_MapTownInfo;	//城镇详情

struct _slk_NetS_TownArmyGroup {
	int m_group_index;	//国战信息-集结索引
	int m_group_id;	//国战信息-集结id
	char m_attack;	//国战信息-1攻击2防御
	int m_statetime;	//国战信息-倒计时
	int m_stateduration;	//国战信息-倒计时
	char m_nation;	//国战信息
	char m_t_nation;	//国战信息
	int m_total;	//国战信息
	int m_t_total;	//国战信息
	char m_type;	//国战信息
};
typedef struct _slk_NetS_TownArmyGroup SLK_NetS_TownArmyGroup;	//国战信息

struct _slk_NetS_TownArmyGroupList {
	short m_count;	//国战信息列表
	SLK_NetS_TownArmyGroup m_list[16];	//国战信息列表
	char m_flag;	//国战信息列表
	char m_nation;	//国战信息列表
	int m_unit_index;	//国战信息列表
	short m_totalcount;	//国战信息列表
	int m_townid;	//国战信息列表
};
typedef struct _slk_NetS_TownArmyGroupList SLK_NetS_TownArmyGroupList;	//国战列表

struct _slk_NetS_SystalkJson {
	short m_msglen;	//json系统消息
	char m_msg[1024];	//json系统消息
	int m_optime;	//json系统消息
	char m_roll;	//json系统消息
};
typedef struct _slk_NetS_SystalkJson SLK_NetS_SystalkJson;	//json系统消息

struct _slk_NetS_RollMsgJson {
	short m_msglen;	//json滚动消息
	char m_msg[1024];	//json滚动消息
};
typedef struct _slk_NetS_RollMsgJson SLK_NetS_RollMsgJson;	//json滚动消息

struct _slk_NetS_RollMsg {
	short m_msglen;	//滚动消息
	char m_msg[1024];	//滚动消息
};
typedef struct _slk_NetS_RollMsg SLK_NetS_RollMsg;	//滚动消息

struct _slk_NetS_TownOwnerAsk {
	char m_name_len;	//城主申请玩家
	char m_name[32];	//城主申请玩家
	char m_place;	//城主申请玩家
};
typedef struct _slk_NetS_TownOwnerAsk SLK_NetS_TownOwnerAsk;	//城主申请信息

struct _slk_NetS_TownOwnerAskList {
	char m_count;	//城主申请列表
	SLK_NetS_TownOwnerAsk m_list[5];	//城主申请列表
	int m_sec;	//城主申请列表
};
typedef struct _slk_NetS_TownOwnerAskList SLK_NetS_TownOwnerAskList;	//城主申请列表

struct _slk_NetS_TownFight {
	short m_townid;	//国家战争城镇id
	int m_statetime;	//国家战争倒计时
	char m_attack;	//国家战争攻击还是防守
	char m_nation;	//国家战争城镇国家
};
typedef struct _slk_NetS_TownFight SLK_NetS_TownFight;	//国战信息

struct _slk_NetS_TownFightList {
	short m_count;	//国家战争列表
	SLK_NetS_TownFight m_list[200];	//国家战争列表
};
typedef struct _slk_NetS_TownFightList SLK_NetS_TownFightList;	//国战列表

struct _slk_NetS_MapTownExInfo {
	char m_dev_level;	//都城开发等级
	int m_dev_exp;	//都城开发经验
	int m_dev_expmax;	//都城开发经验
	short m_mytownid;	//我的都城
	int m_dev_cd;	//都城开发冷却
	int m_attackcd;	//攻击倒计时
};
typedef struct _slk_NetS_MapTownExInfo SLK_NetS_MapTownExInfo;	//国都信息

struct _slk_NetS_MapZoneTown {
	short m_townid;	//地区城镇列表
	char m_nation;	//地区城镇列表
	int m_protect_sec;	//地区城镇列表
	short m_from_nation[4];	//地区城镇列表
};
typedef struct _slk_NetS_MapZoneTown SLK_NetS_MapZoneTown;	//地区城镇信息

struct _slk_NetS_MapZoneTownList {
	short m_count;	//地区城镇列表
	SLK_NetS_MapZoneTown m_list[32];	//地区城镇列表
	char m_zoneid;	//地区城镇列表
};
typedef struct _slk_NetS_MapZoneTownList SLK_NetS_MapZoneTownList;	//地区城镇信息列表

struct _slk_NetS_MapCenterTown {
	short m_townid;	//皇城区域城镇列表
	char m_nation;	//皇城区域城镇列表
};
typedef struct _slk_NetS_MapCenterTown SLK_NetS_MapCenterTown;	//皇城地区城镇信息

struct _slk_NetS_MapCenterTownList {
	char m_count;	//皇城区域城镇列表
	SLK_NetS_MapCenterTown m_list[32];	//皇城区域城镇列表
};
typedef struct _slk_NetS_MapCenterTownList SLK_NetS_MapCenterTownList;	//皇城地区城镇列表

struct _slk_NetS_CityWarInfo {
	int m_group_index;	//军情信息-团战索引
	int m_group_id;	//军情信息-团战id
	char m_from_nation;	//军情信息-发起人国家
	short m_from_posx;	//军情信息-发起人坐标
	short m_from_posy;	//军情信息-发起人坐标
	char m_namelen;	//军情信息-发起人名称
	char m_name[32];	//军情信息-发起人名称
	int m_statetime;	//军情信息-时间
	int m_stateduration;	//军情信息-时间
};
typedef struct _slk_NetS_CityWarInfo SLK_NetS_CityWarInfo;	//城战警告

struct _slk_NetS_CityWarList {
	char m_count;	//军情列表
	SLK_NetS_CityWarInfo m_list[16];	//军情列表
};
typedef struct _slk_NetS_CityWarList SLK_NetS_CityWarList;	//城战警告列表

struct _slk_NetS_CityWarDel {
	int m_group_index;	//删除军情
};
typedef struct _slk_NetS_CityWarDel SLK_NetS_CityWarDel;	//城战警告删除

struct _slk_NetS_WorldQuest {
	short m_questid;	//世界任务
	short m_value;	//世界任务
	short m_maxvalue;	//世界任务
	char m_complete;	//世界任务
	char m_dd;	//是否双倍
};
typedef struct _slk_NetS_WorldQuest SLK_NetS_WorldQuest;	//世界任务

struct _slk_NetS_WorldBoss {
	char m_bossid;	//世界boos信息
	int m_hp;	//世界boos信息
	int m_maxhp;	//世界boos信息
	char m_isfight;	//世界boos信息今天是否打过
	short m_actorlevel;	//世界boos信息等级限制
};
typedef struct _slk_NetS_WorldBoss SLK_NetS_WorldBoss;	//世界boss信息

struct _slk_NetS_LostRebuild {
	int m_rb_silver;	//家园重建
	int m_rb_wood;	//家园重建
	int m_rb_food;	//家园重建
	int m_rb_df;	//家园重建
};
typedef struct _slk_NetS_LostRebuild SLK_NetS_LostRebuild;	//家园重建

struct _slk_NetS_KingwarTown {
	char m_id;	//血战据点
	int m_attack_total;	//血战据点
	int m_defense_total;	//血战据点
	char m_nation;	//血战据点
};
typedef struct _slk_NetS_KingwarTown SLK_NetS_KingwarTown;	//血战据点

struct _slk_NetS_KingwarTownList {
	char m_count;	//血战据点列表
	SLK_NetS_KingwarTown m_list[8];	//血战据点列表
	int m_leftstamp;	//血战据点列表
	int m_losthp;	//血战据点列表
};
typedef struct _slk_NetS_KingwarTownList SLK_NetS_KingwarTownList;	//血战据点列表

struct _slk_NetS_KingWarActivity {
	char m_state;	//血战据点活动信息
	int m_beginstamp;	//血战据点活动信息
	int m_endstamp;	//血战据点活动信息
	char m_nation;	//血战据点活动信息
};
typedef struct _slk_NetS_KingWarActivity SLK_NetS_KingWarActivity;	//血战活动

struct _slk_NetS_KingWarNotify {
	short m_a_heroid;	//血战活动战报
	char m_a_color;	//血战活动战报
	char m_a_name_len;	//血战活动战报
	char m_a_name[32];	//血战活动战报
	int m_a_losthp;	//血战活动战报
	char m_a_nation;	//血战活动战报
	short m_d_heroid;	//血战活动战报
	char m_d_color;	//血战活动战报
	char m_d_name_len;	//血战活动战报
	char m_d_name[32];	//血战活动战报
	int m_d_losthp;	//血战活动战报
	char m_d_nation;	//血战活动战报
	char m_result;	//血战活动战报
	char m_id;	//血战活动战报-发生据点
};
typedef struct _slk_NetS_KingWarNotify SLK_NetS_KingWarNotify;	//血战战报

struct _slk_NetS_KingWarNotifyList {
	char m_count;	//血战活动单挑战报列表
	SLK_NetS_KingWarNotify m_list[10];	//血战活动单挑战报列表
};
typedef struct _slk_NetS_KingWarNotifyList SLK_NetS_KingWarNotifyList;	//血战战报列表

struct _slk_NetS_KingWarRank {
	char m_rank;	//血战排行榜
	int m_kill;	//血战排行榜
	char m_nation;	//血战排行榜
	char m_name_len;	//血战排行榜
	char m_name[32];	//血战排行榜
	short m_herokind;	//血战排行榜
	char m_herocolor;	//血战排行榜
};
typedef struct _slk_NetS_KingWarRank SLK_NetS_KingWarRank;	//血战排行榜

struct _slk_NetS_KingWarRankList {
	short m_count;	//血战排行榜列表
	SLK_NetS_KingWarRank m_list[30];	//血战排行榜列表
	int m_mypoint;	//血战排行榜列表-我的总积分
	int m_totalkill;	//血战排行榜列表-当前累计杀敌
	char m_myrank;	//血战排行榜列表-我的排行
};
typedef struct _slk_NetS_KingWarRankList SLK_NetS_KingWarRankList;	//血战排行榜列表

struct _slk_NetS_KingWarPK {
	short m_herokind;	//血战单挑通知
	int m_hp;	//血战单挑通知
	char m_id;	//血战单挑通知
};
typedef struct _slk_NetS_KingWarPK SLK_NetS_KingWarPK;	//血战PK

struct _slk_NetS_KingWarPoint {
	int m_point;	//血战积分
};
typedef struct _slk_NetS_KingWarPoint SLK_NetS_KingWarPoint;	//血战积分

struct _slk_NetS_TreasureActivity {
	char m_state;	//挖宝活动信息
	int m_endstamp;	//挖宝活动信息
	char m_nation;	//挖宝活动信息
	short m_treasure_num[3];	//挖宝活动信息
	short m_treasure_num_max;	//挖宝活动信息
};
typedef struct _slk_NetS_TreasureActivity SLK_NetS_TreasureActivity;	//挖宝活动

struct _slk_NetS_TreasureHas {
	char m_has;	//是否已经挖到图纸
	short m_px;	//宝图坐标
	short m_py;	//宝图坐标
	char m_tn;	//挖宝次数
};
typedef struct _slk_NetS_TreasureHas SLK_NetS_TreasureHas;	//是否已经挖到图纸

struct _slk_NetS_GotoAsyn {
	short m_posx;	//异步跳转
	short m_posy;	//异步跳转
	char m_type;	//异步跳转
	int m_unit_index;	//异步跳转
	char m_op;	//异步跳转
};
typedef struct _slk_NetS_GotoAsyn SLK_NetS_GotoAsyn;	//异步跳转

struct _slk_NetS_TreasureActor {
	short m_itemkind;	//恭贺信息
	char m_name_len;	//恭贺信息
	char m_name[32];	//恭贺信息
};
typedef struct _slk_NetS_TreasureActor SLK_NetS_TreasureActor;	//挖红图信息

struct _slk_NetS_TreasureActorList {
	char m_count;	//恭贺列表
	SLK_NetS_TreasureActor m_list[16];	//恭贺列表
	char m_co;	//玩家是否已经恭贺
};
typedef struct _slk_NetS_TreasureActorList SLK_NetS_TreasureActorList;	//挖红图信息列表

struct _slk_NetS_ChangeAutoBuild {
	char m_autobuild;	//自动建造
	char m_autobuildopen;	//自动建造
	short m_path;	//自动建造
};
typedef struct _slk_NetS_ChangeAutoBuild SLK_NetS_ChangeAutoBuild;	//自动建造

struct _slk_NetS_HeroVisitInfo {
	int m_hv_free_cd;	//良将寻访免费CD
	int m_hv_high_sec;	//神将寻访解锁时长
	char m_hv_high_free;	//神将寻访剩余免费次数
	char m_hv_low_num;	//良将寻访次数
	char m_hv_high_num;	//神将寻访次数
	short m_hv_progress;	//寻访进度
};
typedef struct _slk_NetS_HeroVisitInfo SLK_NetS_HeroVisitInfo;	//武将寻访信息

struct _slk_NetS_NationInfo {
	char m_level;	//国家等级
	int m_exp;	//国家进度
	int m_exp_max;	//今天玩家建设免费
	char m_buildfree;	//今天玩家建设免费
	char m_buildnum;	//今天玩家建设次数
};
typedef struct _slk_NetS_NationInfo SLK_NetS_NationInfo;	//国家信息

struct _slk_NetS_HeroVisitAward {
	short m_id[10];	//寻访获取的奖励id
	char m_count;	//数量
};
typedef struct _slk_NetS_HeroVisitAward SLK_NetS_HeroVisitAward;	//寻访奖励id

struct _slk_NetS_FightPlay {
	char m_flag;	//战斗详情
	short m_content_length;	//战斗详情
	char m_content[1800];	//战斗详情
};
typedef struct _slk_NetS_FightPlay SLK_NetS_FightPlay;	//战斗播放

struct _slk_NetS_StorySweepHero {
	short m_kind;	//扫荡武将信息
	char m_color;	//扫荡武将信息
	short m_level;	//扫荡武将信息
	int m_pre_exp;	//扫荡武将信息
	int m_exp;	//扫荡武将信息
};
typedef struct _slk_NetS_StorySweepHero SLK_NetS_StorySweepHero;	//扫荡武将

struct _slk_NetS_StorySweepResult {
	int m_exp;	//扫荡结果
	int m_silver;	//扫荡结果
	char m_awardcount;	//扫荡结果
	SLK_NetS_AwardInfo m_award[16];	//扫荡结果
	char m_herocount;	//扫荡结果
	SLK_NetS_StorySweepHero m_hero[4];	//扫荡结果
};
typedef struct _slk_NetS_StorySweepResult SLK_NetS_StorySweepResult;	//扫荡结果

struct _slk_NetS_QuestTalk {
	int m_talkid;	//任务对话
	short m_herokind;	//任务对话
	int m_talk_textid;	//任务对话
	int m_btn_textid;	//任务对话
};
typedef struct _slk_NetS_QuestTalk SLK_NetS_QuestTalk;	//任务对话

struct _slk_NetS_RankInfo {
	int m_rank;	//排行榜
	char m_namelen;	//排行榜
	char m_name[32];	//排行榜
	char m_nation;	//排行榜
	short m_level;	//排行榜
	char m_place;	//排行榜
	int m_battlepower;	//排行榜
	int m_actorid;	//排行榜
};
typedef struct _slk_NetS_RankInfo SLK_NetS_RankInfo;	//排行榜信息

struct _slk_NetS_RankList {
	char m_count;	//排行榜列表
	SLK_NetS_RankInfo m_list[10];	//排行榜列表
	char m_type;	//排行榜列表-类型
	int m_page;	//排行榜列表-页号
	int m_myrank;	//排行榜列表-我的排名
};
typedef struct _slk_NetS_RankList SLK_NetS_RankList;	//排行榜列表

struct _slk_NetS_FriendInfo {
	int m_actorid;	//好友信息
	int m_city_index;	//好友信息
	char m_shape;	//好友信息
	char m_namelen;	//好友信息
	char m_name[32];	//好友信息
	short m_level;	//好友信息
	char m_place;	//好友信息
	int m_battlepower;	//好友信息
	char m_ask;	//好友信息
};
typedef struct _slk_NetS_FriendInfo SLK_NetS_FriendInfo;	//好友信息

struct _slk_NetS_FriendList {
	char m_count;	//好友列表
	SLK_NetS_FriendInfo m_list[40];	//好友列表
};
typedef struct _slk_NetS_FriendList SLK_NetS_FriendList;	//好友列表

struct _slk_NetS_ActorSearch {
	int m_actorid;	//查询玩家信息
	int m_city_index;	//查询玩家信息
	char m_namelen;	//查询玩家信息
	char m_name[32];	//查询玩家信息
	char m_signlen;	//查询玩家信息
	char m_sign[128];	//查询玩家信息
	short m_level;	//查询玩家信息
	char m_official;	//查询玩家信息
	char m_place;	//查询玩家信息
	char m_zone;	//查询玩家信息
	char m_nation;	//查询玩家信息
	int m_battlepower;	//查询玩家信息
	int m_killcity;	//查询玩家信息
	char m_herocount;	//查询玩家信息
	SLK_NetS_StorySweepHero m_hero[4];	//查询玩家信息
	int m_bp_hero;	//查询玩家信息
	int m_bp_equip;	//查询玩家信息
	int m_bp_tech;	//查询玩家信息
	int m_bp_nequip;	//查询玩家信息
	int m_bp_girl;	//查询玩家信息
	int m_bp_place;	//查询玩家信息
	int m_my_bp_hero;	//查询玩家信息
	int m_my_bp_equip;	//查询玩家信息
	int m_my_bp_tech;	//查询玩家信息
	int m_my_bp_nequip;	//查询玩家信息
	int m_my_bp_girl;	//查询玩家信息
	int m_my_bp_place;	//查询玩家信息
	char m_shape;	//查询玩家信息
};
typedef struct _slk_NetS_ActorSearch SLK_NetS_ActorSearch;	//玩家查询信息

struct _slk_NetS_BlackInfo {
	int m_actorid;	//黑名单
	char m_namelen;	//黑名单
	char m_name[32];	//黑名单
	short m_level;	//黑名单
	char m_nation;	//黑名单
	char m_shape;	//黑名单
};
typedef struct _slk_NetS_BlackInfo SLK_NetS_BlackInfo;	//黑名单

struct _slk_NetS_BlackList {
	char m_count;	//黑名单列表
	SLK_NetS_BlackInfo m_list[50];	//黑名单列表
};
typedef struct _slk_NetS_BlackList SLK_NetS_BlackList;	//黑名单列表

struct _slk_NetS_BlackListID {
	char m_count;	//黑名单列表
	int m_actorid[50];	//黑名单列表
};
typedef struct _slk_NetS_BlackListID SLK_NetS_BlackListID;	//黑名单列表只发id

struct _slk_NetS_NationEquip {
	short m_nequip_level[6];	//国器信息
	short m_nequip_exp[6];	//国器信息
	char m_nequip_kind;	//国器信息
	char m_nequip_pro;	//国器信息
	int m_nequip_sec;	//国器信息
};
typedef struct _slk_NetS_NationEquip SLK_NetS_NationEquip;	//国器信息

struct _slk_NetS_ShopItem {
	short m_offset;	//商店道具
	int m_awardkind;	//商店道具
	int m_awardnum;	//商店道具
	short m_token;	//商店道具
	short m_sale;	//商店道具
	char m_buyuse;	//商店道具
};
typedef struct _slk_NetS_ShopItem SLK_NetS_ShopItem;	//商店道具

struct _slk_NetS_ShopList {
	char m_type;	//商店列表
	char m_count;	//商店列表
	SLK_NetS_ShopItem m_list[64];	//商店列表
};
typedef struct _slk_NetS_ShopList SLK_NetS_ShopList;	//商店道具列表

struct _slk_NetS_PayStoreGoods {
	short m_goodsid;	//支付商店
	int m_price;	//支付商店
	int m_token;	//支付商店
	short m_day;	//支付商店
	short m_nameid;	//支付商店
	short m_icon;	//支付商店
};
typedef struct _slk_NetS_PayStoreGoods SLK_NetS_PayStoreGoods;	//支付商品

struct _slk_NetS_PayStore {
	char m_count;	//支付商店列表
	SLK_NetS_PayStoreGoods m_list[16];	//支付商店列表
};
typedef struct _slk_NetS_PayStore SLK_NetS_PayStore;	//支付商品商店

struct _slk_NetS_GoodsInfo {
	short m_goodsid;	//商品信息
	int m_price;	//商品信息
	int m_nameid;	//商品信息
	int m_descid;	//商品信息
	char m_icon;	//商品信息
	short m_sale;	//商品信息
	int m_worth;	//商品信息
	int m_bag_time;	//商品信息
	char m_bag_num;	//商品信息
	char m_awardcount;	//商品信息
	SLK_NetS_AwardInfo m_award[16];	//商品信息
};
typedef struct _slk_NetS_GoodsInfo SLK_NetS_GoodsInfo;	//礼包商品信息

struct _slk_NetS_PayStoreActivity {
	char m_count;	//礼包列表
	SLK_NetS_GoodsInfo m_list[8];	//礼包列表
};
typedef struct _slk_NetS_PayStoreActivity SLK_NetS_PayStoreActivity;	//礼包列表

struct _slk_NetS_PayOrder {
	char m_orderid_len;	//支付订单
	char m_orderid[32];	//支付订单
	char m_ext_len;	//支付订单
	char m_ext[64];	//支付订单
	short m_goodsid;	//支付订单
	int m_productid;	//支付订单
	int m_nameid;	//支付订单
	int m_descid;	//支付订单
	int m_price;	//支付订单
};
typedef struct _slk_NetS_PayOrder SLK_NetS_PayOrder;	//订单

struct _slk_NetS_VipShopItem {
	int m_awardkind;	//vip特价商品
	int m_awardnum;	//vip特价商品
	short m_token;	//vip特价商品
	short m_itemkind;	//vip特价商品
	short m_vip_token;	//vip特价商品
	short m_vip_buynum;	//vip特价商品
	short m_vip_buynum_max;	//vip特价商品
	char m_id;	//vip特价商品
};
typedef struct _slk_NetS_VipShopItem SLK_NetS_VipShopItem;	//vip特价商品

struct _slk_NetS_VipShop {
	short m_count;	//vip特价商店
	SLK_NetS_VipShopItem m_list[32];	//vip特价商店
	char m_useitem;	//vip特价商店
};
typedef struct _slk_NetS_VipShop SLK_NetS_VipShop;	//vip特价商店

struct _slk_NetS_VipBag {
	int m_vipbag;	//vip礼包
	char m_vipbag_count;	//vip礼包
};
typedef struct _slk_NetS_VipBag SLK_NetS_VipBag;	//vip礼包

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue );
int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue );
int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue );
int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue );
int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue );
int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue );
int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue );
int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue );
int struct_NetS_DialogUpdate_send( char **pptr, int *psize, SLK_NetS_DialogUpdate *pValue );
int struct_NetS_OfficialHire_send( char **pptr, int *psize, SLK_NetS_OfficialHire *pValue );
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
int struct_NetS_ZoneUnit_send( char **pptr, int *psize, SLK_NetS_ZoneUnit *pValue );
int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue );
int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue );
int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue );
int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue );
int struct_NetS_AddMarchRoute_send( char **pptr, int *psize, SLK_NetS_AddMarchRoute *pValue );
int struct_NetS_DelMarchRoute_send( char **pptr, int *psize, SLK_NetS_DelMarchRoute *pValue );
int struct_NetS_UpdateMapUnit_send( char **pptr, int *psize, SLK_NetS_UpdateMapUnit *pValue );
int struct_NetS_ArmySpeedUpdate_send( char **pptr, int *psize, SLK_NetS_ArmySpeedUpdate *pValue );
int struct_NetS_Equip_send( char **pptr, int *psize, SLK_NetS_Equip *pValue );
int struct_NetS_EquipList_send( char **pptr, int *psize, SLK_NetS_EquipList *pValue );
int struct_NetS_EquipGet_send( char **pptr, int *psize, SLK_NetS_EquipGet *pValue );
int struct_NetS_EquipLost_send( char **pptr, int *psize, SLK_NetS_EquipLost *pValue );
int struct_NetS_HeroEquip_send( char **pptr, int *psize, SLK_NetS_HeroEquip *pValue );
int struct_NetS_Hero_send( char **pptr, int *psize, SLK_NetS_Hero *pValue );
int struct_NetS_HeroList_send( char **pptr, int *psize, SLK_NetS_HeroList *pValue );
int struct_NetS_HeroExp_send( char **pptr, int *psize, SLK_NetS_HeroExp *pValue );
int struct_NetS_HeroSoldiers_send( char **pptr, int *psize, SLK_NetS_HeroSoldiers *pValue );
int struct_NetS_HeroState_send( char **pptr, int *psize, SLK_NetS_HeroState *pValue );
int struct_NetS_HeroReplace_send( char **pptr, int *psize, SLK_NetS_HeroReplace *pValue );
int struct_NetS_HeroGet_send( char **pptr, int *psize, SLK_NetS_HeroGet *pValue );
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
int struct_NetS_BuildingUpgradeInfo_send( char **pptr, int *psize, SLK_NetS_BuildingUpgradeInfo *pValue );
int struct_NetS_Worker_send( char **pptr, int *psize, SLK_NetS_Worker *pValue );
int struct_NetS_BuildingGet_send( char **pptr, int *psize, SLK_NetS_BuildingGet *pValue );
int struct_NetS_BuildingBarracksGet_send( char **pptr, int *psize, SLK_NetS_BuildingBarracksGet *pValue );
int struct_NetS_BuildingResGet_send( char **pptr, int *psize, SLK_NetS_BuildingResGet *pValue );
int struct_NetS_Soldiers_send( char **pptr, int *psize, SLK_NetS_Soldiers *pValue );
int struct_NetS_TrainInfo_send( char **pptr, int *psize, SLK_NetS_TrainInfo *pValue );
int struct_NetS_Quest_send( char **pptr, int *psize, SLK_NetS_Quest *pValue );
int struct_NetS_QuestList_send( char **pptr, int *psize, SLK_NetS_QuestList *pValue );
int struct_NetS_QuestAward_send( char **pptr, int *psize, SLK_NetS_QuestAward *pValue );
int struct_NetS_Function_send( char **pptr, int *psize, SLK_NetS_Function *pValue );
int struct_NetS_CityGuard_send( char **pptr, int *psize, SLK_NetS_CityGuard *pValue );
int struct_NetS_CityGuardList_send( char **pptr, int *psize, SLK_NetS_CityGuardList *pValue );
int struct_NetS_CityGuardSec_send( char **pptr, int *psize, SLK_NetS_CityGuardSec *pValue );
int struct_NetS_BuildingSmithy_send( char **pptr, int *psize, SLK_NetS_BuildingSmithy *pValue );
int struct_NetS_ChangeName_send( char **pptr, int *psize, SLK_NetS_ChangeName *pValue );
int struct_NetS_BuildingAction_send( char **pptr, int *psize, SLK_NetS_BuildingAction *pValue );
int struct_NetS_LevyInfo_send( char **pptr, int *psize, SLK_NetS_LevyInfo *pValue );
int struct_NetS_Chat_send( char **pptr, int *psize, SLK_NetS_Chat *pValue );
int struct_NetS_ChatList_send( char **pptr, int *psize, SLK_NetS_ChatList *pValue );
int struct_NetS_SystalkidValue_send( char **pptr, int *psize, SLK_NetS_SystalkidValue *pValue );
int struct_NetS_Systalkid_send( char **pptr, int *psize, SLK_NetS_Systalkid *pValue );
int struct_NetS_Systalk_send( char **pptr, int *psize, SLK_NetS_Systalk *pValue );
int struct_NetS_Battlepower_send( char **pptr, int *psize, SLK_NetS_Battlepower *pValue );
int struct_NetS_TechChange_send( char **pptr, int *psize, SLK_NetS_TechChange *pValue );
int struct_NetS_CityEvent_send( char **pptr, int *psize, SLK_NetS_CityEvent *pValue );
int struct_NetS_BattleEvent_send( char **pptr, int *psize, SLK_NetS_BattleEvent *pValue );
int struct_NetS_EventList_send( char **pptr, int *psize, SLK_NetS_EventList *pValue );
int struct_NetS_OfficialHireChange_send( char **pptr, int *psize, SLK_NetS_OfficialHireChange *pValue );
int struct_NetS_CityProtect_send( char **pptr, int *psize, SLK_NetS_CityProtect *pValue );
int struct_NetS_HeroWash_send( char **pptr, int *psize, SLK_NetS_HeroWash *pValue );
int struct_NetS_HeroColorup_send( char **pptr, int *psize, SLK_NetS_HeroColorup *pValue );
int struct_NetS_CityAttr_send( char **pptr, int *psize, SLK_NetS_CityAttr *pValue );
int struct_NetS_EquipWash_send( char **pptr, int *psize, SLK_NetS_EquipWash *pValue );
int struct_NetS_MaterialInfo_send( char **pptr, int *psize, SLK_NetS_MaterialInfo *pValue );
int struct_NetS_MaterialList_send( char **pptr, int *psize, SLK_NetS_MaterialList *pValue );
int struct_NetS_MaterialWillInfo_send( char **pptr, int *psize, SLK_NetS_MaterialWillInfo *pValue );
int struct_NetS_MaterialWillList_send( char **pptr, int *psize, SLK_NetS_MaterialWillList *pValue );
int struct_NetS_StoryList_send( char **pptr, int *psize, SLK_NetS_StoryList *pValue );
int struct_NetS_MonsterInfo_send( char **pptr, int *psize, SLK_NetS_MonsterInfo *pValue );
int struct_NetS_StoryRank_send( char **pptr, int *psize, SLK_NetS_StoryRank *pValue );
int struct_NetS_StoryState_send( char **pptr, int *psize, SLK_NetS_StoryState *pValue );
int struct_NetS_StoryRanknum_send( char **pptr, int *psize, SLK_NetS_StoryRanknum *pValue );
int struct_NetS_StoryRanktime_send( char **pptr, int *psize, SLK_NetS_StoryRanktime *pValue );
int struct_NetS_MapZoneChange_send( char **pptr, int *psize, SLK_NetS_MapZoneChange *pValue );
int struct_NetS_MapZoneUnitList_send( char **pptr, int *psize, SLK_NetS_MapZoneUnitList *pValue );
int struct_NetS_BattleInfo_send( char **pptr, int *psize, SLK_NetS_BattleInfo *pValue );
int struct_NetS_BattleList_send( char **pptr, int *psize, SLK_NetS_BattleList *pValue );
int struct_NetS_MapResInfo_send( char **pptr, int *psize, SLK_NetS_MapResInfo *pValue );
int struct_NetS_WeatherChange_send( char **pptr, int *psize, SLK_NetS_WeatherChange *pValue );
int struct_NetS_Mail_send( char **pptr, int *psize, SLK_NetS_Mail *pValue );
int struct_NetS_MailOpResult_send( char **pptr, int *psize, SLK_NetS_MailOpResult *pValue );
int struct_NetS_MailFight_send( char **pptr, int *psize, SLK_NetS_MailFight *pValue );
int struct_NetS_MailView_send( char **pptr, int *psize, SLK_NetS_MailView *pValue );
int struct_NetS_CItyHelp_send( char **pptr, int *psize, SLK_NetS_CItyHelp *pValue );
int struct_NetS_CItyHelpList_send( char **pptr, int *psize, SLK_NetS_CItyHelpList *pValue );
int struct_NetS_MapCItyHelpList_send( char **pptr, int *psize, SLK_NetS_MapCItyHelpList *pValue );
int struct_NetS_CityState_send( char **pptr, int *psize, SLK_NetS_CityState *pValue );
int struct_NetS_CityArmyGroup_send( char **pptr, int *psize, SLK_NetS_CityArmyGroup *pValue );
int struct_NetS_CityArmyGroupList_send( char **pptr, int *psize, SLK_NetS_CityArmyGroupList *pValue );
int struct_NetS_MapTownInfo_send( char **pptr, int *psize, SLK_NetS_MapTownInfo *pValue );
int struct_NetS_TownArmyGroup_send( char **pptr, int *psize, SLK_NetS_TownArmyGroup *pValue );
int struct_NetS_TownArmyGroupList_send( char **pptr, int *psize, SLK_NetS_TownArmyGroupList *pValue );
int struct_NetS_SystalkJson_send( char **pptr, int *psize, SLK_NetS_SystalkJson *pValue );
int struct_NetS_RollMsgJson_send( char **pptr, int *psize, SLK_NetS_RollMsgJson *pValue );
int struct_NetS_RollMsg_send( char **pptr, int *psize, SLK_NetS_RollMsg *pValue );
int struct_NetS_TownOwnerAsk_send( char **pptr, int *psize, SLK_NetS_TownOwnerAsk *pValue );
int struct_NetS_TownOwnerAskList_send( char **pptr, int *psize, SLK_NetS_TownOwnerAskList *pValue );
int struct_NetS_TownFight_send( char **pptr, int *psize, SLK_NetS_TownFight *pValue );
int struct_NetS_TownFightList_send( char **pptr, int *psize, SLK_NetS_TownFightList *pValue );
int struct_NetS_MapTownExInfo_send( char **pptr, int *psize, SLK_NetS_MapTownExInfo *pValue );
int struct_NetS_MapZoneTown_send( char **pptr, int *psize, SLK_NetS_MapZoneTown *pValue );
int struct_NetS_MapZoneTownList_send( char **pptr, int *psize, SLK_NetS_MapZoneTownList *pValue );
int struct_NetS_MapCenterTown_send( char **pptr, int *psize, SLK_NetS_MapCenterTown *pValue );
int struct_NetS_MapCenterTownList_send( char **pptr, int *psize, SLK_NetS_MapCenterTownList *pValue );
int struct_NetS_CityWarInfo_send( char **pptr, int *psize, SLK_NetS_CityWarInfo *pValue );
int struct_NetS_CityWarList_send( char **pptr, int *psize, SLK_NetS_CityWarList *pValue );
int struct_NetS_CityWarDel_send( char **pptr, int *psize, SLK_NetS_CityWarDel *pValue );
int struct_NetS_WorldQuest_send( char **pptr, int *psize, SLK_NetS_WorldQuest *pValue );
int struct_NetS_WorldBoss_send( char **pptr, int *psize, SLK_NetS_WorldBoss *pValue );
int struct_NetS_LostRebuild_send( char **pptr, int *psize, SLK_NetS_LostRebuild *pValue );
int struct_NetS_KingwarTown_send( char **pptr, int *psize, SLK_NetS_KingwarTown *pValue );
int struct_NetS_KingwarTownList_send( char **pptr, int *psize, SLK_NetS_KingwarTownList *pValue );
int struct_NetS_KingWarActivity_send( char **pptr, int *psize, SLK_NetS_KingWarActivity *pValue );
int struct_NetS_KingWarNotify_send( char **pptr, int *psize, SLK_NetS_KingWarNotify *pValue );
int struct_NetS_KingWarNotifyList_send( char **pptr, int *psize, SLK_NetS_KingWarNotifyList *pValue );
int struct_NetS_KingWarRank_send( char **pptr, int *psize, SLK_NetS_KingWarRank *pValue );
int struct_NetS_KingWarRankList_send( char **pptr, int *psize, SLK_NetS_KingWarRankList *pValue );
int struct_NetS_KingWarPK_send( char **pptr, int *psize, SLK_NetS_KingWarPK *pValue );
int struct_NetS_KingWarPoint_send( char **pptr, int *psize, SLK_NetS_KingWarPoint *pValue );
int struct_NetS_TreasureActivity_send( char **pptr, int *psize, SLK_NetS_TreasureActivity *pValue );
int struct_NetS_TreasureHas_send( char **pptr, int *psize, SLK_NetS_TreasureHas *pValue );
int struct_NetS_GotoAsyn_send( char **pptr, int *psize, SLK_NetS_GotoAsyn *pValue );
int struct_NetS_TreasureActor_send( char **pptr, int *psize, SLK_NetS_TreasureActor *pValue );
int struct_NetS_TreasureActorList_send( char **pptr, int *psize, SLK_NetS_TreasureActorList *pValue );
int struct_NetS_ChangeAutoBuild_send( char **pptr, int *psize, SLK_NetS_ChangeAutoBuild *pValue );
int struct_NetS_HeroVisitInfo_send( char **pptr, int *psize, SLK_NetS_HeroVisitInfo *pValue );
int struct_NetS_NationInfo_send( char **pptr, int *psize, SLK_NetS_NationInfo *pValue );
int struct_NetS_HeroVisitAward_send( char **pptr, int *psize, SLK_NetS_HeroVisitAward *pValue );
int struct_NetS_FightPlay_send( char **pptr, int *psize, SLK_NetS_FightPlay *pValue );
int struct_NetS_StorySweepHero_send( char **pptr, int *psize, SLK_NetS_StorySweepHero *pValue );
int struct_NetS_StorySweepResult_send( char **pptr, int *psize, SLK_NetS_StorySweepResult *pValue );
int struct_NetS_QuestTalk_send( char **pptr, int *psize, SLK_NetS_QuestTalk *pValue );
int struct_NetS_RankInfo_send( char **pptr, int *psize, SLK_NetS_RankInfo *pValue );
int struct_NetS_RankList_send( char **pptr, int *psize, SLK_NetS_RankList *pValue );
int struct_NetS_FriendInfo_send( char **pptr, int *psize, SLK_NetS_FriendInfo *pValue );
int struct_NetS_FriendList_send( char **pptr, int *psize, SLK_NetS_FriendList *pValue );
int struct_NetS_ActorSearch_send( char **pptr, int *psize, SLK_NetS_ActorSearch *pValue );
int struct_NetS_BlackInfo_send( char **pptr, int *psize, SLK_NetS_BlackInfo *pValue );
int struct_NetS_BlackList_send( char **pptr, int *psize, SLK_NetS_BlackList *pValue );
int struct_NetS_BlackListID_send( char **pptr, int *psize, SLK_NetS_BlackListID *pValue );
int struct_NetS_NationEquip_send( char **pptr, int *psize, SLK_NetS_NationEquip *pValue );
int struct_NetS_ShopItem_send( char **pptr, int *psize, SLK_NetS_ShopItem *pValue );
int struct_NetS_ShopList_send( char **pptr, int *psize, SLK_NetS_ShopList *pValue );
int struct_NetS_PayStoreGoods_send( char **pptr, int *psize, SLK_NetS_PayStoreGoods *pValue );
int struct_NetS_PayStore_send( char **pptr, int *psize, SLK_NetS_PayStore *pValue );
int struct_NetS_GoodsInfo_send( char **pptr, int *psize, SLK_NetS_GoodsInfo *pValue );
int struct_NetS_PayStoreActivity_send( char **pptr, int *psize, SLK_NetS_PayStoreActivity *pValue );
int struct_NetS_PayOrder_send( char **pptr, int *psize, SLK_NetS_PayOrder *pValue );
int struct_NetS_VipShopItem_send( char **pptr, int *psize, SLK_NetS_VipShopItem *pValue );
int struct_NetS_VipShop_send( char **pptr, int *psize, SLK_NetS_VipShop *pValue );
int struct_NetS_VipBag_send( char **pptr, int *psize, SLK_NetS_VipBag *pValue );

#endif
