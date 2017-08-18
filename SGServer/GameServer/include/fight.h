#ifndef _FIGHT_H_
#define _FIGHT_H_

#define FIGHT_HERO_MAX		256	// 战场双方英雄数量总和
#define FIGHT_TURNS_MAX		8192
#define FIGHT_INT			100
#define FIGHT_FLOAT			100.0f

#define FIGHT_WIN			1	
#define FIGHT_LOSE			2

#define FIGHT_ATTACK		1	
#define FIGHT_DEFENSE		2

#define FIGHTTYPE_STORY		1 // 副本战斗

// 一个战斗单元
typedef struct _fighthero
{
	char	type;				// 英雄所属，1部队，2固定防守对象
	int		index;				// 类型索引
	short	offset;				

	short	herokind;			// 英雄种类
	short	level;				// 等级
	char	color;				// 颜色
	char	corps;				// 兵种
	int		attack;				// 攻击
	int		defense;			// 防御
	int		hp;					// 当前血
	int		maxhp;				// 参战时血
	int		troops;				// 血上限
	short	attack_increase;	// 攻击增强
	short	defense_increase;	// 防御增强
	short	assault;			// 攻城
	short	defend;				// 守城
	char	line;				// 带兵排数
	char	skillid;			// 武将技

	char	line_left;			// 当前剩余排数
	int		line_hp;			// 当前排剩余兵力
	int		damage;				// 总伤害
}FightHero;

// 战斗结构
typedef struct _fight
{
	char	type;							// 战斗类型
	char	result;							// 战斗结果
	int		turns;							// 当前回合
	int		randspeed;						// 随机种子

	int		attack_armyindex;				// 攻击方部队索引，攻击方只能是部队
	char	defense_type;					// 防守方类别，1玩家城池，2部队 ...
	int		defense_index;					// 防守方索引，防守方类别

	FightHero attack_hero[FIGHT_HERO_MAX];	// 攻击方所有英雄
	FightHero defense_hero[FIGHT_HERO_MAX];	// 防御方所有英雄

	short attack_hero_num;					// 攻击方英雄数量
	short defense_hero_num;					// 防御方英雄数量

	short attack_hero_index;				// 攻击方当前出战英雄
	short defense_hero_index;				// 防御方当前出战英雄

	int attack_total_hp;
	int defense_total_hp;

	int attack_total_maxhp;
	int defense_total_maxhp;

	int attack_total_damage;
	int defense_total_damage;
}Fight;

// 向战场里添加一个英雄
int fight_add_hero( int pos, char type, int index, short herokind, short level, char color, char corps, int attack, int defense, int hp, int troops, short attack_increase, short defense_increase, short assault, short defend, char line, char skillid );

// 战斗启动
int fight_start( int attack_armyindex, char defense_type, int defense_index );

// 战斗启动-副本战斗
int fight_start_bystory( int actor_index, SLK_NetC_StoryBattle *pValue, int chapter, int rank );

// 战斗每一回合
int fight_oneturn();

// 使用技能
int fight_useskill( int pos, FightHero *pHero, FightHero *pTargetHero );

// 普通攻击
int fight_damage( int pos, FightHero *pHero, FightHero *pTargetHero );

// 减血
int fight_changehp( int pos, FightHero *pTargetHero, int damage );

// 下一个英雄
FightHero *fight_nextptr( int pos );

#endif

