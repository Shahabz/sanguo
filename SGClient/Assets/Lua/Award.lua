--
AWARDKIND_ITEMBASE		=	0	  -- 道具（0+道具编号）
AWARDKIND_EQUIPBASE		=	10000 -- 装备（10000+装备编号）
AWARDKIND_HEROBASE		=	20000 -- 英雄（20000+英雄编号）
AWARDKIND_BUILDINGBASE	=	30000 -- 建筑（30000+建筑编号）
AWARDKIND_FUNCTION		=	40000 -- 功能（40000+功能编号）
AWARDKIND_VALUEBASE		=	50000 -- 数值
AWARDKIND_SILVER		=	50001 -- 银币
AWARDKIND_WOOD			=	50002 -- 木材
AWARDKIND_FOOD			=	50003 -- 粮食
AWARDKIND_IRON			=	50004 -- 镔铁
AWARDKIND_TOKEN			=	50005 -- 元宝
AWARDKIND_BODY			=	50006 -- 体力
AWARDKIND_INFANTRY		=	50007 -- 步兵
AWARDKIND_CAVALRY		=	50008 -- 骑兵
AWARDKIND_ARCHER		=	50009 -- 弓兵
AWARDKIND_EXP			=	50010 -- 角色经验
AWARDKIND_VIPEXP		=	50011 -- VIP经验
AWARDKIND_AUTOBUILD		=	50012 -- 自动建造次数
AWARDKIND_LEVYNUM		=	50013 -- 征收次数
AWARDKIND_PEOPLE		=	50014 -- 人口
AWARDKIND_PRESTIGE		=	50015 -- 威望值
AWARDKIND_FRIENDSHIP	=	50016 -- 友谊积分


-- 奖励形象
function AwardInfo( awardkind )
	local sprite = nil;
	local name = "";
	-- 掉落包
	if awardkind <= AWARDKIND_ITEMBASE then
		sprite = LoadSprite( "Char_Default" );
	
	-- 道具
	elseif awardkind <= AWARDKIND_EQUIPBASE then
		sprite = ItemSprite( awardkind );
		
	-- 装备
	elseif awardkind <= AWARDKIND_HEROBASE then
		sprite = EquipSprite( awardkind-AWARDKIND_EQUIPBASE ), T()
		
	-- 英雄	
	elseif awardkind <= AWARDKIND_BUILDINGBASE then
		sprite = HeroHeadSprite( awardkind-AWARDKIND_HEROBASE )
	
	-- 建筑	
	elseif awardkind <= AWARDKIND_FUNCTION then
		sprite = BuildingSprite( awardkind-AWARDKIND_BUILDINGBASE )
		
	elseif awardkind == AWARDKIND_SILVER then -- 银币
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_WOOD then	-- 木材
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_FOOD then	-- 粮食
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_IRON then	-- 镔铁
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_TOKEN then-- 元宝
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_BODY then	 -- 体力
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_INFANTRY then -- 步兵
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_CAVALRY then -- 骑兵
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_ARCHER then -- 弓兵
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_EXP then	-- 角色经验
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_VIPEXP then	-- VIP经验
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_AUTOBUILD	then -- 自动建造次数
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_LEVYNUM then	 -- 征收次数
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_PEOPLE then	-- 人口
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_PRESTIGE then	-- 威望值
		sprite = LoadSprite( "Char_Default" );
	elseif awardkind == AWARDKIND_FRIENDSHIP then -- 友谊积分
		sprite = LoadSprite( "Char_Default" );
	end
	
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite, name;
end

