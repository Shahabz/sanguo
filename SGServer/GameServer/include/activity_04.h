#ifndef _ACTIVITY_04_H
#define _ACTIVITY_04_H
#include "define.h"

#define ACTIVITY_SEVENDAY_TYPE1		1	//	累计充值{ 0 }元宝v
#define ACTIVITY_SEVENDAY_TYPE2		2	//	完成研究科技{ 0 } get
#define ACTIVITY_SEVENDAY_TYPE3		3	//	雇佣紫色研究员v
#define ACTIVITY_SEVENDAY_TYPE4		4	//	雇佣紫色内政官v
#define ACTIVITY_SEVENDAY_TYPE5		5	//	主公达到{ 0 }级 get
#define ACTIVITY_SEVENDAY_TYPE6		6	//	购买{ 0 }个特价礼包v
#define ACTIVITY_SEVENDAY_TYPE7		7	//	登入领取高顺 get
#define ACTIVITY_SEVENDAY_TYPE8		8	//	所在地图本国最多拥有{ 0 }座县城 get
#define ACTIVITY_SEVENDAY_TYPE9		9	//	所在地图本国最多拥有{ 0 }座郡城 get
#define ACTIVITY_SEVENDAY_TYPE10	10	//	参与攻下{ 0 }座营地 get
#define ACTIVITY_SEVENDAY_TYPE11	11	//	参与攻下{ 0 }座县城 get
#define ACTIVITY_SEVENDAY_TYPE12	12	//	参与攻下{ 0 }座郡城 get
#define ACTIVITY_SEVENDAY_TYPE13	13	//	击杀6 - 10级流寇{0}名v
#define ACTIVITY_SEVENDAY_TYPE14	14	//	主线副本通关第{ 0 }章 get
#define ACTIVITY_SEVENDAY_TYPE15	15	//	官府等级达到{ 0 }级 get
#define ACTIVITY_SEVENDAY_TYPE16	16	//	击飞5座任意等级的玩家城v
#define ACTIVITY_SEVENDAY_TYPE17	17	//	参与击飞4座至少8级的玩家城v
#define ACTIVITY_SEVENDAY_TYPE18	18	//	参与击飞3座至少10级的玩家城v
#define ACTIVITY_SEVENDAY_TYPE19	19	//	参与击飞2座至少11级的玩家城v
#define ACTIVITY_SEVENDAY_TYPE20	20	//	参与击飞1座至少12级的玩家城v
#define ACTIVITY_SEVENDAY_TYPE21	21	//	收集至少{ 0 }件绿色或以上品质装备v
#define ACTIVITY_SEVENDAY_TYPE22	22	//	收集至少{ 0 }件蓝色或以上品质装备v
#define ACTIVITY_SEVENDAY_TYPE23	23	//	收集{ 0 }件绿色或以上品质兵符v
#define ACTIVITY_SEVENDAY_TYPE24	24	//	收集{ 0 }件绿色或以上品质头盔v
#define ACTIVITY_SEVENDAY_TYPE25	25	//	收集{ 0 }件蓝色或以上品质头盔v
#define ACTIVITY_SEVENDAY_TYPE26	26	//	至少拥有{ 0 }名绿色或以上将v
#define ACTIVITY_SEVENDAY_TYPE27	27	//	至少拥有{ 0 }名蓝色或以上将v
#define ACTIVITY_SEVENDAY_TYPE28	28	//	至少拥有{ 0 }名紫色或以上将v
#define ACTIVITY_SEVENDAY_TYPE29	29	//	至少拥有{ 0 }名金色或以上将v
#define ACTIVITY_SEVENDAY_TYPE30	30	//	至少{ 0 }名武将总资质洗髓到上限v
#define ACTIVITY_SEVENDAY_TYPE31	31	//	至少{ 0 }名金色将总资质洗髓到上限v
#define ACTIVITY_SEVENDAY_TYPE32	32	//	{0}件蓝色或以上品质装备洗炼到满级v
#define ACTIVITY_SEVENDAY_TYPE33	33	//	{0}件紫色或以上品质装备洗炼到满级v
#define ACTIVITY_SEVENDAY_TYPE34	34	//	至少打造{ 0 }件国器v
#define ACTIVITY_SEVENDAY_TYPE35	35	//	至少{0}件国器20级 get
#define ACTIVITY_SEVENDAY_TYPE36	36	//	至少{0}件国器30级 get
#define ACTIVITY_SEVENDAY_TYPE37	37	//	至少{0}件国器40级 get
#define ACTIVITY_SEVENDAY_TYPE38	38	//	战力达到{ 0 } get
#define ACTIVITY_SEVENDAY_TYPE39	39	//	今日登入赠送 get
#define ACTIVITY_SEVENDAY_TYPE_MAXNUM 40
#define ACTIVITY_SEVENDAY_TIME		(7*86400)
int activity_04_addvalue( int city_index, int type, int value );
int activity_04_setvalue( int city_index, int type, int value );
int activity_04_addvalue_cityfight( int city_index, int targetlevel );
int activity_04_addvalue_equip( int city_index, int equiptype, int color );
int activity_04_addvalue_equipwash( int actor_index );
int activity_04_addvalue_hero( int actor_index );
int activity_04_addvalue_herowash( int actor_index );
int activity_04_getvalue( int actor_index, int type, int id );
int activity_04_sendinfo( int actor_index );
int activity_04_get( int actor_index, int id );
#endif
