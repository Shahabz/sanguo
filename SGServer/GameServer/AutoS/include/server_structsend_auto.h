#ifndef __STRUCTSEND0_AUTO_H
#define __STRUCTSEND0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetS_Login {
	int m_result;	//����������-����ֵ
	short m_username_length;	//����������-�û�������
	char m_username[51];	//����������-�û���
	short m_token_length;	//����������-token����
	char m_token[256];	//����������-token
	short m_usertype;	//����������-�û�����
};
typedef struct _slk_NetS_Login SLK_NetS_Login;	//�������˷��͵�¼������ͻ���

struct _slk_ListInfo {
	int m_actorid;	//��ɫID
	char m_name[22];	//��ɫ����
	char m_nation;	//��ɫ����
	short m_level;	//��ɫ�ȼ�
	char m_offset;	//��ɫλ��
	char m_lockstat;	//����״̬
	int m_delete_stoptime;	//ɾ��ʱ��
	int m_lock_endtime;	//����ʱ��
};
typedef struct _slk_ListInfo SLK_ListInfo;	//������ɫ���б���Ϣ

struct _slk_NetS_List {
	char m_actor_num;	//����˷���-��ɫ����
	SLK_ListInfo m_listinfo[8];	//����˷���-��ɫ����
	char m_nation_award;	//����˷���-�ĸ������н���
};
typedef struct _slk_NetS_List SLK_NetS_List;	//�������˷��ͽ�ɫ�б�

struct _slk_NetS_Create {
	char m_result;	//����˷���-������ɫ�Ľ��
	int m_actorid;	//����˷���-������ɫ�ı��
	int m_createtime;	//����˷���-������ɫ��ʱ��
};
typedef struct _slk_NetS_Create SLK_NetS_Create;	//���������ش�����ɫ���

struct _slk_NetS_EnterInfo {
	int m_actorid;	//����˷���-������Ϸ��ɫID
	int m_data_int;	//����˷���-������Ϸ��������[����]
	short m_data_short;	//����˷���-������Ϸ��������[����]
	char m_data_char;	//����˷���-������Ϸ��������[����]
	short m_serverid;	//����˷���-����ķ�����ID
	int m_servertime;	//����˷���-����ķ�����ʱ���
	int m_createtime;	//����˷���-������ɫʱ��
};
typedef struct _slk_NetS_EnterInfo SLK_NetS_EnterInfo;	//���������ؽ�����Ϸ�����Ϣ

struct _slk_NetS_Delete {
	int m_result;	//����������-ɾ������ֵ
	int m_actorid;	//����������-ɾ���Ľ�ɫ���
};
typedef struct _slk_NetS_Delete SLK_NetS_Delete;	//����������ɾ����ɫ

struct _slk_NetS_Heart {
	int m_value;	//����-��ֵ
};
typedef struct _slk_NetS_Heart SLK_NetS_Heart;	//����

struct _slk_NetS_Notify {
	short m_msgid;	//����������-����ϢID
	short m_valuenum;	//����������-����Ϣ��������
	int m_value[10];	//����������-����Ϣ��ֵ����
	short m_msg_length;	//����������-����Ϣ�ַ�����Ϣ����
	char m_msg[1024];	//����������-����Ϣ�ַ�������
};
typedef struct _slk_NetS_Notify SLK_NetS_Notify;	//ͨ�ö���Ϣ

struct _slk_NetS_DialogUpdate {
	char m_ui;	//����������-��ui��Ҫ����
};
typedef struct _slk_NetS_DialogUpdate SLK_NetS_DialogUpdate;	//��ui��Ҫ����

struct _slk_NetS_OfficialHire {
	short m_ofkind;	//��Ӷ��
	int m_ofsec;	//��Ӷ��
	int m_offree;	//��Ӷ��
	int m_ofquick;	//��Ӷ��
};
typedef struct _slk_NetS_OfficialHire SLK_NetS_OfficialHire;	//��Ӷ��

struct _slk_NetS_Building {
	char m_kind;	//��ͨ����-����
	char m_offset;	//��ͨ����-λ��
	char m_level;	//��ͨ����-�ȼ�
	int m_sec;	//��ͨ����-����ʣ��ʱ��
	int m_needsec;	//��ͨ����-������Ҫʱ��
	char m_quick;	//��ͨ����-�Ƿ��м���(�Ƽ���)
	int m_overvalue;	//��ͨ����-��ɺ��ֵ
	int m_value;	//��ͨ����-δ���ֵ
};
typedef struct _slk_NetS_Building SLK_NetS_Building;	//��ͨ������Ϣ

struct _slk_NetS_BuildingBarracks {
	char m_kind;	//��Ӫ����-����
	char m_offset;	//��Ӫ����-λ��
	char m_level;	//��Ӫ����-�ȼ�
	int m_sec;	//��Ӫ����-ļ��ʣ��ʱ��
	int m_needsec;	//��Ӫ����-ļ����Ҫʱ��
	char m_quick;	//��Ӫ����-�Ƿ��м���
	int m_overvalue;	//��Ӫ����-��ɺ������
};
typedef struct _slk_NetS_BuildingBarracks SLK_NetS_BuildingBarracks;	//��Ӫ������Ϣ

struct _slk_NetS_BuildingRes {
	char m_kind;	//��Դ����-����
	char m_offset;	//��Դ����-����(��Ӧָ���ؿ�)
	char m_level;	//��Դ����-�ȼ�
};
typedef struct _slk_NetS_BuildingRes SLK_NetS_BuildingRes;	//��Դ������Ϣ

struct _slk_NetS_BuildingList {
	short m_building_count;	//����������-��ͨ��������
	SLK_NetS_Building m_building[8];	//����������-��ͨ�����б�
	short m_barracks_count;	//����������-��Ӫ��������
	SLK_NetS_BuildingBarracks m_barracks[8];	//����������-��Ӫ�����б�
	short m_res_count;	//����������-��Դ��������
	SLK_NetS_BuildingRes m_res[64];	//����������-��Դ�����б�
	char m_levynum;	//����������-��ǰ�����մ���
	char m_worker_kind;	//����������-������������
	char m_worker_offset;	//���������ͽ�����������
	char m_worker_op;	//����������-�������н�������
	int m_worker_sec;	//����������-��������ʣ��ʱ��
	int m_worker_needsec;	//����������-����������Ҫʱ��
	char m_worker_free;	//����������-�����������ʱ��
	char m_worker_kind_ex;	//����������-������������(����)
	char m_worker_offset_ex;	//����������-������������(����)
	char m_worker_op_ex;	//����������-�������в���(����)
	int m_worker_sec_ex;	//����������-��������ʣ��ʱ��(����)
	int m_worker_needsec_ex;	//����������-����������Ҫʱ��(����)
	char m_worker_free_ex;	//����������-�����������ʱ��(����)
	int m_worker_expire_ex;	//����������-���ý�����е���ʱ��
	int m_function;	//����������-�����Ƿ���
	short m_forgingkind;	//����������-������
	int m_forgingsec;	//����������-������
	int m_forgingsec_need;	//����������-������
	int m_wnquick;	//����������-�����Ķ������
	int m_wnquick_ex;	//����������-�����Ķ������(����)
};
typedef struct _slk_NetS_BuildingList SLK_NetS_BuildingList;	//�����б�

struct _slk_NetS_ActorInfo {
	int m_actorid;	//������������һ�����Ϣ-��ɫ���
	char m_name[22];	//������������һ�����Ϣ-��ɫ����
	char m_nation;	//������������һ�����Ϣ-����
	char m_shape;	//������������һ�����Ϣ-����
	short m_level;	//������������һ�����Ϣ-����
	int m_exp;	//������������һ�����Ϣ-����
	int m_exp_max;	//������������һ�����Ϣ-��������
	int m_token;	//������������һ�����Ϣ-��ʯ
	char m_viplevel;	//������������һ�����Ϣ-VIP�ȼ�
	int m_vipexp;	//������������һ�����Ϣ-VIP����
	int m_vipexp_max;	//������������һ�����Ϣ-VIP��������
	short m_body;	//������������һ�����Ϣ-����
	char m_place;	//������������һ�����Ϣ-��λ
	char m_official;	//������������һ�����Ϣ-��ְ
	short m_zone;	//������������һ�����Ϣ-����
	int m_battlepower;	//������������һ�����Ϣ-ս��
	int m_silver;	//������������һ�����Ϣ-����
	int m_wood;	//������������һ�����Ϣ-ľ��
	int m_food;	//������������һ�����Ϣ-��ʳ
	int m_iron;	//������������һ�����Ϣ-����
	int m_infantry_num;	//������������һ�����Ϣ-������
	int m_cavalry_num;	//������������һ�����Ϣ-�����
	int m_archer_num;	//������������һ�����Ϣ-������
	short m_mokilllv;	//������������һ�����Ϣ-��ɱҰ����߼���
	int m_sflag;	//������������һ�����Ϣ-��־λ
	char m_autobuild;	//������������һ�����Ϣ-�Զ�����
	char m_autobuildopen;	//������������һ�����Ϣ-�Զ�����
	short m_techlevel[40];	//������������һ�����Ϣ-�Ƽ��ȼ�
	short m_techprogress[40];	//������������һ�����Ϣ-�Ƽ�����
	SLK_NetS_OfficialHire m_officialhire[3];	//������������һ�����Ϣ-��Ӷ��
	int m_ptsec;	//������������һ�����Ϣ-����ʱ��
	short m_forgingkind;	//������������һ�����Ϣ-����
	int m_forgingsec;	//������������һ�����Ϣ-����
	int m_actor_sflag;	//������������һ�����Ϣ-��ҽṹ��־
	short m_bodysec;	//������������һ�����Ϣ-�����ָ�ʱ��
	char m_game_day;	//������������һ�����Ϣ-��Ϸ��������
	char m_game_weather;	//������������һ�����Ϣ-��Ϸ��������
	char m_state;	//������������һ�����Ϣ-�ǳ�״̬
	char m_guardnum;	//������������һ�����Ϣ-��������
	short m_guardsec;	//������������һ�����Ϣ-��������
	int m_questid;	//������������һ�����Ϣ-��������id
	int m_permission;	//������������һ�����Ϣ-����Ȩ��
	int m_buff_endtime[6];	//������������һ�����Ϣ-buff
	char m_autoguard;	//������������һ�����Ϣ-�Ƿ�����
	char m_autoguardopen;	//������������һ�����Ϣ-�Ƿ�����
	int m_storyid;	//������������һ�����Ϣ-��������
	short m_posx;	//������������һ�����Ϣ-��������
	short m_posy;	//������������һ�����Ϣ-��������
	char m_open_zone_sili;	//������������һ�����Ϣ-˾������
	char m_open_zone_luoyang;	//������������һ�����Ϣ-��������
	char m_game_day_loop;	//������������һ�����Ϣ-����
	int m_shape_bag;	//������������һ�����Ϣ-ͷ�񱳰�
	short m_bufftrain;	//������������һ�����Ϣ-����ټ���
	char m_maidname_len;	//������������һ�����Ϣ-��Ů����
	char m_maidname[22];	//������������һ�����Ϣ-��Ů����
};
typedef struct _slk_NetS_ActorInfo SLK_NetS_ActorInfo;	//��ɫ������Ϣ

struct _slk_Corrdinate {
	short m_posx;	//����
	short m_posy;	//����
};
typedef struct _slk_Corrdinate SLK_Corrdinate;	//����

struct _slk_NetS_ItemUse {
	short m_itemoffset;	//��Ʒλ��
	short m_usenum;	//ʹ�ø�����װ��λ��
	int m_effres;	//Ӱ�죬һ��������Ч��ʾ
};
typedef struct _slk_NetS_ItemUse SLK_NetS_ItemUse;	//����ʹ��

struct _slk_NetS_ItemPut {
	short m_res_offset;	//��ƷԴ
	short m_res_num;	//��ƷԴ����
	short m_target_offset;	//��ƷĿ��
	short m_target_num;	//Ŀ�����
};
typedef struct _slk_NetS_ItemPut SLK_NetS_ItemPut;	//���߷���|ж��

struct _slk_NetS_ItemSettle {
	short m_itemnum;	//�������ĸ���
	short m_itemoffset[100];	//���õ���Ʒλ��
};
typedef struct _slk_NetS_ItemSettle SLK_NetS_ItemSettle;	//��������

struct _slk_NetS_LostItem {
	short m_itemoffset;	//��Ʒλ��
	short m_itemnum;	//����
	int m_targetid;	//����Ķ���id
	short m_path;	//;��
};
typedef struct _slk_NetS_LostItem SLK_NetS_LostItem;	//��ʧ����

struct _slk_NetS_GetItem {
	short m_itemoffset;	//��Ʒλ��
	short m_kind;	//��Ʒ����
	short m_type;	//��Ʒ����
	char m_color;	//��ɫ
	short m_num;	//����
	int m_targetid;	//����Ķ���id
	short m_path;	//;��
	unsigned char m_situation;	//ʹ��״̬
};
typedef struct _slk_NetS_GetItem SLK_NetS_GetItem;	//��õ���

struct _slk_ItemSmpInfo {
	short m_offset;	//��Ʒλ��
	short m_kind;	//��Ʒ����
	short m_num;	//����
	unsigned char m_situation;	//ʹ��״̬
	char m_color_level;	//��ɫ
};
typedef struct _slk_ItemSmpInfo SLK_ItemSmpInfo;	//���߼�����Ϣ

struct _slk_NetS_ItemList {
	char m_itemext;	//��չ�ĸ���
	short m_kindnum;	//��Ʒ����
	SLK_ItemSmpInfo m_item[300];	//��Ʒ
};
typedef struct _slk_NetS_ItemList SLK_NetS_ItemList;	//�����б�

struct _slk_ItemAttr {
	short m_type;	//��Ʒ������������
	short m_ability;	//��Ʒ����
	int m_value;	//��Ʒ����
	int m_addvalue;	//��Ʒ��������
};
typedef struct _slk_ItemAttr SLK_ItemAttr;	//��������

struct _slk_NetS_ItemInfo {
	short m_itemoffset;	//��Ʒλ��
	short m_itemkind;	//װ��kind,У��
	short m_type;	//��Ʒ����
	short m_level;	//һ��ȼ�
	char m_color_level;	//��ɫ�ּ�
	int m_price;	//�ۼ�
	short m_attr_num;	//���Ը���
	SLK_ItemAttr m_attr[10];	//����
};
typedef struct _slk_NetS_ItemInfo SLK_NetS_ItemInfo;	//������Ϣ

struct _slk_WalkPath {
	short m_pathx[100];	//·����ṹ
	short m_pathy[100];	//·����ṹ
	short m_len;	//·����ṹ
};
typedef struct _slk_WalkPath SLK_WalkPath;	//�ƶ�·��

struct _slk_NetS_ZoneUnit {
	short m_posx;	//��ͼ������ʾ��Ԫ
	short m_posy;	//��ͼ������ʾ��Ԫ
	char m_nation;	//��ͼ������ʾ��Ԫ
	unsigned char m_level;	//��ͼ������ʾ��Ԫ
	int m_zoneunit_index;	//��ӵ�ͼ������ʾ��Ԫ
};
typedef struct _slk_NetS_ZoneUnit SLK_NetS_ZoneUnit;	//������Ԫ

struct _slk_NetS_MapUnitAction {
	int m_unit_index;	//��ͼ������ʾ��Ԫ����
	char m_action;	//��ͼ������ʾ��Ԫ����
};
typedef struct _slk_NetS_MapUnitAction SLK_NetS_MapUnitAction;	//������Ԫ����

struct _slk_NetS_MapPosAction {
	short m_posx;	//��ͼ���궯��
	short m_posy;	//��ͼ���궯��
	char m_action;	//��ͼ���궯��
};
typedef struct _slk_NetS_MapPosAction SLK_NetS_MapPosAction;	//�������궯��

struct _slk_NetS_AddMapUnit {
	char m_type;	//��ͼ��Ԫ-����
	char m_state;	//��ͼ��Ԫ-״̬
	short m_posx;	//��ͼ��Ԫ-��ǰ����
	int m_unit_index;	//��ͼ��Ԫ-���
	short m_posy;	//��ͼ��Ԫ-��ǰ����
	char m_namelen;	//��ͼ��Ԫ-���Ƴ���
	char m_name[32];	//��ͼ��Ԫ-����
	char m_char_value_count;	//��ͼ��Ԫ-ͨ���ֶγ���
	char m_char_value[16];	//��ͼ��Ԫ-ͨ���ֶ�
	char m_short_value_count;	//��ͼ��Ԫ-ͨ���ֶγ���
	short m_short_value[16];	//��ͼ��Ԫ-ͨ���ֶ�
	char m_int_value_count;	//��ͼ��Ԫ-ͨ���ֶγ���
	int m_int_value[16];	//��ͼ��Ԫ-ͨ���ֶ�
	char m_prefixlen;	//��ͼ��Ԫ-�ƺų���
	char m_prefix[22];	//��ͼ��Ԫ-�ƺ�
};
typedef struct _slk_NetS_AddMapUnit SLK_NetS_AddMapUnit;	//��ʾ��Ԫ�����ͼ

struct _slk_NetS_DelMapUnit {
	int m_unit_index;	//��ͼ��Ԫ-���
};
typedef struct _slk_NetS_DelMapUnit SLK_NetS_DelMapUnit;	//��ʾ��Ԫ�뿪��ͼ

struct _slk_NetS_WorldMapInfo {
	char m_area_width;	//�����С��һ�����򼸸���ͼ���ӣ�
	char m_area_height;	//�����С��һ�����򼸸���ͼ���ӣ�
	short m_map_area_xnum;	//һ����ͼ�ж�������
	short m_map_area_ynum;	//һ����ͼ�ж�������
	short m_my_city_posx;	//�ҵĳǳ�λ��
	short m_my_city_posy;	//�ҵĳǳ�λ��
	int m_my_city_unit_index;	//�ҵĳǳص�ͼ��Ԫ-���
	char m_citystate;	//�ҵĳǳ�״̬
	short m_target_posx;	//Ŀ�ĵ�
	short m_target_posy;	//Ŀ�ĵ�
};
typedef struct _slk_NetS_WorldMapInfo SLK_NetS_WorldMapInfo;	//�����ͼ��Ϣ

struct _slk_NetS_MapUnitCorrdinate {
	int m_unit_index;	//��ͼ��Ԫ-���
	short m_posx;	//��ͼ��Ԫ-��ǰ����
	short m_posy;	//��ͼ��Ԫ-��ǰ����
};
typedef struct _slk_NetS_MapUnitCorrdinate SLK_NetS_MapUnitCorrdinate;	//��ͼ��ʾ��Ԫλ��

struct _slk_NetS_AddMarchRoute {
	char m_from_type;	//�о�·��-������Ԫ����
	short m_from_posx;	//�о�·��-����λ��
	short m_from_posy;	//�о�·��-����λ��
	char m_to_type;	//�о�·��-���ﵥԪ����
	short m_to_posx;	//�о�·��-����λ��
	short m_to_posy;	//�о�·��-����λ��
	char m_state;	//�о�·��-״̬
	int m_from_actorid;	//�о�·��-����
	char m_from_nation;	//�о�·��-����
	int m_to_actorid;	//�о�·��-Ŀ�����
	char m_to_nation;	//�о�·��-Ŀ�����
	int m_army_index;	//�о�·��-��������
	char m_action;	//�о�·��-��Ϊ
	char m_from_grid;	//�о�·��-����ռ�ظ���
	char m_to_grid;	//�о�·��-Ŀ��ռ�ظ���
};
typedef struct _slk_NetS_AddMarchRoute SLK_NetS_AddMarchRoute;	//�о�·��

struct _slk_NetS_DelMarchRoute {
	int m_army_index;	//ɾ����-��������
};
typedef struct _slk_NetS_DelMarchRoute SLK_NetS_DelMarchRoute;	//ɾ���о�·��

struct _slk_NetS_UpdateMapUnit {
	SLK_NetS_AddMapUnit m_info;	//���µ�ͼ��ʾ��Ԫ��Ϣ
};
typedef struct _slk_NetS_UpdateMapUnit SLK_NetS_UpdateMapUnit;	//���µ�ͼ��ʾ��Ԫ��Ϣ

struct _slk_NetS_ArmySpeedUpdate {
	int m_unit_index;	//���²����ٶ�
	char m_state;	//���²����ٶ�
	int m_statetime;	//���²����ٶ�
	int m_stateduration;	//���²����ٶ�
};
typedef struct _slk_NetS_ArmySpeedUpdate SLK_NetS_ArmySpeedUpdate;	//���Ӽ���

struct _slk_NetS_Equip {
	int m_offset;	//װ������
	short m_kind;	//װ������
	short m_washid[4];	//ϴ������
};
typedef struct _slk_NetS_Equip SLK_NetS_Equip;	//װ��

struct _slk_NetS_EquipList {
	short m_equipext;	//װ������չ
	short m_count;	//װ������
	SLK_NetS_Equip m_list[100];	//װ��
};
typedef struct _slk_NetS_EquipList SLK_NetS_EquipList;	//װ���б�

struct _slk_NetS_EquipGet {
	int m_offset;	//���һ��װ��
	short m_kind;	//���һ��װ��
	short m_path;	//���һ��װ��
	short m_washid[4];	//���һ��װ��
};
typedef struct _slk_NetS_EquipGet SLK_NetS_EquipGet;	//���һ��װ��

struct _slk_NetS_EquipLost {
	int m_offset;	//����һ��װ��
	short m_kind;	//����һ��װ��
	short m_path;	//����һ��װ��
};
typedef struct _slk_NetS_EquipLost SLK_NetS_EquipLost;	//����һ��װ��

struct _slk_NetS_HeroEquip {
	short m_herokind;	//Ӣ��װ��
	short m_count;	//Ӣ��װ��
	SLK_NetS_Equip m_list[6];	//Ӣ��װ��
};
typedef struct _slk_NetS_HeroEquip SLK_NetS_HeroEquip;	//Ӣ��װ��

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
	char m_god;	//
	char m_girlkind;	//
	char m_sonnum;	//
	int m_sontime;	//
};
typedef struct _slk_NetS_Hero SLK_NetS_Hero;	//Ӣ����Ϣ

struct _slk_NetS_HeroList {
	short m_count;	//Ӣ�۷�������
	SLK_NetS_Hero m_list[30];	//Ӣ���б�
	char m_type;	//0����Ӣ��1����Ӣ��
};
typedef struct _slk_NetS_HeroList SLK_NetS_HeroList;	//Ӣ���б�

struct _slk_NetS_HeroExp {
	short m_kind;	//Ӣ������
	int m_exp;	//Ӣ�۵�ǰ����
	int m_exp_max;	//Ӣ�۾�������
	int m_add;	//Ӣ�ۻ�ȡ����
	char m_isup;	//�Ƿ�����
	short m_level;	//Ӣ�۵ȼ�
	short m_path;	//;��
};
typedef struct _slk_NetS_HeroExp SLK_NetS_HeroExp;	//Ӣ�۾���ı�

struct _slk_NetS_HeroSoldiers {
	short m_kind;	//Ӣ������
	int m_add;	//��ӱ�����
	int m_soldiers;	//��ǰ����
	int m_soldiers_max;	//��������
	short m_path;	//;��
};
typedef struct _slk_NetS_HeroSoldiers SLK_NetS_HeroSoldiers;	//Ӣ�۱���

struct _slk_NetS_HeroState {
	short m_kind;	//Ӣ������
	char m_state;	//Ӣ��״̬
};
typedef struct _slk_NetS_HeroState SLK_NetS_HeroState;	//Ӣ��״̬

struct _slk_NetS_HeroReplace {
	short m_up_kind;	//�����佫
	short m_down_kind;	//�����佫
	short m_up_offset;	//
	short m_down_offset;	//
};
typedef struct _slk_NetS_HeroReplace SLK_NetS_HeroReplace;	//Ӣ���������滻

struct _slk_NetS_HeroGet {
	short m_kind;	//��ȡ���佫
	short m_path;	//;��
	SLK_NetS_Hero m_hero;	//��ȡ��Ӣ����Ϣ
	short m_itemnum;	//ת���ĵ�������
};
typedef struct _slk_NetS_HeroGet SLK_NetS_HeroGet;	//Ӣ�ۻ�ȡ

struct _slk_NetS_AwardInfo {
	int m_kind;	//����
	int m_num;	//����
};
typedef struct _slk_NetS_AwardInfo SLK_NetS_AwardInfo;	//������Ϣ

struct _slk_NetS_AwardInfoList {
	short m_count;	//��������
	SLK_NetS_AwardInfo m_list[32];	//�����б�
	short m_callback_code;	//�ĸ�ϵͳҪ��ʾ���ش�
	int m_value;	//����ֵ
};
typedef struct _slk_NetS_AwardInfoList SLK_NetS_AwardInfoList;	//������Ϣ�б�

struct _slk_NetS_Experience {
	int m_addexp;	//����������-��ȡ����
	int m_curexp;	//����������-��ȡ����
	char m_isup;	//����������-��ȡ����
	short m_path;	//����������-��ȡ����
	short m_level;	//����������-��ȡ����
	int m_expmax;	//����������-��ȡ����
};
typedef struct _slk_NetS_Experience SLK_NetS_Experience;	//�ı侭��

struct _slk_NetS_Body {
	short m_total;	//����
	short m_add;	//��ȡ����
	short m_path;	//;��
	short m_bodysec;	//�����ظ�ʱ��
};
typedef struct _slk_NetS_Body SLK_NetS_Body;	//�ı�����

struct _slk_NetS_Levy {
	short m_total;	//�ı����մ���
	short m_add;	//�ı����մ���
	short m_max;	//�ı����մ���
	int m_sec;	//�ı����մ���
	short m_path;	//�ı����մ���
};
typedef struct _slk_NetS_Levy SLK_NetS_Levy;	//�ı����մ���

struct _slk_NetS_Silver {
	int m_total;	//�ı�����
	int m_add;	//�ı�����
	short m_path;	//�ı�����
};
typedef struct _slk_NetS_Silver SLK_NetS_Silver;	//�ı�����

struct _slk_NetS_Wood {
	int m_total;	//�ı�ľ��
	int m_add;	//�ı�ľ��
	short m_path;	//�ı�ľ��
};
typedef struct _slk_NetS_Wood SLK_NetS_Wood;	//�ı�ľ��

struct _slk_NetS_Food {
	int m_total;	//�ı���ʳ
	int m_add;	//�ı���ʳ
	short m_path;	//�ı���ʳ
};
typedef struct _slk_NetS_Food SLK_NetS_Food;	//�ı���ʳ

struct _slk_NetS_Iron {
	int m_total;	//�ı�����
	int m_add;	//�ı�����
	short m_path;	//�ı�����
};
typedef struct _slk_NetS_Iron SLK_NetS_Iron;	//�ı�����

struct _slk_NetS_People {
	int m_total;	//�ı��˿�
	int m_add;	//�ı��˿�
	short m_path;	//�ı��˿�
};
typedef struct _slk_NetS_People SLK_NetS_People;	//�ı��˿�

struct _slk_NetS_Prestige {
	int m_total;	//�ı�����ֵ
	int m_add;	//�ı�����ֵ
	short m_path;	//�ı�����ֵ
};
typedef struct _slk_NetS_Prestige SLK_NetS_Prestige;	//�ı�����ֵ

struct _slk_NetS_Friendship {
	int m_total;	//�ı��������
	int m_add;	//�ı��������
	short m_path;	//�ı��������
};
typedef struct _slk_NetS_Friendship SLK_NetS_Friendship;	//�ı��������

struct _slk_NetS_Vip {
	int m_addexp;	//�ı�VIP����
	int m_curexp;	//�ı�VIP����
	int m_expmax;	//�ı�VIP����
	char m_isup;	//�ı�VIP����
	char m_level;	//�ı�VIP����
	short m_path;	//�ı�VIP����
};
typedef struct _slk_NetS_Vip SLK_NetS_Vip;	//�ı�VIP����

struct _slk_NetS_Token {
	int m_total;	//�ı���ʯ
	int m_add;	//�ı���ʯ
	short m_path;	//�ı���ʯ
};
typedef struct _slk_NetS_Token SLK_NetS_Token;	//�ı���ʯ

struct _slk_NetS_BuildingUpgradeInfo {
	short m_citylevel;	//������������ٸ��ȼ�
	short m_actorlevel;	//���������������ǵȼ�
	int m_silver;	//����������������
	int m_wood;	//����ľ�Ľ�������
	int m_food;	//������������ʳ��
	int m_iron;	//����������������
	int m_sec;	//������������ʱ��
	char m_maxlevel;	//�������ȼ�
};
typedef struct _slk_NetS_BuildingUpgradeInfo SLK_NetS_BuildingUpgradeInfo;	//����������Ϣ

struct _slk_NetS_Worker {
	char m_worker_kind;	//����������-������������
	char m_worker_offset;	//���������ͽ�����������
	char m_worker_op;	//����������-�������н�������
	int m_worker_sec;	//����������-��������ʣ��ʱ��
	int m_worker_needsec;	//����������-����������Ҫʱ��
	char m_worker_free;	//����������-�����������ʱ��
	char m_worker_kind_ex;	//����������-������������(����)
	char m_worker_offset_ex;	//����������-������������(����)
	char m_worker_op_ex;	//����������-�������в���(����)
	int m_worker_sec_ex;	//����������-��������ʣ��ʱ��(����)
	int m_worker_needsec_ex;	//����������-����������Ҫʱ��(����)
	char m_worker_free_ex;	//����������-�����������ʱ��(����)
	int m_worker_expire_ex;	//����������-���ý�����е���ʱ��
	int m_wnquick;	//����������-�����Ķ������
	int m_wnquick_ex;	//����������-�����Ķ������(����)
};
typedef struct _slk_NetS_Worker SLK_NetS_Worker;	//����������Ϣ

struct _slk_NetS_BuildingGet {
	short m_path;	//��ȡ����
	SLK_NetS_Building m_building;	//��ȡ����
};
typedef struct _slk_NetS_BuildingGet SLK_NetS_BuildingGet;	//��ͨ������ȡ

struct _slk_NetS_BuildingBarracksGet {
	short m_path;	//��ȡ����
	SLK_NetS_BuildingBarracks m_barracks;	//��ȡ����
};
typedef struct _slk_NetS_BuildingBarracksGet SLK_NetS_BuildingBarracksGet;	//��Ӫ������ȡ

struct _slk_NetS_BuildingResGet {
	short m_path;	//��ȡ����
	SLK_NetS_BuildingRes m_res;	//��ȡ����
};
typedef struct _slk_NetS_BuildingResGet SLK_NetS_BuildingResGet;	//��Դ������ȡ

struct _slk_NetS_Soldiers {
	char m_corps;	//���ڱ����ı�
	int m_soldiers;	//���ڱ����ı�
	int m_add;	//���ڱ����ı�
	short m_path;	//���ڱ����ı�
};
typedef struct _slk_NetS_Soldiers SLK_NetS_Soldiers;	//�����仯

struct _slk_NetS_TrainInfo {
	int m_soldiers;	//��Ӫ����
	int m_soldiers_max;	//��Ӫ����
	int m_trainnum;	//��ǰѵ��ʿ����
	int m_trainsec;	//��ǰѵ��ʣ��ʱ��(��)
	int m_trainsec_need;	//��ǰѵ����Ҫʱ��(��)
	int m_queuenum[16];	//����
	char m_queue;	//����
	char m_trainlong;	//ļ����ʱ
	int m_train_confnum;	//ÿ�����ѵ��ʿ����
	int m_train_confsec;	//��ļ���ʱ��ʱ��
	int m_train_conffood;	//��������
};
typedef struct _slk_NetS_TrainInfo SLK_NetS_TrainInfo;	//ѵ��ʿ��

struct _slk_NetS_Quest {
	short m_questid;	//���
	char m_flag;	//��ɱ��
	char m_datatype;	//��������
	short m_datakind;	//��������
	short m_dataoffset;	//������
	int m_value;	//ֵ
	int m_needvalue;	//��Ҫֵ
	int m_awardkind[5];	//����
	int m_awardnum[5];	//����
	int m_nameid;	//����ID
	int m_descid;	//����ID
};
typedef struct _slk_NetS_Quest SLK_NetS_Quest;	//����

struct _slk_NetS_QuestList {
	short m_count;	//�����б�
	SLK_NetS_Quest m_list[16];	//�����б�
};
typedef struct _slk_NetS_QuestList SLK_NetS_QuestList;	//�����б�

struct _slk_NetS_QuestAward {
	short m_questid;	//������
	char m_count;	//��������
	SLK_NetS_AwardInfo m_list[5];	//������
	char m_datatype;	//��������
	short m_datakind;	//��������
	char m_dataoffset;	//������
	int m_value;	//ֵ
	int m_needvalue;	//��Ҫֵ
	int m_nameid;	//����ID
	char m_type;	//����
};
typedef struct _slk_NetS_QuestAward SLK_NetS_QuestAward;	//������

struct _slk_NetS_Function {
	int m_function;	//�����Ĺ����б�
	char m_openoffset;	//�����Ĺ����б�
	short m_path;	//;��
};
typedef struct _slk_NetS_Function SLK_NetS_Function;	//�����Ĺ����б�

struct _slk_NetS_CityGuard {
	char m_corps;	//��������
	char m_color;	//������ɫ
	char m_shape;	//��������
	short m_level;	//�����ȼ�
	int m_soldiers;	//��������
	int m_troops;	//��������
	char m_offset;	//��������
};
typedef struct _slk_NetS_CityGuard SLK_NetS_CityGuard;	//����

struct _slk_NetS_CityGuardList {
	short m_count;	//��������
	SLK_NetS_CityGuard m_list[32];	//�����б�
	short m_guardsec;	//�����ٻ���ȴ
};
typedef struct _slk_NetS_CityGuardList SLK_NetS_CityGuardList;	//�����б�

struct _slk_NetS_CityGuardSec {
	short m_guardsec;	//������ȴ���
};
typedef struct _slk_NetS_CityGuardSec SLK_NetS_CityGuardSec;	//������ȴ����

struct _slk_NetS_BuildingSmithy {
	short m_forgingkind;	//����������-������
	int m_forgingsec;	//����������-������
	int m_forgingsec_need;	//����������-������
};
typedef struct _slk_NetS_BuildingSmithy SLK_NetS_BuildingSmithy;	//������

struct _slk_NetS_ChangeName {
	short m_name_length;	//�޸�����
	char m_name[32];	//�޸�����
	char m_type;	//�޸�����
};
typedef struct _slk_NetS_ChangeName SLK_NetS_ChangeName;	//����

struct _slk_NetS_BuildingAction {
	short m_kind;	//��������
	short m_offset;	//��������
	short m_action;	//ִ�ж���
};
typedef struct _slk_NetS_BuildingAction SLK_NetS_BuildingAction;	//��������

struct _slk_NetS_LevyInfo {
	int m_base[4];	//����
	int m_tech[4];	//�Ƽ��ӳ�
	int m_weather[4];	//�����ӳ�
	int m_activity[4];	//��ӳ�
	int m_offical[4];	//��ְ�ӳ�
	int m_sec;	//��ȴ����ʱ
};
typedef struct _slk_NetS_LevyInfo SLK_NetS_LevyInfo;	//������Ϣ

struct _slk_NetS_Chat {
	int m_actorid;	//������Ϣ-��ɫid
	char m_shape;	//������Ϣ-����
	short m_level;	//������Ϣ-�ȼ�
	char m_namelen;	//������Ϣ-����
	char m_name[32];	//������Ϣ-����
	char m_frame;	//������Ϣ-����
	char m_zone;	//������Ϣ-����
	char m_place;	//������Ϣ-ְλ
	short m_msglen;	//������Ϣ-��Ϣ
	char m_msg[128];	//������Ϣ-��Ϣ
	int m_optime;	//������Ϣ-ʱ��
	char m_channel;	//������Ϣ-Ƶ��
	char m_nation;	//������Ϣ-����
	char m_msgtype;	//������Ϣ-��Ϣ����
};
typedef struct _slk_NetS_Chat SLK_NetS_Chat;	//������Ϣ

struct _slk_NetS_ChatList {
	char m_count;	//�����б�
	SLK_NetS_Chat m_list[10];	//�����б�
	char m_channel;	//�����б�
};
typedef struct _slk_NetS_ChatList SLK_NetS_ChatList;	//������Ϣ�б�

struct _slk_NetS_SystalkidValue {
	char m_vlen;	//ϵͳ��Ϣƴ�Ӵ�����
	char m_v[64];	//ϵͳ��Ϣƴ�Ӵ�
};
typedef struct _slk_NetS_SystalkidValue SLK_NetS_SystalkidValue;	//���ֱ�ϵͳ��Ϣ

struct _slk_NetS_Systalkid {
	char m_count;	//ϵͳ��Ϣ
	SLK_NetS_SystalkidValue m_msglist[8];	//ϵͳ��Ϣ
	int m_textid;	//���ֱ�ID
	int m_optime;	//ʱ��
};
typedef struct _slk_NetS_Systalkid SLK_NetS_Systalkid;	//���ֱ�ϵͳ��Ϣ

struct _slk_NetS_Systalk {
	short m_msglen;	//ֱ�����ֵ�ϵͳ��Ϣ
	char m_msg[1024];	//ֱ�����ֵ�ϵͳ��Ϣ
	int m_optime;	//ʱ��
	char m_roll;	//�Ƿ��й���
};
typedef struct _slk_NetS_Systalk SLK_NetS_Systalk;	//ֱ������ϵͳ��Ϣ

struct _slk_NetS_Battlepower {
	int m_total;	//�ı�ս��
	short m_path;	//�ı�ս��
};
typedef struct _slk_NetS_Battlepower SLK_NetS_Battlepower;	//�ı�ս��

struct _slk_NetS_TechChange {
	char m_kind;	//�Ƽ��仯
	char m_level;	//�Ƽ��仯
	char m_progress;	//�Ƽ��仯
};
typedef struct _slk_NetS_TechChange SLK_NetS_TechChange;	//�Ƽ��仯

struct _slk_NetS_CityEvent {
	char m_type;	//�����¼�
	short m_kind;	//�����¼�
	int m_value;	//�����¼�
	int m_optime;	//�����¼�
};
typedef struct _slk_NetS_CityEvent SLK_NetS_CityEvent;	//�ǳ��¼�

struct _slk_NetS_BattleEvent {
	char m_type;	//�����¼�
	char m_name[22];	//�����¼�
	char m_value;	//�����¼�
	int m_optime;	//�����¼�
	i64 m_mailid;	//�����¼�
};
typedef struct _slk_NetS_BattleEvent SLK_NetS_BattleEvent;	//�����¼�

struct _slk_NetS_EventList {
	short m_cevent_count;	//�¼��б�
	SLK_NetS_CityEvent m_cevent_list[4];	//�¼��б�
	short m_bevent_count;	//�¼��б�
	SLK_NetS_BattleEvent m_bevent_list[4];	//�¼��б�
};
typedef struct _slk_NetS_EventList SLK_NetS_EventList;	//�����¼��б�

struct _slk_NetS_OfficialHireChange {
	char m_type;	//��Ӷ����Ϣ�仯
	SLK_NetS_OfficialHire m_info;	//��Ӷ����Ϣ�仯
};
typedef struct _slk_NetS_OfficialHireChange SLK_NetS_OfficialHireChange;	//��Ա�仯

struct _slk_NetS_CityProtect {
	int m_total;	//�ı䱣��ʱ��
	int m_add;	//�ı䱣��ʱ��
	short m_path;	//�ı䱣��ʱ��
};
typedef struct _slk_NetS_CityProtect SLK_NetS_CityProtect;	//�ǳر���ʱ��

struct _slk_NetS_HeroWash {
	char m_hero_washnum;	//ϴ�����
	int m_hero_washsec;	//ϴ��ʱ��
};
typedef struct _slk_NetS_HeroWash SLK_NetS_HeroWash;	//�佫ϴ��

struct _slk_NetS_HeroColorup {
	int m_value;	//ͻ��ֵ
	char m_isup;	//ͻ�Ƴɹ�
};
typedef struct _slk_NetS_HeroColorup SLK_NetS_HeroColorup;	//�佫ͻ��

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
};
typedef struct _slk_NetS_CityAttr SLK_NetS_CityAttr;	//�ǳ�����

struct _slk_NetS_EquipWash {
	char m_equip_washnum;	//װ��ϴ��
	int m_equip_washsec;	//װ��ϴ��
};
typedef struct _slk_NetS_EquipWash SLK_NetS_EquipWash;	//װ��ϴ��

struct _slk_NetS_MaterialInfo {
	char m_matkind;	//��������
	char m_matnum;	//��������
	int m_matsec;	//��������
	int m_matsec_need;	//��������
};
typedef struct _slk_NetS_MaterialInfo SLK_NetS_MaterialInfo;	//����������Ϣ

struct _slk_NetS_MaterialList {
	char m_count;	//��������
	SLK_NetS_MaterialInfo m_list[8];	//��������
	char m_matquenum;	//��������
	int m_nation_people;	//��������
	int m_city_people;	//��������
	int m_change_nation_people;	//��������
	int m_change_city_people;	//��������
};
typedef struct _slk_NetS_MaterialList SLK_NetS_MaterialList;	//���������б�

struct _slk_NetS_MaterialWillInfo {
	char m_matkind;	//Ԥ������
	int m_matsec;	//Ԥ������
	char m_matkind_will;	//Ԥ������
	char m_matnum_will;	//Ԥ������
};
typedef struct _slk_NetS_MaterialWillInfo SLK_NetS_MaterialWillInfo;	//Ԥ������������Ϣ

struct _slk_NetS_MaterialWillList {
	char m_count;	//Ԥ������
	SLK_NetS_MaterialWillInfo m_list[8];	//Ԥ������
};
typedef struct _slk_NetS_MaterialWillList SLK_NetS_MaterialWillList;	//Ԥ�����������б�

struct _slk_NetS_StoryList {
	short m_story_star[128];	//������Ϣ-����״̬
	short m_story_hero[32];	//������Ϣ-��ļ����
	int m_story_restime[32];	//������Ϣ-��Դ����ʱ��
	short m_story_resnum[32];	//������Ϣ-��Դ��������
	short m_story_resreset[32];	//������Ϣ-��Դ�������ô���
	short m_story_itemnum[64];	//������Ϣ-���߸����������
	short m_story_drawing[16];	//������Ϣ-װ��ͼֽ�����Ƿ���
	short m_sweep_herokind[4];	//������Ϣ-ɨ���佫kind
	short m_storyid;	//������Ϣ-��ҽ���
};
typedef struct _slk_NetS_StoryList SLK_NetS_StoryList;	//������Ϣ

struct _slk_NetS_MonsterInfo {
	int m_monsterid;	//������Ϣ
	short m_shape;	//������Ϣ
	short m_level;	//������Ϣ
	char m_color;	//������Ϣ
	char m_corps;	//������Ϣ
	int m_hp;	//������Ϣ
};
typedef struct _slk_NetS_MonsterInfo SLK_NetS_MonsterInfo;	//������Ϣ

struct _slk_NetS_StoryRank {
	char m_count;	//�����ؿ���Ϣ
	SLK_NetS_MonsterInfo m_list[16];	//�����ؿ���Ϣ
	int m_exp;	//�����ؿ���Ϣ
	char m_body;	//�����ؿ���Ϣ
	char m_type;	//�����ؿ���Ϣ
};
typedef struct _slk_NetS_StoryRank SLK_NetS_StoryRank;	//�����ؿ�

struct _slk_NetS_StoryState {
	int m_storyid;	//����id
	int m_state;	//����״̬
	short m_saveoffset;	//�浵����
	short m_savetype;	//����
	short m_actor_storyid;	//��Ҹ�������id
};
typedef struct _slk_NetS_StoryState SLK_NetS_StoryState;	//����״̬����

struct _slk_NetS_StoryRanknum {
	int m_storyid;	//����id
	short m_num;	//��������
	short m_saveoffset;	//�浵����
};
typedef struct _slk_NetS_StoryRanknum SLK_NetS_StoryRanknum;	//�����ؿ���������

struct _slk_NetS_StoryRanktime {
	int m_storyid;	//����id
	int m_time;	//����ʱ��
	short m_saveoffset;	//�浵����
};
typedef struct _slk_NetS_StoryRanktime SLK_NetS_StoryRanktime;	//�����ؿ�ʱ�����

struct _slk_NetS_MapZoneChange {
	char m_zoneid;	//����id�任
	char m_open;	//����id�Ƿ���
	char m_nation;	//��������
};
typedef struct _slk_NetS_MapZoneChange SLK_NetS_MapZoneChange;	//��ͼ�����л�

struct _slk_NetS_MapZoneUnitList {
	short m_count;	//��ͼ������Ԫ����
	SLK_NetS_ZoneUnit m_list[256];	//��ͼ������Ԫ�б�
	char m_zoneid;	//��ͼ����id
};
typedef struct _slk_NetS_MapZoneUnitList SLK_NetS_MapZoneUnitList;	//��ͼ������Ԫ�б�

struct _slk_NetS_BattleInfo {
	int m_army_index;	//����������Ϣ
	int m_unit_index;	//����������Ϣ
	char m_state;	//����������Ϣ
	int m_statetime;	//����������Ϣ
	int m_stateduration;	//����������Ϣ
	char m_action;	//����������Ϣ
	short m_to_posx;	//����������Ϣ
	short m_to_posy;	//����������Ϣ
	short m_herokind[4];	//����������Ϣ
	char m_to_type;	//����������Ϣ
};
typedef struct _slk_NetS_BattleInfo SLK_NetS_BattleInfo;	//������Ϣ

struct _slk_NetS_BattleList {
	short m_count;	//���������б�
	SLK_NetS_BattleInfo m_list[8];	//���������б�
	int m_unit_index;	//���������б�
};
typedef struct _slk_NetS_BattleList SLK_NetS_BattleList;	//���������б�

struct _slk_NetS_MapResInfo {
	short m_kind;	//�ɼ�Ŀ��kind
	int m_totalnum;	//��Դ������
	int m_totalsec;	//��Դ���ܲɼ�ʱ��
	int m_spacenum;	//�ɼ�ʣ����
	short m_herokind;	//�ɼ��佫
	short m_herolevel;	//�ɼ��佫�ȼ�
	int m_herohp;	//�ɼ��佫����
	char m_herocolor;	//�ɼ��佫��ɫ
	short m_actorlevel;	//�ɼ���ҵȼ�
};
typedef struct _slk_NetS_MapResInfo SLK_NetS_MapResInfo;	//��Դ������

struct _slk_NetS_WeatherChange {
	char m_game_day;	//����ϵͳ
	char m_game_weather;	//����ϵͳ
	char m_game_day_loop;	//����ϵͳ
};
typedef struct _slk_NetS_WeatherChange SLK_NetS_WeatherChange;	//�����ı�

struct _slk_NetS_Mail {
	i64 m_mailid;	//�ʼ�-id
	char m_type;	//�ʼ�-����
	short m_title_len;	//�ʼ�
	char m_title[128];	//�ʼ�����
	short m_content_len;	//�ʼ�
	char m_content[1024];	//�ʼ�-����
	short m_attach_len;	//�ʼ�
	char m_attach[256];	//�ʼ�-����
	char m_attachget;	//�ʼ�-������ȡ
	char m_read;	//�ʼ�-�Ƿ��ȡ
	int m_recvtime;	//�ʼ�-����ʱ��
	i64 m_fightid;	//�ʼ�-����ս��
	char m_lock;	//�ʼ�-����״̬
	int m_actorid;	//�ʼ�-�������
	char m_viewpath;	//�ʼ�-�鿴;��
};
typedef struct _slk_NetS_Mail SLK_NetS_Mail;	//һ���ʼ�

struct _slk_NetS_MailOpResult {
	char m_op;	//�ʼ��������ؽ��
	i64 m_mailid;	//�ʼ��������ؽ��
};
typedef struct _slk_NetS_MailOpResult SLK_NetS_MailOpResult;	//�ʼ��������

struct _slk_NetS_MailFight {
	char m_flag;	//ս���ʼ�����
	short m_content_length;	//ս���ʼ�����
	char m_content[1800];	//ս���ʼ�����
	i64 m_mailid;	//ս���ʼ�����
};
typedef struct _slk_NetS_MailFight SLK_NetS_MailFight;	//ս���ʼ�����

struct _slk_NetS_MailView {
	SLK_NetS_Mail m_mail;	//�ʼ��鿴
};
typedef struct _slk_NetS_MailView SLK_NetS_MailView;	//�ʼ��鿴

struct _slk_NetS_CItyHelp {
	short m_level;	//פ���б�-�ȼ�
	char m_actorname_length;	//פ���б�-�����
	char m_actorname[32];	//פ���б�-�����
	short m_herokind;	//פ���б�-�佫
	int m_soldiers;	//פ���б�-����
	int m_actorid;	//פ���б�-��ɫid
	int m_army_index;	//פ���б�-��������
};
typedef struct _slk_NetS_CItyHelp SLK_NetS_CItyHelp;	//פ��

struct _slk_NetS_CItyHelpList {
	short m_count;	//פ���б�
	SLK_NetS_CItyHelp m_list[32];	//פ���б�
	short m_walllevel;	//פ���б�
};
typedef struct _slk_NetS_CItyHelpList SLK_NetS_CItyHelpList;	//פ���б�

struct _slk_NetS_MapCItyHelpList {
	short m_count;	//��ͼ�鿴פ���б�
	SLK_NetS_CItyHelp m_list[32];	//��ͼ�鿴פ���б�
	short m_walllevel;	//��ͼ�鿴פ���б�
};
typedef struct _slk_NetS_MapCItyHelpList SLK_NetS_MapCItyHelpList;	//��ͼ�鿴פ���б�

struct _slk_NetS_CityState {
	char m_state;	//�ǳص�ǰ״̬
	char m_change;	//�ǳص�ǰ״̬
};
typedef struct _slk_NetS_CityState SLK_NetS_CityState;	//�ǳ�״̬

struct _slk_NetS_CityArmyGroup {
	int m_group_index;	//��ս��Ϣ-��������
	int m_group_id;	//��ս��Ϣ-����id
	char m_attack;	//��ս��Ϣ-1����2����
	int m_statetime;	//��ս��Ϣ-����ʱ
	int m_stateduration;	//��ս��Ϣ-����ʱ
	char m_nation;	//��ս��Ϣ
	char m_t_nation;	//��ս��Ϣ
	short m_level;	//��ս��Ϣ
	short m_t_level;	//��ս��Ϣ
	char m_name_length;	//��ս��Ϣ
	char m_name[32];	//��ս��Ϣ
	char m_t_name_length;	//��ս��Ϣ
	char m_t_name[32];	//��ս��Ϣ
	short m_posx;	//��ս��Ϣ
	short m_posy;	//��ս��Ϣ
	short m_t_posx;	//��ս��Ϣ
	short m_t_posy;	//��ս��Ϣ
	int m_actorid;	//��ս��Ϣ
	int m_t_actorid;	//��ս��Ϣ
	int m_total;	//��ս��Ϣ
	int m_t_total;	//��ս��Ϣ
	char m_type;	//��ս��Ϣ
};
typedef struct _slk_NetS_CityArmyGroup SLK_NetS_CityArmyGroup;	//��ս��Ϣ

struct _slk_NetS_CityArmyGroupList {
	short m_count;	//��ս��Ϣ�б�
	SLK_NetS_CityArmyGroup m_list[16];	//��ս��Ϣ�б�
	char m_nation;	//��ս��Ϣ�б�
	int m_unit_index;	//��ս��Ϣ�б�
	short m_totalcount;	//��ս��Ϣ�б�
	char m_flag;	//��ս��Ϣ�б�
};
typedef struct _slk_NetS_CityArmyGroupList SLK_NetS_CityArmyGroupList;	//��ս�б�

struct _slk_NetS_MapTownInfo {
	int m_protect_sec;	//��������
	int m_produce_sec;	//��������
	int m_own_actorid;	//��������
	char m_own_namelen;	//��������
	char m_own_name[32];	//��������
	int m_own_sec;	//��������
	int m_hp;	//��������
	int m_maxhp;	//��������
	char m_myask;	//��������
	short m_produce_num;	//��������
	short m_townid;	//��������
};
typedef struct _slk_NetS_MapTownInfo SLK_NetS_MapTownInfo;	//��������

struct _slk_NetS_TownArmyGroup {
	int m_group_index;	//��ս��Ϣ-��������
	int m_group_id;	//��ս��Ϣ-����id
	char m_attack;	//��ս��Ϣ-1����2����
	int m_statetime;	//��ս��Ϣ-����ʱ
	int m_stateduration;	//��ս��Ϣ-����ʱ
	char m_nation;	//��ս��Ϣ
	char m_t_nation;	//��ս��Ϣ
	int m_total;	//��ս��Ϣ
	int m_t_total;	//��ս��Ϣ
	char m_type;	//��ս��Ϣ
};
typedef struct _slk_NetS_TownArmyGroup SLK_NetS_TownArmyGroup;	//��ս��Ϣ

struct _slk_NetS_TownArmyGroupList {
	short m_count;	//��ս��Ϣ�б�
	SLK_NetS_TownArmyGroup m_list[16];	//��ս��Ϣ�б�
	char m_flag;	//��ս��Ϣ�б�
	char m_nation;	//��ս��Ϣ�б�
	int m_unit_index;	//��ս��Ϣ�б�
	short m_totalcount;	//��ս��Ϣ�б�
	int m_townid;	//��ս��Ϣ�б�
};
typedef struct _slk_NetS_TownArmyGroupList SLK_NetS_TownArmyGroupList;	//��ս�б�

struct _slk_NetS_SystalkJson {
	short m_msglen;	//jsonϵͳ��Ϣ
	char m_msg[1024];	//jsonϵͳ��Ϣ
	int m_optime;	//jsonϵͳ��Ϣ
	char m_roll;	//jsonϵͳ��Ϣ
};
typedef struct _slk_NetS_SystalkJson SLK_NetS_SystalkJson;	//jsonϵͳ��Ϣ

struct _slk_NetS_RollMsgJson {
	short m_msglen;	//json������Ϣ
	char m_msg[1024];	//json������Ϣ
};
typedef struct _slk_NetS_RollMsgJson SLK_NetS_RollMsgJson;	//json������Ϣ

struct _slk_NetS_RollMsg {
	short m_msglen;	//������Ϣ
	char m_msg[1024];	//������Ϣ
	short m_titlelen;	//������Ϣ
	char m_title[128];	//������Ϣ
};
typedef struct _slk_NetS_RollMsg SLK_NetS_RollMsg;	//������Ϣ

struct _slk_NetS_TownOwnerAsk {
	char m_name_len;	//�����������
	char m_name[32];	//�����������
	char m_place;	//�����������
};
typedef struct _slk_NetS_TownOwnerAsk SLK_NetS_TownOwnerAsk;	//����������Ϣ

struct _slk_NetS_TownOwnerAskList {
	char m_count;	//���������б�
	SLK_NetS_TownOwnerAsk m_list[5];	//���������б�
	int m_sec;	//���������б�
};
typedef struct _slk_NetS_TownOwnerAskList SLK_NetS_TownOwnerAskList;	//���������б�

struct _slk_NetS_TownFight {
	short m_townid;	//����ս������id
	int m_statetime;	//����ս������ʱ
	char m_attack;	//����ս���������Ƿ���
	char m_nation;	//����ս���������
};
typedef struct _slk_NetS_TownFight SLK_NetS_TownFight;	//��ս��Ϣ

struct _slk_NetS_TownFightList {
	short m_count;	//����ս���б�
	SLK_NetS_TownFight m_list[200];	//����ս���б�
};
typedef struct _slk_NetS_TownFightList SLK_NetS_TownFightList;	//��ս�б�

struct _slk_NetS_MapTownExInfo {
	char m_dev_level;	//���ǿ����ȼ�
	int m_dev_exp;	//���ǿ�������
	int m_dev_expmax;	//���ǿ�������
	short m_mytownid;	//�ҵĶ���
	int m_dev_cd;	//���ǿ�����ȴ
	int m_attackcd;	//��������ʱ
};
typedef struct _slk_NetS_MapTownExInfo SLK_NetS_MapTownExInfo;	//������Ϣ

struct _slk_NetS_MapZoneTown {
	short m_townid;	//���������б�
	char m_nation;	//���������б�
	int m_protect_sec;	//���������б�
	short m_from_nation[4];	//���������б�
};
typedef struct _slk_NetS_MapZoneTown SLK_NetS_MapZoneTown;	//����������Ϣ

struct _slk_NetS_MapZoneTownList {
	short m_count;	//���������б�
	SLK_NetS_MapZoneTown m_list[32];	//���������б�
	char m_zoneid;	//���������б�
};
typedef struct _slk_NetS_MapZoneTownList SLK_NetS_MapZoneTownList;	//����������Ϣ�б�

struct _slk_NetS_MapCenterTown {
	short m_townid;	//�ʳ���������б�
	char m_nation;	//�ʳ���������б�
};
typedef struct _slk_NetS_MapCenterTown SLK_NetS_MapCenterTown;	//�ʳǵ���������Ϣ

struct _slk_NetS_MapCenterTownList {
	char m_count;	//�ʳ���������б�
	SLK_NetS_MapCenterTown m_list[32];	//�ʳ���������б�
};
typedef struct _slk_NetS_MapCenterTownList SLK_NetS_MapCenterTownList;	//�ʳǵ��������б�

struct _slk_NetS_CityWarInfo {
	int m_group_index;	//������Ϣ-��ս����
	int m_group_id;	//������Ϣ-��սid
	char m_from_nation;	//������Ϣ-�����˹���
	short m_from_posx;	//������Ϣ-����������
	short m_from_posy;	//������Ϣ-����������
	char m_namelen;	//������Ϣ-����������
	char m_name[32];	//������Ϣ-����������
	int m_statetime;	//������Ϣ-ʱ��
	int m_stateduration;	//������Ϣ-ʱ��
};
typedef struct _slk_NetS_CityWarInfo SLK_NetS_CityWarInfo;	//��ս����

struct _slk_NetS_CityWarList {
	char m_count;	//�����б�
	SLK_NetS_CityWarInfo m_list[16];	//�����б�
};
typedef struct _slk_NetS_CityWarList SLK_NetS_CityWarList;	//��ս�����б�

struct _slk_NetS_CityWarDel {
	int m_group_index;	//ɾ������
};
typedef struct _slk_NetS_CityWarDel SLK_NetS_CityWarDel;	//��ս����ɾ��

struct _slk_NetS_WorldQuest {
	short m_questid;	//��������
	short m_value;	//��������
	short m_maxvalue;	//��������
	char m_complete;	//��������
	char m_dd;	//�Ƿ�˫��
};
typedef struct _slk_NetS_WorldQuest SLK_NetS_WorldQuest;	//��������

struct _slk_NetS_WorldBoss {
	char m_bossid;	//����boos��Ϣ
	int m_hp;	//����boos��Ϣ
	int m_maxhp;	//����boos��Ϣ
	char m_isfight;	//����boos��Ϣ�����Ƿ���
	short m_actorlevel;	//����boos��Ϣ�ȼ�����
	char m_resetnum;	//����boos��Ϣ�������ô���
};
typedef struct _slk_NetS_WorldBoss SLK_NetS_WorldBoss;	//����boss��Ϣ

struct _slk_NetS_LostRebuild {
	int m_rb_silver;	//��԰�ؽ�
	int m_rb_wood;	//��԰�ؽ�
	int m_rb_food;	//��԰�ؽ�
	int m_rb_df;	//��԰�ؽ�
};
typedef struct _slk_NetS_LostRebuild SLK_NetS_LostRebuild;	//��԰�ؽ�

struct _slk_NetS_KingwarTown {
	char m_id;	//Ѫս�ݵ�
	int m_attack_total;	//Ѫս�ݵ�
	int m_defense_total;	//Ѫս�ݵ�
	char m_nation;	//Ѫս�ݵ�
};
typedef struct _slk_NetS_KingwarTown SLK_NetS_KingwarTown;	//Ѫս�ݵ�

struct _slk_NetS_KingwarTownList {
	char m_count;	//Ѫս�ݵ��б�
	SLK_NetS_KingwarTown m_list[8];	//Ѫս�ݵ��б�
	int m_leftstamp;	//Ѫս�ݵ��б�
	int m_losthp;	//Ѫս�ݵ��б�
};
typedef struct _slk_NetS_KingwarTownList SLK_NetS_KingwarTownList;	//Ѫս�ݵ��б�

struct _slk_NetS_KingWarActivity {
	char m_state;	//Ѫս�ݵ���Ϣ
	int m_beginstamp;	//Ѫս�ݵ���Ϣ
	int m_endstamp;	//Ѫս�ݵ���Ϣ
	char m_nation;	//Ѫս�ݵ���Ϣ
};
typedef struct _slk_NetS_KingWarActivity SLK_NetS_KingWarActivity;	//Ѫս�

struct _slk_NetS_KingWarNotify {
	short m_a_heroid;	//Ѫս�ս��
	char m_a_color;	//Ѫս�ս��
	char m_a_name_len;	//Ѫս�ս��
	char m_a_name[32];	//Ѫս�ս��
	int m_a_losthp;	//Ѫս�ս��
	char m_a_nation;	//Ѫս�ս��
	short m_d_heroid;	//Ѫս�ս��
	char m_d_color;	//Ѫս�ս��
	char m_d_name_len;	//Ѫս�ս��
	char m_d_name[32];	//Ѫս�ս��
	int m_d_losthp;	//Ѫս�ս��
	char m_d_nation;	//Ѫս�ս��
	char m_result;	//Ѫս�ս��
	char m_id;	//Ѫս�ս��-�����ݵ�
};
typedef struct _slk_NetS_KingWarNotify SLK_NetS_KingWarNotify;	//Ѫսս��

struct _slk_NetS_KingWarNotifyList {
	char m_count;	//Ѫս�����ս���б�
	SLK_NetS_KingWarNotify m_list[10];	//Ѫս�����ս���б�
};
typedef struct _slk_NetS_KingWarNotifyList SLK_NetS_KingWarNotifyList;	//Ѫսս���б�

struct _slk_NetS_KingWarRank {
	char m_rank;	//Ѫս���а�
	int m_kill;	//Ѫս���а�
	char m_nation;	//Ѫս���а�
	char m_name_len;	//Ѫս���а�
	char m_name[32];	//Ѫս���а�
	short m_herokind;	//Ѫս���а�
	char m_herocolor;	//Ѫս���а�
};
typedef struct _slk_NetS_KingWarRank SLK_NetS_KingWarRank;	//Ѫս���а�

struct _slk_NetS_KingWarRankList {
	short m_count;	//Ѫս���а��б�
	SLK_NetS_KingWarRank m_list[30];	//Ѫս���а��б�
	int m_mypoint;	//Ѫս���а��б�-�ҵ��ܻ���
	int m_totalkill;	//Ѫս���а��б�-��ǰ�ۼ�ɱ��
	char m_myrank;	//Ѫս���а��б�-�ҵ�����
};
typedef struct _slk_NetS_KingWarRankList SLK_NetS_KingWarRankList;	//Ѫս���а��б�

struct _slk_NetS_KingWarPK {
	short m_herokind;	//Ѫս����֪ͨ
	int m_hp;	//Ѫս����֪ͨ
	char m_id;	//Ѫս����֪ͨ
};
typedef struct _slk_NetS_KingWarPK SLK_NetS_KingWarPK;	//ѪսPK

struct _slk_NetS_KingWarPoint {
	int m_point;	//Ѫս����
};
typedef struct _slk_NetS_KingWarPoint SLK_NetS_KingWarPoint;	//Ѫս����

struct _slk_NetS_TreasureActivity {
	char m_state;	//�ڱ����Ϣ
	int m_endstamp;	//�ڱ����Ϣ
	char m_nation;	//�ڱ����Ϣ
	short m_treasure_num[3];	//�ڱ����Ϣ
	short m_treasure_num_max;	//�ڱ����Ϣ
};
typedef struct _slk_NetS_TreasureActivity SLK_NetS_TreasureActivity;	//�ڱ��

struct _slk_NetS_TreasureHas {
	char m_has;	//�Ƿ��Ѿ��ڵ�ͼֽ
	short m_px;	//��ͼ����
	short m_py;	//��ͼ����
	char m_tn;	//�ڱ�����
};
typedef struct _slk_NetS_TreasureHas SLK_NetS_TreasureHas;	//�Ƿ��Ѿ��ڵ�ͼֽ

struct _slk_NetS_GotoAsyn {
	short m_posx;	//�첽��ת
	short m_posy;	//�첽��ת
	char m_type;	//�첽��ת
	int m_unit_index;	//�첽��ת
	char m_op;	//�첽��ת
};
typedef struct _slk_NetS_GotoAsyn SLK_NetS_GotoAsyn;	//�첽��ת

struct _slk_NetS_TreasureActor {
	short m_itemkind;	//������Ϣ
	char m_name_len;	//������Ϣ
	char m_name[32];	//������Ϣ
};
typedef struct _slk_NetS_TreasureActor SLK_NetS_TreasureActor;	//�ں�ͼ��Ϣ

struct _slk_NetS_TreasureActorList {
	char m_count;	//�����б�
	SLK_NetS_TreasureActor m_list[16];	//�����б�
	char m_co;	//����Ƿ��Ѿ�����
};
typedef struct _slk_NetS_TreasureActorList SLK_NetS_TreasureActorList;	//�ں�ͼ��Ϣ�б�

struct _slk_NetS_ChangeAutoBuild {
	char m_autobuild;	//�Զ�����
	char m_autobuildopen;	//�Զ�����
	short m_path;	//�Զ�����
};
typedef struct _slk_NetS_ChangeAutoBuild SLK_NetS_ChangeAutoBuild;	//�Զ�����

struct _slk_NetS_HeroVisitInfo {
	int m_hv_free_cd;	//����Ѱ�����CD
	int m_hv_high_sec;	//��Ѱ�ý���ʱ��
	char m_hv_high_free;	//��Ѱ��ʣ����Ѵ���
	char m_hv_low_num;	//����Ѱ�ô���
	char m_hv_high_num;	//��Ѱ�ô���
	short m_hv_progress;	//Ѱ�ý���
};
typedef struct _slk_NetS_HeroVisitInfo SLK_NetS_HeroVisitInfo;	//�佫Ѱ����Ϣ

struct _slk_NetS_NationBase {
	char m_level;	//���ҵȼ�
	int m_exp;	//���Ҿ���
	char m_donate_num;	//������ҽ������
	int m_myrank;	//�ҵ�����
};
typedef struct _slk_NetS_NationBase SLK_NetS_NationBase;	//������Ϣ

struct _slk_NetS_HeroVisitAward {
	short m_id[10];	//Ѱ�û�ȡ�Ľ���id
	char m_count;	//����
};
typedef struct _slk_NetS_HeroVisitAward SLK_NetS_HeroVisitAward;	//Ѱ�ý���id

struct _slk_NetS_FightPlay {
	char m_flag;	//ս������
	short m_content_length;	//ս������
	char m_content[1800];	//ս������
};
typedef struct _slk_NetS_FightPlay SLK_NetS_FightPlay;	//ս������

struct _slk_NetS_StorySweepHero {
	short m_kind;	//ɨ���佫��Ϣ
	char m_color;	//ɨ���佫��Ϣ
	short m_level;	//ɨ���佫��Ϣ
	int m_pre_exp;	//ɨ���佫��Ϣ
	int m_exp;	//ɨ���佫��Ϣ
	char m_type;	//ɨ���佫��Ϣ
};
typedef struct _slk_NetS_StorySweepHero SLK_NetS_StorySweepHero;	//ɨ���佫

struct _slk_NetS_StorySweepResult {
	int m_exp;	//ɨ�����
	int m_silver;	//ɨ�����
	char m_awardcount;	//ɨ�����
	SLK_NetS_AwardInfo m_award[16];	//ɨ�����
	char m_herocount;	//ɨ�����
	SLK_NetS_StorySweepHero m_hero[4];	//ɨ�����
};
typedef struct _slk_NetS_StorySweepResult SLK_NetS_StorySweepResult;	//ɨ�����

struct _slk_NetS_QuestTalk {
	int m_talkid;	//����Ի�
	short m_shape;	//����Ի�
	int m_talk_textid;	//����Ի�
	int m_btn_textid;	//����Ի�
	char m_op;	//����Ի�
	char m_format;	//����Ի�
};
typedef struct _slk_NetS_QuestTalk SLK_NetS_QuestTalk;	//����Ի�

struct _slk_NetS_RankInfo {
	int m_rank;	//���а�
	char m_namelen;	//���а�
	char m_name[32];	//���а�
	char m_nation;	//���а�
	short m_level;	//���а�
	char m_place;	//���а�
	int m_battlepower;	//���а�
	int m_actorid;	//���а�
};
typedef struct _slk_NetS_RankInfo SLK_NetS_RankInfo;	//���а���Ϣ

struct _slk_NetS_RankList {
	char m_count;	//���а��б�
	SLK_NetS_RankInfo m_list[10];	//���а��б�
	char m_type;	//���а��б�-����
	int m_page;	//���а��б�-ҳ��
	int m_myrank;	//���а��б�-�ҵ�����
};
typedef struct _slk_NetS_RankList SLK_NetS_RankList;	//���а��б�

struct _slk_NetS_FriendInfo {
	int m_actorid;	//������Ϣ
	int m_city_index;	//������Ϣ
	char m_shape;	//������Ϣ
	char m_namelen;	//������Ϣ
	char m_name[32];	//������Ϣ
	short m_level;	//������Ϣ
	char m_place;	//������Ϣ
	int m_battlepower;	//������Ϣ
	char m_ask;	//������Ϣ
};
typedef struct _slk_NetS_FriendInfo SLK_NetS_FriendInfo;	//������Ϣ

struct _slk_NetS_FriendList {
	char m_count;	//�����б�
	SLK_NetS_FriendInfo m_list[40];	//�����б�
	char m_op;	//�����б�
};
typedef struct _slk_NetS_FriendList SLK_NetS_FriendList;	//�����б�

struct _slk_NetS_ActorSearch {
	int m_actorid;	//��ѯ�����Ϣ
	int m_city_index;	//��ѯ�����Ϣ
	char m_namelen;	//��ѯ�����Ϣ
	char m_name[32];	//��ѯ�����Ϣ
	char m_signlen;	//��ѯ�����Ϣ
	char m_sign[128];	//��ѯ�����Ϣ
	short m_level;	//��ѯ�����Ϣ
	char m_official;	//��ѯ�����Ϣ
	char m_place;	//��ѯ�����Ϣ
	char m_zone;	//��ѯ�����Ϣ
	char m_nation;	//��ѯ�����Ϣ
	int m_battlepower;	//��ѯ�����Ϣ
	int m_killcity;	//��ѯ�����Ϣ
	char m_herocount;	//��ѯ�����Ϣ
	SLK_NetS_StorySweepHero m_hero[4];	//��ѯ�����Ϣ
	int m_bp_hero;	//��ѯ�����Ϣ
	int m_bp_equip;	//��ѯ�����Ϣ
	int m_bp_tech;	//��ѯ�����Ϣ
	int m_bp_nequip;	//��ѯ�����Ϣ
	int m_bp_girl;	//��ѯ�����Ϣ
	int m_bp_place;	//��ѯ�����Ϣ
	int m_my_bp_hero;	//��ѯ�����Ϣ
	int m_my_bp_equip;	//��ѯ�����Ϣ
	int m_my_bp_tech;	//��ѯ�����Ϣ
	int m_my_bp_nequip;	//��ѯ�����Ϣ
	int m_my_bp_girl;	//��ѯ�����Ϣ
	int m_my_bp_place;	//��ѯ�����Ϣ
	char m_shape;	//��ѯ�����Ϣ
};
typedef struct _slk_NetS_ActorSearch SLK_NetS_ActorSearch;	//��Ҳ�ѯ��Ϣ

struct _slk_NetS_BlackInfo {
	int m_actorid;	//������
	char m_namelen;	//������
	char m_name[32];	//������
	short m_level;	//������
	char m_nation;	//������
	char m_shape;	//������
};
typedef struct _slk_NetS_BlackInfo SLK_NetS_BlackInfo;	//������

struct _slk_NetS_BlackList {
	char m_count;	//�������б�
	SLK_NetS_BlackInfo m_list[50];	//�������б�
	char m_op;	//�������б�
};
typedef struct _slk_NetS_BlackList SLK_NetS_BlackList;	//�������б�

struct _slk_NetS_BlackListID {
	char m_count;	//�������б�
	int m_actorid[50];	//�������б�
};
typedef struct _slk_NetS_BlackListID SLK_NetS_BlackListID;	//�������б�ֻ��id

struct _slk_NetS_NationEquip {
	short m_neq_lv;	//������Ϣ
	char m_neq_exp;	//������Ϣ
	char m_neq_pro;	//������Ϣ
	int m_neq_sec;	//������Ϣ
	char m_neq_crit;	//������Ϣ
	char m_neq_star;	//������Ϣ
	char m_kind;	//������Ϣ
};
typedef struct _slk_NetS_NationEquip SLK_NetS_NationEquip;	//������Ϣ

struct _slk_NetS_ShopItem {
	short m_offset;	//�̵����
	int m_awardkind;	//�̵����
	int m_awardnum;	//�̵����
	short m_token;	//�̵����
	short m_sale;	//�̵����
	char m_buyuse;	//�̵����
};
typedef struct _slk_NetS_ShopItem SLK_NetS_ShopItem;	//�̵����

struct _slk_NetS_ShopList {
	char m_type;	//�̵��б�
	char m_count;	//�̵��б�
	SLK_NetS_ShopItem m_list[64];	//�̵��б�
};
typedef struct _slk_NetS_ShopList SLK_NetS_ShopList;	//�̵�����б�

struct _slk_NetS_PayStoreGoods {
	short m_goodsid;	//֧���̵�
	int m_price;	//֧���̵�
	int m_token;	//֧���̵�
	short m_day;	//֧���̵�
	short m_nameid;	//֧���̵�
	short m_icon;	//֧���̵�
};
typedef struct _slk_NetS_PayStoreGoods SLK_NetS_PayStoreGoods;	//֧����Ʒ

struct _slk_NetS_PayStore {
	char m_count;	//֧���̵��б�
	SLK_NetS_PayStoreGoods m_list[16];	//֧���̵��б�
	char m_paymode;	//֧���̵��б�
};
typedef struct _slk_NetS_PayStore SLK_NetS_PayStore;	//֧����Ʒ�̵�

struct _slk_NetS_GoodsInfo {
	short m_goodsid;	//��Ʒ��Ϣ
	int m_price;	//��Ʒ��Ϣ
	int m_nameid;	//��Ʒ��Ϣ
	int m_descid;	//��Ʒ��Ϣ
	char m_icon;	//��Ʒ��Ϣ
	short m_sale;	//��Ʒ��Ϣ
	int m_worth;	//��Ʒ��Ϣ
	int m_bag_time;	//��Ʒ��Ϣ
	char m_bag_num;	//��Ʒ��Ϣ
	char m_awardcount;	//��Ʒ��Ϣ
	SLK_NetS_AwardInfo m_award[16];	//��Ʒ��Ϣ
};
typedef struct _slk_NetS_GoodsInfo SLK_NetS_GoodsInfo;	//�����Ʒ��Ϣ

struct _slk_NetS_PayStoreActivity {
	char m_count;	//����б�
	SLK_NetS_GoodsInfo m_list[8];	//����б�
	char m_path;	//����б�
};
typedef struct _slk_NetS_PayStoreActivity SLK_NetS_PayStoreActivity;	//����б�

struct _slk_NetS_PayOrder {
	char m_orderid_len;	//֧������
	char m_orderid[32];	//֧������
	char m_ext_len;	//֧������
	char m_ext[64];	//֧������
	short m_goodsid;	//֧������
	int m_productid;	//֧������
	int m_nameid;	//֧������
	int m_descid;	//֧������
	int m_price;	//֧������
};
typedef struct _slk_NetS_PayOrder SLK_NetS_PayOrder;	//����

struct _slk_NetS_VipShopItem {
	int m_awardkind;	//vip�ؼ���Ʒ
	int m_awardnum;	//vip�ؼ���Ʒ
	short m_token;	//vip�ؼ���Ʒ
	short m_itemkind;	//vip�ؼ���Ʒ
	short m_vip_token;	//vip�ؼ���Ʒ
	short m_vip_buynum;	//vip�ؼ���Ʒ
	short m_vip_buynum_max;	//vip�ؼ���Ʒ
	char m_id;	//vip�ؼ���Ʒ
};
typedef struct _slk_NetS_VipShopItem SLK_NetS_VipShopItem;	//vip�ؼ���Ʒ

struct _slk_NetS_VipShop {
	short m_count;	//vip�ؼ��̵�
	SLK_NetS_VipShopItem m_list[32];	//vip�ؼ��̵�
	char m_useitem;	//vip�ؼ��̵�
};
typedef struct _slk_NetS_VipShop SLK_NetS_VipShop;	//vip�ؼ��̵�

struct _slk_NetS_VipBag {
	int m_vipbag;	//vip���
	char m_vipbag_count;	//vip���
};
typedef struct _slk_NetS_VipBag SLK_NetS_VipBag;	//vip���

struct _slk_NetS_BuffChange {
	char m_buffkind;	//BUFF����
	int m_endtime;	//BUFF����
	short m_path;	//BUFF����
	short m_bufftrain;	//BUFF����
};
typedef struct _slk_NetS_BuffChange SLK_NetS_BuffChange;	//buff����

struct _slk_NetS_ChangeAutoGuard {
	char m_autoguard;	//�Ƿ�����
	char m_autoguardopen;	//�Ƿ�����
	short m_path;	//�Ƿ�����
};
typedef struct _slk_NetS_ChangeAutoGuard SLK_NetS_ChangeAutoGuard;	//�Ƿ�����

struct _slk_NetS_Activity {
	short m_activityid;	//�
	int m_starttime;	//�
	int m_endtime;	//�
	int m_closetime;	//�
	char m_red;	//�
};
typedef struct _slk_NetS_Activity SLK_NetS_Activity;	//�

struct _slk_NetS_ActivityList {
	char m_count;	//��б�
	SLK_NetS_Activity m_list[64];	//��б�
};
typedef struct _slk_NetS_ActivityList SLK_NetS_ActivityList;	//��б�

struct _slk_NetS_NationEquipList {
	char m_count;	//�����б�
	SLK_NetS_NationEquip m_list[6];	//�����б�
};
typedef struct _slk_NetS_NationEquipList SLK_NetS_NationEquipList;	//�����б�

struct _slk_NetS_NationInfo {
	char m_level;	//������Ϣ
	int m_exp;	//������Ϣ
	char m_donate_num;	//������Ϣ
	int m_myrank;	//������Ϣ
	short m_notice_len;	//������Ϣ
	char m_notice[512];	//������Ϣ
	char m_kingname_len;	//������Ϣ
	char m_kingname[32];	//������Ϣ
	short m_questvalue[3];	//������Ϣ
	short m_questvalue_max[3];	//������Ϣ
};
typedef struct _slk_NetS_NationInfo SLK_NetS_NationInfo;	//������Ϣ

struct _slk_NetS_NationTown {
	SLK_NetS_MapTownInfo m_info;	//���ҳǳ�
	char m_path;	//���ҳǳ�
};
typedef struct _slk_NetS_NationTown SLK_NetS_NationTown;	//���ҳǳ�

struct _slk_NetS_NationTownList {
	char m_count;	//�������������б�
	SLK_NetS_NationTown m_list[32];	//�������������б�
	char m_op;	//�������������б�
};
typedef struct _slk_NetS_NationTownList SLK_NetS_NationTownList;	//���ҳǳ��б�

struct _slk_NetS_NationWar {
	int m_group_index;	//���ҽ���-��ս�б�-��������
	int m_group_id;	//���ҽ���-��ս�б�-����id
	char m_attack;	//���ҽ���-��ս�б�--1����2����
	int m_statetime;	//���ҽ���-��ս�б�--����ʱ
	int m_stateduration;	//���ҽ���-��ս�б�--����ʱ
	char m_nation;	//���ҽ���-��ս�б�-
	char m_t_nation;	//���ҽ���-��ս�б�-
	int m_total;	//���ҽ���-��ս�б�-
	int m_t_total;	//���ҽ���-��ս�б�-
	char m_type;	//���ҽ���-��ս�б�-
	short m_townid;	//���ҽ���-��ս�б�-
	int m_unit_index;	//���ҽ���-��ս�б�-
	char m_town_nation;	//���ҽ���-��ս�б�-
};
typedef struct _slk_NetS_NationWar SLK_NetS_NationWar;	//���ҽ���-����ս��

struct _slk_NetS_NationWarList {
	short m_count;	//���ҽ���-����ս���б�
	SLK_NetS_NationWar m_list[32];	//���ҽ���-����ս���б�
	char m_op;	//���ҽ���-����ս���б�
};
typedef struct _slk_NetS_NationWarList SLK_NetS_NationWarList;	//���ҽ���-����ս���б�

struct _slk_NetS_NationCityWar {
	int m_group_index;	//���ҽ���-��ս��Ϣ-��������
	int m_group_id;	//���ҽ���-��ս��Ϣ-����id
	char m_attack;	//��ս��Ϣ-1����2����
	int m_statetime;	//���ҽ���-��ս��Ϣ-����ʱ
	int m_stateduration;	//���ҽ���-��ս��Ϣ-����ʱ
	char m_nation;	//���ҽ���-��ս��Ϣ
	char m_t_nation;	//���ҽ���-��ս��Ϣ
	short m_level;	//���ҽ���-��ս��Ϣ
	short m_t_level;	//���ҽ���-��ս��Ϣ
	char m_name_length;	//���ҽ���-��ս��Ϣ
	char m_name[32];	//���ҽ���-��ս��Ϣ
	char m_t_name_length;	//���ҽ���-��ս��Ϣ
	char m_t_name[32];	//���ҽ���-��ս��Ϣ
	short m_posx;	//���ҽ���-��ս��Ϣ
	short m_posy;	//���ҽ���-��ս��Ϣ
	short m_t_posx;	//���ҽ���-��ս��Ϣ
	short m_t_posy;	//���ҽ���-��ս��Ϣ
	int m_actorid;	//���ҽ���-��ս��Ϣ
	int m_t_actorid;	//���ҽ���-��ս��Ϣ
	int m_total;	//���ҽ���-��ս��Ϣ
	int m_t_total;	//���ҽ���-��ս��Ϣ
	char m_type;	//���ҽ���-��ս��Ϣ
	int m_unit_index;	//���ҽ���-��ս��Ϣ
};
typedef struct _slk_NetS_NationCityWar SLK_NetS_NationCityWar;	//���ҽ���-�ǳ�ս��

struct _slk_NetS_NationCityWarList {
	char m_count;	//���ҽ���-��ս��Ϣ�б�
	SLK_NetS_NationCityWar m_list[16];	//���ҽ���-��ս��Ϣ�б�
	char m_op;	//���ҽ���-��ս��Ϣ�б�
};
typedef struct _slk_NetS_NationCityWarList SLK_NetS_NationCityWarList;	//���ҽ���-�ǳ�ս��

struct _slk_NetS_NationQuest {
	char m_level;	//��������
	char m_kind;	//��������
	int m_value;	//��������
	int m_needvalue;	//��������
	int m_awardkind[5];	//����������
	int m_awardnum[5];	//����������
	int m_other_awardkind;	//����������
	int m_other_awardnum;	//����������
};
typedef struct _slk_NetS_NationQuest SLK_NetS_NationQuest;	//��������

struct _slk_NetS_NationQuestList {
	char m_count;	//���������б�
	SLK_NetS_NationQuest m_list[3];	//���������б�
};
typedef struct _slk_NetS_NationQuestList SLK_NetS_NationQuestList;	//���������б�

struct _slk_NetS_NationMission {
	int m_value[3];	//������������
	int m_needvalue[3];	//������������
	int m_awardkind[2];	//������������
	int m_awardnum[2];	//������������
	char m_isget;	//������������
};
typedef struct _slk_NetS_NationMission SLK_NetS_NationMission;	//������������

struct _slk_NetS_NationMissionList {
	char m_count;	//�������������б�
	SLK_NetS_NationMission m_list[4];	//�������������б�
};
typedef struct _slk_NetS_NationMissionList SLK_NetS_NationMissionList;	//�������������б�

struct _slk_NetS_NationRankMember {
	char m_name_len;	//�����������а��Ա
	char m_name[32];	//�����������а��Ա
	int m_value;	//�����������а��Ա
};
typedef struct _slk_NetS_NationRankMember SLK_NetS_NationRankMember;	//�����������а�

struct _slk_NetS_NationRankList {
	SLK_NetS_NationRankMember m_buildrank[5];	//�����������а�
	SLK_NetS_NationRankMember m_cityrank[5];	//�����������а�
	SLK_NetS_NationRankMember m_townrank[5];	//�����������а�
	short m_vote;	//�����������а�
};
typedef struct _slk_NetS_NationRankList SLK_NetS_NationRankList;	//�����������а�

struct _slk_NetS_NationOfficial {
	char m_official;	//���ҹ�Ա��Ϣ
	char m_namelen;	//���ҹ�Ա��Ϣ
	char m_name[32];	//���ҹ�Ա��Ϣ
	short m_level;	//���ҹ�Ա��Ϣ
	int m_battlepower;	//���ҹ�Ա��Ϣ
	char m_zone;	//���ҹ�Ա��Ϣ
};
typedef struct _slk_NetS_NationOfficial SLK_NetS_NationOfficial;	//���ҹ�Ա

struct _slk_NetS_NationOfficialList {
	char m_count;	//���ҹ�Ա�б�
	SLK_NetS_NationOfficial m_list[12];	//���ҹ�Ա�б�
	int m_endtime;	//���ҹ�Ա�б�
};
typedef struct _slk_NetS_NationOfficialList SLK_NetS_NationOfficialList;	//���ҹ�Ա�б�

struct _slk_NetS_NationCandidate {
	char m_namelen;	//��ѡ��
	char m_name[32];	//��ѡ��
	short m_level;	//��ѡ��
	int m_battlepower;	//��ѡ��
	int m_ballot;	//��ѡ��
	int m_actorid;	//��ѡ��
};
typedef struct _slk_NetS_NationCandidate SLK_NetS_NationCandidate;	//���ҹ�Ա��ѡ��

struct _slk_NetS_NationCandidateList {
	char m_count;	//��ѡ���б�
	SLK_NetS_NationCandidate m_list[10];	//��ѡ���б�
	int m_endtime;	//��ѡ���б�
	short m_myvote;	//��ѡ���б�
	char m_isballot;	//��ѡ���б�
	short m_tokenballot;	//��ѡ���б�
};
typedef struct _slk_NetS_NationCandidateList SLK_NetS_NationCandidateList;	//���ҹ�Ա��ѡ���б�

struct _slk_NetS_WorldDataOpen {
	char m_open_zone_sili;	//˾���Ƿ񿪷�
	char m_open_zone_luoyang;	//�����Ƿ񿪷�
};
typedef struct _slk_NetS_WorldDataOpen SLK_NetS_WorldDataOpen;	//�������ݿ���

struct _slk_NetS_NationReplace {
	char m_namelen;	//��������
	char m_name[32];	//��������
	short m_level;	//��������
	int m_battlepower;	//��������
	char m_place;	//��������
	char m_official;	//��������
	int m_actorid;	//��������
};
typedef struct _slk_NetS_NationReplace SLK_NetS_NationReplace;	//��Ա�滻

struct _slk_NetS_NationReplaceList {
	char m_count;	//���������б�
	SLK_NetS_NationReplace m_list[30];	//���������б�
	char m_op;	//���������б�
};
typedef struct _slk_NetS_NationReplaceList SLK_NetS_NationReplaceList;	//��Ա�滻�б�

struct _slk_NetS_NationLog {
	char m_type;	//������־
	short m_townid;	//������־
	char m_namelen;	//������־
	char m_name[22];	//������־
	char m_target_nation;	//������־
	int m_optime;	//������־
};
typedef struct _slk_NetS_NationLog SLK_NetS_NationLog;	//������־

struct _slk_NetS_NationLogList {
	char m_count;	//������־�б�
	SLK_NetS_NationLog m_list[10];	//������־�б�
};
typedef struct _slk_NetS_NationLogList SLK_NetS_NationLogList;	//������־�б�

struct _slk_NetS_NationHero {
	short m_kind;	//��������
	int m_actorid;	//��������
	char m_namelen;	//��������
	char m_name[32];	//��������
	short m_level;	//��������
	short m_posx;	//��������
	short m_posy;	//��������
	char m_buypos;	//��������
	char m_state;	//��������
	char m_forever;	//��������
	char m_nation;	//��������
};
typedef struct _slk_NetS_NationHero SLK_NetS_NationHero;	//��������

struct _slk_NetS_NationHeroList {
	char m_count;	//���������б�
	SLK_NetS_NationHero m_list[16];	//���������б�
	short m_nationlevel[3];	//���������б�
};
typedef struct _slk_NetS_NationHeroList SLK_NetS_NationHeroList;	//���������б�

struct _slk_NetS_CityNationHero {
	char m_state;	//������������
	short m_kind;	//������������
	char m_forever;	//������������
	char m_loyal;	//������������
	int m_runstamp;	//������������
};
typedef struct _slk_NetS_CityNationHero SLK_NetS_CityNationHero;	//������������

struct _slk_NetS_CityNationHeroList {
	char m_count;	//�������������б�
	SLK_NetS_CityNationHero m_list[16];	//�������������б�
};
typedef struct _slk_NetS_CityNationHeroList SLK_NetS_CityNationHeroList;	//�������������б�

struct _slk_NetS_NationHeroAttr {
	SLK_NetS_Hero m_attr;	//������������
	char m_namelen;	//������������
	char m_name[32];	//������������
	int m_actorid;	//������������
	char m_open;	//������������
	char m_nation;	//������������
	char m_loyal;	//������������
};
typedef struct _slk_NetS_NationHeroAttr SLK_NetS_NationHeroAttr;	//������������

struct _slk_NetS_WishingShopItem {
	short m_id;	//�۱��豦��
	char m_color;	//�۱��豦��
	int m_awardkind;	//�۱��豦��
	int m_awardnum;	//�۱��豦��
	int m_costkind;	//�۱��豦��
	int m_costnum;	//�۱��豦��
	char m_open;	//�۱��豦��
};
typedef struct _slk_NetS_WishingShopItem SLK_NetS_WishingShopItem;	//�۱������

struct _slk_NetS_WishingShop {
	char m_count;	//�۱���
	SLK_NetS_WishingShopItem m_list[9];	//�۱���
	int m_openstamp;	//�۱���
	char m_todaybuy;	//�۱���
};
typedef struct _slk_NetS_WishingShop SLK_NetS_WishingShop;	//�۱���

struct _slk_NetS_WishingChange {
	int m_silver;	//�۱��軻��
	int m_wood;	//�۱��軻��
	int m_silver_to_wood;	//�۱��軻��
	int m_wood_to_silver;	//�۱��軻��
	int m_silver_to_food;	//�۱��軻��
	int m_wood_to_food;	//�۱��軻��
	int m_food;	//�۱��軻��
	int m_food_to_silver;	//�۱��軻��
	int m_food_to_wood;	//�۱��軻��
	int m_cd;	//�۱��軻��
};
typedef struct _slk_NetS_WishingChange SLK_NetS_WishingChange;	//�۱���һ�

struct _slk_NetS_WishingPackItem {
	int m_awardkind;	//�۱�������Դ
	int m_awardnum;	//�۱�������Դ
	int m_costkind;	//�۱�������Դ
	int m_costnum;	//�۱�������Դ
	short m_id;	//�۱�������Դ
	int m_token;	//�۱�������Դ
};
typedef struct _slk_NetS_WishingPackItem SLK_NetS_WishingPackItem;	//�۱���������

struct _slk_NetS_WishingPack {
	char m_count;	//�۱�����
	SLK_NetS_WishingPackItem m_list[3];	//�۱�����
};
typedef struct _slk_NetS_WishingPack SLK_NetS_WishingPack;	//�۱�����

struct _slk_NetS_TianceQuest {
	short m_tc_state;	//��߸�����
	char m_tc_kind;	//��߸�����
	char m_tc_num;	//��߸�����
	short m_tc_tech;	//��߸�����
	char m_nation_tiance_level;	//��߸�����
	int m_nation_tiance_point;	//��߸�����
};
typedef struct _slk_NetS_TianceQuest SLK_NetS_TianceQuest;	//��߸�����

struct _slk_NetS_Student {
	int m_actorid;	//ʦͽ��Ϣ
	int m_city_index;	//ʦͽ��Ϣ
	char m_shape;	//ʦͽ��Ϣ
	char m_namelen;	//ʦͽ��Ϣ
	char m_name[32];	//ʦͽ��Ϣ
	short m_level;	//ʦͽ��Ϣ
	char m_place;	//ʦͽ��Ϣ
	int m_battlepower;	//ʦͽ��Ϣ
	char m_isteacher;	//ʦͽ��Ϣ
};
typedef struct _slk_NetS_Student SLK_NetS_Student;	//ʦͽ�б�

struct _slk_NetS_StudentList {
	char m_count;	//ʦͽ��Ϣ�б�
	SLK_NetS_Student m_list[31];	//ʦͽ��Ϣ�б�
	short m_te_award[10];	//8���ȼ��ǿ���ȡ�Ľ�������
	short m_te_awarded[10];	//8���ȼ��Ľ����Ѿ���ȡ����
	char m_teacheraward_count;	//�Ƿ��а�ʦ����
	SLK_NetS_AwardInfo m_teacheraward[8];	//�Ƿ��а�ʦ����
};
typedef struct _slk_NetS_StudentList SLK_NetS_StudentList;	//ʦͽ�б�

struct _slk_NetS_TeacherShopItem {
	int m_awardkind;	//ʦͽ�̵���Ʒ
	short m_buylevel;	//ʦͽ�̵���Ʒ
	int m_point;	//ʦͽ�̵���Ʒ
	short m_id;	//ʦͽ�̵���Ʒ
};
typedef struct _slk_NetS_TeacherShopItem SLK_NetS_TeacherShopItem;	//ʦͽ�̵�

struct _slk_NetS_TeacherShopList {
	char m_count;	//ʦͽ�̵�
	SLK_NetS_TeacherShopItem m_list[64];	//ʦͽ�̵�
	int m_mypoint;	//ʦͽ�̵�
};
typedef struct _slk_NetS_TeacherShopList SLK_NetS_TeacherShopList;	//ʦͽ�̵�

struct _slk_NetS_Activity03Info {
	int m_value;	//�-�����ӵ�
	char m_state;	//�-�����ӵ�
};
typedef struct _slk_NetS_Activity03Info SLK_NetS_Activity03Info;	//������ӵ�

struct _slk_NetS_Activity03List {
	char m_count;	//�-�����ӵ�
	SLK_NetS_Activity03Info m_list[10];	//�-�����ӵ�
};
typedef struct _slk_NetS_Activity03List SLK_NetS_Activity03List;	//������ӵ�

struct _slk_NetS_Activity05Item {
	int m_awardkind;	//�-���ǰݽ�
	int m_awardnum;	//�-���ǰݽ�
	int m_token;	//�-���ǰݽ�
	char m_isbuy;	//�-���ǰݽ�
};
typedef struct _slk_NetS_Activity05Item SLK_NetS_Activity05Item;	//����ǰݽ�

struct _slk_NetS_Activity05List {
	char m_count;	//�-���ǰݽ�
	SLK_NetS_Activity05Item m_list[6];	//�-���ǰݽ�
	char m_buynum;	//�-���ǰݽ�
	char m_updatenum;	//�-���ǰݽ�
	int m_updatestamp;	//�-���ǰݽ�
	char m_myxw;	//�-���ǰݽ�
};
typedef struct _slk_NetS_Activity05List SLK_NetS_Activity05List;	//����ǰݽ�

struct _slk_NetS_Activity01List {
	char m_count;	//�-�׳����
	SLK_NetS_AwardInfo m_list[8];	//�-�׳����
	char m_fristpay;	//�-�׳����
	char m_fristpay_award;	//�-�׳����
};
typedef struct _slk_NetS_Activity01List SLK_NetS_Activity01List;	//��׳����

struct _slk_NetS_Activity04Info {
	int m_value;	//�-���տ�
	char m_state;	//�-���տ�
};
typedef struct _slk_NetS_Activity04Info SLK_NetS_Activity04Info;	//����տ�

struct _slk_NetS_Activity04List {
	char m_count;	//�-���տ�
	SLK_NetS_Activity04Info m_list[128];	//�-���տ�
	char m_myday;	//�-�ҵڼ���
};
typedef struct _slk_NetS_Activity04List SLK_NetS_Activity04List;	//����տ�

struct _slk_NetS_Girl {
	char m_kind;	//Ů��
	char m_color;	//Ů��
	short m_soul;	//Ů��
	char m_love_level;	//Ů��
	int m_love_exp;	//Ů��
	int m_love_today;	//Ů��
	char m_sflag;	//Ů��
	short m_herokind;	//Ů��
};
typedef struct _slk_NetS_Girl SLK_NetS_Girl;	//Ů����Ϣ

struct _slk_NetS_GirlList {
	char m_count;	//Ů���б�
	SLK_NetS_Girl m_list[80];	//Ů���б�
};
typedef struct _slk_NetS_GirlList SLK_NetS_GirlList;	//Ů���б�

struct _slk_NetS_GirlGet {
	char m_kind;	//Ů����ȡ
	short m_path;	//Ů����ȡ
	short m_soulnum;	//Ů����ȡ
	SLK_NetS_Girl m_girl;	//Ů����ȡ
};
typedef struct _slk_NetS_GirlGet SLK_NetS_GirlGet;	//Ů����ȡ

struct _slk_NetS_GirlLove {
	char m_kind;	//Ů�����ܶ�
	char m_love_level;	//Ů�����ܶ�
	short m_love_exp;	//Ů�����ܶ�
	short m_love_today;	//Ů�����ܶ�
	short m_add;	//Ů�����ܶ�
	short m_path;	//Ů�����ܶ�
};
typedef struct _slk_NetS_GirlLove SLK_NetS_GirlLove;	//Ů�����ܶ�

struct _slk_NetS_GirlLoveResult {
	char m_kind;	//Ů�����ܻ������
	char m_sflag;	//Ů�����ܻ������
	char m_born;	//Ů�����ܻ������
	short m_makelove_exp;	//Ů�����ܻ������
};
typedef struct _slk_NetS_GirlLoveResult SLK_NetS_GirlLoveResult;	//Ů�����ܻ������

struct _slk_NetS_FsInfo {
	char m_freenum;	//������Ѵ���
	char m_nodeid;	//���нڵ�
	char m_awardcount;	//���н���
	SLK_NetS_AwardInfo m_awardlist[16];	//���н���
};
typedef struct _slk_NetS_FsInfo SLK_NetS_FsInfo;	//������Ϣ

struct _slk_NetS_FsVisit {
	char m_nodeid;	//����Ѱ�ý��
	int m_awardkind;	//����Ѱ�ý��
	int m_awardnum;	//����Ѱ�ý��
	char m_girlkind;	//����Ѱ�ý��
};
typedef struct _slk_NetS_FsVisit SLK_NetS_FsVisit;	//����Ѱ�ý��

struct _slk_NetS_FsVisitResult {
	char m_freenum;	//����Ѱ�ý���б�
	char m_count;	//����Ѱ�ý���б�
	SLK_NetS_FsVisit m_list[16];	//����Ѱ�ý���б�
};
typedef struct _slk_NetS_FsVisitResult SLK_NetS_FsVisitResult;	//����Ѱ�ý��

struct _slk_NetS_FsPalace {
	char m_count;	//���лʹ���Ժ����
	SLK_NetS_AwardInfo m_list[5];	//���лʹ���Ժ����
};
typedef struct _slk_NetS_FsPalace SLK_NetS_FsPalace;	//���лʹ���Ժ

struct _slk_NetS_FsPalaceAward {
	int m_awardkind;	//���лʹ���Ժ�������
	int m_awardnum;	//���лʹ���Ժ�������
};
typedef struct _slk_NetS_FsPalaceAward SLK_NetS_FsPalaceAward;	//�ʹ���Ժ����

struct _slk_NetS_FsPalaceResult {
	char m_count;	//���лʹ���Ժ�������
	SLK_NetS_FsPalaceAward m_list[5];	//���лʹ���Ժ�������
};
typedef struct _slk_NetS_FsPalaceResult SLK_NetS_FsPalaceResult;	//�ʹ���Ժ�������

struct _slk_NetS_GirlShopItem {
	int m_awardkind;	//Ů���̵�
	int m_awardnum;	//Ů���̵�
	int m_cost_awardkind;	//Ů���̵�
	int m_cost_awardnum;	//Ů���̵�
	char m_isbuy;	//Ů���̵�
	short m_id;	//Ů���̵�
};
typedef struct _slk_NetS_GirlShopItem SLK_NetS_GirlShopItem;	//Ů���̵�

struct _slk_NetS_GirlShop {
	char m_count;	//Ů���̵��б�
	SLK_NetS_GirlShopItem m_list[9];	//Ů���̵��б�
	int m_update_lefttime;	//ˢ��ʣ��ʱ��
	char m_update_num;	//ˢ�´���
	char m_update_nummax;	//ˢ�´�������
	char m_update_viplevel;	//ˢ�´���vip����
	short m_update_token;	//ˢ��Ԫ��
};
typedef struct _slk_NetS_GirlShop SLK_NetS_GirlShop;	//Ů���̵�

struct _slk_NetS_ZoneMaster {
	char m_nation;	//�����ʵ���Ϣ
	char m_shape;	//�����ʵ���Ϣ
	char m_namelen;	//�����ʵ���Ϣ
	char m_name[22];	//�����ʵ���Ϣ
	char m_zoneid;	//�����ʵ���Ϣ
};
typedef struct _slk_NetS_ZoneMaster SLK_NetS_ZoneMaster;	//�ʵ�������Ϣ

struct _slk_NetS_ZoneMasterList {
	char m_count;	//�����ʵ��б�
	SLK_NetS_ZoneMaster m_list[9];	//�����ʵ��б�
};
typedef struct _slk_NetS_ZoneMasterList SLK_NetS_ZoneMasterList;	//�ʵ������б�

struct _slk_NetS_DelZoneUnit {
	int m_unit_index;	//ɾ��С��ͼ�ϵĵ�Ԫ
	short m_posx;	//ɾ��С��ͼ�ϵĵ�Ԫ
	short m_posy;	//ɾ��С��ͼ�ϵĵ�Ԫ
};
typedef struct _slk_NetS_DelZoneUnit SLK_NetS_DelZoneUnit;	//ɾ��С��ͼ�ϵ����

struct _slk_NetS_FightHelper {
	char m_kind;	//��Ӷ��
	char m_shape;	//��Ӷ��
	char m_color;	//��Ӷ��
	char m_corps;	//��Ӷ��
	int m_attack;	//��Ӷ��
	int m_defense;	//��Ӷ��
	int m_troops;	//��Ӷ��
	int m_token;	//��Ӷ��
	char m_sort;	//��Ӷ��
	char m_buynum;	//��Ӷ��
};
typedef struct _slk_NetS_FightHelper SLK_NetS_FightHelper;	//��Ӷ��

struct _slk_NetS_FightHelperList {
	char m_count;	//��Ӷ���б�
	SLK_NetS_FightHelper m_list[16];	//��Ӷ���б�
	int m_group_index;	//��Ӷ���б�
};
typedef struct _slk_NetS_FightHelperList SLK_NetS_FightHelperList;	//��Ӷ���б�

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
int struct_NetS_MapUnitAction_send( char **pptr, int *psize, SLK_NetS_MapUnitAction *pValue );
int struct_NetS_MapPosAction_send( char **pptr, int *psize, SLK_NetS_MapPosAction *pValue );
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
int struct_NetS_NationBase_send( char **pptr, int *psize, SLK_NetS_NationBase *pValue );
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
int struct_NetS_BuffChange_send( char **pptr, int *psize, SLK_NetS_BuffChange *pValue );
int struct_NetS_ChangeAutoGuard_send( char **pptr, int *psize, SLK_NetS_ChangeAutoGuard *pValue );
int struct_NetS_Activity_send( char **pptr, int *psize, SLK_NetS_Activity *pValue );
int struct_NetS_ActivityList_send( char **pptr, int *psize, SLK_NetS_ActivityList *pValue );
int struct_NetS_NationEquipList_send( char **pptr, int *psize, SLK_NetS_NationEquipList *pValue );
int struct_NetS_NationInfo_send( char **pptr, int *psize, SLK_NetS_NationInfo *pValue );
int struct_NetS_NationTown_send( char **pptr, int *psize, SLK_NetS_NationTown *pValue );
int struct_NetS_NationTownList_send( char **pptr, int *psize, SLK_NetS_NationTownList *pValue );
int struct_NetS_NationWar_send( char **pptr, int *psize, SLK_NetS_NationWar *pValue );
int struct_NetS_NationWarList_send( char **pptr, int *psize, SLK_NetS_NationWarList *pValue );
int struct_NetS_NationCityWar_send( char **pptr, int *psize, SLK_NetS_NationCityWar *pValue );
int struct_NetS_NationCityWarList_send( char **pptr, int *psize, SLK_NetS_NationCityWarList *pValue );
int struct_NetS_NationQuest_send( char **pptr, int *psize, SLK_NetS_NationQuest *pValue );
int struct_NetS_NationQuestList_send( char **pptr, int *psize, SLK_NetS_NationQuestList *pValue );
int struct_NetS_NationMission_send( char **pptr, int *psize, SLK_NetS_NationMission *pValue );
int struct_NetS_NationMissionList_send( char **pptr, int *psize, SLK_NetS_NationMissionList *pValue );
int struct_NetS_NationRankMember_send( char **pptr, int *psize, SLK_NetS_NationRankMember *pValue );
int struct_NetS_NationRankList_send( char **pptr, int *psize, SLK_NetS_NationRankList *pValue );
int struct_NetS_NationOfficial_send( char **pptr, int *psize, SLK_NetS_NationOfficial *pValue );
int struct_NetS_NationOfficialList_send( char **pptr, int *psize, SLK_NetS_NationOfficialList *pValue );
int struct_NetS_NationCandidate_send( char **pptr, int *psize, SLK_NetS_NationCandidate *pValue );
int struct_NetS_NationCandidateList_send( char **pptr, int *psize, SLK_NetS_NationCandidateList *pValue );
int struct_NetS_WorldDataOpen_send( char **pptr, int *psize, SLK_NetS_WorldDataOpen *pValue );
int struct_NetS_NationReplace_send( char **pptr, int *psize, SLK_NetS_NationReplace *pValue );
int struct_NetS_NationReplaceList_send( char **pptr, int *psize, SLK_NetS_NationReplaceList *pValue );
int struct_NetS_NationLog_send( char **pptr, int *psize, SLK_NetS_NationLog *pValue );
int struct_NetS_NationLogList_send( char **pptr, int *psize, SLK_NetS_NationLogList *pValue );
int struct_NetS_NationHero_send( char **pptr, int *psize, SLK_NetS_NationHero *pValue );
int struct_NetS_NationHeroList_send( char **pptr, int *psize, SLK_NetS_NationHeroList *pValue );
int struct_NetS_CityNationHero_send( char **pptr, int *psize, SLK_NetS_CityNationHero *pValue );
int struct_NetS_CityNationHeroList_send( char **pptr, int *psize, SLK_NetS_CityNationHeroList *pValue );
int struct_NetS_NationHeroAttr_send( char **pptr, int *psize, SLK_NetS_NationHeroAttr *pValue );
int struct_NetS_WishingShopItem_send( char **pptr, int *psize, SLK_NetS_WishingShopItem *pValue );
int struct_NetS_WishingShop_send( char **pptr, int *psize, SLK_NetS_WishingShop *pValue );
int struct_NetS_WishingChange_send( char **pptr, int *psize, SLK_NetS_WishingChange *pValue );
int struct_NetS_WishingPackItem_send( char **pptr, int *psize, SLK_NetS_WishingPackItem *pValue );
int struct_NetS_WishingPack_send( char **pptr, int *psize, SLK_NetS_WishingPack *pValue );
int struct_NetS_TianceQuest_send( char **pptr, int *psize, SLK_NetS_TianceQuest *pValue );
int struct_NetS_Student_send( char **pptr, int *psize, SLK_NetS_Student *pValue );
int struct_NetS_StudentList_send( char **pptr, int *psize, SLK_NetS_StudentList *pValue );
int struct_NetS_TeacherShopItem_send( char **pptr, int *psize, SLK_NetS_TeacherShopItem *pValue );
int struct_NetS_TeacherShopList_send( char **pptr, int *psize, SLK_NetS_TeacherShopList *pValue );
int struct_NetS_Activity03Info_send( char **pptr, int *psize, SLK_NetS_Activity03Info *pValue );
int struct_NetS_Activity03List_send( char **pptr, int *psize, SLK_NetS_Activity03List *pValue );
int struct_NetS_Activity05Item_send( char **pptr, int *psize, SLK_NetS_Activity05Item *pValue );
int struct_NetS_Activity05List_send( char **pptr, int *psize, SLK_NetS_Activity05List *pValue );
int struct_NetS_Activity01List_send( char **pptr, int *psize, SLK_NetS_Activity01List *pValue );
int struct_NetS_Activity04Info_send( char **pptr, int *psize, SLK_NetS_Activity04Info *pValue );
int struct_NetS_Activity04List_send( char **pptr, int *psize, SLK_NetS_Activity04List *pValue );
int struct_NetS_Girl_send( char **pptr, int *psize, SLK_NetS_Girl *pValue );
int struct_NetS_GirlList_send( char **pptr, int *psize, SLK_NetS_GirlList *pValue );
int struct_NetS_GirlGet_send( char **pptr, int *psize, SLK_NetS_GirlGet *pValue );
int struct_NetS_GirlLove_send( char **pptr, int *psize, SLK_NetS_GirlLove *pValue );
int struct_NetS_GirlLoveResult_send( char **pptr, int *psize, SLK_NetS_GirlLoveResult *pValue );
int struct_NetS_FsInfo_send( char **pptr, int *psize, SLK_NetS_FsInfo *pValue );
int struct_NetS_FsVisit_send( char **pptr, int *psize, SLK_NetS_FsVisit *pValue );
int struct_NetS_FsVisitResult_send( char **pptr, int *psize, SLK_NetS_FsVisitResult *pValue );
int struct_NetS_FsPalace_send( char **pptr, int *psize, SLK_NetS_FsPalace *pValue );
int struct_NetS_FsPalaceAward_send( char **pptr, int *psize, SLK_NetS_FsPalaceAward *pValue );
int struct_NetS_FsPalaceResult_send( char **pptr, int *psize, SLK_NetS_FsPalaceResult *pValue );
int struct_NetS_GirlShopItem_send( char **pptr, int *psize, SLK_NetS_GirlShopItem *pValue );
int struct_NetS_GirlShop_send( char **pptr, int *psize, SLK_NetS_GirlShop *pValue );
int struct_NetS_ZoneMaster_send( char **pptr, int *psize, SLK_NetS_ZoneMaster *pValue );
int struct_NetS_ZoneMasterList_send( char **pptr, int *psize, SLK_NetS_ZoneMasterList *pValue );
int struct_NetS_DelZoneUnit_send( char **pptr, int *psize, SLK_NetS_DelZoneUnit *pValue );
int struct_NetS_FightHelper_send( char **pptr, int *psize, SLK_NetS_FightHelper *pValue );
int struct_NetS_FightHelperList_send( char **pptr, int *psize, SLK_NetS_FightHelperList *pValue );

#endif
